#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BFFR_SIZE 1024
#define WRITE_BFFR_SIZE 1024
#define BFFR_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for conv_num() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct slnlsstr - singly linked list
 * ptr_to num: the number field
 * ptr_to str: a string
 * ptr_to next: points to the next node
 */
typedef struct slnlsstr
{
	int num;
	char *str;
	struct slnlsstr *next;
} ordseq;

/**
 * struct channeldata - contains pseudo-arguements to pass into a function,
 * allowing uniform prototype for function pointer struct
 * ptr_to arg: a string generated from getline containing arguements
 * ptr_to argv:an array of strings generated from arg
 * ptr_to path: a string path for the current command
 * ptr_to argc: the argument count
 * ptr_to line_count: the error count
 * ptr_to err_num: the error code for exit()s
 * ptr_to linecount_flag: if on count this line of input
 * ptr_to fname: the program filename
 * ptr_to env: linked list local copy of environ
 * ptr_to environ: custom modified copy of environ from LL env
 * ptr_to history: the history node
 * ptr_to alias: the alias node
 * ptr_to env_changed: on if environ was changed
 * ptr_to status: the return status of the last exec'd command
 * ptr_to cmd_bffr: address of pointer referencing cmd_bffr, on if chaining
 * ptr_to cmd_bffr_type: CMD_type ||, &&, ;
 * ptr_to readfd: the fd from which to read line input
 * ptr_to histcount: the history line number count
 */
typedef struct channeldata
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	ordseq *env;
	ordseq *history;
	ordseq *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_bffr; /* pointer referencing cmd ; chain buffer, for memory mangement */
	int cmd_bffr_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} orgdt_st;

#define ORGDT_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
		0, 0, 0}

/**
 * struct builtin - contains a builtin string and related function
 * ptr_to type: the builtin command flag
 * ptr_to func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(orgdt_st *);
} builtin_table;


/* toem_shloop.c */
int hsh(orgdt_st *, char **);
int find_builtin(orgdt_st *);
void find_cmd(orgdt_st *);
void fork_cmd(orgdt_st *);

/* toem_parser.c */
int is_cmd(orgdt_st *, char *);
char *dup_chars(char *, int, int);
char *find_path(orgdt_st *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_errors.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* toem_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* toem_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* toem_exits.c */
char *replctstrn(char *, char *, int);
char *cnctntstrn(char *, char *, int);
char *lctcharstrn(char *, char);

/* toem_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* toem_memoryreallocation.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* toem_memorymgt.c*/
int freebfrstrm(void **);

/* toemstrconv.c */
int intermutual(orgdt_st *);
int is_delim(char, char *);
int alphabetic_char(int);
int _strconv(char *);

/* toem_errors1.c */
int _convstr(char *);
void print_error(orgdt_st *, char *);
int print_d(int, int);
char *conv_num(long int, int, int);
void purge_comm(char *);

/* toem_builtinfnctns.c */
int _myexit(orgdt_st *);
int _mycd(orgdt_st *);
int _myhelp(orgdt_st *);

/* toem_builtin1.c */
int _myhistory(orgdt_st *);
int _myalias(orgdt_st *);

/*toem_getline.c */
ssize_t get_feed(orgdt_st *);
int _getline(orgdt_st *, char **, size_t *);
void sigintHandler(int);

/* toem_getorgdt.c */
void clear_orgdt(orgdt_st *);
void set_orgdt(orgdt_st *, char **);
void free_orgdt(orgdt_st *, int);

/* toem_environ.c */
char *_getenv(orgdt_st *, const char *);
int _myenv(orgdt_st *);
int _mysetenv(orgdt_st *);
int _myunsetenv(orgdt_st *);
int spawn_env_lst(orgdt_st *);

/* toem_getenv.c */
char **get_environ(orgdt_st *);
int _unsetenv(orgdt_st *, char *);
int _setenv(orgdt_st *, char *, char *);

/* toem_history.c */
char *get_history_file(orgdt_st *orgdt);
int write_history(orgdt_st *orgdt);
int read_history(orgdt_st *orgdt);
int build_history_list(orgdt_st *orgdt, char *bffr, int linecount);
int renumber_history(orgdt_st *orgdt);

/* toem_lists.c */
ordseq *add_node(ordseq **, const char *, int);
ordseq *add_node_end(ordseq **, const char *, int);
size_t print_list_str(const ordseq *);
int delete_node_at_index(ordseq **, unsigned int);
void free_list(ordseq **);

/* toem_lists1.c */
size_t list_len(const ordseq *);
char **ordseqo_strings(ordseq *);
size_t print_list(const ordseq *);
ordseq *node_starts_with(ordseq *, char *, char);
ssize_t get_node_index(ordseq *, ordseq *);

/* toem_vars.c */
int is_chain(orgdt_st *, char *, size_t *);
void check_chain(orgdt_st *, char *, size_t *, size_t, size_t);
int replace_alias(orgdt_st *);
int replace_vars(orgdt_st *);
int replace_string(char **, char *);

#endif
