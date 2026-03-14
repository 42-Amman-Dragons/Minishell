# Minishell Cleanup Plan — Norm Compliance & Code Quality

## Current State Summary

- **43 source files**, **2 headers**
- **18 functions** exceed the 25-line Norm limit (worst: `exec_cmd()` at 62 lines)
- **4 files** have more than 5 functions
- **6 bonus `_bonus.c` files** duplicate their non-bonus counterparts with minor diffs
- Empty union structs (`t_pipe_data`, `t_and_data`, `t_or_data`) serve no purpose
- Header inconsistency: `expand_one_arg()` has different signatures in each header
- Missing function definitions: `contains()`, `cal_len()` (called but never defined)

---

## Phase 1: Fix Critical Issues

### 1.1 — Add missing function definitions
- [ ] Define `int contains(char *str, char c)` — called in `expand_word_bonus.c` and `expander_utils_bonus.c`
- [ ] Define `int cal_len(char *str, char c)` — called in `expander_utils_bonus.c`
- [ ] Place them in `expand_utils.c` or a new `expand_astersk_utils.c`

### 1.2 — Fix header inconsistency
- [ ] `expand_one_arg()` returns `void` in `minishell.h` but `char **` in `minishell_bonus.h`
- [ ] Ensure the prototype matches the actual implementation in each build target

---

## Phase 2: Norm — Functions Over 25 Lines

Each function below must be split into smaller helpers (max 25 lines each).

| File | Function | Lines | Action |
|------|----------|-------|--------|
| `Execution/execute_cmd.c` | `exec_cmd()` | 62 | Split into 3 helpers (resolve path, fork/exec, handle builtin) |
| `Execution/handle_redirections.c` | `handle_redirections()` | 33 | Extract loop body into a `dispatch_redir()` helper |
| `Execution/execute_pipe.c` | `exec_pipe()` | 32 | Extract pipe setup and wait logic |
| `Execution/execute_cmd.c` | `execve_cmd()` | 30 | Move error messaging to helper |
| `Execution/execute_cmd.c` | `absoulute_path()` | 28 | Extract path-search loop |
| `Expander/heredoc.c` | `setup_heredoc_fd()` | 31 | Split fork logic from fd setup |
| `Expander/heredoc.c` | `strip_quotes()` | 27 | Simplify or extract char-copy loop |
| `Expander/expand_astersk.c` | `append_astersk()` | 30 | Extract directory-read loop |
| `Expander/expander_utils_bonus.c` | `generate_expanded_list()` | 29 | Extract array copy logic |
| `Main/main.c` | `runshell()` | 30 | Extract readline + parse loop body |
| `Main/main.c` | `main()` | 29 | Move init steps to helper |
| `history.c` | `load_history()` | 32 | Extract file-read loop |
| `Builtins/builtin_dispatch.c` | `ft_exit()` | 27 | Extract validation logic |
| `Builtins/export.c` | `export_one()` | 27 | Extract identifier parsing |
| `Builtins/cd.c` | `add_to_end()` | 26 | Extract array realloc logic |
| `Parser/parse_command.c` | `parse_subshell()` | 26 | Extract redir-parsing tail |
| `Parser/parser.c` | `parse_logic_expr()` | 26 | Extract operator-handling loop |

---

## Phase 3: Norm — Files With >5 Functions

| File | Count | Action |
|------|-------|--------|
| `Execution/execute_cmd.c` | 6 | Move `safe_join()` and `free_splitted()` to `execution_utils.c` |
| `Expander/heredoc.c` | 6 | Move `strip_quotes()` to `expand_utils.c` |
| `Expander/expander_utils_bonus.c` | 6 | Move `add_splitted()` to a new file or inline it |
| `Tokenizer/tokenizer_factory.c` | 6 | Move one token creator (e.g. `create_background_token`) to `tokenizer_utils.c` |

---

## Phase 4: Remove Dead Code & Redundancy

### 4.1 — Remove empty union structs
- [ ] Delete `t_pipe_data`, `t_and_data`, `t_or_data` from both headers (they have no fields)
- [ ] Update `t_node_data` union accordingly — if pipe/and/or nodes carry no extra data, they only need `left`/`right` from `t_tree`

### 4.2 — Merge or deduplicate bonus files
The bonus files are nearly identical to their non-bonus counterparts. Strategy:

| Bonus File | Non-Bonus File | Diff |
|-----------|---------------|------|
| `expand_word_bonus.c` | `expand_word.c` | +5 lines for wildcard in `handle_normal()` |
| `expander_bonus.c` | `expander.c` | +4 lines for error check + arg expansion |
| `expander_utils_bonus.c` | `expander_utils.c` | Different return type + 3 extra functions |

**Options:**
- **Option A (recommended):** Use `#ifdef BONUS` preprocessor guards to keep one file per module
- **Option B:** Keep separate files but make sure only the correct set is compiled via Makefile rules (current approach — just ensure it's clean)

### 4.3 — Clean up unused / misplaced code
- [ ] `token_to_str()` and `syntax_error()` in `node_free.c` are parser utilities, not free functions — move to `parse_command.c` or a new `parser_utils.c`
- [ ] Remove the `@TODO` comment in `expander_bonus.c:29` — either implement it or delete it
- [ ] Fix typo: `absoulute_path` → `absolute_path` (rename function + all call sites)
- [ ] Fix typo: `creat_token` → `create_token`
- [ ] Fix typo: `expand_astersk` → `expand_asterisk` (filename + references)
- [ ] Fix typo: `prepare_prompt_beggining` → `prepare_prompt_beginning`

---

## Phase 5: Header Cleanup

### 5.1 — minishell.h / minishell_bonus.h
- [ ] Remove prototypes for empty-struct constructors if structs are deleted
- [ ] Group prototypes by module with clear section comments:
  ```
  /* ---- Builtins ---- */
  /* ---- Execution ---- */
  /* ---- Expander ---- */
  /* ---- Parser ---- */
  /* ---- Tokenizer ---- */
  /* ---- Signals ---- */
  /* ---- Utils ---- */
  ```
- [ ] Remove any includes not actually needed (audit each `#include`)
- [ ] Fix style: remove extra space before `;` on union field (line ~91)

### 5.2 — Reduce header duplication
- [ ] Factor shared content into a `minishell_common.h` or use `#ifdef BONUS` in a single header

---

## Phase 6: Minor Style Fixes

- [ ] Ensure all files have the 42 header comment
- [ ] Ensure no trailing whitespace
- [ ] Ensure blank line between function definitions
- [ ] Ensure variable declarations are at top of each function (no mid-block declarations)
- [ ] Run `norminette` on every file and fix remaining warnings

---

## Execution Order

1. **Phase 1** first — fixes build-breaking issues
2. **Phase 4.3** next — typos and misplaced code (small, safe changes)
3. **Phase 2** — split long functions (biggest Norm impact)
4. **Phase 3** — redistribute functions across files
5. **Phase 4.1 & 4.2** — structural cleanup
6. **Phase 5** — header cleanup
7. **Phase 6** — final style pass + `norminette` verification

> **Tip:** After each phase, run `make` and test basic commands (`echo`, `ls | grep`, `cd`, pipes, redirections, heredoc) to catch regressions early.
