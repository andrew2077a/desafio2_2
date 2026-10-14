#include "Menu.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

Menu::Menu(UdeABook& sistema)
    : sistema(sistema),
    SUB_RUTA_FIJA("/users/sub-ruta-X-fija/"),
    PALABRA_SALIR ("salir"){}

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

        if (nombre == PALABRA_SALIR || nombre == "SALIR") {
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


void Menu::mostrarMedicion() const {
    cout << "\n     Consumo de recursos    \n";
    cout << "Iteraciones: " << Medidor::getIteraciones() << "\n";
    cout << "Memoria (bytes): " << Medidor::getMemoria() << "\n";
    cout << "                               \n";
}

void Menu::menuPrincipal() {
    bool fotosActualizadas = false;
    string opcion;

    while (true) {
        cout << "\nMenú de opciones:\n\n";
        cout << "1.- Administrar amistades\n";
        cout << "2.- Visitar perfil de un amigo\n";
        cout << "3.- Ver sugerencias de amistad\n";
        cout << "4.- Administrar mensajes\n\n";
        if (!fotosActualizadas) {
            cout << "Ingrese N  para alternar el grupo de fotos de perfil de amigos.\n";
        }
        cout << "Ingrese S para cerrar su sesión.\n\n";
        cout << "Su opción: ";
        getline(cin, opcion);

        if (opcion.empty()) {
            continue;
        }

        Medidor::reiniciar();
        switch (opcion[0]) {
        case '1': menuAmistades();     mostrarMedicion(); break;
        case '2': visitarPerfil();     mostrarMedicion(); break;
        case '3': verSugerencias();    mostrarMedicion(); break;
        case '4': menuMensajes();      mostrarMedicion(); break;
        case 'N':
        case 'n':
            if (fotosActualizadas) {
                cout << "Las fotos solo se pueden actualizar una vez.\n";
            } else {
                mostrarTresAmigos();
                fotosActualizadas = true;
                mostrarMedicion();
            }
            break;
        case 'S':
        case 's':
            return;
        default:
            cout << "Opción no válida.\n";
        }
    }
}