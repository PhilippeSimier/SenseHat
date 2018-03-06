 /***********************************************************************************
/*!
    \file         text.cpp
    \author    	  Philippe SIMIER (Touchard Wahington le Mans)
    \license      GPL2 (see license.txt)
    \date         25 février 2018
    \brief        Programme exemple pour l'affichage d'un message
    \details      Mise en oeuvre de l'opérateur de flux <<
		  pour afficher des messages sur une carte SenseHat

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
    int i=0;
    bool a = true;

    carte.Effacer();
    cout << "Entrer votre message : ";
    getline(cin, message);

    while(1){
	carte << message << ' ' << i++ << 'E' << flush;
	carte << a << endl;
    }
    return 0;
}

