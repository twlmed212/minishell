*This project has been created as part of the 42 curriculum by mtawil, abmoudni.*

# minishell

## Description

**minishell** is a small Unix shell written in C, modeled on **bash**.
The goal of the project is to understand what really happens between typing a command and seeing its result: reading input, parsing it, creating processes, connecting them with pipes, redirecting file descriptors, and handling signals.

The shell shows a prompt, reads a command line, and runs it, either as a built-in command or as an external program found through `PATH`.

### Features

- Interactive prompt with **command history** (GNU readline)
- Runs executables from `PATH`, or from a relative or absolute path
- **Quotes**
  - `'single quotes'` prevent any interpretation
  - `"double quotes"` prevent interpretation except for `$`
- **Redirections**
  - `<` input
  - `>` output
  - `>>` append
  - `<<` heredoc with a delimiter
- **Pipes**: `cmd1 | cmd2 | cmd3`
- **Environment variables**: `$VAR` and `$?` (exit status of the last pipeline)
- **Signals**, same behavior as bash:
  - `Ctrl-C` shows a new prompt
  - `Ctrl-D` exits the shell
  - `Ctrl-\` does nothing
- **Built-in commands**

| Built-in | Behavior |
|---|---|
| `echo` | Prints arguments, supports `-n` |
| `cd` | Changes directory (relative or absolute path) |
| `pwd` | Prints the current directory |
| `export` | Sets environment variables |
| `unset` | Removes environment variables |
| `env` | Prints the environment |
| `exit` | Exits the shell with an optional status |

### How it works

Each command line goes through four steps:

```
input ──► tokenizer ──► parser ──► expander ──► executor
```

1. **Tokenizer**: splits the line into words and operators (`|`, `<`, `>`, `<<`, `>>`), respecting quotes.
2. **Parser**: checks the syntax (unclosed quotes, a pipe with nothing after it, etc.) and builds a list of commands with their arguments and redirections.
3. **Expander**: replaces `$VAR` and `$?` with their values and removes the quotes.
4. **Executor**: runs built-ins in the shell process when possible. Otherwise it forks one child per command, connects the commands with `pipe`, applies redirections with `dup2`, finds the program in `PATH`, runs it with `execve`, and waits for every child to collect the exit status.

Signal handling uses a single global variable that only stores the received signal number, as the subject requires.

## Instructions

### Requirements

- `cc` and `make`
- The GNU **readline** library
  - Debian / Ubuntu: `sudo apt install libreadline-dev`
  - macOS: `brew install readline`. You may need the readline include and lib paths; there is a commented line for this in the `Makefile`.

### Build and run

```bash
git clone https://github.com/twlmed212/minishell.git
cd minishell
make
./minishell
```

Other `Makefile` rules: `make clean`, `make fclean`, `make re`, `make run`.

### Run with Docker

A Docker setup with Ubuntu, readline, and Valgrind is included. It is useful on macOS or for checking memory leaks.

```bash
./docker.sh build      # build the image
./docker.sh run        # run minishell in a container
./docker.sh valgrind   # run minishell under Valgrind
./docker.sh dev        # open a bash shell in the container to work on the code
```

On Linux, `./run.sh` runs minishell under Valgrind with `r.supp`, which hides the known readline leaks.

### Examples

```bash
minishell$ echo "Hello $USER"
minishell$ ls -l | grep .c | wc -l
minishell$ cat < input.txt > output.txt
minishell$ echo "new line" >> output.txt
minishell$ cat << EOF
> first line
> second line
> EOF
minishell$ export NAME=42
minishell$ echo $NAME
minishell$ ls missing_file
minishell$ echo $?
```

### Project structure

```
minishell/
├── Makefile
├── include/minishell.h     # Structures and prototypes
├── libft/                  # Our own C library
└── src/
    ├── main.c              # Prompt loop
    ├── tokenizer/          # Splits input into tokens
    ├── parsing/            # Syntax checks, quotes, expansion, command list
    ├── execution/          # Executor, pipes, redirections, heredoc, PATH lookup
    ├── builtins/           # echo, cd, pwd, export, unset, env, exit
    ├── signals/            # Ctrl-C / Ctrl-D / Ctrl-\ handling
    ├── utils/              # Environment helpers
    └── cleaner/            # Memory cleanup
```

## Resources

### References

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html): the reference for expected behavior
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html)
- Manual pages: `man 2 fork`, `man 2 execve`, `man 2 pipe`, `man 2 dup2`, `man 2 waitpid`, `man 2 sigaction`, `man 3 readline`
- [Write a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/) by Stephen Brennan: a short introduction to the shell loop
- *The Linux Programming Interface* by Michael Kerrisk: processes, pipes, signals, and file descriptors

### AI usage

AI was used to help write and organize this README. The shell itself, including the parsing, execution, and signal handling, was designed and written by the authors, and every part was tested and reviewed between us.

## Authors

- **Mohamed Tawil** (`mtawil`): [@twlmed212](https://github.com/twlmed212)
- **Abdessamad Moudnibe** (`abmoudni`): [@abmoudni](https://github.com/abmoudni)
