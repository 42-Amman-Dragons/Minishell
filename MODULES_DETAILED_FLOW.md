# Minishell Modules: Detailed Functions, Behaviors, and Flow

This document explains the codebase module-by-module, with concrete function responsibilities and runtime flow.

## 1) High-Level Runtime Flow

For each input line, minishell follows this pipeline:

1. Input and shell loop (`src/Main`).
2. Tokenization (`src/Tokenizer`).
3. AST construction (`src/Parser`).
4. Heredoc pre-processing (`src/Heredoc`).
5. Expansion of args/redirections (`src/Expander` + `src/Expander_shared`, or bonus expander).
6. Execution (`src/Execution`), which may call builtins (`src/Builtins`) or `execve`.
7. Signal/exit status synchronization (`src/Signals`).
8. Cleanup and FD lifecycle (`src/utils`, `src/Main/shell_cleanup.c`, parser free logic).

The central orchestrator for this pipeline is `parse_and_execute()`.

## 2) Core Data Model (Public API in `include/minishell.h`)

### 2.1 Tokens

- `t_tokenType`: lexical classes (`WORD`, `PIPE`, `REDIRECT`, `AND`, `OR`, `OPEN_PAREN`, `CLOSE_PAREN`, `ASTERISK`).
- `t_redir_data`: redirection payload (`filename`, mode, heredoc options/fd).
- `t_token`: polymorphic token with `type` + union data.

### 2.2 AST

- `t_node_type`: execution nodes (`NODE_CMD`, `NODE_PIPE`, `NODE_AND`, `NODE_OR`, `NODE_SUBSHELL`).
- `t_tree`: AST node with union payload:
  - command (`args`, `redirections`),
  - binary operator (`left`, `right`),
  - subshell (`child`, `redirections`).

### 2.3 Shell State

`t_minishell` stores runtime context:

- raw line and prompt,
- mutable environment (`char **env`),
- in-memory history list,
- tracked open FDs list,
- terminal modes,
- interactive/child flags,
- global `exit_status`,
- prompt identity fields (`username`, `servername`),
- saved stdio for builtin redirection handling.

## 3) Main Module (`src/Main`)

### Role

Own process lifecycle, shell loop, prompt/history setup, interactive vs non-interactive behavior, and top-level parse/execute call.

### Key files and functions

- `main.c`
  - `main()`: initializes shell struct, mutable env, interactive setup, runs loop, saves history, final cleanup.
  - `runshell()`: repeatedly gets prompt and dispatches line processing.
  - `process_line_interactive()`: `readline`, history append, parse/execute.
- `initializers.c`
  - `init_shell()`: allocates/zeros shell, sets interactivity and default fields.
  - `init_terminal()`: captures and configures terminal attributes.
  - `init_interactive_shell()`: installs prompt-mode signals, loads history, initializes prompt.
- `non_interactive.c`
  - `process_line_non_interactive()`: reads from stdin using `get_next_line`, trims newline, executes.
  - `join_next_line()`: keeps consuming lines if quotes are unclosed.
- `parse_and_execute.c`
  - `parse_and_execute()`: calls tokenizer -> parser -> heredoc init -> executor.
  - `build_tree()`: builds token list and AST, maps parser errors to shell status.
  - `handle_heredoc()`: pre-opens heredocs and handles heredoc-related failures.
- `prompt.c` + `prompt_utils.c`
  - `get_prompt()`: builds colored prompt (`user@host:path$`) and handles cwd/home presentation.
  - `update_prompt_path()`: refreshes prompt after directory changes.
  - `consider_home_dir()`: rewrites absolute home path prefix to `~`.
- `history.c`
  - `add_to_history()`: adds to both readline history and linked-list history.
  - `load_history()`: reads `~/.minishell_history` (or fallback path).
  - `custom_save_history()`: writes list back to history file.
- `shell_cleanup.c`
  - `cleanup_shell()`: restores terminal attrs, clears history/env/line/prompt, closes tracked FDs, frees shell.
  - `free_env()`: releases mutable env array.

### Behavioral notes

- In non-interactive mode, syntax errors can terminate with code `2` from `parse_and_execute()`.
- Prompt is regenerated each loop in interactive mode.
- History persistence is explicit and linked-list-backed, not only readline-backed.

## 4) Tokenizer Module (`src/Tokenizer`)

### Role

Convert raw command text into a linear token stream with quote-aware words and operator tokens.

