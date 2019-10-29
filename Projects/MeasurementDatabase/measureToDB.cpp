/*
 * measureToDB.cpp
 *
 * Created on: November 4 2018
 *      Author: Jon Dellaria
 *			jdellaria@gmail.com
 *      Store all SenseHat measurements to a mysql Database
 */


#include <SenseHat.h>
#include <iostream>

#include "SenseHatMeasurementsDB.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <locale.h>
#include <fcntl.h>
#include <time.h>

#include <iconv.h>
#include <dirent.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <iostream>
#include <string>

#define         MASKBITS                0x3F
#define         MASKBYTE                0x80
#define         MASK2BYTES              0xC0
#define         MASK3BYTES              0xE0
#define         MASK4BYTES              0xF0
#define         MASK5BYTES              0xF8
#define         MASK6BYTES              0xFC

typedef unsigned short   Unicode2Bytes;
typedef unsigned int     Unicode4Bytes;
typedef unsigned char    byte;

using namespace std;


int main(){

    SenseHat senseHat;

    float pressure;
    float temperature;
    float cpuTemperature;
    float hatTemperature;
    float humidity;
    float xa,ya,za,xm,ym,zm;
    float pitch,roll,yaw;

    senseHat.WipeScreen();
    COLOR_SENSEHAT green  = senseHat.ConvertRGB565("#009900");
    COLOR_SENSEHAT red = senseHat.ConvertRGB565(255,0,0);
    int i=0;
    COLOR_SENSEHAT pix0 = red;
    COLOR_SENSEHAT pix1 = green;
    COLOR_SENSEHAT pix2 = BLUE;
    sleep(5); // wait for mysql to get up and running .. assuming we are being executed from boot
    SenseHatMeasurementsDB myDB;

    for (i=0; i<8; i++){
      senseHat.LightPixel(0, i, green);
     	senseHat.LightPixel(1, i, green);
     	senseHat.LightPixel(2, i, green);
   	sleep(1);
    }

    while(1)
    {
      pressure    = senseHat.GetPressure();
      hatTemperature = senseHat.getRawTemperature();
      cpuTemperature = senseHat.getCpuTemperature();
      temperature = senseHat.correctTemperature(hatTemperature, cpuTemperature);
      humidity    = senseHat.GetHumidity();

      usleep(20*1000);
      senseHat.GetAcceleration(xa,ya,za);

      usleep(20*1000);
      senseHat.GetOrientation(pitch,roll,yaw);

      usleep(20*1000);
      senseHat.GetMagnetism(xm,ym,zm);

      system("clear");
      std::cout << "Barometric Pressure: " << pressure << " hPa"<< std::endl;
      std::cout << "Temperature: " << temperature << " C" << std::endl;
      std::cout << "Humidity: " << humidity << " %" << std::endl;
      std::cout << "Acceleration x: " << xa << "(g) y : " << ya << "(g) z : " << za << "(g)" << std::endl;
      std::cout << "Orientation Pitch: " << pitch << " roll : " << roll << " yaw : " << yaw << std::endl;
      std::cout << "Magnetism x: " << xm << "(T) y : " << ym << "(T) z : " << zm << "(T)" << std::endl;
      std::cout << "pix0: " << pix0 << " pix1: " << pix1 << " pix2: " << pix2 << "" << std::endl;
     	senseHat.LightPixel(0, i, pix0);
      senseHat.LightPixel(1, i, pix1);
     	senseHat.LightPixel(2, i, pix2);
      myDB.setTemperature(temperature);
      myDB.setCpuTemperature(cpuTemperature);
      myDB.setHatTemperature(hatTemperature);
      myDB.setPressure(pressure);
      myDB.setHumidity(humidity);
      myDB.setOrientation(pitch, roll, yaw);
      myDB.setAcceleration(xa, ya, za);
      myDB.setMagnetism(xm, ym, zm);
      myDB.setMagnetismSpherical(0, 0, 0);

      myDB.addMeasurements();
      i++;
      if (i >= 8)
      {
        i = 0;
        std::cout << "in if : " << std::endl;
//        senseHat.Effacer();
        if(pix0 == red)
        {
          std::cout << "pix0 == red: " << std::endl;
          pix0 = BLUE;
          pix1 = red;
          pix2 = green;
        }
        else if(pix0 == BLUE)
        {
          std::cout << "pix0 == blue: " << std::endl;
          pix0 = green;
          pix1 = BLEU;
          pix2 = red;
        }
        else if(pix0 == green)
        {
          std::cout << "pix0 == green: " << std::endl;
          pix0 = red;
          pix1 = green;
          pix2 = BLUE;
        }

      }
	sleep(300);
//      usleep(500*1000);
    }
}

