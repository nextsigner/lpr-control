#include "areadeprocesamiento.h"
#include <QtWidgets/QFrame>
#include <QDebug>

AreaDeProcesamiento::AreaDeProcesamiento(QWidget *parent) : QWidget(parent)
{
    QFrame *f = new QFrame(this);
    f->setGeometry(0,0,720,480);
    setGeometry(0,0,720,480);

    /*QLabel linea  = new QLabel(this);
    linea.setGeometry();*/

    /*sent = new QLabel(this);
     sent->setGeometry(0,0,100,100);
     //sent->hide();
    sent->setStyleSheet(""
                       "background-color:#ff6633;"



                      "");
    sent->hide();*/


    wa = new widgetSetAreaProcesamiento(this);
    wa->setObjectName("nwa");
    //wa->setMouseTracking(true);
    //wa->installEventFilter(this);
    connect(wa, SIGNAL(moviendo(int,int,int,int)), this, SLOT(setArea(int,int,int,int)));

    ws = new QWidget(this);
    //ws->setMouseTracking(true);
   // ws->installEventFilter(this);

    wd = new QWidget(this);
    //wd->setMouseTracking(true);
    //wd->installEventFilter(this);

    wi = new QWidget(this);
    //wi->setMouseTracking(true);
    //wi->installEventFilter(this);

    win = new QWidget(this);
    //win->setMouseTracking(true);
    //win->installEventFilter(this);

}

AreaDeProcesamiento::~AreaDeProcesamiento()
{

}

bool AreaDeProcesamiento::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type()==QEvent::MouseMove){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        //qDebug()<<"Moviendose en AP! " << mouseEvent->pos().x() << " "<< mouseEvent->pos().y();
        }
    return false;
}

void AreaDeProcesamiento::setArea(int px, int py, int an, int al)
{
    if(px>2&&px<620&&py>2&&py<380&&an>99&&an<619&&al>99&&al<380&&py<480-al&&px<720-an){
    wa->setGeometry(px,py,an,al);
    wa->setStyleSheet("background-color:transparent;");
    //ws->setStyleSheet("background-color:#0066ff;");
    wa->show();

    ws->setGeometry(0,0,720,480-(480-py));
    ws->setStyleSheet("background-color:#ff0066;");

    wi->setGeometry(0,wa->pos().y(),720-(720-px),al);
    wi->setStyleSheet("background-color:#ff0066;");

    wd->setGeometry(px+an,py,720-(wi->width()+wa->width()),al);
    wd->setStyleSheet("background-color:#ff0066;");

    win->setGeometry(0,py+al,720,480-(480-al));
    win->setStyleSheet("background-color:#ff0066;");
    win->show();

    emit seteandoArea(px, py, an, al);
    }
}

/*void AreaDeProcesamiento::setSentidoDetectado(QString sentido)
{
    QString cadImagen;
    cadImagen.append(":/imagenes/Sentido");
    cadImagen.append(sentido);
    cadImagen.append(".png");
    QImage sentidoImagen(cadImagen);
    QPixmap pix_sentido = QPixmap::fromImage(sentidoImagen.scaled(100, 100, Qt::IgnoreAspectRatio));


    sent->setGeometry(0, 0, 100, 100);
    sent->setPixmap(pix_sentido);
    sent->show();
}*/

void AreaDeProcesamiento::mover(int x, int y)
{
    move(x, y);
}

