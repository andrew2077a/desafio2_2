#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include "Fecha.h"
#include "Lista.h"
#include "album.h"
#include "mensajes.h"

class Usuario {
private:
    unsigned short id;
    std::string nombreUsuario;
    std::string contrasena;
    std::string correo;
    std::string ciudad;
    std::string pais;
    Fecha fechaNacimiento;
    Fecha fechaInscripcion;

    Lista<album*> albumes;
    Lista<Mensaje*> mensajesRecibidos;
    Lista<Mensaje*> mensajesEnviados;
    Lista<unsigned short> solicitudes;

    Usuario(const Usuario&);
    Usuario& operator=(const Usuario&);

public:
    Usuario();
    Usuario(unsigned short id, std::string nombreUsuario, std::string contrasena, std::string correo,
            std::string ciudad, std::string pais, Fecha fechaNacimiento, Fecha fechaInscripcion);
    ~Usuario();
    bool verificarContrasena( const string& contrasena)const;
    void agregarAlbum(album* album);
};

#endif // USUARIO_H