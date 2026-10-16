#include "Menu.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "UdeABook.h"

using namespace std;

// Muestra en pantalla el resultado de enviar una solicitud de amistad
static void mostrarResultadoSolicitud(unsigned char codigo) {
    switch (codigo) {
    case 0: cout << "[OK] Solicitud de amistad enviada.\n\n"; break;
    case 1: cout << "[!] Ese usuario no existe.\n\n"; break;
    case 2: cout << "[!] No puedes enviarte una solicitud a ti mismo.\n\n"; break;
    case 3: cout << "[!] Ya son amigos.\n\n"; break;
    case 4: cout << "[!] Ya le enviaste una solicitud a esa persona.\n\n"; break;
    case 5: cout << "[!] Esa persona ya te envió una solicitud: acéptala en 'Administrar amistades'.\n\n"; break;
    default: cout << "[X] No se pudo enviar la solicitud.\n\n";
    }
}

// Muestra un mensaje dentro de una cajita con emisor, fecha y texto
static void mostrarMensaje(UdeABook& sistema, const Mensaje* m) {
    cout << "+-- De: " << sistema.getUsuario(m->getEmisor())->getNombreUsuario()
    << "  |  Fecha: " << m->getFecha() << "\n"
    << "|  " << m->getTexto() << "\n"
    << "+----------------------------------\n\n";
}

// Constructor: guarda el sistema e inicializa las constantes y el grupo de fotos
Menu::Menu(UdeABook& sistema)
    : sistema(sistema),
    SUB_RUTA_FIJA("/users/sub-ruta-X-fija/"),
    ultimoGrupo{0, 0, 0},
    cantidadUltimo(0) {}

Menu::~Menu() {
}

// Programa principal: carga datos, pide ingreso y muestra el menú hasta que se salga
void Menu::ejecutar() {
    srand(time(nullptr));

    Medidor::reiniciar();
    if (!sistema.cargarDatos()) {
        cout << "[X] Error crítico: No se pudieron cargar los datos del sistema.\n\n";
        return;
    }
    mostrarMedicion("fstream (lectura de archivos)");

    while (pantallaIngreso()) {
        cantidadUltimo = 0;
        mostrarTresAmigos();
        menuPrincipal();
        sistema.cerrarSesion();
    }
    cout << "+==================================+\n";
    cout << "|   ¡Gracias por usar Ude@Book!    |\n";
    cout << "|           Hasta pronto           |\n";
    cout << "+==================================+\n";
}

// Pantalla de bienvenida: pide usuario y contraseña (false si decide salir)
bool Menu::pantallaIngreso() {
    Medidor::reiniciar();
    string nombre, contrasena;

    while (true) {
        cout << "+==================================+\n";
        cout << "|      BIENVENIDO A Ude@Book       |\n";
        cout << "+==================================+\n";
        cout << "Ingrese nombre de usuario (o 'salir' para terminar): ";
        getline(cin, nombre);
        cout << "\n";

        if (nombre == "salir" || nombre == "SALIR") {
            return false;
        }

        cout << "Ingrese contraseña: ";
        getline(cin, contrasena);
        cout << "\n";

        if (sistema.iniciarSesion(nombre, contrasena)) {
            cout << "[OK] ¡Inicio de sesión exitoso!\n\n";
            Medidor::reiniciar();
            return true;
        } else {
            cout << "[X] Usuario o contraseña incorrectos. Intente nuevamente.\n\n";
        }
    }
}

// Sortea y muestra las fotos de perfil de hasta tres amigos
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

    cout << "+==================================+\n";
    cout << "|    FOTOS DE PERFIL DE AMIGOS     |\n";
    cout << "+----------------------------------+\n";
    if (n == 0) {
        cout << (esActualizacion ? "No hay más amigos disponibles para mostrar.\n" : "Aún no tienes amigos en la plataforma.\n");
    }
    else {
        for (unsigned char i = 0; i < n; ++i) {
            Medidor::contar();

            const Usuario* amigo = sistema.getUsuario(ids[i]);
            cout << "Foto perfil de amigo " << (i + 1) << ": "
                 << SUB_RUTA_FIJA << amigo->getNombreUsuario() << "/albums/profile.png\n";
            ultimoGrupo[i] = ids[i];
        }
        cantidadUltimo = n;
    }
    cout << "+==================================+\n\n";
    mostrarMedicion("rand (sorteo de amigos)");
}

