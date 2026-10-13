#include "Menu.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

Menu::Menu(UdeABook& sistema)
    : sistema(sistema), SUB_RUTA_FIJA("/users/sub-ruta-X-fija/") {}

Menu::~Menu() {
}

void Menu::ejecutar() {
    srand(time(nullptr));

    Medidor::reiniciar();
    if (!sistema.cargarDatos()) {
        cout << "Error crítico: No se pudieron cargar los datos del sistema.\n";
        return;
    }
    mostrarMedicion();

    while (pantallaIngreso()) {
        mostrarTresAmigos();
        menuPrincipal();
        sistema.guardarDatos();
        sistema.cerrarSesion();
    }

    sistema.guardarDatos();
    cout << "¡Gracias por usar Ude@Book! Hasta pronto.\n";
}

bool Menu::pantallaIngreso() {
    Medidor::reiniciar();
    string nombre, contrasena;

    while (true) {
        cout << "\n ************************** \n";
        cout << "      BIENVENIDO A Ude@Book   \n";
        cout << "   ************************** \n";
        cout << "Ingrese nombre de usuario (o 'salir' para terminar): ";
        getline(cin, nombre);

        if (nombre == "salir" || nombre == "SALIR") {
            return false;
        }

        cout << "Ingrese contraseña: ";
        getline(cin, contrasena);

        if (sistema.iniciarSesion(nombre, contrasena)) {
            cout << "¡Inicio de sesión exitoso!\n";
            return true;
        } else {
            cout << "Usuario o contraseña incorrectos. Intente nuevamente.\n";
        }
    }
}
