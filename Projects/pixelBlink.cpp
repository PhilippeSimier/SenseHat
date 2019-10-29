/*
 * pixelBlink.cpp
 *
 * Created on: July 31 2019
 *      Author: Jon Dellaria
 *			jdellaria@gmail.com
 *      make a pixel blink on the 8x8 display martix of the SenseHat for the raspberry pi
 */

#include <SenseHat.h>
#include <iostream>
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
	int row;
	int column;
	SenseHat senseHAT;
	int redIntensity;
	int greenIntensity;
	int blueIntensity;
	int minimum = 0;
	int maximum = 255;
  COLOR_SENSEHAT color;

	cout << "Light a Pixel on Pi with a random color" << endl; // prints !!!Hello World!!!
	std::srand(std::time(nullptr) + getpid()); // use current time as seed for random generator
/*
	redIntensity = minimum + std::rand()%(maximum - minimum);
	greenIntensity = minimum + std::rand()%(maximum - minimum);
	blueIntensity = minimum + std::rand()%(maximum - minimum);
	std::cout << "redIntensity: " << redIntensity << endl;
	std::cout << "greenIntensity: " << greenIntensity << endl;
	std::cout << "blueIntensity: " << blueIntensity << endl;
  COLOR_SENSEHAT color = senseHAT.ConvertRGB565(redIntensity,greenIntensity,blueIntensity);
	printf ("argc: %d.\n",argc);
	*/
	if (argc != 3)
	{
		printf ("pixelBlink <row (0-7)> <column (0-7)>.\n");
	}
	else
	{
		row = atoi (argv[1]); // get row from command line
		column = atoi (argv[2]);  // get column from command line
		// senseHAT.AllumerPixel(row, column, vert);
		printf ("Row: %d Column: %d.\n",row,column);
		for(int x = 0; x < 60;x++ ) // do 60 loops... or with 1 second delay.. it would be 1 minute
		{
			redIntensity = minimum + std::rand()%(maximum - minimum); // random intensity for red
			greenIntensity = minimum + std::rand()%(maximum - minimum);  // random intensity for green
			blueIntensity = minimum + std::rand()%(maximum - minimum); // random intensity for blue
			color = senseHAT.ConvertRGB565(redIntensity,greenIntensity,blueIntensity); //create color with red, green and blue filiments
			senseHAT.LightPixel(row, column, color);
			sleep(1); //sleep for 1 second
		}
	}
	return 0;
}
