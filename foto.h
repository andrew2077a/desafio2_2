#ifndef FOTO_H
#define FOTO_H

#include <string>
#include "fecha.h"
using namespace std;


class Foto {
private:
    const Fecha fecha;
    const unsigned int tamano;
    const std::string nombre;
    unsigned int likes;

public:
    Foto(const string& nombre, const Fecha& fecha, unsigned int tamano, unsigned int likes);

    Foto(const Foto& otra);

    ~Foto();

    void darLike();
    void quitarLike();
    const string& getNombre() const;
    const Fecha& getFecha() const;
    unsigned int getTamano() const;
    unsigned int getLikes() const;

    unsigned int bytesUsados() const;
};

#endif