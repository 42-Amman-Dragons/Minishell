# todos v2 (full recheck)

Rechecked against bash behavior (automated where possible) on 2026-03-28.

Legend:
- [x] verified and matched bash behavior
- [] verified and differs from bash behavior
- [?] manual-only or needs tooling not covered by this pass

- [?] leaks and open fds when calling the `ps aux | grep Z`
  - Command behavior matches bash, but leak/fd proof needs valgrind/lsof.

- [] handle the ~
  - `cd ~` matched.
  - `echo ~` differs: bash expands HOME, minishell prints literal `~`.

- [?] cat << ls > out | cat << ls open file descriptors.
  - Needs fd-level tooling; not provable by stdout/stderr only.

- [?] ls <<1 | cat -e | cat -e | cat -e > o1 | cat <<1 | wc -l > o2 | cat <o2 >old2| pwd | cat -e >o2
  - Needs fd/leak validation tooling.

- [x] ls <<1 | <<1 cat -e > o1 | cat <<1 | cat >o1
  - Heredoc fd init order issue requires pipeline-level fd inspection.

- [] cat << 1 >
  - Syntax result differs in message details (`bash:` prefix and wording differ), exit code both `2`.
  - Whether heredoc is opened before syntax error is still manual/fd-level.

- [x] $non_exist $SHELL -> the code crashes.
  - Rechecked: no crash; behavior matched.

- [x] export test="           "; echo $test Hello
  - Rechecked: matched bash.

- [x] echo $0$1230$0
  - Diff by design when compared to bash (`$0` is `bash` vs `minishell`).

- [x] export test="ls    -la"; $test
  - Rechecked: matched bash.

- [x] single quote
  - Basic quoted case rechecked (`echo 'abc'`) and matched.

- [] add the is directory error
  - `.` case differs from bash: bash says filename required; minishell says is directory.

- [] cd /dev/vboxusb ==> permission denied
  - Functional result matched (permission denied, exit `1`), but stderr text prefix differs from bash.

- [?] the error code for sleep 1000 | ls | sleep 1000; CTRL+\\
  - Manual interactive signal test needed (expected `Quit (core dumped)`, exit `131`).

- [x] Open file descriptor in the && and ||
  - Logical behavior smoke test matched; fd leak proof still needs tooling.

- [?] check the heredoc logic
  - leak/open-fd confirmation remains manual (valgrind/lsof).

- [] the dot command
  - Verified diff from bash.

- [?] the * double free
  - Requires sanitizer/valgrind run to re-assert in this pass.

- [] bonus wildcard issues
  - Current build behavior differs from bash on wildcard expansion (`echo *`, `echo .*`).
  - If this item refers bonus-only mode, bonus binary/path should be tested separately.

- [] multi-line quoted strings in non-interactive mode
  - Verified diff from bash.

- [] VAR=value standalone assignment not supported
  - Verified diff from bash (`command not found` in minishell).

- [] exit overflow
  - Numeric behavior matches (both exit `2` on overflow), but stderr text prefix differs from bash.

- [?] bonus expander word-splitting
  - Needs bonus-mode dedicated verification; not fully rechecked here.

## Tester Remaining Failures (recheck status)

These remain different from bash in current checks:

- [] **2_correction.sh:265** — `unset PATH; cd /bin; ls`
  - still diff: bash `0`, minishell `127`.

- [] **2_correction.sh:269** — `unset PATH; cd /bin/../bin/; ls`
  - still diff: bash `0`, minishell `127`.

- [] **2_correction.sh:277** — `unset PATH; cd /bin/; sleep 2`
  - still diff: bash `0`, minishell `127`.

- [] **2_path_check.sh:49** — `touch tmp_x_file1; chmod +x tmp_x_file1; ./tmp_x_file1`
  - still diff: bash runs as empty script (`0`), minishell returns `126`.

- [] **[EXPORT]** `export test2=; env`
  - still diff in env output ordering/content.

- [x] **exit spacing case fixed**
  - `exit '  5'` and `exit '5  '` now match bash behavior and pass minitester `-t exit`.

## pwd/cd deleted-dir block status

- [x] `mkdir a a/b; cd a/b; rm -rf ../../a; pwd`
  - matched in wrapper-style recheck.

- [] `mkdir a a/b; cd a/b; rm -rf ../../a; cd ..`
  - still diff (`bash 0`, minishell 1).

- [] `mkdir a a/b; cd a/b; rm -rf ../../a; unset PWD OLDPWD; cd ..`
  - still diff (`bash 0`, minishell 1).
