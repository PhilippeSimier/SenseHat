/**
 * @file SenseHat.cpp
 * @date 4 mars 2018
 * @version 1.2
 * @authors Philippe SIMIER Philippe CRUCHET Christophe GRILLO
 * @details Classe SenseHat : Gestion de la carte SenseHat
 * @version 1.3
 * @date 30 July 2019
 * @authors Jon Dellaria bug fixes, method translation to English and Temperature Measurement adjustments required for the Raspberry for a true temperature reading.
 */


#include "SenseHat.h"
#include "font.h"
#include <iostream>
#include <stdio.h>
#include <fcntl.h>

#define NUMBER_OF_TRIES_BEFORE_FAILURE 1000

static int is_framebuffer_device(const struct dirent *dir)
{
    return strncmp(FB_DEV_NAME, dir->d_name,
                   strlen(FB_DEV_NAME)-1) == 0;
}


static int open_fbdev(const char *dev_name)
{
    struct dirent **namelist;
    int i, ndev;
    int fd = -1;
    struct fb_fix_screeninfo fix_info;

    ndev = scandir(DEV_FB, &namelist, is_framebuffer_device, versionsort);
    if (ndev <= 0)
        return ndev;

    for (i = 0; i < ndev; i++)
    {
        char fname[64];

        snprintf(fname, sizeof(fname),
                 "%s/%s", DEV_FB, namelist[i]->d_name);
        fd = open(fname, O_RDWR);
        if (fd < 0)
            continue;
        ioctl(fd, FBIOGET_FSCREENINFO, &fix_info);
        if (strcmp(dev_name, fix_info.id) == 0)
            break;
        close(fd);
        fd = -1;
    }
    for (i = 0; i < ndev; i++)
        free(namelist[i]);

    return fd;
}


static int is_event_device(const struct dirent *dir)
{
    return strncmp(EVENT_DEV_NAME, dir->d_name,
                   strlen(EVENT_DEV_NAME)-1) == 0;
}


static int open_evdev(const char *dev_name)
{
  int tries;
    struct dirent **namelist;
    int i, ndev;
    int fd = -1;
    int sortie = false;

    tries = 0;
    while(true)
    {
      ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, versionsort);
      if (ndev <= 0)
      {
        tries++;
        usleep(100);
      }
      else
      {
        break;
      }
      if (tries > NUMBER_OF_TRIES_BEFORE_FAILURE)
      {
        exit (EXIT_FAILURE);
      }
    }

    i=0;
    do
    {
        char fname[64];
        char name[256];

        snprintf(fname, sizeof(fname),
                 "%s/%s", DEV_INPUT_EVENT, namelist[i++]->d_name);

        tries = 0;
        while(true)
        {
          fd = open(fname, O_RDONLY );
          if (fd < 0)
          {
            tries++;
            usleep(100);
          }
          else
          {
            break;
          }
          if (tries > NUMBER_OF_TRIES_BEFORE_FAILURE)
          {
            exit (EXIT_FAILURE);
          }
        }

        ioctl(fd, EVIOCGNAME(sizeof(name)), name);

        if (strcmp(dev_name, name) != 0)
            close(fd);
        else sortie = true;
    }
    while( i<ndev && sortie != true);

    for (i = 0; i < ndev; i++)
        free(namelist[i]);
    return fd;
}


uint16_t handle_events(int evfd)
{
    struct input_event ev;
    int rd;
    uint16_t retour = 0 ;
    int flag;

    flag = fcntl(evfd,F_GETFL,0);
    fcntl(evfd,F_SETFL,flag | O_NONBLOCK | O_NOCTTY);

    rd = read(evfd, &ev, sizeof(struct input_event));
    if(rd > 0)
    {
        if(ev.type == EV_KEY && ev.value == 1)
            retour = ev.code;

    }
    return retour;
}

/**
 * @brief SenseHat::SenseHat
 * @details Constructeur de la classe, initialise les attributs
 *          par défaut imu, leds, Joystick, buffer.
 */

