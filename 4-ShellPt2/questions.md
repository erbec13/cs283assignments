1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  _Using fork allows us to run the command execution as a child process so that if it fails, the entire program does not fail._

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  _If the fork() system call fails, it returns a negative number which my program catches with a conditional. It then returns the appropriate value to indicate such an error has occurred._

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  _The execvp() command sends the command name to the kernel where the kernel runs the command using the parsed arguments in the provided array argv._

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  _The parent process can not be allowed to finish running because if it ended then the child process would not run. It would become a zombie process. We have to wait so that the child process finishes before we continue our program._

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  _WEXITSTATUS() gives the error number of the exec command if there is any error, or it gives 0 if there is no error._

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  _My implementation uses a boolean type of data (0 or 1) to determine if we are in a 'quote mode'. Quote mode is 0 if we have found an even amount of quotes (we are outside of a quotation). Quote mode is 1 if we have found an odd amount of quotes (we are inside of a quotation)._

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  _I decided to completely redo my parsing logic on this assignment for two reasons. First of all, my logic on the prior assignment was a little messy and hard to follow. Second of all, I felt that simply updating variable names would lead to errors. On this assignment, I use a pointer to look at the next character in the command line, and then use a series of nested if elif statements to decide what to do with the character read._

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  _A signal is something that gets sent to perform some task in the middle of a process. The most common example of a signal is using Ctrl C during a program to ask the program to terminate. Signals are the form of IPC usually used in Unix, Unix-like, and other POSIX-compliant operating systems._

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  _SIGINT is used to ask a program to stop, however the program can choose to handle this 'ask' by ignoring it or following through with it. There is no clean up involved. SIGTERM is used to also ask a program to stop, however this signal will perform the necessary cleanup for the program. SIGKILL will forcefully stop the program and perform the proper cleanup._

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  _SIGSTOP is used to stop program so that it may be resumed later. It can not be caught or ignored like SIGINT because the kernel handles the catching - not the user._