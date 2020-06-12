#include "dialogajustes.h"
#include "ui_dialogajustes.h"
#include  <QDebug>

DialogAjustes::DialogAjustes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAjustes)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    setWindowTitle("Ajustes");

    QImage sentidoIDS(":/imagenes/SentidoIDS.png");
    QImage sentidoIDB(":/imagenes/SentidoIDB.png");
    QImage sentidoDIS(":/imagenes/SentidoDIS.png");
    QImage sentidoDIB(":/imagenes/SentidoDIB.png");

    pix_sentidoIDS = QPixmap::fromImage(sentidoIDS.scaled(100, 100, Qt::IgnoreAspectRatio));
    pix_sentidoIDB = QPixmap::fromImage(sentidoIDB.scaled(100, 100, Qt::IgnoreAspectRatio));
    pix_sentidoDIS = QPixmap::fromImage(sentidoDIS.scaled(100, 100, Qt::IgnoreAspectRatio));
    pix_sentidoDIB = QPixmap::fromImage(sentidoDIB.scaled(100, 100, Qt::IgnoreAspectRatio));

    ui->radioButtonIngresoIDS->setChecked(false);
    ui->radioButtonIngresoIDB->setChecked(false);
    ui->radioButtonIngresoDIS->setChecked(false);
    ui->radioButtonIngresoDIB->setChecked(false);

    ui->radioButtonEgresoIDS->setChecked(false);
    ui->radioButtonEgresoIDB->setChecked(false);
    ui->radioButtonEgresoDIS->setChecked(false);
    ui->radioButtonEgresoDIB->setChecked(false);

    ui->labelImgIngreso->setPixmap(pix_sentidoIDS);
    ui->labelImgEgreso->setPixmap(pix_sentidoIDS);

    ui->tabWidget->setTabEnabled(0, false);
    //ui->tabWidget->set



}

DialogAjustes::~DialogAjustes()
{
    delete ui;
}

QString DialogAjustes::getVar(int v)
{
    QString res;
    if(v==1){
        res.append(QString::number(tim_pam));
    }
    if(v==2){
        res.append(QString::number(tim_capmov));
    }
    if(v==3){
        //res.append(QString::number(modo_zm));
    }
    if(v==4){
        res.append(QString::number(lpr_altcarmax));
    }
    if(v==5){
        res.append(QString::number(max_cap));
    }
    if(v==6){
        res.append(sentIngreso);
    }
    if(v==7){
        res.append(sentEgreso);
    }
    if(v==8){
        res.append(QString::number(sens_qm));
    }
    return res;
}

void DialogAjustes::setVar(int v, QString d)
{
    if(v==1){
        ui->spinBoxProcesarAM->setValue(d.toInt());
        tim_pam = d.toInt();
    }

    if(v==2){
        ui->spinBoxCapMov->setValue(d.toInt());
        ui->labelFCPS->setText(QString::number(1000/d.toInt()));
        tim_capmov = d.toInt();
    }

    if(v==3){
        /*if(d.toInt()==1){
            ui->radioButtonPorControl->setChecked(true);
        }else{
            ui->radioButtonPorCamara->setChecked(true);
        }
        modo_zm = d.toInt();*/
    }
    if(v==4){
        ui->spinBoxAltMaxCaracter->setValue(d.toInt());
        lpr_altcarmax = d.toInt();
    }
    if(v==5){
        ui->spinBoxMaxCantCap->setValue(d.toInt());
        max_cap = d.toInt();
    }
    if(v==6){
         sentIngreso = d;
         if(sentIngreso=="IDS"){
             ui->radioButtonIngresoIDS->setChecked(true);
         }
         if(sentIngreso=="IDB"){
             ui->radioButtonIngresoIDB->setChecked(true);
         }
         if(sentIngreso=="DIS"){
             ui->radioButtonIngresoDIS->setChecked(true);
         }
         if(sentIngreso=="DIB"){
             ui->radioButtonIngresoDIB->setChecked(true);
         }
    }
    if(v==7){
         sentEgreso = d;
         if(sentEgreso=="IDS"){
             ui->radioButtonEgresoIDS->setChecked(true);
         }
         if(sentEgreso=="IDB"){
             ui->radioButtonEgresoIDB->setChecked(true);
         }
         if(sentEgreso=="DIS"){
             ui->radioButtonEgresoDIS->setChecked(true);
         }
         if(sentEgreso=="DIB"){
             ui->radioButtonEgresoDIB->setChecked(true);
         }
    }
     if(v==8){
         ui->doubleSpinBox->setValue(d.toDouble());
     }
    qDebug()<<"Seteando valor de Ajustes: valor "<<v<<"="<<d;
}

