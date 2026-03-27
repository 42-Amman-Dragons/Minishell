# Minishell Test Cases

> ⚠️ **IMPORTANT NOTE!** The quotes in Notion may be formatted differently. Replace them with regular `'` or `"` quotes when copying test cases.

Run with valgrind:
```bash
valgrind --trace-children=yes --track-fds=yes ./minishell
```

Check for zombie processes:
```bash
ps aux | grep Z
```

---

## Eval Sheet

| Cases | Expected Output | $? | Done? |
|---|---|---|---|
| /bin/ls | list directory contents | 0 | ✅ |
| /bin/ls filethatdoesntexist | ls: cannot access 'filethatdoesntexist': No such file or directory | 2 | ✅ |
| Empty command (just press enter) | nothing happens, prompt returns | 0 | ✅ |
| Only spaces/tabs | nothing happens, prompt returns | 0 | ✅ |
| echo hello world | hello world | 0 | ✅ |
| echo -n hello | hello (no newline) | 0 | ✅ |
| exit | shell exits | 0 | ✅ |
| exit 42 | shell exits with code 42 | 42 | ✅ |
| expr $? + $? | arithmetic result | 1 | ✅ |
| Ctrl+C in empty prompt | new prompt on new line | 130 | ⚠️ manual |
| Ctrl+\ in empty prompt | nothing | 0 | ⚠️ manual |
| Ctrl+D in empty prompt | shell exits | 0 | ⚠️ manual |
| echo '$USER' | $USER | 0 | ✅ |
| env | prints environment | 0 | ✅ |
| pwd | prints current dir | 0 | ✅ |
| ./a.out (relative path) | executes binary | depends | ✅ (127 when not found) |
| ls (with PATH set) | lists directory | 0 | ✅ |
| unset PATH ; ls | ls: command not found | 127 | ✅ |
| PATH=/bin:/usr/bin ; ls | works again | 0 | ❌ BROKEN — standalone VAR=val not supported (see todos) |
| cat < in.txt > out.txt | out.txt contains contents of in.txt | 0 | ✅ |
| echo hello >> out.txt | appends "hello" | 0 | ✅ |
| cat << EOF + input + EOF | heredoc works | 0 | ✅ |
| `cat file \| grep foo \| wc -l` | line count | 0 | ✅ |
| `ls filethatdoesntexist \| grep foo` | error from ls, pipe runs | 1 | ✅ |
| `cat file \| grep foo > out.txt` | out.txt contains grep result | 0 | ✅ |
| Type random text abcd123 | minishell: abcd123: command not found | 127 | ✅ |
| Very long command | handled gracefully (no crash) | depends | ✅ (tested 10000 chars) |
| echo $USER | prints user value | 0 | ✅ |
| echo "$USER" | prints user value | 0 | ✅ |

---

## Custom Tests

