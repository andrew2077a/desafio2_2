#ifndef MENU_H
#define MENU_H

#include <string>
#include "album.h"
#include "usuario.h"
#include "medidor.h"
#include "UdeABook.h"
using namespace std;
class Menu {
private:
    UdeABook& sistema;
    const string SUB_RUTA_FIJA;

    unsigned short ultimoGrupo[3];
    unsigned short cantidadUltimo;

    bool pantallaIngreso();
    void mostrarTresAmigos();
    void menuPrincipal();
    void menuAmistades();
    void enviarSolicitud();
    void aceptarSolicitudes();
    void visitarPerfil();
    void carrusel(const Album& album, const string& nombreDueno);
    void verSugerencias();
    void menuMensajes();
    void enviarMensaje();
    void leerNuevos();
    void buscarPorFecha();
    void mostrarMedicion(const string& externos = "ninguno") const;
    bool leerNumero(const string& texto, unsigned int& valor) const;
    bool leerFecha(const string& texto, Fecha& resultado) const;
    void guardarYMedir(bool amistades, bool mensajes, bool likes);


public:
    explicit Menu(UdeABook& sistema);
    ~Menu();
    void ejecutar();
};

#endif // MENU_H
