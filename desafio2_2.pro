TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        UdeABook.cpp \
        album.cpp \
        fecha.cpp \
        foto.cpp \
        main.cpp \
        medidor.cpp \
        mensaje.cpp \
        menu.cpp \
        sugenrenciasAmigos.cpp \
        usuario.cpp

HEADERS += \
    Lista.h \
    UdeABook.h \
    album.h \
    fecha.h \
    foto.h \
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
