#include "rejilla.h"
#include <QFrame>

rejilla::rejilla(QWidget *parent) : QWidget(parent)
{
    panel = new QWidget(this);
    panel->setStyleSheet("background-color:transparent; border-style:none;");
    QFrame *f = new QFrame(panel);
    f->setGeometry(0,0,720,576);

    //setGeometry(0,0,720,576);

}

rejilla::~rejilla()
{

}

void rejilla::setearRejilla(int ancho, int alto, int espacio)
{

    int an= espacio;

    while (an<alto) {
        QLabel *lineaH = new QLabel(this);
        lineaH->setGeometry(0,an,ancho, 1);
        lineaH->setStyleSheet("background-color:#ffff00;");
        lineaH->show();

        an = an+espacio;
    }

}

