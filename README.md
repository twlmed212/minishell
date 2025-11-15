# Minishell — Roadmap & File Map

This README gathers the project todo list and a file-by-file description of what each source/header should contain. Use this as the single source of truth while finishing the mini-shell implementation.

## Project todo list

- [x] Scan repository — list source files and confirm implemented parts.
- [x] Parsing — tokenization, quote handling, heredoc and basic parsing implemented in `src/parsing`.
- [ ] REPL loop & prompt — implement main loop using `readline`, handle EOF (Ctrl-D), show prompt.
- [ ] History support — integrate `add_history`, `rl_on_new_line`, `rl_redisplay`; ensure heredoc lines are not added.
- [ ] Signal handling — provide a single global `int` to store received signal number; set up `sigaction` handlers to replicate bash behavior for Ctrl-C, Ctrl-\\ and Ctrl-D as required.
- [ ] Variable expansion & special tokens — support `$VAR`, `$?` and ensure expansion respects single/double quotes.
- [ ] Redirections — implement `<`, `>`, `>>`, `<<` (heredoc). Correctly open files and `dup2` fds in child processes.
- [ ] Pipes — implement pipelines (`|`) by creating pipes between processes and wiring stdin/stdout.
- [ ] Execution engine — distinguish builtins vs external commands; perform PATH search and `execve` in children; collect exit status for `$?`.
- [ ] Builtins integration — wire builtins in `src/builtins`; run parent-affecting builtins (`cd`, `exit`, `export`) in the parent process when required.
- [ ] Testing & validation — add manual tests / scripts to verify parsing, redirections, pipes, expansions, signals and builtins.
- [ ] Quality gates — build, lint, and run tests; check for memory leaks in project code (valgrind or code review).
- [ ] Documentation — keep this README updated with build/run instructions and known limitations.

## High-level contract / success criteria

- Prompt displays while waiting for input.
- Commands run via PATH lookup or direct path.
- Pipes and redirections work together.
- Builtins behave like specified; `cd`/`exit`/`export` alter the current shell when appropriate.
- Signal handling mimics the subject's requirements.

## Files and what each should contain

Root files

- `main.c`
  - Contains the program entrypoint and the main loop (REPL).
  - Initialize global state (including the single `int` global for signals), install signal handlers, and call the parser/executor pipeline.
  - Use `readline` to read user input, call parser, add to history (where appropriate), and call the execution engine.

- `Makefile`
  - Build rules to compile sources into the `minishell` executable.
  - Targets: `all`, `clean`, `fclean`, `re`, and `test` (optional).

- `minishell` (executable produced by the Makefile)
  - The compiled shell binary.

Include

- `include/minishell.h`
  - Public types, prototypes and small shared macros.
  - Declare the single global `int` used to store the last received signal (only the signal number).
  - Declarations for parser, executor, builtin APIs, and utilities.

Source tree (`src/`)

- `src/parsing/pars.c`
  - Top-level parsing orchestration: receives raw input, tokenizes, builds AST or command structures, handles quote removal as required.
  - Should defer expansions (or perform them in the correct stage) and detect redirections, heredoc delimiters and pipelines.

- `src/parsing/token.c`
  - Tokenizer: split input into tokens (words, operators like `|`, `<`, `>`, `<<`, `>>`).
  - Track quote context so tokens inside single quotes are preserved literally; double quotes allow `$` expansion.

- `src/parsing/syntax.c`
  - Syntax validation for tokens: invalid sequences, unmatched quotes, misplaced redirections, empty commands in pipes, etc.

- `src/parsing/herdoc.c`
  - Heredoc implementation: read lines until the delimiter is seen.
  - Use a temporary file or pipe to feed heredoc contents to the command stdin.
  - Ensure heredoc lines are not added to `readline` history.

- `src/parsing/rid.c`
  - Utilities for removing/cleaning tokens or preparing command argument arrays.
  - May include trimming quotes and preparing argv for the executor.

