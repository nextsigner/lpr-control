#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "definiciones.h"


#include <QCameraInfo>
#include <QtWidgets>

#include <tchar.h>
#include <windows.h>

Q_DECLARE_METATYPE(QCameraInfo)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    variablesInicializadas(false),
    incrementando(false),
    movimiento(false),
    segmentoIniciado(false),
    unidadTiempo(0),
    tiempoInactivo(0),
    procesandoEvento(false),
    isCapturingImage(false)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/imagenes/logo.png"));

    procesandoFotograma = false;
    calculandoDiferencia=false;

    ejecutarQLpr = new QProcess(this);
    ejecutarQLpr->close();




    //INICIO OCULTANDO COMPONENTES
    //ui->mainToolBar->actionAt(3)->setVisible(false);

    //FIN OCULTANDO COMPONENTES

    ada = new AdministradorDeArchivos(this);

    /*QString addin_path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dirAppData(addin_path);
    if (!dirAppData.exists())
        dirAppData.mkpath(addin_path);
    if (!dirAppData.exists("control"))
        dirAppData.mkdir("control");

    dirAppData.cd("control");
    qDebug()<<"Carpeta AppData 1:"<<dirAppData.currentPath();*/

    appPath.append(QDir::currentPath());
    if(QDir::currentPath()!=DIR_RUN){
         currentCT = QDir::tempPath();
         qDebug()<<"Carpeta Temporal C:"<<currentCT;
    }else{
        currentCT = "E:/tempD";
    }
    desdeCarpeta = currentCT;
    currentCT.append("/sesion_");
    currentCT.append(QDate::currentDate().toString("dd_MM_yy"));
    currentCT.append("_");
    currentCT.append(QDateTime::currentDateTime().toString("hh_mm_ss"));
    qDebug()<<"Carpeta Temporal:"<<currentCT;

    QDir dir(currentCT);
    if (!dir.exists()){
      dir.mkdir(currentCT);
    }
    //ui->lineEditCarpetaDeTrabajo->setText(currentCT);
    logArch.append(currentCT);
    logArch.append("/log.txt");
    conf = new config(this);
    qDebug()<<"Carpeta AppData:"<<appPath;
    conf->setAppPath(appPath);
    if(!conf->cargarConfiguracion()){
        conf->cargarConfiguracion();
    }

    setAppVersion(conf->getDatoConfig("app_version"));



    //Inicializando Variables Generales
    reg_px = conf->getDatoConfig("reg_px").toInt();
    reg_py = conf->getDatoConfig("reg_py").toInt();
    reg_an = conf->getDatoConfig("reg_an").toInt();
    reg_al = conf->getDatoConfig("reg_al").toInt();

    am_px = conf->getDatoConfig("am_px").toInt();
    am_py = conf->getDatoConfig("am_py").toInt();
    am_an = conf->getDatoConfig("am_an").toInt();
    am_al = conf->getDatoConfig("am_al").toInt();

    ui->spinBoxAR_PX->setValue(reg_px);
    ui->spinBoxAR_PY->setValue(reg_py);
    ui->spinBoxAR_AN->setValue(reg_an);
    ui->spinBoxAR_AL->setValue(reg_al);

    ui->spinBoxAM_PX->setValue(am_px);
    ui->spinBoxAM_PY->setValue(am_py);
    ui->spinBoxAM_AN->setValue(am_an);
    ui->spinBoxAM_AL->setValue(am_al);

    numevento = 1;
    cantcap = 0;
    colorLetra = 0;


    ver_tabevent = conf->getDatoConfig("ver_tabevent");

    tim_pam = conf->getDatoConfig("tim_pam").toInt();
    tim_capmov = conf->getDatoConfig("tim_capmov").toInt();
    lpr_altcarmax = conf->getDatoConfig("lpr_altcarmax").toInt();
    max_cap = conf->getDatoConfig("max_cap").toInt();

    if(ver_tabevent=="1"){
        ui->tableWidgetCola->show();
        ui->menuVer->actions().at(0)->setChecked(true);
    }else{
        ui->tableWidgetCola->hide();
        ui->menuVer->actions().at(0)->setChecked(false);
    }


    sentIngreso = conf->getDatoConfig("sentIngreso");
    sentEgreso = conf->getDatoConfig("sentEgreso");

    sens_qm = conf->getDatoConfig("sens_qm").toDouble();
    qDebug()<<"Sensibilidad de Movimiento seteada en "<< sens_qm;

    //FIN INICIALIZANDO VARIABLES GENERALES

    ui->labelChapa->setText("--- ---");


    ui->tableWidgetCola->setColumnWidth(0,30);
    ui->tableWidgetCola->setColumnWidth(1,70);
    ui->tableWidgetCola->setColumnWidth(2,70);
    ui->tableWidgetProcesadas->setColumnWidth(0, 120);
    ui->tableWidgetProcesadas->setColumnWidth(1, 150);
    //ui->tableWidgetProcesadas->setColumnWidth(2, 100);
    ui->tableWidgetProcesadas->horizontalHeader()->setVisible(true);
    ui->tableWidgetProcesadas->setColumnHidden(2, true);
    ui->tableWidgetProcesadas->setColumnHidden(3, true);
    //ui->tableWidgetProcesadas->setFocusPolicy(Qt::NoFocus);


    //qDebug()<<"Accion 1111: "<<numcap;
    videoWidget = new VideoWidget(ui->widgetVideo);
    videoWidget->setMaximumWidth(QString::number(ANCHO_VIDEO).toInt());
    videoWidget->setMaximumHeight(QString::number(ALTO_VIDEO).toInt());
    ui->widgetVideo->setMaximumWidth(QString::number(ANCHO_VIDEO).toInt());
    ui->widgetVideo->setMaximumHeight(QString::number(ALTO_VIDEO).toInt());
    //qDebug()<<"Accion 2: "<<numcap;



    QActionGroup *videoDevicesGroup = new QActionGroup(this);
    videoDevicesGroup->setExclusive(true);
    foreach (const QCameraInfo &cameraInfo, QCameraInfo::availableCameras()) {
        QAction *videoDeviceAction = new QAction(cameraInfo.description(), videoDevicesGroup);
        connect(videoDeviceAction, SIGNAL(triggered()), this, SLOT(sobre_action_menucamaras_triggered()));
        videoDeviceAction->setCheckable(true);
        videoDeviceAction->setData(QVariant::fromValue(cameraInfo));
        if (cameraInfo == QCameraInfo::defaultCamera())
            videoDeviceAction->setChecked(true);

        ui->menuCamaras->addAction(videoDeviceAction);
    }

    connect(videoDevicesGroup, SIGNAL(triggered(QAction*)), SLOT(updateCameraDevice(QAction*)));

    camaraPresente=false;

    for(int i=0;i<QCameraInfo::availableCameras().size();i++){
        if(QCameraInfo::availableCameras().at(i).description()==QString(CAMARA_POR_DEFECTO)){
            setCamera(QCameraInfo::availableCameras().at(i));
            ui->menuCamaras->actions().at(i)->setChecked(true);
            camaraPresente=true;
        }
    }

    if(!camaraPresente){

    }
    //qDebug()<<"Accion 3: "<<numcap;


    QString miSound1 = "recursos/sonido.wav";
    sonido = new QSound(miSound1);

    QString miSound2 = "recursos/beep-02.wav";
    sonido2 = new QSound(miSound2);


    //timerRevisarCola = new QTimer(this);
    //connect(timerRevisarCola,SIGNAL(timeout()), this, SLOT(revisarCola()));

    timerSuma = new QTimer(this);
    connect(timerSuma,SIGNAL(timeout()), this, SLOT(sumaTiempo()));

    timerProcesarCola = new QTimer(this);
    connect(timerProcesarCola,SIGNAL(timeout()), this, SLOT(procesarCola()));
    timerProcesarCola->start(1000);


    recorteanterior = QImage(QSize(am_an, am_al), QImage::Format_ARGB32);
    recorteactual = QImage(QSize(am_an, am_al), QImage::Format_ARGB32);
    recorteactualAR = QImage(QSize(reg_an, reg_al), QImage::Format_ARGB32);
    contornoAR = new QWidget(ui->widgetPanelDerecho);
    contornoAR->setStyleSheet(""
                              "border-style:solid;"
                              "border-width:4px;"
                              "border-color:#81F7F3;"
                              "background-color:transparent;"
                      "");
    contornoAR->show();

    contornoAM = new QWidget(ui->widgetPanelDerecho);
    contornoAM->setStyleSheet(""
                              "border-style:solid;"
                              "border-width:2px;"
                              "border-color:#ffff00;"
                              "background-color:transparent;"
                      "");
    contornoAM->show();

    contornosChapas = new QWidget(contornoAM);
    contornosChapas->setStyleSheet(""
                              "border-style:solid;"
                              "border-width:1px;"
                              "border-color:#66ff33;"
                              "background-color:transparent;"
                      "");

    titContornoAR = new QLabel(contornoAR);
    titContornoAR->setGeometry(0,0,25,20);
    titContornoAR->setText("AR");
    titContornoAR->setStyleSheet("color:#000; background-color:#81F7F3;");
    titContornoAR->show();

    titContornoAM = new QLabel(contornoAM);
    titContornoAM->setGeometry(0,0,25,20);
    titContornoAM->setText("AM");
    titContornoAM->setStyleSheet("color:#000; background-color:#ffff00;");
    titContornoAM->show();






    contornosChapas->show();
    setearContorno(reg_px, reg_py, reg_an, reg_al);
    setearContornoAM(am_px, am_py, am_an, am_al);


    ap = new AreaDeProcesamiento(ui->widgetPanelDerecho);
    ap->hide();

    ui->tabWidget->setCurrentIndex(0);

    /*chapaIdeal = new QWidget(ui->widgetPanelDerecho);
    QFrame *fchapaIdeal = new QFrame(chapaIdeal);
    fchapaIdeal->setGeometry(0,0,124,60);
    chapaIdeal->setGeometry(0,0,124,60);
    chapaIdeal->setStyleSheet(""
                       "background-image:url(':/imagenes/chapa_ideal_1.png');"
                       "");
    chapaIdeal->hide();*/


    titSentDefIngreso = new QLabel(ui->widgetPanelDerecho);
    titSentDefIngreso->setGeometry(670,0,50,20);
    titSentDefIngreso->setText("Ingreso");
    titSentDefIngreso->setStyleSheet("color:#ffffff; background-color:#000;");
    titSentDefIngreso->hide();
    sentDefIngreso = new QLabel(ui->widgetPanelDerecho);
    sentDefIngreso->setGeometry(670,20,50,50);
    sentDefIngreso->hide();

    titSentDefEgreso = new QLabel(ui->widgetPanelDerecho);
    titSentDefEgreso->setGeometry(670,75,50,20);
    titSentDefEgreso->setText("Egreso");
    titSentDefEgreso->setStyleSheet("color:#ffffff; background-color:#000;");
    titSentDefEgreso->hide();
    sentDefEgreso = new QLabel(ui->widgetPanelDerecho);
    sentDefEgreso->setGeometry(670,95,50,50);
    sentDefEgreso->hide();

    //setearSentidoEnPantalla();

    ui->widgetVideo->setStyleSheet(""
                       "background-image:url(':/imagenes/fondo_sin_camara.png');"
                       "");

    /*punto = new QWidget(ui->widgetPanelDerecho);
    QFrame *fpunto = new QFrame(punto);
    fpunto->setGeometry(0,0,35,35);
    punto->setGeometry(0,0,35,35);
    punto->setStyleSheet(""
                       //"background-image:url(20.jpg);"
                     "background-color:#33ff00;"
                      "");
    punto->show();*/



    //ui->progressBarEventos->lower();
    ui->widgetPanelDerecho->raise();

    //pbe = new QProgressBar(this);
    /*pbe->show();
    pbe->setGeometry(0,0,150,30);*/


    sent = new QLabel(ui->widgetPanelDerecho);
    sent->setGeometry(0,0,100,100);
    //sent->show();
    sent->hide();


    timerProcesarFotograma =  new QTimer(this);
    connect(timerProcesarFotograma, SIGNAL(timeout()), this, SLOT(procesarFotograma()));

    showMaximized();//Ejecutar al final para evitar conflictos con qcamera.
    variablesInicializadas=true;



    //aplicarModoDesarrollo();

}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{


    if(event->key() == Qt::Key_Up){

    }
    if(event->key() == Qt::Key_Right){

    }
    if(event->key() == Qt::Key_Down){

    }
    if(event->key() == Qt::Key_Left){

    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type()==QEvent::MouseMove){
           // qDebug() << "in Note::eventFilter" << endl;

        //ui->labelChapa->setText("Moviendo!");
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            qDebug()<<"Posición Manija..." << mouseEvent->pos().x() << " "<< mouseEvent->pos().y();
            /*if(mouseEvent->pos().x()>(this->width()/2)-25){
                manijaCentral->setStyleSheet("background-color:#00ffcc;");
            }else{
                manijaCentral->setStyleSheet("background-color:#ffffcc;");
            }*/
            /*if (target->objectName()=="grip")
            {
                lastClicked="grip";
                qDebug() << "lastClicked == grip" << endl;
            }
            else
                qDebug() << "grip was not detected" << endl;*/
         }
    return false;
}

