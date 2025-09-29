/**
 * @file a1q3.c
 * @author Mohammad El-Hassan (elha7950@mylaurier.ca)
 * @brief Assignment 1, Question 3
 * @version 1.0
 * @date 2025-09-29
 */

// Imports

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * @brief
 *
 * @return int
 */
int main()
{
    int pipe1[2];
    int pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        perror("pipe");
        exit(1);
    }

    // First child

    if (fork() == 0)
    {
        close(1);      // Close the standard output
        dup(pipe1[1]); // Set the standard output to the writing end of the pipe
        // Close local pipes
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        // Exec
        execlp("ps", "ps", "xao", "sid", NULL);
        perror("exec ps");
        exit(1);
    }

    // Second child
    if (fork() == 0)
    {
        close(0); // Close the standard input
        dup(pipe1[0]);
        close(1);
        dup(pipe2[1]);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        execlp("sort", "sort", "-u", NULL);
        perror("exec sort");
        exit(1);
    }

    // Third child
    if (fork() == 0)
    {
        close(0);
        dup(pipe2[0]);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        execlp("wc", "wc", "-l", NULL);
        perror("exec wc");
        exit(1);
    }

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);
    for (int i = 0; i < 3; i++)
        wait(NULL);
    return 0;
}