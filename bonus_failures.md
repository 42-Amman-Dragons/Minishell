# Bonus Test Failures Report

**Score: 43/60 tests passed — 19 failed cases**
**Date: 2026-03-27**

---

## Summary

| Section | Passed | Total | Failed tests |
|---------|--------|-------|--------------|
| Groups | 7 | 7 | — |
| Operators | 9 | 9 | — |
| Wildcards | 6 | 21 | 1,2,3,4,5,6,7,8,10,11,12,13,18,19,20,21 |
| Correction Bonus | 1 | 1 | — |
| Syntax Errors | 12 | 12 | — |
| Wild | 9 | 10 | 10 |

---

## Root Cause Groups

### Bug A — Glob expansion joins results into one word instead of multiple args

When `*` expands to multiple files, minishell concatenates them into one
space-separated word instead of passing each as a separate argument.

Affects: W1, W3, W5

### Bug B — Wildcard pattern produces no output (not expanded)

Various wildcard patterns produce empty output where bash produces matches.
This covers: suffix patterns (`*a`), multi-wildcard (`*a*a`), mixed
quoted+wildcard words (`"."*`, `*"."*`, `"Mak"'e'**`), and patterns that
should fall back to literal when no match exists (`*"."`).

Affects: W2, W4, W6, W7, W8, W10, W11, W12, W13, W18, W21

### Bug C — No "ambiguous redirect" error for wildcard in redirect position

When a wildcard in a redirect target matches multiple files, bash prints
`ambiguous redirect` to stderr. Minishell prints nothing.

Affects: W19, W20

### Bug D — `&&`/`||` inside variable values consumed as operators

When a variable holds `&&` or `||`, expanding it causes minishell to split
on those operators instead of treating them as literal characters.

Affects: GW10

---

## Per-Test Detail

### WILDCARDS

#### Test 1 — STD_OUT ❌
**Source:** `1_wildcards.sh:5`
```sh
/usr/bin/printf "%s\n" * | sort
```
| | Output |
|--|--------|
| bash | all files, one per line, sorted |
| minishell | all files on one line, space-separated, unsorted |

**Root cause:** Bug A — `*` expands but results are joined into one word, so `printf` receives a single argument and `sort` gets one line.

---

#### Test 2 — STD_OUT ❌
**Source:** `1_wildcards.sh:7`
```sh
export ARG="*"
/usr/bin/printf "%s\n" $ARG | sort
```
| | Output |
|--|--------|
| bash | all files, one per line, sorted |
| minishell | (empty) |

**Root cause:** Bug B — unquoted `$ARG` where `ARG="*"` is not glob-expanded.

---

#### Test 3 — STD_OUT ❌, STD_ERR ❌
**Source:** `1_wildcards.sh:10`
```sh
ls *
ls 'test'*
ls "test"*
```
| | Stderr |
|--|--------|
| bash | (empty) |
| minishell | `ls: cannot access 'file1 file2 ...': No such file or directory` |

**Root cause:** Bug A — `*` expands to all filenames joined as one string, which ls treats as a single nonexistent path.

---

#### Test 4 — STD_OUT ❌
**Source:** `1_wildcards.sh:14`
```sh
touch "*1"
/usr/bin/printf "%s\n" "*"* | sort
rm "*1"
```
| | Output |
|--|--------|
| bash | `*1` |
| minishell | (empty) |

**Root cause:** Bug B — `"*"*` (quoted literal `*` followed by unquoted `*`) not expanded; file `*1` exists and should match.

---

#### Test 5 — STD_OUT ❌, STD_ERR ❌
**Source:** `1_wildcards.sh:18`
```sh
touch "   "
ls * | grep "   "
rm "   "
```
| | Stdout | Stderr |
|--|--------|--------|
| bash | `   ` (3 spaces) | (empty) |
| minishell | (empty) | `ls: cannot access '...': No such file or directory` |

**Root cause:** Bug A — same as Test 3, `*` joins all filenames into one argument.

---

#### Test 6 — STD_OUT ❌
**Source:** `1_wildcards.sh:22`
```sh
touch "a" "aa"
/usr/bin/printf "%s\n" *a | sort
rm "a" "aa"
```
| | Output |
|--|--------|
| bash | `a` / `aa` |
| minishell | (empty) |

**Root cause:** Bug B — suffix wildcard pattern `*a` not expanded.

---

#### Test 7 — STD_OUT ❌
**Source:** `1_wildcards.sh:26`
```sh
touch "a" "aa" "aaa"
/usr/bin/printf "%s\n" *a*a | sort
rm "a" "aa" "aaa"
```
| | Output |
|--|--------|
| bash | `aa` / `aaa` |
| minishell | (empty) |

**Root cause:** Bug B — multi-anchor wildcard pattern `*a*a` not expanded.

