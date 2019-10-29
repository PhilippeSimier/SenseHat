# Project Folder

**pixelBlink** is an application that blinks 1 pixel of the SenseHat's 64 pixels with a random color
**digitalClock** is an application that displays the complete time in hour and minute format on the SenseHat display matrix

The **MeasurementDatabase** folder contains the **measureToDB** application which takes all the measurements of the SenseHat and records them in a mySQL database table.
The **SparkelContainer** folder contains the **SparkelContainer** application which runs 64 individual executable containers of the **pixelBlink** application located in this folder.

**Compilation** : pixelBlink and digitalClock applications
```bash
    pi@raspberry:~/SensHat/Project $ make
```

**To start the pixelBlink simply type the following at the command line**
```bash
    pi@raspberry:~/SensHat/Project $ ./pixelBlink 1 1
```
Once the application has started, you will see the pixel (defined by ./pixelBlink **row(0-7)** **column(0-7)** command line options) on the 8x8 display matrix begin to blink for approximately 1 minute with random colors.

**To start the digitalClock simply type the following at the command line**
```bash
    pi@raspberry:~/SensHat/Project $ ./digitalClock
```
Once the application has started, you will see the digital clock on the 8x8 display matrix.