void MainWindow::setearModoDesarrollo(bool b)
{
    modoDesarrollo=b;
}

void MainWindow::aplicarModoDesarrollo()
{

        //DESHABILITAR PARA PROBAR REPRODUCTOR DE DVD
        //<----
        if(QDir::currentPath()==QString(DIR_RUN)){
            currentCT="E:/sesion_01_05_15_16_25_17";

            //int cantDir=1;
            QStringList listaCarpetaEventos= ada->listaDeRutas("carpetas", currentCT);
            //QStringList files;
            qDebug()<<"Cantidad de Eventos: "<<listaCarpetaEventos.size();
            for (int i =0; i < listaCarpetaEventos.size(); i++){
                QString rutaACarpetaEvento;
                rutaACarpetaEvento.append(currentCT);
                rutaACarpetaEvento.append("/evento_");
                rutaACarpetaEvento.append(QString::number(i+1));
                rutaACarpetaEvento.append("/");
                //files.clear();
                int cantidadDeCapturas = ada->listaDeRutas("archivos", rutaACarpetaEvento).size();
                QByteArray dato;
                dato.append(QString::number(i+1));
                QByteArray dato2;
                dato2.append(QString::number(cantidadDeCapturas-1));

                if(cantidadDeCapturas>0){
                int cantFilas = ui->tableWidgetCola->rowCount();
                ui->tableWidgetCola->insertRow(cantFilas);
                ui->tableWidgetCola->setItem(cantFilas,0, new QTableWidgetItem(dato.constData()));
                ui->tableWidgetCola->setItem(cantFilas,1, new QTableWidgetItem("00:00:01"));
                ui->tableWidgetCola->setItem(cantFilas,2, new QTableWidgetItem("00:00:05"));
                ui->tableWidgetCola->setItem(cantFilas,3, new QTableWidgetItem(dato2.constData()));


                }
            }

        }

    ui->tableWidgetProcesadas->setColumnHidden(2, false);
    ui->tableWidgetProcesadas->setColumnHidden(3, false);
    setWindowTitle("Control en Modo Depuración");
}

void MainWindow::appendLog(QString fila)
{

    QString nf;
    nf.append(fila);
    nf.append("\r");
    nf.append("\n");

    QString contAnt;
    //QString contAct;

    QFile log(logArch);

    if(log.open(QIODevice::ReadOnly)){
        QTextStream leer(&log);
        contAnt.append(leer.readAll());
        log.close();
    }

    if(log.open(QIODevice::WriteOnly)){
        QTextStream escribir(&log);
        escribir<<contAnt;
        escribir<<nf;
        log.close();
    }
}

