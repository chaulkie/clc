// A PERSONAL STACK-BASED POST-FIX CALCULATOR
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// TYPES AND MACROS
#define WORD_SIZE 16
#define STACK_SIZE 20

#define ERR_LOW_STACK -1
#define ERR_BIG_WORD -2
#define ERR_INVALID -3

typedef double Item;

struct Command
{
    char key[WORD_SIZE];
    int (*func)(void);
};
typedef struct Command Command;

/// PROTOTYPES
Item    STACK_pop   ();
void    STACK_push  ();
int     STACK_print ();

int     STACK_add   ();
int     STACK_sub   ();
int     STACK_mul   ();
int     STACK_div   ();

int     STACK_neg   ();
int     STACK_del   ();

int     STACK_sum   ();
int     STACK_clr   ();

void    handle_error(int err_code);
int     prompt      ();
int     parse_word  (char word[]);
char    skip_space  ();
void    skip_line   ();

/// EXTERNAL VARIABLES
Item STACK_contents[STACK_SIZE];
int STACK_top = 0;

const Command commands[] = {
    {"+",   STACK_add},
    {"-",   STACK_sub},
    {"*",   STACK_mul},
    {"/",   STACK_div},
    {"neg", STACK_neg},
    {"d",   STACK_del},
    {"c",   STACK_clr},
    {"sum", STACK_sum}
};
const int num_commands = sizeof(commands) / sizeof(Command);

/// BODY
int main (void)
{
    int code;
    for (;;)
    {
        printf(" > ");
        code = prompt();
        if (code < 0) handle_error(code);
        STACK_print();
    }
    exit(EXIT_SUCCESS);
}

void handle_error (int err_code)
{
    printf("ERROR: ");
    switch (err_code)
    {
        case ERR_LOW_STACK:
            printf("Not enough items in the stack to perform operation.\n");
            break;
        case ERR_BIG_WORD:
            printf("Exceeded maximum word-length.\n");
            break;
        case ERR_INVALID:
            printf("Unrecognised word.\n");
            break;       
    }
}

/// INTERFACE
int parse_word (char word[])
{   // Pushes numbers and runs commands based on the given word
    char* endptr; // The first unconvertable char found by the following function call
    double conv = strtod(word, &endptr);
    if ( *endptr == '\0' )
    {   // Word is a number
        STACK_push(conv);
        return 0;
    }

    for ( int i=0 ; i<num_commands ; i++ )
    {   // Check word against commands
        if ( strcmp(word, commands[i].key) == 0 )
            return commands[i].func();
    }
    return ERR_INVALID; // Unrecognized word
}

int prompt ()
{   // Process a user-inputted line and pass the words onto the parser
    char ch = ' ';
    char word[WORD_SIZE];

    for ( int i=0, code ; ch != '\n' ; i=0 )
    {
        ch = skip_space(); // Beginning of word
        while ( ch != ' ' && ch != '\n' && ch != '\t' )
        {   // Extract word
            if ( i == WORD_SIZE - 1 )
            {
                skip_line();
                return ERR_BIG_WORD;
            }
            word[i++] = ch;
            ch = getchar();
        }
        
        word[i] = '\0'; // Terminating the string
        code = parse_word(word);
        if ( code < 0 )
        {   // Error code
            if ( ch != '\n' ) skip_line();
            return code;
        }
    }
    return 0;
}

char skip_space()
{   // Returns the next non-whitespace character
    char ch;
    while ( (ch = getchar()) == ' ' )
        ;
    return ch;
}

void skip_line()
{
    char ch;
    while ( (ch = getchar()) != '\n' ) ;
}

/// STACK OPERATIONS
// Binary operations
int STACK_add ()
{
    if ( STACK_top < 2 ) return ERR_LOW_STACK;
    STACK_push( STACK_pop() + STACK_pop() );
    return 0;
}

int STACK_sub ()
{
    if ( STACK_top < 2 ) return ERR_LOW_STACK;
    Item b = STACK_pop();
    Item a = STACK_pop();
    STACK_push( a - b );
    return 0;
}

int STACK_mul ()
{
    if ( STACK_top < 2 ) return ERR_LOW_STACK;
    STACK_push( STACK_pop() * STACK_pop() );
    return 0;
}

int STACK_div ()
{
    if ( STACK_top < 2 ) return ERR_LOW_STACK;
    Item d = STACK_pop();
    Item n = STACK_pop();
    STACK_push( n / d );
    return 0;
}

// Unary operations
int STACK_neg ()
{
    if ( STACK_top < 1 ) return ERR_LOW_STACK;
    STACK_push( -STACK_pop() );
    return 0;
}

int STACK_del ()
{
    if ( STACK_top >= 1) STACK_pop();
    return 0;
}

// Full stack operations
int STACK_sum ()
{
    double sum = 0;
    while (STACK_top > 0)
        sum += STACK_pop();
    STACK_push(sum);
    return 0;
}

int STACK_clr ()
{   // Clears the stack and returns the number of items cleared
    int items_cleared = STACK_top;
    STACK_top = 0;
    return items_cleared;
}

/// BASIC STACK OPERATIONS

void STACK_push (Item val)
{   // Add an item to the top of the stack
    if ( STACK_top == STACK_SIZE )
    {
        printf("ERROR: stack limit exceeded.\n");
        exit(EXIT_FAILURE);
    }
    STACK_contents[STACK_top++] = val;
}

Item STACK_pop ()
{   // Remove the top item from the stack and return it
    if ( STACK_top != 0 )
        return STACK_contents[--STACK_top];
    printf("ERROR: attempt to pop empty stack.\n");
    exit(EXIT_FAILURE);
}

int STACK_print ()
{   // Prints the stack without altering it
    for ( int i=0 ; i<STACK_top-1 ; i++ )
    {
        printf("%.2lf ", STACK_contents[i]);
    }
    printf("[ %.2lf ]", STACK_contents[STACK_top-1]);
    
    putchar('\n');
    return 0;
}
