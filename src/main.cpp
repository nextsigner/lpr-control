#include "mainwindow.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QDebug>
#include <QPluginLoader>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if(argc>1){
        //qDebug()<<"Arg 0: "<<argv[0];
        //qDebug()<<"Arg 1: "<<argv[1];
        //qDebug()<<"Arg 2: "<<argv[2];
        QByteArray par1;
        par1.append(argv[1]);
        if(par1=="md"){
            w.aplicarModoDesarrollo();
        }else{
            //w.setearModoDesarrollo(false);
        }
    }
    QDesktopWidget desktop;
    if(desktop.geometry().width()>=1024 || desktop.geometry().height()>=720){
        //qDebug()<<"Ancho: "<<desktop.geometry().width();
        //qDebug()<<"Alto:  "<<desktop.geometry().height();
        w.show();
    }else{
        //qDebug()<<"Ancho: --- "<<desktop.geometry().width();
        //qDebug()<<"Alto: ---- "<<desktop.geometry().height();

        QMessageBox msgBox;
        msgBox.setWindowTitle("Error de Resolución de Pantalla.");
        msgBox.setInformativeText("Para un correcto funcionamiento de \nesta aplicación, se recomienda utilizar una\nresolución con un alto de pantalla igual o superior a 720 píxeles.");
        msgBox.setWindowModality(Qt::ApplicationModal);
        msgBox.setStandardButtons(QMessageBox::Ok );
        msgBox.exec();

        return -1;
        //w.show();
    }
    return a.exec();
}
