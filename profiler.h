#include <memory>
#include <map>
#include <functional>
#include <fstream>
#include "allocator.h"

//Visual studio doesn't understand c99 :(
typedef unsigned long long int uint64_t;

class Profiler{
public:
	//Get a pointer to the profiler
	static Profiler* getInstance();

	//Allocation and deallocation of memory
	void* allocate(size_t);
	void deallocate(void*);

	//Get the current allocations in total
	inline uint64_t getCurrentAllocation(){
		return currentlyAllocated;
	}

	//Get the current amount of tracked pointers
	inline unsigned int getCurrentTrackees(){
		return trackedPointers->size();
	}

	//Set a new output stream for the profiler
	void setNewOutputStream(std::ofstream*);

	//Set an outside ticker for the profiler
	inline void setTicker(uint64_t const* newTicker){
		reset();
		currentTickAddr = newTicker;
	}

	//Call this function each time a new measurement should be done (must be static in order to be threadable)
	static inline void tick(){
		Profiler::getInstance()->allocationSum += Profiler::getInstance()->currentlyAllocated;
	}

	//Writes one report to the output stream
	void report();

	//Resets the profiler statistics
	void reset();

	//Including and excluding of pointers in the profiler
	inline void trackPointer(void* p, size_t size){
		/* The passing of a NULL pointer will be treated as an error but 
		 * if a pointer which is already being tracked is passed again for 
		 * tracking nothing is done and the operation is valid.
		 */
		
		if(p == NULL)
			throw std::invalid_argument("The NULL pointer was passed for tracking.");

		//Apparently one can't chain operators which is why an actual method call is needed
		trackedPointers->operator [](p) = size;
	}

	inline void untrackPointer(void* p){
		/*No checks on the pointer are done as untracking of a pointer that was never 
		tracked is a valid operation (as is the untracking of NULL as well)*/
		trackedPointers->erase(p);
	}

private:
	//Typedef for the type of the pointer map used by this class, the need for it is quite obvious...
	typedef std::map<void* , size_t , std::less<void*>, BypassAllocator<std::pair<void*, size_t>>> pointerMap;

	//Data structures:
	uint64_t			currentlyAllocated;	//The amount of memory currently beign tracked
	uint64_t const*		currentTickAddr;	//The address for the current tick (i.e. the amount of measurements done)
	uint64_t			peakAllocation;		//The max amount of memory ever tracked
	uint64_t			peakTick;			//The tick on which the previous occured
	uint64_t			startTick;			//The tick when the profiler was last reset
	double				allocationSum;		//The sum of all the memory ever tracked (used to count the average)
	pointerMap*			trackedPointers;	//A map for all the pointers tracked
	std::ofstream*		outputStream;		//Output stream for the textual reports
    static Profiler*	me;					//Pointer to the profiler (this is a singleton class)

	//Flags:
	bool	iHazStreamz;	//Tells this object if an outside stream is used or not

	Profiler(uint64_t startMemory = 0, uint64_t const* tickAddr = NULL, const char filePath[11] = "memlog.log"):
		currentlyAllocated(startMemory),
		peakAllocation(startMemory),
		allocationSum((double) startMemory){

		if(!tickAddr){
			//No ticker was specified => create one
			currentTickAddr = new (malloc(sizeof(uint64_t))) uint64_t(1);
		}
		else{
			//A ticker was specified, use that
			currentTickAddr =  tickAddr;
		}

		peakTick	= *currentTickAddr;
		startTick	= *currentTickAddr;

		//Iniatilize objects
		trackedPointers = new (malloc(sizeof(pointerMap))) pointerMap();
		outputStream	= new (malloc(sizeof(std::ofstream))) std::ofstream(filePath, std::ios_base::trunc);
		iHazStreamz		= true;
	}

	//Make the copying and destructing private in order prevent the use of these operations
	Profiler(const Profiler&);
	Profiler& operator= (const Profiler&);
	~Profiler(){};
};

//Overrides for the global news and deletes
inline void* operator new(size_t size) throw(...){
	return Profiler::getInstance()->allocate(size);
}

inline void* operator new[] (size_t size) throw(...){
	return Profiler::getInstance()->allocate(size);
};

inline void* operator new(size_t size, const std::nothrow_t& n) throw(){
	//A bit uggly, but this way all forms of new call allocate()
	try{
		return Profiler::getInstance()->allocate(size);
	}catch(std::bad_alloc){
		return NULL;
	}
}

inline void* operator new[](size_t size, const std::nothrow_t& n) throw(){
	//Call the non-array version of this operator in order to reduce code duplication
	return ::operator new(size, n);
}

inline void operator delete(void* data){
	Profiler::getInstance()->deallocate(data);
}

inline void operator delete [](void* data){
	Profiler::getInstance()->deallocate(data);
}
