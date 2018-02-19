#ifndef SENSEHAT_H
#define SENSEHAT_H


#define DEV_FB "/dev"
#define FB_DEV_NAME "fb"
#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"

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
#include <string.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <linux/input.h>

#include "RTIMULib.h"

struct fb_t {
	uint16_t pixel[8][8];
};


class SenseHat
{
public:
    SenseHat();
    ~SenseHat();

    void AfficherMessage(std::string message, int vitesseDefilement = 100, uint16_t CouleurTexte = BLEU, uint16_t couleurFond = NOIR);

    void AfficherLettre(char lettre, uint16_t couleurTexte = BLEU, uint16_t couleurFond = NOIR);

    void AllumerPixel(int ligne, int colonne, uint16_t couleur);
    uint16_t ObtenirPixel(int ligne, int colonne);
    void AfficherMotif(uint16_t motif[][8]);
    void PivoterMotif(int rotation);
    char ScannerJoystick();
    uint16_t ConvertirRGB565(uint8_t rouge, uint8_t vert,uint8_t bleu);
    uint16_t ConvertirRGB565(uint8_t couleur[]);
    void Effacer(uint16_t couleur=NOIR);

    float ObtenirTemperature();
    float ObtenirPression();
    float ObtenirHumidite();
    void  ObtenirOrientation(float &pitch, float &roll, float & yaw);
    void  InitialiserAcceleration();
    void  ObtenirAcceleration(float &pitch, float &roll, float & yaw);
    void  ObtenirMagnetisme(float &x, float &y, float &z);
    void  Version();

private:
    void InitialiserLeds();
    void InitialiserJoystik();
    void InitialiserPression();
    void InitialiserHumidite();
    void InitialiserOrientation();


    // Converti un caractère en Motif affichable sur la matrice de leds - Fait par Grilo Christophe
    void ConvertirCaractereEnMotif(char c, uint16_t image[8][8], uint16_t couleurTexte, uint16_t couleurFond);
    // Permet de savoir si une colonne de leds est vide - Fait par Grilo Christophe
    bool ColonneVide(int numColonne, uint16_t image[8][8], uint16_t couleurFond);
    // Permet de supprimer une colonne sur un ensemble de motif (image) - Fait par Grilo Christophe
    void TassementDeLimage(int numColonne, uint16_t image[][8][8], int taille);

    struct fb_t *fb;
    int joystick;
    RTIMUSettings *settings;
    RTIMU *imu;
    RTPressure *pressure;
    RTHumidity *humidite;
};

#endif // SENSEHAT_H
