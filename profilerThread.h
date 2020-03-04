#include "profiler.h"

class RunnableProfiler{
public:
	static RunnableProfiler* getInstance();

	inline void end(){
		stop = true;
	}

	void start();

private:
	bool stop;
	uint64_t ticker;
	static RunnableProfiler* me;

	RunnableProfiler(){
		stop = false;
		ticker = 1;
	}
	RunnableProfiler(const RunnableProfiler&);
	RunnableProfiler& operator= (const RunnableProfiler&);

	//This function must be defined like this in order to comply with the win API
	static void run(void* p);
};