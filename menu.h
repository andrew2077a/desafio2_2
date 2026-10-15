#ifndef MENU_H
#define MENU_H

#include <string>
#include "Album.h"
#include "Usuario.h"
#include "medidor.h"
#include "UdeABook.h"
using namespace std;
class Menu {
private:
    UdeABook& sistema;
    const string SUB_RUTA_FIJA;
    const string PALABRA_SALIR;
    const string RUTA_DATOS;

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

public:
    explicit Menu(UdeABook& sistema);
    ~Menu();
    void ejecutar();
};

#endif // MENU_H