// Menú principal del usuario con sesión iniciada
void Menu::menuPrincipal() {
    bool fotosActualizadas = false;
    string opcion;
    Usuario* yo = sistema.getUsuarioActivo();

    while (true) {
        cout << "+==================================+\n";
        cout << "|         MENÚ DE OPCIONES         |\n";
        cout << "+==================================+\n";
        cout << " Bienvenido a UdeABook " << yo->getNombreUsuario() << "\n";
        cout << "+----------------------------------+\n\n";
        cout << "1.- Administrar amistades\n";
        cout << "2.- Visitar perfil de un amigo\n";
        cout << "3.- Ver sugerencias de amistad\n";
        cout << "4.- Administrar mensajes\n\n";
        cout << "----------------------------------------\n";
        if (!fotosActualizadas && cantidadUltimo > 0) {
            cout << "Ingrese 'N' o 'n' para alternar el grupo de fotos de perfil de amigos.\n";
        }
        cout << "Ingrese 'S' o 's' para cerrar su sesión.\n";
        cout << "----------------------------------------\n";
        cout << "Su opción: ";
        getline(cin, opcion);
        cout << "\n";

        if (opcion.size() != 1) {
            cout << "[!] Opción no válida.\n\n";
            continue;
        }

        switch (opcion[0]) {
        case '1':
            menuAmistades();
            guardarYMedir(true, false, false);
            break;
        case '2':
            visitarPerfil();
            guardarYMedir(false, false, true);
            break;
        case '3':
            verSugerencias();
            guardarYMedir(true, false, false);
            break;
        case '4':
            menuMensajes();
            guardarYMedir(false, true, false);
            break;
        case 'N':
        case 'n':
            if (fotosActualizadas || cantidadUltimo == 0) {
                cout << "[!] Las fotos solo se pueden actualizar una vez.\n\n";
            } else {
                cout << "~ Nuevo grupo de fotos de perfil ~\n";
                mostrarTresAmigos();
                fotosActualizadas = true;
            }
            break;
        case 'S':
        case 's':
            cout << "Cerrando sesión...\n\n";
            return;
        default:
            cout << "[!] Opción no válida.\n\n";
        }
    }
}

// Submenú de amistades (enviar y aceptar solicitudes)
void Menu::menuAmistades() {
    unsigned int op;
    do {
        cout << "+==================================+\n";
        cout << "|      ADMINISTRAR AMISTADES       |\n";
        cout << "+==================================+\n";
        cout << "1.- Enviar solicitud de amistad\n";
        cout << "2.- Aceptar solicitudes de amistad recibidas\n";
        cout << "0.- Volver\n";
        cout << "+----------------------------------+\n";

        if (!leerNumero("Su opción: ", op)) {
            op = 99;
        }

        switch (op) {
        case 1: enviarSolicitud(); break;
        case 2: aceptarSolicitudes(); break;
        case 0: break;
        default: cout << "[!] Opción inválida.\n\n";
        }
    } while (op != 0);
}

// Submenú de mensajes (enviar, leer nuevos y buscar por fecha)
void Menu::menuMensajes() {
    unsigned int op;
    do {
        cout << "+==================================+\n";
        cout << "|       ADMINISTRAR MENSAJES       |\n";
        cout << "+==================================+\n";
        cout << "1.- Enviar mensaje\n";
        cout << "2.- Leer nuevos mensajes\n";
        cout << "3.- Buscar mensajes por rango de fecha\n";
        cout << "0.- Volver\n";
        cout << "+----------------------------------+\n";

        if (!leerNumero("Su opción: ", op)) {
            op = 99;
        }

        switch (op) {
        case 1: enviarMensaje(); break;
        case 2: leerNuevos(); break;
        case 3: buscarPorFecha(); break;
        case 0: break;
        default: cout << "[!] Opción inválida.\n\n";
        }
    } while (op != 0);
}

// Pide un nombre de usuario y le envía una solicitud de amistad
void Menu::enviarSolicitud() {
    Medidor::reiniciar();

    string nombre;
    cout << "+==================================+\n";
    cout << "|        ENVIAR SOLICITUD          |\n";
    cout << "+==================================+\n";
    cout << "Nombre de usuario al que desea enviar la solicitud: ";
    getline(cin, nombre);
    cout << "\n";
    // sizeof(string) + los caracteres reales que guarda
    Medidor::sumarMemoria(sizeof(nombre) + nombre.size());

    unsigned char codigo = sistema.enviarSolicitud(nombre);
    mostrarResultadoSolicitud(codigo);

    mostrarMedicion();
}

