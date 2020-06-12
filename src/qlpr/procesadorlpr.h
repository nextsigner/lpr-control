#ifndef PROCESADORLPR_H
#define PROCESADORLPR_H

#include <QObject>
#include <QThread>
#include <QStringList>

#include <Windows.h>

#include "simplelpr.hpp"

using namespace SimpleLPR2_Native;

class ProcesadorLPR : public QThread
{
    Q_OBJECT
public:

    bool detenido;



signals:

public slots:
    void stop();
    void setearEstado(bool b);
    void inicializarLPR();
    QString getChapa(const QString &img);
    bool validarChapa(QString c);
    QByteArray clasificarChapas(QStringList lc);
    int contar(QString cadena, QString l);
private:
    QMutex m_mutex;
    bool m_stop;
    void run();

    HMODULE hLib;
    ISimpleLPR *pLPR;
    int nErr;

    QStringList listaDeChapasOptimas;



};

#endif // PROCESADORLPR_H
