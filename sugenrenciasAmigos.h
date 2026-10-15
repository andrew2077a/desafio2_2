#ifndef SUGENRENCIAS_AMIGOS_H
#define SUGENRENCIAS_AMIGOS_H

#include "medidor.h"

class SugerenciasAmigos {
private:
    unsigned short cantidadUsuarios;
    unsigned int palabras;
    unsigned long long* bloque;

    unsigned int posicion(unsigned short i, unsigned short j) const;

public:
    SugerenciasAmigos();
    SugerenciasAmigos(const SugerenciasAmigos& otro);
    ~SugerenciasAmigos();

    void reiniciar(unsigned short cantidadUsuarios);
    void crearAmistad(unsigned short a, unsigned short b);
    bool sonAmigos(unsigned short a, unsigned short b) const;
    unsigned short contarAmigos(unsigned short a) const;
    unsigned short listarAmigos(unsigned short a, unsigned short* destino) const;
    unsigned short amigosEnComun(unsigned short a, unsigned short b) const;
    unsigned short calcularSugerencias(unsigned short usuario, unsigned short k, unsigned short* ids, unsigned short* comunes) const;

    unsigned long long bytesUsados() const;
};

#endif