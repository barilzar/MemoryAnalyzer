#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <exception>

void* operator new(size_t size) throw(...);

void* operator new[] (size_t size) throw(...);

void operator delete (void* data);

void operator delete [] (void* data);

//#define __func__ __FUNCTION__
//#define new(size) new(size_t size, const char* name = __func__)

class profiler{
public:
	static profiler& Instance(){
		static profiler singleton;
		return singleton;
	}

	void setStream(std::ostream here){
		output = here;
	}

private:
	profiler(){
		output = std::cout;
	}
	std::ostream output;
};