SenseHat::SenseHat()
{
  int tries;
  settings = new RTIMUSettings("RTIMULib");

  tries = 0;
  while(true)
  {
    imu = RTIMU::createIMU(settings);
    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL))
    {
      tries++;
      usleep(100);
    }
    else
    {
      break;
    }
    if (tries > NUMBER_OF_TRIES_BEFORE_FAILURE)
    {
      exit (EXIT_FAILURE);
    }
  }

  imu->IMUInit();
  imu->setSlerpPower(0.02);
  imu->setGyroEnable(true);
  imu->setAccelEnable(true);
  imu->setCompassEnable(true);
  InitializeLeds();
  InitializeJoystick();
  InitializeHumidity();
  InitializePressure();
  buffer=" ";
  couleur=BLEU;
  rotation = 0;
}

/**
 * @brief SenseHat::~SenseHat
 * @details Destructeur de la classe
 */

SenseHat::~SenseHat()
{
    delete settings;
}

/**
 * @brief SenseHat::operator<<
 * @details surcharge de l'opérateur << pour les modificateurs endl et flush
 */

SenseHat& SenseHat::operator<<(SenseHat& (*fp)(SenseHat&))
{
      return (*fp)(*this);
}


/**
 * @brief SenseHat::Version
 * @details affiche la version de la classe
 */

void SenseHat::Version()
{
    std::cout << "SenseHat PCT,PSR,CGO Version 1.2.0" << std::endl;
}

void SenseHat::FixerCouleur(uint16_t _couleur)
{
  couleur = _couleur;
}

void SenseHat::SetColor(uint16_t _couleur)
{
   couleur = _couleur;
}

void SenseHat::FixerRotation(uint16_t _rotation)
{
   rotation = _rotation;
}

void SenseHat::SetRotation(uint16_t _rotation)
{
   rotation = _rotation;
}


/**
 * @brief SenseHat::AfficherLettre
 * @param lettre char
 * @param couleurTexte  uint16_t une couleur au format 565
 * @param couleurFond   uint16_t une couleur au format 565
 * @details affiche un caractère (lettre ponctuation signe) sur l'afficheur
 */

void SenseHat::AfficherLettre(char lettre, uint16_t couleurTexte, uint16_t couleurFond)
{
  ViewLetter(lettre, couleurTexte,  couleurFond);
}

void SenseHat::ViewLetter(char lettre, uint16_t couleurTexte, uint16_t couleurFond)
{
	uint16_t chr[8][8];
//	ConvertirCaractereEnMotif(lettre,chr,couleurTexte,couleurFond);
  ConvertCharacterToPattern(lettre,chr,couleurTexte,couleurFond);
//	AfficherMotif(chr);
  ViewPattern(chr);
}

/**
 * @brief SenseHat::AllumerPixel
 * @param int indice de la ligne
 * @param int indice de la colonne
 * @param couleur   uint16_t une couleur au format 565
 * @details fixe la couleur d'un pixel désigné par ses coordonnées
 */
void SenseHat::AllumerPixel(int ligne, int colonne, uint16_t couleur)
{
  LightPixel(ligne, colonne, couleur);
}

void SenseHat::LightPixel(int row, int column, uint16_t color)
{
    if(row < 0)
	row = 0;
    if(column < 0)
	column = 0;

    fb->pixel[row%8][column%8] = color;
}


/**
 * @brief SenseHat::ObtenirPixel
 * @param int indice de la ligne
 * @param int indice de la colonne
 * @return uint16_t une couleur au format 565
 * @details  retourne, sous la forme d'un entier sur 16 bits non signé, la couleur du pixel
 *           dont les coordonnées sont passées à la fonction.
 */
uint16_t SenseHat::ObtenirPixel(int ligne, int colonne)
{
  return (GetPixel(ligne, colonne));
}

uint16_t SenseHat::GetPixel(int row, int column)
{
    if(row < 0)
        row = 0;
    if(column < 0)
        column = 0;

    return fb->pixel[row%8][column%8] ;
}

/**
 * @brief SenseHat::AfficherMotif
 * @param tableau 8*8 de uint16_t
 * @details Affiche le tableau de pixel sur l'afficheur
 *          en tenant compte de l'angle de rotation
 */
void SenseHat::AfficherMotif(uint16_t motif[][8])
{

  ViewPattern(motif);
}

void SenseHat::ViewPattern(uint16_t motif[][8])
{
    for(int ligne=0; ligne <8 ; ligne++)
    {
        for(int colonne=0 ; colonne <8 ; colonne++)
        {
            switch(this->rotation)
            {
            case   90:
	    case -270:
                fb->pixel[7 - colonne][ligne] = motif[ligne][colonne];
                break;
            case  180:
	    case -180:
                fb->pixel[7 - ligne][7 - colonne] = motif[ligne][colonne];
                break;
            case  270:
	    case  -90:
                fb->pixel[colonne][7 - ligne] = motif[ligne][colonne];
                break;
	    default:
		fb->pixel[ligne][colonne] = motif[ligne][colonne];
            }
        }
    }
}