### Key files and functions

- `tokenizer.c`
  - `tokenizer()`: scans input, skips whitespace, repeatedly creates tokens.
- `tokenizer_factory.c`
  - `create_token()`: dispatches to specialized token constructors.
  - `free_token()`: frees token payloads safely.
- `tokenizer_utils.c`
  - `identify_token()`: classifies lexeme start (`&&`, `||`, `<<`, `>>`, `<`, `>`, `|`, parentheses, word).
  - `skip_whitespaces()`, `is_whitespace()`.
- `token_word.c`
  - `word_boundary()`: quote-aware boundary finder; emits unclosed-quote syntax error (`err=2`).
  - `extract_word()`, `create_word_token()`.
- `token_redirect.c`
  - `identify_redirection_mode()`: maps operators to redirection mode enum.
  - `create_redirect_token()`: initializes redirection token payload.
- `token_operators.c`
  - constructors for pipe, logical op, and parentheses.

### Behavioral notes

- Quote parsing is handled at token extraction time, so parser sees quote-preserved words.
- Unclosed quotes fail early during tokenization.
- `>` is interpreted as truncate-out mode (`DIR_OUT_TRUNC`).

## 5) Parser Module (`src/Parser`)

### Role

Build an AST with precedence-aware grammar for logical operators, pipelines, command/subshell nodes, and redirections.

### Grammar shape (implemented behavior)

1. `logic_expr`: pipeline sequences joined by `&&` / `||`.
2. `pipe_seq`: command/subshell nodes joined by `|`.
3. `cmd_or_sub`: either `(logic_expr)` subshell or simple command.
4. `simple_cmd`: sequence of words and redirections.

### Key files and functions

- `parser.c`
  - `build_ast()`: entry point, validates complete token consumption.
  - `parse_logic_expr()`: creates `NODE_AND` / `NODE_OR`.
  - `parse_pipe_seq()`: creates `NODE_PIPE`.
- `parse_command.c`
  - `parse_redir()`: parses redirect operator + word target.
  - `build_redir()`: copies redirection metadata into AST redir nodes.
  - `parse_subshell()`: parses parenthesized expression + trailing redirections.
  - `parse_cmd_or_sub()`.
- `parse_simple_cmd.c`
  - collects words/redirections in order,
  - converts word list into `args[]`,
  - creates command node even for redirection-only command forms.
- `node_factory.c`
  - node constructors (`create_oper_node`, `create_cmd_node`, `create_subshell_node`).
  - cursor helpers (`cur_type`, `advance`).
- `node_free.c`
  - `free_tree()`, `free_redir()`, `free_args()`, `syntax_error()`, `token_to_str()`.

### Behavioral notes

- Syntax errors map to status `2` and report the unexpected token or `newline`.
- Subshell nodes can hold both child AST and their own redirection list.
- Heredoc redirection nodes store quote-dependent expansion behavior (`heredoc_expand`).

## 6) Expander Modules (`src/Expander`, `src/Expander_shared`, bonus variant)

### Role

Apply shell expansions over command args and non-heredoc redirection targets.

### Mandatory expansion behavior

- Entry: `expander()` in `expander.c`.
- For command args:
  - expand each argument via `expand_one_arg()`,
  - perform split-on-space behavior when unquoted variable values contain spaces,
  - remove empty args created by unquoted expansion (`strip_empty_args`).
- For redirections:
  - expand filename for all redirections except heredoc delimiters.

### Key files and functions

- `expand_word.c`
  - state machine over `EXP_NORMAL`, `EXP_SQUOTE`, `EXP_DQUOTE`.
  - handles `$?`, digit cases (`$0` -> `minishell`), and quoted/unquoted rules.
  - `expand_word_heredoc()`: simpler mode used for heredoc content expansion.
- `expand_dollar.c` (shared)
  - `expand_dollar()`: variable name extraction and environment lookup.
  - `get_unquoted_var_val()`: helper for deciding split behavior in unquoted text.
- `expand_args.c` + shared args utils
  - `expand_one_arg()`: expansion + removal/splitting logic.
  - `add_to_args()` / `generate_expanded_list` behavior.
  - `word_has_quotes()`, `unquoted_dollar_has_space()`, `normalize_spaces()`.

### Bonus wildcard behavior (`src/Expander_bonus`)