// Lista las solicitudes recibidas y deja aceptar una por una
void Menu::aceptarSolicitudes() {
    Medidor::reiniciar();

    Usuario* activo = sistema.getUsuarioActivo();
    unsigned int opcion = 0;
    Medidor::sumarMemoria(sizeof(activo) + sizeof(opcion));

    while (true) {
        unsigned int total = activo->cantidadSolicitudes();
        if (total == 0) {
            cout << "No tienes solicitudes de amistad pendientes.\n\n";
            break;
        }

        cout << "+==================================+\n";
        cout << "|      SOLICITUDES RECIBIDAS       |\n";
        cout << "+==================================+\n";
        for (unsigned int i = 0; i < total; ++i) {
            Medidor::contar();
            cout << (i + 1) << ".- "<< sistema.getUsuario(activo->getSolicitud(i))->getNombreUsuario() << "\n";
        }
        cout << "+----------------------------------+\n";

        if (!leerNumero("Número de la solicitud a aceptar (0 para volver): ", opcion)|| opcion > total) {
            cout << "[!] Opción inválida.\n\n";
            continue;
        }
        if (opcion == 0) {
            break;
        }
        sistema.aceptarSolicitud(opcion - 1);
        cout << "[OK] Solicitud aceptada.\n\n";

    }

    mostrarMedicion();
}

// Muestra los álbumes visibles de un amigo y permite abrir uno
void Menu::visitarPerfil() {
    Medidor::reiniciar();

    string nombre;
    cout << "+==================================+\n";
    cout << "|          VISITAR PERFIL          |\n";
    cout << "+==================================+\n";
    cout << "Nombre de usuario del amigo: ";
    getline(cin, nombre);
    cout << "\n";
    Medidor::sumarMemoria(sizeof(nombre) + nombre.size());


    Usuario* amigo = sistema.obtenerAmigo(nombre);
    if (amigo == nullptr) {
        cout << "[!] Ese usuario no existe o no es tu amigo.\n\n";
        mostrarMedicion();
        return;
    }

    unsigned int total = amigo->cantidadAlbumes();
    unsigned int visibles = 0;
    unsigned int op = 0;
    Medidor::sumarMemoria(sizeof(amigo) + sizeof(total) + sizeof(visibles) + sizeof(op));

    cout << "+==================================+\n";
    cout << " ÁLBUMES DE " << amigo->getNombreUsuario() << "\n";
    cout << "+----------------------------------+\n";
    for (unsigned int i = 0; i < total; ++i) {
        Medidor::contar();
        const Album* album = amigo->getAlbum(i);
        if (album->esVisiblePara(false, true)) {
            cout << (i + 1) << ".- " << album->getNombre() << "\n";
            ++visibles;
        }
    }
    cout << "+----------------------------------+\n";

    if (visibles == 0) {
        cout << "Este amigo no tiene álbumes visibles para ti.\n\n";
    } else if (!leerNumero("Número de álbum a ver (0 para volver): ", op) || op > total) {
        cout << "[!] Opción inválida.\n\n";
    } else if (op != 0) {
        const Album* album = amigo->getAlbum(op - 1);
        if (album->esVisiblePara(false, true)) {
            carrusel(*album, amigo->getNombreUsuario());
        } else {
            cout << "[!] Ese álbum no está disponible.\n\n";
        }
    }

    mostrarMedicion();
}