/**
 * @brief SenseHat::PivoterMotif
 * @param int angle de rotation 90, 180, 270, -90, -180, -270
 * @details Fait pivoter le motif afficher autour du milieu d'un angle de
 *          rotation.
 */

void SenseHat::PivoterMotif(int angle)
{
  RotatePattern(angle);
}

void SenseHat::RotatePattern(int angle)
{

    uint16_t tabAux[8][8];

    for(int ligne=0; ligne <8 ; ligne++)
    {
        for(int colonne=0 ; colonne <8 ; colonne++)
        {
            switch(angle)
            {
            case   90:
	    case -270:
                tabAux[7 - colonne][ligne] = fb->pixel[ligne][colonne];
                break;
            case  180:
	    case -180:
                tabAux[7 - ligne][7 - colonne] = fb->pixel[ligne][colonne];
                break;
            case  270:
	    case  -90:
                tabAux[colonne][7 - ligne] = fb->pixel[ligne][colonne];
                break;
	    default:
		tabAux[ligne][colonne] = fb->pixel[ligne][colonne];
            }
        }
    }

//    AfficherMotif(tabAux);
    ViewPattern(tabAux);
}

/**
 * @brief SenseHat::Effacer
 * @param uint16_t une couleur au format 565
 * @details Affiche la couleur sur l'ensemble de l'afficheur à leds
 *          une couleur Noir éteind l'écran
 */
void SenseHat::Effacer(uint16_t couleur)
{
    memset(fb, couleur, 128);
}

void SenseHat::WipeScreen(uint16_t couleur)
{
    memset(fb, couleur, 128);
}

/**
 * @brief SenseHat::ScannerJoystick
 * @return le code équivalent aux touches du clavier enter,
 * fleche droite, gauche, haut et bas.
 */
char SenseHat::ScannerJoystick()
{
    return handle_events(joystick);
}

char SenseHat::ScanJoystick()
{
    return handle_events(joystick);
}

/**
 * @brief SenseHat::ConvertirRGB565
 * @param uint8_t composante rouge
 * @param uint8_t composante verte
 * @param uint8_t composante bleu
 * @return uint16_t une couleur codée en RGB565
 * @details permet de convertir une couleur exprimer sous la forme
 *          de trois entiers non signés sur 8 bits en un entier
 *          représentant la couleur codée en RGB565
 */

uint16_t SenseHat::ConvertirRGB565(uint8_t rouge, uint8_t vert, uint8_t bleu)
{

return (ConvertRGB565(rouge, vert, bleu));
}


uint16_t SenseHat::ConvertRGB565(uint8_t red, uint8_t green, uint8_t blue)
{

   blue &= 0xF8;
   green &= 0xFC;
   red &= 0xF8;

   return ((red<<8) + (green<<3) + (blue>>3));
}

/**
 * @brief surcharge de SenseHat::ConvertirRGB565
 * @param un tableau de trois uint8_t
 * @return uint16_t une couleur codée en RGB565
 * @details permet de convertir une couleur exprimer sous la forme
 *          d'un tableaude  trois entiers non signés sur 8 bits en un entier
 *          représentant la couleur codée en RGB565
 */

uint16_t SenseHat::ConvertirRGB565(uint8_t couleur[])
{
    return ConvertirRGB565(couleur[1],couleur[2],couleur[3]);
}

uint16_t SenseHat::ConvertRGB565(uint8_t color[])
{
    return ConvertRGB565(color[1],color[2],color[3]);
}

/**
 * @brief surcharge de SenseHat::ConvertirRGB565
 * @param string chaine de caratère représentant une couleur au format hexa
 * @return uint16_t une couleur codée en RGB565
 * @details permet de convertir une couleur exprimer sous la forme
 *          d'une chaîne de caractère au format hexa en un entier
 *          représentant la couleur codée en RGB565
 */

COULEUR SenseHat::ConvertirRGB565(std::string hexCode)
{
  return ConvertRGB565(hexCode);
}

