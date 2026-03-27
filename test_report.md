# Minishell Tester Report

**Date:** 2026-03-26

---

## Mandatory: 857/888 passed — 31 failed

---

### Variables `1_variables.sh` — 13 failures (tests 40–55)

IFS handling broken. When `$X` contains internal spaces, minishell preserves them as double-spaces instead of collapsing on word-split.

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 40 | `export X="  A  B  "` / `/bin/echo "1"$X'2'` | `1  A  B  2` | `1 A B 2` |
| 41–49 | Same pattern with different quoting/surrounding | double spaces preserved | single space |
| 50 | `export X="   "` / `/bin/echo "1"$X'2'` | `1   2` | `1 2` |
| 53 | `export X="  A  B  "` / `/bin/echo ?$X'2'` | `?  A  B  2` | `? A B 2` |
| 54 | `/bin/echo "1"$X?` | `1  A  B  ?` | `1 A B ?` |
| 55 | `/bin/echo $X'2'` | `  A  B  2` | `A B 2` |

**Root cause:** Unquoted `$X` must undergo word splitting — internal runs of IFS chars collapse to one separator and leading/trailing IFS chars are stripped.

---

### Parsing Hell `10_parsing_hell.sh` — 3 failures

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 30 | `export T="-n test1 -n test 2"` / `echo $T` | `-n test1 -n test 2` | `test1 -n test 2` |
| 77/78 | `export T="echo segfault \| grep segfault"` / `$T` | **Segfault (SIGSEGV)** | runs normally |
| 120 | `<\| echo ok` | exit code `2` | exit code `127` |

**Notes:**
- Test 30: when `$T` expands to a string starting with `-n`, `echo` should interpret it as a flag.
- Test 77/78: running a command stored in a variable that contains a pipe causes a crash.
- Test 120: syntax error exit code mismatch (`2` vs `127`).

---

### Redirections `1_redirs.sh` — 3 failures (tests 80, 81, 83)

Heredoc with a **quoted delimiter** — the body should be output verbatim (no quote stripping).

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 80 | `cat << lim` with body line ` "lim"` | ` lim` | ` "lim"` |
| 81 | `<<lim cat` with body line ` "lim"` | ` lim` | ` "lim"` |
| 83 | `cat << EOF` with body line `"EOF"` | `EOF` | `"EOF"` |

**Root cause:** Minishell is stripping double quotes from heredoc body content instead of outputting them literally.

---

### Correction `2_correction.sh` — 4 failures

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 17 | `echo -n -n -n 42` / `exit` | outputs `exit` as text | nothing (exit is a builtin) |
| 106 | `unset PATH` / `cd /bin` / `ls` | exit code `127`, empty output | exit code `0`, lists `/bin` |
| 107 | `unset PATH` / `cd /bin/../bin/` / `ls` | exit code `127`, empty output | exit code `0`, lists `/bin` |
| 109 | `unset PATH` / `cd /bin/../bin/.../` / `../bin/../bin/ls` | exit code `127` + STDERR diff | exit code `0` |

**Notes:**
- Test 17: `exit` appearing in stdout suggests minishell is echoing the word "exit" rather than exiting.
- Tests 106/107/109: after `unset PATH`, relative/absolute binary execution without PATH fails — minishell should still be able to exec `/bin/ls` when given directly or via `cd` + relative path.

---

### Builtins `1_builtins.sh` — 2 failures

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 132 | `export` (prints all env) | `SHLVL="3"` / `_=".//minishell"` / unescaped JSON in `VSCODE_NLS_CONFIG` | `SHLVL="4"` / no `_` line / escaped JSON |
| 235 | `exit` (within test harness session) | outputs `exit` as text | silent |

**Notes:**
- Test 132: three sub-issues: SHLVL incremented one too few times; `_` variable should not be exported; embedded double quotes in env values must be backslash-escaped in `export` output.
- Test 235: same issue as correction test 17 — `exit` echoed to stdout.

---

### Pipelines `1_pipelines.sh` — 1 failure

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 27 | `ls \| cat << stop \| ls -la \| cat << stop1 \| ls \| cat << stop2 \| ls -la > > out \| cat << stop3` | exit code `0` | exit code `2` |

**Root cause:** `> >` (two separate redirects) is a syntax error in bash (exit 2), but minishell returns 0.

---

### Path Check `2_path_check.sh` — 1 failure

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 10 | `touch tmp_x_file1` / `tmp_x_file1` / `echo $?` / `./tmp_x_file1` | lists tester directory files in stdout | only echo output |

**Root cause:** minishell likely runs `ls` or similar when attempting to execute `tmp_x_file1`, or CWD is wrong.

---

### Simple Commands `1_scmds.sh` — 1 failure

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 7 | `touch "\n"` / `/bin/rm -f "\n"` | STDERR differs | no stderr |

---

