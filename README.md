*This project has been created as part of the 42 curriculum by hal-lawa, mabuqare.*

# Minishell

## Description

This project is part of the 42 curriculum. The goal is to build a mini shell that mimics
bash behaviour from scratch.

The pipeline follows these stages:
- **Tokenization**: converts raw input text into meaningful tokens.
- **Parsing**: takes the tokens and builds an Abstract Syntax Tree (AST) that encodes the grammatical structure of the command.
- **Expansion**: expands environment variables, handles quoting rules, and performs glob expansion (bonus).
- **Execution**: traverses the AST, applies redirections, and executes commands accordingly.

## Instructions

Clone the repository:
```
git clone https://github.com/42-Amman-Dragons/Minishell.git
cd Minishell
```

Build the mandatory part:
```
make
```

Build the bonus part (includes wildcards, `&&`, `||`, and subshells):
```
make bonus
```

### Usage

Interactive mode:
```
./minishell
```

Non-interactive mode:
```
echo "<command>" | ./minishell
```

## Resources

- [GNU Readline](https://tiswww.case.edu/php/chet/readline/readline.html#index-rl_005fredisplay)
- [How bash history works](https://www.abc.se/~torgny/bashhistory.html)
- [Interactive and non-interactive shells in Linux](https://medium.com/@linuxrootroom/what-are-interactive-and-non-interactive-shells-in-linux-5f25ce19e537)
- [Signals in C](https://www.geeksforgeeks.org/c/signals-c-language/)
- [Understanding signals in C](http://medium.com/@razika28/signals-ad83f38f80b6)
- [termios(3)](https://man7.org/linux/man-pages/man3/termios.3.html)
- [Exit codes in Linux](https://itsfoss.com/linux-exit-codes/)
- [XV6 shell implementation](https://github.com/ag6288/XV6-Shell-Implementation/blob/master/shell.c)

### AI Usage

- Code cleanup and norm compliance.
- Bug fixes.
- Demonstrating critical bugs and edge cases.
- Finding unhandled special cases.
- Planning assistance.
- Memory leak investigation.
