#ifndef GALERIADECAPTURAS_H
#define GALERIADECAPTURAS_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QDir>
#include <QHBoxLayout>

class GaleriaDeCapturas : public QWidget
{
    Q_OBJECT
public:
    explicit GaleriaDeCapturas(QWidget *parent = 0);

signals:

public slots:

    void reproducir(QString ev, int cant);
    void setearCarpetaEvento(QString cev);
    void nextFotograma();
    void prevFotograma();
    void setearEstadoCinematica(bool b);
    void setearVelocidad(int v);
private:
    QLabel *img;
    QLabel *infoimg;
    QTimer *timerPlay;
    QStringList imagenes;
    //QImage fotograma;
    int numcap;
    QDir dirTemp;
    QString carpEvento;
    QList <QImage> listaFotogramas;
    int velocidad;
};

#endif // GALERIADECAPTURAS_H
