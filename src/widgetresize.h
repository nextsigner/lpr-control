#ifndef WIDGETRESIZE_H
#define WIDGETRESIZE_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class widgetResize : public QWidget
{
    Q_OBJECT
public:
    explicit widgetResize(QWidget *parent = 0);
    ~widgetResize();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void redimensionando(int posicion, int sent, int pixeles);

public slots:
    void setPosicion(int p);

private:
    QLabel *fondo;
    QPoint oldPos;

    int posicion;
};

#endif // WIDGETRESIZE_H
