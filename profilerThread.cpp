#include <stdexcept>
#include <process.h>
#include <Windows.h>
#include "profilerThread.h"

RunnableProfiler* RunnableProfiler::me = NULL;

RunnableProfiler* RunnableProfiler::getInstance(){
	if(!me)
		/* Standard global new has been already redefined, so in order to
		 * prevent endless recursion placement new and malloc must be used 
		 * instead 
		 */
		me = new (malloc(sizeof(RunnableProfiler))) RunnableProfiler;

	return me;
}

void RunnableProfiler::run(void* p){
	//Run the thread until stop is set to true (someone called the function end())
	while(!RunnableProfiler::getInstance()->stop){
		Profiler::tick();
		Profiler::getInstance()->report();
		++RunnableProfiler::getInstance()->ticker;
		Sleep(100);
	}
}

void RunnableProfiler::start(){
	//Set the profilers ticker to point to the ticker in this class
	Profiler::getInstance()->setTicker(&(RunnableProfiler::getInstance()->ticker));

	//Start a thread which handles the profilers updating
	if(_beginthread(run, 0, NULL) < 0)
		throw std::runtime_error("Thread initialization failed.");
}