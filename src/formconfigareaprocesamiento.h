#ifndef FORMCONFIGAREAPROCESAMIENTO_H
#define FORMCONFIGAREAPROCESAMIENTO_H

#include <QWidget>

namespace Ui {
class FormConfigAreaProcesamiento;
}

class FormConfigAreaProcesamiento : public QWidget
{
    Q_OBJECT

public:
    explicit FormConfigAreaProcesamiento(QWidget *parent = 0);
    ~FormConfigAreaProcesamiento();
    void closeEvent(QCloseEvent *event);

public slots:
    void setValores(int px, int py, int an, int al);

private slots:
    void on_spinBoxPosX_valueChanged(int arg1);

    void on_spinBoxPosY_valueChanged(int arg1);

    void on_spinBoxAncho_valueChanged(int arg1);

    void on_spinBoxAlto_valueChanged(int arg1);

    void on_pushButtonListo_clicked();
signals:
    void modificando(int ix, int fx, int iy, int fy);
    void cerrando();

private:
    Ui::FormConfigAreaProcesamiento *ui;
};

#endif // FORMCONFIGAREAPROCESAMIENTO_H