| Cases | Expected Output | $? | Required | Done |
|---|---|---|---|---|
| echo $PWD"$USER"'$HOME' | /home/name/minishellname$HOME | 0 | BONUS | ✅ |
| cat << ls > out | heredoc then cat out shows content | 0 | YES | ✅ |
| ls <<1 \| cat -e \| cat -e \| cat -e > o1 \| cat <<1 \| wc -l > o2 \| cat <o2 >old2\| pwd \| cat -e >o2 | various outputs | 0 | YES | ✅ exit=0 |
| ls <<1 \| <<1 cat -e > o1 \| cat <<1 \| cat >o1 | cat o1 shows word3 | 0 | YES | ❌ o1=word2$ — heredoc FD init order broken in 4-stage pipeline (see todos) |
| ls <<1 \| cat -e > o1 \| cat \| cat >o1 | cat o1 shows ls output ending with $ suffix | 0 | YES | ✅ |
| cat << 1 > | syntax error near unexpected token `newline' | 2 | YES | ✅ |
| echo $PATH$PATH$PATH… (many times) | PATH values | 0 | YES | ✅ |

---

## Syntax

| Cases | Expected Output | $? | Required | Done? |
|---|---|---|---|---|
| ls > | syntax error near unexpected token `newline' | 2 | YES | ✅ |
| >>> | syntax error near unexpected token `>' | 2 | YES | ✅ |
| <<< | bash: `newline' / mini: `<' — exit=2 both | 2 | YES | ✅ exit OK, ⚠️ token msg differs from bash |
| \| | syntax error near unexpected token `\|' | 2 | YES | ✅ (test desc was wrong — both bash & mini say `\|') |
| \| \| | syntax error near unexpected token `\|' | 2 | YES | ✅ |
| << | syntax error near unexpected token `newline' | 2 | YES | ✅ |
| >> | syntax error near unexpected token `newline' | 2 | YES | ✅ |

---

## Expander

| Cases | Expected Output | $? | Required | Done? |
|---|---|---|---|---|
| $non_exist $SHELL | shell> (empty) | 0 | YES | ✅ |
| export test=" "; echo $test $test $test abcd $non_exist $test 123 $test | abcd 123 | 0 | YES | ✅ |
| export test="      abcd     123 "; echo $test "$test" | abcd 123       abcd     123  | 0 | YES | ✅ |
| echo $0$1230$0 | minishell230minishell | 0 | YES | ✅ (fixed in both builds: $0→"minishell", $1→skip+literal "230") |
| export test="ls    -la"; $test | ls with -la flag output | 0 | YES | ✅ |

---

## Env

| Cases | Expected Output | $? | Required | Done? |
|---|---|---|---|---|
| echo $SHLVL; ./minishell; echo $SHLVL | first: current lvl, second: SHLVL + 1 | 0 | DEBATABLE | ✅ SHLVL increments correctly |
| export SHLVL=999; ./minishell | minishell: warning: shell level (1000) too high, resetting to 1 | 0 | DEBATABLE | ❌ DEBATABLE — no warning, no reset; prints 1000 |
| export SHLVL=-129139; ./minishell; echo $SHLVL | 0 | 0 | DEBATABLE | ❌ DEBATABLE — bash resets to 1, we just increment (-129138) |
| ls -la; echo $_ | ls output then -la | 0 | DEBATABLE | ❌ DEBATABLE — $_ prints shell path instead of "-la" |

---

## Export

| Cases | Expected Output | $? | Required | Done? |
|---|---|---|---|---|
| export test1="abcd" test2="123" test3="minishell" | adds test1 test2 test3 to envs | 0 | YES | ✅ |
| export 1test | minishell: export: `1test': not a valid identifier | 1 | YES | ✅ |
| export test="123" 1test="test" test3="abcd" | adds test and test3 to envs, error for 1test | 1 | YES | ✅ |
| export test_empty; env | test_empty not in env but shows in `export` as `declare -x test_empty` | 0 | YES | ✅ |

---

## Pipes

| Cases | Expected Output | $? | Required | Done |
|---|---|---|---|---|
| cat \| cat \| cat \| echo | (empty line) | 0 | YES | ✅ |
| cat Makefile \| base64 \| xxd -p \| xxd -r -p \| base64 --decode | Makefile code | 0 | YES | ✅ |
| cat Makefile \| cat -e \| cat -e \| ... (10x) | Makefile with $$$... | 0 | YES | ✅ |
| yes yes \| head -n 10000000 \| wc | 10000000 10000000 40000000 | 0 | YES | ✅ |
| echo \| echo \| echo \| ... (many) | (empty) | 0 | YES | ✅ |
| . \| .. \| . | (empty) | 0 | NO | ❌ DEBATABLE — bash: ". filename required" (exit 2), we: "command not found" (exit 127) |
| exit 1 \| exit 255 \| exit 2 | (empty) | 2 | YES | ✅ |

---

## Echo

