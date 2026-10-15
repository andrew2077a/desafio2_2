#include "Menu.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "UdeABook.h"

using namespace std;

static void mostrarResultadoSolicitud(unsigned char codigo) {
    switch (codigo) {
    case 0: cout << "Solicitud de amistad enviada.\n"; break;
    case 1: cout << "Ese usuario no existe.\n"; break;
    case 2: cout << "No puedes enviarte una solicitud a ti mismo.\n"; break;
    case 3: cout << "Ya son amigos.\n"; break;
    case 4: cout << "Ya le enviaste una solicitud a esa persona.\n"; break;
    case 5: cout << "Esa persona ya te envió una solicitud: acéptala en 'Administrar amistades'.\n"; break;
    default: cout << "No se pudo enviar la solicitud.\n";
    }
}

static void mostrarMensaje(UdeABook& sistema, const Mensaje* m) {
    cout << "\nDe: " << sistema.getUsuario(m->getEmisor())->getNombreUsuario()
    << "  |  Fecha: " << m->getFecha() << "\n"
    << m->getTexto() << "\n";
}

Menu::Menu(UdeABook& sistema)
    : sistema(sistema),
    SUB_RUTA_FIJA("/users/sub-ruta-X-fija/"),
    PALABRA_SALIR ("salir"),
    RUTA_DATOS("datos/"),
    ultimoGrupo{0, 0, 0},
    cantidadUltimo(0) {}

Menu::~Menu() {
}

void Menu::ejecutar() {
    srand(time(nullptr));

    Medidor::reiniciar();
    if (!sistema.cargarDatos()) {
        cout << "Error crítico: No se pudieron cargar los datos del sistema.\n";
        return;
    }
    mostrarMedicion("fstream (lectura de archivos)");

    while (pantallaIngreso()) {
        cantidadUltimo = 0;
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
            Medidor::reiniciar();
            return true;
        } else {
            cout << "Usuario o contraseña incorrectos. Intente nuevamente.\n";
        }
    }
}

void Menu::mostrarTresAmigos() {
    Medidor::reiniciar();

    unsigned short ids[3];
    unsigned short previos[3] = {65535, 65535, 65535};
    Medidor::sumarMemoria(sizeof(ids) + sizeof(previos));


    bool esActualizacion = (cantidadUltimo > 0);
    for (unsigned short i = 0; i < cantidadUltimo; ++i) {
        Medidor::contar();
        previos[i] = ultimoGrupo[i];
    }


    unsigned char n = sistema.elegirTresAmigos(ids, esActualizacion ? previos : nullptr);

    cout << "\n--------------------------------------------\n";
    if (n == 0) {
        cout << (esActualizacion ? "No hay más amigos disponibles para mostrar.\n"
                                 : "Aún no tienes amigos en la plataforma.\n");
    } else {
        for (unsigned char i = 0; i < n; ++i) {
            Medidor::contar();

            const Usuario* amigo = sistema.getUsuario(ids[i]);
            cout << "Foto perfil de amigo " << (i + 1) << ": "
                 << SUB_RUTA_FIJA << amigo->getNombreUsuario() << "/albums/profile.png\n";
            ultimoGrupo[i] = ids[i];
        }
        cantidadUltimo = n;
    }
    cout << "--------------------------------------------\n";
    mostrarMedicion("rand (sorteo de amigos)");
}

void Menu::menuPrincipal() {
    bool fotosActualizadas = false;
    string opcion;
    Usuario* yo = sistema.getUsuarioActivo();

    while (true) {
        cout << "\nBienvenido a UdeABook " << yo->getNombreUsuario() << "\n";
        cout << "\nMenú de opciones:\n\n";
        cout << "1.- Administrar amistades\n";
        cout << "2.- Visitar perfil de un amigo\n";
        cout << "3.- Ver sugerencias de amistad\n";
        cout << "4.- Administrar mensajes\n\n";
        if (!fotosActualizadas && cantidadUltimo > 0) {
            cout << "Ingrese 'N' o 'n' para alternar el grupo de fotos de perfil de amigos.\n";
        }
        cout << "Ingrese 'S' o 's' para cerrar su sesión.\n\n";
        cout << "Su opción: ";
        getline(cin, opcion);

        if (opcion.size() != 1) {
            cout << "Opción no válida.\n";
            continue;
        }

        switch (opcion[0]) {
        case '1':
            menuAmistades();
            sistema.guardarDatos(true, false, false);
            break;
        case '2':
            visitarPerfil();
            sistema.guardarDatos(false, false, true);
            break;
        case '3':
            verSugerencias();
            sistema.guardarDatos(true, false, false);
            break;
        case '4':
            menuMensajes();
            sistema.guardarDatos(false, true, false);
            break;
        case 'N':
        case 'n':
            if (fotosActualizadas || cantidadUltimo == 0) {
                cout << "Las fotos solo se pueden actualizar una vez.\n";
            } else {
                cout << "\n--- Nuevo grupo de fotos de perfil ---";
                mostrarTresAmigos();
                fotosActualizadas = true;
            }
            break;
        case 'S':
        case 's':
            cout << "Cerrando sesión...\n";
            return;
        default:
            cout << "Opción no válida.\n";
        }
    }
}