void MainWindow::regrabarConfig(int v)
{
    if(v==1){
        conf->setDatoConfig("tim_pam", daj->getVar(1));
        tim_pam = daj->getVar(1).toInt();

    }
    if(v==2){
        conf->setDatoConfig("tim_capmov", daj->getVar(2));
        tim_capmov = daj->getVar(2).toInt();
    }
    if(v==3){
        //qDebug()<<"informacion 444"<<" -444";
        //conf->setDatoConfig("modo_zm", daj->getVar(3));
        //modo_zm = daj->getVar(3).toInt();
    }
    if(v==4){
        conf->setDatoConfig("lpr_altcarmax", daj->getVar(4));
        lpr_altcarmax = daj->getVar(4).toInt();
    }
    if(v==5){
        conf->setDatoConfig("max_cap", daj->getVar(5));
        max_cap = daj->getVar(5).toInt();
    }
    if(v==6){
        conf->setDatoConfig("sentIngreso", daj->getVar(6));
        sentIngreso = daj->getVar(6);
        setearSentidoEnPantalla();
    }
    if(v==7){
        conf->setDatoConfig("sentEgreso", daj->getVar(7));
        sentEgreso = daj->getVar(7);
        setearSentidoEnPantalla();
    }
    if(v==8){
        conf->setDatoConfig("sens_qm", daj->getVar(8));
        sens_qm = daj->getVar(8).toDouble();
        //motionDetector_.set_threshold(sens_qm);
    }
    qDebug()<<"Modificando Variable: "<<v<<"="<<daj->getVar(v);
}

void MainWindow::setProgresoEvento(int cap, int de)
{
    double p1= (double)100/de*cap;

    //pbe->setValue(p1);
    //ui->progressBarProcesoEvento->setValue(p1);

}

/*void MainWindow::getFotogramaDeVideo(const QImage &img)
{
    qDebug()<<"Fotograma Disponible!: "<<img.width();

    IplImage *imgHeaderCaptura = cvCreateImageHeader( cvSize(img.width(), img.height()), IPL_DEPTH_8U, 4);
    imgHeaderCaptura->imageData = (char*) img.bits();

    cvFlip(imgHeaderCaptura, NULL, 0);




    cvSetImageROI(imgHeaderCaptura, cvRect(reg_px, reg_py, reg_an, reg_al));

    recorte3 = cvCreateImage(cvGetSize(imgHeaderCaptura), imgHeaderCaptura->depth, imgHeaderCaptura->nChannels);

    cvCopy(imgHeaderCaptura, recorte3, NULL);
    cvResetImageROI(imgHeaderCaptura);



    cvReleaseImageHeader(&imgHeaderCaptura);
    cvReleaseImage(&recorte3);
}*/



void MainWindow::setAppVersion(QString uav)
{
    QString dv = QDate::currentDate().toString("yyyy:MM:dd");
    dv.replace("201", "");
    QStringList m1dv = dv.split(":");
    QStringList uappv = uav.split(".");

    QString t;
    t.append(NOMBRE_DE_APP);
    t.append(" Versión ");

    qDebug()<<"Nuevo valor de versión: "<<dv;
    qDebug()<<"Ultimo valor de versión: "<<uav;

    bool renovar=false;
    if(m1dv.at(0).toInt()>uappv.at(0).toInt()){
        renovar=true;
    }
    if(m1dv.at(1).toInt()>uappv.at(1).toInt()){
        renovar=true;
    }
    if(m1dv.at(2).toInt()>uappv.at(2).toInt()){
        renovar=true;
    }


    if(renovar){
    qDebug()<<"Seteando nueva valor de versión.";
        QString dv = QDate::currentDate().toString("dd:MM:yyyy");
    QStringList m1dv = dv.split(":");
    QString dvDia = m1dv.at(0);
    QString dvMes = m1dv.at(1);
    QString dvAnio = m1dv.at(2);

     QString nappv = dvAnio.replace("201", "")+"."+dvMes.replace("0", "")+"."+dvDia.replace("0", "");
     t.append(nappv);
     if(QDir::currentPath()==DIR_RUN){
           conf->setDatoConfig("app_version", nappv);
     }
    }else{
        qDebug()<<"Manteniendo valor de versión.";
        t.append(uav);
    }
    setWindowTitle(t);
}

void MainWindow::setEstadoVigilancia(bool b)
{
    if(b){        
        timerProcesarFotograma->start(tim_pam);
        //procCM.start();
    }else{
        timerProcesarFotograma->stop();
        //procCM.st
    }
}



void MainWindow::setearEstadoDeSegmento()
{

       if(!segmentoIniciado){
                iniciarSegmento();

          }
 }



void MainWindow::mostrarEnTabla(QByteArray t, QByteArray tiempo, QString nevento, QString ncapturas)
{
    QString ultimaChapa;
    if(ui->tableWidgetProcesadas->rowCount()>0){
    QTableWidgetItem *uc = ui->tableWidgetProcesadas->item(0,1);
    ultimaChapa=uc->text();
    }else{
        ultimaChapa="";
    }
    bool mostrar=false;
    if(ultimaChapa!=QString(t)){
    if(t=="--- ---"){
        mostrar = false;
    }else{
        mostrar = true;
    }

    if(mostrar){
    //QByteArray h;
    QByteArray ie;
    ie.append(uSentido);

    QByteArray ievento;
    ievento.append(nevento);

    QByteArray icapturas;
    icapturas.append(ncapturas);

    //h.append(QDateTime::currentDateTime().toString("hh:mm:ss"));
    //QTableWidgetItem *twimg=new QTableWidgetItem;
    //twimg->

    ui->tableWidgetProcesadas->insertRow(0);
    ui->tableWidgetProcesadas->setItem(0,0, new QTableWidgetItem(tiempo.constData()));
    ui->tableWidgetProcesadas->setItem(0,1, new QTableWidgetItem(t.constData()));
    ui->tableWidgetProcesadas->setItem(0,2, new QTableWidgetItem(ievento.constData()));
    ui->tableWidgetProcesadas->setItem(0,3, new QTableWidgetItem(icapturas.constData()));

    //QWidget *wimg=new QWidget;
    //wimg->setStyleSheet("background-color:#ff00ff;");
    //ui->tableWidgetProcesadas->setCellWidget(0,2, wimg);
    //ui->tableWidgetProcesadas->setItem(0,2, new QTableWidgetItem(ie.constData()));


    QString logdatos;
    logdatos.append(tiempo);
    logdatos.append(" ");
    logdatos.append(t);
    //logdatos.append(" ");
    //logdatos.append(uSentido);
    appendLog(logdatos);

     if(t!="--- ---"){
         ui->labelChapa->setText(t);
         QStringList hd=QString(tiempo).split(":");
         ui->labelHD->setText(hd.at(0));
         ui->labelMD->setText(hd.at(1));
         ui->labelSD->setText(hd.at(2));
         //ui->labelHoraDigital->setText(h);
         sonido2->play();

     }
    }
    if(ui->tableWidgetProcesadas->rowCount()>16){
        ui->tableWidgetProcesadas->removeRow(ui->tableWidgetProcesadas->rowCount()-1);
    }
    }
}

void MainWindow::mostrarEnTablaCola(int n, QString i, QString f)
{
    QByteArray num;
    num.append(QString::number(n));

    QByteArray inicio;
    inicio.append(i);

    QByteArray fin;
    fin.append(f);

    QString rl;
    rl.append(currentCT);
    rl.append("/");
    rl.replace("/", "\\");

   QString ni;
    ni.append(rl);
    ni.append("evento_");
    ni.append(QString::number(numevento));

    QByteArray cant;
    cant.append(QString::number(cantcap));

    int cantidadEnCola=ui->tableWidgetCola->rowCount();
    ui->tableWidgetCola->insertRow(cantidadEnCola);
    ui->tableWidgetCola->setItem(cantidadEnCola,0, new QTableWidgetItem(num.constData()));
    ui->tableWidgetCola->setItem(cantidadEnCola,1, new QTableWidgetItem(inicio.constData()));
    ui->tableWidgetCola->setItem(cantidadEnCola,2, new QTableWidgetItem(fin.constData()));
    ui->tableWidgetCola->setItem(cantidadEnCola,3, new QTableWidgetItem(cant.constData()));
}

