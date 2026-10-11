#ifndef MENSAJES_H
#define MENSAJES_H

#include <string>
#include "Fecha.h"

class mensaje {
private:
    const unsigned short emisor;
    const unsigned short receptor;
    const std::string texto;
    const Fecha fecha;
    bool leido;

public:
    mensaje(unsigned short emisor, unsigned short receptor, const std::string& texto, const Fecha& fecha);

    mensaje(const mensaje& otro);

    ~mensaje();

    void marcarLeido();
    bool estaLeido() const;

    unsigned short getEmisor() const;
    unsigned short getReceptor() const;
    const std::string& getTexto() const;
    const Fecha& getFecha() const;   // cambio: referencia constante

    unsigned int bytesUsados() const;
};

#endif