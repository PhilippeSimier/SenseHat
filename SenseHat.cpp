/*
    Classe SenseHat
    compilation : make
    installation : make install


    Author : Philippe CRUCHET (PCT) -  Philippe SIMIER (PSR) -  Christophe GRILO (CGO)
*/

#include "SenseHat.h"
#include "font.h"
#include <iostream>

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
    struct dirent **namelist;
    int i, ndev;
    int fd = -1;
    int sortie = false;

    ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, versionsort);
    if (ndev <= 0)
        exit (EXIT_FAILURE);

    i=0;
    do
    {
        char fname[64];
        char name[256];

        snprintf(fname, sizeof(fname),
                 "%s/%s", DEV_INPUT_EVENT, namelist[i++]->d_name);

        fd = open(fname, O_RDONLY );
        if (fd < 0)
            exit (EXIT_FAILURE);

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


SenseHat::SenseHat()
{
    settings = new RTIMUSettings("RTIMULib");
    imu = RTIMU::createIMU(settings);
    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL))
    {
        printf("Pas d'IMU trouvé \n");
        exit(1);
    }
    imu->IMUInit();
    imu->setSlerpPower(0.02);
    imu->setGyroEnable(true);
    imu->setAccelEnable(true);
    imu->setCompassEnable(true);
    InitialiserLeds();
    InitialiserJoystik();
    InitialiserHumidite();
    InitialiserPression();
    buffer="";
}

SenseHat::~SenseHat()
{
    delete settings;
}

SenseHat& SenseHat::operator<<(SenseHat& (*m)(SenseHat&))
{
      return (*m)(*this);
}

void SenseHat::Version()
{
    printf("\nSenseHat PCT,PSR,CGO Version 1.1.0\n");
}


void SenseHat::AfficherLettre(char lettre, uint16_t couleurTexte, uint16_t couleurFond)
{
	uint16_t chr[8][8];
	ConvertirCaractereEnMotif(lettre,chr,couleurTexte,couleurFond);
	AfficherMotif(chr);
}


void SenseHat::AllumerPixel(int ligne, int colonne, uint16_t couleur)
{
    fb->pixel[ligne%8][colonne%8]=couleur;
}

uint16_t SenseHat::ObtenirPixel(int ligne, int colonne)
{

    return fb->pixel[ligne%8][colonne%8] ;
}

void SenseHat::AfficherMotif(uint16_t motif[][8])
{
    memcpy(fb,motif,128);
}

void SenseHat::PivoterMotif(int rotation)
{
    int ligne;
    int colonne;
    uint16_t tabAux[8][8];


    for(ligne=0; ligne <8 ; ligne++)
    {
        for(colonne=0 ; colonne <8 ; colonne++)
        {
            switch(rotation)
            {
            case 90:
                tabAux[7 - colonne][ligne] = fb->pixel[ligne][colonne];
                break;
            case 180:
                tabAux[7 - ligne][7 - colonne] = fb->pixel[ligne][colonne];
                break;
            case 270:
                tabAux[colonne][ligne] = fb->pixel[ligne][colonne];
                break;
            }
        }
    }

    AfficherMotif(tabAux);
}

void SenseHat::Effacer(uint16_t couleur)
{
    memset(fb, couleur, 128);
}

char SenseHat::ScannerJoystick()
{
    return handle_events(joystick);
}

uint16_t SenseHat::ConvertirRGB565(uint8_t rouge, uint8_t vert, uint8_t bleu)
{

   bleu  &= 0xF8;
   vert  &= 0xFC;
   rouge &= 0xF8;

   return ((rouge<<8) + (vert<<3) + (bleu>>3));
}

uint16_t SenseHat::ConvertirRGB565(uint8_t couleur[])
{
    return ConvertirRGB565(couleur[1],couleur[2],couleur[3]);
}

COULEUR SenseHat::ConvertirRGB565(std::string hexCode)
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

   return ConvertirRGB565(r,g,b);


}

float SenseHat::ObtenirTemperature()
{
    RTIMU_DATA data;
    pressure->pressureRead(data);
    return data.temperature;;
}

// Méthode pour obtenir la pression
float SenseHat::ObtenirPression()
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

// Méthode pour obtenir l'humidité
float SenseHat::ObtenirHumidite()
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

void SenseHat::ObtenirOrientation(float &pitch, float &roll, float &yaw)
{
    while (imu->IMURead()){
	RTIMU_DATA imuData = imu->getIMUData();
	pitch = imuData.gyro.x();
	roll  = imuData.gyro.y();
	yaw   = imuData.gyro.z();
    }
}

