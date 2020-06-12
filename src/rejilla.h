#ifndef REJILLA_H
#define REJILLA_H

#include <QWidget>
#include <QLabel>

class rejilla : public QWidget
{
    Q_OBJECT
public:
    explicit rejilla(QWidget *parent = 0);
    ~rejilla();

signals:

public slots:
    void setearRejilla(int ancho, int alto, int espacio);
private:
    QWidget *panel;
};

#endif // REJILLA_H