void MainWindow::mostrarEnTablaColaDesdeCarpeta(QDir carpeta)
{
    QStringList files = carpeta.entryList(QDir::Files);
    for(int i=0;i<files.size();i++){
        QString nomArch = QString(files.at(i));
        QString fn;
        if(nomArch.contains("_proc_")){

            fn.append(carpeta.path());
            fn.append("/");
            fn.append(nomArch);
            QFile::remove(fn);
            qDebug()<<"Encontró "<<fn;
        }else{
            qDebug()<<"No Encontró "<<fn;
        }
    }


    desdeCarpeta.clear();
    desdeCarpeta.append(carpeta.path());

    ui->tableWidgetCola->insertRow(0);
    ui->tableWidgetCola->setItem(0,0, new QTableWidgetItem("-1"));
    ui->tableWidgetCola->setItem(0,1, new QTableWidgetItem("00:00:00"));
    ui->tableWidgetCola->setItem(0,2, new QTableWidgetItem("00:00:00"));
    ui->tableWidgetCola->setItem(0,3, new QTableWidgetItem("0"));

    procesarCola();

}

bool MainWindow::validarChapa(QString c)
{
    QStringList m1 = c.split(" ");
    if(m1.size()<2){
        return false;
    }else{
        return true;
    }
    return false;
}

void MainWindow::revisarCola()
{
    if(ui->tableWidgetCola->rowCount()>0){
        QTableWidgetItem *item = ui->tableWidgetCola->item(0,0);
        QString ruta;
        ruta.append(item->text());
        ruta.replace("/", "\\");
        //procesarImagen(ruta);
    }


    /*int px= fam->pos().x()+17;
    int py= fam->pos().y()+17;




    w->setGeometry(px,py,35,35);*/


}


void MainWindow::iniciarSegmento()
{
    cantcap =0;
    segmentoIniciado = true;
    //inicio = new QTime();
    inicio = QTime::currentTime();
    ui->labelInicio->setText(inicio.currentTime().toString("hh:mm:ss"));


    QString rl;
    rl.append(currentCT);
    rl.append("/");
    rl.replace("/", "\\");

    QByteArray ni;
    ni.append(rl);
    ni.append("evento_");
    ni.append(QString::number(numevento));
    //ni.append(".");
    //ni.append(QString(TIPO_DE_IMAGEN));

    QDir dir(ni);
    if (!dir.exists()){
      dir.mkdir(ni);
    }

    //qDebug()<<"Creando Dir: "<<ni;*/

    /*ListaDeMiniaturas *lista1=new ListaDeMiniaturas(this);
    QString nomLista;
    nomLista.append("lista_");
    nomLista.append(QString::number(numevento));
    lista1->setObjectName(nomLista);*/

    //ui->lcdNumberInicio->set
    //timerIncrementaSegmento->start(10);
    //timerMultiDisparo->start(tim_capmov);
    tiempoInactivo =0;
    timerSuma->start(100);

    //ATENCION! Sonido desactivado para version simple
    //sonido2->play();
    //ui->labelChapa->setText("--- ---");
}
void MainWindow::setDurSegmento()
{
    //int ms = inicio.elapsed();
    int ms = unidadTiempo*100;
    int s  = ms / 1000;    ms %= 1000;
    int m  = s  / 60;      s  %= 60;
    int h  = m  / 60;

    QTime ti;
    ti.setHMS(h,m,s,ms);
    //QTime *ti = new QTime();
    //ti->setHMS(inicio+QTime::currentTime());
    //QStringList m1 = ui->labelInicio->text().split(":");
    //ti->setHMS(m1.at(0).toInt(),m1.at(1).toInt(), m1.at(2).toInt());
    ui->labelDuracion->setText(ti.toString("hh:mm:ss"));
}





void MainWindow::sumaTiempo()
{
    unidadTiempo++;
    if(movimiento){

        setDurSegmento();
        tiempoInactivo=0;
    }else{
        tiempoInactivo++;
    }
    if( tiempoInactivo>=12){
        timerSuma->stop();
        //timerMultiDisparo->stop();
        incrementando = false;
        segmentoIniciado = false;
        ui->labelFin->setText(QTime::currentTime().toString("hh:mm:ss"));

        if(cantcap>1){
        mostrarEnTablaCola(numevento, ui->labelInicio->text(), ui->labelFin->text());
        }
        numevento++;
        unidadTiempo=0;
        tiempoInactivo=0;
        //ATENCION! Sonido desactivado para version simple
        //sonido->play();
     }

}

void MainWindow::procesarCola()
{
    //qDebug()<<"Procesando Cola";
    if(ui->tableWidgetCola->rowCount()>0 && !procesandoEvento){
        //ui->progressBarProcesoEvento->setFormat("Procesando... %p%");
        //qDebug()<<"Re Procesando Cola";
        cuentaSegundoDeProcesoEvento=0;
        procesarEvento();
    }else{
        qDebug()<<"No Procesando Cola";

        if(cuentaSegundoDeProcesoEvento<10){
            cuentaSegundoDeProcesoEvento++;
        }else{
            cuentaSegundoDeProcesoEvento=0;
            //procesandoEvento=false;
            //ejecutarQLpr->st
            //ejecutarQLpr->kill();
            //delete ejecutarQLpr;
        }
        qDebug()<<"Cuenta Cola: "<<cuentaSegundoDeProcesoEvento;
        //ui->progressBarProcesoEvento->setFormat("Sin movimientos pendientes.");
        //ui->progressBarProcesoEvento->setValue(0);
    }

    if(ejecutarQLpr->state()==QProcess::Running){
        qDebug()<<"Estado Runningggg..: ";
     }else{
        qDebug()<<"Estado "<<ejecutarQLpr->state();
     }
}

void MainWindow::procesarEvento()
{
        //timerProcesarCola->stop();

        listaDePointsSentido.clear();
        listaDeChapasOptimas.clear();

        contornoAR->update();


        procesandoEvento = true;
        QStringList limgproc;
        QStringList listaCandidatos;
        QTableWidgetItem *tev = ui->tableWidgetCola->item(0,0);
        QString nev = tev->text();


            QString rl;
            rl.append(currentCT);
            rl.append("/");
            //rl.replace("/", "\\");

            QByteArray ni;
            ni.append(rl);
            ni.append("evento_");
            ni.append(nev);





            ui->labelEstadoProceso->setText("X");
            ejecutarQLpr = new QProcess(this);
            connect(ejecutarQLpr, SIGNAL(readyReadStandardOutput()), this, SLOT(procesarRespuestaQLpr()));
            connect(ejecutarQLpr, SIGNAL(readyReadStandardError()), this, SLOT(procesarRespuestaQLprError()));
            connect(ejecutarQLpr, SIGNAL(finished(int)), this, SLOT(setearEstadoProceso(int)));

            QString appQLpr;
            appQLpr.append(QDir::currentPath());
            appQLpr.append("\\");
            appQLpr.append("qlpr.exe");
            appQLpr.replace("/", "\\\\");

            //qDebug()<<"Ruta appQLrp: "<<appQLpr;

            QStringList arg;
           // arg.append("start");
            //arg.append(appQLpr);
            arg.append(ni);

            qDebug()<<"Argumento qlpr: "<<arg;
            ejecutarQLpr->start(appQLpr, arg);


        /*cantcap =0;
        procesandoEvento = false;
        ui->widgetVideo->setVisible(true);
        timerProcesarCola->start(1000);
        timerProcesarFotograma->start(tim_pam);*/



}

void MainWindow::procesarRespuestaQLpr()
{
    QString resultado;
    resultado.append(ejecutarQLpr->readAllStandardOutput());

    if(!resultado.contains("[2]: \"LICENSE EXPIRED\"")){
    if(resultado.contains("]:")){

        //qDebug()<<"con resultado err... "<<resultado;


        if(resultado.contains("[1]:")){
            resultado.replace("[1]:", "");
            resultado.replace("\"", "");
            resultado.replace("\n", "");

            if(validarChapa(resultado)){
                QByteArray patente;
                patente.append(resultado);



                QTableWidgetItem *ttiempo = ui->tableWidgetCola->item(0,1);
                QString ntiempoInicio = ttiempo->text();
                QByteArray tiempoDeInicio;
                tiempoDeInicio.append(ntiempoInicio);

                QTableWidgetItem *tevento = ui->tableWidgetCola->item(0,0);
                QTableWidgetItem *tcapturas = ui->tableWidgetCola->item(0,3);
                //QString nevento = tevento->text();

                mostrarEnTabla(patente, tiempoDeInicio, tevento->text(), tcapturas->text());
            }
        }

        if(resultado.contains("[0]:")){

        }


        //ejecutarQLpr->kill();
        //ejecutarQLpr->close();
        ui->tableWidgetCola->removeRow(0);
        //timerProcesarCola->start(1000);
        procesandoEvento = false;
    }else{
        //qDebug()<<"sin resultado err ...";
    }
    }else{
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error SimpleLPR.");
        msgBox.setInformativeText("Se ha vencido la licencia del SimpleLPR.");
        msgBox.setWindowModality(Qt::ApplicationModal);
        msgBox.setStandardButtons(QMessageBox::Ok );
        msgBox.exec();
        //timerProcesarCola->stop();
        timerProcesarFotograma->stop();
        ui->actionEncender_Apagar->setChecked(false);
    }

}

