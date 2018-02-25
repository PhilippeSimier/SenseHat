/***********************************************************************************
/*!
    \file         text.cpp
    \author    	  Philippe SIMIER (Touchard Wahington le Mans)
    \license      GPL2 (see license.txt)
    \date         25 février 2018
    \brief        Programme exemple pour l'affichage d'un message
    \details      Mise en oeuvre de la méthode AfficherMessage()
    \compilation: g++ text.cpp -l SenseHat -l RTIMULib -o text
    \version      v1.0
/***********************************************************************************/

#include <SenseHat.h>
#include <iostream>
#include <string>


using namespace std;

int main() {

    SenseHat carte;
    string message;
    COULEUR jaunePale =  carte.ConvertirRGB565(50,50,0);

    carte.Effacer();
    cout << "Entrer votre message : ";
    getline(cin, message);

    while(1)
    	carte.AfficherMessage(message, 80, jaunePale);
    return 0;
}
