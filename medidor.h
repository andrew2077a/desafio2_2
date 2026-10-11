#ifndef MEDIDOR_H
#define MEDIDOR_H

class Medidor{
private:
    static unsigned long long iteraciones;
    static unsigned long long memoria;
    Medidor();

public:
    static void reiniciar();
    static void contar(unsigned long long n=1);
    static void sumarMemoria(unsigned long long bytes);
    static unsigned long long getIteraciones();
    static unsigned long long getMemoria();
};

#endif // MEDIDOR_H