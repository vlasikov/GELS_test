QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): CONFIG += c++11
lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++11

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chrome_mb.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    settings.cpp

HEADERS += \
    chrome_mb.h \
    mainwindow.h \
    qcustomplot.h \
    settings.h

FORMS += \
    GUI1.ui

INCLUDEPATH += $$PWD/libmodbus-3.1.8/include
DEPENDPATH += $$PWD/libmodbus-3.1.8/include
LIBS += -L$$PWD/libmodbus-3.1.8/lib/ -lmodbus
LIBS += -lws2_32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    gui.qrc

RC_ICONS = icons/chrome1.ico
