#include "minishell.h"

extern int	SIGNUM;

void	ctrl_c_handler(int signalNumber)
{
	SIGNUM = signalNumber;
   rl_on_new_line();
   rl_redisplay();
   printf("^C\n");
   rl_redisplay();
   rl_on_new_line();
   rl_redisplay();
}

// the ctrl_backslash function does nothing and ignores the signal.
// it is used because the macro functions like SIG_IGN is forbidden
void	ctrl_backslash(int signalNumber)
{
	SIGNUM = signalNumber;
   rl_on_new_line();
   rl_redisplay();
}
