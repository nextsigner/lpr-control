#include "pantalla.h"
#include <QPainter>

Pantalla::Pantalla(QWidget *parent) : QWidget(parent)
{
    //fotograma=new QImage(720,576,QImage::Format_ARGB32);
}

Pantalla::~Pantalla()
{

}

void Pantalla::paintEvent(QPaintEvent *event)
{
   QPainter painter(this);
   painter.drawImage(QPoint(0,0), fotograma) ;
}

void Pantalla::setFotograma(const QImage &img)
{
    fotograma=img;
    update();
}

