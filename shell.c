/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   newmain.c
 * Author: ishan
 *
 * Created on March 4, 2018, 10:41 AM
 */

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Shell.h"





/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */

int main(int argc, char **argv, char **envp)
{
  char *line;
  char **args;
  int status;
  
  printf(BOLDBLUE);
  printf("\nWelcome to IshanShell!"
          "\nRun this like a normal shell"
          "\nwith normal linux shell commands.\n");
  printf(RESET);
  do {
    /* gets the users name and the current directory */
    char *user = getenv("USER");
    char path[1024];
    getcwd(path,sizeof(path));
    printf(BOLDGREEN);
    printf("%s@IshanShell",user); 
    printf(RESET);
    printf(":");
    printf(BOLDBLUE);
    printf("~%s",path); 
    printf(RESET);
    printf("$ ");
    line = get_line();
    args = split_line(line);
    status = execute_command(args);

    free(line);
    free(args);
  } while (status);

  return EXIT_SUCCESS;
}
