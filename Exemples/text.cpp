/*

Compilation : g++ text.cpp -l SenseHat -l RTIMULib -o text


*/
#include <SenseHat.h>

int main() {

    SenseHat carte;

    carte.Effacer();
    while(1){
    	carte.AfficherMessage(" Bonjour 9876543210 !");
	sleep(1);
    }
}
