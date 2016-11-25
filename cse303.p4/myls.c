#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "support.h"
#include <dirent.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
/*
 * myls() - produce the appropriate directory listing(s)
 */
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
      while(c1 == 46)
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
      while(c2 == 46)
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


void myls(char **roots)
{
  struct stat statbuf;
  int y=1;
  int numReg=0;
  int numDirectories =0;
  int totalArgs=0;
  while(roots[y]!=NULL)
    {
      stat( (roots[y]), &statbuf );
      //if file/dir doesnt exist here
      if( lstat(roots[y],&statbuf) )
	{
	  totalArgs++;
	}
      //is a directory
      else if(S_ISDIR(statbuf.st_mode) != 0)
	{
	  numDirectories++; 
	} 

      //is a vaild file
       else 
	{
	  numReg++;
	}
       y++;
    }
  totalArgs=totalArgs+numDirectories+numReg;
  char *f[numReg];
  char *d[numDirectories];

  struct stat statbuf1;
  y=1;
  numReg=0;
  numDirectories=0;
  while(roots[y]!=NULL)
    {
      stat((roots[y]), &statbuf1);
      if(lstat(roots[y],&statbuf1))
	{
	  perror(roots[y]);
	}
      else if(S_ISDIR(statbuf1.st_mode) != 0)
	{
	  d[numDirectories] = roots[y];
	  numDirectories++;
	}
      
      else //(S_ISREG(statbuf1.st_mode) != 0)
	{
	  f[numReg] = roots[y];
	  numReg++;
	}
      
      
      y++;
    }
  qsort(f,numReg,sizeof(char*),myCompare);
  qsort(d,numDirectories,sizeof(char*),myCompare);


 
  int q =0;
  for(q=0;q<numReg;q++)
    {
      printf("%s\n",f[q]);
    }
  printf("\n");
  //  printf("%i\n",numDirectories);
  int w =0;
  for(w=0;w<numDirectories;w++)
    {
       if(numReg>1)
	 printf("%s:\n",d[w]);
      DIR *mydir1;
      struct dirent *dpp;
      if ((mydir1 = opendir (d[w])) == NULL)
	{
	  perror ("Cannot open .");
	  exit (1);
	}
      int countFiles =0;
      while((dpp = readdir (mydir1))!=NULL)
	{
	  countFiles++;
	}
      //     printf("%i\n",countFiles);
      char *  aFiles[countFiles];

      DIR *dir111;
      struct dirent *dp111;
      if ((dir111 = opendir (d[w])) == NULL)
	{
	  perror ("Cannot open .");
	  exit (1);
	}
      int counter =0;
      while ((dp111 = readdir (dir111)) != NULL)
	{

	  if((strcmp(dp111->d_name,".")!=0) && (strcmp(dp111->d_name,"..")!=0))
	    {
	      aFiles[counter]=dp111->d_name;
	      counter++;
	    }
	  else
	    countFiles--;
	}

      qsort(aFiles,countFiles,sizeof(char*),myCompare);
      int up;
      for(up=0;up<countFiles;up++)
	{
	  printf("%s\n",aFiles[up]); 
	}
      //return;
    }


  
  if(totalArgs>0)
    return;
    
  
  
 
  // char *args[sizeof(roots)];
  qsort(roots,y,sizeof(char*),myCompare);
   
  DIR *dir;
  struct dirent *dp;
  
  if ((dir = opendir (".")) == NULL)
    {
      perror ("Cannot open .");
      exit (1);
    }
  
  int numFiles = 0;
  while ((dp = readdir (dir)) != NULL)
    {
      numFiles++;
    }
  
  char *  allFiles[numFiles];
  
  DIR *dir1;
  struct dirent *dp1;
  if ((dir1 = opendir (".")) == NULL)
    {
      perror ("Cannot open .");
      exit (1);
    }
  
  int countUp =0;
  while ((dp1 = readdir (dir1)) != NULL)
    {
      
      if((strcmp(dp1->d_name,".")!=0) && (strcmp(dp1->d_name,"..")!=0))
	{
	  allFiles[countUp]=dp1->d_name;
	  countUp++;
	}
      else
	numFiles--;
    }
  
  qsort(allFiles,numFiles,sizeof(char*),myCompare);
  
  int k =0;
  for(k=0;k<numFiles;k++)
    {
      printf("%s\n",allFiles[k]);
    }
  
}





/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s [FILE]...\n", progname);
    printf("List information about the FILEs (the current directory by default).\n");
    printf("Behavior should mirror /bin/ls -1\n");
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

    myls(argv);
}
