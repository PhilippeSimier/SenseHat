/*
 * digitalClock.cpp
 *
 * Created on: July 31 2019
 *      Author: Jon Dellaria
 *			jdellaria@gmail.com
 *      disply the time on the 8x8 display with no scroling
 */


#include <SenseHat.h>
#include <iostream>
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>
#include <time.h>

using namespace std;

int piDisplayCharacter[] = {
		0,1,1,1, //Zero
		0,1,0,1,
		0,1,0,1,
		0,1,1,1,
		0,0,1,0, // One
		0,1,1,0,
		0,0,1,0,
		0,1,1,1,
		0,1,1,1, // Two
		0,0,1,1,
		0,1,1,0,
		0,1,1,1,
		0,1,1,1, // Three
		0,0,1,1,
		0,0,1,1,
		0,1,1,1,
		0,1,0,1, // Four
		0,1,1,1,
		0,0,0,1,
		0,0,0,1,
		0,1,1,1, // Five
		0,1,1,0,
		0,0,1,1,
		0,1,1,1,
		0,1,0,0, // Six
		0,1,1,1,
		0,1,0,1,
		0,1,1,1,
		0,1,1,1, // Seven
		0,0,0,1,
		0,0,1,0,
		0,1,0,0,
		0,1,1,1, // Eight
		0,1,1,1,
		0,1,1,1,
		0,1,1,1,
		0,1,1,1, // Nine
		0,1,0,1,
		0,1,1,1,
		0,0,0,1,
		0,0,0,0, // Blank
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
};


int hour_color[] = {0,0,255}; // Blue
int minute_color[] = {248,226,55}; // GeekPub Yellow
int empty[] = {0,0,0}; // Black

int display_image[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0
};

int hour = 4;
int minute = 0;
int setRawDisplay(int row, int column, int color)
{
	display_image[row + column*8] = color;
}

struct tm * getTime()
{
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

    return timeinfo;
}

int setClockDisplay()
{
// Map digits to the clock_image array
	int pixel_offset = 0;
	int index = 0;
  int hiHour;
  int lowHour;
  int hiMinute;
  int lowMinute;
  struct tm * myTime;

  myTime = getTime();
  hiHour = myTime->tm_hour/10;
  lowHour = myTime->tm_hour%10;
  hiMinute = myTime->tm_min/10;
  lowMinute = myTime->tm_min%10;
  //cout << "hiHour: " << hiHour << " lowHour: " << lowHour << endl;
  //cout << "hiMinute: " << hiMinute << " lowMinute: " << lowMinute << endl;

	for(int quadrant = 0; quadrant < 4;quadrant++)
	{
		int row = 0;
		int column = 0;
		int characterOffset = hour * 16;
		int rowOffset = 0;
		int columnOffset = 0;
		if (quadrant == 0) // Hi Hour
		{
			rowOffset = 0;
			columnOffset = 0;
      characterOffset = hiHour * 16;
		}
		if (quadrant == 1) // Hi Minute
		{
			rowOffset = 0;
			columnOffset = 4;
      characterOffset = hiMinute * 16;
		}
		if (quadrant == 2) // Low Hour
		{
			rowOffset = 4;
			columnOffset = 0;
      characterOffset = lowHour * 16;
		}

		if (quadrant == 3) // Low Minute
		{
			rowOffset = 4;
			columnOffset = 4;
      characterOffset = lowMinute * 16;
		}
		setRawDisplay( 0 + rowOffset, 0 + columnOffset,  piDisplayCharacter[(characterOffset)]);
		setRawDisplay( 1 + rowOffset, 0 + columnOffset,  piDisplayCharacter[(characterOffset + 1)]);
		setRawDisplay( 2 + rowOffset, 0 + columnOffset,  piDisplayCharacter[(characterOffset + 2)]);
		setRawDisplay( 3 + rowOffset, 0 + columnOffset,  piDisplayCharacter[(characterOffset + 3)]);
		setRawDisplay( 0 + rowOffset, 1 + columnOffset,  piDisplayCharacter[(characterOffset + 4)]);
		setRawDisplay( 1 + rowOffset, 1 + columnOffset,  piDisplayCharacter[(characterOffset + 5)]);
		setRawDisplay( 2 + rowOffset, 1 + columnOffset,  piDisplayCharacter[(characterOffset + 6)]);
		setRawDisplay( 3 + rowOffset, 1 + columnOffset,  piDisplayCharacter[(characterOffset + 7)]);
		setRawDisplay( 0 + rowOffset, 2 + columnOffset,  piDisplayCharacter[(characterOffset + 8)]);
		setRawDisplay( 1 + rowOffset, 2 + columnOffset,  piDisplayCharacter[(characterOffset + 9)]);
		setRawDisplay( 2 + rowOffset, 2 + columnOffset,  piDisplayCharacter[(characterOffset + 10)]);
		setRawDisplay( 3 + rowOffset, 2 + columnOffset,  piDisplayCharacter[(characterOffset + 11)]);
		setRawDisplay( 0 + rowOffset, 3 + columnOffset,  piDisplayCharacter[(characterOffset + 12)]);
		setRawDisplay( 1 + rowOffset, 3 + columnOffset,  piDisplayCharacter[(characterOffset + 13)]);
		setRawDisplay( 2 + rowOffset, 3 + columnOffset,  piDisplayCharacter[(characterOffset + 14)]);
		setRawDisplay( 3 + rowOffset, 3 + columnOffset,  piDisplayCharacter[(characterOffset + 15)]);
	}
}

