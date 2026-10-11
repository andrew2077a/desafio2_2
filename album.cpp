#include "album.h"

album::album(const std::string& nombre, unsigned char privacidad, unsigned int cantidadFotos)
    : nombre(nombre), privacidad(privacidad), fotos(cantidadFotos) {}

album::album(const album& otro)
    : nombre(otro.nombre), privacidad(otro.privacidad), fotos(otro.fotos.getTamano()) {   // cambio: getTamano()
    for (unsigned int i = 0; i < otro.fotos.getTamano(); ++i) {
        Medidor::contar();
        Foto* nuevaFoto = new Foto(*(otro.fotos[i]));
        fotos += nuevaFoto;
    }
}

album::~album() {
    for (unsigned int i = 0; i < fotos.getTamano(); ++i) {
        Medidor::contar();
        delete fotos[i];
    }
}

void album::agregarFoto(Foto* foto) {
    fotos += foto;
}

Foto* album::getFoto(unsigned int i) const {
    return fotos[i];
}

unsigned int album::cantidadFotos() const {
    return fotos.getTamano();
}

bool album::esVisiblePara(bool esDueno, bool esAmigo) const {
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

const std::string& album::getNombre() const {
    return nombre;
}

unsigned char album::getPrivacidad() const {
    return privacidad;
}

unsigned int album::bytesUsados() const {
    unsigned int total = sizeof(album) + nombre.size();
    total += fotos.bytesUsados();
    for (unsigned int i = 0; i < fotos.getTamano(); ++i) {
        Medidor::contar();
        total += fotos[i]->bytesUsados();
    }
    return total;
}