TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        UdeABook.cpp \
        album.cpp \
        fecha.cpp \
        foto.cpp \
        listaT.cpp \
        main.cpp \
        medidor.cpp \
        mensaje.cpp \
        menu.cpp \
        sugenrenciasAmigos.cpp \
        usuario.cpp

HEADERS += \
    UdeABook.h \
    album.h \
    fecha.h \
    foto.h \
    listaT.h \
    medidor.h \
    mensaje.h \
    menu.h \
    sugenrenciasAmigos.h \
    usuario.h

DISTFILES += \
    albumes \
    amistades \
    mensajes \
    usuarios
