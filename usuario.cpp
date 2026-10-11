#include "usuario.h"

// Inicializa atributos directamente
Usuario::Usuario(unsigned short id, const string& nombreUsuario, const string& contrasena, const string& correo,
                 const string& ciudad, const string& pais, const Fecha& fechaNacimiento, const Fecha& fechaInscripcion)
    : id(id), nombreUsuario(nombreUsuario), contrasena(contrasena), correo(correo),
    ciudad(ciudad), pais(pais), fechaNacimiento(fechaNacimiento), fechaInscripcion(fechaInscripcion) {
}

// Libera memoria dinámica
Usuario::~Usuario() {
    for (unsigned int i = 0; i < albumes.getTamano(); i++) {
        delete albumes[i];
        Medidor::contar();
    }
    for (unsigned int i = 0; i < mensajesRecibidos.getTamano(); i++) {
        delete mensajesRecibidos[i];
        Medidor::contar();
    }
    // No borra mensajesEnviados (evita doble borrado)
}

// Valida la clave
bool Usuario::verificarContrasena(const string& contrasena) const {
    return this->contrasena == contrasena;
}

// Agrega un álbum
void Usuario::agregarAlbum(Album* album) {
    this->albumes += album;
}

// Retorna álbum i
Album* Usuario::getAlbum(unsigned int i) const {
    return albumes[i];
}

// Total de álbumes
unsigned int Usuario::cantidadAlbumes() const {
    return albumes.getTamano();
}

// Guarda mensaje recibido
void Usuario::recibirMensaje(Mensaje* mensaje) {
    insertarPorFecha(mensajesRecibidos, mensaje);
}

// Guarda mensaje enviado
void Usuario::registrarEnviado(Mensaje* mensaje) {
    insertarPorFecha(mensajesEnviados, mensaje);
}

// Inserción ordenada por fecha
void Usuario::insertarPorFecha(Lista<Mensaje*>& lista, Mensaje* mensaje) {
    unsigned int pos = lista.getTamano();

    while (pos > 0 && mensaje->getFecha() < lista[pos - 1]->getFecha()) {
        pos--;
        Medidor::contar();
    }

    lista.insertarEn(pos, mensaje);
}

// Mensaje recibido i
Mensaje* Usuario::getRecibido(unsigned int i) const {
    return mensajesRecibidos[i];
}

// Total de recibidos
unsigned int Usuario::cantidadRecibidos() const {
    return mensajesRecibidos.getTamano();
}

// Mensaje enviado i
Mensaje* Usuario::getEnviado(unsigned int i) const {
    return mensajesEnviados[i];
}

// Total de enviados
unsigned int Usuario::cantidadEnviados() const {
    return mensajesEnviados.getTamano();
}

// Búsqueda binaria por fecha
unsigned int Usuario::primerIndiceDesde(const Fecha& fecha) const {
    unsigned int izq = 0;
    unsigned int der = mensajesRecibidos.getTamano();

    while (izq < der) {
        Medidor::contar();
        unsigned int medio = (izq + der) / 2;

        if (mensajesRecibidos[medio]->getFecha() < fecha) {
            izq = medio + 1;
        } else {
            der = medio;
        }
    }

    return izq;
}

// Agrega solicitud
void Usuario::agregarSolicitud(unsigned short idEmisor) {
    solicitudes += idEmisor;
}

// Elimina solicitud i
void Usuario::eliminarSolicitud(unsigned int pos) {
    solicitudes.eliminarEn(pos);
}

// Busca solicitud por id
bool Usuario::tieneSolicitudDe(unsigned short id) const {
    for (unsigned int i = 0; i < solicitudes.getTamano(); i++) {
        Medidor::contar();
        if (solicitudes[i] == id) {
            return true;
        }
    }
    return false;
}

// Solicitud en pos i
unsigned short Usuario::getSolicitud(unsigned int pos) const {
    return solicitudes[pos];
}

// Total de solicitudes
unsigned int Usuario::cantidadSolicitudes() const {
    return solicitudes.getTamano();
}

// Getters de consulta
unsigned short Usuario::getId() const {
    return this->id;
}

const string& Usuario::getNombreUsuario() const {
    return this->nombreUsuario;
}

const string& Usuario::getCorreo() const {
    return this->correo;
}

const string& Usuario::getCiudad() const {
    return this->ciudad;
}

const string& Usuario::getPais() const {
    return this->pais;
}

Fecha Usuario::getFechaNacimiento() const {
    return this->fechaNacimiento;
}

Fecha Usuario::getFechaInscripcion() const {
    return this->fechaInscripcion;
}

// Memoria usada en RAM
unsigned int Usuario::bytesUsados() const {
    long long bytesTotal = 0;
    bytesTotal += sizeof(Usuario) + nombreUsuario.size() + contrasena.size() + correo.size() + ciudad.size() + pais.size();

    // Memoria de álbumes
    bytesTotal += albumes.bytesUsados();
    for (unsigned int i = 0; i < albumes.getTamano(); i++) {
        bytesTotal += albumes[i]->bytesUsados();
        Medidor::contar();
    }

    // Memoria de recibidos
    bytesTotal += mensajesRecibidos.bytesUsados();
    for (unsigned int i = 0; i < mensajesRecibidos.getTamano(); i++) {
        bytesTotal += mensajesRecibidos[i]->bytesUsados();
        Medidor::contar();
    }

    // Memoria de listas internas
    bytesTotal += mensajesEnviados.bytesUsados();
    bytesTotal += solicitudes.bytesUsados();

    return bytesTotal;
}