void MainWindow::procesarRespuestaQLprError()
{
    QString resultado;
    resultado.append(ejecutarQLpr->readAllStandardError());

    if(!resultado.contains("[2]: \"LICENSE EXPIRED\"")){
    if(resultado.contains("]:")){
        //qDebug()<<"con resultado err... "<<resultado;
        if(resultado.contains("[1]:")){
            resultado.replace("[1]:", "");
            resultado.replace("\"", "");
            resultado.replace("\n", "");

            if(validarChapa(resultado)){
                QByteArray patente;
                patente.append(resultado);

                QTableWidgetItem *ttiempo = ui->tableWidgetCola->item(0,1);
                QString ntiempoInicio = ttiempo->text();
                QByteArray tiempoDeInicio;
                tiempoDeInicio.append(ntiempoInicio);

                QTableWidgetItem *tevento = ui->tableWidgetCola->item(0,0);
                QTableWidgetItem *tcapturas = ui->tableWidgetCola->item(0,3);
                 mostrarEnTabla(patente, tiempoDeInicio, tevento->text(), tcapturas->text());
            }
        }

        if(resultado.contains("[0]:")){

        }
        ui->tableWidgetCola->removeRow(0);
         procesandoEvento = false;
    }else{
        //qDebug()<<"sin resultado err ...";
    }
    }else{
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error SimpleLPR.");
        msgBox.setInformativeText("Se ha vencido la licencia del SimpleLPR.");
        msgBox.setWindowModality(Qt::ApplicationModal);
        msgBox.setStandardButtons(QMessageBox::Ok );
        msgBox.exec();
        //timerProcesarCola->stop();
        timerProcesarFotograma->stop();
        ui->actionEncender_Apagar->setChecked(false);
    }
}

void MainWindow::setearEstadoProceso(int ret)
{
    if(ret==0){
    ui->labelEstadoProceso->setText("Fin 0");
    }else{
        ui->labelEstadoProceso->setText("Fin "+QString::number(ret));
    }
}

/*void MainWindow::getFotogramaDeAvi()
{
    //img222 = cvQueryFrame(capture);
    //cvShowImage("Salida:", img222);
}*/

void MainWindow::setearSentido()
{

    for(int i=0;i<listaDePointsSentido.size();i++){
        qDebug()<<"Points encontrados: "<<listaDePointsSentido.at(i).x()<<" "<<i;
    }

    QString nuevoSentido;
    QString nuevoSentidoIcono="Indefinido";
    uSentido="Indefinido";

    if(listaDePointsSentido.size()>1){
    QPoint img1 = listaDePointsSentido.at(0);
    QPoint img2 = listaDePointsSentido.at(listaDePointsSentido.size()-1);

    int px1= img1.x();
    int py1= img1.y();

    int px2= img2.x();
    int py2= img2.y();

    if(px1<px2){
        nuevoSentido.append("ID");
    }else{
        nuevoSentido.append("DI");
    }

    if(py1<py2){
        nuevoSentido.append("B");
    }else{
        nuevoSentido.append("S");
    }

    if(nuevoSentido==sentIngreso){
        uSentido="Ingreso";
    }
    if(nuevoSentido==sentEgreso){
        uSentido="Egreso";
    }

    nuevoSentidoIcono=nuevoSentido;
  }
   //ap->setSentidoDetectado(uSentido);




   QString cadImagen;
   cadImagen.append(":/imagenes/Sentido");
   cadImagen.append(nuevoSentidoIcono);
   cadImagen.append(".png");
   QImage sentidoImagen(cadImagen);
   qDebug()<<"Cadimagen: "<<cadImagen;
   QPixmap pix_sentido = QPixmap::fromImage(sentidoImagen.scaled(100, 100, Qt::IgnoreAspectRatio));
   sent->setPixmap(pix_sentido);
}

int MainWindow::contarCapturas(QString path)
{
     qDebug()<<"Contando capturas en: "<<path;
    QDir c;
    c.setPath(path);
    QStringList files = c.entryList(QDir::Files);
    return files.size();
}

void MainWindow::vaciarCarpeta(QString path)
{

    QDir dir(path);
    dir.setNameFilters(QStringList() << "*.*");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }
}





void MainWindow::sobre_action_menucamaras_triggered()
{
    camaraPresente=true;
}



/*QImage MainWindow::Mat2QImage(cv::Mat const& src)
{
     cv::Mat temp(src.cols,src.rows,src.type()); // make the same cv::Mat
     cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
     QImage dest= QImage((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     return dest;
}*/

/*cv::Mat MainWindow::QImage2Mat(QImage const& src)
{
     QImage temp = src.copy();
     cv::Mat res(temp.height(),temp.width(),CV_8UC3,(uchar*)temp.bits(),temp.bytesPerLine());
     cvtColor(res, res,CV_BGR2RGB); // make convert colort to BGR !
     return res;
}*/

/*IplImage *MainWindow::QImage2IplImage(QImage qimg){
    IplImage *imgHeader = cvCreateImageHeader( cvSize(qimg->width(), qimg->height()), IPL_DEPTH_8U, 4);
    imgHeader->imageData = (char*) qimg->bits();

    uchar* newdata = (uchar*) malloc(sizeof(uchar) * qimg->byteCount());
    memcpy(newdata, qimg->bits(), qimg->byteCount());
    imgHeader->imageData = (char*) newdata;
    //cvClo
    return imgHeader;
}*/





//**********************************************************//
//                                  Funcion LPR                                  //
//**********************************************************//







/*void MainWindow::on_pushButtonSeleccionarCT_clicked()
{
    //carpetaSeleccionada = true;
    QString carpetaAnterior;
    carpetaAnterior.append(currentCT);
    currentCT = QFileDialog::getExistingDirectory(this, tr("Seleccionar Carpeta de Almacenamiento"),
                                                 currentCT,
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    //currentCT.replace("/", "\\");

    if(carpetaAnterior!=currentCT){
            ui->lineEditCarpetaDeTrabajo->setText(currentCT);
            logArch.clear();
            logArch.append(currentCT);
            logArch.append("/log.txt");
   }
}*/

