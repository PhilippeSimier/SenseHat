/*
Compilation: g++ text2.cpp -l SenseHat -l RTIMULib -o text2
*/

#include <SenseHat.h>

int main() {

    SenseHat carte;
    char lettre;

    carte.Effacer();
    while(1)
    	carte.AfficherMessage("à â ç è é ê $ ,");
    return 0;
}