| Cases | Expected Output | $? | Required | Done |
|---|---|---|---|---|
| echo -n test | testminishell> (no newline) | 0 | YES | ✅ |
| echo -nnnnnnn -n -nnnnnnb test | -nnnnnnb testminishell> | 0 | YES | ✅ |
| echo -nnnnnnn -n -nnnennE test | testminishell> | 0 | NO | ❌ DEBATABLE — bash treats -E as flag and prints `test` (no newline), we print `-nnnennE test` |
| echo test | test | 0 | YES | ✅ |
| echo """""""" | (empty) | 0 | YES | ✅ |
| echo A""'' | A | 0 | DEBATABLE | ✅ |

---

## Unset

| Cases | Expected Output | $? | Required | Done? |
|---|---|---|---|---|
| unset USER | removes USER env | 0 | YES | ✅ |
| unset USER PATH | removes USER and PATH envs | 0 | YES | ✅ |

---

## Cd

| Cases | Expected Output | $? | Required | Done? |
|---|---|---|---|---|
| cd test/ | minishell: cd: No such file or directory | 1 | YES | ✅ |
| cd /dev/vboxusb | minishell: cd: Permission denied | 1 | YES | ✅ (fixed: was hardcoded "No such file or directory") |
| cd ../../ | (prompt) | 0 | YES | ✅ |
| cd a b | minishell: cd: too many arguments | 1 | YES | ✅ |

---

## Exit

| Cases | Expected Output | $? | Required | Done? |
|---|---|---|---|---|
| exit -1 | exit | 255 | YES | ✅ |
| exit 9223372036854775808- | minishell: exit: 9223372036854775808- : numeric argument required | 2 | YES | ✅ |
| exit -9223372036854775808 | exit | 0 | YES | ✅ |
| exit 9223372036854775808 | minishell: exit: 9223372036854775808 : numeric argument required | 2 | YES | ✅ (fixed: check_boundries was broken) |
| exit 9223372036854775807 | exit | 255 | YES | ✅ |
| exit 1 abc | minishell: exit: too many arguments | 1 | YES | ✅ |
| exit abc | minishell: exit: abc: numeric argument required | 2 | YES | ✅ |
| exit abc 123 | minishell: exit: abc: numeric argument required | 2 | YES | ✅ |
| exit 123 abc | minishell: exit: too many arguments | 1 | YES | ✅ |

---

## Signals

| Cases | Expected Output | $? | Required | Done? |
|---|---|---|---|---|
| sleep 1000 \| ls \| sleep 1000 + CTRL+C | minishell> | 130 | YES | ⚠️ manual |
| sleep 1000 \| ls \| sleep 1000 + CTRL+\ | Quit (core dumped) minishell> | 131 | YES | ⚠️ manual |
| sleep 1 \| ls \| sleep 1000 (check ps aux after 1s) | no zombie processes | 0 | YES | ✅ no zombies confirmed |
| CTRL+C | minishell> | 130 | YES | ⚠️ manual |
| CTRL+\ | minishell> | 0 | YES | ⚠️ manual |

---

## Error Handling

| Cases | Expected Output | $? | Required | Done? |
|---|---|---|---|---|
| ./ | minishell: ./: Is a directory | 126 | NO | ✅ |
| . | minishell> | … | NO | ❌ DEBATABLE — bash: "filename required" (exit 2), we: "command not found" (exit 127) |
| / | minishell: ./: Is a directory | 126 | NO | ✅ |
| file/ | file/: Not a directory | 126 | NO | ✅ |
| touch file_test; chmod +x file_test; ./file_test | no leaks or open fds | … | YES | ✅ no leaks/FDs |

---

## Redirections

| Cases | Expected Output | $? | Required | Done? |
|---|---|---|---|---|
| in < in2 < in3 cat > out > out2 > out3 | reads from in3, writes to out3 | 0 | YES | ✅ |
| << 123 cat << 234 -e > out | reads from 234 with -e, outputs to out | … | YES | ✅ warns EOF for both heredocs, matches bash |
| > / | minishell: /: Is a directory | 1 | NO | ✅ |
| << 123 + CTRL+C | minishell> | 130 | YES | ⚠️ manual |
| << 123 cat + 1234 + CTRL+D | warning: here-document delimited by end-of-file | 0 | YES | ✅ |
| export test="123 test"; < $test | minishell: $test: ambiguous redirect | 1 | NO | ❌ DEBATABLE — we say "No such file or directory", bash says "ambiguous redirect" |
| export test="123 test"; ls > $test | minishell: $test: ambiguous redirect | 1 | NO | ❌ DEBATABLE — we create file "123 test" (exit 0), bash rejects as ambiguous |

