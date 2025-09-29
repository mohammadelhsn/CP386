/**
 * @file q1q2a.c
 * @author Mohammad El-Hassan (elha7950@mylaurier.ca) 169067950
 * @version 1.0
 * @date 2025-09-28
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        perror("Failed to open file");
        return 1;
    }

    int n;
    if (fscanf(fp, "%d\n", &n) != 1)
    {
        fprintf(stderr, "Failed to read the number of programs\n");
        fclose(fp);
        return 1;
    }

    char paths[n][1024];

    for (int i = 0; i < n; i++)
    {
        if (fgets(paths[i], sizeof(paths[i]), fp) != NULL)
        {
            paths[i][strcspn(paths[i], "\n")] = 0;
        }
    }

    for (int i = 0; i < n; i++)
    {
        int rc = fork();
        if (rc < 0)
        {
            fprintf(stderr, "Fork Failed");
            exit(1);
        }
        else if (rc == 0)
        {
            execl(paths[i], paths[i], NULL);
            perror("exec failed");
            exit(1);
        }
        else
        {
            wait(NULL);
        }
    }

    fclose(fp);
    return 0;
}