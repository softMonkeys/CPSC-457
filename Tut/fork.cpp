/*
#include <iostream>
#include <cstdio>
#include <unistd.h>
using namespace std;

int main()
{
	pid_t pid;
	cout<<"Hello World"<<endl;
	
	pid = fork();
	
	if(pid > 0)
		cout<<"I'm the Parent and the PID of my child is "<<pid<<endl;	//parent == 1
	else
		cout<<"I'm the Child"<<endl;	// child == 0
	
	cout<<"Goodbye World"<<endl;
   
   return 0;
}
*/
#include <iostream>
#include <cstdio>
#include <unistd.h>

using namespace std;

int main()
{
    pid_t pid;
	cout<<"Hello World"<<endl;
	pid=fork();
	pid=fork();
	pid=fork();
	cout<<"Goodbye World "<<pid<<endl;

   return 0;
}
