#include <csignal>
#include <iostream>
#include <cassert>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <atomic>
#include <thread>
#include <mutex>

using namespace std;

static atomic_bool	stopFlag( false );
static mutex		coutMutex;	

/*void sleep( uint32_t ms)
{
	std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
}
*/

void sigHandler( int sig )
{
	stopFlag = true; 
	std::cout << "set stopFlag to true in " << __FILE__ << "\n";
}

void waitForChildProc()
{
	pid_t childPid = fork();
	if( childPid < 0 )
	{
		std::cout << "failed in creating child process\n";		
	}
	else if( childPid > 0)
	{
		int status;
		// function wait( int* status) : return childPid when child process exited; if no child processes of current process exists, return -1;
		// function waitpid( pid_t childPid/-1(any child process), int* status, int options )
		pid_t _pid = 0;
//		do
//		{
//			lock_guard< mutex > lg( coutMutex );
//
//			sleep( 1 );
//			std::cout << "child process with pid=" << childPid << " is still running , say hello in work thread of main process\n";
//			_pid = waitpid( childPid, &status, WNOHANG );
//		} while( _pid ==0 || _pid == -1);  // -1 means waiting on wrong child process; 0 means child process with childPid doesn't exit
		_pid = waitpid( childPid, &status, 0);

		if( -1 == _pid)	
		{
			std::cout << "unknown child process exited with status=" << status << ", say sorry in work thread of main process\n";
		}
		else
		{
			std::cout  << "child process with pid=" << _pid << " exited, say bye bye in work thread of main process\n";
		}
	}
	else // child process
	{
		sleep( 6 );
		std::cout << "Hi, I'm in child process, bye , see you later\n";
	}
}

int main()
{
	assert( signal( SIGINT, sigHandler ) != SIG_ERR ); // ctrl + c

	thread childThread( waitForChildProc );
	childThread.detach();
	
	while( !stopFlag )
	{
		lock_guard< mutex > lg( coutMutex );
		sleep( 1 );
		std::cout << "say hi in master thread\n";
	}
	std::cout << "say bye bye in master thread\n";

	return 0;
}
