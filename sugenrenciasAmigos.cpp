#include "sugenrenciasAmigos.h"

// Constructor por defecto: inicializa atributos en cero / nulo.
SugerenciasAmigos::SugerenciasAmigos()
    : cantidadUsuarios(0), palabras(0), bloque(nullptr) {}

// Constructor de copia: duplica el arreglo de bits.
SugerenciasAmigos::SugerenciasAmigos(const SugerenciasAmigos& otro)
    : cantidadUsuarios(otro.cantidadUsuarios), palabras(otro.palabras), bloque(nullptr) {
    if (palabras > 0) {
        bloque = new unsigned long long[palabras];
        for (unsigned int i = 0; i < palabras; ++i) {
            Medidor::contar();
            bloque[i] = otro.bloque[i];
        }
    }
}

// Destructor: libera la memoria del arreglo.
SugerenciasAmigos::~SugerenciasAmigos() {
    delete[] bloque;
}

// Calcula el índice 1D en la matriz triangular asegurando que i > j.
unsigned int SugerenciasAmigos::posicion(unsigned short i, unsigned short j) const {
    if (i < j) {
        unsigned short temp = i;
        i = j;
        j = temp;
    }
    return (unsigned int)i * (i - 1) / 2 + j;
}

// Reasigna la memoria según la nueva cantidad de usuarios.
void SugerenciasAmigos::reiniciar(unsigned short cantidadUsuarios) {
    delete[] bloque;
    this->cantidadUsuarios = cantidadUsuarios;

    unsigned long long parejas = (unsigned long long)cantidadUsuarios * (cantidadUsuarios - 1) / 2;

    palabras = (parejas + 63) / 64; // Calcula bloques de 64 bits necesarios

    if (palabras > 0) {
        bloque = new unsigned long long[palabras];
        for (unsigned int i = 0; i < palabras; ++i) {
            Medidor::contar();
            bloque[i] = 0;
        }
    } else {
        bloque = nullptr;
    }
}

// Enciende el bit correspondiente a la amistad entre a y b.
void SugerenciasAmigos::crearAmistad(unsigned short a, unsigned short b) {
    if (a == b) return;
    unsigned int k = posicion(a, b);
    bloque[k / 64] |= (1ULL << (k % 64));
}

// Lee el bit para verificar si a y b son amigos.
bool SugerenciasAmigos::sonAmigos(unsigned short a, unsigned short b) const {
    if (a == b) return false;
    unsigned int k = posicion(a, b);
    return (bool)((bloque[k / 64] >> (k % 64)) & 1);
}

// Retorna el total de amigos del usuario a.
unsigned short SugerenciasAmigos::contarAmigos(unsigned short a) const {
    unsigned short contador = 0;
    for (unsigned short b = 0; b < cantidadUsuarios; ++b) {
        Medidor::contar();
        if (sonAmigos(a, b)) {
            contador++;
        }
    }
    return contador;
}

// Guarda los IDs de los amigos de a en destino y retorna el total.
unsigned short SugerenciasAmigos::listarAmigos(unsigned short a, unsigned short* destino) const {
    unsigned short contador = 0;
    for (unsigned short b = 0; b < cantidadUsuarios; ++b) {
        Medidor::contar();
        if (sonAmigos(a, b)) {
            destino[contador++] = b;
        }
    }
    return contador;
}

// Cuenta cuántos amigos en común tienen a y b.
unsigned short SugerenciasAmigos::amigosEnComun(unsigned short a, unsigned short b) const {
    unsigned short contador = 0;
    for (unsigned short c = 0; c < cantidadUsuarios; ++c) {
        Medidor::contar();
        if (c != a && c != b && sonAmigos(a, c) && sonAmigos(b, c)) {
            contador++;
        }
    }
    return contador;
}

// Encuentra las k mejores sugerencias de amistad para usuario.
unsigned short SugerenciasAmigos::calcularSugerencias(unsigned short usuario, unsigned short k, unsigned short* ids, unsigned short* comunes) const {

    unsigned short* contador = new unsigned short[cantidadUsuarios];
    unsigned short* amigos = new unsigned short[cantidadUsuarios];

    Medidor::sumarMemoria(cantidadUsuarios * sizeof(unsigned short) * 2);

    for (unsigned short i = 0; i < cantidadUsuarios; ++i) {
        contador[i] = 0;
    }

    unsigned short cantAmigos = listarAmigos(usuario, amigos);

    // Cuenta amigos en común evaluando los amigos de mis amigos (G)
    for (unsigned short i = 0; i < cantAmigos; ++i) {
        unsigned short F = amigos[i];
        for (unsigned short G = 0; G < cantidadUsuarios; ++G) {
            Medidor::contar();
            if (G != usuario && G != F && sonAmigos(F, G) && !sonAmigos(usuario, G)) {
                contador[G]++;
            }
        }
    }

    unsigned short encontradas = 0;

    // Extrae los k usuarios con más conexiones en común
    for (unsigned short i = 0; i < k; ++i) {
        unsigned short maxComun = 0;
        unsigned short mejorG = 65535;

        for (unsigned short G = 0; G < cantidadUsuarios; ++G) {
            Medidor::contar();
            if (contador[G] > maxComun) {
                maxComun = contador[G];
                mejorG = G;
            }
        }

        if (maxComun == 0) {
            break; // Detiene si ya no hay más opciones válidas
        }

        ids[encontradas] = mejorG;
        comunes[encontradas] = maxComun;
        contador[mejorG] = 0; // Descarta al usuario ya seleccionado
        encontradas++;
    }

    delete[] contador;
    delete[] amigos;

    return encontradas;
}

// Retorna el tamaño en bytes ocupado por la matriz de bits.
unsigned long long SugerenciasAmigos::bytesUsados() const {
    return palabras * sizeof(unsigned long long);
}