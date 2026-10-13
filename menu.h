#ifndef MENU_H
#define MENU_H

#include <string>
#include "Album.h"
#include "Usuario.h"
#include "Medidor.h"


class UdeABook {
public:
    UdeABook() {}
    ~UdeABook() {}
    bool cargarDatos() { return true; }
    bool guardarDatos() { return true; }
    bool iniciarSesion(const std::string&, const std::string&) { return true; }
    void cerrarSesion() {}
    Usuario* getUsuarioActivo() { return nullptr; }
    Usuario* getUsuario(unsigned short) { return nullptr; }
    Usuario* obtenerAmigo(const std::string&) { return nullptr; }
    unsigned char elegirTresAmigos(unsigned short*, const unsigned short*) { return 0; }
    unsigned char enviarSolicitud(const std::string&) { return 0; }
    void aceptarSolicitud(unsigned int) {}
    unsigned short obtenerSugerencias(unsigned short, unsigned short*, unsigned short*) { return 0; }
    unsigned char enviarMensaje(const std::string&, const std::string&) { return 0; }
    unsigned long long bytesUsados() const { return sizeof(UdeABook); }
};

class Menu {
private:
    UdeABook& sistema;
    const std::string SUB_RUTA_FIJA;

    bool pantallaIngreso();
    void mostrarTresAmigos();
    void menuPrincipal();
    void menuAmistades();
    void enviarSolicitud();
    void aceptarSolicitudes();
    void visitarPerfil();
    void carrusel(const Album& album, const std::string& nombreDueno);
    void verSugerencias();
    void menuMensajes();
    void enviarMensaje();
    void leerNuevos();
    void buscarPorFecha();
    void mostrarMedicion();

public:
    explicit Menu(UdeABook& sistema);
    ~Menu(); // Destructor explícito
    void ejecutar();
};

#endif // MENU_H