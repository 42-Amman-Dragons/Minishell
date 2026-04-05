| Cases | Expected Output | `$?` | Done? |
| :--- | :--- | :--- | :--- |
| `/bin/ls` | list directory contents | 0 | ✅ |
| `/bin/ls filethatdoesntexist` | ls: cannot access 'filethatdoesntexist': No such file or directory | 2 | ✅ |
| Empty command (just press enter) | nothing happens, prompt returns | 0 | ✅ |
| Only spaces/tabs | nothing happens, prompt returns | 0 | ✅ |
| `echo hello world` | hello world | 0 | ✅ |
| `echo -n hello` | hello (no newline) | 0 | ✅ |
| `exit` | shell exits | 0 | ✅ |
| `exit 42` | shell exits with code 42 | 42 | ✅ |
| `expr $? + $?` | arithmetic result | 1 | ✅ |
| Ctrl+C in empty prompt | new prompt on new line | 130 | (interactive) |
| Ctrl+\\ in empty prompt | nothing | 0 | (interactive) |
| Ctrl+D in empty prompt | shell exits | 0 | (interactive) |
| `echo '$USER'` | $USER | 0 | ✅ |
| `env` | prints environment | 0 | ✅ |
| `pwd` | prints current dir | 0 | ✅ |
| `./a.out (relative path)` | executes binary | depends | (needs binary) |
| `ls (with PATH set)` | lists directory | 0 | ✅ |
| `unset PATH ; ls` | ls: command not found | 127 | ⚠️ exit code correct (127), but message is `ls: No such file or directory` instead of `ls: command not found` |
| `PATH=/bin:/usr/bin ; ls` | works again | 0 | ❌ standalone variable assignment not supported — tries to execute `PATH=/bin:/usr/bin` as a command |
| `cat < in.txt > out.txt` | out.txt contains contents of in.txt | 0 | ✅ |
| `echo hello >> out.txt` | appends "hello" | 0 | ✅ |
| `cat << EOF + input + EOF` | heredoc works | 0 | ✅ |
| `cat file \| grep foo \| wc -l` | line count | 0 | ✅ |
| `ls filethatdoesntexist \| grep foo` | error from ls, pipe runs | 1 | ✅ |
| `cat file \| grep foo > out.txt` | out.txt contains grep result | 0 | ✅ |
| Type random text `abcd123` | minishell: abcd123: command not found | 127 | ⚠️ exit code correct (127), but missing `minishell:` prefix |
| Very long command | handled gracefully (no crash) | depends | ✅ |
| `echo $USER` | prints user value | 0 | ✅ |
| `echo "$USER"` | prints user value | 0 | ✅ |



| Cases | Expected Output | `$?` | Required | Done |
| :--- | :--- | :--- | :--- | :--- |
| `echo $PWD"$USER"'$HOME'` | `/home/name/minishellname$HOME` | 0 | BONUS | ✅ matches bash |
| `cat << ls > out &#124; cat << ls` | `2` (stdout), `cat out` → `1` | 0 | YES | ✅ |
| `ls <<1 &#124; cat -e &#124; cat -e &#124; cat -e > o1 &#124; cat <<1 &#124; wc -l > o2 &#124; cat <o2 >old2&#124; pwd &#124; cat -e >o2` | `o1` = ls output with `$$$`, `o2` = `1\npwd$` | 0 | YES | ✅ matches bash |
| `ls <<1 &#124; <<1 cat -e > o1 &#124; cat <<1 &#124; cat >o1` | `cat o1` → `word3` | 0 | YES | ❌ minishell writes `word2$` — last `cat >o1` does not overwrite |
| `ls <<1 &#124; cat -e > o1 &#124; cat &#124; cat >o1` | `cat o1` → ls output with `$` | 0 | YES | ✅ matches bash |
| `cat << 1 >` | `minishell: syntax error near unexpected token 'newline'` | 2 | YES | ✅ |
| `echo $PATH$PATH$PATH$PATH$PATH$PATH$PATH$PATH.....` | all PATH values concatenated | 0 | YES | ✅ |



| Cases | Expected Output | `$?` | Required | Done? |
| :--- | :--- | :--- | :--- | :--- |
| `ls >` | `minishell: syntax error near unexpected token 'newline'` | 2 | YES | ✅ |
| `>>>` | `minishell: syntax error near unexpected token '>'` | 2 | YES | ✅ |
| `<<<` | `minishell: syntax error near unexpected token '<'` | 2 | YES | ⚠️ outputs `'<'` (correct exit 2), but bash outputs `'newline'` (`<<<` is here-string in bash) |
| `&#124;` | `minishell: syntax error near unexpected token '&#124;'` | 2 | YES | ✅ (expected in table was wrong — bash also says `'|'` not `'newline'`) |
| `&#124;&#124;` | `minishell: syntax error near unexpected token '&#124;&#124;'` | 2 | YES | ✅ |
| `<<` | `minishell: syntax error near unexpected token 'newline'` | 2 | YES | ✅ |
| `>>` | `minishell: syntax error near unexpected token 'newline'` | 2 | YES | ✅ |

