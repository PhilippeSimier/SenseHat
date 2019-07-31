# SparkelContainer

# Description
**SparkelContainer** is an application that runs 64 containers of the PixelBlink application. Each PixelBlink container is focused on blinking 1 pixel on the 8x8 display matrix on the SenseHat.

# Dependanceies
**SparkelContainer** requires that the pixelBlink application is in the location defined by the CHILDCONTAINERNAME #defined statement in SparkelContainer.cpp

**Compilation** : SparkelContainer application
```bash
    pi@raspberry:~ $ make
```

**To start the SparkelContainer simply type the following at the command line**
```bash
    pi@raspberry:~ $ ./SparkelContainer
```

Once the application has started, you will see pixels on the 8x8 display matrix begin to blink for approximately 1 minute.
