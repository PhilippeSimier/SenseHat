/*
	Classe SenseHat
	compilation : g++ -c sensehat.cpp   -->  on obtient sensehat.o

	Pour convertir le fichier objet sensehat.o en une librairie statique libSenseHat.a
	ar cr libSenseHat.a sensehat.o

	ar s libSenseHat.a

	puis copier les fichiers
	cp libSenseHat.a /usr/lib/libSenseHat.a
	cp sensehat.h    /usr/include/SenseHat.h

*/

#include "SenseHat.h"


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

}

SenseHat::~SenseHat()
{
    delete settings;
}

void SenseHat::Version()
{
    printf("\nSenseHat PCT Version 1.0\n");
}

void SenseHat::AfficherMessage(char *message, int vitesseDefilement, uint16_t CouleurTexte, uint16_t couleurFond)
{

}

void SenseHat::AfficherLettre(char lettre, uint16_t couleurTexte, uint16_t couleurFond)
{

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

float SenseHat::ObtenirTemperature()
{
    RTIMU_DATA imuData = imu->getIMUData();
    pressure->pressureRead(imuData);
    return imuData.temperature;;
}

float SenseHat::ObtenirPression()
{
    RTIMU_DATA imuData = imu->getIMUData();
    pressure->pressureRead(imuData);
    return imuData.pressure;
}

float SenseHat::ObtenirHumidite()
{
    RTIMU_DATA imuData = imu->getIMUData();
    humidite->humidityRead(imuData);
    return imuData.humidity;
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
        printf("Pas de mesure humidité\n");
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

