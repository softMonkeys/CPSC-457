/*
 * banker.cpp
 *
 * Student Name: BenKun Chen
 * Student Number: 30005337
 *
 * Class: CPSC 457 Spring 2017
 * Instructor: Pavol Federl
 *
 * Copyright 2017 University of Calgary. All rights reserved.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

class Banker
{
private:
    int numProc;      // the number of processes
    int numResources; // the number of resources
    int * available;  // number of available instances of each resource
    int ** max;       // the max demand of each process, e.g., max[i][j] = k
                      // means process i needs at most k instances of resource j
    int ** allocation;// the number of resource instances already allocated
    int ** need;      // the number of resource isntances needed by each process

public:

    /* Initializing the vectors and matrixes for the Banker's Algorithm. Takes ownership of
     * all arrays.
     * @param avail  The available vector
     * @param m      The max demand matrix
     * @param alloc  The allocation matrix
     * @param p      The number of processes
     * @param r      The number of resources
     */
    Banker (int * avail, int ** m, int ** alloc, int p, int r) {
        numProc = p;
        numResources = r;

        // Setup the available vector, the max matrix, and the allocation matrix
        available = avail;
        max = m;
        allocation = alloc;

        // Initialize the need matrix
        need = new int*[numProc];
        for (int i = 0; i < numProc; i++)
            need[i] = new int[numResources];
    }

    /* Destroy the vectors and matrixes
     */
    ~Banker() {
        numProc = 0;
        numResources = 0;

        // Free all allocated memory space
        delete[] available;
        for (int i = 0; i < numProc; i++)
        {
            delete[] need[i];
            delete[] max[i];
            delete[] allocation[i];
        }
        delete[] need;
        delete[] max;
        delete[] allocation;
    }

    /* Check whether it is safe to grant the request
     * @param pid    The process that is making the request
     * @param req    The request
     * @param sequenceOrReason  The safe execution sequence returned by the algorithm
     * @return Whether granting the request will lead to a safe state.
     */
    bool isSafe (int pid, int * req, string & sequenceOrReason, int numProc) {
      for(int i = 0; i < numResources; i++){
        if(req[i] > available[i]){    // if request is larger than the available resource, there is not enough resource for the execution continue
          sequenceOrReason = "not enough resources available.";
          return false;
        }
        if(allocation[pid][i] + req[i] > max[pid][i]){    // if the allocation for the request process is larger than the maximum resource, it is invalid
          sequenceOrReason = "request is invalid (exceeding declared max for process).";
          return false;
        }
      }
      // If prerequisites pass, then we calculate the new 'available' and the new 'allocation'
      for(int i = 0; i < numResources; i++){
        available[i] = available[i] - req[i];
        allocation[pid][i] = allocation[pid][i] + req[i];
      }
      // Calculate the need matrix
      for(int i = 0; i < numProc; i++){
        for(int j = 0; j < numResources; j++){
          need[i][j] = max[i][j] - allocation[i][j];    // Need = Max - Allocation
        }
      }
      // After we calculated the need, we can start to comparing it with the available
      bool safe[numProc] = {false};   // array to save the the process been executed
      int count = 0;    // counter for counting
      int processCount = 0;
      for(int b = 0; b < (numProc * numProc) + 1; b++){
        for(int c = 0; c < numProc; c++){   // checks if the process have already finished exection
          if(safe[c] == false){
            processCount++;
          }
        }
        if(processCount == 0){    // if finished, return true
          return true;
        }else{    // if not, reset the processCount to 0 for next loop
          processCount = 0;
        }
        if(b == (numProc * numProc) + 1){   // since worst case senario for comparing available with need is number of proceses square, if the for loop exceed the limit, it means it is an unsafe state
          sequenceOrReason = "request would result in an unsafe state.";
          return false;
        }
        for(int i = 0; i < numProc; i++){
          if(safe[i] == true){    // if the process has already been executed, skip the current index
            continue;
          }else{
            for(int j = 0; j < numResources; j++){
              if(available[j] < need[i][j]){    // if we see avaliable is smaller than need, we record it in the counter
                count++;
              }
            }
            if(count == 0){   // if counter is not been incremented, we can know that all resources in available is bigger or equal to all resources in need
              for(int a = 0; a < numResources; a++){
                available[a] = available[a] + allocation[i][a];   // calculate the new avaliable
              }
              safe[i] = true;   // set the corresponding index in the array to true
              sequenceOrReason = sequenceOrReason + "P" + to_string(i) + ", ";
              continue;
            }else{
              count = 0;
            }
          }

        }

      }
/*
///// prints allocation, available and need///////////////////////
      printf("Allocation\n");
      for(int i = 0; i < numProc; i++){
        for(int j = 0; j < numResources; j++){
          printf("%d ", allocation[i][j]);
        }
        printf("\n");
      }
      printf("Available\n");
      for(int i = 0; i < numResources; i++){
        printf("%d ", available[i]);
      }
      printf("\n");
      printf("Need\n");
      for(int i = 0; i < numProc; i++){
        for(int j = 0; j < numResources; j++){
          printf("%d ", need[i][j]);
        }
        printf("\n");
      }
///////////////////////////////////////////////////////////
*/
    }
};

