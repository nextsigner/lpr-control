#ifndef CONFIG_H
#define CONFIG_H
#include "definiciones.h"
#include <QObject>
#include <QtXml/QDomDocument>
#include <QXmlStreamWriter>
#include <QDir>
#include <QDebug>

class config : public QObject
{
    Q_OBJECT
public:
    explicit config(QObject *parent = 0);
    ~config();

    int maxMensajes;
signals:

public slots:
    void setAppPath(QString ap);
    bool cargarConfiguracion();
    QString getDatoConfig(QString n);
    void setDatoConfig(QString n, QString v);
private:
    QDomDocument *xmlCONFIG;
    QString appPath;


};

#endif // CONFIG_H
/*#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

class config : public QObject
{
    Q_OBJECT
public:
    explicit config(QObject *parent = 0);
    ~config();

signals:

public slots:
};

#endif // CONFIG_H*/
