#ifndef WIDGETSETAREAPROCESAMIENTO_H
#define WIDGETSETAREAPROCESAMIENTO_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>

#include "widgetresize.h"

class widgetSetAreaProcesamiento : public QWidget
{
    Q_OBJECT
public:
    explicit widgetSetAreaProcesamiento(QWidget *parent = 0);
    ~widgetSetAreaProcesamiento();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void resizeEvent(QResizeEvent *event);

    bool eventFilter(QObject *obj, QEvent *event);


signals:
    void moviendo(int x, int y, int an, int al);

public slots:
    void redimensionarAP(int pos, int sent, int pixeles);

private:
    bool esMovible;
    QPoint oldPos;
    QLabel *manijaCentral;

    QLabel *manijaSuperior;

    widgetResize *sup;
    widgetResize *der;
    widgetResize *inf;
    widgetResize *izq;
};

#endif // WIDGETSETAREAPROCESAMIENTO_H
