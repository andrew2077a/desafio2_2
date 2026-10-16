#include "UdeABook.h"
#include<sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

// Constructor: empieza sin sesión activa (-1) y con las listas vacías
UdeABook::UdeABook()
    : usuarios(), amistades(), usuarioActivo(-1),
    cambiosAmistades(false), cambiosMensajes(false), cambiosLikes(false) {}

// Destructor: borra cada Usuario (cada uno libera sus álbumes, fotos y mensajes recibidos)
UdeABook::~UdeABook() {
    for (unsigned int i = 0; i < usuarios.getTamano(); i++) {
        Medidor::contar(); // se cuenta cada vuelta
        if (usuarios[i] != nullptr) {
            delete usuarios[i];
        }
    }

}

// Carga todo en orden obligatorio: usuarios, matriz de amistades, amistades/solicitudes, mensajes y álbumes.
// Devuelve false si algo falla
bool UdeABook::cargarDatos(){
    try{
        if(!cargarUsuarios()){
            throw string ("Algo salio mal cargando usuarios");

        }
        amistades.reiniciar(usuarios.getTamano());
        if(!cargarAmistades()||!cargarMensajes()||!cargarAlbumes()){
            throw string ("Algo salio mal cargando los datos");

        }
    }
    catch(const string& error){
        cout<<"Error dectectado: "<<error<<endl;
        return false;
    }
    return true;
}
// Lee los usuarios: cuenta las líneas (N), reserva la lista exacta y pone a cada usuario en la posición de su id
bool UdeABook::cargarUsuarios() {
    try {
        string Archivo_Usuarios = "C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\desafio2_2\\usuarios";
        ifstream archivo(Archivo_Usuarios);
        if (!archivo.is_open()) {
            throw string("no se abrio el archivo de usuarios");
        }

        string linea;
        unsigned int N = 0;

        // Apunte: primera pasada solo cuenta usuarios (ignora lineas vacias o con solo '\r'); N = capacidad exacta
        while (getline(archivo, linea)) {
            if (!linea.empty() && linea != "\r") {
                N++;
            }
            Medidor::contar();
        }

        archivo.close();
        usuarios = Lista<Usuario*>(N);
        for (unsigned int i = 0; i < N; i++) {
            usuarios += nullptr;
            Medidor::contar();
        }

        archivo.open(Archivo_Usuarios);
        if (!archivo.is_open()) {
            throw string("no se abrio el archivo de usuarios");
        }

        while (getline(archivo, linea)) {
            Medidor::contar();
            if (linea.empty()) {
                continue;
            }

            istringstream flujo(linea);
            string campoId, nombreUsuario, contrasena, correo, ciudad, pais, fechaNac, fechaInsc;

            getline(flujo, campoId, ';');
            getline(flujo, nombreUsuario, ';');
            getline(flujo, contrasena, ';');
            getline(flujo, correo, ';');
            getline(flujo, ciudad, ';');
            getline(flujo, pais, ';');
            getline(flujo, fechaNac, ';');

            // Apunte: el ultimo campo se lee hasta '\r' (asi no se usa pop_back, que esta prohibido)
            getline(flujo, fechaInsc, '\r');

            unsigned int id = stoi(campoId);
            if (id >= N || usuarios[id] != nullptr) {
                throw string("archivo mal hecho, usuario repetido");
            }

            usuarios[id] = new Usuario((unsigned short)id, nombreUsuario, contrasena, correo,
                                       ciudad, pais, textoAFecha(fechaNac), textoAFecha(fechaInsc));
        }
    }
    catch (const string& error) {
        cout << "Error: " << error << endl;
        return false;
    }
    return true;
}
// Lee las amistades: por cada línea, los amigos van a la matriz de bits y las solicitudes a la lista del usuario
bool UdeABook::cargarAmistades() {
    try {
        string Archivo_Amistades = "C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\desafio2_2\\amistades";
        ifstream archivo(Archivo_Amistades);
        if (!archivo.is_open()) {
            throw string("no se abrio el archivo de amistades");
        }

        unsigned int N = usuarios.getTamano();
        string linea;
        string idUsuario, idsAmigos, idsSolicitudes, token;
        stringstream flujo, flujoAmigos, flujoSolicitudes;

        while (getline(archivo, linea)) {
            Medidor::contar();

            if (linea.empty()) {
                continue;
            }

            flujo.clear();
            flujo.str(linea);

            getline(flujo, idUsuario, ';');
            unsigned int id = stoi(idUsuario);
            if (id >= N) {
                continue;
            }

            getline(flujo, idsAmigos, ';');

            // Apunte: ultimo campo (solicitudes) hasta '\r', sin pop_back
            getline(flujo, idsSolicitudes, '\r');

            flujoAmigos.clear();
            flujoAmigos.str(idsAmigos);
            while (getline(flujoAmigos, token, ',')) {
                if (!token.empty()) {
                    unsigned int amigo = stoi(token);
                    if (amigo < N) {
                        amistades.crearAmistad((unsigned short)id, (unsigned short)amigo);
                    }
                }
                Medidor::contar();
            }

            flujoSolicitudes.clear();
            flujoSolicitudes.str(idsSolicitudes);
            while (getline(flujoSolicitudes, token, ',')) {
                if (!token.empty()) {
                    unsigned int solicitante = stoi(token);
                    if (solicitante < N) {
                        usuarios[id]->agregarSolicitud((unsigned short)solicitante);
                    }
                }
                Medidor::contar();
            }
        }
    }
    catch (string& error) {
        cout << "Error: " << error << endl;
        return false;
    }
    return true;
}
// Lee los mensajes: crea cada uno una sola vez; el receptor lo posee y el emisor solo lo apunta
bool UdeABook::cargarMensajes() {
    try {
        string Archivo_Mensajes = "C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\desafio2_2\\mensajes";
        ifstream archivo(Archivo_Mensajes);
        string linea;
        if (!archivo.is_open()) {
            throw string("no se abrio el archivo de mensajes");
        }

        unsigned int N = usuarios.getTamano();

        while (getline(archivo, linea)) {
            Medidor::contar();

            if (linea.empty()) {
                continue;
            }

            string emisor, receptor, fecha, leido, texto;
            istringstream flujo(linea);

            getline(flujo, emisor, ';');
            getline(flujo, receptor, ';');
            getline(flujo, fecha, ';');
            getline(flujo, leido, ';');

            // Apunte: el texto es el resto de la linea hasta '\r' (puede traer ';' adentro)
            getline(flujo, texto, '\r');

            if (emisor == "" || receptor == "") {
                continue;
            }

            unsigned int idEmisor = stoi(emisor);
            unsigned int idReceptor = stoi(receptor);

            if (idEmisor >= N || idReceptor >= N) {
                continue;
            }

            Mensaje* m = new Mensaje((unsigned short)idEmisor, (unsigned short)idReceptor, texto, textoAFecha(fecha));
            if (leido == "1") {
                m->marcarLeido();
            }

            usuarios[idReceptor]->recibirMensaje(m);
            usuarios[idEmisor]->registrarEnviado(m);
        }
    }
    catch (string& error) {
        cout << "Error: " << error << endl;
        return false;
    }
    return true;
}
// Lee los álbumes: una línea de álbum seguida de exactamente cantidadFotos líneas de fotos
bool UdeABook::cargarAlbumes() {
    try {
        string Archivo_Album = "C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\desafio2_2\\albumes";
        ifstream archivo(Archivo_Album);
        string linea;

        if (!archivo.is_open()) {
            throw string("no se abrio el archivo de albumes");
        }
        unsigned int N = usuarios.getTamano();
        string idUsuario, nombreAlbum, privacidad, cantidadFotos, nombreFoto, fecha, tamano, likes;
        istringstream flujo;
        while (getline(archivo, linea)) {
            Medidor::contar();

            if (linea.empty()) {
                continue;
            }

            flujo.clear();
            flujo.str(linea);
            getline(flujo, idUsuario, ';');
            getline(flujo, nombreAlbum, ';');
            getline(flujo, privacidad, ';');
            // Apunte: cantidadFotos es el ultimo campo de la linea del album, se lee hasta '\r'
            getline(flujo, cantidadFotos, '\r');
            unsigned int canti = stoi(cantidadFotos);
            Album* albu = new Album(nombreAlbum, (unsigned char)stoi(privacidad), canti);
            for (unsigned int i = 0; i < canti; i++) {
                if (!getline(archivo, linea)) {
                    delete albu;
                    throw string("el archivo de albumes termino antes de tiempo");
                }
                Medidor::contar();
                flujo.clear();
                flujo.str(linea);
                getline(flujo, nombreFoto, ';');
                getline(flujo, fecha, ';');
                getline(flujo, tamano, ';');

                // Apunte: likes es el ultimo campo de la linea de la foto, se lee hasta '\r'
                getline(flujo, likes, '\r');

                albu->agregarFoto(new Foto(nombreFoto, textoAFecha(fecha), stoi(tamano), stoi(likes)));
            }

            unsigned int id = stoi(idUsuario);
            if (id < N) {
                usuarios[id]->agregarAlbum(albu);
            } else {
                delete albu;
            }
        }
    }
    catch (string& error) {
        cout << "Error: " << error << endl;
        return false;
    }
    return true;
}

