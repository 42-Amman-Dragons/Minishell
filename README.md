*This project has been created as part of the 42 curriculum by hal-lawa, mabuqare.*

# Minishell 🔥🐉

## Description

This project is an educational project that requires to create a mini shell that mimics the 
bash behaviour from scratch.
The steps that we followed in this project are
- Tokanization : convert the row text inserted into meaningfull tokens.
- Building an Abstract syntax tree: this steps takes the tokens and give them more meaning by adding grammer in putting it structurally in an abstract syntax tree.
- Expander: the expander task is to expand the values of environment variables.
- Execute: this takes the abstract syntax tree, handle redirections, and execute command based in the grammer given in the tree.

## Instructions
In order to open this project in your devise follow these steps:
1. Clone the repository using git clone.  
```
git clone https://github.com/42-Amman-Dragons/Minishell.git
```
2. Run make to execute the mandatory part and make bonus to run the bonus part.

For mandatory:

```
make
```

For bonus:

```
make bonus
```

3. To use the interactive mode run:
```
./minishell
```
4. To use uniteractive mode run:
```
echo "<cmd that you want to execute>" | ./minishell
```
Have fun in exploring our dradons shell :D.

## Resources

- [GNU Readline](https://tiswww.case.edu/php/chet/readline/readline.html#index-rl_005fredisplay)

- [How the bash history works](https://www.abc.se/~torgny/bashhistory.html)

- [What are interactive and non-interactive shells in Linux](https://medium.com/@linuxrootroom/what-are-interactive-and-non-interactive-shells-in-linux-5f25ce19e537)

- [Signals in C language](https://www.geeksforgeeks.org/c/signals-c-language/)

- [Understanding Signals in the C Language: Harness the Power of Asynchronous Event Handling](http://medium.com/@razika28/signals-ad83f38f80b6)

- [termios(3)](https://man7.org/linux/man-pages/man3/termios.3.html)

- [What are Exit Codes in Linux?](https://itsfoss.com/linux-exit-codes/)

- [XV6-Shell-Implementation/shell.c (github)](https://github.com/ag6288/XV6-Shell-Implementation/blob/master/shell.c)

### AI Usage

- Cleaning the code and norminate.
- Fix some bugs.
- Demonistrate some critical bugs and concepts.
- Finding any special cases that we did not handle.
- Helping in planning (todo lists and features to go ...).
- Editing this README.
- Helping with memory leaks.