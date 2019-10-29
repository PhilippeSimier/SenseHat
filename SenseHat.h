/**
* @file SenseHat.h
* @date 4 mars 2018
* @version 1.2
* @authors Philippe SIMIER Philippe CRUCHET Christophe GRILLO
* @details Classe SenseHat : Gestion de la carte SenseHat
* @version 1.3
* @date 30 July 2019
* @authors Jon Dellaria bug fixes, method translation to English and Temperature Measurement adjustments required for the Raspberry for a true temperature reading.
*/



#ifndef SENSEHAT_H
#define SENSEHAT_H


#define DEV_FB "/dev"
#define FB_DEV_NAME "fb"
#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"

#define COULEUR uint16_t
#define COLOR_SENSEHAT uint16_t
#define PI 3.14159265

#define ROUGE   0xF800
#define RED 	  0xF800

#define BLEU    0x001F
#define BLUE    0x001F

#define VERT    0x07E0
#define GREEN   0x07E0

#define BLANC   0xFFFF
#define WHITE   0xFFFF

#define NOIR    0x0000
#define BLACK   0x0000

#define ORANGE  0xFC00
#define CYAN    0x87FF
#define MAGENTA 0xF81F

#define JAUNE   0xFFE0
#define YELLOW  0xFFE0


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <linux/input.h>
#include <sstream>
#include <math.h>
#include <RTIMULib.h>
#include <iostream>
#include <iomanip>

struct fb_t {
	uint16_t pixel[8][8];
};



class SenseHat
{
public:
    SenseHat();
    ~SenseHat();

    SenseHat& operator<<(SenseHat& (*)(SenseHat&));
    SenseHat& operator<<(const std::string&);
    SenseHat& operator<<(const int);
    SenseHat& operator<<(const double);
    SenseHat& operator<<(const char);
    SenseHat& operator<<(const char *);
    SenseHat& operator<<(const bool);


    void AfficherMessage(const std::string message, int vitesseDefilement = 100, uint16_t CouleurTexte = BLEU, uint16_t couleurFond = NOIR);
		void ViewMessage(const std::string message, int vitesseDefilement = 100, uint16_t CouleurTexte = BLEU, uint16_t couleurFond = NOIR);

    void AfficherLettre(char lettre, uint16_t couleurTexte = BLEU, uint16_t couleurFond = NOIR);
		void ViewLetter(char lettre, uint16_t couleurTexte = BLEU, uint16_t couleurFond = NOIR);

    void AllumerPixel(int ligne, int colonne, uint16_t couleur);
		void LightPixel(int ligne, int colonne, uint16_t couleur);

    uint16_t ObtenirPixel(int ligne, int colonne);
		uint16_t GetPixel(int ligne, int colonne);

    void AfficherMotif(uint16_t motif[][8]);
		void ViewPattern(uint16_t motif[][8]);

    void PivoterMotif(int rotation);
		void RotatePattern(int rotation);

    char ScannerJoystick();
    char ScanJoystick();

    COULEUR ConvertirRGB565(uint8_t rouge, uint8_t vert,uint8_t bleu);
    COLOR_SENSEHAT ConvertRGB565(uint8_t red, uint8_t green,uint8_t blue);

    COULEUR ConvertirRGB565(uint8_t couleur[]);
		COLOR_SENSEHAT ConvertRGB565(uint8_t color[]);

    COULEUR ConvertirRGB565(std::string couleur);
		COLOR_SENSEHAT ConvertRGB565(std::string color);

    void Effacer(uint16_t couleur=NOIR);
		void WipeScreen(uint16_t couleur=NOIR);

    float ObtenirTemperature();
		float GetTemperature();

		float correctTemperature(float senseHatTemp, float cpuTemp);
		float getRawTemperature();
		float getCpuTemperature();

    float ObtenirPression();
		float GetPressure();

    float ObtenirHumidite();
		float GetHumidity();

    void  ObtenirOrientation(float &pitch, float &roll, float & yaw);
		void  GetOrientation(float &pitch, float &roll, float & yaw);

    void  ObtenirAcceleration(float &x, float &y, float &z);
		void  GetAcceleration(float &x, float &y, float &z);

    void  ObtenirMagnetisme(float &x, float &y, float &z);
		void  GetMagnetism(float &x, float &y, float &z);

    void  ObtenirMagnetismeSpherique(float &ro, float &teta, float &delta);
		void  GetSphericalMagnetism(float &ro, float &teta, float &delta);

    void  Version();
    void  Flush();

    void  FixerCouleur(uint16_t);
		void  SetColor(uint16_t);

    void  FixerRotation(uint16_t);
		void  SetRotation(uint16_t);

private:
    void  InitialiserLeds();
		void  InitializeLeds();

    void  InitialiserJoystik();
		void  InitializeJoystick();

    void  InitialiserPression();
		void  InitializePressure();

    void  InitialiserHumidite();
		void  InitializeHumidity();

    void  InitialiserOrientation();
		void  InitializeOrientation();

    void  InitialiserAcceleration();
		void  InitializeAcceleration();

    void ConvertirCaractereEnMotif(char c, uint16_t image[8][8], uint16_t couleurTexte, uint16_t couleurFond);
		void ConvertCharacterToPattern(char c, uint16_t image[8][8], uint16_t couleurTexte, uint16_t couleurFond);

    bool ColonneVide(int numColonne, uint16_t image[8][8], uint16_t couleurFond);
		bool EmptyColumn(int numColonne, uint16_t image[8][8], uint16_t couleurFond);

    void TassementDeLimage(int numColonne, uint16_t image[][8][8], int taille);
		void ImageContainment(int numColonne, uint16_t image[][8][8], int taille);

    struct fb_t *fb;
    int joystick;
    RTIMUSettings *settings;
    RTIMU *imu;
    RTPressure *pressure;
    RTHumidity *humidite;
    std::string buffer;
    uint16_t couleur;
    int rotation;
};

// surcharge des manipulators
SenseHat& endl(SenseHat& os);
SenseHat& flush(SenseHat& os);

// surcharge du manipulateur setcouleur
struct _SetCouleur { int val; };
_SetCouleur setcouleur(int n);
SenseHat&  operator<<(SenseHat& os, _SetCouleur couleur);

// surcharge du manipulateur setrotation
struct _SetRotation { int val; };
_SetRotation setrotation(int n);
SenseHat&  operator<<(SenseHat& os, _SetRotation rotation);
#endif // SENSEHAT_H
