#include <stdlib.h>
#include <vector>
#include <iostream>
#include <windows.h>
#include "profilerThread.h"

using std::vector;
using std::cout;
using std::endl;

int main(int argc, char* argv[]){
	int objectCount,
		objectSize,
		time;

	char* cache;
	
	//arg format: program <object count> <object size> <sleep time>
	if(argc != 4){
		cout << "Use: program <object count> <object size> <sleep time>" << endl;
		return(EXIT_FAILURE);
	}

	objectCount = atoi(argv[1]);
	objectSize  = atoi(argv[2]);
	time		= atoi(argv[3]);

	if(objectCount <= 0 || objectSize <= 0 || time <= 0){
		cout << "Invalid parameters, use only positive integers." << endl;
		return(EXIT_FAILURE);
	}

	RunnableProfiler::getInstance()->start();

	for(int i = 0; i < objectCount; ++i){
		cache = new char[objectSize];
		Sleep(time);
	}

	char c;
	std::cin >> c;

	RunnableProfiler::getInstance()->end();

	return(EXIT_SUCCESS);
}