COLOR_SENSEHAT SenseHat::ConvertRGB565(std::string hexCode)
{
   int r, g, b;

   // Retire le hashtag ...
   if(hexCode.at(0) == '#') {
      hexCode = hexCode.erase(0, 1);
   }
   // puis extraction des valeurs r g b.
   std::istringstream(hexCode.substr(0,2)) >> std::hex >> r;
   std::istringstream(hexCode.substr(2,2)) >> std::hex >> g;
   std::istringstream(hexCode.substr(4,2)) >> std::hex >> b;

   return ConvertRGB565(r,g,b);
}

/**
 * @brief SenseHat::ObtenirTemperature
 * @return float la valeur de la température exprimée en °C,
 */


float SenseHat::ObtenirTemperature()
{
  return (GetTemperature());
}

float SenseHat::GetTemperature()
{
    float cpuTemp;
    float correctedTemp;
    float senseHatTemp;

    senseHatTemp = getRawTemperature();
    cpuTemp = getCpuTemperature();

    //temp_calibrated = temp - ((cpu_temp - temp)/FACTOR)
    correctedTemp = correctTemperature(senseHatTemp, cpuTemp);
    return (correctedTemp);
}



float SenseHat::correctTemperature(float senseHatTemp, float cpuTemp)
{
    float correctedTemp;
    float TEMPERATUREFACTOR = 1.2;

    //temp_calibrated = temp - ((cpu_temp - temp)/FACTOR)
    correctedTemp = senseHatTemp - ((cpuTemp - senseHatTemp)/TEMPERATUREFACTOR);
    return (correctedTemp);
}



/**
 * @brief SenseHat::getRawTemperature
 * @return float la valeur de la température exprimée en °C,
 */


float SenseHat::getRawTemperature()
{
    RTIMU_DATA data;
    float senseHatTemp;

    pressure->pressureRead(data);
    senseHatTemp = data.temperature;
    return (senseHatTemp);
}


/**
 * @brief SenseHat::getCoreTemperature
 * @return float la valeur de la température exprimée en °C,
 */


float SenseHat::getCpuTemperature()
{
    FILE *temperatureFile;
    double T;
    temperatureFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
    if (temperatureFile == NULL)
    {
//      printf ("Error getting Core Temperature!")  ; //print some message
      return (0);
    }
    fscanf (temperatureFile, "%lf", &T);
    T /= 1000;
//    printf ("The temperature is %6.3f C.\n", T);
    fclose (temperatureFile);
    return(T);
}


/**
 * @brief SenseHat::ObtenirPression
 * @return float la valeur de la Pression exprimée en hPa,
 * @detail la valeur correspond à la valeur brute mesurée à l'altitude du capteur
 *         elle doit donc être convertie pour être ramenée au niveau de la mer
 */
float SenseHat::ObtenirPression()
{
  return(GetPressure());
}

float SenseHat::GetPressure()
{
    RTIMU_DATA data;
    float pression = nan("");  // initialise la valeur à Not-A-Number
    if (pressure->pressureRead(data)){
    	if (data.pressureValid){
    	    pression = data.pressure;
        }
    }
    return pression;
}

/**
 * @brief SenseHat::ObtenirHumidite
 * @return float la valeur de l'humidité relative exprimée en %,
 */
float SenseHat::ObtenirHumidite()
{
  return(GetHumidity());
}

float SenseHat::GetHumidity()
{
    RTIMU_DATA data;
    float humidi = nan("");  // initialise la valeur à Not-A-Number
    if (humidite->humidityRead(data)){
	if (data.humidityValid){
	    humidi = data.humidity;
	}
    }
    return humidi;
}

/**
 * @brief SenseHat::ObtenirOrientation
 * @return float la valeur de l'accélération angulaire suivant pitch roll et yaw
 * @detail la valeur est exprimée en rd/s
 *
 */
void SenseHat::ObtenirOrientation(float &pitch, float &roll, float &yaw)
{
  GetOrientation(pitch, roll, yaw);
}
void SenseHat::GetOrientation(float &pitch, float &roll, float &yaw)

{
    while (imu->IMURead()){
	RTIMU_DATA imuData = imu->getIMUData();
	pitch = imuData.gyro.x();
	roll  = imuData.gyro.y();
	yaw   = imuData.gyro.z();
    }
}

/**
 * @brief SenseHat::ObtenirAcceleration
 * @return float la valeur de l'accélération linéaire suivant X,Y,Z
 * @detail la valeur est exprimée en m/s*s
 *
 */