### Go Wild `9_go_wild.sh` — 2 failures

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 4 | `/bin/echo -e "\033[33mHello, world!\033[0m"` | output mismatch | correct ANSI escape output |
| 10 | `export test="arg1\targ2"` / `bash tmp_test_sh $test` | `arg1 arg2` (on one line) | `arg1` then `arg2` on separate lines |

**Note:** Test 10: tab in `$test` should split into two separate arguments when unquoted (`$1`=`arg1`, `$2`=`arg2`).

---

---

## Bonus: 43/60 passed — 17 failed

All failures are in wildcards.

---

### Wildcards `1_wildcards.sh` — 16 failures

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 1 | `/usr/bin/printf "%s\n" * \| sort` | **Abort / core dump (exit 134)** | lists all files sorted |
| 2 | `export ARG="*"` / `/usr/bin/printf "%s\n" $ARG \| sort` | lists all files (wrong — should expand `$ARG` as literal `*`) | `*` |
| 3 | `ls *` | **Abort / core dump (exit 134)** | lists all files |
| 4 | `/usr/bin/printf "%s\n" "*"*` after `touch "*1"` | lists all files | only `*1` |
| 5 | `touch "   "` / `ls * \| grep "   "` | **Abort / core dump (exit 134)** | finds file with spaces |
| 6 | `touch a aa` / `/usr/bin/printf "%s\n" *a \| sort` | **Abort / core dump (exit 134)** | `a`, `aa` |
| 7 | `touch a aa aaa` / `/usr/bin/printf "%s\n" *a*a \| sort` | **Abort / core dump (exit 134)** | `aa`, `aaa` |
| 8 | `/usr/bin/printf "%s\n" "Mak"'e'*'*' \| sort` | `Make*'*'` (quotes not stripped) | `Make**` |
| 10 | `/usr/bin/printf "%s\n" ***"Mak"'e'** \| sort` | `***"Mak"'e'**` | `***Make**` |
| 11 | `/usr/bin/printf "%s\n" "."* \| sort` | `.git . ..` | `.git` |
| 12 | `/usr/bin/printf "%s\n" *"." \| sort` | `*"."` (literal) | `*.` |
| 13 | `/usr/bin/printf "%s\n" *"."* \| sort` | `*"."*` (literal) | expands to all files containing `.` |
| 18 | `touch minishelxxxl` / multiple `printf "%s\n" minishe*l` variants | **Abort / core dump (exit 134)** | correct matches |
| 19 | `/bin/echo 42 >tmp_o*t*` (redirect with wildcard, no match) | STDERR differs | no stderr |
| 20 | `touch tmp_out1 tmp_out2` / `/bin/echo 42 >tmp_out*` | STDERR differs | no stderr |
| 21 | `touch "$USER * ?eHallo"` / `/bin/echo "$USER "*" ?e"*` | `mohannad * ?eHallo` | `mohannad * ?eHallo` (with literal `" ?e"` section) |

**Root causes:**
- **Crashes (tests 1,3,5,6,7,18):** wildcard expansion segfaults/aborts — likely NULL deref or bad memory in the glob/expansion code.
- **Wrong expansion scope (test 2):** `$ARG` where `ARG="*"` — the `*` inside the variable should NOT be glob-expanded (variable assignment quoting not respected post-expansion).
- **Quoted literal in pattern (tests 8,10,12,13):** quoted parts within wildcard patterns should be treated as literal characters, not included in the glob with their quotes.
- **Hidden files (test 11):** `.*` should not match `.` and `..`.

---

### Go Wild bonus `9_go_wild.sh` — 1 failure

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 10 | `export or='\|\|'` / `export and='&&'` / `echo 42 $and echo 43 $or echo 43` | **Segfault (exit 139)** | `42 && echo 43 \|\| echo 43` (printed literally) |

**Root cause:** Expanding operator-like values from variables inside a command causes a segfault — likely a parser/executor crash when `&&` or `||` tokens appear from variable expansion.

---

---

---

## Tester 2 (LucasKuhn): 96/146 passed — 50 failed

---

### Builtins — 9 failures

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 15 | `echo "exit_code ->$? user ->$USER home -> $HOME"` | *(empty output)* | `exit_code ->0 user ->mohannad home -> /home/mohannad` |
| 16 | `echo 'exit_code ->$? user ->$USER home -> $HOME'` | *(empty output)* | `exit_code ->$? user ->$USER home -> $HOME` |
| 26 | `export A-` | exit `0` | exit `1` |
| 30 | `export HELLO-=123` | exit `0` | exit `1` |
| 31 | `export =` | exit `0` | exit `1` |
| 32 | `export 123` | exit `0` | exit `1` |
| 40 | `cd $PWD hi` | exit `0` | exit `1` |
| 41 | `cd 123123` | exit `0` | exit `1` |
| 51 | `exit 42 world` | exit `0` | exit `1` |