QByteArray MainWindow::clasificarChapas(QStringList lc)
{
    QStringList abc;
    abc.append("A");
    abc.append("B");
    abc.append("C");
    abc.append("D");
    abc.append("E");
    abc.append("F");
    abc.append("G");
    abc.append("H");
    abc.append("I");
    abc.append("J");
    abc.append("K");
    abc.append("L");
    abc.append("M");
    abc.append("N");
    abc.append("O");
    abc.append("P");
    abc.append("Q");
    abc.append("R");
    abc.append("S");
    abc.append("T");
    abc.append("U");
    abc.append("V");
    abc.append("W");
    abc.append("X");
    abc.append("Y");
    abc.append("Z");


    QStringList dig;

    dig.append("0");
    dig.append("1");
    dig.append("2");
    dig.append("3");
    dig.append("4");
    dig.append("5");
    dig.append("6");
    dig.append("7");
    dig.append("8");
    dig.append("9");

    //QStringList lc = ui->plainTextEdit->toPlainText().split("\n");


    QString col1;
    QString col2;
    QString col3;

    QString col4;
    QString col5;
    QString col6;

    QByteArray cad;


    for(int i=0;i<lc.size();i++){
        col1.append(lc.at(i).mid(0,1));
        col2.append(lc.at(i).mid(1,1));
        col3.append(lc.at(i).mid(2,1));

        col4.append(lc.at(i).mid(4,1));
        col5.append(lc.at(i).mid(5,1));
        col6.append(lc.at(i).mid(6,1));
    }

    int mayorCol1= 0;
    int mc1= 0;

    int mayorCol2= 0;
    int mc2= 0;

    int mayorCol3= 0;
    int mc3= 0;

    int mayorCol4= 0;
    int mc4= 0;

    int mayorCol5= 0;
    int mc5= 0;

    int mayorCol6= 0;
    int mc6= 0;

    for(int i=0;i<3;i++){
        for(int i2=0;i2<abc.size();i2++){

            if(i==0){
            int a = contar(col1, abc.at(i2));
            //qDebug()<<"-->Letra "<<abc.at(i2)<<" "<<a;
               if(a>mayorCol1){
                   mayorCol1 = a;
                   mc1 = i2;
                   //qDebug()<<"-->Mayor  "<<i2<<" "<<mayorCol1;
               }
            }

            if(i==1){
            int a = contar(col2, abc.at(i2));
            //qDebug()<<"-->Letra "<<abc.at(i2)<<" "<<a;
               if(a>mayorCol2){
                   mayorCol2 = a;
                   mc2 = i2;
                   //qDebug()<<"-->Mayor  "<<i2<<" "<<mayorCol1;
               }
            }

            if(i==2){
            int a = contar(col3, abc.at(i2));
            //qDebug()<<"-->Letra "<<abc.at(i2)<<" "<<a;
               if(a>mayorCol3){
                   mayorCol3 = a;
                   mc3 = i2;
                   //qDebug()<<"-->Mayor  "<<i2<<" "<<mayorCol1;
               }
            }





        }
    }

    for(int i=0;i<3;i++){
        for(int i2=0;i2<dig.size();i2++){

            if(i==0){
            int a = contar(col4, dig.at(i2));
            //qDebug()<<"-->Letra "<<abc.at(i2)<<" "<<a;
               if(a>mayorCol4){
                   mayorCol4 = a;
                   mc4 = i2;
                   //qDebug()<<"-->Mayor  "<<i2<<" "<<mayorCol1;
               }
            }

            if(i==1){
            int a = contar(col5, dig.at(i2));
            //qDebug()<<"-->Letra "<<abc.at(i2)<<" "<<a;
               if(a>mayorCol5){
                   mayorCol5 = a;
                   mc5 = i2;
                   //qDebug()<<"-->Mayor  "<<i2<<" "<<mayorCol1;
               }
            }

            if(i==2){
            int a = contar(col6, dig.at(i2));
            //qDebug()<<"-->Letra "<<abc.at(i2)<<" "<<a;
               if(a>mayorCol6){
                   mayorCol6 = a;
                   mc6 = i2;
                   //qDebug()<<"-->Mayor  "<<i2<<" "<<mayorCol1;
               }
            }
        }
    }

    QString sfinal;
    sfinal.append(abc.at(mc1));
    sfinal.append(abc.at(mc2));
    sfinal.append(abc.at(mc3));
    sfinal.append(" ");
    sfinal.append(dig.at(mc4));
    sfinal.append(dig.at(mc5));
    sfinal.append(dig.at(mc6));

    //ui->lineEdit->setText(sfinal);




    bool coherencia=true;
    for(int i=0;i<2;i++){
        for(int i2=0;i2<dig.size();i2++){
            if(sfinal.at(i)==dig.at(i2)){
                coherencia = false;
            }
        }
   }

    for(int i=4;i<6;i++){
        for(int i2=0;i2<abc.size();i2++){
            if(sfinal.at(i)==abc.at(i2)){
                coherencia = false;
            }
        }
   }




    QByteArray sfinal2;
    sfinal2.append(sfinal);

    if(sfinal!="AAA 000" && coherencia){
        return sfinal2;
    }else{
       return "--- ---";
    }

}

int MainWindow::contar(QString cadena, QString l){
    int t=cadena.size();
    int c=0;
    for(int i=0;i<t;i++){
        if(cadena.at(i)==l) c++;
    }
    return c;
}

void MainWindow::modificarAreaDeProcesamiento(int px, int py, int an, int al)
{
    conf->setDatoConfig("reg_px", QString::number(px));
    conf->setDatoConfig("reg_py", QString::number(py));
    conf->setDatoConfig("reg_an", QString::number(an));
    conf->setDatoConfig("reg_al", QString::number(al));

        /*conf->setDatoConfig("ap_cam4_px", QString::number(px));
        conf->setDatoConfig("ap_cam4_py", QString::number(py));
        conf->setDatoConfig("ap_cam4_an", QString::number(an));
        conf->setDatoConfig("ap_cam4_al", QString::number(al));*/


    reg_px = px;
    reg_py = py;
    reg_an = an;
    reg_al = al;

    setearContorno(px, py, an,  al);
    fap->setValores(px, py, an, al);
    qDebug()<<"Moviendo...";
    ap->setArea(px, py, an, al);
}

void MainWindow::ocultarAP()
{
    contornoAR->show();
    ap->hide();
}

void MainWindow::setearContorno(int px, int py, int an, int al)
{
    contornoAR->setGeometry(reg_px, reg_py, reg_an, reg_al);
    //contornosChapas->setGeometry(5,5,reg_an-10, reg_al-10);
}

void MainWindow::setearContornoAM(int px, int py, int an, int al)
{
    contornoAM->setGeometry(am_px, am_py, am_an, am_al);
    contornosChapas->setGeometry(5,5,am_an-10, am_al-10);
}

void MainWindow::setearSentidoEnPantalla()
{

    QString cadImagen1;
    cadImagen1.append(":/imagenes/Sentido");
    cadImagen1.append(sentIngreso);
    cadImagen1.append(".png");
    QImage sentidoImagen(cadImagen1);
    QPixmap pix_sentido = QPixmap::fromImage(sentidoImagen.scaled(50, 50, Qt::IgnoreAspectRatio));
    sentDefIngreso->setPixmap(pix_sentido);

    QString cadImagen2;
    cadImagen2.append(":/imagenes/Sentido");
    cadImagen2.append(sentEgreso);
    cadImagen2.append(".png");
    QImage sentidoImagen2(cadImagen2);
    QPixmap pix_sentido2 = QPixmap::fromImage(sentidoImagen2.scaled(50, 50, Qt::IgnoreAspectRatio));
    sentDefEgreso->setPixmap(pix_sentido2);

}
bool MainWindow::moviendo(const  QImage &firstImage, const QImage &secondImage, double lim)
{
    calculandoDiferencia=true;
    bool m=false;
    double totaldiff = 0.0 ; //holds the number of different pixels
    int h = firstImage.height( ) ;
    int w = firstImage.width( ) ;
    int hsecond = secondImage.height( ) ;
    int wsecond = secondImage.width( ) ;
    if ( w != wsecond || h != hsecond ) {
        //std::cerr << "Error, pictures must have identical dimensions!\n" ;
        //ui->statusBar->showMessage("Error en Comparación...");
        //QTimerDisparo->stop();
        return false ;
    }
    for ( int y = 0 ; y < h ; y++ ) {
        uint *firstLine = ( uint* )firstImage.scanLine( y ) ;
        uint *secondLine = ( uint* )secondImage.scanLine( y ) ;
        for ( int x = 0 ; x < w ; x++ ) {
            uint pixelFirst = firstLine[ x ] ;
            int rFirst = qRed( pixelFirst ) ;
            int gFirst = qGreen( pixelFirst ) ;
            int bFirst = qBlue( pixelFirst ) ;
            uint pixelSecond = secondLine[ x ] ;
            int rSecond = qRed( pixelSecond ) ;
            int gSecond = qGreen( pixelSecond ) ;
            int bSecond = qBlue( pixelSecond ) ;
            totaldiff += std::abs( rFirst - rSecond ) / 255.0 ;
            totaldiff += std::abs( gFirst - gSecond ) / 255.0 ;
            totaldiff += std::abs( bFirst -bSecond ) / 255.0 ;
        }
    }
       //std::cout << "The difference of the two pictures is " <<
          //(totaldiff * 100)  / (w * h * 3)  << " % !\n" ;
    if((totaldiff * 100)  / (w * h * 3)>lim){
        m=true;
    }
    return m;
}

