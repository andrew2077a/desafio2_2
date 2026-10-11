#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include "Foto.h"
#include "Lista.h"
#include "medidor.h"

class album {
private:
    const std::string nombre;
    const unsigned char privacidad;
    Lista<Foto*> fotos;

    album& operator=(const album&);

public:
    album(const std::string& nombre, unsigned char privacidad, unsigned int cantidadFotos);

    album(const album& otro);

    ~album();

    void agregarFoto(Foto* foto);
    Foto* getFoto(unsigned int i) const;
    unsigned int cantidadFotos() const;
    bool esVisiblePara(bool esDueno, bool esAmigo) const;

    const std::string& getNombre() const;
    unsigned char getPrivacidad() const;

    unsigned int bytesUsados() const;
};

#endif // ALBUM_H
