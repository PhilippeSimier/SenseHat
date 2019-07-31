/*
 * IsolatedContainer.h
 *
 *  Created on: May 3, 2019
 *      Author: jdellaria
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>



#include <unistd.h>
#include <stdio.h>

#ifndef ISOLATEDCONTAINER_H_
#define ISOLATEDCONTAINER_H_
#define RETURNBUFFERLENGTH 1000
class IsolatedContainer {
	int infds[2];
	int errfds[2];
	int outfds[2];

public:
	IsolatedContainer();
	virtual ~IsolatedContainer();
	int OpenChild(const char*  darg[]);
	void CloseChild();
	int hasChildCompleted();
	std::string errorMessage;
	std::string incomingMessage; // this will be the stdout from the container
	std::string outgoingMessage;
	pid_t programID;
};

#endif /* ISOLATEDCONTAINER_H_ */
