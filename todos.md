- [x] leaks and open fds when calling the `ps aux | grep Z`
- [?] handle the ~ — `cd ~` works (both builds), but `echo ~` prints literal `~`. Tilde expansion missing in expander (both builds).
- [x] cat << ls > out | cat << ls open file descriptors.
- [x] ls <<1 | cat -e | cat -e | cat -e > o1 | cat <<1 | wc -l > o2 | cat <o2 >old2| pwd | cat -e >o2
- [?] ls <<1 | <<1 cat -e > o1 | cat <<1 | cat >o1 → o1 should be "word3" but we get "word2$". heredoc FD init order issue in 4-stage pipelines (both builds).
- [] cat << 1 > — should open heredoc then raise syntax error. We give syntax error without opening heredoc (both builds).
- [x] $non_exist $SHELL -> the code crashes.
- [x] export test="           "; echo $test Hello — working correctly, no memory leak (both builds).
- [x] echo $0$1230$0 — fixed in both builds: $0→"minishell" added to expand_word.c (mandatory) and expand_word_bonus.c (bonus). $1230 skips $1 and prints "230" literally.
- [x] export test="ls    -la"; $test
- [x] single quote — working correctly (both builds).
- [x] add the is directory error
- [x] cd /dev/vboxusb ==> permission denied — fixed in cd.c with strerror(errno) (both builds).
- [] the error code for sleep 1000 | ls | sleep 1000; CTRL+\ — manual test needed. Expected: "Quit (core dumped)" + exit 131 (both builds).
- [x] Open file descriptor in the && and ||
- [x] check the heredoc logic — no leaks/open fds confirmed in valgrind.
- [] the dot command — not required. We return "command not found" (127), bash returns "filename required" (2).
- [x] the * double free — fixed in generate_expanded_list.c (bonus).
- [x] bonus wildcard issues — fully fixed `.` and `..` filtering, sorting, quoted stars (via `\x01` sentinel), ambiguous redirects, and variable expansion globbing. Only known unfixable edge cases (Test W5 spaces filename and GW10 tokenizer bug) remain.
- [] multi-line quoted strings in non-interactive mode — gives "unclosed quote" error (both builds).
- [] VAR=value standalone assignment not supported — executes as command (both builds).
- [x] exit overflow — fixed in exit.c with check_boundries using unsigned long long (both builds).
- [x] bonus expander word-splitting — synced bonus expander_utils_bonus.c to use mandatory's normalize_ifs and unquoted_dollar_has_space logic.

## Tester Remaining Failures (Manual Recheck)

These 5 test cases (10 individual checks) still fail in both builds. Run `bash 42_minishell_tester/tester.sh m` to reproduce.

- [] **1_scmds.sh:19** — `touch "\n"` (multi-line quoted filename via literal newline in the script).
  - STD_ERR ❌: we print "unclosed quote" error, bash creates the file silently.
  - Root cause: non-interactive mode rejects multi-line quoted strings. Both builds.

- [] **2_correction.sh:265** — `unset PATH; cd /bin; ls`
  - STD_OUT ❌, STD_ERR ❌, EXIT_CODE ❌: we exit 127 ("ls: command not found"), bash outputs full `ls` listing (exit 0).
  - Root cause: bash falls back to searching the cwd (or `/bin/ls` is found via exec directly). We only search PATH. Both builds.

- [] **2_correction.sh:269** — `unset PATH; cd /bin/../bin/; ls`
  - STD_OUT ❌, STD_ERR ❌, EXIT_CODE ❌: same as above — exit 127 instead of exit 0 with listing.
  - Root cause: same as :265. Both builds.

- [] **2_correction.sh:277** — `unset PATH; cd /bin/; sleep 2`
  - STD_ERR ❌, EXIT_CODE ❌: we exit 127 ("sleep: command not found"), bash exits 0 (or runs sleep successfully).
  - Root cause: same PATH-fallback issue. Both builds.

- [] **2_path_check.sh:49** — `touch tmp_x_file1; chmod +x tmp_x_file1; ./tmp_x_file1` (empty executable file).
  - STD_OUT ❌: we print "Exec format error" (exit 126), bash treats an empty executable as an empty shell script (exit 0, no output).
  - Root cause: execve returns ENOEXEC for empty files; bash retries with `/bin/sh`, we don't. Both builds.
