#include "config.h"

config::config(QObject *parent) : QObject(parent)
{

}

config::~config()
{

}

void config::setAppPath(QString ap)
{
    appPath = ap;
}

bool config::cargarConfiguracion()
{
    xmlCONFIG = new QDomDocument();

    QByteArray dirCompletaDeConfig;
    dirCompletaDeConfig.append(appPath);
    dirCompletaDeConfig.append("/config.xml");

    QFile f(dirCompletaDeConfig);

    if (!f.open(QIODevice::ReadOnly ))
    {
        QByteArray cont;
        cont.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                    "<CONFIG VERSION=\"1.0\" ANIO=\"2014\">"

                    "<DATO ID=\"ver1\">"
                    "   <NAME>ver_tabevent</NAME>"
                    "   <VALUE>0</VALUE>"
                    "</DATO>"

                    "<DATO ID=\"dato0\">"
                    "   <NAME>app_version</NAME>"
                    "   <VALUE>5.1.25</VALUE>"
                    "</DATO>"

                    "<DATO ID=\"dato4\">"
                    "   <NAME>reg_px</NAME>"
                    "   <VALUE>80</VALUE>"
                    "</DATO>"
                    "<DATO ID=\"dato5\">"
                    "   <NAME>reg_py</NAME>"
                    "   <VALUE>30</VALUE>"
                    "</DATO>"
                    "<DATO ID=\"dato6\">"
                    "   <NAME>reg_an</NAME>"
                    "   <VALUE>550</VALUE>"
                    "</DATO>"
                    "<DATO ID=\"dato7\">"
                    "   <NAME>reg_al</NAME>"
                    "   <VALUE>310</VALUE>"
                    "</DATO>"

                    "<DATO ID=\"dato4\">"
                    "   <NAME>am_px</NAME>"
                    "   <VALUE>80</VALUE>"
                    "</DATO>"
                    "<DATO ID=\"dato5\">"
                    "   <NAME>am_py</NAME>"
                    "   <VALUE>150</VALUE>"
                    "</DATO>"
                    "<DATO ID=\"dato6\">"
                    "   <NAME>am_an</NAME>"
                    "   <VALUE>550</VALUE>"
                    "</DATO>"
                    "<DATO ID=\"dato7\">"
                    "   <NAME>am_al</NAME>"
                    "   <VALUE>150</VALUE>"
                    "</DATO>"

                    "<DATO ID=\"dato8\">"
                    "   <NAME>tim_pam</NAME>"
                    "   <VALUE>100</VALUE>"
                    "</DATO>"
                    "<DATO ID=\"dato9\">"
                    "   <NAME>tim_capmov</NAME>"
                    "   <VALUE>125</VALUE>"
                    "</DATO>"

                    "<DATO ID=\"dato11\">"
                    "   <NAME>lpr_altcarmax</NAME>"
                    "   <VALUE>120</VALUE>"
                    "</DATO>"
                    "<DATO ID=\"dato12\">"
                    "   <NAME>max_cap</NAME>"
                    "   <VALUE>200</VALUE>"
                    "</DATO>"

                    "<DATO ID=\"dato13\">"
                    "   <NAME>sentIngreso</NAME>"
                    "   <VALUE>IDS</VALUE>"
                    "</DATO>"

                    "<DATO ID=\"dato14\">"
                    "   <NAME>sentEgreso</NAME>"
                    "   <VALUE>DIB</VALUE>"
                    "</DATO>"

                    "<DATO ID=\"dato15\">"
                    "   <NAME>sens_qm</NAME>"
                    "   <VALUE>2.5</VALUE>"
                    "</DATO>"
                    "<DATO ID=\"dato16\"><NAME>ap_cam1_px</NAME><VALUE>187</VALUE></DATO><DATO ID=\"dato17\"><NAME>ap_cam1_py</NAME><VALUE>187</VALUE></DATO><DATO ID=\"dato18\"><NAME>ap_cam1_an</NAME><VALUE>445</VALUE></DATO><DATO ID=\"dato16\"><NAME>ap_cam1_al</NAME><VALUE>204</VALUE></DATO><DATO ID=\"dato19\"><NAME>ap_cam4_px</NAME><VALUE>187</VALUE></DATO><DATO ID=\"dato16\"><NAME>ap_cam4_py</NAME><VALUE>34</VALUE></DATO><DATO ID=\"dato20\"><NAME>ap_cam4_an</NAME><VALUE>445</VALUE></DATO><DATO ID=\"dato21\"><NAME>ap_cam4_al</NAME><VALUE>354</VALUE></DATO>"

                    "</CONFIG>");

        QFile nf(dirCompletaDeConfig);
        nf.open(QIODevice::ReadWrite );
        QTextStream out(&nf);
        out << cont;
        nf.close();
        return false;

    }

    xmlCONFIG->setContent(&f);
    f.close();
    QDomElement root=xmlCONFIG->documentElement();

    QString Type=root.tagName();
    QString Board=root.attribute("VERSION","No name");
    int Year=root.attribute("ANIO","2013").toInt();

    QDomElement Datos=root.firstChild().toElement();

    while(!Datos.isNull())
    {

        if (Datos.tagName()=="DATO")
        {
            QString ID=Datos.attribute("ID","No ID");
            QDomElement Child=Datos.firstChild().toElement();
            QString Nombre;
            QString Valor;
            while (!Child.isNull())
            {
                if (Child.tagName()=="NAME"){
                     Nombre=Child.firstChild().toText().data();
                }
                if (Child.tagName()=="VALUE"){
                      Valor=Child.firstChild().toText().data();
                }
                Child = Child.nextSibling().toElement();
            }


        }
        Datos = Datos.nextSibling().toElement();
    }
    return true;
}