---

## Bonus

| Cases | Expected Output | $? | Required | Done? |
|---|---|---|---|---|
| test && echo hi \| sleep 10 \|\| echo nice | nice | 0 | BONUS | ✅ |
| test && exit \|\| yes yes \| head -n 100 && cat + CTRL+D | yes yes ... | 0 | BONUS | ✅ |
| test \|\| exit && echo hi \| exit | (exit) | 0 | BONUS | ✅ |
| (ls && pwd && ls -la) | ls, pwd, ls -la output | 0 | BONUS | ✅ |
| ************M*******a******k***** | Makefile: command not found | 127 | BONUS | ✅ |
| echo .* | .git .gitignore .minishell_history | 0 | BONUS | ❌ includes . and .. (see todos), unsorted |
| echo * | all files sorted | 0 | BONUS | ❌ unsorted output (see todos) |
| echo .*. | .. | 0 | BONUS | ❌ should return literal `.*. ` (no match after excluding . and ..) |

---

## Summary of Checks

### Core Functionality

| Command | Description | Expected Behavior | Status |
|---|---|---|---|
| `/bin/ls` | Basic external command | Lists directory, status 0 | |
| `ls -la \| grep "txt"` | Pipe with filtering | Show only files with "txt" | |
| `echo $HOME` | Env var expansion | Prints home directory path | |

### Redirection & Heredocs

| Command | Description | Expected Behavior | Status |
|---|---|---|---|
| `cat < file.txt` | Input redirection | Displays content of file.txt | |
| `echo "hello" > out.txt` | Output redirection | Creates file with "hello" | |
| `<<EOF cat` | Heredoc | Reads until EOF marker | |
| `< file1 < file2 cat > out1 > out2` | Multiple redirections | Reads from file2, writes to out2 | |

### Signals & Process Management

| Signal Test | Description | Expected Behavior | Status |
|---|---|---|---|
| CTRL+C during command | Interrupt signal | Terminates process, returns to prompt | |
| CTRL+D at prompt | EOF handling | Exits shell gracefully | |
| CTRL+\ during command | Quit signal | Shows "Quit" message | |
| Zombie process check | Process cleanup | No zombie processes | |

### Built-in Commands

| Command | Description | Expected Behavior | Status |
|---|---|---|---|
| `cd /tmp` | Change directory | Changes to /tmp | |
| `export VAR=value` | Set env variable | Creates/updates VAR | |
| `exit 42` | Exit with status | Exits with status 42 | |
| `unset VAR` | Remove env variable | Removes VAR | |

### Error Handling

| Test Case | Description | Expected Behavior | Status |
|---|---|---|---|
| `./nonexistent` | Command not found | Error message, status 127 | |
| `< nonexistent cat` | Missing input file | Error message, status 1 | |
| `> /` | Invalid output location | "Is a directory" error | |
| `ls \| \| wc` | Syntax error | Parse error message | |

### Memory Management

| Test Case | Description | Expected Behavior | Status |
|---|---|---|---|
| Valgrind check | Memory leak detection | No leaks reported | |
| File descriptor check | FD leak detection | All FDs properly closed | |
| Child process cleanup | Process management | No zombie processes | |

### Bonus Features

| Feature | Description | Expected Behavior | Status |
|---|---|---|---|
| Wildcards | Filename expansion | `echo *` lists all files | |
| Logical operators | AND/OR control | `cmd1 && cmd2 \|\| cmd3` works | |
| Subshells | Command grouping | `(cmd1 && cmd2)` executes as unit | |

---

**Good luck with your minishell 🔥**