| Cases | Expected Output | `$?` | Required | Done? |
| :--- | :--- | :--- | :--- | :--- |
| `$non_exist $SHELL` | `shell>` (interactive prompt of $SHELL) | 0 | YES | ✅ exits 0, $SHELL is launched (no visible prompt in non-interactive) |
| `export test="`<br>`";`<br>`echo $test $test`<br>`$test abcd`<br>`$non_exist $test`<br>`123 $test` | blank line + `abcd: command not found` + `123: command not found` | 127 | YES | ✅ matches bash |
| `export test="`<br>`abcd  123 "`<br>`echo $test "$test"` | `abcd 123 ` (unquoted, word-split) + newline + `abcd  123 ` (quoted, preserved) | 0 | YES | ✅ matches bash |
| `echo $0$1230$0` | `minishell230minishell` | 0 | YES | ✅ ($0=minishell, $1 empty so $1230=230) |
| `export test="ls -la"`<br>`$test` | `ls -la` output | 0 | YES | ✅ word-splits variable correctly into cmd + args |



### Env:

| Cases | Expected Output | `$?` | Required | Done? |
| :--- | :--- | :--- | :--- | :--- |
| `echo $SHLVL`<br>`./minishell`<br>`echo $SHLVL` | first echo will output current shell lvl second will output SHLVL + 1 | 0 | DEBATABLE | ✅ increments correctly (e.g. SHLVL=5 → 6) |
| `export SHLVL=999`<br>`./minishell` | minishell: warning: shell level (1000) too high, resetting to 1 | 0 | DEBATABLE | ✅ |
| `export SHLVL=-129139`<br>`./minishell`<br>`echo $SHLVL` | 0 | 0 | DEBATABLE | ✅ |
| `ls -la`<br>`echo $_` | ls output<br>-la | 0 | DEBATABLE | ✅ `$_` correctly expands to last arg of previous command |

### Export:

| Cases | Expected Output | `$?` | Required | Done? |
| :--- | :--- | :--- | :--- | :--- |
| `export test1="abcd"`<br>`test2="123"`<br>`test3="minishell"` | Must add test1 test2 test3 to envs | 0 | YES | ✅ test1 exported; `test2=123` and `test3=minishell` are bare assignments → "command not found" (same as bash — only `export` adds to env) |
| `export 1test` | minishell: export: '1test': not a valid identifier | 1 | YES | ✅ |
| `export test="123"`<br>`1test="test"`<br>`test3="abcd"` | Will add test and test3 to envs | 0 | YES | ✅ test exported; `1test=test` → command not found; `test3=abcd` → command not found (matches bash) |
| `export test_empty;`<br>`env` | All envs except test_empty | 0 | YES | ✅ unexported var not in env |

### PIPES:

