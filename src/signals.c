#include "minishell.h"

int SIGNUM = 0;

void ctrl_c_handler(int signalNumber) 
{
   SIGNUM = signalNumber;
   printf("\n");
   rl_on_new_line();
   rl_redisplay();
}

void ctrl_d_handler(int signalNumber) 
{
   SIGNUM = signalNumber;
   exit(0);
}

void ctrl_backslash(int signalNumber) 
{
   SIGNUM = signalNumber;
   rl_on_new_line();
   rl_redisplay();
}
