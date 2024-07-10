# Minishell

The Minishell project at 42 School is designed to give students a comprehensive understanding of how a Unix-like shell operates. This project challenges students to create their own simple shell, handling a variety of commands and functionalities similar to those found in standard Unix shells. Here are the main aspects of the Minishell project:

### Objectives
- **Understand Unix System Calls**: Gain practical experience with system calls like `fork()`, `execve()`, `wait()`, `pipe()`, and signal handling.
- **Develop a Functional Shell**: Build a command-line interface that can execute basic commands, handle piping, and manage processes.

### Key Requirements
Here are the key components and requirements of the Minishell project:

1. **Basic Shell Functions**: 
   - Implement a command line interface that accepts and executes commands.
   - Support built-in shell commands like `cd`, `echo`, `exit`, `env`, `setenv`, and `unsetenv`.

2. **Execution of Commands**:
   - Handle the execution of external commands using `execve`.
   - Manage process creation and termination using `fork`, `wait`, and `signal`.

3. **Pipes and Redirections**:
   - Implement support for pipes (`|`) to allow chaining commands.
   - Handle input (`<`) and output (`>`, `>>`) redirections.

4. **Environment Variables**:
   - Manage environment variables and their usage within the shell.
   - Allow exporting and unsetting environment variables.

5. **Signal Handling**:
   - Handle signals such as `Ctrl+C` and `Ctrl+D` gracefully, ensuring that the shell remains responsive.

6. **Error Handling**:
   - Provide meaningful error messages for invalid commands, missing files, or syntax errors.

7. **Robustness and Edge Cases**:
   - Ensure the shell handles unexpected input and edge cases without crashing.

### Learning Outcomes
- **In-depth Shell Knowledge**: Understand the inner workings of Unix-like shells, including command parsing, execution, and job control.
- **System Programming Proficiency**: Improve skills in C programming, focusing on systems programming concepts and practices.
- **Problem-Solving Abilities**: Develop the ability to tackle complex programming challenges involving process management, inter-process communication, and signal handling.

The Minishell project is a fundamental part of the 42 School curriculum, helping students build a strong foundation in systems programming and Unix concepts, which are essential for more advanced projects and real-world software development.
