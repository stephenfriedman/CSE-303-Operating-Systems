#include <dirent.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "support.h"





int  myCompare(const void* word1, const void* word2);
int  myCompare(const void* word1, const void* word2)
{
  const char* word11 = *(const char **)word1;
  const char* word22 = *(const char **)word2;

  int i = 0;
  int countx =0;
  int county = 0;
  for(i=0;i<sizeof(word11);i++)
    {
      char c1 = tolower(word11[countx]);
      while(c1 == 46 || c1 == 47)
	{
	  if((countx+1) <  sizeof(word11))
	    {
	      countx++;
	      c1 = tolower(word11[countx]);
	    }
	  else
	    c1 = 0;
	}

      char c2 = tolower(word22[county]);
      while(c2 == 46 || c2 ==47)
	{
	  if((county+1) <  sizeof(word22))
	    {
	      county++;
	      c2 = tolower(word22[county]);
	    }
	  else c2=0;
	}
      if(c1 == 0)
	return -1;
      else if(c2 == 0)
	return 1;

      if(c1 < c2)
	return -1;
      else if(c1 > c2)
	return 1;
      
      countx++;
      county++;
    }
  return 0;
}

/*
 * mylsr() - produce the appropriate directory listing(s)
 */
void mylsr(char **roots)
{
  /* TODO: Complete this function */
  printf("%s:\n",roots[1]);

  DIR *dir;
  if ((dir = opendir (roots[1])) == NULL)
    {
      perror ("Cannot open .");
      exit (1);
    }
  
  int numFiles = 0;
  struct dirent *dp;
  while ((dp = readdir (dir)) != NULL)
    {
      if((strcmp(dp->d_name,".")!=0) && (strcmp(dp->d_name,"..")!=0))
	{
	  // allFiles[counter]= dp11->d_name;
	  numFiles++;
	}
      //else
      //counter--;
      //numFiles++;
    }
  
  char *allFiles[numFiles];

  
  
  DIR *dir11;
  struct dirent *dp11;
  if((dir11 = opendir(roots[1]))==NULL)
    {
      perror ("Cannot open .");
      exit (1);
    }
 
  int counter =0;
  while ((dp11 = readdir (dir11)) != NULL)
    {   
      if((strcmp(dp11->d_name,".")!=0) && (strcmp(dp11->d_name,"..")!=0))
	{
	  allFiles[counter]= dp11->d_name;
	  counter++;
	}
      //else
      //counter--;
    }
  
  qsort(allFiles,numFiles,sizeof(char*),myCompare);
  
  int c=0;
  int numDirs =0;
  for(c=0;c<counter;c++)
    {
      struct stat statbuf1;
      char* fullPath =  malloc (sizeof(allFiles[c])+sizeof(char)+20);
      strcpy(fullPath,roots[1]);
      strcat(fullPath,"/");
      strcat(fullPath,allFiles[c]);

      //stat(allFiles[c],&statbuf1);
      stat(fullPath,&statbuf1);
      if(S_ISDIR(statbuf1.st_mode) != 0)
	{
	  numDirs++;
	}
    }
  //printf("\n");
  //printf("Num dirs = %i\n",numDirs);
  
  char *dirs[numDirs];
  int dirsCounter =0;
  
  for(c=0;c<counter;c++)
    {
      // puts("loop");
      struct stat statbuf;
      //stat(allFiles[c],&statbuf);
      char* fullPath1 =  malloc (sizeof(allFiles[c])+sizeof(char)+20);
      strcpy(fullPath1,roots[1]);
      strcat(fullPath1,"/");
      strcat(fullPath1,allFiles[c]);

      //stat(allFiles[c],&statbuf1);
      stat(fullPath1,&statbuf);
      
      //puts(allFiles[c]);
      if(S_ISDIR(statbuf.st_mode) != 0)
	{
	  char *holder = malloc (sizeof(roots[1])+sizeof(char));
	  strcpy(holder,roots[1]);
	  strcat(holder,"/");
	  char *holder2 =  malloc (sizeof(holder)+sizeof(char));
	  strcpy(holder2,holder);
	  strcat(holder2,allFiles[c]);
	  
	  dirs[dirsCounter]=holder2;
	  //printf("(%s)",dirs[dirsCounter]);
	  dirsCounter++;
	}
      
      printf("%s\n",allFiles[c]);
    }

  //printf("dirsCounter = %i\n",dirsCounter);


  
  printf("\n");
  /*printf("%i\n",dirsCounter);
  printf("%s\n",dirs[0]);
  printf("%s\n",dirs[1]);
  */
  for(c=0;c<dirsCounter;c++)
    {
      // printf("(%s)\n",dirs[c]);
      //printf("(%i)\n",strlen(dirs[c]));
      char **recursion = (char**) malloc(5 * sizeof(char*));
      int u=0;
      for(u=0;u<2;u++)
	{
	  recursion[u]=(char*) malloc(20*sizeof(char));
	}

      
      strncpy(recursion[0]," ",20);
      strncpy(recursion[1],dirs[c],20);
     
      printf("\n");
      mylsr(recursion);
      //free(recursion);
      //puts("shoodnt get here");
    }
  // printf("Exiting method\n");
}

/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s [FILE]...\n", progname);
    printf("List information about the FILEs (the current directory by default).\n");
    printf("List subdirectories recursively\n");
    printf("Behavior should mirror /bin/ls -1 -r\n");
}

/*
 * main() - The main routine parses arguments and dispatches to the
 *          task-specific code.
 */
int main(int argc, char **argv) {
    /* for getopt */
    long opt;

    /* run a student name check */
    check_student(argv[0]);

    /* parse the command-line options.  For this program, we only support  */
    /* the parameterless 'h' option, for getting help on program usage. */
    /* TODO: make sure to handle any other arguments specified by the assignment */
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch(opt) {
          case 'h': help(argv[0]); break;
        }
    }
   
    /* TODO: fix this invocation */
    if(argc==1)
      {
	char*a[2]={"mylsr","."};
	mylsr(a);
      }
    else
      mylsr(argv);
}
