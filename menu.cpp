#include "Menu.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "UdeABook.h"

using namespace std;

// ---------------------------------------------------------------
// Ayudas visuales: solo dibujan en pantalla, no tocan la logica
// ---------------------------------------------------------------
static const unsigned int ANCHO = 54;   // ancho interior de las cajas y reglas

// Cuenta caracteres visibles (una tilde ocupa 2 bytes pero 1 espacio en pantalla)
static unsigned int largoVisible(const string& texto) {
    unsigned int cuenta = 0;
    for (unsigned int i = 0; i < texto.size(); ++i) {
        if ((static_cast<unsigned char>(texto[i]) & 0xC0) != 0x80) {
            ++cuenta;
        }
    }
    return cuenta;
}

// Escribe un mismo caracter varias veces
static void repetir(char c, unsigned int cantidad) {
    for (unsigned int i = 0; i < cantidad; ++i) {
        cout << c;
    }
}

// Linea completa del mismo caracter
static void regla(char c) {
    repetir(c, ANCHO + 2);
    cout << "\n";
}

// Titulo de seccion:  == TITULO ====================
static void titulo(const string& texto) {
    cout << "\n== " << texto << " ";
    unsigned int usados = largoVisible(texto) + 4;
    if (usados < ANCHO + 2) {
        repetir('=', ANCHO + 2 - usados);
    }
    cout << "\n";
}

// Borde horizontal de una caja:  +-------------------+
static void bordeCaja() {
    cout << '+';
    repetir('-', ANCHO);
    cout << "+\n";
}

// Fila de caja con el texto centrado:  |      TEXTO      |
static void filaCentrada(const string& texto) {
    unsigned int largo = largoVisible(texto);
    unsigned int izquierda = 0;
    unsigned int derecha = 0;
    if (largo < ANCHO) {
        izquierda = (ANCHO - largo) / 2;
        derecha = ANCHO - largo - izquierda;
    }
    cout << '|';
    repetir(' ', izquierda);
    cout << texto;
    repetir(' ', derecha);
    cout << "|\n";
}

// Fila de caja con etiqueta y valor:  | Etiqueta        : valor      |
static void filaDato(const string& etiqueta, const string& valor) {
    cout << "| " << etiqueta;
    repetir(' ', 21 - etiqueta.size());
    cout << ": " << valor;
    unsigned int usados = 24 + largoVisible(valor);
    if (usados < ANCHO) {
        repetir(' ', ANCHO - usados);
    }
    cout << "|\n";
}

static void mostrarResultadoSolicitud(unsigned char codigo) {
    switch (codigo) {
    case 0: cout << "\n[OK] Solicitud de amistad enviada.\n"; break;
    case 1: cout << "\n[!] Ese usuario no existe.\n"; break;
    case 2: cout << "\n[!] No puedes enviarte una solicitud a ti mismo.\n"; break;
    case 3: cout << "\n[!] Ya son amigos.\n"; break;
    case 4: cout << "\n[!] Ya le enviaste una solicitud a esa persona.\n"; break;
    case 5: cout << "\n[!] Esa persona ya te envió una solicitud: acéptala en 'Administrar amistades'.\n"; break;
    default: cout << "\n[!] No se pudo enviar la solicitud.\n";
    }
}

static void mostrarMensaje(UdeABook& sistema, const Mensaje* m) {
    cout << "\nDe: " << sistema.getUsuario(m->getEmisor())->getNombreUsuario()
    << "  |  Fecha: " << m->getFecha() << "\n"
    << "  " << m->getTexto() << "\n";
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
        cout << "\n[!] Error crítico: No se pudieron cargar los datos del sistema.\n";
        return;
    }
    mostrarMedicion("fstream (lectura de archivos)");

    while (pantallaIngreso()) {
        cantidadUltimo = 0;
        mostrarTresAmigos();
        menuPrincipal();
        sistema.cerrarSesion();
    }
    cout << "\n";
    bordeCaja();
    filaCentrada("¡Gracias por usar Ude@Book! Hasta pronto.");
    bordeCaja();
}

