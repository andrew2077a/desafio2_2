#ifndef UDEABOOK_H
#define UDEABOOK_H
#include "usuario.h"
#include "sugenrenciasAmigos.h"
#include <iostream>
#include <fstream>
#include <string>
#include "medidor.h"


class UdeABook{
private:
    Lista<Usuario*> usuarios;
    SugerenciasAmigos amistades;
    short usuarioActivo;
    bool cargarUsuarios();
    bool cargarAmistades();
    bool cargarMensajes();
    bool cargarAlbumes();

public:
    UdeABook();
    ~UdeABook();
    bool cargarDatos();
};

#endif // UDEABOOK_H