int printClock()
{
	printf("%d %d %d %d %d %d %d %d\n",display_image[0],display_image[1],display_image[2],display_image[3],display_image[4],display_image[5],display_image[6],display_image[7]);
	printf("%d %d %d %d %d %d %d %d\n",display_image[8],display_image[9],display_image[10],display_image[11],display_image[12],display_image[13],display_image[14],display_image[15]);
	printf("%d %d %d %d %d %d %d %d\n",display_image[16],display_image[17],display_image[18],display_image[19],display_image[20],display_image[21],display_image[22],display_image[23]);
	printf("%d %d %d %d %d %d %d %d\n",display_image[24],display_image[25],display_image[26],display_image[27],display_image[28],display_image[29],display_image[30],display_image[31]);
	printf("%d %d %d %d %d %d %d %d\n",display_image[32],display_image[33],display_image[34],display_image[35],display_image[36],display_image[37],display_image[38],display_image[39]);
	printf("%d %d %d %d %d %d %d %d\n",display_image[40],display_image[41],display_image[42],display_image[43],display_image[44],display_image[45],display_image[46],display_image[47]);
	printf("%d %d %d %d %d %d %d %d\n",display_image[48],display_image[49],display_image[50],display_image[51],display_image[52],display_image[53],display_image[54],display_image[55]);
	printf("%d %d %d %d %d %d %d %d\n",display_image[56],display_image[57],display_image[58],display_image[59],display_image[60],display_image[61],display_image[62],display_image[63]);
}


