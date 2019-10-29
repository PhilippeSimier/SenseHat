# SparkelContainer

# Description
**SparkelContainer** is an application that runs 64 containers of the pixelBlink application. Each pixelBlink container is focused on blinking 1 pixel on the 8x8 display matrix on the SenseHat.

# Dependencies
**SparkelContainer** requires that the pixelBlink application is in the location defined by the CHILDCONTAINERNAME #define statement in SparkelContainer.cpp

**Compilation** : SparkelContainer application
```bash
    pi@raspberry:~/SensHat/Projects/SparkelContainer $ make
```

**To start the SparkelContainer simply type the following at the command line**
```bash
    pi@raspberry:~/SensHat/Projects/SparkelContainer $ ./SparkelContainer
```

Once the application has started, you will see all the pixels on the 8x8 display matrix begin to blink for approximately 1 minute.