void SenseHat::ObtenirAcceleration(float &x, float &y, float &z)
{
    while (imu->IMURead()){
	RTIMU_DATA imuData = imu->getIMUData();
	x = imuData.accel.x();
	y = imuData.accel.y();
	z = imuData.accel.z();
    }
}

void SenseHat::ObtenirMagnetisme(float &x, float &y, float &z)
{
    while (imu->IMURead()){
        RTIMU_DATA imuData = imu->getIMUData();
        x = imuData.compass.x();
        y = imuData.compass.y();
        z = imuData.compass.z();
    }
}

/* Renvoie la valeur du vecteur champ magnétique en coordonnées sphérique
        teta l'angle mesuré depuis l'axe des X
        si teta = 0 alors l'axe X est dirigé vers le nord magnétique
	si teta = 180 ou -180 alors l'axe des X est dirigé vers le sud
*/
void SenseHat::ObtenirMagnetismeSpherique(float &ro, float &teta, float &delta)
{
    float x,y,z;

    ObtenirMagnetisme(x,y,z);
    teta = atan2 (y,x) * 180/PI;
    ro   = sqrt(x*x + y*y + z*z);
    delta =  atan2 (z,sqrt(x*x + y*y)) * 180/PI;
}

void SenseHat::InitialiserLeds()
{
    int fbfd ;

    fbfd = open_fbdev("RPi-Sense FB");
    if (fbfd > 0)
    {
        fb = (struct fb_t*)mmap(0, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
        if (!fb)
        {
            printf("Failed to mmap.\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf("Error: cannot open framebuffer device.\n");
        close(fbfd);
        exit(EXIT_FAILURE);

    }

    memset(fb, 0, 128);
}

void SenseHat::InitialiserJoystik()
{
    joystick = open_evdev("Raspberry Pi Sense HAT Joystick");
}


void SenseHat::InitialiserPression()
{
    pressure = RTPressure::createPressure(settings);
    if (pressure == NULL)
    {
        printf("Pas de mesure de pression/température \n");
        exit(1);

    }
    pressure->pressureInit();
}

void SenseHat::InitialiserHumidite()
{
    humidite = RTHumidity::createHumidity(settings);
    if(humidite == NULL)
    {
        printf("Pas de mesure d'humidité\n");
        exit(1);
    }
    humidite->humidityInit();

}

void SenseHat::InitialiserOrientation()
{

}

void SenseHat::InitialiserAcceleration()
{
    imu->setAccelEnable(true);
}

void SenseHat::ConvertirCaractereEnMotif(char c,uint16_t image[8][8],uint16_t couleurTexte, uint16_t couleurFond)
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
	ConvertirCaractereEnMotif(255,image,couleurTexte,couleurFond);
}


bool SenseHat::ColonneVide(int numColonne,uint16_t image[8][8],uint16_t couleurFond)
{
    int i=0;
    for(i=0;i<8;i++)
	if(image[i][numColonne]!=couleurFond)
 	   return false;
    return true;
}


void SenseHat::TassementDeLimage(int numColonne,uint16_t image[][8][8], int taille)
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
	ConvertirCaractereEnMotif(message[i],chaine[j],couleurTexte,couleurFond);
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

		if(ColonneVide(k,chaine[i],couleurFond)) // Une colonne Vide avant chaque caractère à ne pas supprimer
		{
			isuivant=(++l)/8;
			ksuivant=(l)%8;
			nombreDeColonneVide=1;

			// compter les colonnes vide après la première afin de les supprimer
			// si plus de 4 c'est le caractère espace que l'on doit garder
			while(ColonneVide(ksuivant,chaine[isuivant],couleurFond) && nombreDeColonneVide++ < 4)
			{
				TassementDeLimage(l,chaine,taille);
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
		AfficherMotif(chaine[0]);
	}

}


SenseHat& operator<<(SenseHat &carte, const std::string &message)
{
    carte.buffer += message;
    return carte;
}

SenseHat& operator<<(SenseHat &carte, const int valeur)
{
    carte.buffer += std::to_string(valeur);
    return carte;
}

SenseHat& operator<<(SenseHat &carte, const double valeur)
{
    carte.buffer += std::to_string(valeur);
    return carte;
}

SenseHat& operator<<(SenseHat &carte, char caractere)
{
   carte.buffer += std::string(1, caractere);
   return carte;
}

// Méthode Flush() Affiche le buffer puis le vide
void SenseHat::Flush()
{
    AfficherMessage(buffer, 80, ORANGE);
    buffer = "";
}

// Modificator endl
// (endl manipulator) effectue un flush du buffer
SenseHat& endl(SenseHat& carte)
{
   carte.Flush();
   return carte;
}


