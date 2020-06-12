#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define NOMINMAX

#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraImageCapture>
#include <QtMultimedia/QMediaRecorder>

#include <QMainWindow>
#ifdef Q_OS_WIN
#include <Windows.h>
#include <tchar.h>
#include <windows.h>
#endif
#include <QtMultimediaWidgets/QVideoWidget>

#include <QTimer>
#include <QSound>
#include <QTime>
#include <QProcess>

#include <QTableWidgetItem>
#include <QScrollBar>

#include "config.h"
#include "administradordearchivos.h"

#include "formconfigareaprocesamiento.h"
#include "dialogajustes.h"
#include "areadeprocesamiento.h"

#include "listademiniaturas.h"

#include "pantalla.h"
#include "videowidget.h"
#include "videowidgetsurface.h"
//#include "galeriadecapturas.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    void setearModoDesarrollo(bool b);
    void aplicarModoDesarrollo();

public slots:
    void appendLog(QString fila);
    void regrabarConfig(int v);
    void setProgresoEvento(int cap, int de);

private slots:
    //void getFotogramaDeVideo(const QImage &img);

    void setAppVersion(QString uav);
    void setEstadoVigilancia(bool b);

    void setearEstadoDeSegmento();


    void mostrarEnTabla(QByteArray t, QByteArray tiempo, QString nevento, QString ncapturas);
    void mostrarEnTablaCola(int n, QString i, QString f);
    void mostrarEnTablaColaDesdeCarpeta(QDir carpeta);
    bool validarChapa(QString c);
    void revisarCola();

    QByteArray clasificarChapas(QStringList lc);
    int contar(QString cadena, QString l);

    //Funciones de Pantalla
    void modificarAreaDeProcesamiento(int px, int py, int an, int al);
    void ocultarAP();
    void setearContorno(int px, int py, int an, int al);
    void setearContornoAM(int px, int py, int an, int al);
    void setearSentidoEnPantalla();


    //Funciones de Comparación
    bool moviendo(const  QImage &firstImage, const QImage &secondImage, double lim);


    //Funciones de Detección
    void procesarFotograma();

    //Funciones de Segmentos
    void iniciarSegmento();
    void setDurSegmento();
    void sumaTiempo();

    //Funciones de Procesos
    void procesarCola();
    void procesarEvento();
    void procesarRespuestaQLpr();
    void procesarRespuestaQLprError();
    void setearEstadoProceso(int ret);

    //Funciones de AVI
    //void getFotogramaDeAvi();

    //Funciones de Sentido
    void setearSentido();
    int contarCapturas(QString path);
    void vaciarCarpeta(QString path);




     void setCamera(const QCameraInfo &cameraInfo);

     void displayRecorderError();
     void displayCameraError();

     void updateCameraDevice(QAction *action);

     void updateCameraState(QCamera::State state);
     void updateCaptureMode();

     void updateRecorderState(QMediaRecorder::State state);
     void updateLockStatus(QCamera::LockStatus status, QCamera::LockChangeReason);

     void updateRecordTime();


     //void processCapturedImage(int requestId, const QImage &img);



     void readyForCapture(bool ready);
     void imageSaved(int id, const QString &fileName);
     //void procesarVideoFrameBuffer(int n, QVideoFrame frame);

     void sobre_action_menucamaras_triggered();

     void on_actionArea_de_Detecci_n_triggered();

     void on_actionAjustes_triggered();

     void on_actionTabla_de_Eventos_triggered();

     void on_actionEncender_Apagar_triggered();

     void on_actionAbrir_Log_txt_triggered();

     void on_actionApagar_triggered();

     void on_radioButtonCAM4_toggled(bool checked);

     void on_radioButtonCAM1_toggled(bool checked);

     void on_actionIr_a_Carpeta_de_Datos_triggered();

     void on_spinBoxAR_PX_valueChanged(int arg1);

     void on_spinBoxAR_PY_valueChanged(int arg1);

     void on_spinBoxAR_AN_valueChanged(int arg1);

     void on_spinBoxAR_AL_valueChanged(int arg1);

     void on_spinBoxAM_PX_valueChanged(int arg1);

     void on_spinBoxAM_PY_valueChanged(int arg1);

     void on_spinBoxAM_AN_valueChanged(int arg1);

     void on_spinBoxAM_AL_valueChanged(int arg1);



     void on_tableWidgetProcesadas_itemClicked(QTableWidgetItem *item);

     void on_toolButtonReproducir_toggled(bool checked);

     void on_radioButtonVelocidadLenta_toggled(bool checked);

     void on_radioButtonVelocidadMedia_toggled(bool checked);

     void on_radioButtonVelocidadRapida_toggled(bool checked);

     void on_tabWidgetEventos_currentChanged(int index);

     void on_toolButtonPrevFotograma_clicked();

     void on_toolButtonNextFotograma_clicked();

     void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    bool modoDesarrollo;

    config *conf;
    AdministradorDeArchivos *ada;

    bool variablesInicializadas;

    QLabel *titContornoAR;
    QLabel *titContornoAM;

    //GaleriaDeCapturas *gdc;

    bool camaraPresente;

    QString chapaCalibre;
    int alturaIdealDeCaracter;
    int anchoIdealDeCaracter;
    int toleranciaAlturaIdealDeCaracter;
    int maxAlturaCaracter;
    QStringList listaDeChapasOptimas;

    int cantcap;
    //bool procesando;

    Pantalla *miPantalla;
    QImage *recorte;
    QImage recorte2;


    QImage recorteanterior;
    QImage recorteactual;
    QImage recorteactualAR;

    int numevento;
    int cuentaSegundoDeProcesoEvento;
    int colorLetra;

    QList<QPoint> listaDePointsSentido;
    QString uSentido;

    QLabel *sent;
    QLabel *titSentDefIngreso;
    QLabel *sentDefIngreso;
    QLabel *titSentDefEgreso;
    QLabel *sentDefEgreso;

    QWidget *contornoAR;
    QWidget *contornoAM;
    QWidget *contornosChapas;

    QSound *sonido;
    QSound *sonido2;

    QString appPath;
    QDir pluginsDir;
    QStringList pluginFileNames;
    QString currentCT;
    QString logArch;

    QString desdeCarpeta;




    //VARIABLES DE COMPARACION
    bool calculandoDiferencia;


    //VARIABLES DE PROCESAR FOTOGRAMA
    QTimer *timerProcesarFotograma;
    bool procesandoFotograma;


    VideoWidget *videoWidget2;

    QTimer *timerSuma;
    int unidadTiempo;
    int tiempoInactivo;

    int reg_px;
    int reg_py;
    int reg_an;
    int reg_al;

    int am_px;
    int am_py;
    int am_an;
    int am_al;

    //Varialbles Generales
    QString ver_tabevent;
    int tim_pam;
    int tim_capmov;

    int lpr_altcarmax;
    int max_cap;

    QString sentIngreso;
    QString sentEgreso;
    double sens_qm;



    QTimer *timerProcesarCola;



    bool incrementando;
    bool segmentoIniciado;
    bool movimiento;
    bool cierreIniciado;
    bool procesandoEvento;


    FormConfigAreaProcesamiento *fap;
    DialogAjustes *daj;
    AreaDeProcesamiento *ap;


    QTime inicio;
    QTime fin;

    VideoWidget *videoWidget;

    QCamera *camera;
    QCameraImageCapture *imageCapture;
    QMediaRecorder* mediaRecorder;

    QImageEncoderSettings imageSettings;
    QAudioEncoderSettings audioSettings;
    QVideoEncoderSettings videoSettings;
    QString videoContainerFormat;
    bool isCapturingImage;
    bool applicationExiting;



    QProcess *lanzaBlocDeNotas;
    QProcess *ejecutarQLpr;
};

#endif // MAINWINDOW_H
