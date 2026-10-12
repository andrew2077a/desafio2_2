#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include "Foto.h"
#include "Lista.h"
#include "medidor.h"

using namespace std;

class Album {
private:
    const string nombre;
    const unsigned char privacidad;
    Lista<Foto*> fotos;


public:
    Album(const string& nombre, unsigned char privacidad, unsigned int cantidadFotos);

    Album(const Album& otro);

    ~Album();

    void agregarFoto(Foto* foto);
    Foto* getFoto(unsigned int i) const;
    unsigned int cantidadFotos() const;
    bool esVisiblePara(bool esDueno, bool esAmigo) const;

    const string& getNombre() const;
    unsigned char getPrivacidad() const;

    unsigned int bytesUsados() const;
};

#endif // ALBUM_H
