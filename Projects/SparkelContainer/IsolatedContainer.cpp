/*
 * IsolatedContainer.cpp
 *
 *  Created on: May 3, 2019
 *      Author: jdellaria
 */

#include "IsolatedContainer.h"

IsolatedContainer::IsolatedContainer() {
	// TODO Auto-generated constructor stub
	programID = 0;
}

IsolatedContainer::~IsolatedContainer() {
	// TODO Auto-generated destructor stub
}


//int MP3Stream::ExecuteDecoder(char* const argv[], int *infd, int *outfd, int *errfd)
int IsolatedContainer::OpenChild(const char*  darg[])
{
	/*main invoke a new child process
	 argv is an array exactly like the array passed to the main routine.
	 This should contain the following in order:

	 1.command to execute at the command line. Example, MPG123
	 2.any parameters. (-s)
	 3.name of music file
	 */
	errorMessage.erase();
	incomingMessage.erase();
	outgoingMessage.erase();
	pipe(infds);
	pipe(outfds);
	pipe(errfds);
	programID = fork();
	if (programID < 0)
	{     /* fork a child process           */
//		perror ("*** ERROR: LinuxCommand::Execute forking child process failed.\n");
		return(-1);
	}
	if(programID == 0)
	{
		// this is a child
		dup2(infds[1],1);// copy output pipe to standard output
		close(infds[0]);
		dup2(errfds[1],2);// copy output pipe to standard error
		close(infds[0]);
		dup2(outfds[0],0);// copy input pipe to standard inputint WaitForCommandToExit()
		close(outfds[1]);
		execvp(*darg,(char* const*)darg); // this should not return
		exit(0);
	}
	else
	{
		int i;

		/* in parent */
		close(infds[1]);                /* parent closes p[1], stdout     */
		close(errfds[1]);
		close(outfds[1]);

		return (programID);
	}
}

void IsolatedContainer::CloseChild()
{
	int i;
	char returnBuffer[1000];
	do
	{
//		std::cout << "Waiting for child to complete"<< "\n";
		i = read(infds[0], returnBuffer, RETURNBUFFERLENGTH-1); // get 1st 1023 bytes of the string returned from program that was executed
		returnBuffer[i] = '\0';         /* string terminator     */
		incomingMessage.append(returnBuffer);
		i = read(errfds[0], returnBuffer, RETURNBUFFERLENGTH-1); // get 1st 1023 bytes of the string returned from program that was executed
		returnBuffer[i] = '\0';         /* string terminator     */
		errorMessage.append(returnBuffer);
		i = read(outfds[0], returnBuffer, RETURNBUFFERLENGTH-1); // get 1st 1023 bytes of the string returned from program that was executed
		returnBuffer[i] = '\0';         /* string terminator     */
		outgoingMessage.append(returnBuffer);
		usleep(100);
	}while (!hasChildCompleted()); // we have to use this command to wait for the child process to complete.

	close(infds[0]); //close stdin	otherwise we have a bunch of child processes forked.
	close(errfds[0]); //close errin
	close(outfds[0]); //close stdout
	programID = 0;
}

int IsolatedContainer::hasChildCompleted()
{
    int Stat;
    pid_t wpid = waitpid(programID, &Stat, WNOHANG);
    if (wpid == 0)
    {
    	return 0; //if waitpid returns 0 the status has not changed.
    }

	if (WIFEXITED(Stat)) // returns true if the child terminated normally, that is, by calling exit(3) or _exit(2), or by returning from main().
		return 1;
	if (WIFSIGNALED(Stat)) //  returns true if the child process was terminated by a signal.
		return 1;
	if (WIFSTOPPED(Stat)) // returns true if the child terminated normally, that is, by calling exit(3) or _exit(2), or by returning from main().
		return 1;
	if (WIFCONTINUED(Stat)) //  returns true if the child process was terminated by a signal.
		return 1;
	return -1;

}
