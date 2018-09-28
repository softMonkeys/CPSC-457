/**********************************************
 * Last Name:   CHEN
 * First Name:  BENKUN
 * Student ID:  30005337
 * Course:      CPSC 457
 * Tutorial:    T02
 * Assignment:  2
 * Question:    Q6
 *
 * File name: scan.cpp
 *********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>

#define MAX_FNAME_SIZE 512
#define MAX_FILES 1024

// the function is refencing is http://www.cplusplus.com/reference/cstdlib/qsort/
int compare(const void *a, const void * b) {
	return -( *(int*)a - *(int*)b );
}

// Create a structure to save the file size and the original file name index corresponding to the size
struct fileInfo{
	long long fileSize;
	int indexInFiles;
};

int main( int argc, char *argv[]) {
  // handle command line arguments
  if(argc != 3) {
    fprintf(stderr, "Please input valid number of arguments\nUseage: ./<file_name> <the suffix which the file ends with> <numbers of ouputs>\n");
    exit(-1);
  } else{
    //printf("%s\n%s\n%s\n", argv[0], argv[1], argv[2]);
  }
  std::string findCommand = std::string("find . -type f -name '*") + argv[1] + std::string("'");  // find . -type f -name <suffix>
  //std::cout << findCommand << std::endl; // print
  const char *findCommandChar = findCommand.c_str();    // convert the string to const char*
  // open 'find...'
  FILE * fp = popen(findCommandChar, "r");
  if( fp == NULL) {
    perror("popen failed:");
    exit(-1);
  }

  // read in all filenames
  char buff[MAX_FNAME_SIZE];
  int nFiles = 0;
  char * files[MAX_FILES];
  while(fgets(buff,MAX_FNAME_SIZE,fp)) {
    int len = strlen(buff) - 1;
    files[nFiles] = strndup(buff,len);
    nFiles ++;
  }

  struct fileInfo storage[nFiles];

  fclose(fp);
  printf("Found %d files:\n", nFiles);
  // get file sizes for each file and sum them up
  long long totalSize = 0;
  struct stat st;
  for(int i = 0 ; i < nFiles ; i ++ ) {
    if( 0 != stat(files[i],&st)) {
      perror("stat failed:");
      exit(-1);
    }
	// storage the file size and index of the file into the structure array
	storage[i].fileSize = st.st_size;
	storage[i].indexInFiles = i;
  }

  qsort(storage, nFiles, sizeof(struct fileInfo), compare);		// use qsort to sort the list

	// Compare the number of the files found with the user inputs in order to avoid core dumped error
	int counter = 0;
	if(std::stoi(argv[2]) >= nFiles){
		counter = nFiles;
	}else{
		counter = std::stoi(argv[2]);
	}

	// Prints out the sorted result
  for(int i = 0; i < counter; i++){
	  totalSize = totalSize + storage[i].fileSize;
	  printf("\t%s : %ld\n", files[storage[i].indexInFiles], storage[i].fileSize);
  }
  printf("Total size: %lld bytes.\n", totalSize);
  // clean up
  for(int i = 0; i < nFiles ; i ++ ) {
    free(files[i]);
  }
  // return success
  return 0;
}
