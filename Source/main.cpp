/*
    RePaint, the network painting tool
    Copyright (C) 2012-2023 Yauheni Bolshakov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
  */

#include <QtCore>
#include <QtWidgets/QApplication>
#include "MHPwindow.h"
#include <QtWidgets/QInputDialog>
//#include <QPlastiqueStyle>
#include <QSplashScreen>
//#include <QStyleHints>

QByteArray GenRawKey(quint32 RegNum,int Modifier=0);
quint32 DecryptInt(QByteArray Crypt);
int ValidateKey(QString HexKey);


/*
bool QCoreApplication::notify(QObject* receiver, QEvent* event){
    try {
         return QCoreApplication::notify(receiver, event);
     } catch (std::exception &e) {
         qFatal("Error %s sending event %s to object %s (%s)",
             e.what(), typeid(*event).name(), qPrintable(receiver->objectName()),
             typeid(*receiver).name());
     } catch (...) {
         qFatal("Error <unknown> sending event %s to object %s (%s)",
             typeid(*event).name(), qPrintable(receiver->objectName()),
             typeid(*receiver).name());
     }

     // qFatal aborts, so this isn't really necessary
     // but you might continue if you use a different logging lib
     return false;
}
*/
int main (int argc, char *argv[])
{
    //Q_INIT_RESOURCE(mdi);
    //splash.finish(&MHPWin);
    QApplication app(argc,argv);


    QPixmap pixmap(":/splash.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();

    //   app.processEvents();
    //app.setStyle(new QPlastiqueStyle);
    //app.setStyle(new QMacStyle);

    /* block for encryption
    bool ok;
    QInputDialog DlgGetKey;
    //int nw=3420; //new width

    QString ikey=DlgGetKey.getText(0, "Input Your Serial",
                                            "KEY:", QLineEdit::Normal,
                         "", &ok);



    if ((true) ||  (((ok) && (!ikey.isEmpty())) && (ValidateKey(ikey)>-1)))
    {

       MHPwindow MHPWin;
       MHPWin.show();
    //   splash->finish(&MHPWin);
       return app.exec();
    }    else {      return 0;   }
    */
    MHPwindow MHPWin;
    MHPWin.show();
    return app.exec();


}


QByteArray GenRawKey(quint32 RegNum,int Modifier){
    QByteArray *gphrase=new QByteArray();
    QDataStream tds(gphrase,QIODevice::ReadWrite);
    tds << QChar(8);
    tds << QChar(121);
    tds << QChar(225);
    tds << QChar(17);
    tds << QChar(44);
    tds << QChar(230);
    tds << QChar(175);
    tds << QChar(58);
    tds << QChar(165);
    tds << QChar(4);
    tds << QChar(23);
    tds << QChar(133);
    tds << QChar(100+Modifier); // identifier for key region starts with
    tds << QChar(253);
    tds << RegNum;

    return QCryptographicHash::hash(*gphrase,QCryptographicHash::Sha1);

}

quint32 DecryptInt(QByteArray Crypt){

    QByteArray *DeCryptMatrix=new QByteArray();
    quint32 result;

    quint8 d3;
    quint8 d2;
    quint8 d1;
    quint8 d0;
    /*
    CryptMatrix->append((quint8)CryptedNum->at(3)+(quint8)(97));
    CryptMatrix->prepend((quint8)CryptedNum->at(2)+(quint8)CryptedNum->at(3)*3+(quint8)11  );
    CryptMatrix->prepend((quint8)CryptedNum->at(1)+(quint8)CryptedNum->at(2)*2+(quint8)34+(quint8)CryptedNum->at(3)*4  );
    CryptMatrix->prepend((quint8)CryptedNum->at(0)+(quint8)CryptedNum->at(1)*7+(quint8)75+(quint8)CryptedNum->at(3)*5  );
  */
    d3=(quint8)Crypt.at(3)-(quint8)(97);
    d2=(quint8)Crypt.at(2)-d3*3-(quint8)(11);
    d1=(quint8)Crypt.at(1)-d2*2-(quint8)(34)-d3*4;
    d0=(quint8)Crypt.at(0)-d1*7-(quint8)(75)-d3*5;

    DeCryptMatrix->append(d3);
    DeCryptMatrix->prepend(d2);
    DeCryptMatrix->prepend(d1);
    DeCryptMatrix->prepend(d0);

    QDataStream cmds(DeCryptMatrix,QIODevice::ReadWrite);
    cmds >> result;
    return result;
}


int ValidateKey(QString HexKey){

    QByteArray EncNum;
    QByteArray ConvertedKey; // full key converted from hexes;
    ConvertedKey=QByteArray(HexKey.toLatin1());
    ConvertedKey=ConvertedKey.fromHex(ConvertedKey);

    EncNum=ConvertedKey.left(3+4);
    EncNum=EncNum.right(4);
    quint32 RegNumber=DecryptInt(EncNum); // our registration number

    QByteArray ExtRawKey;
    ExtRawKey=ConvertedKey.left(3);
    ExtRawKey.append(ConvertedKey.right(ConvertedKey.count()-4-3));

    if (GenRawKey(RegNumber)==ExtRawKey)
        return RegNumber;
    else return -1;
}
