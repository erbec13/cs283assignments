#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"
#include <errno.h>

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */

int build_cmd_buff(char* cmd_line, cmd_buff_t* cmd_buff) {
    int pointer = 0;
    int exec_pointer = 0;
    int arg_pointer = 0;
    int quote_mode = 0;
    int exec_started = 0;
    int exec_finished = 0;
    int arg_counter = 0;
    int arg_started = 0;
    int arg_finished = 0;
    (*cmd_buff)._cmd_buffer = malloc(sizeof(char) * EXE_MAX);
    if ((*cmd_buff)._cmd_buffer == NULL) {
        return ERR_MEMORY;
    }
    while (*(cmd_line + pointer) != '\0') {
        if (exec_started == 0) {
            if (*(cmd_line + pointer) == ' ') {
                pointer++;
                continue;
            } else {
                *((*cmd_buff)._cmd_buffer + exec_pointer) = *(cmd_line + pointer);
                exec_pointer++;
                exec_started = 1;
            }
        } else if (exec_finished == 0) {
            if (exec_pointer >= EXE_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            } else if (*(cmd_line + pointer) == ' ') {
                *((*cmd_buff)._cmd_buffer + exec_pointer) = '\0';
                (*cmd_buff).argv[0] = (*cmd_buff)._cmd_buffer;
                exec_finished = 1;
                arg_counter++;
            } else {
                *((*cmd_buff)._cmd_buffer + exec_pointer) = *(cmd_line + pointer);
                exec_pointer++;
            }
        } else if (arg_started == 0) {
            if (*(cmd_line + pointer) == ' ') {
                pointer++;
                continue;
            } else if (arg_counter >= CMD_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            } else {
                (*cmd_buff).argv[arg_counter] = malloc(sizeof(char) * ARG_MAX);
                if ((*cmd_buff).argv[arg_counter] == NULL) {
                    return ERR_MEMORY;
                }

                if (*(cmd_line + pointer) == '\"') {
                    if (quote_mode == 0) {
                        quote_mode = 1;
                    } else if (quote_mode == 1) {
                        quote_mode = 0;
                    }
                    pointer++;
                    arg_started = 1;
                    continue;
                }

                *((*cmd_buff).argv[arg_counter] + arg_pointer) = *(cmd_line + pointer);
                arg_pointer++;
                arg_started = 1;
            }
        } else if (arg_finished == 0) {
            if (arg_pointer >= ARG_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            } else if (*(cmd_line + pointer) == ' ' && quote_mode == 0) {
                *((*cmd_buff).argv[arg_counter] + arg_pointer) = '\0';
                arg_started = 0;
                arg_counter++;
                arg_pointer = 0;
            } else if (*(cmd_line + pointer) == '\"') {
                if (quote_mode == 0) {
                    quote_mode = 1;
                } else if (quote_mode == 1) {
                    quote_mode = 0;
                }
                arg_pointer++;
            } else {
                *((*cmd_buff).argv[arg_counter] + arg_pointer) = *(cmd_line + pointer);
                arg_pointer++;
            }
        }
        pointer++;
    }
    (*cmd_buff).argv[0] = (*cmd_buff)._cmd_buffer;

    if (*(cmd_line + pointer - 1) != ' ') {
        arg_counter++;
    }
    (*cmd_buff).argv[arg_counter] = NULL;

    (*cmd_buff).argc = arg_counter;
    
    return OK;
}

int exec_cmd(cmd_buff_t *cmd) {
    int f_result;
    int c_result;

    f_result = fork();

    if (f_result < 0){
        return ERR_EXEC_CMD;
    }

    if (f_result == 0){
        int fork_rc;

        fork_rc = execvp((*cmd).argv[0], (*cmd).argv);
        if (fork_rc < 0){
            return ERR_EXEC_CMD;
        }
    } else {
        wait(&c_result);
    }

    return OK;
}

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(ARG_MAX * sizeof(char));
    if (cmd_buff == NULL) {
        return ERR_MEMORY;
    }
    int rc = 0;
    cmd_buff_t cmd;

    // TODO IMPLEMENT MAIN LOOP

    while(1){
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            return OK;
        }
        
        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';

        //IMPLEMENT THE REST OF THE REQUIREMENTS

        rc = build_cmd_buff(cmd_buff, &cmd);

        if (rc == ERR_TOO_MANY_COMMANDS) {
            printf("\n");
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            return rc;
        }

        if (rc == ERR_MEMORY) {
            printf("\n");
            return rc;
        }

        if (rc == ERR_CMD_OR_ARGS_TOO_BIG) {
            printf("\n");
            return rc;
        }

        if (*(cmd._cmd_buffer + 0) == '\0') {
            printf(CMD_WARN_NO_CMD);
        } else {
            if (strcmp("exit", cmd._cmd_buffer) == 0) {
                return OK;
            } else if (strcmp("dragon", cmd._cmd_buffer) == 0) {
                print_dragon();
                continue;
            } else if (strcmp("cd", cmd._cmd_buffer) == 0) {
                if (cmd.argc > 0) {
                    chdir(cmd.argv[1]);
                }
                continue;
            }
        }

        rc = exec_cmd(&cmd);
        if (rc == ERR_EXEC_CMD) {
            return ERR_EXEC_CMD;
        }
    }

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
}