void DialogAjustes::on_pushButtonListo_clicked()
{
    this->close();
}

void DialogAjustes::on_spinBoxProcesarAM_valueChanged(int arg1)
{
    tim_pam = arg1;
    emit modificando(1);
}

void DialogAjustes::on_spinBoxCapMov_valueChanged(int arg1)
{
    tim_capmov = arg1;
    ui->labelFCPS->setText(QString::number(1000/arg1));
    emit modificando(2);
}





void DialogAjustes::on_spinBoxAltMaxCaracter_valueChanged(int arg1)
{
    lpr_altcarmax = arg1;
    emit modificando(4);
}

void DialogAjustes::on_spinBoxMaxCantCap_valueChanged(int arg1)
{
    max_cap = arg1;
    emit modificando(5);
}

//INGRESOS

void DialogAjustes::on_radioButtonIngresoIDS_toggled(bool checked)
{
    if(checked){
       ui->labelImgIngreso->setPixmap(pix_sentidoIDS);
       ui->radioButtonEgresoIDS->setEnabled(false);
       sentIngreso="IDS";
       emit modificando(6);
    }else{
        ui->radioButtonEgresoIDS->setEnabled(true);
    }
}

void DialogAjustes::on_radioButtonIngresoIDB_toggled(bool checked)
{
    if(checked){
       ui->labelImgIngreso->setPixmap(pix_sentidoIDB);
       ui->radioButtonEgresoIDB->setEnabled(false);
       sentIngreso="IDB";
       emit modificando(6);
    }else{
        ui->radioButtonEgresoIDB->setEnabled(true);
    }
}

void DialogAjustes::on_radioButtonIngresoDIS_toggled(bool checked)
{
    if(checked){
       ui->labelImgIngreso->setPixmap(pix_sentidoDIS);
       ui->radioButtonEgresoDIS->setEnabled(false);
       sentIngreso="DIS";
       emit modificando(6);
    }else{
        ui->radioButtonEgresoDIS->setEnabled(true);
    }
}

void DialogAjustes::on_radioButtonIngresoDIB_toggled(bool checked)
{
    if(checked){
       ui->labelImgIngreso->setPixmap(pix_sentidoDIB);
       ui->radioButtonEgresoDIB->setEnabled(false);
       sentIngreso="DIB";
       emit modificando(6);
    }else{
        ui->radioButtonEgresoDIB->setEnabled(true);
    }
}

//EGRESOS

void DialogAjustes::on_radioButtonEgresoIDS_toggled(bool checked)
{
    if(checked){
       ui->labelImgEgreso->setPixmap(pix_sentidoIDS);
       ui->radioButtonIngresoIDS->setEnabled(false);
       sentEgreso="IDS";
       emit modificando(7);
    }else{
        ui->radioButtonIngresoIDS->setEnabled(true);
    }
}

void DialogAjustes::on_radioButtonEgresoIDB_toggled(bool checked)
{
    if(checked){
       ui->labelImgEgreso->setPixmap(pix_sentidoIDB);
       ui->radioButtonIngresoIDB->setEnabled(false);
       sentEgreso="IDB";
       emit modificando(7);
    }else{
        ui->radioButtonIngresoIDB->setEnabled(true);
    }
}

void DialogAjustes::on_radioButtonEgresoDIS_toggled(bool checked)
{
    if(checked){
       ui->labelImgEgreso->setPixmap(pix_sentidoDIS);
       ui->radioButtonIngresoDIS->setEnabled(false);
       sentEgreso="DIS";
       emit modificando(7);
    }else{
        ui->radioButtonIngresoDIS->setEnabled(true);
    }
}

void DialogAjustes::on_radioButtonEgresoDIB_toggled(bool checked)
{
    if(checked){
       ui->labelImgEgreso->setPixmap(pix_sentidoDIB);
       ui->radioButtonIngresoDIB->setEnabled(false);
       sentEgreso="DIB";
       emit modificando(7);
    }else{
        ui->radioButtonIngresoDIB->setEnabled(true);
    }
}

void DialogAjustes::on_doubleSpinBox_valueChanged(double arg1)
{
    sens_qm = arg1;
    emit modificando(8);
}
