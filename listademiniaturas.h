#ifndef LISTADEMINIATURAS_H
#define LISTADEMINIATURAS_H

#include <QObject>
#include <QList>
#include <QImage>

class ListaDeMiniaturas : public QObject
{
    Q_OBJECT
public:
    explicit ListaDeMiniaturas(QObject *parent = 0);
    ~ListaDeMiniaturas();
    int cantidades();
    int cant;


signals:

public slots:
    //void eliminar();
    void insertarImagen(const QImage &img);


    QImage getImagen(int n);


private:
    QList<QImage> *lista;
};

#endif // LISTADEMINIATURAS_H
