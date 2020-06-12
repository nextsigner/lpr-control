#include "listademiniaturas.h"

#include <QPainter>
#include <QDebug>

ListaDeMiniaturas::ListaDeMiniaturas(QObject *parent) : QObject(parent)
{
    lista = new QList<QImage>;

}

ListaDeMiniaturas::~ListaDeMiniaturas()
{
    qDebug()<<"Eliminado --------------  "<<this->objectName();
    //delete this;
    lista->clear();
    delete lista;
}



void ListaDeMiniaturas::insertarImagen(const QImage &img)
{
    //QImage image(img.size(), QImage::Format_ARGB32_Premultiplied);
        //QPainter imagePainter(&image);
        //imagePainter.initFrom(this);
        //imagePainter.setRenderHint(QPainter::Antialiasing, true);
        //imagePainter.eraseRect(rect());
        //draw(&imagePainter);
        //imagePainter.end();

        //QPainter widgetPainter(this);
        //widgetPainter.drawImage(0, 0, image);

    //QImage ni =    img.convertToFormat(QImage::Format_RGBA8888);

    lista->append(img);
}

int ListaDeMiniaturas::cantidades()
{
    return lista->size();
}

QImage ListaDeMiniaturas::getImagen(int n)
{
    return lista->at(n);
}