| Cases | Expected Output | `$?` | Required | Done |
| :--- | :--- | :--- | :--- | :--- |
| `cat &#124; cat &#124; cat &#124; echo` | blank line (echo prints immediately) | 0 | YES | ✅ |
| `cat Makefile &#124; base64 &#124; xxd -p &#124; xxd -r -p &#124; base64 --decode` | `Makefile code` | 0 | YES | ✅ md5sum matches bash |
| `cat Makefile &#124; cat -e &#124; cat -e &#124; cat -e &#124; cat -e &#124; cat -e &#124; cat -e &#124; cat -e &#124; cat -e &#124; cat -e` | lines ending with `$$$$$$$$$` (9 $'s) | 0 | YES | ✅ |
| `yes yes &#124; head -n 10000000 &#124; wc` | `10000000 10000000 40000000` | 0 | YES | ✅ |
| `echo &#124; echo &#124; echo &#124; echo &#124; echo &#124; echo &#124; echo &#124; echo &#124; echo &#124; echo &#124; echo &#124; echo` | blank line | 0 | YES | ✅ |
| `. &#124; .. &#124; .` | `minishell>` | 0 | NO | (not required) |
| `exit 1 &#124; exit 255 &#124; exit 2` | exits with last pipeline exit code | 2 | YES | ✅ |


### Echo:

| Cases | Expected Output | `$?` | Required | Done |
| :--- | :--- | :--- | :--- | :--- |
| `echo -n test` | `testminishell>` | 0 | YES | ✅ |
| `echo -nnnnnn -n -nnnnnnb test` | `-nnnnnnb test` (no newline) | 0 | YES | ✅ matches bash |
| `echo -nnnnnn -n -nnnennE test` | `testminishell>` | 0 | NO | ⚠️ outputs `-nnnennE test` — minishell doesn't treat `-e`/`-E` as valid echo flags (not required) |
| `echo test` | `test` | 0 | YES | ✅ |
| `echo """"""""` | (empty line) | 0 | YES | ✅ |
| `echo A""""` | `A` | 0 | DEBATABLE | ✅ |

### Unset:

| Cases | Expected Output | `$?` | Required | Done |
| :--- | :--- | :--- | :--- | :--- |
| `unset USER` | Removes USER env | 0 | YES | ✅ |
| `unset USER PATH` | Removes USER and<br>PATH envs | 0 | YES | ✅ |

### Cd:

| Cases | Expected Output | `$?` | Required | Done |
| :--- | :--- | :--- | :--- | :--- |
| `cd test/` | `minishell: cd: test/: No such file or directory` | 1 | YES | ✅ |
| `cd /dev/vboxusb` | `minishell: cd: /dev/vboxusb: Permission denied` | 1 | YES | ✅ |
| `cd ../../` | changes dir correctly | 0 | YES | ✅ |
| `cd a b` | `minishell: cd: too many arguments` | 1 | YES | ⚠️ outputs `cd: too many arguments` — missing `minishell:` prefix |

### Exit:

| Cases | Expected Output | `$?` | Required | Done |
| :--- | :--- | :--- | :--- | :--- |
| `exit -1` | `exit` | 255 | YES | ✅ |
| `exit 9223372036854775808-` | `minishell: exit: 9223372036854775808-: numeric argument required` | 2 | YES | ✅ |
| `exit -9223372036854775808` | `exit` | 0 | YES | ✅ |
| `exit 9223372036854775808` | `minishell: exit: 9223372036854775808: numeric argument required` | 2 | YES | ✅ |
| `exit 9223372036854775807` | `exit` | 255 | YES | ✅ |
| `exit 1 abc` | `minishell: exit: too many arguments` | 1 | YES | ✅ |
| `exit abc` | `minishell: exit: abc: numeric argument required` | 2 | YES | ✅ |
| `exit abc 123` | `minishell: exit: abc: numeric argument required` | 2 | YES | ✅ |
| `exit 123 abc` | `minishell: exit: too many arguments` | 1 | YES | ✅ |



### Error Handling:

| Cases | Expected Output | `$?` | Required | Done |
| :--- | :--- | :--- | :--- | :--- |
| `./` | `./: Is a directory` | 126 | NO | ✅ correct exit code and message |
| `.` | `.: command not found` | 127 | NO | ✅ |
| `/` | `/: Is a directory` | 126 | NO | ✅ |
| `file/` | `file/: Not a directory` | 126 | NO | ✅ |
| `touch file_test`<br>`chmod +x file_test`<br>`./file_test` | Must not leak or<br>leave open fds | ... | YES | ✅ runs (returns "Exec format error" exit 126 for empty file) |

### Redirections:

| Cases | Expected Output | `$?` | Required | Done |
| :--- | :--- | :--- | :--- | :--- |
| `cat < in < in2 < in3 > out > out2 > out3` | cat reads from in3, output to out3 (last redirect wins) | 0 | YES | ✅ matches bash |
| `<< 123 cat << 234 -e > out` | cat reads from 2nd heredoc with -e, outputs to out | 0 | YES | ✅ matches bash |
| `> /` | `minishell: /: Is a directory` | 1 | NO | ✅ |
| `<< 123`<br>`CTRL+C` | `minishell>` | 130 | YES | (interactive) |
| `<< 123 cat`<br>`1234`<br>`CTRL+D` | warning: here-document delimited by end-of-file | 0 | YES | (interactive) |
| `export test="123 test"`<br>`< $test` | `minishell: $test: ambiguous redirect` | 1 | NO | ⚠️ no ambiguous redirect detection — treats expanded value as filename |
| `export test="123 test"`<br>`ls > $test` | `minishell: $test: ambiguous redirect` | 1 | NO | ⚠️ no ambiguous redirect detection — creates file instead |

### Bonus:

| Cases | Expected Output | `$?` | Required | Done |
| :--- | :--- | :--- | :--- | :--- |
| `test && echo hi &#124; sleep 10 &#124;&#124; echo nice` | `nice` | 0 | BONUS | ✅ |
| `test && exit &#124;&#124; yes yes &#124; head -n 100 && cat`<br>`CTRL+D` | `yes` repeated 100 times | 0 | BONUS | (interactive) |
| `test &#124;&#124; exit && echo hi` | shell exits | 1 | BONUS | ✅ matches bash (exit inherits $?=1 from `test`) |
| `(ls && pwd && ls -la)` | `ls output`<br>`pwd output`<br>`ls -la output` | 0 | BONUS | ✅ |
| `in < (cat && ls && ...) > out` | `minishell>` | 0 | NO | (not required — syntax error as expected) |
| `*************M*******a******k*****` | `Makefile: command not found` | 127 | BONUS | ✅ |
| `echo .*` | hidden files (excluding `.` and `..`) | 0 | BONUS | ✅ (files not sorted alphabetically, bash sorts them) |
| `echo *` | all non-hidden files | 0 | BONUS | ✅ (files not sorted alphabetically, bash sorts them) |
| `echo .*.` | `.*. ` (no match, literal) | 0 | BONUS | ✅ matches bash |