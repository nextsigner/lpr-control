#include "formconfigareaprocesamiento.h"
#include "ui_formconfigareaprocesamiento.h"

#include <QDebug>

FormConfigAreaProcesamiento::FormConfigAreaProcesamiento(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormConfigAreaProcesamiento)
{
    ui->setupUi(this);
    setWindowTitle("Configurar rectángulo");
    ui->spinBoxPosX->setFocus();
}

FormConfigAreaProcesamiento::~FormConfigAreaProcesamiento()
{
    delete ui;
}

void FormConfigAreaProcesamiento::closeEvent(QCloseEvent *event)
{
    emit cerrando();
}

void FormConfigAreaProcesamiento::setValores(int px, int py, int an, int al)
{
    qDebug()<<"Seteando valores FAP..." << px << " "<< py << " "<< an << " "<< al << " ------";
    ui->spinBoxPosX->setValue(px);
    ui->spinBoxPosY->setValue(py);
    ui->spinBoxAncho->setValue(an);
    ui->spinBoxAlto->setValue(al);
}

void FormConfigAreaProcesamiento::on_spinBoxPosX_valueChanged(int arg1)
{
    emit modificando(ui->spinBoxPosX->value(), ui->spinBoxPosY->value(), ui->spinBoxAncho->value(), ui->spinBoxAlto->value());
}

void FormConfigAreaProcesamiento::on_spinBoxPosY_valueChanged(int arg1)
{
    emit modificando(ui->spinBoxPosX->value(), ui->spinBoxPosY->value(), ui->spinBoxAncho->value(), ui->spinBoxAlto->value());
}

void FormConfigAreaProcesamiento::on_spinBoxAncho_valueChanged(int arg1)
{
    emit modificando(ui->spinBoxPosX->value(), ui->spinBoxPosY->value(), ui->spinBoxAncho->value(), ui->spinBoxAlto->value());
}

void FormConfigAreaProcesamiento::on_spinBoxAlto_valueChanged(int arg1)
{
    emit modificando(ui->spinBoxPosX->value(), ui->spinBoxPosY->value(), ui->spinBoxAncho->value(), ui->spinBoxAlto->value());
}


void FormConfigAreaProcesamiento::on_pushButtonListo_clicked()
{
    emit cerrando();
    this->close();
}