bool Menu::pantallaIngreso() {
    Medidor::reiniciar();
    string nombre, contrasena;

    while (true) {
        cout << "\n";
        bordeCaja();
        filaCentrada("BIENVENIDO A Ude@Book");
        bordeCaja();
        cout << "Ingrese nombre de usuario (o 'salir' para terminar): ";
        getline(cin, nombre);

        if (nombre == PALABRA_SALIR || nombre == "SALIR") {
            return false;
        }

        cout << "Ingrese contraseña: ";
        getline(cin, contrasena);

        if (sistema.iniciarSesion(nombre, contrasena)) {
            cout << "\n[OK] ¡Inicio de sesión exitoso!\n";
            Medidor::reiniciar();
            return true;
        } else {
            cout << "\n[!] Usuario o contraseña incorrectos. Intente nuevamente.\n";
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

    titulo(esActualizacion ? "NUEVO GRUPO DE FOTOS DE PERFIL" : "FOTOS DE PERFIL DE TUS AMIGOS");
    if (n == 0) {
        cout << (esActualizacion ? "No hay más amigos disponibles para mostrar.\n"
                                 : "Aún no tienes amigos en la plataforma.\n");
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
    regla('-');
    mostrarMedicion("rand (sorteo de amigos)");
}

void Menu::menuPrincipal() {
    bool fotosActualizadas = false;
    string opcion;
    Usuario* yo = sistema.getUsuarioActivo();

    while (true) {
        cout << "\n";
        bordeCaja();
        filaCentrada("Bienvenido a UdeABook " + yo->getNombreUsuario());
        bordeCaja();
        cout << "\nMenú de opciones:\n\n";
        cout << "1.- Administrar amistades\n";
        cout << "2.- Visitar perfil de un amigo\n";
        cout << "3.- Ver sugerencias de amistad\n";
        cout << "4.- Administrar mensajes\n";
        regla('-');
        if (!fotosActualizadas && cantidadUltimo > 0) {
            cout << "Ingrese 'N' o 'n' para alternar el grupo de fotos de perfil de amigos.\n";
        }
        cout << "Ingrese 'S' o 's' para cerrar su sesión.\n";
        regla('-');
        cout << "Su opción: ";
        getline(cin, opcion);

        if (opcion.size() != 1) {
            cout << "\n[!] Opción no válida.\n";
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
                cout << "\n[!] Las fotos solo se pueden actualizar una vez.\n";
            } else {
                mostrarTresAmigos();
                fotosActualizadas = true;
            }
            break;
        case 'S':
        case 's':
            cout << "\nCerrando sesión...\n";
            return;
        default:
            cout << "\n[!] Opción no válida.\n";
        }
    }
}

void Menu::menuAmistades() {
    unsigned int op;
    do {
        titulo("ADMINISTRAR AMISTADES");
        cout << "1.- Enviar solicitud de amistad\n";
        cout << "2.- Aceptar solicitudes de amistad recibidas\n";
        cout << "0.- Volver\n";

        if (!leerNumero("\nSu opción: ", op)) {
            op = 99;
        }

        switch (op) {
        case 1: enviarSolicitud(); break;
        case 2: aceptarSolicitudes(); break;
        case 0: break;
        default: cout << "\n[!] Opción inválida.\n";
        }
    } while (op != 0);
}

void Menu::menuMensajes() {
    unsigned int op;
    do {
        titulo("ADMINISTRAR MENSAJES");
        cout << "1.- Enviar mensaje\n";
        cout << "2.- Leer nuevos mensajes\n";
        cout << "3.- Buscar mensajes por rango de fecha\n";
        cout << "0.- Volver\n";

        if (!leerNumero("\nSu opción: ", op)) {
            op = 99;
        }

        switch (op) {
        case 1: enviarMensaje(); break;
        case 2: leerNuevos(); break;
        case 3: buscarPorFecha(); break;
        case 0: break;
        default: cout << "\n[!] Opción inválida.\n";
        }
    } while (op != 0);
}

void Menu::enviarSolicitud() {
    Medidor::reiniciar();

    string nombre;
    titulo("ENVIAR SOLICITUD DE AMISTAD");
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

        titulo("SOLICITUDES RECIBIDAS");
        for (unsigned int i = 0; i < total; ++i) {
            Medidor::contar();
            cout << (i + 1) << ".- "
                 << sistema.getUsuario(activo->getSolicitud(i))->getNombreUsuario() << "\n";
        }

        if (!leerNumero("\nNúmero de la solicitud a aceptar (0 para volver): ", opcion)
            || opcion > total) {
            cout << "\n[!] Opción inválida.\n";
            continue;
        }
        if (opcion == 0) {
            break;
        }
        sistema.aceptarSolicitud(opcion - 1);
        cout << "\n[OK] Solicitud aceptada.\n";

    }

    mostrarMedicion();
}

void Menu::visitarPerfil() {
    Medidor::reiniciar();

    string nombre;
    titulo("VISITAR PERFIL DE UN AMIGO");
    cout << "Nombre de usuario del amigo: ";
    getline(cin, nombre);
    Medidor::sumarMemoria(sizeof(nombre) + nombre.size());


    Usuario* amigo = sistema.obtenerAmigo(nombre);
    if (amigo == nullptr) {
        cout << "\n[!] Ese usuario no existe o no es tu amigo.\n";
        mostrarMedicion();
        return;
    }

    unsigned int total = amigo->cantidadAlbumes();
    unsigned int visibles = 0;
    unsigned int op = 0;
    Medidor::sumarMemoria(sizeof(amigo) + sizeof(total) + sizeof(visibles) + sizeof(op));

    titulo("ÁLBUMES DE " + amigo->getNombreUsuario());
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
    } else if (!leerNumero("\nNúmero de álbum a ver (0 para volver): ", op) || op > total) {
        cout << "\n[!] Opción inválida.\n";
    } else if (op != 0) {
        const Album* album = amigo->getAlbum(op - 1);
        if (album->esVisiblePara(false, true)) {
            carrusel(*album, amigo->getNombreUsuario());
        } else {
            cout << "\n[!] Ese álbum no está disponible.\n";
        }
    }

    mostrarMedicion();
}