int main (int argc, char * const argv[])
{
    ifstream config;       // Configuration file
    string conffile;       // The configuration file name
    int numProc;           // The number of processes
    int numResources;      // The number of resources
    string sequenceOrReason;       // The execution sequence returned by the Banker's Algorithm
    int i, j, index;       // Indices for the vectors and matrixes
    int pid;               // The ID of the process that is making the request
    string reqStr;         // The request vector in string format

    // Read in the config file name from the commanda-line arguments
    if (argc < 2)
    {
        cout << "Usage: banker <config file>\n";
        return 0;
    }
    else
    {
        conffile = argv[1];
    }

    // Open the file
    config.open(conffile.c_str());

    // Get the number of process and the number of resources
    string line, var, equal;    // strings for parsing a line in the config file
    getline(config, line);
    istringstream iss(line);
    iss >> var >> equal >> numProc;     // Get the number of processes
    iss.clear();

    getline(config, line);
    iss.str(line);
    iss >> var >> equal >> numResources;    // Get the number of resources
    iss.clear();

    // Create the available vector, the max matrix, and the allocation matrix
    // according to the number of processes and the number of resources
    int * available = new int[numResources];
    int ** max = new int*[numProc];
    int ** allocation = new int*[numProc];
    for (int i = 0; i < numProc; i++)
    {
        max[i] = new int[numResources];
        allocation[i] = new int[numResources];
    }

    // Get the available vector
    getline(config, line);
    replace(line.begin(), line.end(), '<', ' ');  // Remove "<" and ">"
    replace(line.begin(), line.end(), '>', ' ');
    iss.str(line);
    iss >> var >> equal;
    for (j = 0; j < numResources; j++)        // Read in the "available" vector
        iss >> available[j];
    iss.clear();

    // Get the max matrix and the allocation matrix
    for (i = 0; i < numProc; i++)
    {
        getline(config, line);
        replace(line.begin(), line.end(), '<', ' ');
        replace(line.begin(), line.end(), '>', ' ');
        iss.str(line);
        iss >> var;
        index = atoi(&var.at(1));            // Get the process ID
        if (index < 0 || index >= numProc)
        {
            cerr << "Invalid process ID: " << var << endl;
            return 0;
        }

        // Get the number of resources allocated to process "index".
        for (j = 0; j < numResources; j++)
            iss >> allocation[index][j];

        // Get the max allocation to process "index".
        for (j = 0; j < numResources; j++)
            iss >> max[index][j];

        iss.clear();
    }

    // Get the request vector
    int * request = new int[numResources];
    getline(config, line);
    reqStr = line.substr(line.find('<'), line.find('>') - line.find('<') + 1);
    replace(line.begin(), line.end(), '<', ' ');
    replace(line.begin(), line.end(), '>', ' ');
    iss.str(line);
    iss >> var >> pid >> equal;
    for (j = 0; j < numResources; j++)          // Read in the "request" vector
        iss >> request[j];
    iss.clear();

    // Check the request using the Banker's algorithm.
    Banker * banker = new Banker(available, max, allocation, numProc, numResources);
    if (banker -> isSafe(pid, request, sequenceOrReason, numProc))
        cout << "Grant request " << reqStr << " from P" << pid << ".\n"
             << "Sequence: " << sequenceOrReason << "\n";
    else
        cout << "Reject request " << reqStr << " from P" << pid << ".\n"
             << "Reason: " << sequenceOrReason << "\n";
}
