# measureToDB

# Description
**measureToDB** is an application that runs 64 containers of the PixelBlink application. Each PixelBlink container is focused on blinking 1 pixel on the 8x8 display matrix on the SenseHat.

# Dependencies
**measureToDB** requires that the SensHat library, a mysql database and the mysql client library are installed on the raspberry pi. The table configuration file (database definition (DDL)) is SensHat.sql

**To Install mySQL**
```bash
    pi@raspberry:~ $ sudo apt-get install mysql-server
    pi@raspberry:~ $ sudo apt-get install libmysqlclient-dev
```

**Compilation** : measureDB application
```bash
    pi@raspberry:~/SensHat/Projects/MeasurementDatabase $ make
```

**To start the measureToDB simply type the following at the command line**
```bash
    pi@raspberry:~/SensHat/Projects/MeasurementDatabase $ ./measureDB
```


**To start the measureToDB application automatically from boot. copy measureDB application from the Project directory to /etc/init.d**
```bash
    pi@raspberry:~ $ sudo cp measureDB /etc/init.d
    pi@raspberry:~ $ sudo cd /etc/rc3.d
    pi@raspberry:~ $ sudo ln -s /etc/init.d/measureDB S99measureDB
    pi@raspberry:~ $ sudo cd /etc/rc5.d
    pi@raspberry:~ $ sudo ln -s /etc/init.d/measureDB S99measureDB
```
