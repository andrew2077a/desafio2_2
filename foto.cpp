#include "Foto.h"

// Constructor parametrizado
Foto::Foto(const string& nombre, const Fecha& fecha, unsigned int tamano, unsigned int likes)
    : fecha(fecha), tamano(tamano), nombre(nombre), likes(likes) {}

// Constructor de copia
Foto::Foto(const Foto& otra)
    : fecha(otra.fecha), tamano(otra.tamano), nombre(otra.nombre), likes(otra.likes) {}

// Destructor
Foto::~Foto() {
}

// Incrementa el contador de likes
void Foto::darLike() {
    likes = likes + 1;
}

// Decrementa el contador de likes
void Foto::quitarLike() {
    if (likes > 0) {
        likes = likes - 1;
    }
}

// Retorna referencia constante al nombre
const string &Foto::getNombre() const {
    return nombre;
}

// Retorna la fecha de la foto
const Fecha Foto::getFecha() const {
    return fecha;
}

// Retorna el tamaño en archivo
unsigned int Foto::getTamano() const {
    return tamano;
}

// Retorna la cantidad de likes
unsigned int Foto::getLikes() const {
    return likes;
}

// Memoria estática del objeto más la longitud del string dinámico
unsigned int Foto::bytesUsados() const {
    return sizeof(Foto) + nombre.size();
}
