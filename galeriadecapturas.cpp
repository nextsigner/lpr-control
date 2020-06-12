#include "galeriadecapturas.h"
#include <QDebug>

GaleriaDeCapturas::GaleriaDeCapturas(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *main = new QHBoxLayout(this);
    //main->si
    img = new QLabel(this);
    infoimg = new QLabel(this);
    infoimg->setText("");
    infoimg->setVisible(false);
    infoimg->setStyleSheet("background-color:#000; color:#ffffff;  font-weight: bold; font-size:12px; padding:3px; ");

    velocidad=400;

    main->addWidget(img);
    //img->setGeometry(0,0,100,100);
    //img->setStyleSheet("background-color:#3300ff;");
    timerPlay= new QTimer(this);
    connect(timerPlay, SIGNAL(timeout()), this, SLOT(nextFotograma()));
    numcap=0;
    //listaFotogramas=new QList
}

void GaleriaDeCapturas::reproducir(QString ev, int cant)
{
    numcap=0;
    listaFotogramas.clear();
    timerPlay->stop();
    //dirTemp.setCurrent(carpEvento);
    //dirTemp.setSorting(QDir::Name);
    //imagenes = dirTemp.entryList(QDir::Files);
    for (int i = 0; i < cant-1; ++i) {
        QString dt;
        dt.append(ev);
        dt.append("/");
        dt.append(QString::number(i));
        dt.append(".jpg");

        dt.replace("\\", "/");
         //qDebug()<<"00000Ruta: "<<dt;

        listaFotogramas.append(QImage(dt));
    }
    //qDebug()<<"111Ruta de Capturas: "<<ev<<" Cantidad de Imagenes: "<<listaFotogramas.size();

    //img->setGeometry(0,0,width(), height());
    //setMinimumHeight(listaFotogramas.at(0).height());
    //img->setGeometry(0,0,listaFotogramas.at(0).width(),listaFotogramas.at(0).height());
    timerPlay->start(velocidad);

}

void GaleriaDeCapturas::setearCarpetaEvento(QString cev)
{
    carpEvento=cev;
}

void GaleriaDeCapturas::nextFotograma()
{

    infoimg->setVisible(true);
    QImage foto= listaFotogramas.at(numcap).scaled(img->size(),Qt::KeepAspectRatio);
    img->setPixmap(QPixmap::fromImage(foto));
    img->setGeometry(0,0,foto.width(), foto.height());
    QString info;
    info.append("Cap ");
    info.append(QString::number(numcap+1));
    info.append(" de ");
    info.append(QString::number(listaFotogramas.size()+1));
    infoimg->setText(info);
    if(numcap<listaFotogramas.size()-1){
        numcap++;
    }else{
        numcap=0;
    }
}

void GaleriaDeCapturas::prevFotograma()
{
    if(numcap!=0){
    timerPlay->stop();
    infoimg->setVisible(true);
    numcap--;
    QImage foto= listaFotogramas.at(numcap).scaled(img->size(),Qt::KeepAspectRatio);
    img->setPixmap(QPixmap::fromImage(foto));
    img->setGeometry(0,0,foto.width(), foto.height());
    QString info;
    info.append("Cap ");
    info.append(QString::number(numcap));
    info.append(" de ");
    info.append(QString::number(listaFotogramas.size()));
    infoimg->setText(info);
     }
}

void GaleriaDeCapturas::setearEstadoCinematica(bool b)
{
    if(b){
        timerPlay->start(velocidad);
    }else{
      timerPlay->stop();
    }
}

void GaleriaDeCapturas::setearVelocidad(int v)
{
    timerPlay->stop();
    timerPlay->start(v);
    velocidad=v;
}