**Root causes:**
- Tests 15/16: echo produces no output — likely a session/stdin issue with `$?` expansion.
- Tests 26/30/31/32: `export` with invalid identifiers must return exit code 1 and print an error.
- Tests 40/41: `cd` with too many args or non-existent dir must return exit code 1.
- Test 51: `exit` with multiple args must return exit code 1 without exiting.

---

### Pipes — 1 failure

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 55 | `cat minishell.h \| grep ");"$` | exit `0` | exit `1` |

`minishell.h` doesn't exist in the tester's working directory — `cat` should fail with exit `1`.

---

### Redirects — 22 failures

All redirect failures are **exit code 0 instead of 1** when a redirect fails (permission denied or missing file).

| Tests | Pattern | Example | Expected exit |
|-------|---------|---------|---------------|
| 58 | Invalid filename as redirect arg | `grep hi "<infile" < ./test_files/infile` | `2` (syntax error) |
| 98–108 | Write to `invalid_permission` file or read missing file | `echo bye >./test_files/invalid_permission` | `1` |
| 115–120 | Append to `invalid_permission` file | `ls >>./test_files/invalid_permission` | `1` |
| 128–129 | Piped command with invalid redirect target | `echo hi \| echo bye >>./test_files/invalid_permission` | `1` |
| 130 | No space between redirect and filename | `cat <minishell.h>./outfiles/outfile` | `1` |

**Root cause:** When a redirection fails (open returns -1), minishell exits with `0` instead of `1`. The child process exit code from a failed `open()` is not being propagated.

---

### Extras — 14 failures

All extras failures are **wrong exit code (0 instead of 126/127)** when executing invalid/non-existent files, plus two behavioral bugs.

| # | Command | Mini exit | Bash exit | Notes |
|---|---------|-----------|-----------|-------|
| 133 | `$PWD` | `0` | `126` | Expanding to a directory path — should be "is a directory" |
| 135 | `$EMPTY echo hi` | `139` (SIGSEGV) | `0` | When `$EMPTY` is empty, rest of line should execute; minishell crashes |
| 136 | `./test_files/invalid_permission` | `0` | `126` | Wrong error: "command not found" instead of "permission denied" |
| 137 | `./missing.out` | `0` | `127` | Wrong error: "command not found" instead of "no such file" |
| 138 | `missing.out` | `0` | `127` | Non-existent command must exit `127` |
| 139 | `"aaa"` | `0` | `127` | Quoted non-existent command must exit `127` |
| 140 | `test_files` | `0` | `127` | Non-existent command (no `./`) must exit `127` |
| 141 | `./test_files` | `0` | `126` | Directory as command must exit `126` |
| 142 | `/test_files` | `0` | `127` | Wrong error: "command not found" instead of "no such file" |
| 143 | `minishell.h` | `0` | `127` | Non-existent command must exit `127` |
| 144 | `$` | `0` | `127` | `$` alone with no var name must exit `127` |
| 145 | `$?` | `0` | `127` | `$?` expands to `0`, then tries to exec `0` — must exit `127` |
| 146 | `README.md` | `0` | `127` | Non-existent command must exit `127` |

**Root cause:** minishell always exits `0` after failed `execve()` instead of setting:
- `126` for permission denied or is-a-directory
- `127` for command not found / no such file

---

## Priority Fix List

| Priority | Area | Failures | Root Cause |
|----------|------|----------|------------|
| 1 | **Exit codes for failed commands** | 36 (tester 2) | `execve` failure / redirect failure always returns 0 instead of 126/127/1 |
| 2 | **Wildcard segfaults** | 6 bonus crashes | NULL deref / bad memory in glob code |
| 3 | **`$EMPTY echo hi` segfault** | 1 tester 2 | Empty variable expansion at start of command crashes executor |
| 4 | **IFS / word splitting** | 13 mandatory | Spaces not collapsed on unquoted `$VAR` expansion |
| 5 | **Heredoc body quote stripping** | 3 mandatory | `"` removed from heredoc body output |
| 6 | **`export` invalid identifier exit code** | 4 tester 2 | `export A-` / `export 123` should exit 1 with error |
| 7 | **`cd` / `exit` argument errors** | 3 tester 2 | Wrong number/type of args should exit 1 |
| 8 | **`exit` printed to stdout** | 2 mandatory | `exit` builtin leaking word to stdout |
| 9 | **`unset PATH` + exec** | 3 mandatory | Can't exec binaries after PATH is unset |
| 10 | **Wildcard quoting in patterns** | 7 bonus | Quoted chars inside glob not treated as literals |
| 11 | **`$VAR` with `*` not re-globbed** | 1 bonus | Post-expansion glob applied to variable value |
| 12 | **`.*` matches `.` and `..`** | 1 bonus | Hidden file pattern should exclude `.` and `..` |
| 13 | **Operator in variable segfault** | 1 bonus | `&&`/`\|\|` from `$VAR` expansion crashes executor |
| 14 | **Exit code mismatches** | 3 mandatory | `> >` syntax error, pipe exit code |