// Convierte un texto dd/mm/aaaa en una Fecha (se usa al cargar usuarios, mensajes y álbumes)
Fecha UdeABook::textoAFecha(const string& texto) {
    stringstream flujo(texto);
    string dia, mes, anio;

    getline(flujo, dia, '/');
    getline(flujo, mes, '/');
    getline(flujo, anio);

    return Fecha((unsigned char)stoi(dia), (unsigned char)stoi(mes), (unsigned short)stoi(anio));
}
// Guarda solo lo que se pida (amistades, mensajes y/o likes); por defecto guarda todo
bool UdeABook::guardarDatos(bool conAmistades, bool conMensajes, bool conLikes){
    if(conAmistades && cambiosAmistades){
        if(!guardarAmistades()){
            return false;
        }
        cambiosAmistades = false;
    }
    if(conMensajes && cambiosMensajes){
        if(!guardarMensajes()){
            return false;
        }
        cambiosMensajes = false;
    }
    if(conLikes && cambiosLikes){
        if(!guardarLikes()){
            return false;
        }
        cambiosLikes = false;
    }
    return true;
}
// Reescribe el archivo de amistades completo: id;amigos;solicitudes de cada usuario
bool UdeABook::guardarAmistades(){
    try{
        string Archivo_Amistades="C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\desafio2_2\\amistades";
        ofstream archivo(Archivo_Amistades);
        if(!archivo.is_open()){
            throw string ("no se abrio el archivo de amistades para escribir");
        }
        unsigned int N = usuarios.getTamano();

        unsigned short* amigos = new unsigned short[N];

        Medidor::sumarMemoria(N * sizeof(unsigned short));

        for(unsigned int i=0;i<N;i++){

            Medidor::contar();
            unsigned short c = amistades.listarAmigos((unsigned short)i, amigos);

            archivo << i << ';';

            for(unsigned short j=0;j<c;j++){
                // Antes de cada amigo, menos el primero, escribe una coma
                if(j>0){
                    archivo << ',';
                }
                // Escribe el id del amigo
                archivo << amigos[j];
            }
            // Cierra el campo de amigos con el segundo separador (si no hay amigos, el campo queda vacío)
            archivo << ';';
            // Recorre las solicitudes que tiene el usuario i
            for(unsigned int j=0;j<usuarios[i]->cantidadSolicitudes();j++){
                // Antes de cada solicitud, menos la primera, escribe una coma
                if(j>0){
                    archivo << ',';
                }
                // Escribe el id de quien envió la solicitud
                archivo << usuarios[i]->getSolicitud(j);
            }
            archivo << '\n';
        }
        // Libera el arreglo local
        delete[] amigos;
        // Cierra el archivo para asegurar que todo quedó escrito
        archivo.close();
    }
    // Atrapa los errores lanzados con throw string
    catch(string& error){
        cout<<"Error: "<<error<<endl;
        return false;
    }
    // Todo salió bien
    return true;
}
// Reescribe el archivo de mensajes completo (solo los recibidos, para no repetirlos)
bool UdeABook::guardarMensajes(){
    string Archivo_Mensajes="C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\desafio2_2\\mensajes";
    ofstream archivo(Archivo_Mensajes);
    try{
        // Apunte: si el archivo NO abre, se lanza el error
        if (!archivo.is_open()){
            throw string("no se abrio el archivo de mensajes para escribir");
        }
        unsigned int N= usuarios.getTamano();
        for(unsigned int i=0;i<N;i++){
            // Apunte: solo se recorren los recibidos, asi cada mensaje se guarda una sola vez
            unsigned n=usuarios[i]->cantidadRecibidos();
            for(unsigned int e=0;e<n;e++){
                Medidor::contar(); // una vuelta por mensaje
                // Apunte: el mensaje ya existe (no se crea con new), se pide el e-esimo recibido del usuario i
                Mensaje* m= usuarios[i]->getRecibido(e);
                archivo<<m->getEmisor()<<';';
                archivo<<m->getReceptor()<<';';
                archivo<<m->getFecha()<<';';
                archivo<<m->estaLeido()<<';';
                archivo<<m->getTexto()<<'\n';
            }
        }
        archivo.close(); // se cierra para asegurar que todo quedo escrito
    }
    catch (string& error){
        cout<<"Error: "<<error<<endl;
        return false;
    }
    return true;

}
// Reescribe el archivo de álbumes completo, con los likes actualizados
bool UdeABook:: guardarLikes(){
    string Archivo_Album="C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\desafio2_2\\albumes";
    ofstream archivo(Archivo_Album);
    try{
        if (!archivo.is_open()){
            throw string("no se abrio el archivo de albumes para escribir");
        }
        unsigned int N= usuarios.getTamano();
        for(unsigned int i=0;i<N;i++){
            unsigned int n=usuarios[i]->cantidadAlbumes();
            for(unsigned int e=0;e<n;e++){
                Medidor::contar();
                Album* abu=usuarios[i]->getAlbum(e);
                unsigned int ne=abu->cantidadFotos();
                archivo<<i<<';';
                archivo<<abu->getNombre()<<';';
                archivo<<static_cast<int>(abu->getPrivacidad())<<';';
                archivo<<ne<<'\n';
                for(unsigned int u=0;u<ne;u++){
                    Medidor::contar(); // tambien una vuelta por cada linea de foto
                    Foto* fot=abu->getFoto(u);
                    archivo<<fot->getNombre()<<';';
                    archivo<<fot->getFecha()<<';';
                    archivo<<fot->getTamano()<<';';
                    archivo<<fot->getLikes()<<'\n';
                }
            }
        }
        archivo.close();

    }
    catch(string& error){
        cout<<"Error: "<<error<<endl;
        return false;
    }
    return true;

}
// Valida nombre y contraseña; si coinciden deja ese usuario como activo
// (no distingue cuál de los dos falló)
bool UdeABook::iniciarSesion(const string& nombre, const string& contrasena) {
    int indice = buscarUsuario(nombre);

    if (indice == -1) {
        return false;
    }

    if (usuarios[indice]->verificarContrasena(contrasena)) {
        usuarioActivo = indice;
        return true;
    }

    return false;
}
// Cierra la sesión: -1 significa que no hay usuario activo
void UdeABook::cerrarSesion(){
    usuarioActivo=-1;
}

