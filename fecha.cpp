#include "fecha.h"
#include <ctime>

// Constructor por defecto con valores iniciales
Fecha::Fecha() : dia(1), mes(1), anio(2026) {}

// Lista de inicializacion
Fecha::Fecha(unsigned char dia, unsigned char mes, unsigned short anio)
    : dia(dia), mes(mes), anio(anio) {}

// Comparacion jerarquica: anio -> mes -> dia
bool Fecha::operator<(const Fecha& otra) const {
    if (this->anio != otra.anio) {
        return this->anio < otra.anio;
    }
    if (this->mes != otra.mes) {
        return this->mes < otra.mes;
    }
    return this->dia < otra.dia;
}

bool Fecha::operator==(const Fecha& otra) const {
    if (this->anio == otra.anio && this->mes == otra.mes && this->dia == otra.dia) {
        return true;
    }
    return false;
}

Fecha Fecha::hoy() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    // tm_mon inicia en 0; tm_year cuenta desde 1900
    return Fecha(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
}

// Sin Fecha:: por ser friend. Usa 'salida' para soportar cout o archivos
ostream& operator<<(ostream& salida, const Fecha& f) {
    if (f.dia < 10) {
        // static_cast<int> evita que el unsigned char se imprima como caracter ASCII
        salida << "0" << static_cast<int>(f.dia) << "/";
    } else {
        salida << static_cast<int>(f.dia) << "/";
    }
    if (f.mes < 10) {
        salida << "0" << static_cast<int>(f.mes) << "/";
    } else {
        salida << static_cast<int>(f.mes) << "/";
    }
    salida << f.anio;

    return salida; // Permite encadenar impresiones
}

// Getters
unsigned short Fecha::getAnio() const {
    return this->anio;
}

unsigned char Fecha::getMes() const {
    return this->mes;
}

unsigned char Fecha::getDia() const {
    return this->dia;
}