void MainWindow::procesarFotograma()
{
    //qDebug() <<  "pF"  << QDateTime::currentDateTime().toString("zzzz");
    if(!procesandoFotograma){
        procesandoFotograma=true;

        QImage imgCompleta = videoWidget->getfotogramaActual().mirrored(false, true);
        recorteactual = imgCompleta.copy(am_px, am_py, am_an, am_al);
        recorteactualAR = imgCompleta.copy(reg_px, reg_py, reg_an, reg_al);

        QString nomLista;
        nomLista.append("lista_");
        nomLista.append(QString::number(numevento));

        if(moviendo(recorteanterior, recorteactual, sens_qm)){
            contornosChapas->setStyleSheet("border-color:#ff3300;");
            movimiento = true;
            //procLPR.setearEstado(true);
           if(!segmentoIniciado){
                    iniciarSegmento();
              }else{
               if(cantcap<=max_cap){
                    //findChild<ListaDeMiniaturas *>(nomLista)->insertarImagen(recorteactual);
                   QString rl;
                   rl.append(currentCT);
                   rl.append("/");
                   rl.replace("/", "\\");

                   QByteArray ni;
                   ni.append(rl);
                   ni.append("evento_");
                   ni.append(QString::number(numevento));
                   ni.append("/");
                   ni.append(QString::number(cantcap));
                   ni.append(".jpg");

                   recorteactualAR.save(ni);

                   cantcap++;
               }else{
                   //timerProcesarFotograma->stop();
               }
           }

           //numcap++;

        }else{
           contornosChapas->setStyleSheet("border-color:#66ff33;");
           movimiento = false;
           //procLPR.setearEstado(false);

        }
        recorteanterior=recorteactual.copy();
    }
    procesandoFotograma=false;
}



void MainWindow::on_actionArea_de_Detecci_n_triggered()
{
    ap->show();
    contornoAR->hide();

    fap = new FormConfigAreaProcesamiento();

    connect(fap, SIGNAL(modificando(int,int,int,int)), this, SLOT(modificarAreaDeProcesamiento(int,int,int,int)));
    connect(ap, SIGNAL(seteandoArea(int,int,int,int)), fap, SLOT(setValores(int,int,int,int)));
    connect(fap, SIGNAL(cerrando()), this, SLOT(ocultarAP()));
    fap->move(0,0);
    fap->setValores(conf->getDatoConfig("reg_px").toInt(), conf->getDatoConfig("reg_py").toInt(), conf->getDatoConfig("reg_an").toInt(), conf->getDatoConfig("reg_al").toInt());
    fap->setWindowFlags(fap->windowFlags() | Qt::WindowStaysOnTopHint );
    fap->show();
}



void MainWindow::on_actionAjustes_triggered()
{
    daj = new DialogAjustes(this);
    daj->setVar(1, QString::number(tim_pam));
    daj->setVar(2, QString::number(tim_capmov));

    daj->setVar(5, QString::number(max_cap));

    daj->setVar(6, sentIngreso);
    daj->setVar(7, sentEgreso);
    daj->setVar(8, QString::number(sens_qm));

    connect(daj, SIGNAL(modificando(int)), this, SLOT(regrabarConfig(int)));
    daj->move(0,0);
    daj->exec();

}

/*void MainWindow::on_actionPosici_n_Ideal_triggered()
{
    if(chapaIdeal->isVisible()){
        chapaIdeal->hide();
        ui->menuVer->actions().at(0)->setChecked(false);
    }else{
        chapaIdeal->show();
        ui->menuVer->actions().at(0)->setChecked(true);

        int npx = reg_px+(reg_an/2)-(chapaIdeal->width()/2);
        int npy = reg_py+(reg_al/2)-(chapaIdeal->height()/2);
        chapaIdeal->move(npx, npy);
    }

}*/

void MainWindow::on_actionTabla_de_Eventos_triggered()
{
    if(ui->tableWidgetCola->isVisible()){
        ui->tableWidgetCola->hide();
        ui->menuVer->actions().at(0)->setChecked(false);
        conf->setDatoConfig("ver_tabevent", "0");
    }else{
        ui->tableWidgetCola->show();
        ui->menuVer->actions().at(0)->setChecked(true);
        conf->setDatoConfig("ver_tabevent", "1");
    }


}

/*void MainWindow::on_pushButtonExplorarCarpeta_clicked()
{
    QDesktopServices::openUrl(ui->lineEditCarpetaDeTrabajo->text());
}*/

/*void MainWindow::on_actionMostrar_Caracteres_Detectados_triggered()
{
    if(ver_cardetect=="1"){
        ver_cardetect.clear();
        ver_cardetect.append("0");
        ui->menuVer->actions().at(2)->setChecked(false);
        conf->setDatoConfig("ver_cardetect", "0");
    }else{
        ver_cardetect.clear();
        ver_cardetect.append("1");
        ui->menuVer->actions().at(2)->setChecked(true);
        conf->setDatoConfig("ver_cardetect", "1");
    }

}*/

/*void MainWindow::on_actionMostrar_Resultados_Nulos_triggered()
{
    if(ver_resultnull=="1"){
        ver_resultnull.clear();
        ver_resultnull.append("0");
        ui->menuVer->actions().at(3)->setChecked(false);
        conf->setDatoConfig("ver_resultnull", "0");
    }else{
        ver_resultnull.clear();
        ver_resultnull.append("1");
        ui->menuVer->actions().at(3)->setChecked(true);
        conf->setDatoConfig("ver_resultnull", "1");
    }

}*/

void MainWindow::on_actionEncender_Apagar_triggered()
{
    if(camaraPresente){
    if(ui->actionEncender_Apagar->isChecked()){
        //rectZM = new QImage(getZM());
        setEstadoVigilancia(true);
        //QTimerDisparo->start(100);
    }else{
        setEstadoVigilancia(false);
        //QTimerDisparo->stop();
    }
    }else{
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error al Encender Reconocimiento!.");
        msgBox.setInformativeText("Aún no se ha seleccionado la cámara correctamente.");
        msgBox.setWindowModality(Qt::ApplicationModal);
        msgBox.setStandardButtons(QMessageBox::Ok );
        msgBox.exec();

        ui->actionEncender_Apagar->setCheckable(false);
        ui->actionEncender_Apagar->setChecked(false);
        ui->actionEncender_Apagar->setCheckable(true);

    }

}

void MainWindow::on_actionAbrir_Log_txt_triggered()
{
    QFile f(logArch);
    if(f.exists()){
        lanzaBlocDeNotas = new QProcess(this);
        QStringList param;
        param.append(logArch);
        lanzaBlocDeNotas->start("notepad", param);
    }else{
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error al mostrar log.txt.");
        msgBox.setInformativeText("Aún el archivo no existe o está vacío.");
        msgBox.setWindowModality(Qt::ApplicationModal);
        msgBox.setStandardButtons(QMessageBox::Ok );
        msgBox.exec();
    }
}

void MainWindow::on_actionApagar_triggered()
{

    timerSuma->stop();
    qApp->exit(1);
}

void MainWindow::on_radioButtonCAM4_toggled(bool checked)
{
    if(checked){
        reg_px=conf->getDatoConfig("ap_cam4_px").toInt();
        reg_py=conf->getDatoConfig("ap_cam4_py").toInt();
        reg_an=conf->getDatoConfig("ap_cam4_an").toInt();
        reg_al=conf->getDatoConfig("ap_cam4_al").toInt();
        setearContorno(reg_px, reg_py, reg_an, reg_al);
    }
}

void MainWindow::on_radioButtonCAM1_toggled(bool checked)
{
    if(checked){
        reg_px=conf->getDatoConfig("ap_cam1_px").toInt();
        reg_py=conf->getDatoConfig("ap_cam1_py").toInt();
        reg_an=conf->getDatoConfig("ap_cam1_an").toInt();
        reg_al=conf->getDatoConfig("ap_cam1_al").toInt();
        setearContorno(reg_px, reg_py, reg_an, reg_al);
    }
}

