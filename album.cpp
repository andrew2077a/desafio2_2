#include "album.h"

// Inicializa atributos directamente
Album::Album(const string& nombre, unsigned char privacidad, unsigned int cantidadFotos)
    : nombre(nombre), privacidad(privacidad), fotos(cantidadFotos) {}

// Constructor copia (duplica fotos dinámicamente)
Album::Album(const Album& otro)
    : nombre(otro.nombre), privacidad(otro.privacidad), fotos(otro.fotos.getTamano()) {
    for (unsigned int i = 0; i < otro.fotos.getTamano(); ++i) {
        Medidor::contar();
        Foto* nuevaFoto = new Foto(*(otro.fotos[i]));
        fotos += nuevaFoto;
    }
}

// Libera memoria de las fotos
Album::~Album() {
    for (unsigned int i = 0; i < fotos.getTamano(); ++i) {
        Medidor::contar();
        delete fotos[i];
    }
}

// Agrega foto a la lista
void Album::agregarFoto(Foto* foto) {
    fotos += foto;
}

// Retorna foto en posición i
Foto* Album::getFoto(unsigned int i) const {
    return fotos[i];
}

// Total de fotos
unsigned int Album::cantidadFotos() const {
    return fotos.getTamano();
}

// Valida si se puede ver (público, amigos o dueño)
bool Album::esVisiblePara(bool esDueno, bool esAmigo) const {
    if (esDueno) {
        return true;
    }
    if (privacidad == 2) {
        return true;
    }
    if (privacidad == 1) {
        return esAmigo;
    }
    return false;
}

// Nombre del álbum
const string& Album::getNombre() const {
    return nombre;
}

// Nivel de privacidad
unsigned char Album::getPrivacidad() const {
    return privacidad;
}

// Memoria usada en RAM
unsigned int Album::bytesUsados() const {
    unsigned int total = sizeof(Album) + nombre.size();
    total += fotos.bytesUsados();
    for (unsigned int i = 0; i < fotos.getTamano(); ++i) {
        Medidor::contar();
        total += fotos[i]->bytesUsados();
    }
    return total;
}