void Menu::menuAmistades() {
    unsigned int op;
    do {
        cout << "\n--- ADMINISTRAR AMISTADES ---\n";
        cout << "1.- Enviar solicitud de amistad\n";
        cout << "2.- Aceptar solicitudes de amistad recibidas\n";
        cout << "0.- Volver\n";

        if (!leerNumero("Su opción: ", op)) {
            op = 99;
        }

        switch (op) {
        case 1: enviarSolicitud(); break;
        case 2: aceptarSolicitudes(); break;
        case 0: break;
        default: cout << "Opción inválida.\n";
        }
    } while (op != 0);
}

void Menu::menuMensajes() {
    unsigned int op;
    do {
        cout << "\n--- ADMINISTRAR MENSAJES ---\n";
        cout << "1.- Enviar mensaje\n";
        cout << "2.- Leer nuevos mensajes\n";
        cout << "3.- Buscar mensajes por rango de fecha\n";
        cout << "0.- Volver\n";

        if (!leerNumero("Su opción: ", op)) {
            op = 99;
        }

        switch (op) {
        case 1: enviarMensaje(); break;
        case 2: leerNuevos(); break;
        case 3: buscarPorFecha(); break;
        case 0: break;
        default: cout << "Opción inválida.\n";
        }
    } while (op != 0);
}

void Menu::enviarSolicitud() {
    Medidor::reiniciar();

    string nombre;
    cout << "Nombre de usuario al que desea enviar la solicitud: ";
    getline(cin, nombre);
    // sizeof(string) + los caracteres reales que guarda
    Medidor::sumarMemoria(sizeof(nombre) + nombre.size());

    unsigned char codigo = sistema.enviarSolicitud(nombre);
    mostrarResultadoSolicitud(codigo);

    mostrarMedicion();
}

void Menu::aceptarSolicitudes() {
    Medidor::reiniciar();

    Usuario* activo = sistema.getUsuarioActivo();
    unsigned int opcion = 0;
    Medidor::sumarMemoria(sizeof(activo) + sizeof(opcion));

    while (true) {
        unsigned int total = activo->cantidadSolicitudes();
        if (total == 0) {
            cout << "\nNo tienes solicitudes de amistad pendientes.\n";
            break;
        }

        cout << "\n--- SOLICITUDES RECIBIDAS ---\n";
        for (unsigned int i = 0; i < total; ++i) {
            Medidor::contar();
            cout << (i + 1) << ".- "
                 << sistema.getUsuario(activo->getSolicitud(i))->getNombreUsuario() << "\n";
        }

        if (!leerNumero("Número de la solicitud a aceptar (0 para volver): ", opcion)
            || opcion > total) {
            cout << "Opción inválida.\n";
            continue;
        }
        if (opcion == 0) {
            break;
        }
        sistema.aceptarSolicitud(opcion - 1);
        cout << "Solicitud aceptada.\n";

    }

    mostrarMedicion();
}

