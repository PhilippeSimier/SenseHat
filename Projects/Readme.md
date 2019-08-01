# pixelBlink

# Description
**pixelBlink** is an application that blinks 1 pixel of the SensHat's 64 pixels with a random color

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
