1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  _The fgets() operation is good for this application because it reads line by line, which is how a shell operates. It looks at the line entered and then the shell runs using that one line as input. We also don't need to go back to reading any previous lines, so we don't need an operation that does anything more than retrieve the next line, which is what fgets does._

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  _The malloc() operation is used to allocate memory so that the memory will exist outside of the execution of the block it's in. If we want to access the cmd_buff after the main function finishes, we can not by using a fixed-size array._


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  _If we include spaces into our command arrays, our command execution handler will have a hard time discerning commands because it will have to account for all kinds of whitespaces. Our shell should be able to take a command with spaces and treat it as if it doesn't have spaces._

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  _Redirection is when the streams associated with STDIN, STDOUT, or STDERR are sent somewhere other than their expected location. One use of redirection would be to redirect STDERR into a log file that keeps track of errors. Another use of redirection would be to redirect the STDOUT into a log file that keeps track of all the output. With both of these examples, we would need some way of eliminating the log files when we are done with them. The last use of redirect could be to accept input from a file other than STDIN. This would be used for piping._

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  _The main difference between piping and redirection is that piping takes the output of a process and uses it as the input of another process, whereas redirection involves moving around streams from a file. Piping also follows something known as the synchronization effect where the process being piped into will not run if the process being piped out of has no output._

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  _It is important to separate STDERR and STDOUT because we might not use input handling to be able to decipher an error message, so if one process reads the output of another, and the output is an error, the process will not work correctly._

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  _Our shell should not stop running, but instead it should print the error message after the output has been printed. We should display the STDOUT and then the STDERR so that I user may be able to see where a process failed when using print commands to debug a process._