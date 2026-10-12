#include "UdeABook.h"
#include<sstream>
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
///// bool UdeABook::cargarDatos(){}
//1;xx_daniel25;pass6647;xx.daniel25@mail.com;Barranquilla;Colombia;01/01/2004;30/03/2025
bool UdeABook::cargarDatos(){
    try{
        if(!cargarUsuarios()){
            throw string ("Algo salio mal cargando usuarios");

        }
        amistades.reiniciar(usuarios.getTamano());
        if(!cargarAmistades()||!cargarMensajes()||!cargarAlbunes()){
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
    try{
        string Archivo_Usuarios="C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\desafio2_2\\usuarios.txt";
        ifstream archivo(Archivo_Usuarios);
        if(!archivo.is_open()){
            throw string ("no se abrio el archivo de usuarios");
        }
        string linea;
        unsigned int N=0;
        while(getline(archivo,linea)){
            if(!linea.empty()&&linea[linea.size()-1]=='\r'){
                linea.pop_back();
            }
            if(!linea.empty()){
                N++;
            }
            Medidor::contar();
        }
        archivo.close();
        usuarios=Lista<Usuario*>(N);
        for(unsigned int i=0;i<N;i++){
            usuarios+=nullptr;
            Medidor::contar();
        }
        archivo.open(Archivo_Usuarios);
        if(!archivo.is_open()){
            throw string ("no se abrio el archivo de usuarios");
        }
        while(getline(archivo,linea)){
            Medidor::contar();
            if(!linea.empty()&&linea[linea.size()-1]=='\r'){
                linea.pop_back();
            }
            if(linea.empty()){
                continue;
            }
            istringstream flujo(linea);
            string campoId, nombreUsuario, contrasena,correo,ciudad,pais,fechaNac,fechaInsc;
            getline(flujo,campoId,';');
            getline(flujo,nombreUsuario,';');
            getline(flujo,contrasena,';');
            getline(flujo,correo,';');
            getline(flujo,ciudad,';');
            getline(flujo,pais,';');
            getline(flujo,fechaNac,';');
            getline(flujo,fechaInsc,';');
            unsigned id=stoi(campoId);
            if(id>=N||usuarios[id]!=nullptr){
                throw string ("archivo mal hecho, usuario repetido");
            }
            usuarios[id] = new Usuario((unsigned short)id, nombreUsuario, contrasena, correo,
                                       ciudad, pais, textoAFecha(fechaNac), textoAFecha(fechaInsc));
        }
    }
    catch(const string& error){
        cout<<"Error: "<<error<<endl;
        return false;
    }
    return true;
}
bool UdeABook::cargarAmistades(){

    try{
        string Archivo_Amistades="C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\desafio2_2\\amistades.txt";
        ifstream archivo(Archivo_Amistades);
        if(!archivo.is_open()){
            throw string ("no se abrio el archivo de amistades");
        }
        unsigned int N = usuarios.getTamano();
        string linea;
        string idUsuario, idsAmigos, idsSolicitudes, token;
        stringstream flujo, flujoAmigos, flujoSolicitudes;
        while(getline(archivo,linea)){
            Medidor::contar();
            if(!linea.empty()&&linea[linea.size()-1]=='\r'){
                linea.pop_back();
            }
            if(linea.empty()){
                continue;
            }
            flujo.clear();                                  // CAMBIO 2: se reutiliza el flujo
            flujo.str(linea);
            getline(flujo,idUsuario,';');
            unsigned int id = stoi(idUsuario);
            if(id >= N){                                    // CAMBIO 1: si el usuario no existe, se descarta la línea
                continue;
            }
            getline(flujo,idsAmigos,';');
            getline(flujo,idsSolicitudes,';');

            flujoAmigos.clear();                            // CAMBIO 2
            flujoAmigos.str(idsAmigos);
            while(getline(flujoAmigos,token,',')){
                unsigned int amigo = stoi(token);
                if(amigo < N){                              // CAMBIO 1: solo ids que existen
                    amistades.crearAmistad((unsigned short)id,(unsigned short)amigo);
                }
                Medidor::contar();
            }

            flujoSolicitudes.clear();                       // CAMBIO 2
            flujoSolicitudes.str(idsSolicitudes);
            while(getline(flujoSolicitudes,token,',')){
                unsigned int solicitante = stoi(token);
                if(solicitante < N){                        // CAMBIO 1
                    usuarios[id]->agregarSolicitud((unsigned short)solicitante);
                }
                Medidor::contar();
            }
        }
    }
    catch(string& error){
        cout<<"Error: "<<error<<endl;
        return false;
    }
    return true;
}
bool UdeABook::cargarMensajes(){
    try{
        // CAMBIO 1: la variable se llamaba Archivo_Amistades pero abre mensajes.txt
        string Archivo_Mensajes="C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\desafio2_2\\mensajes.txt";
        ifstream archivo(Archivo_Mensajes);
        string linea;
        if(!archivo.is_open()){
            throw string ("no se abrio el archivo de mensajes");
        }
        // CAMBIO 2: cantidad de usuarios, para validar los ids antes de usarlos
        unsigned int N = usuarios.getTamano();
        while(getline(archivo,linea)){
            Medidor::contar(); // CAMBIO 3: faltaba contar una vuelta por línea
            // CAMBIO 4 (error): el corchete estaba mal puesto: linea[linea.size()-1=='\r'].
            // Eso indexaba la línea con un 0 o un 1 y borraba SIEMPRE el último carácter.
            // Lo correcto es linea[linea.size()-1]=='\r'
            if(!linea.empty()&&linea[linea.size()-1]=='\r'){
                linea.pop_back();
            }
            if(linea.empty()){
                continue;
            }
            string emisor, receptor, fecha, leido,texto;
            istringstream flujo(linea);
            getline(flujo,emisor,';');
            getline(flujo,receptor,';');
            getline(flujo,fecha,';');
            getline(flujo,leido,';');
            getline(flujo,texto);
            if(emisor==""||receptor==""){
                continue;
            }
            // CAMBIO 5: se convierte cada id UNA sola vez (antes había 2 stoi por id y otros más)
            unsigned int idEmisor = stoi(emisor);
            unsigned int idReceptor = stoi(receptor);
            // CAMBIO 6: si el emisor o el receptor no existen, se salta la línea ANTES del new
            // (antes se leía fuera de la lista y, si fallaba, se perdía memoria)
            if(idEmisor >= N || idReceptor >= N){
                continue;
            }
            // CAMBIO 7 (error): el constructor recibe unsigned short, no string;
            // además "texro" estaba mal escrito
            Mensaje* m =new Mensaje((unsigned short)idEmisor,(unsigned short)idReceptor,texto,textoAFecha(fecha));
            if (leido=="1"){
                m->marcarLeido();
            }
            usuarios[idReceptor]->recibirMensaje(m);
            usuarios[idEmisor]->registrarEnviado(m);
        }
    }
    catch(string & error){
        cout<<"Error: "<<error<<endl;
        return false;
    }
    return true;
}
bool UdeABook::cargarAlbumes(){
    try{
        string Archivo_Album="C:\\Users\\gisel\\OneDrive\\Escritorio\\practi_c++\\fina2.0\\desafio2_2\\albumes.txt";
        ifstream archivo(Archivo_Album);
        string linea;
        if(!archivo.is_open()){
            throw string("no se abrio el archivo de albumes");
        }
        unsigned int N = usuarios.getTamano();
        string idUsuario,nombreAlbum,privacidad,cantidadFotos,nombreFoto,fecha,tamano,likes;
        istringstream flujo;
        while(getline(archivo,linea)){
            Medidor::contar();
            if(!linea.empty()&&linea[size(linea)-1]=='\r'){
                linea.pop_back();
            }
            if(linea.empty()){
                continue;
            }
            flujo.clear();
            flujo.str(linea);
            getline(flujo,idUsuario,';');
            getline(flujo,nombreAlbum,';');
            getline(flujo,privacidad,';');
            getline(flujo,cantidadFotos,';');
            unsigned int canti=stoi(cantidadFotos);
            Album* albu=new Album(nombreAlbum,(unsigned char)stoi(privacidad),canti);
            for(unsigned int i=0;i<canti;i++){
                if(!getline(archivo,linea)){
                    delete albu;
                    throw string("el archivo de albumes termino antes de tiempo");
                }
                Medidor::contar();
                // CAMBIO: borrado el bloque que quitaba el '\r' (stoi(likes) ya lo ignora)
                flujo.clear();
                flujo.str(linea);
                getline(flujo,nombreFoto,';');
                getline(flujo,fecha,';');
                getline(flujo,tamano,';');
                getline(flujo,likes,';');
                albu->agregarFoto(new Foto(nombreFoto,textoAFecha(fecha),stoi(tamano),stoi(likes)));
            }
            unsigned int id=stoi(idUsuario);
            if(id<N){
                usuarios[id]->agregarAlbum(albu);
            }
            else{
                delete albu;
            }
        }
    }
    catch(string& error){
        cout<<"Error: "<<error<<endl;
        return false;
    }
    return true;
}