// Devuelve el usuario con sesión iniciada, o nullptr si no hay ninguno
Usuario* UdeABook:: getUsuarioActivo(){
    if(usuarioActivo==-1){
        return nullptr;
    }
    return usuarios[usuarioActivo];
}
// Devuelve el usuario por id (el id es su posición en la lista, acceso directo); nullptr si no existe
Usuario* UdeABook:: getUsuario( unsigned short id ){
    if(id>=usuarios.getTamano()){
        return nullptr;
    }
    return usuarios[id];
}
// Busca por nombre recorriendo la lista; devuelve su posición o -1 si no existe
short UdeABook::buscarUsuario(const string& nombre) {
    for (unsigned int i = 0; i < usuarios.getTamano(); i++) {
        Medidor::contar();
        if (usuarios[i]->getNombreUsuario() == nombre) {
            return static_cast<short>(i);
        }
    }
    return -1;
}
// Devuelve al usuario solo si existe y es amigo del activo; si no, nullptr
Usuario* UdeABook:: obtenerAmigo(const string& nombre){
    short indi=buscarUsuario(nombre);
    if(indi==-1){
        return nullptr;
    }
    if(!amistades.sonAmigos(usuarioActivo,indi)){
        return nullptr;
    }
    return usuarios[indi];

}
// Peso para el sorteo: hasta 10 amigos en común -> 2, hasta 20 -> 5, más de 20 -> 7
// (prioridades 1, 2.5 y 3.5 multiplicadas por 2 para trabajar con enteros)
unsigned char UdeABook::pesoPorAmigosEnComun(unsigned short comunes){
    if(comunes<=10){
        return 2;
    }
    if(comunes<=20){
        return 5;
    }
    return 7;
}
#include <cstdlib>   // para rand()