// Recorre las fotos de un álbum (siguiente, previa, like y quitar like)
void Menu::carrusel(const Album& album, const string& nombreDueno) {
    unsigned int n = album.cantidadFotos();
    if (n == 0) {
        cout << "Este álbum no tiene fotos.\n\n";
        return;
    }

    unsigned int i = 0;
    unsigned int op = 0;
    bool salir = false;
    Medidor::sumarMemoria(sizeof(n) + sizeof(i) + sizeof(op) + sizeof(salir));
    int repitLike=0,repiDislike=0;

    while (!salir) {
        Medidor::contar();
        Foto* foto = album.getFoto(i);

        cout << "+==================================+\n";
        cout << " Foto " << (i + 1) << " de " << n << "  |  Álbum: " << album.getNombre() << "\n";
        cout << "+----------------------------------+\n";
        cout << "Ruta: " << SUB_RUTA_FIJA << nombreDueno << "/albums/"<< album.getNombre() << "/" << foto->getNombre() << ".png\n";
        cout << "Fecha: " << foto->getFecha() << "  |  Likes: " << foto->getLikes() << "\n";
        cout << "+----------------------------------+\n";
        cout << "1.- Siguiente\n";
        cout << "2.- Previa\n";
        cout << "3.- Dar like\n";
        cout << "4.- Quitar like\n";
        cout << "0.- Salir del álbum\n";
        cout << "+----------------------------------+\n";

        if (!leerNumero("Su opción: ", op)) {
            op = 99;
        }

        switch (op) {
        case 1: i = (i + 1) % n; break;
        case 2: i = (i + n - 1) % n; break;
        case 3:
            if (repitLike == 0) {
                foto->darLike();
                sistema.marcarCambioLikes();
                repitLike = 1;
                repiDislike = 0;
            }
            else {
                cout << "Ya le dio like\n\n";
            }
            break;
        case 4:
            if (repiDislike == 0) {
                foto->quitarLike();
                sistema.marcarCambioLikes();
                repiDislike = 1;
                repitLike = 0;
            }
            else {
                cout << "Ya le dio Dislike\n\n";
            }
            break;
        case 0: salir = true; break;
        default: cout << "[!] Opción inválida.\n\n";
        }
    }
}

// Pide cuántas sugerencias ver y permite enviar solicitud a una de ellas
void Menu::verSugerencias() {
    Medidor::reiniciar();

    unsigned int k = 0;
    cout << "+==================================+\n";
    cout << "|      SUGERENCIAS DE AMISTAD      |\n";
    cout << "+==================================+\n";
    while (!leerNumero("¿Cuántas sugerencias desea ver? (1 a 20): ", k) || k < 1 || k > 20) {
        cout << "[!] Valor inválido.\n\n";
    }

    unsigned short* ids = new unsigned short[k];
    unsigned short* comunes = new unsigned short[k];
    Medidor::sumarMemoria(sizeof(k) + sizeof(ids) + sizeof(comunes)
                          + 2 * k * sizeof(unsigned short));

    unsigned short n = sistema.obtenerSugerencias(static_cast<unsigned short>(k), ids, comunes);

    if (n == 0) {
        cout << "No hay sugerencias de amistad por ahora.\n\n";
    } else {
        cout << "+----------------------------------+\n";
        for (unsigned short i = 0; i < n; ++i) {
            Medidor::contar();
            cout << (i + 1) << ".- " << sistema.getUsuario(ids[i])->getNombreUsuario() << " (" << comunes[i] << " amigos en común)\n";
        }
        cout << "+----------------------------------+\n";
        if (n < k) {
            cout << "Solo hay " << n << " sugerencias disponibles.\n";
        }

        unsigned int op = 0;
        if (!leerNumero("Número al que desea enviar solicitud (0 para ninguno): ", op) || op > n) {
            cout << "[!] Opción inválida.\n\n";
        } else if (op != 0) {
            unsigned char codigo = sistema.enviarSolicitud(sistema.getUsuario(ids[op - 1])->getNombreUsuario());
            mostrarResultadoSolicitud(codigo);
        }
    }

    delete[] ids;
    delete[] comunes;

    mostrarMedicion();
}

// Pide el amigo y el texto, y envía el mensaje
void Menu::enviarMensaje() {
    Medidor::reiniciar();

    string nombre, texto;
    cout << "+==================================+\n";
    cout << "|          ENVIAR MENSAJE          |\n";
    cout << "+==================================+\n";
    cout << "Nombre del amigo: ";
    getline(cin, nombre);
    cout << "\n";
    cout << "Mensaje (máximo 512 caracteres): ";
    getline(cin, texto);
    cout << "\n";
    Medidor::sumarMemoria(sizeof(nombre) + sizeof(texto) + nombre.size() + texto.size());

    unsigned char codigo = sistema.enviarMensaje(nombre, texto);
    switch (codigo) {
    case 0: cout << "[OK] Mensaje enviado.\n\n"; break;
    case 1: cout << "[!] Ese usuario no existe o no es tu amigo.\n\n"; break;
    case 2: cout << "[!] No se puede enviar un mensaje vacío.\n\n"; break;
    case 3: cout << "[!] El mensaje supera los 512 caracteres.\n\n"; break;
    default: cout << "[X] No se pudo enviar el mensaje.\n\n";
    }

    mostrarMedicion(codigo == 0 ? "ctime (fecha de envío)" : "ninguno");
}

