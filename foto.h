#ifndef FOTO_H
#define FOTO_H

#include <string>
#include "Fecha.h"

class Foto {
private:
    const Fecha fecha;
    const unsigned int tamano;
    const std::string nombre;
    unsigned int likes;

public:
    Foto(const std::string& nombre, const Fecha& fecha, unsigned int tamano, unsigned int likes);

    Foto(const Foto& otra);

    ~Foto();

    void darLike();
    void quitarLike();
    const std::string& getNombre() const;
    const Fecha getFecha() const;
    unsigned int getTamano() const;
    unsigned int getLikes() const;

    unsigned int bytesUsados() const;
};

#endif
