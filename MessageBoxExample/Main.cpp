#include <Windows.h>	// Sleep, MessageBoxA
#include <thread>		// std::thread

// Macro definition for the code which is shared between the threads.
// Any other functions or classes etc can be used here instead but this is done
// because I wanted to retain the internal name of the current function.
// This just shows a MessageBox with a message saying which function is calling.
#define ThreadCall	MessageBoxA(0, "Hello from " __FUNCDNAME__, __FUNCDNAME__, MB_ICONEXCLAMATION); \
					g_iCompletedThreads++

// Global variable to show how many threads have been completed.
int g_iCompletedThreads = 0;

void Thread0(void)
{
	ThreadCall;
}

void Thread1(void)
{
	ThreadCall;
}

void main(void)
{
	// Create the new thread instances. Once the std::thread constructor is called 
	// with a valid function, the thread is started.
	std::thread Thread_Instance_One(Thread0);
	std::thread Thread_Instance_Two(Thread1);

	// Wait until both threads have said they are done.
	while (g_iCompletedThreads != 2) 
		Sleep(100); // Sleep to save CPU cycles.

	// Join both of our new threads with the main thread to prevent an abort() call error.
	Thread_Instance_One.join();
	Thread_Instance_Two.join();
}