// Sorteo de hasta 3 amigos: más amigos en común = más probabilidad de salir.
// Guarda los ids en resultado y devuelve cuántos eligió. previos son los 3 de la vez anterior (nullptr la primera vez)
unsigned char UdeABook::elegirTresAmigos(unsigned short* resultado, const unsigned short* previos){
    if (usuarioActivo == -1){
        return 0;
    }

    unsigned int comunes;
    unsigned int total = amistades.contarAmigos(usuarioActivo);
    if (total == 0){
        return 0;
    }

    unsigned short* ids = new unsigned short[total];
    unsigned char* pesos = new unsigned char[total];
    Medidor::sumarMemoria(total * sizeof(unsigned short) + total * sizeof(unsigned char));

    amistades.listarAmigos(usuarioActivo, ids);

    // Pesos: se calculan una sola vez
    unsigned int suma = 0;
    for (unsigned int i = 0; i < total; i++){
        Medidor::contar();
        if (previos != nullptr && (ids[i] == previos[0] || ids[i] == previos[1] || ids[i] == previos[2])){
            pesos[i] = 0;
        } else {
            comunes = amistades.amigosEnComun(usuarioActivo, ids[i]);
            pesos[i] = pesoPorAmigosEnComun(comunes);
        }
        suma += pesos[i];
    }
    unsigned char elegidos = 0;
    while(elegidos < 3 && suma > 0){
        // Número aleatorio entre 0 y suma-1
        unsigned int r = rand() % suma;
        for (unsigned int i = 0; i < total; i++){
            Medidor::contar();
            // Si r cae dentro del peso de este amigo, es el elegido
            if (r < pesos[i]){
                resultado[elegidos] = ids[i];  // Guarda el id
                suma -= pesos[i];              // Quita su peso del total
                pesos[i] = 0;                  // Lo anula para no repetirlo
                elegidos++;
                break;
            }
            // Si no cayó aquí, se le resta su peso a r y se sigue con el siguiente
            r -= pesos[i];
        }
    }

    delete[] ids;
    delete[] pesos;
    return elegidos;
}
// Envía una solicitud de amistad. Códigos: 0 enviada, 1 no existe, 2 es él mismo,
// 3 ya son amigos, 4 ya la había enviado, 5 esa persona ya le envió una a él
unsigned char UdeABook::enviarSolicitud(const string& nombre) {
    short indice = buscarUsuario(nombre);
    if (indice == -1) {
        return 1;
    }
    if (indice == usuarioActivo) {
        return 2;
    }
    if (amistades.sonAmigos(usuarioActivo, indice)) {
        return 3;
    }
    if (usuarios[indice]->tieneSolicitudDe(usuarioActivo)) {
        return 4;
    }
    if (usuarios[usuarioActivo]->tieneSolicitudDe(indice)) {
        return 5;
    }
    usuarios[indice]->agregarSolicitud(usuarioActivo);
    cambiosAmistades = true;

    return 0;

    return 0;
}
// Acepta la solicitud de esa posición: crea la amistad y la quita de la lista
void UdeABook:: aceptarSolicitud(unsigned int posicion ){
    Usuario* activo= usuarios[usuarioActivo];
    if(activo->cantidadSolicitudes()<=posicion){
        return;
    }
    unsigned int idusu=activo->getSolicitud(posicion);
    amistades.crearAmistad(usuarioActivo,idusu);
    // Al quitarla, las solicitudes siguientes se corren una posición
    activo->eliminarSolicitud(posicion);
    cambiosAmistades = true;
}