void MainWindow::on_actionIr_a_Carpeta_de_Datos_triggered()
{
    QDesktopServices::openUrl( QUrl::fromLocalFile(currentCT));
}



void MainWindow::on_spinBoxAR_PX_valueChanged(int arg1)
{
    if(variablesInicializadas && arg1<int(ANCHO_VIDEO)-(contornoAR->width()+2)){
        reg_px=arg1;
        conf->setDatoConfig("reg_px", QString::number(arg1));
        setearContorno(reg_px, reg_py, reg_an, reg_al);
    }else{
        ui->spinBoxAR_PX->setValue(reg_px);
    }
}

void MainWindow::on_spinBoxAR_PY_valueChanged(int arg1)
{
    if(variablesInicializadas && arg1<int(ALTO_VIDEO)-(contornoAR->height()+2)){
        reg_py=arg1;
        conf->setDatoConfig("reg_py", QString::number(arg1));
        setearContorno(reg_px, reg_py, reg_an, reg_al);
    }else{
        ui->spinBoxAR_PY->setValue(reg_py);
    }


}

void MainWindow::on_spinBoxAR_AN_valueChanged(int arg1)
{
    if(variablesInicializadas && arg1>100&& arg1<int(ANCHO_VIDEO)-(10)){
        reg_an=arg1;
        conf->setDatoConfig("reg_an", QString::number(arg1));
        if(reg_px>(int(ANCHO_VIDEO)-reg_an)){
            reg_px=(int(ANCHO_VIDEO)-reg_an);
        }
        setearContorno(reg_px, reg_py, reg_an, reg_al);
    }else{
        ui->spinBoxAR_AN->setValue(reg_an);
    }
}

void MainWindow::on_spinBoxAR_AL_valueChanged(int arg1)
{
    if(variablesInicializadas && arg1>100&& arg1<int(ALTO_VIDEO)-(10)){
        reg_al=arg1;
        conf->setDatoConfig("reg_al", QString::number(arg1));
        if(reg_py>(int(ALTO_VIDEO)-reg_al)){
            reg_py=(int(ALTO_VIDEO)-reg_al);
        }
        setearContorno(reg_px, reg_py, reg_an, reg_al);
    }else{
        ui->spinBoxAR_AL->setValue(reg_al);
    }
    //regrabarConfig();
}

void MainWindow::on_spinBoxAM_PX_valueChanged(int arg1)
{
    if(variablesInicializadas && arg1<int(ANCHO_VIDEO)-(contornoAM->width()+2)){
        am_px=arg1;
        conf->setDatoConfig("am_px", QString::number(arg1));
        setearContornoAM(am_px, am_py, am_an, am_al);
    }else{
        ui->spinBoxAM_PX->setValue(am_px);
    }
}

void MainWindow::on_spinBoxAM_PY_valueChanged(int arg1)
{
    if(variablesInicializadas && arg1<int(ALTO_VIDEO)-(contornoAM->height()+2)){
        am_py=arg1;
        conf->setDatoConfig("am_py", QString::number(arg1));
        setearContornoAM(am_px, am_py, am_an, am_al);
    }else{
        ui->spinBoxAM_PY->setValue(am_py);
    }
}

void MainWindow::on_spinBoxAM_AN_valueChanged(int arg1)
{
    if(variablesInicializadas && arg1>100&& arg1<int(ANCHO_VIDEO)-(10)){
        am_an=arg1;
        conf->setDatoConfig("am_an", QString::number(arg1));
        if(am_px>(int(ANCHO_VIDEO)-am_an)){
            am_px=(int(ANCHO_VIDEO)-am_an);
        }
        setearContornoAM(am_px, am_py, am_an, am_al);
    }else{
        ui->spinBoxAM_AN->setValue(reg_an);
    }
}

void MainWindow::on_spinBoxAM_AL_valueChanged(int arg1)
{
    if(variablesInicializadas && arg1>100&& arg1<int(ALTO_VIDEO)-(10)){
        am_al=arg1;
        conf->setDatoConfig("am_al", QString::number(arg1));
        if(am_py>(int(ALTO_VIDEO)-am_al)){
            am_py=(int(ALTO_VIDEO)-am_al);
        }
        setearContornoAM(am_px, am_py, am_an, am_al);
    }else{
        ui->spinBoxAM_AL->setValue(am_al);
    }
}



void MainWindow::on_tableWidgetProcesadas_itemClicked(QTableWidgetItem *item)
{
    ui->widgetImagenes->setearEstadoCinematica(false);

    ui->toolButtonReproducir->setEnabled(true);
    ui->toolButtonNextFotograma->setEnabled(true);
    ui->toolButtonPrevFotograma->setEnabled(true);

    ui->radioButtonVelocidadLenta->setEnabled(true);
    ui->radioButtonVelocidadMedia->setEnabled(true);
    ui->radioButtonVelocidadRapida->setEnabled(true);

    ui->tabWidgetEventos->setCurrentIndex(1);

    QTableWidgetItem *datoEvento = ui->tableWidgetProcesadas->item(item->row(),2);
    QTableWidgetItem *datoCapturas = ui->tableWidgetProcesadas->item(item->row(),3);

    QString rl;
    rl.append(currentCT);
    rl.append("/");
    rl.replace("/", "\\");

    QString ni;
    ni.append(rl);
    ni.append("evento_");
    ni.append(datoEvento->text());
    ui->widgetImagenes->reproducir(ni, datoCapturas->text().toInt());
    ui->toolButtonReproducir->setChecked(true);

    //timerProcesarCola->start(1000);

}

void MainWindow::on_toolButtonReproducir_toggled(bool checked)
{
    if(checked){
        ui->widgetImagenes->setearEstadoCinematica(true);
        ui->toolButtonReproducir->setText("Detener");
    }else{
        ui->widgetImagenes->setearEstadoCinematica(false);
        ui->toolButtonReproducir->setText("Reproducir");
    }
}

void MainWindow::on_radioButtonVelocidadLenta_toggled(bool checked)
{
    if(checked){
        ui->widgetImagenes->setearVelocidad(400);
    }
}

void MainWindow::on_radioButtonVelocidadMedia_toggled(bool checked)
{
    if(checked){
        ui->widgetImagenes->setearVelocidad(250);
    }
}

void MainWindow::on_radioButtonVelocidadRapida_toggled(bool checked)
{
    if(checked){
        ui->widgetImagenes->setearVelocidad(100);
    }
}

void MainWindow::on_tabWidgetEventos_currentChanged(int index)
{
    if(index==0){
        //ui->widgetImagenes->setearEstadoCinematica(false);
        ui->toolButtonReproducir->setChecked(false);
        //ejecutarQLpr->close();
        //ejecutarQLpr->kill();
        //procesarCola();

    }
}

void MainWindow::on_toolButtonPrevFotograma_clicked()
{
    ui->toolButtonReproducir->setChecked(false);
    ui->widgetImagenes->prevFotograma();
}

void MainWindow::on_toolButtonNextFotograma_clicked()
{
    ui->toolButtonReproducir->setChecked(false);
    ui->widgetImagenes->nextFotograma();
}

void MainWindow::on_pushButton_clicked()
{
    //QTableWidgetItem *datoEvento = ui->tableWidgetProcesadas->item(item->row(),2);

    QString rl4;
    rl4.append(currentCT);
    rl4.append("/");
    rl4.replace("/", "\\");

    QString ni4;
    ni4.append(rl4);
    ni4.append("evento_");
    ni4.append("4");
    ui->widgetImagenes->setearCarpetaEvento(ni4);
    ui->widgetImagenes->reproducir(ni4, 53);
    //ui->toolButtonReproducir->setChecked(true);

    ui->toolButtonReproducir->setEnabled(true);
    ui->toolButtonNextFotograma->setEnabled(true);
    ui->toolButtonPrevFotograma->setEnabled(true);

    ui->radioButtonVelocidadLenta->setEnabled(true);
    ui->radioButtonVelocidadMedia->setEnabled(true);
    ui->radioButtonVelocidadRapida->setEnabled(true);

    //timerProcesarCola->start(1000);
}