void Menu::visitarPerfil() {
    Medidor::reiniciar();

    string nombre;
    cout << "Nombre de usuario del amigo: ";
    getline(cin, nombre);
    Medidor::sumarMemoria(sizeof(nombre) + nombre.size());


    Usuario* amigo = sistema.obtenerAmigo(nombre);
    if (amigo == nullptr) {
        cout << "Ese usuario no existe o no es tu amigo.\n";
        mostrarMedicion();
        return;
    }

    unsigned int total = amigo->cantidadAlbumes();
    unsigned int visibles = 0;
    unsigned int op = 0;
    Medidor::sumarMemoria(sizeof(amigo) + sizeof(total) + sizeof(visibles) + sizeof(op));

    cout << "\n--- ÁLBUMES DE " << amigo->getNombreUsuario() << " ---\n";
    for (unsigned int i = 0; i < total; ++i) {
        Medidor::contar();
        const Album* album = amigo->getAlbum(i);
        if (album->esVisiblePara(false, true)) {
            cout << (i + 1) << ".- " << album->getNombre() << "\n";
            ++visibles;
        }
    }

    if (visibles == 0) {
        cout << "Este amigo no tiene álbumes visibles para ti.\n";
    } else if (!leerNumero("Número de álbum a ver (0 para volver): ", op) || op > total) {
        cout << "Opción inválida.\n";
    } else if (op != 0) {
        const Album* album = amigo->getAlbum(op - 1);
        if (album->esVisiblePara(false, true)) {
            carrusel(*album, amigo->getNombreUsuario());
        } else {
            cout << "Ese álbum no está disponible.\n";
        }
    }

    mostrarMedicion();
}

void Menu::carrusel(const Album& album, const string& nombreDueno) {
    unsigned int n = album.cantidadFotos();
    if (n == 0) {
        cout << "Este álbum no tiene fotos.\n";
        return;
    }

    unsigned int i = 0;
    unsigned int op = 0;
    bool salir = false;
    Medidor::sumarMemoria(sizeof(n) + sizeof(i) + sizeof(op) + sizeof(salir));

    while (!salir) {
        Medidor::contar();
        Foto* foto = album.getFoto(i);

        cout << "\nFoto " << (i + 1) << " de " << n << "\n";
        cout << "Ruta: " << SUB_RUTA_FIJA << nombreDueno << "/albums/"
             << album.getNombre() << "/" << foto->getNombre() << ".png\n";
        cout << "Fecha: " << foto->getFecha() << "  |  Likes: " << foto->getLikes() << "\n";
        cout << "1.- Siguiente\n";
        cout << "2.- Previa\n";
        cout << "3.- Dar like\n";
        cout << "4.- Quitar like\n";
        cout << "0.- Salir del álbum\n";

        if (!leerNumero("Su opción: ", op)) {
            op = 99;
        }

        switch (op) {
        case 1: i = (i + 1) % n; break;
        case 2: i = (i + n - 1) % n; break;
        case 3: foto->darLike(); break;
        case 4: foto->quitarLike(); break;
        case 0: salir = true; break;
        default: cout << "Opción inválida.\n";
        }
    }
}

void Menu::verSugerencias() {
    Medidor::reiniciar();

    unsigned int k = 0;
    while (!leerNumero("¿Cuántas sugerencias desea ver? (1 a 20): ", k) || k < 1 || k > 20) {
        cout << "Valor inválido.\n";
    }

    unsigned short* ids = new unsigned short[k];
    unsigned short* comunes = new unsigned short[k];
    Medidor::sumarMemoria(sizeof(k) + sizeof(ids) + sizeof(comunes)
                          + 2 * k * sizeof(unsigned short));

    unsigned short n = sistema.obtenerSugerencias(static_cast<unsigned short>(k), ids, comunes);

    if (n == 0) {
        cout << "\nNo hay sugerencias de amistad por ahora.\n";
    } else {
        cout << "\n--- SUGERENCIAS DE AMISTAD ---\n";
        for (unsigned short i = 0; i < n; ++i) {
            Medidor::contar();
            cout << (i + 1) << ".- " << sistema.getUsuario(ids[i])->getNombreUsuario()
                 << " (" << comunes[i] << " amigos en común)\n";
        }
        if (n < k) {
            cout << "Solo hay " << n << " sugerencias disponibles.\n";
        }

        unsigned int op = 0;
        if (!leerNumero("Número al que desea enviar solicitud (0 para ninguno): ", op) || op > n) {
            cout << "Opción inválida.\n";
        } else if (op != 0) {
            unsigned char codigo =
                sistema.enviarSolicitud(sistema.getUsuario(ids[op - 1])->getNombreUsuario());
            mostrarResultadoSolicitud(codigo);
        }
    }

    delete[] ids;
    delete[] comunes;

    mostrarMedicion();
}
void Menu::enviarMensaje() {
    Medidor::reiniciar();

    string nombre, texto;
    cout << "Nombre del amigo: ";
    getline(cin, nombre);
    cout << "Mensaje (máximo 512 caracteres): ";
    getline(cin, texto);
    Medidor::sumarMemoria(sizeof(nombre) + sizeof(texto) + nombre.size() + texto.size());

    unsigned char codigo = sistema.enviarMensaje(nombre, texto);
    switch (codigo) {
    case 0: cout << "Mensaje enviado.\n"; break;
    case 1: cout << "Ese usuario no existe o no es tu amigo.\n"; break;
    case 2: cout << "No se puede enviar un mensaje vacío.\n"; break;
    case 3: cout << "El mensaje supera los 512 caracteres.\n"; break;
    default: cout << "No se pudo enviar el mensaje.\n";
    }

    mostrarMedicion(codigo == 0 ? "ctime (fecha de envío)" : "ninguno");
}

