#ifndef S_YVES.H
#define S_YVES.H

#include <errno.h>
#include <fcntl.h>
#include <regex.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


size_t str_len(const char *s);
char *str_dup(const char *str);
char *str_chr(const char *str, int c);
char *str_r_chr(const char *s, int c);
char *str_cpy(char *dest, const char *src);



char *new_ijambo(const char *str, int start, int end);

typedef struct _path.file
{
        char *pathname;
        struct _path.file *next;
} path_file_t;

oid free_aliases(alias_t **aliases);
void aliases_printer(const alias_shell_t *aliases);
int unalias(alias_t **aliases, char *command);
char *receive_alias(alias_shell_t *aliases, const char *name);
int alias_handler(alias_shell_t **aliases, char *cmd_line);

