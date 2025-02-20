#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

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
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    //Move the starting index to be the first index that doesn't have a space
    int startOfCommandLine = 0;
    while (*(cmd_line + startOfCommandLine) == SPACE_CHAR) {
        startOfCommandLine++;
    }

    if (*(cmd_line + startOfCommandLine) == '\0') {
        (*clist).num = 0;
        return OK;
    }

    //Count the number of pipes/commands
    int commandLinePointerIndex = startOfCommandLine;
    int commandCounter = 1;
    while (*(cmd_line + commandLinePointerIndex) != '\0') {
        if (*(cmd_line + commandLinePointerIndex) == PIPE_CHAR) {
            commandCounter++;
        }
        if (commandCounter > CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }
        commandLinePointerIndex++;
    }
    (*clist).num = commandCounter;

    commandLinePointerIndex = startOfCommandLine;

    for (int i = 0; i < commandCounter; i++) {
        int thisCommandExecPointerIndex = 0;
        int thisCommandArgPointerIndex = 0;
        while (*(cmd_line + commandLinePointerIndex) != PIPE_CHAR && *(cmd_line + commandLinePointerIndex) != '\0' && *(cmd_line + commandLinePointerIndex) != SPACE_CHAR) {
            ((*clist).commands[i]).exe[thisCommandExecPointerIndex] = *(cmd_line + commandLinePointerIndex);
            thisCommandExecPointerIndex++;
            commandLinePointerIndex++;
            if (thisCommandExecPointerIndex > EXE_MAX-1) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
        }

        ((*clist).commands[i]).exe[thisCommandExecPointerIndex] = '\0';
        while (*(cmd_line + commandLinePointerIndex) == SPACE_CHAR) {
            commandLinePointerIndex++;
        }

        while (*(cmd_line + commandLinePointerIndex) != PIPE_CHAR && *(cmd_line + commandLinePointerIndex) != '\0') {
            ((*clist).commands[i]).args[thisCommandArgPointerIndex] = *(cmd_line + commandLinePointerIndex);
            thisCommandArgPointerIndex++;
            commandLinePointerIndex++;
            if (thisCommandArgPointerIndex > ARG_MAX-1) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
        }

        ((*clist).commands[i]).args[thisCommandArgPointerIndex] = '\0';
        while (*(cmd_line + commandLinePointerIndex) == SPACE_CHAR) {
            commandLinePointerIndex++;
        }
        if (*(cmd_line + commandLinePointerIndex) == PIPE_CHAR) {
            commandLinePointerIndex++;
            while (*(cmd_line + commandLinePointerIndex) == SPACE_CHAR) {
                commandLinePointerIndex++;
            }
            continue;
        }
        if (*(cmd_line + commandLinePointerIndex) == '\0') {
            break;
        }
    }

    return OK;
}