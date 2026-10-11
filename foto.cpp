#include "Foto.h"
Foto::Foto(const std::string& nombre, const Fecha& fecha, unsigned int tamano, unsigned int likes)
    : fecha(fecha), tamano(tamano), nombre(nombre), likes(likes) {}

Foto::Foto(const Foto& otra)
    : fecha(otra.fecha), tamano(otra.tamano), nombre(otra.nombre), likes(otra.likes) {}

Foto::~Foto() {
}

void Foto::darLike() {
    likes = likes + 1;
}

void Foto::quitarLike() {
    if (likes > 0) {
        likes = likes - 1;
    }
}

const std::string& Foto::getNombre() const {
    return nombre;
}

const Fecha& Foto::getFecha() const {
    return fecha;
}

unsigned int Foto::getTamano() const {
    return tamano;
}

unsigned int Foto::getLikes() const {
    return likes;
}

unsigned int Foto::bytesUsados() const {
    return sizeof(Foto) + nombre.size();
}