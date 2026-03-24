# Execution Module Audit Report

Files audited: `execute_cmd.c`, `exeute_cmd_utils.c`, `execution.c`, `execution_utils.c`,
`handle_redirections.c`, `execute_pipe.c`, `execute_subshell.c`, `execute_oper.c`

---

## CRITICAL — Bugs that will crash or leak

---

### [1] FD LEAK: `temp_redir` partial success
**File:** [execute_pipe.c:100-110](src/Execution/execute_pipe.c#L100-L110)

```c
int temp_redir(int *temp_stdin, int *temp_stdout)
{
    *temp_stdin = dup(STDIN_FILENO);   // succeeds, opens fd N
    *temp_stdout = dup(STDOUT_FILENO); // fails → -1
    if (*temp_stdin == -1 || *temp_stdout == -1)
    {
        perror("dup error");
        return (-1);                   // returns WITHOUT closing *temp_stdin ← LEAK
    }
    return (0);
}
```

And the caller at [execute_pipe.c:137-138](src/Execution/execute_pipe.c#L137-L138):

```c
if (temp_redir(&temp_std[0], &temp_std[1]) == -1)
    return (-1);   // no cleanup of temp_std[0] if it was opened
```

**Impact:** Every time `dup(STDIN_FILENO)` succeeds but `dup(STDOUT_FILENO)` fails, `temp_stdin` is leaked.

**Fix:** Close whichever fd was opened before returning -1:
```c
if (*temp_stdin != -1)
    close(*temp_stdin);
if (*temp_stdout != -1)
    close(*temp_stdout);
```

---

### [2] NULL DEREFERENCE: `ft_split` result not checked in `absoulute_path`
**File:** [exeute_cmd_utils.c:63-66](src/Execution/exeute_cmd_utils.c#L63-L66)

```c
paths = ft_split(path, ':');
free(path);
// NO NULL CHECK HERE
while (paths[i])   // ← crash if ft_split returned NULL
```

`ft_split` can return NULL on malloc failure. Dereferencing NULL is undefined behavior.

**Fix:** Add a NULL check after `ft_split`:
```c
paths = ft_split(path, ':');
free(path);
if (!paths)
    return (NULL);
```

---

### [3] MALLOC FAIL: `cmd_name = NULL` propagates into `absoulute_path`
**File:** [execute_cmd.c:46-54](src/Execution/execute_cmd.c#L46-L54)

```c
cmd_name = ft_strdup(node->data.cmd.args[0]);   // could return NULL
if (!ft_strchr(node->data.cmd.args[0], '/'))
{
    free(node->data.cmd.args[0]);
    node->data.cmd.args[0] = absoulute_path(cmd_name, shell->env); // cmd_name=NULL
}
```

If `ft_strdup` fails, `cmd_name` is NULL. It is then passed to `absoulute_path` as `cmd`,
then passed to `asssemple_path(paths[i], cmd)`, which calls:

```c
sub = ft_strjoin(one_path, "/");
sub = safe_join(sub, cmd);   // safe_join(sub, NULL) → ft_strjoin(sub, NULL)
```

`safe_join` does not guard against `str2 == NULL`. If `ft_strjoin` does not handle a NULL
second argument (standard libft doesn't), this crashes.

**Fix:** Guard at the top of `execve_cmd`:
```c
cmd_name = ft_strdup(node->data.cmd.args[0]);
if (!cmd_name)
    free_and_exit(node, shell, 1);
```

---

## HIGH — Logic errors or incorrect behavior

---

### [4] DESIGN BUG: Fork failure exits the parent shell process
**File:** [execute_pipe.c:48-52](src/Execution/execute_pipe.c#L48-L52) and [execute_pipe.c:77-81](src/Execution/execute_pipe.c#L77-L81)

```c
if (left_id == -1)
{
    perror("PIPE ERROR: ");
    free_and_exit(node, shell, 1);   // ← calls exit() in the parent!
}
```

`free_and_exit` calls `exit()`. If fork fails in the parent, the entire shell process exits.
The shell should print an error and return a non-zero status, not die.

At the point of fork failure in `handle_right_pipe`, `fd[0]`, `fd[1]`, `temp_std[0]`,
`temp_std[1]` are all open and are abandoned before `exit()`.

---

### [5] INCONSISTENCY: `get_path` uses the LAST `PATH=` match, not the first
**File:** [exeute_cmd_utils.c:23-28](src/Execution/exeute_cmd_utils.c#L23-L28)

```c
while (env[i])
{
    if (ft_strnstr(env[i], "PATH=", 5))
        path = env[i];   // keeps overwriting → last match wins
    i++;
}
```

No `break` after finding a match. If `PATH=` appears more than once in the environment
(which is possible), this uses the last occurrence. All other shells use the first.

**Fix:** Add `break;` after `path = env[i];`.

---

## MEDIUM — Inconsistencies and maintainability issues

---

### [6] INCONSISTENCY: `redirect_heredoc` return value silently discarded
**File:** [handle_redirections.c:89-90](src/Execution/handle_redirections.c#L89-L90)

```c
else if (rd->mode == DIR_IN_HEREDOC)
    redirect_heredoc(rd);   // return value not checked
```

Every other branch checks for `-1`:
```c
if (redirect_input(rd, node, shell) == -1)
    return (-1);
```

`redirect_heredoc` currently always returns 0, so this is not a runtime bug today.
But the inconsistency makes the code fragile — if `redirect_heredoc` is ever changed
to return -1 on failure, this path will silently ignore the error.

**Fix:** Make it consistent:
```c
else if (rd->mode == DIR_IN_HEREDOC)
{
    if (redirect_heredoc(rd) == -1)
        return (-1);
}
```

---

### [7] INCONSISTENCY: `handle_left_pipe` return value discarded
**File:** [execute_pipe.c:149](src/Execution/execute_pipe.c#L149)

```c
handle_left_pipe(fd, temp_std, node, shell);   // pid_t return value discarded
right_id = handle_right_pipe(fd, temp_std, node, shell);
```

`handle_left_pipe` returns `pid_t left_id` but the caller drops it. The function signature
implies the caller needs it, yet `wait_all(-1)` collects all children so it's not needed.

Either:
- Change `handle_left_pipe` to return `void`, or
- Store it: `left_id = handle_left_pipe(...)` (even if unused, at least document why)

---

## LOW — Dead code, redundancy, naming

---

### [8] DEAD PARAMETERS: `node` and `shell` in redirect functions
**File:** [handle_redirections.c:15-67](src/Execution/handle_redirections.c#L15-L67)

```c
static int redirect_input(t_redir_data *rd, t_tree *node, t_minishell *shell)
{
    (void)node;
    (void)shell;
    ...
}
```

All three static redirect functions (`redirect_input`, `redirect_output`, `redirect_append`)
suppress `node` and `shell` with `(void)`. These parameters are passed through the entire
call chain (`handle_redirections` → `redirect` → individual functions) but never used.

This is dead parameter passing — it adds noise to every call site with no benefit.

---

### [9] REDUNDANCY: Two close wrappers with incompatible APIs
**File:** [execute_pipe.c:112-119](src/Execution/execute_pipe.c#L112-L119) vs [execution_utils.c:38-45](src/Execution/execution_utils.c#L38-L45)

```c
// execute_pipe.c
void safe_close(int *fd, char *msg)      // takes pointer, sets to -1, no exit

// execution_utils.c
void secure_close(int fd, t_tree *node, t_minishell *shell)  // takes value, exits on error
```

Two close-wrapper functions with different names, different signatures, and different
error strategies. Both are used throughout the execution module. The inconsistency
makes it hard to reason about which to use.

---

### [10] TYPOS in identifiers and filenames

| Location | Typo | Should be |
|---|---|---|
| `exeute_cmd_utils.c` filename | `exeute` | `execute` |
| `exeute_cmd_utils.c:44` | `asssemple_path` (3×`s`) | `assemble_path` |
| `execute_cmd.c:50` | `absoulute_path` | `absolute_path` |

These typos propagate into the header declarations and all call sites.

---

### [11] MISSING BLANK LINE before `exec_pipe`
**File:** [execute_pipe.c:129-130](src/Execution/execute_pipe.c#L129-L130)

```c
}
int exec_pipe(t_tree *node, t_minishell *shell)
```

No blank line between `restore_close_redir` and `exec_pipe`. Minor style inconsistency
with the rest of the codebase.

---

## Summary Table

| # | Severity | File | Lines | Type | Description |
|---|---|---|---|---|---|
| 1 | ~~CRITICAL~~ **FIXED** | execute_pipe.c | 100-110, 137-138 | FD leak | `temp_redir` leaks stdin fd on partial dup failure |
| 2 | ~~CRITICAL~~ **FIXED** | exeute_cmd_utils.c | 63-66 | Crash | `ft_split` result not NULL-checked → deref NULL |
| 3 | ~~CRITICAL~~ **FIXED** | execute_cmd.c | 46-54 | Malloc fail | NULL `cmd_name` propagates to `safe_join(sub, NULL)` |
| 4 | ~~HIGH~~ **FIXED** | execute_pipe.c | 48-52, 77-81 | Design bug | Fork failure exits parent shell via `free_and_exit` |
| 5 | ~~HIGH~~ **FIXED** | exeute_cmd_utils.c | 23-28 | Logic error | `get_path` finds last PATH entry, not first |
| 6 | ~~MEDIUM~~ **FIXED** | handle_redirections.c | 89-90 | Inconsistency | `redirect_heredoc` return value not checked |
| 7 | ~~MEDIUM~~ **FIXED (via #4)** | execute_pipe.c | 149 | Inconsistency | `handle_left_pipe` pid_t return silently discarded |
| 8 | ~~LOW~~ **FIXED** | handle_redirections.c | 15-67 | Dead code | `node`/`shell` params passed but never used |
| 9 | ~~LOW~~ **FIXED** | execute_pipe.c + execution_utils.c | — | Redundancy | Two close wrappers with incompatible APIs — `secure_close` removed, replaced with plain `close()` |
| 10 | ~~LOW~~ **FIXED** | Multiple | — | Naming | Typos: `exeute`, `asssemple`, `absoulute` |
| 11 | ~~LOW~~ **FIXED** | execute_pipe.c | 129-130 | Style | Missing blank line before `exec_pipe` |
