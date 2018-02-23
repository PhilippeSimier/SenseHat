/*
Compilation: g++ text2.cpp -l SenseHat -l RTIMULib -o text2
*/

#include <SenseHat.h>

int main() {

    SenseHat carte;
    char lettre;

    carte.Effacer();
    while(1)
    	carte.AfficherMessage("(àâ) {éèê} [ç\\] £$", 90, ORANGE);
    return 0;
}
