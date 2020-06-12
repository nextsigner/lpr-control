#ifndef PANTALLA_H
#define PANTALLA_H

#include <QWidget>

class Pantalla : public QWidget
{
    Q_OBJECT
public:
    explicit Pantalla(QWidget *parent = 0);
    ~Pantalla();
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void setFotograma(const QImage &img);
private:

    QImage fotograma;
};

#endif // PANTALLA_H