- Bonus `expander()` adds wildcard (`*`) handling after variable expansion.
- `append_astersk()`: scans current directory and collects matching entries.
- `is_matching()`: recursive matcher for wildcard patterns.
- `handle_wild_redirect()`: special logic for redirection targets:
  - no match => keep literal,
  - single match => replace target,
  - multiple matches => mark ambiguous redirection.

### Behavioral notes

- Expansion happens at execution time (`exec_cmd`/`exec_subshell` path), not parse time.
- Quoted segments inhibit splitting and affect `$` interpretation.

### Dedicated Expansion Logic and Flow

This section focuses only on the expansion pipeline, from raw parsed `args` to final argv passed to builtin/`execve`.

#### A) Expansion Entry Points

- `exec_cmd()` and `exec_subshell()` call `expander()` before actual command execution.
- `expander()` walks each command node and processes:
  - command `args[]`,
  - redirection filenames (excluding heredoc limiters).

#### B) Argument Expansion Pipeline (Per Argument)

For each original argument, the mandatory flow is:

1. `expand_one_arg()` starts with the raw token text.
2. `expand_word()` scans character-by-character using state:
  - `EXP_NORMAL` (no quote context),
  - `EXP_SQUOTE` (single-quoted),
  - `EXP_DQUOTE` (double-quoted).
3. When `$` is found in expandable context, `expand_dollar()` is used:
  - `$VAR` -> env value,
  - `$?` -> last exit code,
  - `$0` -> `minishell` behavior in this project.
4. The expanded string is post-processed for splitting rules:
  - if unquoted variable expansion introduces spaces, argument may split,
  - if expansion becomes empty in unquoted context, entry may be removed.
5. Resulting pieces are appended to the rebuilt argv using shared arg utilities.

#### C) Quote-Sensitive Behavior

- Single quotes: content is literal; `$` is not expanded.
- Double quotes: `$` is expanded, but word-splitting is constrained compared to unquoted context.
- Unquoted context: full split/empty-removal behavior is applied after expansion.

#### D) Redirection Target Expansion

- Non-heredoc redirection filenames go through expansion like regular words.
- Heredoc limiter tokens are not expanded here.
- In bonus wildcard mode, redirect targets may become ambiguous if multiple filesystem matches are produced.

#### E) Bonus Wildcard Layer (After Variable Expansion)

In bonus mode, wildcard handling is applied after normal `$` expansion:

1. Detect candidate args containing `*`.
2. Enumerate directory entries.
3. Match with `is_matching()`.
4. Replace argument with matched list (or keep literal if no matches).
5. For redirect targets, detect and flag ambiguous expansion when multiple matches exist.

#### F) End-to-End Mini Example

Input args before expansion:

- `"$USER"`
- `$EMPTY`
- `$PATH_VAR_WITH_SPACES`
- `*.c` (bonus)

Output behavior:

- `"$USER"` -> single argument with substituted value.
- `$EMPTY` -> may disappear if unquoted and empty.
- `$PATH_VAR_WITH_SPACES` -> may split into multiple args if unquoted.
- `*.c` -> replaced with matching files in bonus mode, unchanged in mandatory mode.

This final expanded argv is what builtin dispatch or `execve` receives.

## 7) Heredoc Module (`src/Heredoc`)

### Role

Create and prepare heredoc input streams before command execution, with correct delimiter quote semantics and signal behavior.

### Key files and functions

- `heredoc.c`
  - `init_heredocs()`: traverses AST recursively, processes all heredoc redirections first.
  - `strip_quotes()`: removes quote chars from delimiter while preserving expansion policy.
  - `push_heredoc_line()`: writes line with optional variable expansion.
- `heredoc_io.c`
  - `setup_heredoc_fd()`: creates temporary file path and routes interactive/non-interactive handling.
  - interactive path forks child reader (`readline "> "`) and parent reopens temp file readonly.
- `heredoc_nonint.c`
  - `read_heredoc_nointeractive()`: consumes stdin lines until limiter for script mode.
- `heredoc_utils.c`
  - utility output and warnings (EOF before limiter, etc.).

### Behavioral notes

- Delimiter quoting controls whether heredoc body is expanded.
- Temporary heredoc files are unlinked after opening for reading.
- Interrupt during heredoc updates global signal status and abort path.

## 8) Execution Module (`src/Execution`)

### Role

Interpret AST nodes, manage forks/pipes/redirections, call builtins, run external programs, and propagate shell-compliant exit codes.