void Menu::carrusel(const Album& album, const string& nombreDueno) {
    unsigned int n = album.cantidadFotos();
    if (n == 0) {
        cout << "\nEste álbum no tiene fotos.\n";
        return;
    }

    unsigned int i = 0;
    unsigned int op = 0;
    bool salir = false;
    Medidor::sumarMemoria(sizeof(n) + sizeof(i) + sizeof(op) + sizeof(salir));

    while (!salir) {
        Medidor::contar();
        Foto* foto = album.getFoto(i);

        titulo("FOTO " + to_string(i + 1) + " DE " + to_string(n));
        cout << "Ruta: " << SUB_RUTA_FIJA << nombreDueno << "/albums/"
             << album.getNombre() << "/" << foto->getNombre() << ".png\n";
        cout << "Fecha: " << foto->getFecha() << "  |  Likes: " << foto->getLikes() << "\n";
        regla('-');
        cout << "1.- Siguiente\n";
        cout << "2.- Previa\n";
        cout << "3.- Dar like\n";
        cout << "4.- Quitar like\n";
        cout << "0.- Salir del álbum\n";

        if (!leerNumero("\nSu opción: ", op)) {
            op = 99;
        }

        switch (op) {
        case 1: i = (i + 1) % n; break;
        case 2: i = (i + n - 1) % n; break;
        case 3: foto->darLike(); break;
        case 4: foto->quitarLike(); break;
        case 0: salir = true; break;
        default: cout << "\n[!] Opción inválida.\n";
        }
    }
}

