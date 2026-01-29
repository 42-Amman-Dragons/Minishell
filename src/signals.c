#include "minishell.h"

extern int SIGNUM;

void ctrl_c_handler(int signalNumber)
{
   SIGNUM = signalNumber;
   rl_replace_line("\0", 1);
   printf("^C");
   rl_redisplay();
   rl_on_new_line();
   ioctl(STDIN_FILENO, TIOCSTI, "\n"); // this means faking an input of new line (this mimics passing a return key (ENTER) to readline)
}

void ctrl_d_handler(int signalNumber)
{
   SIGNUM = signalNumber;
   exit(0);
}

// the ctrl_backslash function does nothing and ignores the signal.
// it is used because the macro functions like SIG_IGN is forbidden
void ctrl_backslash(int signalNumber)
{
   SIGNUM = signalNumber;
}