void SenseHat::ObtenirAcceleration(float &x, float &y, float &z)
{
  GetAcceleration(x, y, z);
}
void SenseHat::GetAcceleration(float &x, float &y, float &z)
{
    while (imu->IMURead()){
	RTIMU_DATA imuData = imu->getIMUData();
	x = imuData.accel.x();
	y = imuData.accel.y();
	z = imuData.accel.z();
    }
}

/**
 * @brief SenseHat::ObtenirMagnétisme
 * @return float la valeur du champ magnétique terrestre suivant X,Y,Z
 * @detail la valeur est exprimée en micro Tesla
 *
 */

void SenseHat::ObtenirMagnetisme(float &x, float &y, float &z)
{
  GetMagnetism(x, y, z);
}

void SenseHat::GetMagnetism(float &x, float &y, float &z)
{
    while (imu->IMURead()){
        RTIMU_DATA imuData = imu->getIMUData();
        x = imuData.compass.x();
        y = imuData.compass.y();
        z = imuData.compass.z();
    }
}

/**
 * @brief SenseHat::ObtenirMagnetismeSpherique
 * @return la valeur du vecteur champ magnétique en coordonnées sphérique
 * @detail teta l'angle mesuré depuis l'axe des X
 *      si teta = 0 alors l'axe X est dirigé vers le nord magnétique
 *	si teta = 180 ou -180 alors l'axe des X est dirigé vers le sud
 */
void SenseHat::ObtenirMagnetismeSpherique(float &ro, float &teta, float &delta)
{
  GetSphericalMagnetism(ro, teta, delta);
}

void SenseHat::GetSphericalMagnetism(float &ro, float &teta, float &delta)

{
    float x,y,z;

 //   ObtenirMagnetisme(x,y,z);
    GetMagnetism(x,y,z);
    teta = atan2 (y,x) * 180/PI;
    ro   = sqrt(x*x + y*y + z*z);
    delta =  atan2 (z,sqrt(x*x + y*y)) * 180/PI;
}

/**
 * @brief  SenseHat::InitialiserLeds
 * @detail initialise de framebuffer
 */
void SenseHat::InitialiserLeds()
{
  InitializeLeds();
}

