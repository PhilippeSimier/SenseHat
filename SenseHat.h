/**
 * @file SenseHat.h
 * @date 4 mars 2018
 * @version 1.2
 * @authors Philippe SIMIER Philippe CRUCHET Christophe GRILLO
 * @details Classe SenseHat : Gestion de la carte SenseHat
 */
#ifndef SENSEHAT_H
#define SENSEHAT_H


#define DEV_FB "/dev"
#define FB_DEV_NAME "fb"
#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"

#define COULEUR uint16_t
#define PI 3.14159265

#define ROUGE   0xF800
#define BLEU    0x001F
#define VERT    0x07E0
#define BLANC   0xFFFF
#define NOIR    0x0000
#define ORANGE  0xFC00
#define CYAN    0x87FF
#define MAGENTA 0xF81F
#define JAUNE   0xFFE0


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
    SenseHat& operator<<(char);
    SenseHat& operator<<(bool);


    void AfficherMessage(const std::string message, int vitesseDefilement = 100, uint16_t CouleurTexte = BLEU, uint16_t couleurFond = NOIR);
    void AfficherLettre(char lettre, uint16_t couleurTexte = BLEU, uint16_t couleurFond = NOIR);

    void AllumerPixel(int ligne, int colonne, uint16_t couleur);
    uint16_t ObtenirPixel(int ligne, int colonne);
    void AfficherMotif(uint16_t motif[][8]);
    void PivoterMotif(int rotation);
    char ScannerJoystick();

    COULEUR ConvertirRGB565(uint8_t rouge, uint8_t vert,uint8_t bleu);
    COULEUR ConvertirRGB565(uint8_t couleur[]);
    COULEUR ConvertirRGB565(std::string couleur);

    void Effacer(uint16_t couleur=NOIR);

    float ObtenirTemperature();
    float ObtenirPression();
    float ObtenirHumidite();
    void  ObtenirOrientation(float &pitch, float &roll, float & yaw);

    void  ObtenirAcceleration(float &x, float &y, float &z);
    void  ObtenirMagnetisme(float &x, float &y, float &z);
    void  ObtenirMagnetismeSpherique(float &ro, float &teta, float &delta);
    void  Version();
    void  Flush();
    void  FixerCouleur(uint16_t);
    void  FixerRotation(uint16_t);

private:
    void  InitialiserLeds();
    void  InitialiserJoystik();
    void  InitialiserPression();
    void  InitialiserHumidite();
    void  InitialiserOrientation();
    void  InitialiserAcceleration();


    void ConvertirCaractereEnMotif(char c, uint16_t image[8][8], uint16_t couleurTexte, uint16_t couleurFond);
    bool ColonneVide(int numColonne, uint16_t image[8][8], uint16_t couleurFond);
    void TassementDeLimage(int numColonne, uint16_t image[][8][8], int taille);

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
