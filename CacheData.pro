QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

OBJECTS_DIR = ./
MOC_DIR = ./
RCC_DIR = ./

INCLUDEPATH += "C:\Program Files\OpenSSL-Win64\include"

LIBS += \
    -L"C:\Program Files\OpenSSL-Win64\lib\VC" -llibssl64MD \
    -L"C:\Program Files\OpenSSL-Win64\lib\VC" -llibcrypto64MD

SOURCES += \
        aesencryption.cpp \
        cachedata.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    aesencryption.h \
    cachedata.h