- `src/execution/exuction.c` (note: file currently named `exuction.c` — consider renaming to `execution.c`)
  - Core execution engine.
  - Build pipelines of processes, create pipes, `fork`, `dup2` to set stdin/stdout/stderr for each child according to redirections and pipes.
  - Distinguish builtins and external commands. For external commands: perform PATH search (loop `PATH` entries and use `access(2)`), `execve` in the child.
  - Parent process collects children statuses (update `$?`/last status global) and handles closing file descriptors.
  - Handle file redirections (`open` with proper flags and modes) and error reporting via `perror`/`strerror`.

- `src/builtins/builtin_echo.c`
  - Implement `echo` with `-n` option (suppress trailing newline).
  - Return status conventions (usually 0).

- `src/builtins/builtin_cd.c`
  - Implement `cd` for relative and absolute paths only.
  - Update the environment (e.g. `PWD`, `OLDPWD`) in the parent process.
  - Return non-zero on error and report via `perror`.

- `src/builtins/builtin_pwd.c` (if present or to be added)
  - Implement `pwd` (no options). Print current working directory using `getcwd`.

- `src/builtins/builtin_export.c`
  - Implement `export` without options: add/update environment variables in parent environment.
  - When called without args, print exported variables (format similar to `declare -x VAR=\"value\"`).

- `src/builtins/builtin_unset.c`
  - Implement `unset` to remove variables from the environment.

- `src/builtins/builtin_env.c`
  - Implement `env` to print current environment variables (no options or args allowed by subject).

- `src/builtins/builtin_exit.c`
  - Implement `exit` with no options: exit the shell (in parent process), optionally accept a numeric argument to set exit code.

- `src/utils/utilis.c`
  - Helper utilities used across parser/executor/builtins: string helpers, array helpers, environment helpers, error helpers, freeing utilities.
  - Keep these functions small, well-documented, and testable.

Other notes & constraints

- Allowed functions: only the functions listed in the subject are permitted (e.g. `readline`, `fork`, `execve`, `pipe`, `dup2`, `open`, etc.). Use `libft` helpers where appropriate.
- Global variables: only one global variable is allowed for signals (an `int`). Do not use global structs to hold shell state.
- Memory leaks: `readline` may leak; your own code should avoid leaking memory for the features you implement.

How to use this README

- Keep the TODO section updated as you complete tasks.
- For each file, when you implement or modify behavior, update the short description above to reflect the current state.

If you want, I can now:

1. Inspect `main.c` and `src/execution/exuction.c` and provide a concrete patch plan to implement the REPL and wiring to the executor.
2. Start implementing the REPL + signal handlers and create small tests.

Tell me which of these you want me to do next.

---
Generated on: 2025-11-14

## REPL & Signal handler implementation (instructions)

This section contains a concrete, copy-pasteable plan and example code snippets to implement the interactive REPL prompt and proper signal handling while following the project constraints (only one global used for signal number).

Design notes

- Use exactly one small global variable to indicate a received signal. Prefer `volatile sig_atomic_t g_signal = 0;` (safe for signal handlers), or `int g_signal` if you strictly need `int`. The variable must only store the signal number (or 0 when none).
- Install signal handlers in the parent (interactive shell). Set child processes to default behavior before `execve` so external programs receive signals normally.
- Avoid complex logic in signal handlers: set the global flag, perform minimal async-signal-safe writes (like a newline), and use `readline` helpers from the main loop to redraw the prompt when safe.

Suggested files & declarations

- `include/minishell.h` (add prototypes and extern):

  // at top-level of header
  #include <signal.h>
  extern volatile sig_atomic_t g_signal; // single global used by handlers
  void init_signals_parent(void);
  void set_signals_default(void);

