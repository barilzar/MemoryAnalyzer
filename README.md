# MemoryAnalyzer
A simple utility for observing the runtime memory consumption of a program.

Actual concumption can be viewed from a log file createted.

Simply include the header to an existing C++ program source and call tick() every time you want to know the current amount of memory in use by the program. Also function report() will create output of the current consumption to a specified file.

Check the classes profilerThread.cpp and main.cpp form a simple example case of how to use this untility in a threaded instance.
