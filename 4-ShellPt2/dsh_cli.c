#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "dshlib.h"

/*
 * Implement your main function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.  Since we want fgets to also handle
 * end of file so we can run this headless for testing we need to check
 * the return code of fgets.  I have provided an example below of how
 * to do this assuming you are storing user input inside of the cmd_buff
 * variable.
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
 *
 *   Expected output:
 *
 *      CMD_OK_HEADER      if the command parses properly. You will
 *                         follow this by the command details
 *
 *      CMD_WARN_NO_CMD    if the user entered a blank command
 *      CMD_ERR_PIPE_LIMIT if the user entered too many commands using
 *                         the pipe feature, e.g., cmd1 | cmd2 | ... |
 *
 *  See the provided test cases for output expectations.
 */

int main()
{
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
    int rc = 0;
    command_list_t clist;

    while(1){
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL){
            printf("\n");
            exit(OK);
        }
        
        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';

        //IMPLEMENT THE REST OF THE REQUIREMENTS

        rc = build_cmd_list(cmd_buff, &clist);

        if (rc == ERR_TOO_MANY_COMMANDS) {
            printf("\n");
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            continue;
        }

        if (clist.num == 0) {
            printf(CMD_WARN_NO_CMD);
        } else {
            if (strcmp("exit", clist.commands[0].exe) == 0) {
                exit(OK);
            } else if (strcmp("dragon", clist.commands[0].exe) == 0) {
                print_dragon();
                continue;
            } else if (strcmp("cd", clist.commands[0].exe) == 0) {
                if (clist.commands[0].args[0] != '\0') {
                    int pointer = 0;
                    while (clist.commands[0].args[pointer] != '\0' && clist.commands[0].args[pointer] != ' ') {
                        pointer++;
                    }
                    clist.commands[0].args[pointer] = '\0';
                    char* directory_name = malloc(sizeof(char) * (pointer + 1));
                    strcpy(directory_name, clist.commands[0].args);
                    chdir(directory_name);
                    free(directory_name);
                }
                continue;
            }
            printf("\n");
            printf(CMD_OK_HEADER, clist.num);
        }

        for (int i = 0; i < clist.num; i++) {
            printf("<%d> %s", i+1, clist.commands[i].exe);
            if (clist.commands[i].args[0] != '\0') {
                printf(" [%s]", clist.commands[i].args);
            }
            printf("\n");
        }
    }
}