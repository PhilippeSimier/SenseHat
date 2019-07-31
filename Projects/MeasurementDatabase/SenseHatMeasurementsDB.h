 /*
 * SenseHatMeasurementsDB.h
  *
  * Created on: November 4 2018
  *      Author: Jon Dellaria
  *			jdellaria@gmail.com
  *      Store all SenseHat measurements to a mysql Database
  */


#ifndef SenseHatMeasurementsDB_H_
#define SenseHatMeasurementsDB_H_
#include <mysql/mysql.h>
#include <string>
#include <iostream>


class SenseHatMeasurementsDB{
public:

	long id;

 	char time[20];
 	float temp;
	float cpuTemp;
	float hatTemp;
 	float pressure;
 	float humidity;
 	float orientationPitch;
 	float orientationRoll;
 	float orientationYaw;
 	float accelerationX;
 	float accelerationY;
 	float accelerationZ;
 	float magnetismX;
 	float magnetismY;
 	float magnetismZ;
 	float magnetismSphericalRo;
 	float magnetismSphericalTeta;
 	float magnetismSphericalDelta;

	MYSQL	dbaseConnection;

		SenseHatMeasurementsDB();
		~SenseHatMeasurementsDB();
	MYSQL	OpenConnection();
	void	CloseConnection();

	long	addMeasurements();

	void	setTime();
	void	setTemperature(float degreesC);
	void	setCpuTemperature(float degreesC);
	void	setHatTemperature(float degreesC);
	void	setPressure(float pressure);
	void	setHumidity(float  humidity);
	void	setOrientation(float pitch, float Roll, float Yaw);
	void	setAcceleration(float x, float y, float z);
	void	setMagnetism(float x, float y, float z);
	void	setMagnetismSpherical(float Ro, float Teta, float Delta);


};

#endif /* SenseHatMeasurementsDB_H_ */
