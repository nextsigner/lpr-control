#-------------------------------------------------
#
# Project created by QtCreator 2014-12-27T13:13:53
#
#-------------------------------------------------

QT       += core gui xml multimedia multimediawidgets
CONFIG += plugin

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lpr-control
TEMPLATE = app

win32 {
       #CONFIG += embed_manifest_exe
       #QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
       #QMAKE_CFLAGS_RELEASE -= -MD
       #QMAKE_CFLAGS_RELEASE = -MT
 }

SOURCES += main.cpp\
        mainwindow.cpp \
    config.cpp \
    stdafx.cpp \
    funcionesqcamera.cpp \
    areadeprocesamiento.cpp \
    dialogajustes.cpp \
    widgetsetareaprocesamiento.cpp \
    formconfigareaprocesamiento.cpp \
    widgetresize.cpp \
    rejilla.cpp \
    listademiniaturas.cpp \
    pantalla.cpp \
    videowidget.cpp \
    videowidgetsurface.cpp \
    funcionesvarias.cpp \
    galeriadecapturas.cpp \
    administradordearchivos.cpp

HEADERS  += mainwindow.h \
    config.h \
    definiciones.h \
    stdafx.h \
    areadeprocesamiento.h \
    dialogajustes.h \
    widgetsetareaprocesamiento.h \
    formconfigareaprocesamiento.h \
    widgetresize.h \
    rejilla.h \
    listademiniaturas.h \
    videowidget.h \
    videowidgetsurface.h \
    pantalla.h \
    galeriadecapturas.h \
    administradordearchivos.h

FORMS    += mainwindow.ui \
    formareadm.ui \
    dialogajustes.ui \
    formconfigareaprocesamiento.ui \
    formdesdecarpeta.ui \
    formprocesadordevideo.ui

QT+=widgets

RESOURCES += \
    recursos.qrc

DISTFILES +=
\
#INCLUDEPATH += E:\\OpenCV-2.3.1\\opencv\\build\\include

#LIBS += -LE:\\OpenCV-2.3.1\\mybuild\\lib\\Debug \
 #       -lopencv_core231d \
 #       -lopencv_highgui231d \
 #       -lopencv_imgproc231d \



#LIBS += -Lplugins
