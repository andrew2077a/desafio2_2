#ifndef FECHA_H
#define FECHA_H
#include <iostream>
using namespace std;

class Fecha {

public:
    Fecha();
    Fecha(unsigned char dia, unsigned char mes, unsigned short anio);

    // const: No modifica los atributos
    bool operator<(const Fecha&) const;
    bool operator==(const Fecha&) const;

    // static: Se usa con Fecha::hoy()
    static Fecha hoy();

    // friend: Accede a privados; sin Fecha::
    friend ostream& operator<<(ostream& salida, const Fecha& f);

    // Getters const: Obligatorios para objetos constantes
    unsigned char getDia() const;
    unsigned char getMes() const;
    unsigned short getAnio() const;
    ~Fecha();


private:
    unsigned char dia;
    unsigned char mes;
    unsigned short anio;
};

#endif // FECHA_H
