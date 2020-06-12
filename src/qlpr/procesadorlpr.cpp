#include "procesadorlpr.h"
#include <QDebug>

#include <tchar.h>
//#include <QPainter>

#include <QDir>


IProcessor *pProc;
ICandidates *pCds;
ICandidate *pCd;

void ProcesadorLPR::run()
{
    /*qDebug()<<"Ejecutando ProcesadorLPR: "<<currentThreadId();
    int v=0;
    while (1) {
                {
                QMutexLocker locker(&m_mutex);
                if (m_stop) break;
                }
                //qDebug()<<"ProcesadorLPR --> "<<v;
        detenido=false;
        msleep(10);

                v++;
            }*/
     detenido=false;
}

void ProcesadorLPR::stop()
{
    qDebug()<<"Se ha detenido ProcesadorLPR ";
    QMutexLocker locker(&m_mutex);
    m_stop=true;
    detenido=true;

    //pProc->release();
    //pCds->release();
    //pCd->release();
}

void ProcesadorLPR::setearEstado(bool b)
{
    detenido=b;
}

void ProcesadorLPR::inicializarLPR()
{


        QString rl;
        rl.append(QDir::currentPath());
        rl.append("/");





        //if (IsWow64()) {
            //ui->textBrowser->appendPlainText("Configurando sistema en modo 64bit.");
            rl.append("liblpr/SimpleLPR2_native.dll");


        const wchar_t *wc0 = (const wchar_t*) rl.utf16();

        hLib = LoadLibraryEx(  wc0, NULL, 0 );
        if ( hLib == NULL )
         {
            //qDebug()<<"LoadLibraryEx failed";
            //ui->textBrowser->appendPlainText("Error al cargar librería LPR.");
            //ui->textBrowser->appendPlainText("\n\n");
         }else{
             //qDebug()<<"Librería SimpleLPR cargada correctamente.";
             //ui->textBrowser->appendPlainText("Librería LPR cargada correctamente.");
             //ui->textBrowser->appendPlainText("\n");
         }

        SimpleLPRSetupFunc pfnSetup = (SimpleLPRSetupFunc)::GetProcAddress( hLib, "Setup" );
        if ( pfnSetup == 0 ) {
             //qDebug()<<"GetProcAddress failed.";
            //ui->textBrowser->appendPlainText("Error al ejecutar setup LPR.");
            //ui->textBrowser->appendPlainText("\n");
             ::FreeLibrary( hLib );
        }else{
             //qDebug()<<"La función Setup de SimpleLPR se ejecutó con éxito.";
             //ui->textBrowser->appendPlainText("La función Setup de LPR se ejecutó con éxito.");
             //ui->textBrowser->appendPlainText("\n");
        }

        pLPR = (*pfnSetup)();
        if ( pLPR == NULL ){
                qDebug()<<"Setup SimpleLPR ha fallado.";
                ::FreeLibrary( hLib );
            }else{
                 //qDebug()<<"Puntero ISimpleLPR *pLPR creado correctamente.";
            }

            nErr = 0;

            SIMPLELPR_VersionNumber ver;
            pLPR->versionNumber_get( ver );
            //qDebug() <<  "SimpleLPR version " << ver.A << "." << ver.B << "." << ver.C << "." << ver.D  ;
            uint countryId = 1;

            for (uint i = 0; i < pLPR->numSupportedCountries_get(); ++i )
                pLPR->countryWeight_set( i, 0.f );

           nErr = pLPR->countryWeight_set( countryId, 1.f ) ? 0 : -4;
           if ( nErr == 0 )
               nErr = pLPR->realizeCountryWeights() ? 0 : -5;

                if ( pProc != NULL )
                    pProc->release();
            if ( nErr != 0 )
            {
                SimpleLPR2_Native::IErrorInfo *pErr = pLPR->lastError_get();
                if ( pErr != NULL )
                {
                    qDebug() <<  "Error ocurrido. Código de Error: " << pErr->errorCode <<  ", description: " << pErr->description  ;
                    pErr->release();
                }
                else
                    qDebug() <<  "Error ocurrido. Código de Error: " << nErr  ;
            }
            //FINALIZA CARGA DE LIBRERÍA SIMPLELPR2


}

