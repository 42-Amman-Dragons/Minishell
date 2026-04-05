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
| `export SHLVL=999`<br>`./minishell` | minishell: warning: shell level (1000) too high, resetting to 1 | 0 | DEBATABLE | ❌ no warning, sets SHLVL=1000 without resetting |
| `export SHLVL=-129139`<br>`./minishell`<br>`echo $SHLVL` | 0 | 0 | DEBATABLE | ❌ outputs -129138 (just increments); bash clamps negative SHLVL to 0 |
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