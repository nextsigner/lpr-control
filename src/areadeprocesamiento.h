#ifndef AREADEPROCESAMIENTO_H
#define AREADEPROCESAMIENTO_H

#include <QWidget>
#include <QLabel>

#include "widgetsetareaprocesamiento.h"

class AreaDeProcesamiento : public QWidget
{
    Q_OBJECT
public:
    explicit AreaDeProcesamiento(QWidget *parent = 0);
    ~AreaDeProcesamiento();
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void seteandoArea(int px, int py, int an, int al);
public slots:
    void setArea(int px, int py, int an, int al);
    //void setSentidoDetectado(QString sentido);
    void mover(int x, int y);
private:
    widgetSetAreaProcesamiento *wa;
    //QWidget *wa;
    QWidget *ws;
    QWidget *wd;
    QWidget *wi;
    QWidget *win;

    QLabel *sent;
};

#endif // AREADEPROCESAMIENTO_H