QString config::getDatoConfig(QString n)
{
    QDomElement root=xmlCONFIG->documentElement();
    QDomElement Datos=root.firstChild().toElement();
    while(!Datos.isNull())
    {

        if (Datos.tagName()=="DATO")
        {
            QString ID=Datos.attribute("ID","No ID");
            QDomElement Child=Datos.firstChild().toElement();

            QString Nombre;
            QString Valor;
            while (!Child.isNull())
            {
                if (Child.tagName()=="NAME"){
                     Nombre=Child.firstChild().toText().data();
                }
                if (Child.tagName()=="VALUE"){
                      Valor=Child.firstChild().toText().data();
                }

                Child = Child.nextSibling().toElement();
            }

            if(Nombre == n){
               return  Valor;
            }


        }
        Datos = Datos.nextSibling().toElement();
    }
    return  "null";
}

void config::setDatoConfig(QString n, QString v)
{

    QDomDocument nuevoXmlCONFIG;


    QByteArray dirActual;
    dirActual.append(QDir::currentPath());
    QByteArray dirCompletaDeConfig;
    dirCompletaDeConfig.append(dirActual);
    dirCompletaDeConfig.append("/");
    dirCompletaDeConfig.append("config.xml");


    QDomElement root=xmlCONFIG->documentElement();
    QDomElement Datos=root.firstChild().toElement();

    QFile outFile(dirCompletaDeConfig);
    outFile.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&outFile);
    xmlWriter.setAutoFormatting(false);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("CONFIG");
    xmlWriter.writeAttribute("VERSION","1.0");
    xmlWriter.writeAttribute("ANIO","2014");

    while(!Datos.isNull())
    {

        if (Datos.tagName()=="DATO")
        {
            QString ID=Datos.attribute("ID","No ID");
            QDomElement Child=Datos.firstChild().toElement();
            QString Nombre;
            QString Valor;
            while (!Child.isNull())
            {
                if (Child.tagName()=="NAME"){
                     Nombre=Child.firstChild().toText().data();
                }
                if (Child.tagName()=="VALUE"){
                      Valor=Child.firstChild().toText().data();
                }

                Child = Child.nextSibling().toElement();
            }

            xmlWriter.writeStartElement("DATO");
            xmlWriter.writeAttribute("ID",ID);
            xmlWriter.writeStartElement("NAME");
            xmlWriter.writeCharacters(Nombre);
            xmlWriter.writeEndElement();
            xmlWriter.writeStartElement("VALUE");
            if(n!=Nombre){
                xmlWriter.writeCharacters(Valor);
            }else{
                xmlWriter.writeCharacters(v);
            }
            xmlWriter.writeEndElement();
            xmlWriter.writeEndElement();


        }
        Datos = Datos.nextSibling().toElement();

    }




    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    outFile.close();
    cargarConfiguracion();
}

