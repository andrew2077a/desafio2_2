#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include "Fecha.h"
#include "Lista.h"
#include "Album.h"
#include "Mensaje.h"

using namespace std;

class Usuario {
private:
    unsigned short id;
    string nombreUsuario;
    string contrasena;
    string correo;
    string ciudad;
    string pais;
    Fecha fechaNacimiento;
    Fecha fechaInscripcion;

    Lista<Album*> albumes;
    Lista<Mensaje*> mensajesRecibidos;
    Lista<Mensaje*> mensajesEnviados;
    Lista<unsigned short> solicitudes;
    void insertarPorFecha( Lista<Mensaje*>& lista, Mensaje* mensaje);

public:

    Usuario(unsigned short id, const string& nombreUsuario, const string& contrasena, const string& correo,
            const string& ciudad, const string& pais, const Fecha& fechaNacimiento, const Fecha& fechaInscripcion);
    ~Usuario();
    bool verificarContrasena( const string& contrasena)const;
    void agregarAlbum(Album* album);
    Album* getAlbum(unsigned int i)const;
    unsigned int cantidadAlbumes()const;
    void recibirMensaje(Mensaje* mensaje);
    void registrarEnviado(Mensaje* mensaje);
    Mensaje* getRecibido(unsigned int i)const;
    unsigned int cantidadRecibidos()const;
    Mensaje* getEnviado(unsigned int i)const;
    unsigned int cantidadEnviados()const;
    unsigned int primerIndiceDesde(const Fecha& fecha)const;
    void agregarSolicitud(unsigned short idEmisor);
    void eliminarSolicitud(unsigned int pos );
    bool tieneSolicitudDe(unsigned short id )const;
    unsigned short getSolicitud(unsigned int pos)const ;
    unsigned int cantidadSolicitudes()const;
    unsigned short getId() const;
    const string& getNombreUsuario() const;
    const string& getCorreo() const;
    const string& getCiudad() const;
    const string& getPais() const;
    Fecha getFechaNacimiento() const;
    Fecha getFechaInscripcion() const;
    unsigned int bytesUsados()const;


};
#endif // USUARIO_H