- Create `src/signals/signals.c` (new file) and define the global plus handlers.

  #include "../include/minishell.h"
  #include <unistd.h>
  #include <signal.h>
  #include <readline/readline.h>
  #include <readline/history.h>

  volatile sig_atomic_t g_signal = 0;

  static void sigint_handler(int signum)
  {
      g_signal = signum;            // record signal number
      write(1, "\n", 1);         // async-signal-safe: print newline
      /* Do NOT call non-async-signal-safe funcs here except write().
         We'll let the main loop call readline helpers to reset the prompt. */
  }

  void init_signals_parent(void)
  {
      struct sigaction sa_int;
      struct sigaction sa_quit;

      sigemptyset(&sa_int.sa_mask);
      sa_int.sa_flags = SA_RESTART;
      sa_int.sa_handler = sigint_handler;
      sigaction(SIGINT, &sa_int, NULL);

      /* In interactive parent ignore SIGQUIT (Ctrl-\) */
      sigemptyset(&sa_quit.sa_mask);
      sa_quit.sa_flags = SA_RESTART;
      sa_quit.sa_handler = SIG_IGN;
      sigaction(SIGQUIT, &sa_quit, NULL);
  }

  void set_signals_default(void)
  {
      signal(SIGINT, SIG_DFL);
      signal(SIGQUIT, SIG_DFL);
  }

Where to call these

- In `main.c` before entering the main loop, call `init_signals_parent();` so the interactive shell has the proper handlers set.
- When forking a child to execute external commands (or running a builtin in a child), call `set_signals_default();` in the child before `execve()` so the child responds to signals normally.

Integrating with `readline` prompt

- Because the signal handler only sets `g_signal` and writes a newline, the main loop should detect `g_signal` immediately after `readline` returns and then call readline helpers to clear/redisplay the prompt. Example in `main.c`:

  while (1) {
      line = readline("minishell$ ");
      if (line == NULL) { /* EOF (Ctrl-D) */ break; }

      if (g_signal == SIGINT) {
          /* User pressed Ctrl-C while waiting for input. Reset handler state for UI. */
          g_signal = 0;
          rl_on_new_line();
          rl_replace_line("", 0);
          rl_redisplay();
          free(line);
          continue; /* show prompt again */
      }

      if (*line) add_history(line);
      ... /* existing parsing/execution logic */
  }

Notes on readline helpers in handlers

- Many real minishell projects call `rl_on_new_line`, `rl_replace_line`, and `rl_redisplay` directly from the signal handler — this often works in practice but is not strictly async-signal-safe. The recommended safe approach above is to only perform an async-signal-safe write in the handler and perform `readline` UI calls from the main loop after checking `g_signal`.

Behavior specifics to match the subject

- Ctrl-C in interactive mode: display a new prompt on a new line (the code above prints newline in handler and redisplays prompt in loop).
- Ctrl-D in interactive mode: `readline` returns `NULL`. Print `exit\n` and exit the loop (this is already present in `main.c`).
- Ctrl-\ (SIGQUIT): do nothing in the interactive parent — set to `SIG_IGN` as shown.
- In child processes: revert to defaults so `^\` produces a core dump message (or default behavior) and `^C` interrupts the child.

Heradoc note

- During heredoc reading, signal handling is special: you typically want Ctrl-C to abort the heredoc and return control to the prompt. You can reuse the same `g_signal` flag: when heredoc reader detects `g_signal == SIGINT`, abort the heredoc read, cleanup temporary resources, and return to prompt without adding heredoc lines to history.

Testing checklist

- Start shell and press Ctrl-C at prompt: new empty prompt on new line.
- Start shell and press Ctrl-D: shell prints `exit` and quits.
- Run `sleep 10` (external program) and press Ctrl-C: child terminates and prompt returns.
- Run `cat` and send Ctrl-\: child shows default behavior (not ignored by parent settings) if appropriate.

If you want, I can implement these changes for you (create `src/signals/signals.c`, update header and `main.c` calls). For now I only added instructions and example code here as requested.