// Muestra los mensajes no leídos y los marca como leídos
void Menu::leerNuevos() {
    Medidor::reiniciar();

    Usuario* activo = sistema.getUsuarioActivo();
    unsigned int total = activo->cantidadRecibidos();
    unsigned int nuevos = 0;
    Medidor::sumarMemoria(sizeof(activo) + sizeof(total) + sizeof(nuevos));

    cout << "+==================================+\n";
    cout << "|         MENSAJES NUEVOS          |\n";
    cout << "+==================================+\n\n";
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
    } else {
        sistema.marcarCambioMensajes();
    }

    mostrarMedicion();
}

// Pide un rango de fechas y muestra los mensajes recibidos en él
void Menu::buscarPorFecha() {
    Medidor::reiniciar();

    Usuario* activo = sistema.getUsuarioActivo();
    Fecha inicio;
    Fecha fin;

    cout << "+==================================+\n";
    cout << "|    BUSCAR MENSAJES POR FECHA     |\n";
    cout << "+==================================+\n";

    while (true) {
        while (!leerFecha("Fecha de inicio", inicio)) {
            cout << "[!] Fecha inválida. Intente de nuevo.\n\n";
        }
        while (!leerFecha("Fecha de fin", fin)) {
            cout << "[!] Fecha inválida. Intente de nuevo.\n\n";
        }
        if (!(fin < inicio)) {
            break;
        }
        cout << "[!] La fecha de fin es anterior a la de inicio. Ingréselas de nuevo.\n\n";
    }

    unsigned int total = activo->cantidadRecibidos();
    unsigned int i = activo->primerIndiceDesde(inicio);
    unsigned int mostrados = 0;
    Medidor::sumarMemoria(sizeof(activo) + sizeof(inicio) + sizeof(fin)+ sizeof(total) + sizeof(i) + sizeof(mostrados));

    cout << "+==================================+\n";
    cout << "|  MENSAJES RECIBIDOS EN EL RANGO  |\n";
    cout << "+==================================+\n\n";
    while (i < total && !(fin < activo->getRecibido(i)->getFecha())) {
        Medidor::contar();
        mostrarMensaje(sistema, activo->getRecibido(i));
        ++i;
        ++mostrados;
    }
    if (mostrados == 0) {
        cout << "No hay mensajes recibidos en ese rango.\n\n";
    }

    mostrarMedicion();
}

// Muestra las iteraciones, la memoria usada y los componentes externos
void Menu::mostrarMedicion(const string& externos) const {
    unsigned long long iteraciones = Medidor::getIteraciones();
    unsigned long long memoria = Medidor::getMemoria() + sizeof(*this)+ sistema.bytesUsados();
    cout << "+==================================+\n";
    cout << "|       CONSUMO DE RECURSOS        |\n";
    cout << "+----------------------------------+\n";
    cout << " Iteraciones          : " << iteraciones << "\n";
    cout << " Memoria (bytes)      : " << memoria << "\n";
    cout << " Componentes externos : " << externos << "\n";
    cout << "+==================================+\n\n";
}

// Lee un número entero positivo (devuelve false si lo escrito no es válido)
bool Menu::leerNumero(const string& texto, unsigned int& valor) const {
    cout << texto;
    string linea;
    getline(cin, linea);
    cout << "\n";
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

// Lee día, mes y año, y arma la fecha (devuelve false si algo es inválido)
bool Menu::leerFecha(const string& texto, Fecha& resultado) const {
    unsigned int d = 0, m = 0, a = 0;
    if (!leerNumero(texto + " - día: ", d) || d < 1 || d > 31) return false;
    if (!leerNumero(texto + " - mes: ", m) || m < 1 || m > 12) return false;
    if (!leerNumero(texto + " - año: ", a) || a < 1900 || a > 2100) return false;
    resultado = Fecha(static_cast<unsigned char>(d),static_cast<unsigned char>(m),static_cast<unsigned short>(a));
    return true;
}

void Menu::guardarYMedir(bool amistades, bool mensajes, bool likes) {
    Medidor::reiniciar();
    sistema.guardarDatos(amistades, mensajes, likes);
    mostrarMedicion("fstream (escritura de archivos)");
}
