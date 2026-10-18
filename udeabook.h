#ifndef UDEABOOK_H
#define UDEABOOK_H
#include "usuario.h"
#include "sugenrenciasAmigos.h"
#include <string>
#include "medidor.h"


class UdeABook{
private:
    Lista<Usuario*> usuarios;
    SugerenciasAmigos amistades;
    short usuarioActivo;
    bool cambiosAmistades;
    bool cambiosMensajes;
    bool cambiosLikes;
    bool cargarUsuarios();
    bool cargarAmistades();
    bool cargarMensajes();
    bool cargarAlbumes();
    static Fecha textoAFecha(const string& texto);
    bool guardarAmistades();
    bool guardarMensajes();
    bool guardarLikes();
    short buscarUsuario(const string& nombre);



public:
    UdeABook();
    ~UdeABook();
    bool cargarDatos();
    bool guardarDatos(bool conAmistades = true, bool conMensajes = true, bool conLikes = true);
    bool iniciarSesion(const string& nombre , const string& contrasena);
    void cerrarSesion();
    Usuario* getUsuarioActivo();
    Usuario* getUsuario( unsigned short id );
    Usuario* obtenerAmigo(const string& nombre);
    unsigned char pesoPorAmigosEnComun(unsigned short comunes);
    unsigned char elegirTresAmigos( unsigned short* resultado,const unsigned short* previos );
    unsigned char enviarSolicitud(const string& nombre);
    void aceptarSolicitud(unsigned int posicion );
    unsigned short obtenerSugerencias(unsigned short k,unsigned short* ids,unsigned short* comunes);
    unsigned char enviarMensaje(const string& nombreAmigo , const string& texto );
    void marcarCambioMensajes();
    void marcarCambioLikes();
    unsigned long long bytesUsados();
};

#endif // UDEABOOK_H