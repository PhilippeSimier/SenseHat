//============================================================================
// Name        : SparkelContainer.cpp
// Author      : Jon Dellaria
// Version     : 1.0
// Description : Is an application that runs 64 containers of the pixelBlink application.
// Each pixelBlink container is focused on blinking 1 pixel on the 8x8 display matrix on the SenseHat.
//============================================================================

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
//#include <DLog.h>
//#include <LinuxCommand.h>

#include <unistd.h>
#include <sys/wait.h>
#include <algorithm>

#include "IsolatedContainer.h"

#define CHILDCONTAINERNAME "/home/jdellaria/Desktop/SenseHat/Projects/pixelBlink"

//#define CHILDCONTAINERNAME "/Users/dellj/eclipse-workspace/piSenseClock/Debug/piSenseClock"

int main_event_handler();
int eventHandler();
int configApp();
//void doRecurseDirectory(char const * text, int x);
//int callChildContainer(std::string text);
int callChildContainer(int row, int column);
int checkChildContainers();
int incrementCurrentlyActiveContainer();
using namespace std;

int songFD = 0;
//DLog myLog;



IsolatedContainer *myContainerPtr;
int currentlyActiveContainer = 0;
long numberOfContainers = 0;

int fixDirectoryCount;


using namespace std;
bool cmdOptionExists(char** begin, char** end, const string& option);
char* getCmdOption(char ** begin, char ** end, const std::string & option);
int removeEmptyDirectory(const char* directory);
void doRemoveEmptyDirectories(char const * text, int x);
void myCallBack(char const * text, int event);
int waitForAllChildContainersToClose();

bool cmdOptionExists(char** begin, char** end, const string& option)
{
    return std::find(begin, end, option) != end;
}

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

int main(int argc, char* argv[])
{
	string message;
	char intbuffer[50];

	time_t Start_t, End_t;
	int time_task1;

    char * STRnumberOfContainers = getCmdOption(argv, argv + argc, "-c");

    if (STRnumberOfContainers)
    {
    	char *pNext;
    	numberOfContainers = strtol (STRnumberOfContainers, &pNext, 10);
    }
    else
    {
    	numberOfContainers = 64;
    }
	message.clear();
	message.append("Number of Containers ");

	sprintf(intbuffer,"%ld",numberOfContainers);
	message.append(intbuffer);
	cout << message << endl;
//	myLog.print(logWarning, message);

	IsolatedContainer *myDummyPtr = new IsolatedContainer[numberOfContainers];
	myContainerPtr = myDummyPtr;


	Start_t = time(NULL);    //record time that task 1 begins
//	myDirectory.Recurse("/home/jdellaria/Desktop/doFiles", doRecurseDirectory);
	message.clear();
	message.append("Waiting for children to close");
	cout << message << endl;
//	myLog.print(logWarning, message);

//	callChildContainer(1,1);
//	callChildContainer(2,2);
//	callChildContainer(3,3);

	for (int column = 0; column < 8; column++)
	{
		for (int row=0; row < 8; row++)
		{
			callChildContainer(row,column);
		}
	}
  message.clear();
	message.append("Waiting For All Child Containers To Close");
	cout << message << endl;
	waitForAllChildContainersToClose();

	End_t = time(NULL);    //record time that task 1 ends
	time_task1 = difftime(End_t, Start_t);    //compute elapsed time of task 1

	message.clear();
	message.append("Task took ");

	sprintf(intbuffer,"%d",time_task1);
	message.append(intbuffer);
	message.append(" seconds.");
	cout << message << endl;

	message = "SparkelContainer.cpp :";
	message.append(__func__);
	message.append(": SparkelContainer exiting Normally");
	cout << message << endl;

}

#define MAIN_EVENT_TIMEOUT 3 // sec unit


//int callChildContainer(string originalFileName)
int callChildContainer(int row, int column)
{
	string destinationFileName;
	string destinationVersionFileName;
//	int returnValue;
	string message;
	string rowString;
	string columnString;
	char const *darg[7]={CHILDCONTAINERNAME, NULL,  NULL,  NULL, NULL};
	std::stringstream ssRow;
	std::stringstream ssColumn;

	ssRow << row;
	rowString.append(ssRow.str());
	ssColumn << column;
	columnString.append(ssColumn.str());

	darg[1] = rowString.c_str();
	darg[2] = columnString.c_str();


	char intbuffer[50];

	do
	{
//		std::cout << "----working on file " << originalFileName << " programID:" << myContainerPtr[currentlyActiveContainer].programID << "\n" ;

//		myLog.print(logDebug, message);

		if ( myContainerPtr[currentlyActiveContainer].programID == 0)
		{
			myContainerPtr[currentlyActiveContainer].OpenChild(darg);
			message.clear();
			message.append("parameteres passed are ");
			message.append(rowString);
			message.append(" and ");
			message.append(columnString);
			message.append(" programID:");
			sprintf(intbuffer,"%d",myContainerPtr[currentlyActiveContainer].programID);
			message.append(intbuffer);
			cout << message << endl;

			incrementCurrentlyActiveContainer();
			break;
		}
		else
		{
			incrementCurrentlyActiveContainer();
		}
		checkChildContainers();
	}while ( true);
	return 0;
}

