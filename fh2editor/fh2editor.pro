TARGET = fh2editor

QT += widgets xml

RESOURCES = resources.qrc

win32-g++-cross {
    CONFIG += console
}

HEADERS += mainwindow.h mapwindow.h mapdata.h dialogs.h
SOURCES += program.cpp engine.cpp mainwindow.cpp mapwindow.cpp mapdata.cpp dialogs.cpp global.cpp
