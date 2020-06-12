#ifndef DIALOGAJUSTES_H
#define DIALOGAJUSTES_H

#include <QDialog>

namespace Ui {
class DialogAjustes;
}

class DialogAjustes : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAjustes(QWidget *parent = 0);
    ~DialogAjustes();

public slots:
    QString getVar(int v);
    void setVar(int v, QString d);

private slots:
    void on_pushButtonListo_clicked();
    void on_spinBoxProcesarAM_valueChanged(int arg1);

    void on_spinBoxCapMov_valueChanged(int arg1);

    void on_spinBoxAltMaxCaracter_valueChanged(int arg1);

    void on_spinBoxMaxCantCap_valueChanged(int arg1);

    void on_radioButtonIngresoIDS_toggled(bool checked);

    void on_radioButtonIngresoIDB_toggled(bool checked);

    void on_radioButtonIngresoDIS_toggled(bool checked);

    void on_radioButtonIngresoDIB_toggled(bool checked);

    void on_radioButtonEgresoDIS_toggled(bool checked);

    void on_radioButtonEgresoDIB_toggled(bool checked);

    void on_radioButtonEgresoIDS_toggled(bool checked);

    void on_radioButtonEgresoIDB_toggled(bool checked);

    void on_doubleSpinBox_valueChanged(double arg1);

signals:
    void modificando(int);
private:
    Ui::DialogAjustes *ui;

    //Varialbles Generales
    int tim_pam;
    int tim_capmov;
    int lpr_altcarmax;
    int max_cap;
    QString sentIngreso;
    QString sentEgreso;
    double sens_qm;

    QPixmap pix_sentidoIDS;
    QPixmap pix_sentidoIDB;
    QPixmap pix_sentidoDIS;
    QPixmap pix_sentidoDIB;

};

#endif // DIALOGAJUSTES_H
