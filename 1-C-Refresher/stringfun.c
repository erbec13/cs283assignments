#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
int reverse_words(char *, int);
int word_print(char *, int, int);
int search_and_replace(char*, int, char*, char*);

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions

    int indexFlag;

    for (int i = 0; i < len; i++) {
        //check to see if the string has any characters left and then replace the remaining spots with '.'s
        if (*(user_str + i) == '\0') {
            for (int j = i; j < len; j++) {
                *(buff + j) = '.';
            }
            indexFlag = i;      //bookmark the index that the for loop stopped at
            break;
        }

        //check to see if the string will run beyond the allotted space
        if ((i == (len - 1)) && (*(user_str + len) != '\0')) {
            return -1;
        }

        //convert all tab whitespaces into space whitespaces
        if (*(user_str + i) == '\t') {
            *(user_str + i) = ' ';
        }

        //combine all the neighboring whitespaces together by checking if the next character after a space is also a space
        if (*(user_str + i) == ' ') {
            //keep checking to find triples, quadruples, etc
            while (*(user_str + i + 1) == ' ') {
                for (int j = i; j < len; j++) {
                    if (*(user_str + j) == '\0') {
                        break;
                    } else {
                        *(user_str + j) = *(user_str + j + 1);      //move every character to the left by one to eliminate that extra space
                    }
                }
            }
        }

        *(buff + i) = *(user_str + i);      //add the character to the buffer
    }
    return indexFlag; //return the size of the buffer without the periods 
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int user_str_len){
    int indexFlag;
    
    //check to see where the first non-space/punctuation character is
    for (int i = 0; i < user_str_len; i++) {
        if ((*(buff + i) == ' ') || (*(buff + i) == '.')) {
            if (i == (user_str_len - 1)) {
                return 0;       //if all the indices are filled with spaces and periods then there are 0 words
            }
        } else {
            indexFlag = i;      //bookmark the index where the first word starts
            break;
        }
    }

    int wordCounter = 1;        //initialize a word counter

    //search the rest of the buffer and consider every space to be a separater for a new word
    for (int i = indexFlag; i < len; i++) {
        if ((*(buff + i) == ' ') && (i != (len - 1))) {
            if (*(buff + i + 1) != '.') {
                wordCounter += 1;       //increase the word counter
            }
        }
    }

    return wordCounter;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int reverse_words(char *buff, int user_str_len) {

    printf("Reversed String: ");

    //display every character in reverse order
    for (int i = 1; i <= user_str_len; i++) {
        printf("%c", *(buff + user_str_len - i));
    }
    printf("\n");

    return 0;
}

int word_print(char *buff, int len, int user_str_len) {

    int j = 0;

    int letterCount;

    //get the word count
    int wordCount = count_words(buff, len, user_str_len);
    if (wordCount < 0) {
        return -1;
    }

    printf("Word Print\n----------\n");

    for (int i = 1; i <= wordCount; i++) {
        //for each word print a numbering "bullet point"
        printf("%d. ", i);

        //find the index where the first word starts
        while (*(buff + j) == ' ') {
            j++;
        }
        letterCount = 0;

        //display each character from the "first word index" onwards until a space or period is found, or until the byte limit is reached
        while ((*(buff + j) != ' ') && (j < len) && (*(buff + j) != '.')) {
            printf("%c", *(buff + j));
            j++;
            letterCount++;      //keep track of the letter count
        }
        printf(" (%d)\n", letterCount);     //display the letter count
    }

    return 0;
}

int search_and_replace(char *buff, int len, char *target, char *replacement) {
    int j = 0;
    int indexFlag;
    int keepGoingIndex;
    int targetFound = 0;

    //for each character in the buffer
    for (int i = 0; i < len; i++) {
        //check if the buffer character matches the first target character
        if (*(buff + i) == *(target + j)) {
            if (*(target + j + 1) == '\0') {
                //if the target has been completely found turn on a flag, identify the index for the start and stop of the target within the buffer
                targetFound = 1;
                indexFlag = i - j;
                keepGoingIndex = i;
                break;
            } else {
                //if the target hasn't been completely found but some amount of characters has matched so far then check the next character
                j++;
            }
        } else {
            j = 0;
        }
    }

    //replace the target if the entire target was found
    if (targetFound == 1) {
        int k = 0;
        printf("Modified String: ");

        //display the buffer up until the target's first index in buffer
        for (int i = 0; i < indexFlag; i++) {
            printf("%c", *(buff + i));
        }

        //keep displaying replacement until it ends or until the buffer space runs out
        while ((*(replacement + k) != '\0') && (indexFlag < len)) {
            printf("%c", *(replacement + k));
            k++;
            //if capacity has been reached then stop displaying characters
            if ((k + indexFlag) == len) {
                printf("\n");
                return 0;
            }
        }

        //display the rest of the buffer after the target
        for (int i = 1; i < (len - indexFlag); i++) {
            //if the program hits a period then consider that the end
            if (*(buff + keepGoingIndex + i) == '.') {
                printf("\n");
                return 0;
            }
            printf("%c", *(buff + keepGoingIndex + i));
        }
        printf("\n");
        return 0;
    }
    
    return -1;
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string
    char *target;
    char *replacement;

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      This is a safe operation because (argc < 2) will return true if argv has less than 2 elements. It will definitely have 1 because
    //      the first argument is the executable. Since this conditional returns true if there's only one element (aka it catches if argv[1] exists)
    //      we then just have to make sure that argv[1] doesn't get referenced in the body of the if statement, which it doesn't. Therefore,
    //      since this conditional will catch if argv[1] doesn't exist, and since the body exits the program without referencing argv[1], we can
    //      safely assume no error will be raised based on the existance of argv[1].
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      This program will either take two arguments (one being the executable and the other being 'h'), take more than two arguments, or it
    //      will not have the right amount and formatting of the arguments. We have already identified and caught the case where only one argument
    //      gets passed. We have already identified and caught the case where the proper formatting of two arguments gets passed. We just need a way
    //      to differentiate between the program having the right number of arguments or having two arguments in the wrong formatting. This if
    //      statement will identify when there were two arguments passed that didn't use the 'h' argument. Then it will exit the program with
    //      a signal identifying there was a formatting error.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = malloc(BUFFER_SZ);
    if (buff == NULL) {
        exit(99);           //determine if the pointer is null and exit with a return code of 99 if it is
    }


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d\n", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d\n", rc);      //display error message if count_words fails
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            rc = reverse_words(buff, user_str_len);                 //reverse the words in the buffer
            if (rc < 0) {
                printf("Error reversing words, rc = %d\n", rc);     //display error message if reverse_words fails
                exit(2);
            }
            break;
        case 'w':
            rc = word_print(buff, BUFFER_SZ, user_str_len);         //print the words in the buffer
            if (rc < 0) {
                printf("Error printing words, rc = %d\n", rc);      //display error message if word_print fails
                exit(2);
            }
            break;
        case 'x':
            if (argc < 5) {
                usage(argv[0]);
                exit(1);
            }
            target = *(argv + 3);           //Assign argument 4 to be the target
            replacement = *(argv + 4);      //Assign argument 5 to be the replacement
            rc = search_and_replace(buff, BUFFER_SZ, target, replacement);  //display the string after replacing the first occurrence of target
            if (rc < 0) {
                printf("Error replacing words, rc = %d\n", rc);     //display error message if search_and_replace fails
                exit(2);
            }
            break;
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          Having both the pointer and the length allows you to find your specific data (pointer), but it also tells you
//          which memory is yours to use. If you didn't have the length, it would be a lot harder to make sure the program stops
//          before providing a segmentation fault. It's also important practice because despite what our main function says, if someone else
//          wanted to use our helper functions, they could do it without the restraint of having 50 bytes no matter what. Now our functions
//          are more universal which is always beneficial.