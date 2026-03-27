# Execution Module — Error / Cleanup / Wait / Close Flow Analysis

## Current Structure

### Functions and what they own

```
CHILD PROCESS DEATH
  free_and_exit(node, shell, code)
    └── free_tree(node)        ← frees AST
    └── exit(cleanup_shell())  ← tears down shell struct

SHELL STRUCT TEARDOWN
  cleanup_shell(shell, exit_code)
    └── close_tracked_fds()   ← closes openfiles list
    └── free env, history, prompt, line

HEREDOC FD LIFETIME — TWO SEPARATE SYSTEMS:
  [system A] openfiles list (child-side)
    track_fd(shell, fd)         ← adds fd to shell->openfiles
    close_tracked_fds(shell)    ← closes all, clears list
    called before: execve, pipe children, heredoc child

  [system B] node walk (parent-side)
    close_heredoc_fds(node)     ← walks redir list, closes heredoc_fd
    called in: run_cmd() parent, after waitpid
    NOTE: free_redir() in node_free.c ALSO closes heredoc_fd on free

PIPE FD MANAGEMENT
  temp_redir(stdin*, stdout*)   ← dup() saves current stdin/stdout
  safe_close(fd*, msg)          ← non-fatal close, sets *fd = -1
  restore_close_redir(fd, in*, out*)
    └── safe_close pipe ends
    └── dup2 back to saved
    └── safe_close saved

PIPE CHILD FD CLOSE
  secure_close(fd, node, shell) ← close pipe end, fatal if error

BUILTIN ERROR HANDLERS
  temp_dup_error(in, out, shell)      ← dup() failed: just close temps
  redirection_failure(in, out, shell) ← redir failed: dup2 back + close temps

WAIT / STATUS
  wait_all(pid)             ← waits all children, tracks rightmost exit
  child_exit_status(status) ← translates waitpid status → exit code
```

---

## The Problems

### 1. Heredoc FDs get touched 3 different ways

```
track_fd()          → adds fd to openfiles list
close_tracked_fds() → closes fd by int value (does NOT reset rd->heredoc_fd)
close_heredoc_fds() → closes rd->heredoc_fd, sets it to -1
free_redir()        → checks rd->heredoc_fd >= 0, closes it again
```

If the child calls `close_tracked_fds` and later the parent calls `free_redir` via
`free_tree`, the fd number in `rd->heredoc_fd` was never reset to -1 by the list-based
close — **double close on every heredoc**. In the parent, `close_heredoc_fds` +
`free_redir` can also double-close.

### 2. Two "close" functions with inverted naming

| Function | Takes | On failure | Sets -1 |
|---|---|---|---|
| `safe_close` | `int *fd` | prints, continues | yes |
| `secure_close` | `int fd` | calls `free_and_exit` | no |

The names imply `secure` is safer than `safe`, but it is the opposite —
`secure_close` kills the process on error.

### 3. Builtin error path is split across two tiny functions

`temp_dup_error` and `redirection_failure` both clean up the same two temp fds
(`temp_stdin` / `temp_stdout`) in `exec_cmd`. They differ by one step (the dup2
restore). The success path at the bottom of `exec_cmd` does the restore and close
inline. Three different places manage the same two fds.

### 4. `free_and_exit` frees the AST unnecessarily in children

The process immediately calls `exit()`. The OS reclaims everything. `free_tree(node)`
before exit is pure noise — and risky if the node is partially valid after an error.

### 5. Signal restore is the caller's responsibility on every exit path

Every caller of `set_signals_exec()` must remember to call `set_signals_prompt()` on
every return path. `run_cmd`, `exec_pipe`, and `exec_subshell` all duplicate this
pattern. A forgotten `set_signals_prompt()` would leave the shell silently ignoring
signals.

---

## Proposed Streamlining

### A — Collapse the two heredoc fd systems into one

The `openfiles` list exists because children need to close fds they inherited from
*other* nodes, not just their own node's redir list. But `close_heredoc_fds` in the
parent is redundant — `free_redir` already handles it when the AST is freed at the end
of `parse_and_execute`.

**Fix:** Drop `close_heredoc_fds` from `run_cmd`. Let `free_redir` do the parent-side
cleanup. For children, keep `close_tracked_fds` but store `int *` (a pointer to
`rd->heredoc_fd`) in the list instead of a copy of the int — then `close_tracked_fds`
can close and reset to -1 atomically, eliminating the double-close.

### B — Rename to reflect behavior, not perceived safety level

```c
safe_close    → close_fd       // non-fatal, resets pointer
secure_close  → close_fd_fatal // exits on error, or just inline it (3 lines)
```

### C — Consolidate the builtin temp-fd cleanup

Instead of `temp_dup_error` and `redirection_failure` as two separate functions,
one helper that always restores then closes:

```c
int restore_temps(int temp_in, int temp_out, t_minishell *shell, int code);
```

`exec_cmd` calls it on both the error path and the success path — the restore is
always needed.

### D — Drop `free_tree` from `free_and_exit`

Children are about to `exit()`. The OS handles memory. Removing the `free_tree` call
simplifies `free_and_exit` to two lines and removes the risk of operating on a
partially-valid node after an error.

```c
void free_and_exit(t_tree *node, t_minishell *shell, int exit_code)
{
    (void)node;
    exit(cleanup_shell(shell, exit_code));
}
```

### E — Wrap the signal sandwich into a helper

The pattern below appears identically in `run_cmd`, `exec_pipe`, and `exec_subshell`:

```c
set_signals_exec();
// ... fork, work, waitpid ...
set_signals_prompt();
```

A documented wrapper or macro that enforces the restore on all paths would prevent
signal-restore bugs from accumulating as the code grows.

---

## Priority

| Fix | Impact | Risk |
|-----|--------|------|
| A — drop `close_heredoc_fds`, fix double-close | High | Low |
| D — drop `free_tree` from `free_and_exit` | Medium | Low |
| C — consolidate builtin temp-fd cleanup | Medium | Low |
| B — rename close functions | Low | Very low |
| E — signal sandwich wrapper | Low | Low |