### Dispatch and node execution

- `execution.c`: `exec_tree()` dispatches by node type.
- `execute_oper.c`: `exec_and_or()` short-circuits using left result.
- `execute_pipe.c`: builds pipeline via two forks + `dup2`, waits and returns rightmost status.
- `execute_subshell.c`: forks child for subshell context and executes nested AST.

### Command execution path

- `execute_cmd.c`
  - `exec_cmd()`:
    1. expand args/redirections,
    2. if only redirections, apply/restore and return,
    3. builtin dispatch in parent,
    4. otherwise fork and run external.
  - builtin path uses `temp_redir()` + `restore_redir()` so parent stdio is preserved.
  - external path uses `execve_cmd()`.
- `exec_cmd_utils.c`
  - path resolution via `PATH` splitting (`absoulute_path()`).
- `exec_cmd_err.c`
  - user-facing error messages and exit code mapping (`126` vs `127`).

### Redirection handling

- `handle_redirect.c`
  - applies node redirection list in order.
- `handle_redirect_utils.c`
  - concrete file open/dup logic per mode (`<`, `>`, `>>`, heredoc fd).

### Process and FD utilities

- `exec_pipe_utils.c` / `exec_pipe_utils_2.c`
  - wait helpers, stdio save/restore, safe close helpers.
- `process_utils.c`
  - `child_exit_status()`, `free_and_exit()`.

### Behavioral notes

- Builtins execute without forking in simple command contexts.
- Pipelines and subshells isolate state through child processes.
- Exit status honors signal convention: `128 + signum`.

## 9) Builtins Module (`src/Builtins`)

### Role

Implement shell builtins and mutable environment operations.

### Dispatch and builtin set

- `builtin_dispatch.c`
  - `is_builtin()`, `call_builtin()`.