SenseHatMeasurementsDB::SenseHatMeasurementsDB()
{
	OpenConnection();
	id = 0;

 	time[0] = 0;
 	temp = 0;
 	pressure = 0;
 	humidity = 0;
 	orientationPitch = 0;
 	orientationRoll = 0;
 	orientationYaw = 0;
 	accelerationX = 0;
 	accelerationY = 0;
 	accelerationZ = 0;
 	magnetismX = 0;
 	magnetismY = 0;
 	magnetismZ = 0;
 	magnetismSphericalRo = 0;
 	magnetismSphericalTeta = 0;
 	magnetismSphericalDelta = 0;
}


SenseHatMeasurementsDB::~SenseHatMeasurementsDB()
{
	CloseConnection();
}


void SenseHatMeasurementsDB::setTime()
{

}


void SenseHatMeasurementsDB::setTemperature(float degreesC)
{
	temp = degreesC;
}


void SenseHatMeasurementsDB::setHatTemperature(float degreesC)
{
	hatTemp = degreesC;
}

void SenseHatMeasurementsDB::setCpuTemperature(float degreesC)
{
	cpuTemp = degreesC;
}



void SenseHatMeasurementsDB::setPressure(float myPressure)
{
	pressure = myPressure;
}


void SenseHatMeasurementsDB::setHumidity(float  myHumidity)
{
	humidity = myHumidity;
}

void SenseHatMeasurementsDB::setOrientation(float pitch, float Roll, float Yaw)
{
 	orientationPitch = pitch;
 	orientationRoll = Roll;
 	orientationYaw = Yaw;
}

void SenseHatMeasurementsDB::setAcceleration(float x, float y, float z)
{
 	accelerationX = x;
 	accelerationY = y;
 	accelerationZ = z;
}


void SenseHatMeasurementsDB::setMagnetism(float x, float y, float z)
{
 	magnetismX = x;
 	magnetismY = y;
 	magnetismZ = z;
}

void SenseHatMeasurementsDB::setMagnetismSpherical(float Ro, float Teta, float Delta)
{
 	magnetismSphericalRo = Ro;
 	magnetismSphericalTeta = Teta;
 	magnetismSphericalDelta = Delta;
}


// Database Connection
MYSQL SenseHatMeasurementsDB::OpenConnection()
{
	int	err = 0;
	/* now to connect to the database */

	if(mysql_init(&dbaseConnection) == NULL)
	{
		err = 1;
	}
	else
	{
		if(mysql_real_connect(&dbaseConnection,"localhost","root","dlirius","SenseHat",0,NULL,0) == NULL)
			err = 1;
	}
	if(err)
	{
		printf("Failed to connect to database: Error: %s\n", mysql_error(&dbaseConnection));
		return(dbaseConnection);
	}
	return (dbaseConnection);
}

void SenseHatMeasurementsDB::CloseConnection()
{
	mysql_close(&dbaseConnection);
}

long SenseHatMeasurementsDB::addMeasurements()	//album must be set before calling function
{
	char SQLStmt[1000];
//	char tempAlbum[256];
//	char tempArtist[256];
//	char tempYear[256];
//	long albumID;
	MYSQL_RES *queryResult;
	MYSQL_ROW row;
	int nrows;

//	strcpy (tempAlbum,album);
//	normalizeString(tempAlbum, 149);
//	mysql_real_escape_string(&dbaseConnection, tempAlbum, album, strlen(album));
//	mysql_real_escape_string(&dbaseConnection, tempArtist, artist, strlen(artist));

  sprintf(SQLStmt, "INSERT INTO SenseHat.Measurements (temp, cpuTemp, hatTemp, pressure, humidity, orientationPitch, orientationRoll, orientationYaw, accelerationX, accelerationY, accelerationZ, magnetismX, magnetismY, magnetismZ, magnetismSphericalRo, magnetismSphericalTeta, magnetismSphericalDelta) values ( %5.5f ,%5.5f ,%5.5f ,%5.5f ,%5.5f ,%5.5f ,%5.5f ,%5.5f ,%5.5f ,%5.5f ,%5.5f ,%5.5f ,%5.5f ,%5.5f ,%5.5f ,%5.5f ,%5.5f );", temp, cpuTemp, hatTemp, pressure, humidity, orientationPitch, orientationRoll, orientationYaw, accelerationX, accelerationY, accelerationZ,  magnetismX, magnetismY, magnetismZ, magnetismSphericalRo, magnetismSphericalTeta, magnetismSphericalDelta);


	if (mysql_query(&dbaseConnection, SQLStmt))
	{
		fprintf(stderr, " addMeasurements\n");
		fprintf(stderr, " %s\n",  mysql_error(&dbaseConnection));
		return(0);
	}


	return(1);
}
