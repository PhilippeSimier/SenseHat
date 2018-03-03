/*
Author : Philippe SIMIER
Compilation : g++ caractere.cpp -l SenseHat -l RTIMULib -o caractere
*/

#include <SenseHat.h>

int main() {

    SenseHat carte;

    carte.Effacer();
    while(1){
    	for (char lettre='A'; lettre <= 'Z'; lettre++){
    	   carte.AfficherLettre(lettre);
	   sleep(1);
    	}
    	for (char lettre='a'; lettre <= 'z'; lettre++){
            carte << lettre << endl;
    	}
        for (char chiffre='0'; chiffre <= '9'; chiffre++){
            carte << chiffre;
        }
	carte << flush;
    }
}