void Menu::leerNuevos() {
    Medidor::reiniciar();

    Usuario* activo = sistema.getUsuarioActivo();
    unsigned int total = activo->cantidadRecibidos();
    unsigned int nuevos = 0;
    Medidor::sumarMemoria(sizeof(activo) + sizeof(total) + sizeof(nuevos));

    cout << "\n--- MENSAJES NUEVOS ---\n";
    for (unsigned int i = 0; i < total; ++i) {
        Medidor::contar();
        Mensaje* m = activo->getRecibido(i);
        if (!m->estaLeido()) {
            mostrarMensaje(sistema, m);
            m->marcarLeido();
            ++nuevos;
        }
    }
    if (nuevos == 0) {
        cout << "No tienes mensajes nuevos.\n";
    }

    mostrarMedicion();
}
void Menu::buscarPorFecha() {
    Medidor::reiniciar();

    Usuario* activo = sistema.getUsuarioActivo();
    Fecha inicio;
    Fecha fin;

    while (true) {
        while (!leerFecha("Fecha de inicio", inicio)) {
            cout << "Fecha inválida. Intente de nuevo.\n";
        }
        while (!leerFecha("Fecha de fin", fin)) {
            cout << "Fecha inválida. Intente de nuevo.\n";
        }
        if (!(fin < inicio)) {
            break;
        }
        cout << "La fecha de fin es anterior a la de inicio. Ingréselas de nuevo.\n";
    }

    unsigned int total = activo->cantidadRecibidos();
    unsigned int i = activo->primerIndiceDesde(inicio);
    unsigned int mostrados = 0;
    Medidor::sumarMemoria(sizeof(activo) + sizeof(inicio) + sizeof(fin)
                          + sizeof(total) + sizeof(i) + sizeof(mostrados));

    cout << "\n--- MENSAJES RECIBIDOS EN EL RANGO ---\n";
    while (i < total && !(fin < activo->getRecibido(i)->getFecha())) {
        Medidor::contar();
        mostrarMensaje(sistema, activo->getRecibido(i));
        ++i;
        ++mostrados;
    }
    if (mostrados == 0) {
        cout << "No hay mensajes recibidos en ese rango.\n";
    }

    mostrarMedicion();
}
void Menu::mostrarMedicion(const string& externos) const {
    unsigned long long iteraciones = Medidor::getIteraciones();
    unsigned long long memoria = Medidor::getMemoria()
                                 + sizeof(*this)
                                 + sistema.bytesUsados();
    cout << "\n     Consumo de recursos    \n";
    cout << "Iteraciones: " << iteraciones << "\n";
    cout << "Memoria (bytes): " << memoria << "\n";
    cout << "Componentes externos: " << externos << "\n\n";
}
bool Menu::leerNumero(const string& texto, unsigned int& valor) const {
    cout << texto;
    string linea;
    getline(cin, linea);
    if (linea.empty() || linea.size() > 9) {
        return false;
    }
    unsigned int v = 0;
    for (unsigned int i = 0; i < linea.size(); ++i) {
        if (linea[i] < '0' || linea[i] > '9') {
            return false;
        }
        v = v * 10 + static_cast<unsigned int>(linea[i] - '0');
    }
    valor = v;
    return true;
}
bool Menu::leerFecha(const string& texto, Fecha& resultado) const {
    unsigned int d = 0, m = 0, a = 0;
    if (!leerNumero(texto + " - día: ", d) || d < 1 || d > 31) return false;
    if (!leerNumero(texto + " - mes: ", m) || m < 1 || m > 12) return false;
    if (!leerNumero(texto + " - año: ", a) || a < 1900 || a > 2100) return false;
    resultado = Fecha(static_cast<unsigned char>(d),
                      static_cast<unsigned char>(m),
                      static_cast<unsigned short>(a));
    return true;
}