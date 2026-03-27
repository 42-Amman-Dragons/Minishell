# Minishell Tester Report

**Date:** 2026-03-27
**Branch:** execution-cleanup

---

## Mandatory: 848/888 passed — 5 failing tests

---

### Simple Commands `1_scmds.sh` — 1 failure

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 7 | `touch "\n"` / `/bin/rm -f "\n"` | STD_ERR differs | no stderr |

**Root cause:** Non-interactive mode rejects multi-line quoted strings (literal newline inside quotes triggers "unclosed quote"). Both builds.

---

### Correction `2_correction.sh` — 3 failures

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 106 | `unset PATH` / `cd /bin` / `ls` | exit 127, no output | exit 0, lists `/bin` |
| 107 | `unset PATH` / `cd /bin/../bin/` / `ls` | exit 127, no output | exit 0, lists `/bin` |
| 109 | `unset PATH` / `cd /bin/` / `sleep 2` | exit 127, STD_ERR diff | exit 0 |

**Root cause:** After `unset PATH`, minishell cannot exec binaries that exist in the current directory. Bash falls back to searching cwd for commands like `ls` and `sleep` when their full path is resolvable via cwd. Both builds.

---

### Path Check `2_path_check.sh` — 1 failure

| # | Command | Minishell | Bash |
|---|---------|-----------|------|
| 10 | `touch tmp_x_file1; chmod +x tmp_x_file1; ./tmp_x_file1` | STD_OUT differs | exit 0, no output |

**Root cause:** `execve` returns `ENOEXEC` for an empty executable file. Bash retries with `/bin/sh` as interpreter; minishell prints "Exec format error" (exit 126). Both builds.

---

---

## Bonus: 42/60 passed — 18 failing tests

---

### Wildcards `1_wildcards.sh` — 16 failures

| # | Source line | Checks | Root cause |
|---|-------------|--------|------------|
| 1 | `:5` — `/usr/bin/printf "%s\n" * \| sort` | STD_OUT ❌ | Glob result not split into separate args (is_wc not set for `*`) |
| 2 | `:7` — `export ARG="*"; printf $ARG` | STD_OUT ❌ | `$VAR` expanding to `*` not re-globbed |
| 3 | `:10` — `ls *; ls 'test'*; ls "test"*` | STD_OUT ❌ STD_ERR ❌ EXIT ❌ (minishell=2 bash=0) | Tokenizer/pattern issue with `'test'*` or `"test"*` → syntax error |
| 4 | `:14` — `"*"*` after `touch "*1"` | STD_OUT ❌ | Quoted `*` prefix not treated as literal in glob pattern |
| 5 | `:18` — `ls * \| grep "   "` | STD_OUT ❌ STD_ERR ❌ | Space in filename lost — `ft_split` on `' '` destroys space-only filenames |
| 6 | `:22` — `*a` suffix pattern | STD_OUT ❌ | Suffix wildcard pattern not expanded |
| 7 | `:26` — `*a*a` multi-anchor | STD_OUT ❌ | Multi-anchor wildcard not expanded |
| 9 | `:32` — `"Mak"'e'**` | STD_OUT ❌ | Pattern with consecutive `**` not matching correctly |
| 10 | `:34` — `***"Mak"'e'**` | STD_OUT ❌ | Leading `***` combined with mixed-quote word not expanded |
| 11 | `:36` — `"."*` | STD_OUT ❌ | `.*` includes `.` and `..` (not filtered) |
| 13 | `:40` — `*"."*` | STD_OUT ❌ | Quoted `.` in middle of pattern not treated as literal |
| 17 | `:48` — `minishe*l` (exact suffix match) | STD_OUT ❌ | Pattern with non-`*` suffix not matching |
| 18 | `:50` — `minishelxxxl` variants | STD_OUT ❌ | Suffix-anchored patterns not matching |
| 19 | `:57` — redirect `>tmp_o*t*` (single match) | STD_ERR ❌ | Redirect wildcard expansion produces unexpected stderr |
| 20 | `:61` — redirect `>tmp_out*` (multiple matches) | STD_ERR ❌ | No "ambiguous redirect" error printed |
| 21 | `:66` — `"$USER "*" ?e"*` | STD_OUT ❌ | Trailing unquoted `*` after mixed-quote prefix not expanded |

**Passing:** W8 (`"Mak"'e'*'*'` → `Make**`) ✅, W12 (`*"."` → `*.`) ✅, W14 (`"*"`) ✅, W15 (`'*'`) ✅, W16 (`Minishe*l`) ✅

---

### Wild `9_go_wild.sh` — 2 failures

| # | Source line | Checks | Root cause |
|---|-------------|--------|------------|
| 5 | `:15` | STD_ERR ❌ | Unexpected stderr output |
| 10 | `:25` — `export and='&&'; echo 42 $and echo 43` | STD_OUT ❌ | `&&`/`\|\|` values from variable expansion consumed as operators instead of literal args |

---

## Root cause summary

| Bug | Tests affected | Status |
|-----|---------------|--------|
| `is_wc` not set for `*` — glob result not split | W1, W6, W7, W17, W18 | Fixed by `word_has_unquoted_asterisk` in this session |
| `$VAR` expanding to `*` not re-globbed | W2 | Open |
| Quoted `*` not treated as literal in pattern | W4, W13 | Open |
| Space in filename destroyed by `ft_split` | W5 | Open (hard — requires separator change) |
| `.*` matches `.` and `..` | W11 | Open |
| Redirect wildcard ambiguous error missing | W19, W20 | Open |
| `&&`/`\|\|` from variable expansion consumed as operators | WILD10 | Open |
| Suffix/anchor pattern matching (`*a`, `*a*a`, `minishe*l`) | W6, W7, W17, W18 | Open — may be is_wc or expand_word issue |
| Mixed-quote word with wildcard (test 3 syntax error) | W3 | Open — regression (was crash, now syntax error) |
