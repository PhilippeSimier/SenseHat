# pixelBlink

# Description
**pixelBlink** is an application that runs 64 containers of the PixelBlink application. Each PixelBlink container is focused on blinking 1 pixel on the 8x8 display matrix on the SenseHat.

# Dependanceies
**pixelBlink** requires that the pixelBlink application is in the location defined by the CHILDCONTAINERNAME #defined statement in SparkelContainer.cpp

**Compilation** : pixelBlink application
```bash
    pi@raspberry:~ /Project $ make
```

**To start the SparkelContainer simply type the following at the command line**
```bash
    pi@raspberry:~  /Project $ ./pixelBlink 1 1
```

Once the application has started, you will see the pixel (defined by ./pixelBlink <row> <column> command line options) on the 8x8 display matrix begin to blink for approximately 1 minute with random colors.
