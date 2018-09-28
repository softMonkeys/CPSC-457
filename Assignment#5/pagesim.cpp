/*
 * pagesim.cpp
 *
 * Student Name: BenKun Chen
 * Student Number: 30005337
 *
 * Class: CPSC 457 Spring 2017
 * Instructor: Pavol Federl
 *.
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>

using namespace std;

// Method that prints the outputs
void print(string title, vector<int> referenceString, int avaliableFrame[], int numberOfAvaliableFrame, int pageMiss, int pageHit){
  printf("%s\n", title.c_str());
  printf("  - frames: ");
  for(int i = 0; i < numberOfAvaliableFrame; i++){
    if(avaliableFrame[i] == -1){
      printf("empty ");
    }else{
      printf("%d ", avaliableFrame[i]);
    }
  }
  printf("\n");
  pageMiss = referenceString.size() - pageHit;
  printf("  - page faults: %d\n", pageMiss);
  printf("\n");
}


// Main method
int main(int argc, char *argv[]){
  ifstream config;       // Configuration file
  string conffile;       // The configuration file name
  vector<int> referenceString;    // create vector to store the reference string since we do not know the size
  int numberOfAvaliableFrame;	// number of avaliable frame
  int pageHit = 0;
  int pageMiss = 0;

  // Read in the config file name from the commanda-line arguments
  if(argc != 2){
    cout << "Usage: ./pagesim '# of avaliable frame' < 'file name.txt'\n";
    return 0;
  }
  numberOfAvaliableFrame = atoi(argv[1]);   // get the number of avaliable frame from the command line
  int avaliableFrame[numberOfAvaliableFrame];   // create array with size of avaliable frame
  int referenceBit[numberOfAvaliableFrame];   // create array with size of avaliable frame for reference bits
    for(int i = 0; i < numberOfAvaliableFrame; i++){	// initialize the all values in the array to -1
    avaliableFrame[i] = -1;
    referenceBit[i] = -1;
  }
  
  // Open the file
  int value = 0;
  cin >> value;   // start reading the file before pass into the loop in order to avoid error
  while(!cin.eof()){
    referenceString.push_back(value);   // store the reference string
    cin >> value;
  }

  // Optimal ///////////////////////////////////////////////////////////
  for(int i = 0; i < referenceString.size(); i++){
    bool flag = true;
	// Checks if the frame is full
    for(int j = 0; j < numberOfAvaliableFrame; j++){
      for(int a = 0; a < numberOfAvaliableFrame; a++){
        if(avaliableFrame[a] == -1){
          flag = false;
        }
      }
    }
    if(flag){   // if the frame is full, read the dighit in the reference string
      int secondBreak = 0;
      for(int b = 0; b < numberOfAvaliableFrame; b++){
        if(avaliableFrame[b] == referenceString.at(i)){
          pageHit++;	// if the digit already exist, page hit
          secondBreak++;	// jump out
          break;   // if the number is already in the frame, page hit
        }
      }
      if(secondBreak != 0){
        continue;
      }
      // if no page hit exist, we start checking
      int compareFrame[numberOfAvaliableFrame];		// create an extra array to avoid duplicate digit
      for(int f = 0; f < numberOfAvaliableFrame; f++){
        compareFrame[f] = -1;
      }
      int count = 0;
      if(i + 1 != referenceString.size()){
        for(int d = i + 1; d < referenceString.size(); d++){
          for(int e = 0; e < numberOfAvaliableFrame; e++){
            if(referenceString.at(d) == avaliableFrame[e]){		// if there exist digit whcih matches the digit in the frame, remember its index in the frame	
              if(compareFrame[e] == -1){
                compareFrame[e] = 0;
                count = e;
              }		// if the digit is been found twice, ignore it on the second time
            }
          }
        }
        for(int g = 0; g < numberOfAvaliableFrame; g++){
          if(compareFrame[g] == -1){
            count = g;
            break;
          }
        }
        avaliableFrame[count] = referenceString.at(i);
      }else{		// if it is the last digit, replace the first digit in the frame to the digit without checking
        avaliableFrame[0] = referenceString.at(i);
      }
    }else{    // if the frame is not full, only put the digit been read into the first avaliable frame
      for(int a = 0; a < numberOfAvaliableFrame; a++){
        if(avaliableFrame[a] == -1){
          avaliableFrame[a] = referenceString.at(i);
          break;
        }
      }
    }
  }  
  print("Optimal", referenceString, avaliableFrame, numberOfAvaliableFrame, pageMiss, pageHit);	// prints the outputs

  // Reset the values
  pageMiss = 0;
  pageHit = 0;
  for(int i = 0; i < numberOfAvaliableFrame; i++){
    avaliableFrame[i] = -1;
  }

  // Least Recent Used(LRU) ////////////////////////////////////////////
  // Same structure as the optimal except checking backwards
  for(int i = 0; i < referenceString.size(); i++){
    bool flag = true;
    for(int j = 0; j < numberOfAvaliableFrame; j++){
      for(int a = 0; a < numberOfAvaliableFrame; a++){
        if(avaliableFrame[a] == -1){
          flag = false;
        }
      }
    }
    if(flag){   // if the frame is full
      int secondBreak = 0;
      for(int b = 0; b < numberOfAvaliableFrame; b++){
        if(avaliableFrame[b] == referenceString.at(i)){
          pageHit++;
          secondBreak++;
          break;   // if the number is already in the frame, page hit
        }
      }
      if(secondBreak != 0){
        continue;
      }
      // if no page hit exist, we start checking
      int compareFrame[numberOfAvaliableFrame];
      for(int f = 0; f < numberOfAvaliableFrame; f++){
        compareFrame[f] = -1;
      }
      int count = 0;
      if(i - 1 != 0){
        for(int d = i - 1; d >= 0; d--){	// check backwards
          for(int e = 0; e < numberOfAvaliableFrame; e++){
            if(referenceString.at(d) == avaliableFrame[e]){
              if(compareFrame[e] == -1){
                compareFrame[e] = 0;
                count = e;
              }
            }
          }
        }
        for(int g = 0; g < numberOfAvaliableFrame; g++){
          if(compareFrame[g] == -1){
            count = g;
            break;
          }
        }
        avaliableFrame[count] = referenceString.at(i);
      }else{
        avaliableFrame[0] = referenceString.at(i);
      }
    }else{    // if the frame is not full
      for(int a = 0; a < numberOfAvaliableFrame; a++){
        if(avaliableFrame[a] == -1){
          avaliableFrame[a] = referenceString.at(i);
          break;
        }
      }
    }
  }
  print("LRU", referenceString, avaliableFrame, numberOfAvaliableFrame, pageMiss, pageHit);	// prints the outputs

  // Reset the values
  pageMiss = 0;
  pageHit = 0;
  for(int i = 0; i < numberOfAvaliableFrame; i++){
    avaliableFrame[i] = -1;
  }

  // Clock algorithm ///////////////////////////////////////////////////
  int pointer = 0;		// pointer for pointing the index
  for(int i = 0; i < referenceString.size(); i++){
    bool flag = true;
    for(int j = 0; j < numberOfAvaliableFrame; j++){
      for(int a = 0; a < numberOfAvaliableFrame; a++){
        if(avaliableFrame[a] == -1){
          flag = false;
        }
      }
    }
    if(flag){   // if the frame is full
      int secondBreak = 0;
      for(int b = 0; b < numberOfAvaliableFrame; b++){
        if(avaliableFrame[b] == referenceString.at(i)){
          pageHit++;
          secondBreak++;
          referenceBit[b] = 1;	// change the correspond frame's reference bit to 1
          break;   // if the number is already in the frame, page hit
        }
      }
      if(secondBreak != 0){
        continue;
      }
      // if no page hit exist, we start checking
      int compareFrame[numberOfAvaliableFrame];
      for(int f = 0; f < numberOfAvaliableFrame; f++){
        compareFrame[f] = -1;
      }
      int count = 0;
      while(true){
        pointer++;
        if(pointer == numberOfAvaliableFrame){		// if pointer has exceed the frame size, reset it back to 0
          pointer = 0;
        }
        if(referenceBit[pointer] == 1){		// while scnning through the frame, set the priority frame back to normal by switching the reference bit back to 0
          referenceBit[pointer] = 0;
        }else if(referenceBit[pointer] == 0){		// if sees normal frame(with refernece bit 0), we switch the content in the frame to the new content
          referenceBit[pointer] = 1;	// also, set the normal frame back to priority by sitching the reference bit back to 1
          avaliableFrame[pointer] = referenceString.at(i);
          break;
        }
      }
    }else{    // if the frame is not full
      for(int a = 0; a < numberOfAvaliableFrame; a++){
        if(avaliableFrame[a] == -1){
          avaliableFrame[a] = referenceString.at(i);
          pointer = a;
          referenceBit[a] = 1;
          break;
        }
      }
    }

  }
  print("Clock", referenceString, avaliableFrame, numberOfAvaliableFrame, pageMiss, pageHit);	// prints the outputs

  return 0;
}
