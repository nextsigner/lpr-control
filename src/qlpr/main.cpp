#include <QCoreApplication>
#include "procesadorlpr.h"
#include <iostream>

#include <QDebug>
#include <QDir>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //if(argv)
    ProcesadorLPR procLPR;
    procLPR.inicializarLPR();

    //qDebug() <<  "Ejecutando QLpr..."<<argv[1] ;
    //cout<<"Ruta:"<<argv[1];

    QStringList listaCandidatos;
    QStringList listaDeChapasOptimas;


    QDir path(argv[1]);
    //QDir path(QDir::currentPath());
    QStringList files = path.entryList(QDir::Files);

    //QStringList files = path.entryList(QDir::Files);

for(int i=0;i<files.size();i++){
    QString rutaImagen;
    rutaImagen.append(argv[1]);
    //rutaImagen.append(QDir::currentPath());
    rutaImagen.append("/");
    rutaImagen.append(files.at(i));
    QString chapa = procLPR.getChapa(rutaImagen);
    QByteArray resultadoChapa;
    resultadoChapa.append(chapa);
    //cout<<"Chapa: " << resultadoChapa.constData();
    if(chapa!=""){
        listaCandidatos.append(chapa);
    }
}

if(listaCandidatos.size()==1){
    QString cha;
    cha.append(listaCandidatos.at(0));
    //mostrarEnTabla(cha);
    qDebug() << "[1]:" <<cha;
}else if(listaCandidatos.size()>2 && listaCandidatos.size()<6){
    QStringList nl = listaCandidatos;
    for(int i=0;i<listaDeChapasOptimas.size();i++){
        nl.append(listaDeChapasOptimas.at(i));
    }
    qDebug() << "[1]:" <<procLPR.clasificarChapas(listaCandidatos).constData();
}else if(listaCandidatos.size()>=6){
    int mitad = listaCandidatos.size()/2;
    QStringList nl = listaCandidatos;

    for(int i=0;i<listaDeChapasOptimas.size();i++){
        nl.append(listaDeChapasOptimas.at(i));
    }
    qDebug() << "[1]:" <<procLPR.clasificarChapas(nl).constData();
    procLPR.stop();

}else{

    qDebug() << "[0]:" <<"--- ---";

}


    //a.exit(0);


//delete procLPR;

    return 0;
}
