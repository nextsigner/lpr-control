#ifndef ADMINISTRADORDEARCHIVOS_H
#define ADMINISTRADORDEARCHIVOS_H

#include <QObject>
#include <QThread>
#include <QDir>
#include <QDirIterator>
#include <QDebug>

class AdministradorDeArchivos : public QThread
{
    Q_OBJECT
public:
    AdministradorDeArchivos(QObject*);
    void run();

signals:

public slots:
    QStringList listaDeRutas(QString modo, QString raiz);
};

#endif // ADMINISTRADORDEARCHIVOS_H
