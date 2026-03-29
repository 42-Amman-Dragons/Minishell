# todos v2

Legend:
- [x] verified and matched bash behavior
- [] verified and differs from bash behavior
- [?] manual-only or needs tooling not covered by this pass

## Completed ([x])

- [x?] race condition: `ls <<1 | cat -e | cat -e | cat -e > o1 | cat <<1 | wc -l > o2 | cat <o2 >old2| pwd | cat -e >o2`
  - Needs fd/leak validation tooling.

- [x?] race condition: `ls <<1 | <<1 cat -e > o1 | cat <<1 | cat >o1`
  - Heredoc fd init order issue.

- [x] `$non_exist $SHELL -> the code crashes.`
  - Rechecked: no crash; behavior matched.

- [x] `export test="           "; echo $test Hello`
  - Rechecked: matched bash.

- [x] `echo $0$1230$0`
  - Diff by design when compared to bash (`$0` is `bash` vs `minishell`).

- [x] `export test="ls    -la"; $test`
  - Rechecked: matched bash.

- [x] single quote
  - Basic quoted case rechecked (`echo 'abc'`) and matched.

- [x] Open file descriptor in the `&&` and `||`
  - Logical behavior smoke test matched; fd leak proof still needs tooling.

- [x] `cd /dev/vboxusb ==> permission denied`

- [x] **exit spacing case fixed**
  - `exit '  5'` and `exit '5  '` now match bash behavior and pass minitester `-t exit`.

- [x] `cat << ls > out | cat << ls` open file descriptors
  - Needs fd-level tooling; not provable by stdout/stderr only.




## pwd/cd deleted-dir block status [too much]

- [x] `mkdir a a/b; cd a/b; rm -rf ../../a; pwd`

- [] `mkdir a a/b; cd a/b; rm -rf ../../a; cd ..`
cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
- [] `mkdir a a/b; cd a/b; rm -rf ../../a; unset PWD OLDPWD; cd ..`



## Mohannad:
- [] `unset PATH; cd /bin; ls`
  - still diff: bash `0`, minishell `127`.

- [] `unset PATH; cd /bin/../bin/; ls`
  - still diff: bash `0`, minishell `127`.

- [] `unset PATH; cd /bin/; sleep 2`
  - still diff: bash `0`, minishell `127`.

- [] `touch tmp_x_file1; chmod +x tmp_x_file1; ./tmp_x_file1`
  - still diff: bash runs as empty script (`0`), minishell returns `126`.
- [] unclosed quotes exit code must be 2
- [] the error code for `sleep 1000 | ls | sleep 1000; CTRL+\\`
  - Manual interactive signal test needed (expected `Quit (core dumped)`, exit `131`).

## Haya

- [] `NONCMD | NONCMD | NONCMD`
- `cd ~/Desktop` ==> needs implementations
- [] `cat << 1 >`
  - Syntax result differs in message details (`bash:` prefix and wording differ), exit code both `2`.
  - Whether heredoc is opened before syntax error is still manual/fd-level.



- Execution  --> 
- Expansion  --> 