QString ProcesadorLPR::getChapa(const QString &img)
{
    //procesando = true;

    QString resChapa;
    resChapa.append("");
    //qDebug()<<"Intentando procesar "<<img;
    //ui->labelLP->setText("--- ---");
    pProc = NULL;
    if ( nErr == 0 )
    {
        pProc = pLPR->createProcessor();
        nErr = ( pProc != NULL ) ? 0 : -7;
    }

    pCds = NULL;
    if (nErr == 0)
    {
         //qDebug() <<  "Procesando..."  ;
         const wchar_t *wc1 = (const wchar_t*) img.utf16();
         pCds = pProc->analyze( wc1, 120 );
         nErr = ( pCds != NULL ) ? 0 : -8;
    }

    if ( nErr == 0 )
    {
        if ( pCds->numCandidates_get() == 0 ){
            /*QString linea;
            linea.append(" ");
            linea.append("000-000");
            ui->labelChapa->setText(linea);*/
            //mostrarEnTabla(linea);
            //resChapa
        }else{
             //qDebug() << pCds->numCandidates_get() <<  " license plates found:"  ;


             pCd = pCds->candidate_get(0);
             //int ultimoCandidato=0;
             nErr = ( pCd != NULL ) ? 0 : -9;
             //for ( uint i = 0; nErr == 0 && i < pCds->numCandidates_get(); ++i ){
                 //if(ui->checkBox->isChecked()){
                     //if (pCds->candidate_get(0)->confidence > pCd->confidence){
                         pCd = pCds->candidate_get(0);
                         //QStringList m1= QString::fromWCharArray(pCd->text_get()).split(" ");
                         //if(m1.size()<1){
                             //pCd = pCds->candidate_get(ultimoCandidato);
                             //ultimoCandidato=i;
                         //}
                     //}
                 //}else{
                     //pCd = pCds->candidate_get(0);
                 //}
                 //nErr = ( pCd != NULL ) ? 0 : -9;
             //}

             if ( nErr == 0 )
             {
                      //ui->labelLP->setText(QString::fromWCharArray(pCd->text_get()));

                      //QString hora = QDateTime::currentDateTime().toString("hh:mm:ss");

                 resChapa =   QString::fromWCharArray(pCd->text_get());





                 pCd->release();
             }
        }
    }else{
        SimpleLPR2_Native::IErrorInfo *pErr = pLPR->lastError_get();
        if ( pErr != NULL )
        {
            const wchar_t *ed1 = pErr->description;

            QByteArray infoError;
            infoError.append(pErr->errorCode);

            QByteArray infoErrorDes;
            infoErrorDes.append(QString::fromWCharArray(ed1));
            //ui->textBrowser->appendPlainText("ERROR! -->");

            //ui->textBrowser->appendPlainText(infoError);
            //ui->textBrowser->appendPlainText("DESCRIPCION DE ERROR! -->");
            //ui->textBrowser->appendPlainText(infoErrorDes);

            //sonido2->play();

            qDebug() << "[2]:" <<infoErrorDes;

            pErr->release();
       }else{
            //ui->textBrowser->appendPlainText("Error en cero y nulo!");
            /*QString linea;
            linea.append(" ");
            linea.append("000-000");
            ui->labelChapa->setText(linea);*/
            //mostrarEnTabla(linea);
        }
    }

   if ( pCds != NULL )
        pCds->release();


   //ui->tableWidgetCola->removeRow(0);
   pProc->release();
   //procesando = false;

   return resChapa;

}

bool ProcesadorLPR::validarChapa(QString c)
{
    QStringList m1 = c.split(" ");
    if(m1.size()!=2){
        return false;
    }else{
        return true;
    }
    return false;
}

QByteArray ProcesadorLPR::clasificarChapas(QStringList lc)
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

int ProcesadorLPR::contar(QString cadena, QString l){
    int t=cadena.size();
    int c=0;
    for(int i=0;i<t;i++){
        if(cadena.at(i)==l) c++;
    }
    return c;
}

