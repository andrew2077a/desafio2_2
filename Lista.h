#ifndef LISTAT_H
#define LISTAT_H
#include "medidor.h"
template <typename T>
class Lista {
private:
    T* datos;
    unsigned int tamano;
    unsigned int capacidad;
    void ampliar(){
        // CAMBIO 1 (error): se calcula en una variable local y se garantiza que crezca al menos 1
        // (antes con capacidad 0 o 1 no crecía y se escribía fuera del arreglo)
        unsigned int nuevaCapacidad = capacidad + (capacidad / 2);
        if (nuevaCapacidad <= capacidad) {
            nuevaCapacidad = capacidad + 1;
        }
        T* datosNuevos = new T[nuevaCapacidad];
        for(unsigned long long i=0;i<tamano;i++){
            datosNuevos[i]=this->datos[i];
            Medidor::contar();
        }
        delete[]datos;
        this->datos=datosNuevos;
        this->capacidad=nuevaCapacidad; // CAMBIO 1: la capacidad se actualiza al final
    }

public:
    // Constructor por defecto
    Lista() : datos(nullptr), tamano(0), capacidad(0) {}

    // Constructor con capacidad inicial
    // CAMBIO 6 (opcional): explicit evita conversiones implícitas de unsigned int a Lista
    explicit Lista(unsigned int capacidad) {
        this->capacidad = capacidad;
        this->tamano = 0;

        if (capacidad > 0) {
            datos = new T[capacidad];
        } else {
            datos = nullptr;
        }
    }
    Lista(const Lista<T>& otra){
        this->capacidad=otra.capacidad;
        this->tamano=otra.tamano;
        if (this->capacidad>0){
            this->datos=new T[capacidad];
            for(unsigned int i=0;i<tamano;i++){
                this->datos[i]=otra.datos[i];
                Medidor::contar();
            }
        }
        else{
            datos=nullptr;
        }
    }
    ~Lista(){
        delete[] datos;
    }

    Lista<T>& operator=(const Lista<T>& otra) {
        // 1. Evitar autoasignación (a = a)
        if (this == &otra) {
            return *this;
        }
        T* nuevoDatos = nullptr;
        // CAMBIO 2 (error): se evalúa la capacidad de 'otra', no la del objeto actual
        if (otra.capacidad>0){
            nuevoDatos = new T[otra.capacidad];
            for (unsigned int i = 0; i < otra.tamano; i++) {
                nuevoDatos[i] = otra.datos[i];
                Medidor::contar();
            }
        }
        // 4. Liberar la memoria antigua
        delete[] datos;
        // 5. Reasignar puntero y actualizar atributos
        datos = nuevoDatos;
        tamano = otra.tamano;
        capacidad = otra.capacidad;
        // 6. Retornar la referencia al objeto actual
        return *this;
    }
    T& operator[](unsigned int i){
        return this->datos[i];
    }
    const T& operator[](unsigned int i ) const{
        return this->datos[i];
    }
    Lista<T>& operator+=(const T& elemento){
        if(this->tamano==this->capacidad){
            ampliar();
        }
        this->datos[tamano]=elemento;
        this->tamano+=1;
        return *this;
    }
    void insertarEn(unsigned int pos ,const T& elemento ){
        // CAMBIO 4 (opcional): si la posición es inválida no se hace nada
        if(pos>this->tamano){
            return;
        }
        if(this->tamano==this->capacidad){
            ampliar();
        }
        for(unsigned int i=this->tamano;i>pos;i--){
            this->datos[i]=datos[i-1];
            Medidor::contar();
        }
        this->datos[pos]=elemento;
        tamano+=1;

    }
    void eliminarEn(unsigned int pos){
        // CAMBIO 3 (opcional): valida lista vacía y posición inválida
        // (con tamano=0, 'tamano-1' daba un número enorme por ser unsigned)
        if(pos>=this->tamano){
            return;
        }
        for(unsigned int i=pos;i<tamano-1;i++){
            this->datos[i]=datos[i+1];
            Medidor::contar();
        }
        tamano-=1;
    }
    unsigned int getTamano()const{
        return this->tamano;
    }
    unsigned int getCapacidad()const{
        return this->capacidad;
    }
    unsigned long long bytesUsados()const{
        return (unsigned long long)this->capacidad*sizeof(T);
    }
};

#endif // LISTAT_H
