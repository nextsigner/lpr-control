#include "administradordearchivos.h"

AdministradorDeArchivos::AdministradorDeArchivos(QObject* parent):QThread(parent)
{
    qDebug() << "is createad Thread ok :D";
}
void AdministradorDeArchivos::run()
{
    qDebug() << "Ejecutando ADA";
    //qDebug() << "begin thread";     this->msleep(10000);
    //qDebug() << "end thread";
}

QStringList AdministradorDeArchivos::listaDeRutas(QString modo, QString raiz)
{
    /*QDir traiz;
    traiz.setCurrent(raiz);
    traiz.setSorting(QDir::Name);*/
    QStringList lista;

    if(modo=="carpetas"){
        /*QDirIterator directories(raiz, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while(directories.hasNext()){
            directories.next();
            lista.append(directories.filePath());
            qDebug() << "Listando Carpetas: "<<lista.at(lista.size()-1);
        }*/
        QDir path(raiz);
        path.setSorting(QDir::Time);

        lista = path.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    }
    if(modo=="archivos"){
        /*QDirIterator directories(raiz, QDir::Files);
        while(directories.hasNext()){
            directories.next();
            lista.append(directories.filePath());
            qDebug() << "Listando Archivos: "<<lista.at(lista.size()-1);
        }*/
        QDir path(raiz);
        path.setSorting(QDir::Name);

        lista = path.entryList(QDir::Files);
    }

    /*for (int i = 0; i < lista.size(); ++i) {
        qDebug() << "Listando "<<modo << ": " <<lista.at(i);
    }*/



    return lista;
}
