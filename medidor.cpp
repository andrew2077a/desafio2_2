#include "medidor.h"

unsigned long long Medidor::iteraciones=0;
unsigned long long Medidor::memoria=0;

Medidor::Medidor(){}


void Medidor::reiniciar() {
    iteraciones = 0;
    memoria = 0;
}

void Medidor::contar(unsigned long long n){
    iteraciones+=n;
}

void Medidor::sumarMemoria(unsigned long long bytes){
    memoria+=bytes;
}
unsigned long long Medidor::getIteraciones(){
    return iteraciones;
}
unsigned long long Medidor::getMemoria(){
    return memoria;
}


