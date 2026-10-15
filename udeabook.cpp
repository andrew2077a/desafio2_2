#include "UdeABook.h"
#include<sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

UdeABook::UdeABook()
    : usuarios(), amistades(), usuarioActivo(-1) {
}

UdeABook::~UdeABook() {
    for (unsigned int i = 0; i < usuarios.getTamano(); i++) {
        Medidor::contar(); // se cuenta cada vuelta
        if (usuarios[i] != nullptr) {
            delete usuarios[i];
        }
    }

}

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
bool UdeABook::cargarUsuarios() {
    try {
        string Archivo_Usuarios = "C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\desafio2_2\\usuarios";
        ifstream archivo(Archivo_Usuarios);
        if (!archivo.is_open()) {
            throw string("no se abrio el archivo de usuarios");
        }

        string linea;
        unsigned int N = 0;

        // CAMBIO: Se lee hasta '\r' para ignorar el retorno de carro en el conteo
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

            // CAMBIO: Se usa '\r' como delimitador para descartar el retorno de carro en el ultimo campo
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

            // CAMBIO: Se usa '\r' como delimitador para descartar el salto de linea del ultimo campo
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

            // CAMBIO: Se usa '\r' como delimitador en el último campo para descartar el salto de línea sin usa pop_back()
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
            // CAMBIO: Se usa '\r' como delimitador para que el salto de línea no quede en la variable
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

                // CAMBIO: Se usa '\r' como delimitador para limpiar el último dato de la foto
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

Fecha UdeABook::textoAFecha(const string& texto) {
    stringstream flujo(texto);
    string dia, mes, anio;

    getline(flujo, dia, '/');
    getline(flujo, mes, '/');
    getline(flujo, anio);

    return Fecha((unsigned char)stoi(dia), (unsigned char)stoi(mes), (unsigned short)stoi(anio));
}
bool UdeABook::guardarDatos(bool conAmistades, bool conMensajes, bool conLikes){
    if(conAmistades && !guardarAmistades()){
        return false;
    }
    if(conMensajes && !guardarMensajes()){
        return false;
    }
    if(conLikes && !guardarLikes()){
        return false;
    }
    return true;
}

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
bool UdeABook::guardarMensajes(){
    string Archivo_Mensajes="C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\desafio2_2\\mensajes";
    ofstream archivo(Archivo_Mensajes);
    try{
        // CAMBIO 1 (error de lógica): estaba al revés. Lanzaba el error cuando el archivo SÍ abría.
        // Además el mensaje decía "amistades"
        if (!archivo.is_open()){
            throw string("no se abrio el archivo de mensajes para escribir");
        }
        unsigned int N= usuarios.getTamano();
        // CAMBIO 2 (error): i no estaba inicializada, empezaba con basura
        for(unsigned int i=0;i<N;i++){
            unsigned n=usuarios[i]->cantidadRecibidos();
            // CAMBIO 2: igual con e
            for(unsigned int e=0;e<n;e++){
                Medidor::contar(); // CAMBIO 3: faltaba contar una vuelta por mensaje
                // CAMBIO 4 (error de compilación y de lógica): no se crea nada con new, el mensaje ya existe.
                // Se pide el mensaje e del usuario i (antes usabas usuarios[e] y no le pasabas el índice)
                Mensaje* m= usuarios[i]->getRecibido(e);
                archivo<<m->getEmisor()<<';';
                archivo<<m->getReceptor()<<';';
                archivo<<m->getFecha()<<';';
                archivo<<m->estaLeido()<<';';
                archivo<<m->getTexto()<<'\n';
            }
        }
        archivo.close(); // CAMBIO 7: se cierra para asegurar que todo quedó escrito
    }
    catch (string& error){
        cout<<"Error: "<<error<<endl;
        return false;
    }
    return true;

}
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
                    Medidor::contar(); // CAMBIO 2: también una vuelta por cada línea de foto
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
void UdeABook::cerrarSesion(){
    usuarioActivo=-1;
}

Usuario* UdeABook:: getUsuarioActivo(){
    if(usuarioActivo==-1){
        return nullptr;
    }
    return usuarios[usuarioActivo];
}
Usuario* UdeABook:: getUsuario( unsigned short id ){
    if(id>=usuarios.getTamano()){
        return nullptr;
    }
    return usuarios[id];
}
short UdeABook::buscarUsuario(const string& nombre) {
    for (unsigned int i = 0; i < usuarios.getTamano(); i++) {
        Medidor::contar();
        if (usuarios[i]->getNombreUsuario() == nombre) {
            return static_cast<short>(i);
        }
    }
    return -1;
}
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

    return 0;
}
void UdeABook:: aceptarSolicitud(unsigned int posicion ){
    Usuario* activo= usuarios[usuarioActivo];
    if(activo->cantidadSolicitudes()<=posicion){
        return;
    }
    unsigned int idusu=activo->getSolicitud(posicion);
    amistades.crearAmistad(usuarioActivo,idusu);
    activo->eliminarSolicitud(posicion);
}

unsigned short UdeABook::obtenerSugerencias(unsigned short k,unsigned short* ids,unsigned short* comunes){
    return amistades.calcularSugerencias(usuarioActivo,k,ids,comunes);
}

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

    Mensaje* m = new Mensaje(usuarioActivo, amigo->getId(), texto, Fecha::hoy());

    usuarios[usuarioActivo]->registrarEnviado(m);
    amigo->recibirMensaje(m);

    return 0;
}
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