int setTemperatureDisplay(float myTemp)
{
// Map digits to the clock_image array
	int pixel_offset = 0;
	int index = 0;
  int hiTemp;
  int lowTemp;
  int hiMinute = 0;
  int lowMinute = 0;
	int roundedTemp;

	roundedTemp = myTemp;
  cout << "myTemp: " << myTemp << " roundedTemp: " << roundedTemp << endl;

  hiTemp = roundedTemp/10;
  lowTemp = roundedTemp%10;

  cout << "hiTemp: " << hiTemp << " lowTemp: " << lowTemp << endl;

	for(int quadrant = 0; quadrant < 4;quadrant++)
	{
		int row = 0;
		int column = 0;
		int characterOffset = hour * 16;
		int rowOffset = 0;
		int columnOffset = 0;
		if (quadrant == 0) // Hi Temperature
		{
			rowOffset = 0;
			columnOffset = 0;
      characterOffset = hiTemp * 16;
		}
		if (quadrant == 1) // Hi Minute
		{
			rowOffset = 0;
			columnOffset = 4;
      characterOffset = 10 * 16; // Blank
		}
		if (quadrant == 2) // Low Temperature
		{
			rowOffset = 4;
			columnOffset = 0;
      characterOffset = lowTemp * 16;
		}

		if (quadrant == 3) // Low Minute
		{
			rowOffset = 4;
			columnOffset = 4;
      characterOffset = 10 * 16;// Blank
		}
		setRawDisplay( 0 + rowOffset, 0 + columnOffset,  piDisplayCharacter[(characterOffset)]);
		setRawDisplay( 1 + rowOffset, 0 + columnOffset,  piDisplayCharacter[(characterOffset + 1)]);
		setRawDisplay( 2 + rowOffset, 0 + columnOffset,  piDisplayCharacter[(characterOffset + 2)]);
		setRawDisplay( 3 + rowOffset, 0 + columnOffset,  piDisplayCharacter[(characterOffset + 3)]);
		setRawDisplay( 0 + rowOffset, 1 + columnOffset,  piDisplayCharacter[(characterOffset + 4)]);
		setRawDisplay( 1 + rowOffset, 1 + columnOffset,  piDisplayCharacter[(characterOffset + 5)]);
		setRawDisplay( 2 + rowOffset, 1 + columnOffset,  piDisplayCharacter[(characterOffset + 6)]);
		setRawDisplay( 3 + rowOffset, 1 + columnOffset,  piDisplayCharacter[(characterOffset + 7)]);
		setRawDisplay( 0 + rowOffset, 2 + columnOffset,  piDisplayCharacter[(characterOffset + 8)]);
		setRawDisplay( 1 + rowOffset, 2 + columnOffset,  piDisplayCharacter[(characterOffset + 9)]);
		setRawDisplay( 2 + rowOffset, 2 + columnOffset,  piDisplayCharacter[(characterOffset + 10)]);
		setRawDisplay( 3 + rowOffset, 2 + columnOffset,  piDisplayCharacter[(characterOffset + 11)]);
		setRawDisplay( 0 + rowOffset, 3 + columnOffset,  piDisplayCharacter[(characterOffset + 12)]);
		setRawDisplay( 1 + rowOffset, 3 + columnOffset,  piDisplayCharacter[(characterOffset + 13)]);
		setRawDisplay( 2 + rowOffset, 3 + columnOffset,  piDisplayCharacter[(characterOffset + 14)]);
		setRawDisplay( 3 + rowOffset, 3 + columnOffset,  piDisplayCharacter[(characterOffset + 15)]);
	}
}

int main()
{

	SenseHat senseHat;
	float x,y,z;
	int angle;
	COLOR_SENSEHAT  green  = senseHat.ConvertRGB565("#009900");
	COLOR_SENSEHAT  black  = senseHat.ConvertRGB565("#000000");
	float temperature;
	float temperatureF;

	senseHat.WipeScreen();
	senseHat << setcouleur(senseHat.ConvertRGB565(64,84,0));

	do
	{
//		setClockDisplay();
		temperature = senseHat.GetTemperature();
		//convert to Fahrenheit
		cout << "Celcius: " << temperature << endl;

		temperatureF = (temperature*9/5) + 32;
		setTemperatureDisplay(temperatureF);

		for (int column = 0; column < 8; column++)
		{
		  for (int row=0; row < 8; row++)
		  {
		    if (display_image[column + (row * 8)] == 1)
		    {
		      senseHat.LightPixel(row, column, green);
		    }
				if (display_image[column + (row * 8)] == 0)
				{
					senseHat.LightPixel(row, column, black);
				}
		  }
		}

		sleep(1);

   }while(z > -0.5);

	return 0;
}