- Implemented builtins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`.

### Environment management

- `mutable_env.c`
  - `init_mutable_env()`: duplicates initial env and increments `SHLVL`.
  - `mk_env_entry()`.
- `env_crud.c`
  - `get_env_value()`, `set_env_value()`, `del_env_value()`, `add_env()`.

### Selected builtin behavior

- `cd.c`
  - supports no-arg/home and `~` prefixed input,
  - updates `OLDPWD` and `PWD`, refreshes prompt path.
- `export.c`
  - identifier validation,
  - assignment and declare-only modes,
  - prints sorted env when no args.
- `env.c`
  - prints only entries containing `=`.
- `exit.c`
  - numeric validation and overflow/boundary checks,
  - too-many-args error without exiting,
  - exits with parsed status when valid.

## 10) Signals Module (`src/Signals`)

### Role

Switch signal behavior based on shell phase and synchronize resulting status.

### Key files and functions

- `set_signal.c`
  - `set_signals_prompt()`: interactive prompt behavior (`SIGINT` handled, `SIGQUIT` ignored).
  - `set_signals_exec()`: parent ignores signals while children run.
  - `set_signals_child()`: children use default handlers.
  - `set_signals_heredoc()`: heredoc child-specific setup.
- `signals.c`
  - `handle_sigint()`: readline-aware prompt reset.
- `signal_status.c`
  - `handle_signal_status()`: maps `g_signum` to shell exit status.

### Behavioral notes

- Signal setup is deliberately phase-specific and frequently switched.
- Readline integration avoids broken prompt display after Ctrl-C.

## 11) Utility Modules (`src/utils`)

### `string_utils.c`

- `safe_join()`: concatenates and frees left side (ownership helper).
- `append_char()`, `append_str()`: incremental string builders.
- `normalize_spaces()`: tabs/newlines -> spaces for expansion splitting behavior.

### `fd_utils.c`

- `track_fd()`: registers fd pointers in `shell->openfiles`.
- `close_tracked_fds()`: closes all tracked descriptors and nullifies them.

These helpers are critical for avoiding FD leaks during heredoc, builtins redirection, and error paths.

## 12) Libft Module (`libft`)

### Role

Project-local standard utility layer used throughout modules:

- string/memory functions (`ft_*`),
- list API (`ft_lst*`),
- split/join/substr/itoa,
- `ft_printf`,
- `get_next_line` for non-interactive reads and history loading.

Behavior in minishell depends heavily on these helpers for allocation and linked-list ownership patterns.

## 13) Mandatory vs Bonus Build Composition

From `Makefile`:

- Mandatory build uses:
  - `src/Expander/*`
  - `src/Execution/execute_oper.c`
  - `src/Execution/execute_subshell.c`
- Bonus build swaps in:
  - `src/Expander_bonus/*`
  - `src/Execution/execute_oper_bonus.c`
  - `src/Execution/execute_subshell_bonus.c`

Shared modules remain the same (`Tokenizer`, `Parser`, `Main`, `Builtins`, `Heredoc`, `Signals`, `utils`, `Expander_shared`).

## 14) End-to-End Example Flow (Concrete)

For input:

`cat <<EOF | grep $USER && echo done`

Expected flow in this codebase:

1. `tokenizer()` emits WORD/REDIRECT/PIPE/WORD/AND tokens.
2. `build_ast()` creates a tree: `NODE_AND(left=NODE_PIPE(...), right=NODE_CMD(echo ...))`.
3. `init_heredocs()` pre-reads `<<EOF`, creates temp file, stores `heredoc_fd`.
4. `exec_tree()` on `NODE_AND`:
   - executes left pipe first,
   - in command execution, expander substitutes `$USER`,
   - pipe forks children and connects stdout/stdin.
5. If left returns `0`, right command (`echo done`) runs; otherwise short-circuit.
6. Exit code propagates through `shell->exit_status`.

## 15) Practical Ownership and Error-Handling Patterns

Common patterns used across modules:

- AST/list/string ownership transfer is explicit (e.g., `free_words_list()` after pointer transfer to args).
- Most allocation failures map to status `1`.
- Syntax/parse errors map to status `2`.
- Command-not-found/path errors map to `127`; non-executable or directory cases to `126`.
- `free_and_exit()` is used in child/error execution paths to guarantee cleanup.

## 16) Nontrivial Code Parts (Worth Extra Attention)

This section highlights macros and shell-specific system/library functions that are easy to misuse.

### 16.1 Core macros and constants

- Prompt styling macros in `include/minishell.h` (`DRAGON_GREEN`, `DRAGON_CYAN`, `RESET`, `PROMPT_EMOJI`) are wrapped with readline control markers (`\001` / `\002`).
- Why nontrivial: missing these wrappers breaks prompt cursor positioning and causes visual glitches while editing command lines.
- `PATH_MAX` and fixed-size buffers are used in prompt/cwd logic; changes here can introduce truncation or fallback-path bugs.

### 16.2 Readline/history API

- `readline`, `add_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `rl_clear_history` are central to interactive mode.
- Why nontrivial: these APIs must be coordinated with signal handlers so Ctrl-C redraws the prompt correctly and does not corrupt internal readline state.
- History load/save in `src/Main/history.c` uses shell-managed linked-list data plus readline’s own history state.

### 16.3 Terminal and signal functions

- `tcgetattr` / `tcsetattr` are used to snapshot and restore terminal settings.
- `sigaction`-based mode switching is used for prompt, execution, child, and heredoc phases.
- Why nontrivial: each phase requires different signal behavior; a wrong mode can cause parent interruption during child execution or broken heredoc cancel behavior.

### 16.4 Filesystem and metadata calls

- `open`, `close`, `read`, `write`, `unlink`, `access`, `getcwd`, `chdir`, `stat` are heavily used in redirects, heredoc, and path checks.
- Why nontrivial: ordering of `open`/`dup2`/`close` and error-path cleanup determines correctness and leak safety.
- `stat` is also used to detect command-is-directory cases for correct exit code mapping.

### 16.5 Process control primitives

- `fork`, `execve`, `waitpid`, `pipe`, `dup`, `dup2` implement command execution and pipelines.
- Why nontrivial: builtin commands must run in parent in some cases, while external commands and subshells run in children; mixing responsibilities causes state bugs.
- Exit-status propagation must preserve shell semantics (including signal-derived statuses).

### 16.6 Shell-specific tricky flows tied to these APIs

- Non-interactive multiline read join (`src/Main/non_interactive.c`) relies on repeated `get_next_line` + quote checks.
- Heredoc preprocessing (`src/Heredoc`) combines forked readers, temp files, and signal-sensitive cancellation.
- Expander split/remove behavior (`src/Expander*`) must preserve argument boundaries under quote rules.

---

If you want, I can also generate a second Markdown file that includes sequence diagrams (tokenization -> AST -> heredoc -> execution) for easier onboarding.