void Menu::verSugerencias() {
    Medidor::reiniciar();

    titulo("SUGERENCIAS DE AMISTAD");
    unsigned int k = 0;
    while (!leerNumero("¿Cuántas sugerencias desea ver? (1 a 20): ", k) || k < 1 || k > 20) {
        cout << "\n[!] Valor inválido.\n";
    }

    unsigned short* ids = new unsigned short[k];
    unsigned short* comunes = new unsigned short[k];
    Medidor::sumarMemoria(sizeof(k) + sizeof(ids) + sizeof(comunes)
                          + 2 * k * sizeof(unsigned short));

    unsigned short n = sistema.obtenerSugerencias(static_cast<unsigned short>(k), ids, comunes);

    if (n == 0) {
        cout << "\nNo hay sugerencias de amistad por ahora.\n";
    } else {
        titulo("TUS SUGERENCIAS");
        for (unsigned short i = 0; i < n; ++i) {
            Medidor::contar();
            cout << (i + 1) << ".- " << sistema.getUsuario(ids[i])->getNombreUsuario()
                 << " (" << comunes[i] << " amigos en común)\n";
        }
        if (n < k) {
            cout << "Solo hay " << n << " sugerencias disponibles.\n";
        }

        unsigned int op = 0;
        if (!leerNumero("\nNúmero al que desea enviar solicitud (0 para ninguno): ", op) || op > n) {
            cout << "\n[!] Opción inválida.\n";
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
    titulo("ENVIAR MENSAJE");
    cout << "Nombre del amigo: ";
    getline(cin, nombre);
    cout << "Mensaje (máximo 512 caracteres): ";
    getline(cin, texto);
    Medidor::sumarMemoria(sizeof(nombre) + sizeof(texto) + nombre.size() + texto.size());

    unsigned char codigo = sistema.enviarMensaje(nombre, texto);
    switch (codigo) {
    case 0: cout << "\n[OK] Mensaje enviado.\n"; break;
    case 1: cout << "\n[!] Ese usuario no existe o no es tu amigo.\n"; break;
    case 2: cout << "\n[!] No se puede enviar un mensaje vacío.\n"; break;
    case 3: cout << "\n[!] El mensaje supera los 512 caracteres.\n"; break;
    default: cout << "\n[!] No se pudo enviar el mensaje.\n";
    }

    mostrarMedicion(codigo == 0 ? "ctime (fecha de envío)" : "ninguno");
}

void Menu::leerNuevos() {
    Medidor::reiniciar();

    Usuario* activo = sistema.getUsuarioActivo();
    unsigned int total = activo->cantidadRecibidos();
    unsigned int nuevos = 0;
    Medidor::sumarMemoria(sizeof(activo) + sizeof(total) + sizeof(nuevos));

    titulo("MENSAJES NUEVOS");
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

    titulo("BUSCAR MENSAJES POR RANGO DE FECHA");
    while (true) {
        while (!leerFecha("Fecha de inicio", inicio)) {
            cout << "\n[!] Fecha inválida. Intente de nuevo.\n";
        }
        while (!leerFecha("Fecha de fin", fin)) {
            cout << "\n[!] Fecha inválida. Intente de nuevo.\n";
        }
        if (!(fin < inicio)) {
            break;
        }
        cout << "\n[!] La fecha de fin es anterior a la de inicio. Ingréselas de nuevo.\n";
    }

    unsigned int total = activo->cantidadRecibidos();
    unsigned int i = activo->primerIndiceDesde(inicio);
    unsigned int mostrados = 0;
    Medidor::sumarMemoria(sizeof(activo) + sizeof(inicio) + sizeof(fin)
                          + sizeof(total) + sizeof(i) + sizeof(mostrados));

    titulo("MENSAJES RECIBIDOS EN EL RANGO");
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
    cout << "\n";
    bordeCaja();
    filaCentrada("CONSUMO DE RECURSOS");
    bordeCaja();
    filaDato("Iteraciones", to_string(iteraciones));
    filaDato("Memoria (bytes)", to_string(memoria));
    filaDato("Componentes externos", externos);
    bordeCaja();
    cout << "\n";
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