void SenseHat::InitializeLeds()
{
    int fbfd ;
    int tries;
    int tries2;

    tries = 0;
    tries2 = 0;
    while(true)
    {
      fbfd = open_fbdev("RPi-Sense FB");
      if (fbfd > 0)
      {
          while (true)
          {
            fb = (struct fb_t*)mmap(0, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
            if (!fb)
            {
                tries2++;
                usleep(100);
                if (tries2 > NUMBER_OF_TRIES_BEFORE_FAILURE)
                {
                  printf("Failed to mmap.\n");
                  exit(EXIT_FAILURE);
                }
            }
            else
            {

              memset(fb, 0, 128);
              return;
            }

          }
      }
      else
      {
        close(fbfd);
        tries++;
        usleep(100);
        if (tries > NUMBER_OF_TRIES_BEFORE_FAILURE)
        {
          printf("Error: cannot open framebuffer device.\n");
          exit (EXIT_FAILURE);
        }
      }

    }

}

/**
 * @brief  SenseHat::InitialiserJoystik
 * @detail initialise le Joystick
 */
void SenseHat::InitialiserJoystik()
{
  InitializeJoystick();
}

void SenseHat::InitializeJoystick()
{
    joystick = open_evdev("Raspberry Pi Sense HAT Joystick");
}

/**
 * @brief  SenseHat::InitialiserPression
 * @detail initialise le capteur de pression
 */

void SenseHat::InitialiserPression()
{
  InitializePressure();
}

void SenseHat::InitializePressure()
{
  int tries;

  tries = 0;
  while(true)
  {
    pressure = RTPressure::createPressure(settings);
    if (pressure == NULL)
    {
      tries++;
      usleep(100);
    }
    else
    {
      break;
    }
    if (tries > NUMBER_OF_TRIES_BEFORE_FAILURE)
    {
      printf("Pas de mesure de pression/température \n");
      exit (EXIT_FAILURE);
    }
  }
  pressure->pressureInit();
}

/**
 * @brief  SenseHat::InitialiserHumidite
 * @detail initialise le capteur d'humidité
 */
void SenseHat::InitialiserHumidite()
{
  InitializeHumidity();
}

void SenseHat::InitializeHumidity()
{
  int tries;

    tries = 0;
    while(true)
    {
      humidite = RTHumidity::createHumidity(settings);
      if(humidite == NULL)
      {
        tries++;
        usleep(100);
      }
      else
      {
        break;
      }
      if (tries > NUMBER_OF_TRIES_BEFORE_FAILURE)
      {
        printf("Pas de mesure de pression/température \n");
        exit (EXIT_FAILURE);
      }
    }
    humidite->humidityInit();
}

void SenseHat::InitialiserOrientation()
{
  InitializeOrientation();
}

void SenseHat::InitializeOrientation()
{

}

void SenseHat::InitialiserAcceleration()
{
  imu->setAccelEnable(true);
}

void SenseHat::InitializeAcceleration()
{
    imu->setAccelEnable(true);
}


/**
 * @brief  SenseHat::ConvertirCaractereEnMotif
 * @detail Converti un caractère en Motif affichable sur la matrice de leds
 * - Fait par Grilo Christophe
 */
void SenseHat::ConvertirCaractereEnMotif(char c,uint16_t image[8][8],uint16_t couleurTexte, uint16_t couleurFond)
{
  ConvertCharacterToPattern(c, image, couleurTexte, couleurFond);
}

void SenseHat::ConvertCharacterToPattern(char c, uint16_t image[8][8], uint16_t couleurTexte, uint16_t couleurFond)
{
    int i=0;
    int j,k;
    int tailleTableDeConvertion=sizeof(font)/sizeof(Tfont);

    // Recherche si le caractere existe dans la table de convertion (cf font.h)
    while(c!=font[i].caractere && i < tailleTableDeConvertion )
	i++;

    // Si le caractere est dans la table on le converti
    if(i < tailleTableDeConvertion)
    {
	for (j=0;j<8;j++)
	{
            for(k=0;k<8;k++)
	    {
		if(font[i].motifbinaire[j][k])
		    image[j][k]=couleurTexte;
		else
		    image[j][k]=couleurFond;
	    }
	}
    }
    else // caractère inexistant on le remplace par un glyphe inconnu
//	ConvertirCaractereEnMotif(255,image,couleurTexte,couleurFond);
    ConvertCharacterToPattern(255,image,couleurTexte,couleurFond);
}


bool SenseHat::ColonneVide(int numColonne,uint16_t image[8][8],uint16_t couleurFond)
{
  return (EmptyColumn(numColonne,image,couleurFond));
}
bool SenseHat::EmptyColumn(int numColonne,uint16_t image[8][8],uint16_t couleurFond)
{
    int i=0;
    for(i=0;i<8;i++)
	if(image[i][numColonne]!=couleurFond)
 	   return false;
    return true;
}


void SenseHat::TassementDeLimage(int numColonne,uint16_t image[][8][8], int taille)
{
  ImageContainment(numColonne, image, taille);
}

void SenseHat::ImageContainment(int numColonne,uint16_t image[][8][8], int taille)
{
    int i=0,j=0,k=0,l=0,isuivant,ksuivant;
    int nombredecolonnes=taille*8; //8 colonnes par motif

    for(l=numColonne;l<nombredecolonnes-1;l++)
    {
	i=l/8;
	k=l%8;
	isuivant=(l+1)/8;
	ksuivant=(l+1)%8;
	for(j=0;j<8;j++)
		image[i][j][k]=image[isuivant][j][ksuivant];
    }
}

void SenseHat::AfficherMessage(const std::string message, int vitesseDefilement, uint16_t couleurTexte, uint16_t couleurFond)
{
  ViewMessage( message, vitesseDefilement,  couleurTexte,  couleurFond);
}

void SenseHat::ViewMessage(const std::string message, int vitesseDefilement, uint16_t couleurTexte, uint16_t couleurFond)
{
    int taille=message.length();
    uint16_t chaine[taille][8][8]; /* Le tableau de motif (image/caractère) à afficher */
    int i=0,j=0,k=0,l=0,nombreDeColonneVide=0;
    int isuivant=0,ksuivant=0,nombreDeColonnes=0;

    /* Convertion de tout le message en tableau de motifs
     * format caractère : 1 colonne vide + 5 colonnes réellement utilisées
     * + 2 colonnes vides */
    for( i=0,j=0; i<taille; i++,j++)
    {

	if(message[i]==195)  // les lettres accentuées sont codées sur deux octets  (195 167 pour ç)
	{
	    i++;
	    k++;
	}
  	// ligne suivante à décommenter pour obtenir le code des caractères UTF8
	// std::cout << "code : " << (int)message[i] << std::endl;
//	ConvertirCaractereEnMotif(message[i],chaine[j],couleurTexte,couleurFond);
	ConvertCharacterToPattern(message[i],chaine[j],couleurTexte,couleurFond);
    }
	taille = taille - k;
	nombreDeColonnes=(taille)*8-2;
	k = 0;
	// Parcours de toutes les colonnes de tous les motifs qui compose
	// la chaine de caractères à afficher pour supprimer les colonnes vides sauf celle
	// qui sépare les motifs (caractères). + gestion du caractère espace.
	for( l=0;l<nombreDeColonnes;l++)
	{
		i=l/8;
		k=l%8;

//		if(ColonneVide(k,chaine[i],couleurFond)) // Une colonne Vide avant chaque caractère à ne pas supprimer
		if(EmptyColumn(k,chaine[i],couleurFond)) // Une colonne Vide avant chaque caractère à ne pas supprimer

		{
			isuivant=(++l)/8;
			ksuivant=(l)%8;
			nombreDeColonneVide=1;

			// compter les colonnes vide après la première afin de les supprimer
			// si plus de 4 c'est le caractère espace que l'on doit garder
//			while(ColonneVide(ksuivant,chaine[isuivant],couleurFond) && nombreDeColonneVide++ < 6)
			while(EmptyColumn(ksuivant,chaine[isuivant],couleurFond) && nombreDeColonneVide++ < 6)
			{
//				TassementDeLimage(l,chaine,taille);
				ImageContainment(l,chaine,taille);
				nombreDeColonnes--;
			}

		}
	}

	// Parcours de toutes les colonnes de tous les motifs qui composent
	// la chaine de caractères à afficher (sans les colonnes vides superflues).
	for( l=0; l<nombreDeColonnes; l++)
	{
		// Decalage des colonnes vers la gauche sur l'image Numero 0 (celle qu'on affiche sur la matrice de LED
		for( i=0;i<taille;i++)
		{
			// Cas Normal, les colonnes sont sur le même motif
			for(j=0;j<8;j++)
			{
				for(k=0;k<7;k++)
					chaine[i][j][k]=chaine[i][j][k+1];

			}
			// Cas où l'on doit changer de motif
			for(j=0;j<8;j++)
				chaine[i][j][7]=chaine[i+1][j][0];
		}
		usleep(1000*vitesseDefilement);
//		AfficherMotif(chaine[0]);
		ViewPattern(chaine[0]);
	}

}


SenseHat& SenseHat::operator<<(const std::string &message)
{
    buffer += message;
    return *this;
}

SenseHat& SenseHat::operator<<(const int valeur)
{
    buffer += std::to_string(valeur);
    return *this;
}

SenseHat& SenseHat::operator<<(const double valeur)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << valeur;
    buffer += ss.str();
    return *this;
}

