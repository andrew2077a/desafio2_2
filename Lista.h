#ifndef LISTAT_H
#define LISTAT_H
#include "medidor.h"

template <typename T>
class Lista {
private:
    T* datos;
    unsigned int tamano;
    unsigned int capacidad;

    // Redimensiona la memoria y copia los elementos al nuevo arreglo
    void ampliar() {
        unsigned int nuevaCapacidad = capacidad + (capacidad / 2);
        if (nuevaCapacidad <= capacidad) {
            nuevaCapacidad = capacidad + 1;
        }

        T* datosNuevos = new T[nuevaCapacidad];

        // Copia ordenadamente contando cada operación
        for (unsigned int i = 0; i < tamano; i++) {
            datosNuevos[i] = this->datos[i];
            Medidor::contar();
        }

        delete[] datos; // Libera el arreglo anterior
        this->datos = datosNuevos;
        this->capacidad = nuevaCapacidad;
    }

public:
    // Estado inicial de la lista
    Lista() : datos(nullptr), tamano(0), capacidad(0) {}

    // Reserva espacio inicial para el arreglo
    Lista(unsigned int capacidad) {
        this->capacidad = capacidad;
        this->tamano = 0;

        if (capacidad > 0) {
            datos = new T[capacidad];
        } else {
            datos = nullptr;
        }
    }

    // Copia los elementos de otra lista
    Lista(const Lista<T>& otra) {
        this->capacidad = otra.capacidad;
        this->tamano = otra.tamano;

        if (this->capacidad > 0) {
            this->datos = new T[this->capacidad];
            for (unsigned int i = 0; i < tamano; i++) {
                this->datos[i] = otra.datos[i];
                Medidor::contar();
            }
        } else {
            datos = nullptr;
        }
    }

    // Libera la memoria del arreglo
    ~Lista() {
        delete[] datos;
    }

    // Asigna otra lista evitando autoasignacion
    Lista<T>& operator=(const Lista<T>& otra) {
        if (this == &otra) {
            return *this;
        }

        T* nuevoDatos = nullptr;
        if (otra.capacidad > 0) {
            nuevoDatos = new T[otra.capacidad];
            for (unsigned int i = 0; i < otra.tamano; i++) {
                nuevoDatos[i] = otra.datos[i];
                Medidor::contar();
            }
        }

        delete[] datos; // Limpia la memoria actual
        datos = nuevoDatos;
        tamano = otra.tamano;
        capacidad = otra.capacidad;

        return *this;
    }

    // Acceso directo a una posicion (modificable)
    T& operator[](unsigned int i) {
        return this->datos[i];
    }

    // Acceso directo a una posicion (solo lectura)
    const T& operator[](unsigned int i ) const {
        return this->datos[i];
    }

    // Agrega un elemento al final
    Lista<T>& operator+=(const T& elemento) {
        if (this->tamano == this->capacidad) {
            ampliar();
        }
        this->datos[tamano] = elemento;
        this->tamano += 1;
        return *this;
    }

    // Desplaza de atras hacia adelante para abrir el espacio en pos
    void insertarEn(unsigned int pos, const T& elemento) {
        if (pos > this->tamano) {
            return;
        }
        if (this->tamano == this->capacidad) {
            ampliar();
        }

        // Recorrido inverso para rodar los elementos
        for (unsigned int i = this->tamano; i > pos; i--) {
            this->datos[i] = datos[i - 1];
            Medidor::contar();
        }
        this->datos[pos] = elemento;
        tamano += 1;
    }

    // Desplaza de adelante hacia atras para cubrir el hueco en pos
    void eliminarEn(unsigned int pos) {
        if (pos >= this->tamano) {
            return;
        }

        // Recorrido directo para rodar los elementos
        for (unsigned int i = pos; i < tamano - 1; i++) {
            this->datos[i] = datos[i + 1];
            Medidor::contar();
        }
        tamano -= 1;
    }

    // Cantidad de elementos guardados
    unsigned int getTamano() const {
        return this->tamano;
    }

    // Capacidad total del arreglo
    unsigned int getCapacidad() const {
        return this->capacidad;
    }

    // Memoria total que ocupa el arreglo en bytes
    unsigned long long bytesUsados() const {
        return (unsigned long long)this->capacidad * sizeof(T);
    }
};

#endif // LISTAT_H