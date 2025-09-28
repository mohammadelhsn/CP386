/**
 * @file a1q1.c
 * @author Mohammad El-Hassan (elha7950@mylaurier.ca) 169067950
 * @brief Assignment 1, Question 1
 * @version 1.0
 * @date 2025-09-28
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[])
{
    // Fork the first time
    int rc = fork();
    // Save the parent's ID so it doesn't get overriden
    int parentId = (int)getppid();
    // Fork Failed
    if (rc < 0)
    {
        fprintf(stderr, "Fork Failed\n");
        exit(1);
    }
    else if (rc == 0) // The Child branch
    {
        int rc1 = fork(); // Fork again

        if (rc1 < 0)
        {
            // Fork Failed
            fprintf(stderr, "Fork Failed\n");
            exit(1);
        }
        else if (rc1 == 0) // The grand child
        {
            printf("My process id is %d, my parent’s id is %d, and my Grandparent’s id is %d\n", (int)getpid(), (int)getppid(), parentId);
        }
    }
    return 0;
}