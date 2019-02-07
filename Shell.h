/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Shell.h
 * Author: ishan
 *
 * Created on March 10, 2018, 8:52 AM
 */

#ifndef SHELL_H
#define SHELL_H

#ifdef __cplusplus
extern "C" {
#endif


#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h> 
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
    


#define TOK_BUFSIZE 70
#define TOK_DELIMETER " \t\r\n\a"
#define BUFSIZE 1024    
#define BOLDGREEN   "\033[1m\033[32m"
#define BOLDBLUE    "\033[1m\033[34m"
#define RESET   "\033[0m"


/*
  Function Declarations for builtin shell commands:*/
 
int cd(char **args);
int dir(char **args);
int clr(char **args);
int echo(char **args);
int environment(char **args);
int help(char **args);
int pause_program(char **args);
int quit_shell(char **args);


/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "dir",
  "clr",
  "echo",
  "environ",
  "help",
  "pause",
  "quit"
};

int (*builtin_func[]) (char **) = {
  &cd,
  &dir,
  &clr,
  &echo,
  &environment,
  &help,
  &pause_program,
  &quit_shell
};

int num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/*just changes to the next directory inside the directory*/
int cd(char **args) {
	char *path;
	char cwd[1024];
	// gets the current directory
	getcwd(cwd,1024);
	path = malloc(sizeof(path));
	//malloc the stored chars
	
	
	// copies pwd to direc
	path = strcpy(path, cwd);
	
	// adds / to the for the current format
	
	path = strcat(path, "/");
	// // adds the input from the user to the current directory
	
	path = strcat(path, args[1]);
	
	// changes the current directory
	chdir(path);
	
	if(chdir(path) == -1) {
		printf("cannot find the file\n");
		//return -1;
	}
	return 1;
}

/*List all the directories*/
int dir(char **args){
    if (args[1]!= NULL){
        perror("Error");
    }
        DIR *dfd = NULL;
	char *direc;
	struct dirent *dp;
	
	char curr_dir[1024];
	
	getcwd(curr_dir, 1024);
	
	
	if(curr_dir== NULL) {
		printf("Could not find the current directory\n");
	}
	
	dfd = opendir(curr_dir);
	
	if(dfd == NULL) {
		printf("Count not open this working directory\n");
	}
	int count = 0;
	printf(BOLDGREEN);
	for(count = 0; NULL != (dp = readdir(dfd));count++) {
		if(dp->d_name[0] != '.')
	
		printf("%9.9s ", dp->d_name);
	}
	printf(RESET);
	
	printf("\n");
	return 1;
}

/*Just clears the screen and brings command on the top of console*/
int clr(char **args){
    printf("\033[H\033[J");
    return 1;
}

/*Pauses the screen till user hits "ENTER"*/
int pause_program(char **args){
    char c;
    c = getchar();
    while(c != '\n'){
        c=getchar();
    }
    return 1;      
}

/*Prints the command line again in new line*/
int echo(char **args){
    
    int i = 1;
    while(args[i]!= NULL){
        printf("%s ", args[i]);
        i++;
    }
    printf("\n");
    return 1;
}

/*Shows all environment variables*/
extern char **environ;
int environment(char **args){
    
    if (args[1]!= NULL){
        perror("Error");
    }
   
    char **env = environ;
	// we print the environment variables along with
	// their values
		
    while(*env){
      printf("%s\n", *env++);
    }
    return 1;
}

/*Shows the manual for builtin commands*/
int help(char **args)
{
  int i;
  printf("IshanShell program\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

/*Exits the IshanShell Program*/
int quit_shell(char **args)
{
    printf("IshanShell ended!\n\n");
    return 0;
}

int in, out, input, output, append; // I/O redirection parameters
char *inputFile, *outputFile; // I/O input and output files
FILE *fp;             //pointer to file for ouput file  

// check the command for any I/O redirection
void checkIO(char **args){
  // reset input and output and append
  

  int i = 0;

  // loop through the command line input
  while(args[i] != NULL){
    if (!strcmp(args[i], "<")){           //check for input <
        inputFile = args[i + 1];
        args[i] = NULL;
        int fd0;
        if ((fd0 = open(inputFile, O_RDONLY, 0)) < 0) {
            perror("Couldn't open input file");
            exit(0);
        }   
        break;
        // dup2() copies content of fdo in input of preceeding file
        dup2(fd0, 0); // STDIN_FILENO here can be replaced by 0 

        close(fd0); // necessary
        }
    else if (!strcmp(args[i], ">")){      //check for output >
      outputFile = args[i+1]; 
      args[i] = NULL;
      int fd1 ;
        if ((fd1 = creat(outputFile , 0644)) < 0) {
            perror("Couldn't open the output file");
            exit(0);
        }           

        dup2(fd1, STDOUT_FILENO); // 1 here can be replaced by STDOUT_FILENO
        close(fd1);
      
    }
    else if (!strcmp(args[i], ">>")){     //check for append output >>
      outputFile = args[i+1];
      args[i] = NULL;
      int fd2 ;
        if ((fd2 = open(outputFile ,O_WRONLY, 0666)) < 0) {
            perror("Couldn't open the output file");
            exit(0);
        }           

        dup2(fd2, STDOUT_FILENO); // 1 here can be replaced by STDOUT_FILENO
        close(fd2);
    }
    i++;
  }
}


    
/**
  Launch a program and wait for it to terminate.
  args Null terminated list of arguments (including program).
  Always returns 1, to continue execution.
 */
int start_process(char **args)
{
  pid_t pid, wpid;
  int status;
  int i;
  int j=0;
 
  
 
  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("Error");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("Error");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

/**
   Execute shell built-in or launch program.
   args Null terminated list of arguments.
   return 1 if the shell should continue running, 0 if it should terminate
 */
int execute_command(char **args)
{
  int i;
   checkIO(args);
  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }
  
  for (i = 0; i < num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return start_process(args);
}

/**
   Read a line of input from stdin.
   return The line from stdin.
 */
char *get_line(void)
{
  int bufsize = BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "Error: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "Error: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}




/**
   Split a line into tokens.
   return Null-terminated array of tokens.
 */
char **split_line(char *line)
{
  int bufsize = TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;
  

  if (!tokens) {
    fprintf(stderr, "Error: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOK_DELIMETER);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "Error: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOK_DELIMETER);
  }
  tokens[position] = NULL;
  return tokens;
}


#ifdef __cplusplus
}
#endif

#endif /* SHELL_H */

