/**********************************************
 * Last Name:   CHEN
 * First Name:  BENKUN
 * Student ID:  30005337
 * Course:      CPSC 457
 * Tutorial:    T02
 * Assignment:  2
 * Question:    Q8
 *
 * File name: sum.cpp
 *********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <list>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>
#include <thread>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
int userInputT;
int nums;
vector<int> totalThread;
vector<int> storage;

#define MAX_READ_SIZE 1000000

void * find(void * i){
  int n = *(int *)i;
  int counter = 0;
  int size = 0;
  if (n < (userInputT - 1)){
      size = nums;
  } else {
    size = storage.size()- n *nums;
  }
  int m = n*nums;
  while(m < (n*nums+size)){
      counter = counter + storage[m];
      m++;
  }
  totalThread[n] = counter;
}

int main (int argc, char* argv[]){
  // handle command line arguments
  if(argc != 3) {
    fprintf(stderr, "Please input valid number of arguments\nUseage: ./<file_name> <other file_name> <number>\n");
    exit(-1);
  } else{
    // printf("%s\n%s\n%s\n", argv[0], argv[1], argv[2]);
    userInputT = stoi(argv[2]);
    // try to open the .txt file
    ifstream check;
    check.open(argv[1]);
    if(check.is_open() == false){
        perror("error when opening the file");
        exit(-1);
    } else{
        // create bufffer size 1024 for reading the file
        char buffer[MAX_READ_SIZE];
        while(check.getline(buffer, MAX_READ_SIZE)){
            storage.push_back(stoi(buffer));
        }
        check.close();    // close stream
    }
  }
  nums = round((float)storage.size()/(float)userInputT);
  pthread_t threads[userInputT];
  totalThread = std::vector<int>(userInputT);
  // generates threads
  for(int i = 0; i < userInputT; i++){
    pthread_create(&threads[i], NULL, find, (void *) (new int(i)));
    // prints out the total threads and results after calculation
    pthread_join(threads[i], NULL);
    printf("Thread %d: %ld\n", i+1, totalThread[i]);
  }
  int sum = 0;
  for(int i = 0; i < userInputT; i++){
      sum = sum + totalThread[i];
  }
  printf("Sum = %ld\n", sum);
  // return success
  return 0;
}
