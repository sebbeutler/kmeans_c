#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>

#include "pcg_basic.h"

void onStart(int argc, char* argv[]);
void onExit(void);

int main(int argc, char* argv[])
{
    onStart(argc, argv);



    return EXIT_SUCCESS;
}

#ifdef _WIN32
#include <sys\timeb.h> 
struct timeb progStart, progEnd;
#endif // !_WIN32

char BIN_PATH[255];

void onStart(int argc, char* argv[])
{
    printf(
" _                                   \n"
"| |                                  \n"
"| | ___ __ ___   ___  __ _ _ __  ___ \n"
"| |/ / '_ ` _ \\ / _ \\/ _` | '_ \\/ __|\n"
"|   <| | | | | |  __/ (_| | | | \\__ \\\n"
"|_|\\_\\_| |_| |_|\\___|\\__,_|_| |_|___/\n");
    // Add function on program exit event
    atexit(&onExit);
    // Setup the random number generator
    pcg32_srandom((unsigned long long)time(NULL), (unsigned long long) & printf);

    // Get the program binary path from the program arguments
    strcpy(BIN_PATH, argv[0]);
	for (size_t i=strlen(BIN_PATH)-1; i>=0; i--)
	{
		if (BIN_PATH[i] == '/' || BIN_PATH[i] == '\\')
		{
			BIN_PATH[i+1] = '\0';
			break;
		}
	}
    
#ifdef _WIN32
    // Save the time when the program starts to get the execution time later
    ftime(&progStart);
#endif // !_WIN32
}

void onExit(void)
{

#ifdef _WIN32

    ftime(&progEnd);
    
    printf("\n\n---------------------------------------------------------------------------------------------------------------------\n");
    printf("Program ended in %.3f seconds.\n", (1000 * (progEnd.time - progStart.time) + (progEnd.millitm - progStart.millitm)) / 1000.f);
    //system("pause");
#endif // !_WIN32
}