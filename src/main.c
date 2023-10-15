#include <stdlib.h>
#include "auxf.h"

int main(int argc, char *argv[])
{
    if (1 == argc)
    {
        argc = 2;
        argv[1] = ".";
        argv[2] = NULL;
    }

    parseArgs(argc, argv);

    exit(EXIT_SUCCESS);
}
