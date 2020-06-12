#include "widgetresize.h"
#include <QDebug>


widgetResize::widgetResize(QWidget *parent) : QWidget(parent)
{

    posicion = 0;

    QFrame *f = new QFrame(this);
    f->setGeometry(0,0,720,480);
    setGeometry(0,0,720,480);
    f->setStyleSheet("background-color:transparent;");
    f->setMouseTracking(true);
    f->installEventFilter(this);
}

widgetResize::~widgetResize()
{

}

void widgetResize::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint delta = event->globalPos() - oldPos;

    if(posicion==1){
        int dif = event->globalPos().y()-oldPos.y();
        if(dif < 0){
            qDebug()<<"Subiendo...";
            //move(10, y()+delta.y());
            emit redimensionando(1, 1, delta.y());
            oldPos =  QPoint(10, event->globalPos().y());
        }else{
            qDebug()<<"Bajando..";
            //move(10, y()+delta.y());
            emit redimensionando(1, 2, 0-(delta.y()));
            oldPos =  QPoint(10, event->globalPos().y());
        }
    }

    if(posicion==2){
        int dif = event->globalPos().x()-oldPos.x();
        if(dif > 0){
            qDebug()<<"Resize hacia la derecha derecha...";
            //move(10, y()+delta.y());
            emit redimensionando(2, 1, delta.x());
            oldPos =  QPoint(event->globalPos().x(), 0);
        }else{
            qDebug()<<"Resize hacia la derecha izquierda...";
            //move(10, y());
            emit redimensionando(2, 2, 0-(delta.x()));
            oldPos =  QPoint(event->globalPos().x(), 0);
        }
    }

    if(posicion==3){
        int dif = event->globalPos().y()-oldPos.y();
        if(dif > 0){
            qDebug()<<"Subiendo...";
            //move(10, y()+delta.y());
            emit redimensionando(3, 1, delta.y());
            oldPos =  QPoint(10, event->globalPos().y());
        }else{
            qDebug()<<"Bajando..";
            //move(10, y());
            emit redimensionando(3, 2, 0-(delta.y()));
            oldPos =  QPoint(10, event->globalPos().y());
        }
    }

    if(posicion==4){
        int dif = event->globalPos().x()-oldPos.x();
        if(dif > 0){
            qDebug()<<"Resize hacia la izquierda  derecha...";
            //move(10, y()+delta.y());
            emit redimensionando(4, 1, delta.x());
            oldPos =  QPoint( event->globalPos().x(), 0);
        }else{
            qDebug()<<"Resize hacia la izquierda  izquierda...";
            //move(10, y());
            emit redimensionando(4, 2, 0-(delta.x()));
            oldPos =  QPoint(event->globalPos().x(), 0);
        }
    }
}

void widgetResize::mousePressEvent(QMouseEvent *event)
{
     if(posicion==1){
        oldPos = QPoint(10, event->globalPos().y());
     }

     if(posicion==2){
        oldPos = QPoint(event->globalPos().x(), 0);
     }

     if(posicion==3){
        oldPos = QPoint(10, event->globalPos().y());
     }

     if(posicion==4){
        oldPos = QPoint(event->globalPos().x(), 0);
     }
}

bool widgetResize::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type()==QEvent::MouseMove){
        if(posicion==1){
            this->setStyleSheet("background-image:url(':/imagenes/mini_flecha_sup_2.png');");
        }
        if(posicion==2){
            this->setStyleSheet("background-image:url(':/imagenes/mini_flecha_der_2.png');");
        }
        if(posicion==3){
            this->setStyleSheet("background-image:url(':/imagenes/mini_flecha_inf_2.png');");
        }
        if(posicion==4){
            this->setStyleSheet("background-image:url(':/imagenes/mini_flecha_izq_2.png');");
        }
    }
    return false;
}

void widgetResize::setPosicion(int p)
{
    posicion = p;
}