int incrementCurrentlyActiveContainer()
{
	currentlyActiveContainer++;
	if (currentlyActiveContainer >= numberOfContainers)
	{
		currentlyActiveContainer = 0;
	}
	return 0;
}

int checkChildContainers()
{
	int x;
	for (x = 0; x < numberOfContainers; x++)
	{
//		std::cout << "----checkChildContainers x=" << x << " \n" ;
		if ( myContainerPtr[x].programID != 0)
		{
			if ( myContainerPtr[x].hasChildCompleted() > 0)
			{
				myContainerPtr[x].CloseChild();
			}
		}
	}
//	std::cout << "----exiting checkChildContainers " << " \n" ;
	return 0;
}

int waitForAllChildContainersToClose()
{
	int x;
	int allClosed = 0;
	string message;
	char intbuffer[50];

	do
	{
		allClosed = 0;
		for (x = 0; x < numberOfContainers; x++)
		{
//			std::cout << "----waitForAllChildContainersToClose x=" << x << " \n" ;
			if ( myContainerPtr[x].programID != 0)
			{
				sprintf(intbuffer,"%d",myContainerPtr[x].programID);
				if ( myContainerPtr[x].hasChildCompleted() > 0)
				{
					myContainerPtr[x].CloseChild();
// #define JONDEBUGMESSAGES
#ifdef JONDEBUGMESSAGES // comment this out to get all messages(error and otherwise) from the Child Containers

					if(myContainerPtr[x].errorMessage.length() > 0)
					{
						message.clear();
						message.append("Error:");
						message.append(intbuffer);
						message.append(" ");
						message.append(myContainerPtr[x].errorMessage);
						cout << message << endl;
					}
					if(myContainerPtr[x].incomingMessage.length() > 0)
					{
						message.clear();
						message.append("Incoming Message:");
						message.append(intbuffer);
						message.append(" ");
						message.append(myContainerPtr[x].incomingMessage);
						cout << message << endl;
					}
					if(myContainerPtr[x].outgoingMessage.length() > 0)
					{
						message.clear();
						message.append("Outgoing Message:");
						message.append(intbuffer);
						message.append(" ");
						message.append(myContainerPtr[x].outgoingMessage);
						cout << message << endl;
					}
#endif     // JONDEBUGMESSAGES     
//					std::cout << "----waitForAllChildContainersToClose x=" << x << " has closed \n" ;
				}
			}
			else
			{
				allClosed++; //if any are open.. this will cause the while loop to [erform again.
			}

		}
	}while (allClosed != numberOfContainers);
//	std::cout << "----exiting waitForAllChildContainersToClose " << " \n" ;
	return 0;
}

#ifdef JON
int configApp()
{
	string message;

	myLog.logFileName = myConfig.logFileName;
	myLog.printFile = myConfig.logPrintFile;
	myLog.printScreen = myConfig.logPrintScreen;
	myLog.printTime = myConfig.logPrintTime;

	if (myConfig.logValue.find("logDebug")!=string::npos)
	{
		myLog.logValue = logDebug;
		message = "myLog.logValue = logDebug";
		myLog.print(logInformation, message);
	}
	if (myConfig.logValue.find("logInformation")!=string::npos)
	{
		myLog.logValue = logInformation;
		message = "myLog.logValue = logInformation";
		myLog.print(logInformation, message);
	}
	if (myConfig.logValue.find("logWarning")!=string::npos)
	{
		myLog.logValue = logWarning;
		message = "myLog.logValue = lo(more) arguments, they aregWarning";
		myLog.print(logInformation, message);
	}
	if (myConfig.logValue.find("logError")!=string::npos)
	{
		myLog.logValue = logError;
		message = "myLog.logValue = logError";
		myLog.print(logInformation, message);
	}

	return (1);
}
#endif
