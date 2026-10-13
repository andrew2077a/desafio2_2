#ifndef MENSAJES_H
#define MENSAJES_H

#include <string>
#include "Fecha.h"
using namespace std;

class Mensaje {
private:
    const unsigned short emisor;
    const unsigned short receptor;
    const string texto;
    const Fecha fecha;
    bool leido;

public:
    Mensaje(unsigned short emisor, unsigned short receptor, const string& texto, const Fecha& fecha);

    Mensaje(const Mensaje& otro);

    ~Mensaje();

    void marcarLeido();
    bool estaLeido() const;

    unsigned short getEmisor() const;
    unsigned short getReceptor() const;
    const string& getTexto() const;
    const Fecha& getFecha() const;   // cambio: referencia constante

    unsigned int bytesUsados() const;
};

#endif
