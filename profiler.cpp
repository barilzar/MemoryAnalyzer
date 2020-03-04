#include "profiler.h"

Profiler* Profiler::me = NULL;

Profiler* Profiler::getInstance(){
	if(!me)
		/*Use placement new and malloc for instantiating in order to bypass 
		the operator new (which has been redefined)*/
		me = new (malloc(sizeof(Profiler))) Profiler;

	return me;
}

void* Profiler::allocate(size_t size) throw(...){
	void* p = malloc(size);

	if(!p)
		throw std::bad_alloc();

	//Increase the memory allocation counter appropriately
	currentlyAllocated	+= size;

	//Check if a new peak was achieved
	if(peakAllocation < currentlyAllocated){
		peakAllocation = currentlyAllocated;
		peakTick = *currentTickAddr;
	}

	Profiler::getInstance()->trackPointer(p, size);

	return p;
}

void Profiler::deallocate(void* data){
	//If a pointer, which is not tracked, is deallocated nothing is done
	if(trackedPointers->find(data) == trackedPointers->end())
		return;

	currentlyAllocated -= trackedPointers->operator [](data);

	Profiler::getInstance()->untrackPointer(data);

	free(data);
}


void Profiler::reset(){
	currentlyAllocated	= 0;
	peakAllocation		= 0;
	allocationSum		= 0;

	startTick			= *currentTickAddr;
	peakTick			= startTick;

	trackedPointers->clear();
}

void Profiler::report(){
	uint64_t ticksPassed = *currentTickAddr - startTick;

	//Check for the case of dividing by zero (in the last output line)
	if(ticksPassed == 0)
		ticksPassed = 1;

	(*outputStream) << "On tick: " << *currentTickAddr << " memory used in total: " << currentlyAllocated << " for: " << getCurrentTrackees() << " objects.\n";
	(*outputStream) << "Current peak at tick: " << peakTick << " with value: " << peakAllocation << ".\n";
	(*outputStream) << "Memory use average: " << (uint64_t) (allocationSum / ticksPassed) << std::endl;
}

void Profiler::setNewOutputStream(std::ofstream* o){
	//Check if the stream is controlled by this object or not:
	if(iHazStreamz){
		//Get rid of the previous stream
		outputStream->close();
		delete outputStream;
	}
	//Copy the new stream
	outputStream = o;
}
