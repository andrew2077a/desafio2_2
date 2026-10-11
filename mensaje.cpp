#include "mensaje.h"

// Constructor
Mensaje::Mensaje(unsigned short emisor, unsigned short receptor, const string& texto, const Fecha& fecha)
    : emisor(emisor), receptor(receptor), texto(texto), fecha(fecha), leido(false) {}

// Copia
Mensaje::Mensaje(const Mensaje& otro)
    : emisor(otro.emisor), receptor(otro.receptor), texto(otro.texto), fecha(otro.fecha), leido(otro.leido) {}

// Destructor
Mensaje::~Mensaje() {
}

void Mensaje::marcarLeido() {
    leido = true;
}

bool Mensaje::estaLeido() const {
    return leido;
}

unsigned short Mensaje::getEmisor() const {
    return emisor;
}

unsigned short Mensaje::getReceptor() const {
    return receptor;
}

const string& Mensaje::getTexto() const {
    return texto;
}

const Fecha& Mensaje::getFecha() const {
    return fecha;
}

// Memoria
unsigned int Mensaje::bytesUsados() const {
    return sizeof(Mensaje) + texto.size();
}
