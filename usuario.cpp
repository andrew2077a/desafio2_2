#include "Usuario.h"

using namespace std;

Usuario::Usuario(unsigned short id, string nombreUsuario, string contrasena, string correo,
                 string ciudad, string pais, Fecha fechaNacimiento, Fecha fechaInscripcion)
    : id(id), nombreUsuario(nombreUsuario), contrasena(contrasena), correo(correo),
    ciudad(ciudad), pais(pais), fechaNacimiento(fechaNacimiento), fechaInscripcion(fechaInscripcion) {
}
Usuario::~Usuario() {
    // Libera cada álbum (contar cada vuelta)
    for (unsigned int i = 0; i < albumes.getTamano(); i++) {
        delete albumes[i];
        Medidor::contar();
    }
    // Libera cada mensaje recibido (contar cada vuelta)
    for (unsigned int i = 0; i < mensajesRecibidos.getTamano(); i++) {
        delete mensajesRecibidos[i];
        Medidor::contar();
    }
    // NO se toca mensajesEnviados: solo apunta a mensajes de otros
}
bool Usuario::verificarContrasena( const string& contrasena)const {
    return this->contrasena==contrasena;
}
void Usuario::agregarAlbum(Album* album){
    this->albumes+=album;
}

