#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAXLINE 80 /*The maximum length comman*/
#define MAXARGS (MAXLINE / 20 + 1)

typedef struct command
{
    char *name;
    int seq_num;
    struct command *link;
} CMD;
int length = 0;

int parse_command(char *line, char **args, int max_args);
CMD *create_point(char *name);
void insert(CMD *CD, CMD *head);
void free_args(char **args, int max_args);

int main()
{
    char *args[MAXARGS] = {NULL};
    char line[MAXLINE];
    int should_run = 1;

    CMD *head = create_point("null");

    while (should_run)
    {
        printf("osh> ");
        fflush(stdout); /*immeadiately print char to IO*/

        /*1. read a line*/
        if (fgets(line, MAXLINE, stdin) == NULL)
        {
            break; // EOF mistake
        }

        line[strcspn(line, "\n")] = '\0';
        int argc = parse_command(line, args, MAXARGS);

        if (argc == 0)
            continue;

        if (strcmp(args[0], "exit") == 0)
        {

            free_args(args, MAXARGS);
            break;
        }
        else if (strcmp(args[0], "history") == 0)
        {
            if (length == 0)
                printf("no commands in history\n");
            else
            {
                CMD *point = head->link;
                while (point != NULL)
                {
                    printf("%d %s\n", point->seq_num, point->name);
                    point = point->link;
                }
            }
            free_args(args, MAXARGS);
            continue;
        }
        else if (strcmp(args[0], "!!") == 0)
        {
            if (length == 0)
            {
                printf("no commands in history\n");
            }
            else
            {
                CMD *point = head->link;
                printf("%d %s\n", point->seq_num, point->name);
            }
            free_args(args, MAXARGS);
            continue;
        }
        else if (strcmp(args[0], "!") == 0)
        {
            if (atoi(args[1]) < length || atoi(args[1]) > length)
                printf("no such command in history");
            else
            {
                CMD *point = head->link;
                for (int i = 0; i < length - atoi(args[1]); ++i)
                {
                    point = point->link;
                }
                printf("%d %s\n", point->seq_num, point->name);
            }
            free_args(args, MAXARGS);
            continue;
        }

        CMD *p = create_point(args[0]);
        insert(p, head);

        pid_t rc;
        rc = fork();
        if (rc < 0)
        {
            perror("fork!\n");
            exit(EXIT_FAILURE);
        }
        else if (rc == 0)
        {
            execvp(args[0], args);
        }
        else
        {
            wait(NULL);
            free_args(args, MAXARGS);
        }
    }

    /*release link*/
    if (length > 0)
    {
        CMD *cur = head->link;
        while (cur != NULL)
        {
            CMD *next = cur->link;
            free(cur->name);
            free(cur);
            cur = next;
        }
    }
    free(head->name);
    free(head);

    return 0;
}

int parse_command(char *line, char **args, int max_args)
{
    int i = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL && i < max_args - 1)
    {
        args[i] = strdup(token); /*can't the pointer = another pointer that will over the pointer*/
        /*trends allocate*/

        if (!args[i])
        {
            perror("args!");
            exit(EXIT_FAILURE);
        }
        i++;
        token = strtok(NULL, " \t\n"); /*strtok function to split a string*/
    }
    args[i] = NULL;
    return i;
}

void insert(CMD *CD, CMD *head)
{
    CD->link = head->link;
    head->link = CD;
    length++;
    CD->seq_num = length;
};

CMD *create_point(char *name)
{
    /*create virtual head*/
    CMD *part = (CMD *)malloc(sizeof(CMD));
    part->name = strdup(name);
    part->link = NULL;
    return part;
}

void free_args(char **args, int max_args)
{
    for (int i = 0; i < max_args; ++i)
    {
        if (args[i])
        {
            free(args[i]);
            args[i] = NULL;
        }
    }
}