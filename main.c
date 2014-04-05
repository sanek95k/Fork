#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>
#include <time.h>

int main(int argc, char** argv)
{
    pid_t compiler, running;
    int exitStatus;
    double start, end;

    compiler = fork();

    if (!compiler)
    {
        execlp("javac", "javac",  argv[1], NULL);
        fprintf (stderr, "---- Java compiler refuses to start ----\n");
        return 1;
    }

    wait (&exitStatus);

    if (WIFEXITED(exitStatus))
    {
        if (exitStatus)
        {
            fprintf (stderr, "---- Failed while compiling ----\n");
            return 1;
        }
    }

    int fd[2];
    pipe(fd);

    running = fork();
    start = clock();
    if (!running)
    {
        execlp("java", "java",  argv[2], NULL);
        fprintf (stderr, "---- JVM refuses to start ----\n");
    }

    wait (&exitStatus);
    end = clock();

    if (WIFEXITED(exitStatus))
    {
        if (exitStatus)
        {
            fprintf (stderr, "---- Failed while running ----\n");
            return 1;
        }
        else
        {
            printf("---- Execution time of %s:  %f seconds ----\n", argv[2], (end - start) / CLOCKS_PER_SEC);
        }
    }
    return 0;
}
