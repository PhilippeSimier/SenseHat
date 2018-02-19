/*
Author : Philippe CRUCHET -  Philippe SIMIER -  Christophe GRILO
Compilation : g++ caractere.cpp -l SenseHat -l RTIMULib -o caractere
*/

#include <SenseHat.h>

int main() {

    SenseHat carte;
    char lettre;

    carte.Effacer();
    while(1){
    	for (lettre='A'; lettre <= 'Z'; lettre++){
    	   carte.AfficherLettre(lettre);
	   sleep(1);
    	}
    	for (lettre='a'; lettre <= 'z'; lettre++){
            carte.AfficherLettre(lettre);
            sleep(1);
    	}
        for (lettre='0'; lettre <= '9'; lettre++){
            carte.AfficherLettre(lettre);
            sleep(1);
        }
    }
}