// Pide a la matriz de amistades las k mejores sugerencias (amigos de amigos) con sus amigos en común
unsigned short UdeABook::obtenerSugerencias(unsigned short k,unsigned short* ids,unsigned short* comunes){
    return amistades.calcularSugerencias(usuarioActivo,k,ids,comunes);
}

// Envía un mensaje a un amigo. Códigos: 0 enviado, 1 no existe o no es amigo,
// 2 mensaje vacío, 3 más de 512 caracteres
unsigned char UdeABook::enviarMensaje(const string& nombreAmigo, const string& texto) {
    if (texto.size() == 0) {
        return 2;
    }
    if (texto.size() > 512) {
        return 3;
    }
    Usuario* amigo = obtenerAmigo(nombreAmigo);
    if (amigo == nullptr) {
        return 1;
    }

    // Todo se valida antes del new, así un mensaje rechazado no deja memoria reservada
    Mensaje* m = new Mensaje(usuarioActivo, amigo->getId(), texto, Fecha::hoy());

    usuarios[usuarioActivo]->registrarEnviado(m);
    amigo->recibirMensaje(m);
    cambiosMensajes = true;

    return 0;

    return 0;
}
// Memoria total del sistema en bytes: el objeto, la lista de usuarios, cada usuario y la matriz de amistades
unsigned long long UdeABook::bytesUsados() {
    unsigned long long total = 0;

    total += sizeof(UdeABook);

    total += usuarios.bytesUsados();

    for (unsigned int i = 0; i < usuarios.getTamano(); i++) {
        Medidor::contar();
        if (usuarios[i] != nullptr) {
            total += usuarios[i]->bytesUsados();
        }
    }

    total += amistades.bytesUsados();

    return total;
}
// Avisa que hay mensajes por guardar (ej: se marcaron como leidos)
void UdeABook::marcarCambioMensajes() {
    cambiosMensajes = true;
}

// Avisa que hay likes por guardar
void UdeABook::marcarCambioLikes() {
    cambiosLikes = true;
}