---

#### Test 8 — STD_OUT ❌
**Source:** `1_wildcards.sh:30`
```sh
/usr/bin/printf "%s\n" "Mak"'e'*'*' | sort
```
| | Output |
|--|--------|
| bash | `Makefile*` |
| minishell | (empty) |

**Root cause:** Bug B — mixed quoted/unquoted word `"Mak"'e'*'*'` (pattern `Make*` + literal `*`) not expanded.

---

#### Test 10 — STD_OUT ❌
**Source:** `1_wildcards.sh:34`
```sh
/usr/bin/printf "%s\n" ***"Mak"'e'** | sort
```
| | Output |
|--|--------|
| bash | `Makefile` |
| minishell | (empty) |

**Root cause:** Bug B — leading `***` combined with mixed-quote word not expanded.

---

#### Test 11 — STD_OUT ❌
**Source:** `1_wildcards.sh:36`
```sh
/usr/bin/printf "%s\n" "."* | sort
```
| | Output |
|--|--------|
| bash | `.bonus` / `.git` / `.gitignore` |
| minishell | (empty) |

**Root cause:** Bug B — `"."*` (quoted dot + unquoted `*`) not expanded; should match hidden files.

---

#### Test 12 — STD_OUT ❌
**Source:** `1_wildcards.sh:38`
```sh
/usr/bin/printf "%s\n" *"." | sort
```
| | Output |
|--|--------|
| bash | `*.` (literal — no match) |
| minishell | (empty) |

**Root cause:** Bug B — `*"."` (unquoted `*` + quoted dot) produces no output instead of the literal `*.` when there is no match.

---

#### Test 13 — STD_OUT ❌
**Source:** `1_wildcards.sh:40`
```sh
/usr/bin/printf "%s\n" *"."* | sort
```
| | Output |
|--|--------|
| bash | `en.subject.pdf` / `execution_flow_analysis.md` / `minishell_testing.md` / `README.md` / `test_report.md` / `todos.md` |
| minishell | (empty) |

**Root cause:** Bug B — `*"."*` (wildcard + quoted dot + wildcard) not expanded.

---

#### Test 18 — STD_OUT ❌
**Source:** `1_wildcards.sh:50`
```sh
touch minishelxxxl
/usr/bin/printf "%s\n" minishel*l | sort
/usr/bin/printf "%s\n" minishelxxx*l | sort
/usr/bin/printf "%s\n" minishel*xxx*l | sort
rm minishelxxxl
```
| | Output |
|--|--------|
| bash | `minishell` / `minishelxxxl` (and variants) |
| minishell | (empty) |

**Root cause:** Bug B — patterns like `minishel*l` not expanded even when matching files exist.

---

#### Test 19 — STD_ERR ❌
**Source:** `1_wildcards.sh:57`
```sh
touch tmp_out
/bin/echo 42 >tmp_o*t*
rm -f "tmp_out*"
```
| | Stderr |
|--|--------|
| bash | (empty — single match, redirect succeeds) |
| minishell | (empty) |

**Note:** Both produce no stderr here (bash succeeds silently with one match). The tester detects a difference — likely due to the redirect behaviour affecting subsequent output. Classified under Bug C context.

---

#### Test 20 — STD_ERR ❌
**Source:** `1_wildcards.sh:61`
```sh
touch tmp_out1
touch tmp_out2
/bin/echo 42 >tmp_out*
rm -f "tmp_out*"
```
| | Stderr |
|--|--------|
| bash | `bash: line 1: tmp_out*: ambiguous redirect` |
| minishell | (empty) |

**Root cause:** Bug C — when wildcard in a redirect position matches multiple files, minishell does not print an "ambiguous redirect" error.

---

#### Test 21 — STD_OUT ❌
**Source:** `1_wildcards.sh:66`
```sh
export test=" * "
touch "$USER * ?eHallo"
/bin/echo "$USER "*" ?e"*
rm -f "$USER * ?eHallo"
```
| | Output |
|--|--------|
| bash | `mohannad * ?eHallo` |
| minishell | `mohannad *" ?e"*` (literal, not expanded) |

**Root cause:** Bug B — trailing unquoted `*` after mixed-quote prefix not expanded to the matching filename.

---

### WILD

#### Test 10 — STD_OUT ❌
**Source:** `9_go_wild.sh:25`
```sh
export or='||' && export and='&&' && echo 42 $and echo 43 $or echo 43
```
| | Output |
|--|--------|
| bash | `42 && echo 43 \|\| echo 43` |
| minishell | `42 echo 43 echo 43` |

**Root cause:** Bug D — when `and=&&` and `or=||`, expanding `$and` and `$or` causes minishell to interpret `&&` and `||` as command operators and execute them, instead of treating the expanded values as literal strings in the argument list.