SenseHat& SenseHat::operator<<(const char caractere)
{
   buffer += std::string(1, caractere);
   return *this;
}

SenseHat& SenseHat::operator<<(const char * message)
{
   buffer += std::string(message);
   return *this;
}

SenseHat& SenseHat::operator<<(const bool valeur)
{
   buffer +=  std::to_string(valeur);
   return *this;
}
// Méthode Flush() Affiche le buffer puis le vide
void SenseHat::Flush()
{
    buffer += "  ";
 //   AfficherMessage(buffer, 80, couleur);
    ViewMessage(buffer, 80, couleur);
    buffer = " ";
}

// Modificator endl
// (endl manipulator) effectue un flush du buffer
SenseHat& endl(SenseHat& os)
{
   os.Flush();
   return os;
}


SenseHat& flush(SenseHat& os)
{
    os.Flush();
    return os;
}


_SetCouleur setcouleur(int n)
{
    return { n };
}


SenseHat&  operator<<(SenseHat& os, _SetCouleur couleur)
{
//    os.FixerCouleur(couleur.val);
    os.SetColor(couleur.val);
    return os;
}

_SetRotation setrotation(int n)
{
    return { n };
}


SenseHat&  operator<<(SenseHat& os, _SetRotation rotation)
{
//    os.FixerRotation(rotation.val);
    os.SetRotation(rotation.val);
    return os;
}
