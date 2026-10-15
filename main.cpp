#include <iostream>
#include "menu.h"
#include "UdeABook.h"
using namespace std;

int main(){
    UdeABook ude;
    Menu facebook(ude);
    facebook.ejecutar();

    return 0;
}