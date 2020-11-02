QT += core gui svg
#QT += multimedia
#QT += multimediawidgets
QT += network websockets
#QT += multimedia
#QT += script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += lang-zh_CN
CONFIG += console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

VERSION = 2.0.0
QMAKE_TARGET_COMPANY = BigtreeTech
QMAKE_TARGET_PRODUCT = OctoBTT
QMAKE_TARGET_COPYRIGHT = Copyright(C) 2020 By BigtreeTech
QMAKE_TARGET_DESCRIPTION = First suggested for OctoPi


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cameradialog.cpp \
    configuration.cpp \
    controlpanel.cpp \
    filedialog.cpp \
    gcodedialog.cpp \
    inputdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    materialctrlpanel.cpp \
    mylineedit.cpp \
    octonetwork.cpp \
    terminaldialog.cpp \
    url_camera.cpp \
    wlanconfig.cpp

HEADERS += \
    cameradialog.h \
    configuration.h \
    controlpanel.h \
    filedialog.h \
    gcodedialog.h \
    inputdialog.h \
    mainwindow.h \
    materialctrlpanel.h \
    mylineedit.h \
    octonetwork.h \
    terminaldialog.h \
    url_camera.h \
    wlanconfig.h

FORMS += \
    cameradialog.ui \
    configuration.ui \
    controlpanel.ui \
    filedialog.ui \
    gcodedialog.ui \
    inputdialog.ui \
    mainwindow.ui \
    materialctrlpanel.ui \
    terminaldialog.ui \
    url_camera.ui \
    wlanconfig.ui

TRANSLATIONS += \
    OctoBTT_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets.qrc

DISTFILES += \
    start.sh \
    wav/KBClick.wav
