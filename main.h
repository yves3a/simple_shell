#ifndef MAIN_H
#define MAIN_H

#include <errno.h>
#include <fcntl.h>
#include <regex.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* macros */

#define BUFF_SIZE 1024
#define SPACE ' '
#define CMD_NOT_FOUND 127
#define PROMPT_SIZE 4096
#define PATH_SIZE 2048
#define NOT_BUILTIN 18
#define RUNNING 1
#define CMD_ERR 2
#define MAX_ALIAS_LENGTH 50
#define MAX_VALUE_LENGTH 2048

/* function macros */

#define isspace(c) ((c) == SPACE)
#define isdigit(c) (((c) >= '0' && (c) <= '9')) /* checks for digits */
#define isalpha(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define isnegative(c) (((c) == '-') ? -1 : 1)
#define issign(c) ((c) == '-' || (c) == '+')
#define isquote(c) ((c) == '"' || (c) == '\'')

/*  handlers trings */

size_t str_len(const char *str);
char *str_dup(const char *str);
char *str_chr(const char *str, int c);
char *str_r_chr(const char *s, int c);
char *str_cpy(char *dest, const char *src);
char *str_cat(char *dest, const char *src);
int str_cmp(const char *s1, const char *s2);
char **dup_str_array(char **og);
void concat_arrays(char ***dest, char **src);
char *strp_brk(const char *str, const char *accept);
size_t str_spn(const char *str, const char *accept);
char **str_tok(const char *str, const char *delim);
char *strn_cpy(char *dest, const char *src, size_t n);
int strn_cmp(const char *str1, const char *str2, size_t n);
int get_count_word(const char *str, const char *delim);
char *str_str(const char *str, const char *sub_str);

/* Handle numbers */

int _atoi(const char *str);
void _itoa(size_t n, char *str);
void _reverse(char *buff, size_t len);

/* handle memory */

void str_free(char ***str_array);
char *new_ijambo(const char *str, int start, int end);
void *_realloc(void *old_mem_blk, size_t old_size, size_t new_size);
char *_memcpy(char *dest, char *src, unsigned int n)
/*  a safely way of deallocating dynamic memory */
void _free(void **ptr);
#define free_safely(ptr) _free((void **)&(ptr)) /* _free's frontend */

/* Frees memory for a variable number of objects based on format specifier */
void free_multipler(const char *format, ...);

/* Custom implementation of getline to read input from a file descriptor */
ssize_t _getline(char **lineptr, size_t *n, int fd);

/* Displays the shell prompt in interactive mode */
void prompt_shower(void);

/* Retrieves the hostname from the '/etc/hostname' file */
char *get_hostname(char *buffer);

/* Environment variables and PATH handlers */
extern char **environ;

/**
 * struct _path - builds a linked list of the PATH variable in an environment
 * variable
 * @pathname: each path name in the PATH variable
 * @next: a pointer to the next node
 */

typedef struct _path
{
	char *pathname;
	struct _path *next;
} path_t;

void env_print(void);
void print_path(path_t *list);
void free_list(path_t **head);
char *get_env(const char *name);
path_t *path_builder(path_t **head);

/* aliases */

/**
 * struct alias - the blueprint for the built-in alias command
 * @name: the name of the alias
 * @value: the value assigned to the name
 * @next: a pointer to the next alias_t node
 */
typedef struct alias
{
	char *name;
	char *value;
	struct alias *next;
} alias_t;

void free_aliases(alias_t **head);
void aliases_printer(const alias_t *aliases);
int unalias(alias_t **aliases, char *command);
char *receive_alias(alias_t *aliases, const char *name);
int alias_handler(alias_t **aliases, char *cmd_line);
int print_alias(const alias_t *aliases, const char *name);
void parsing_aliases(const char *data, alias_t **aliases);
void cmd_alias_builder(char ***sub_cmd, char *value_of_alias);
alias_t *alias_add(alias_t **aliases, const char *izina, const char *value);
void non_matching_processor(alias_t *aliases, const char *non_match, int finish);


/* Shell command context */

/**
 * struct shell - Represents the shell's state and environment
 * @aliases: List of command aliases
 * @listof_path: List of directories in the PATH environment variable
 * @line: Command string entered by the user
 * @commands: Array of commands tokenized by semi-colons and newlines
 * @sub_command: Array of tokenized components for each command
 * @tokens: Array of tokens for further processing
 * @token: Single token from the commands
 * @program_name: Name of the program being executed
 * @counter_of_cmd: Number of commands executed since the shell started
 * @code_exiter: Exit code of the last executed program
 */
typedef struct shell
{
	alias_t *aliases;
	path_t *listof_path;
	char *line;
	char **commands;
	char **sub_command;
	char **tokens;
	char *token;
	const char *program_name;
	size_t counter_of_cmd;
	int code_exiter;
} shell_t;

shell_t *shell_init(void);
void handle_signalint(int signum);

/* They handle builtins functions */

int unset_env(const char *name);
int cd_handler(shell_t *ptr);
int set_env(const char *name, const char *value, int over_write);
int builtin_handler(shell_t *ptr);
int exit_handler(shell_t *ptr, void (*cleanup)(const char *format, ...));

/* parsers and executors */

char *rec_operator(char *str);
char *comment_handler(char *cmd);
int line_parsing(shell_t *ptr);
int command_executor(const char *pathname, shell_t *ptr);
int parsing_and_execute(shell_t *ptr, size_t i);
int handler_of_path(shell_t *ptr);
int print_command_not_found(shell_t *ptr);
void file_as_input_handler(const char *filename, shell_t *ptr);
char **variables_handler(shell_t *ptr);
int parsing(shell_t *ptr);
void parsing_helper(shell_t *ptr, size_t index);

#endif /* MAIN_H */
