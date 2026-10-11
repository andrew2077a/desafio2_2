#include "mensajes.h"

mensaje::mensaje(unsigned short emisor, unsigned short receptor, const std::string& texto, const Fecha& fecha)
    : emisor(emisor), receptor(receptor), texto(texto), fecha(fecha), leido(false) {}

mensaje::mensaje(const mensaje& otro)
    : emisor(otro.emisor), receptor(otro.receptor), texto(otro.texto), fecha(otro.fecha), leido(otro.leido) {}

mensaje::~mensaje() {
}

void mensaje::marcarLeido() {
    leido = true;
}

bool mensaje::estaLeido() const {
    return leido;
}

unsigned short mensaje::getEmisor() const {
    return emisor;
}

unsigned short mensaje::getReceptor() const {
    return receptor;
}

const std::string& mensaje::getTexto() const {
    return texto;
}

const Fecha& mensaje::getFecha() const {
    return fecha;
}

unsigned int mensaje::bytesUsados() const {
    return sizeof(mensaje) + texto.size();
}