#include "widgetsetareaprocesamiento.h"
#include <QFrame>
#include <QLabel>
#include <QDebug>

widgetSetAreaProcesamiento::widgetSetAreaProcesamiento(QWidget *parent) : QWidget(parent)
{

    esMovible = false;

    QFrame *f = new QFrame(this);
    f->setGeometry(0,0,720,576);
    setGeometry(0,0,720,576);
    f->setStyleSheet("background-color:transparent;");
    f->setMouseTracking(true);
    f->installEventFilter(this);


    manijaCentral = new QLabel(this);
    manijaCentral->setObjectName("nManijaCentral");
    manijaCentral->setMouseTracking(true);
    manijaCentral->installEventFilter(this);
    manijaCentral->setGeometry(this->width()/2-25,this->height()/2-25,50,50);
    manijaCentral->setStyleSheet("background-color:transparent");
    manijaCentral->show();

    sup = new widgetResize(this);
    sup->installEventFilter(this);
    sup->setObjectName("nSup");
    connect(sup, SIGNAL(redimensionando(int,int,int)), this, SLOT(redimensionarAP(int,int,int)));
    sup->setPosicion(1);

    der = new widgetResize(this);
    der->installEventFilter(this);
    der->setObjectName("nDer");
    connect(der, SIGNAL(redimensionando(int,int,int)), this, SLOT(redimensionarAP(int,int,int)));
    der->setPosicion(2);

    inf = new widgetResize(this);
    inf->setObjectName("nInf");
    inf->installEventFilter(this);
    connect(inf, SIGNAL(redimensionando(int,int,int)), this, SLOT(redimensionarAP(int,int,int)));
    inf->setPosicion(3);

    izq = new widgetResize(this);
    izq->setObjectName("nIzq");
    izq->installEventFilter(this);
    connect(izq, SIGNAL(redimensionando(int,int,int)), this, SLOT(redimensionarAP(int,int,int)));
    izq->setPosicion(4);


    manijaCentral->setStyleSheet("background-image:url(':/imagenes/icono_mover_1.png');");
    sup->setStyleSheet("background-image:url(':/imagenes/mini_flecha_sup.png');");
    der->setStyleSheet("background-image:url(':/imagenes/mini_flecha_der.png');");
    inf->setStyleSheet("background-image:url(':/imagenes/mini_flecha_inf.png');");
    izq->setStyleSheet("background-image:url(':/imagenes/mini_flecha_izq.png');");


}

widgetSetAreaProcesamiento::~widgetSetAreaProcesamiento()
{

}

void widgetSetAreaProcesamiento::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint delta = event->globalPos() - oldPos;
    int enX= x()+delta.x();
    int enY= y()+delta.y();

    if(esMovible && enX >1 && enX < (720-this->width())-2 && enY > 1 && enY < (576-this->height())-2){
        move(x()+delta.x(), y()+delta.y());
        //qDebug()<<"Posición FAP..." << x() << " "<< y();
        emit moviendo(x(), y(), this->width(), this->height());
    }
    oldPos = event->globalPos();
}

void widgetSetAreaProcesamiento::mousePressEvent(QMouseEvent *event)
{
    const QPoint delta = event->globalPos() - oldPos;
    int enX= event->pos().x();
    int enY= event->pos().y();
    if(enX > manijaCentral->x() && enX < manijaCentral->x()+50   && enY > manijaCentral->y() && enY < manijaCentral->y()+50){
        esMovible=true;
    }else{
        esMovible=false;
    }
    oldPos = event->globalPos();
}

void widgetSetAreaProcesamiento::resizeEvent(QResizeEvent *event)
{
    manijaCentral->setGeometry(this->width()/2-25,this->height()/2-25,50,50);

    sup->setGeometry(20, 0, this->width()-40,20);
    der->setGeometry(this->width()-20, 0, 20,this->height());
    inf->setGeometry(20, this->height()-20, this->width()-40,20);
    izq->setGeometry(0, 0, 20,this->height());

}

bool widgetSetAreaProcesamiento::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type()==QEvent::MouseMove){
        // qDebug() << "in Note::eventFilter" << endl;
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(obj->objectName()=="nManijaCentral"){
            //qDebug()<<"Posición Manija..." << mouseEvent->pos().x() << " "<< mouseEvent->pos().y();
            manijaCentral->setStyleSheet("background-image:url(':/imagenes/icono_mover_1.png');");
        }else{
            manijaCentral->setStyleSheet("background-image:url(':/imagenes/icono_mover_2.png');");
        }

        sup->setStyleSheet("background-image:url(':/imagenes/mini_flecha_sup.png');");
        der->setStyleSheet("background-image:url(':/imagenes/mini_flecha_der.png');");
        inf->setStyleSheet("background-image:url(':/imagenes/mini_flecha_inf.png');");
        izq->setStyleSheet("background-image:url(':/imagenes/mini_flecha_izq.png');");
    }
    return false;
}

void widgetSetAreaProcesamiento::redimensionarAP(int pos, int sent, int pixeles)
{
    qDebug()<<"Redimensionando en Pos:"<<pos<<" sent:"<<sent;
    if(pos==1 && sent == 1){
        //move(x(), y());

        emit moviendo(x(), y()+pixeles, this->width(), this->height()-pixeles);
    }
     if(pos==1 && sent == 2){
        //move(x(), y());
        emit moviendo(x(), y()-pixeles, this->width(), this->height()+pixeles);
    }

     if(pos==2 && sent == 1){
         //move(x(), y());
         emit moviendo(x(), y(), this->width()+pixeles, this->height());
     }
      if(pos==2 && sent == 2){
         //move(x(), y());
         emit moviendo(x(), y(), this->width()-pixeles, this->height());
     }

    if(pos==3 && sent == 1){
        //move(x(), y());
        emit moviendo(x(), y(), this->width(), this->height()+pixeles);
    }
    if(pos==3 && sent == 2){
        //move(x(), y());
        emit moviendo(x(), y(), this->width(), this->height()-pixeles);
    }

    if(pos==4 && sent == 1){
        //move(x(), y());
        emit moviendo(x()+pixeles, y(), this->width()-pixeles, this->height());
    }
    if(pos==4 && sent == 2){
        //move(x(), y());
        emit moviendo(x()-pixeles, y(), this->width()+pixeles, this->height());
    }
}
