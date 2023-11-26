#include "dbkeeper.h"
#include <QTextStream>
#include <QFile>
#include <QstringList>
#include <QDataStream>
#include <QSqlQuery>

DBKeeper::DBKeeper(QObject *parent)
{
  QString Appath= QApplication::applicationDirPath();

  /*QString fpath;
  fpath.append(Appath);
  fpath.append("/!udb.txt");
  DBPath=fpath;*/
  /*
  DBfile=new QFile(DBPath);
    DBfile->open(QFile::ReadWrite);
*/
SLType=slSMF;
//DBHost="127.0.0.1";
DBHost="46.252.193.146";
}
void DBKeeper::ReadBase(){

    if (SLType==slLocal) {
/*
        QStringList tokens;
        QString tLine;
        QTextStream txt(DBfile);
        tLine=txt.readLine();

        while (!tLine.isNull()){
            QStringList flist;
            UserRec cRec;

            tokens=tLine.split(QRegExp("\\s"),QString::SkipEmptyParts);
            cRec.GUID=tokens.at(0).toInt();
            cRec.Name=tokens.at(1);
            cRec.Pass=tokens.at(2);

            flist=tokens.at(3).split(',');
            for (int i=0;i<flist.count();i++){
                cRec.Friends.append(flist[i].toInt());
                }
            BASE.append(cRec);
            tLine="";
            tLine=txt.readLine();
        }

        BASE[0].Friends.append(8);
        SendMsg(QString::number(BASE.at(1).Friends.count()));

        for (int k=0;k<BASE.count();k++) {
            foreach(long i,BASE.at(k).Friends){
                if (BASE.count()>i)
                    BASE[k].FriendNames.append(BASE.at(i).Name);
                else
                    BASE[k].FriendNames.append("-404 FNF-");
                }
            }
        DBfile->close();
        emit SendMsg("LOCAL Base read, totatl records:"+QString::number(BASE.count()));
*/
}

    if (SLType==slSMF) {
/*
        DBName="test";
        DBLogin="root";
        DBPass="";

        TLogins="eUsers";
  */
        DBName="smfDB";
        DBLogin="readmin";
        DBPass="10qowox49j";

        Flogin="member_name";
        Fpass="passwd";



        TLogins="members";
        TFriends="members";

        Ffriends="buddy_list";
        FMainId="id_member";
        FlinkedID="id_member";

        //SaveBase();
        // --------------- SQL base code

        MainDB = QSqlDatabase::addDatabase("QMYSQL");
          MainDB.setHostName(DBHost);
          MainDB.setDatabaseName(DBName);
          MainDB.setUserName(DBLogin);
          MainDB.setPassword(DBPass);
         bool ok =MainDB.open();
         if (ok) emit SendMsg("connected to sqlDB for SMF forums at:"+DBHost);
         else {
             emit SendMsg("ERROR connecting to SQLDB with:"+DBLogin+" : "+DBPass);
             SendMsg(MainDB.lastError().text());
         }

      TableUsers = new QSqlTableModel(0,MainDB);

      TableUsers->setTable(TLogins);
      TableUsers->setEditStrategy(QSqlTableModel::OnFieldChange);
      TableUsers->select();

      QSqlQueryModel LoginQuery;
      LoginQuery.setQuery("SELECT * FROM "+TLogins,MainDB);
      QString basepass=LoginQuery.record(0).value(Flogin).toString();
      int rc=LoginQuery.rowCount();
       SendMsg("zero login:"+basepass);

        // ---------------
    }

}

void DBKeeper::SaveBase(){

    QStringList tokens;
    QString tLine;
    QFile *Sfile=new QFile(DBPath);
    Sfile->open(QFile::WriteOnly|QFile::Truncate);

    QTextStream stxt(Sfile);

    UserRec cRec;
    QStringList flist;
    QString cstr="";
    for (int i=0;i<BASE.count();i++){
        cstr.append(QString::number(BASE.at(i).GUID));
        cstr.append(" ");
        cstr.append(BASE.at(i).Name);
        cstr.append(" ");
        cstr.append(BASE.at(i).Pass);
        cstr.append(" ");
        for (int j=0;j<BASE[i].Friends.count();j++){
            cstr.append(QString::number(BASE.at(i).Friends.at(j)));
            cstr.append(",");
        }
        cstr.chop(1);
        cstr.append("\n");

       stxt << cstr;
       cstr="";
   }

Sfile->close();
delete Sfile;
//SendMsg(QString::number(BASE.at(0).Friends.at(0)));
TableUsers->submitAll();

}


LongList DBKeeper::OpenStringList(QString StList){

    QStringList idslist=StList.split(",");
    LongList idlist;
    foreach (QString st, idslist){
        idlist.append(st.toInt());
    }
return idlist;
}

QString DBKeeper::FoldLongList(LongList lst){
    QString res="";
    foreach (long id,lst){
        res.append(QString::number(id)+",");
    }
    res.chop(1);
    return res;
}
QList <QString> DBKeeper::GenFnames(LongList lst){


}

void DBKeeper::GatherFriends(long guid){
    //please

  QList <QString> FNlist;
  QList <long> FIDlist;
// getting friendlist
  QSqlQueryModel qrFlist;
  qrFlist.setQuery("select * from "+TFriends+" where "+FlinkedID+" ="+QString::number(guid),MainDB);
  QString strFlist=qrFlist.record(0).value(Ffriends).toString();

  //prepending self to be always first in list
  if (strFlist.length()>0)
        strFlist.prepend(QString::number(guid)+",");
  else
        strFlist.prepend(QString::number(guid));

// generating corresponding name list
 QSqlQueryModel sqt;
 sqt.setQuery("select * from "+TLogins+" where "+FMainId+" in ("+strFlist+") order by "+ Flogin,MainDB);
 //FIDlist.append(guid);
 //FNlist.append(qrFlist.record(0).value(Flogin).toString());

 emit SendMsg("gathering:"+strFlist +"count:" + QString::number(sqt.rowCount()));

 for (int i=0;i<sqt.rowCount();i++){
    FNlist.append(sqt.record(i).value(Flogin).toString());
    FIDlist.append(sqt.record(i).value(FMainId).toLongLong());
    emit SendMsg("gathering:"+FNlist.at(i));
  }
 // emit SendFlists(guid,Flist,FNlist);

emit SendFlists(guid,FIDlist,FNlist);


}

void DBKeeper::TryAuthSQL(quint8 atype, c_rsock *client){
    if (!client->RegName.isEmpty()){
        /* no more hardware registration.
        if (atype==atRegister) {
           SendMsg("Trying to reg name:"+client->RegName);
           QString uname=client->RegName;
           QString pass=client->RegPass;
           //please optimize
           //for (int i=0;i<BASE.count();i++){
             //   if (BASE.at(i).Name.toLower().compare(client->RegName.toLower())==0) {
               //     SendMsg("rgister fail:"+client->RegName);
                 //   emit FailReg(client);
                 //   return;
                  //  }
               // }
           //check for present name;
           QSqlQueryModel lfn; lfn.setQuery("SELECT * FROM eUsers WHERE Name = \"" + uname+"\"",MainDB);
           if (lfn.rowCount()>0) {
               SendMsg("rgister fail:"+client->RegName);
               emit FailReg(client);
               return;
            }

           // find last guid;
           QSqlQueryModel sqmax; sqmax.setQuery("select MAX(GUID) from eusers",MainDB);
           long maxid= sqmax.record(0).value(0).toLongLong();
           maxid=maxid+1;

           // add data to table;
           QSqlQuery query(MainDB);
           query.prepare("INSERT INTO eusers (GUID, Name, Pass, Friends) VALUES (?, ?, ?, ?);");
           query.addBindValue(QString::number(maxid));
           query.addBindValue(client->RegName);
           query.addBindValue(client->RegPass);
           query.addBindValue("");
           query.exec();
           client->UGUID = maxid;
           client->FGuids.append(client->UGUID);

           emit SendMsg("User registered:"+client->RegName);
           emit GoodAuth(client);
           return;
        }
     else
        */
        if (atype==atLogin){

            QSqlQueryModel LoginQuery;
            LoginQuery.setQuery("SELECT * FROM " + TLogins + " WHERE " +Flogin+ " = \"" + client->RegName + "\"",MainDB);
            QString basepass=LoginQuery.record(0).value(Fpass).toString();
            int rc=LoginQuery.rowCount();

            //--------------- cyphering the password
                    QByteArray ba;
                    QByteArray cst;
                    cst.append(client->RegName.toLower().toLatin1());
                    cst.append(client->RegPass.toLatin1());
                    ba=QCryptographicHash::hash(cst,QCryptographicHash::Sha1);
                    QString CypherPass = (ba.toHex());
                    SendMsg(client->RegName+", "+QString(cst)+" hash:"+CypherPass);


            if (rc>0 && basepass.compare(CypherPass)==0) {
                LongList flist;
                    flist = OpenStringList(LoginQuery.record(0).value("buddy_list").toString());
                    client->UGUID=LoginQuery.record(0).value(FMainId).toLongLong();

                    SendMsg("friends string:"+LoginQuery.record(0).value("buddy_list").toString());
                    /*
                    foreach (int f,flist){
                        client->FGuids.append(f);
                         }
                         */
                    client->RegName=LoginQuery.record(0).value(Flogin).toString();
                    SendMsg("login success:"+client->RegName+" "+QString::number(client->UGUID));
                    emit GoodAuth(client);
                    return;



                    }
               else { emit FailLogin(client); return; }
            }

            emit FailLogin(client); return;
    }
    emit FailLogin(client); return;
}


void DBKeeper::TryAuth(quint8 atype, c_rsock *client){
if (!client->RegName.isEmpty()){
    if (atype==atRegister) {
       SendMsg("Trying to reg name:"+client->RegName);
       QString uname=client->RegName;
       QString pass=client->RegPass;
       //please optimize
       for (int i=0;i<BASE.count();i++){
            if (BASE.at(i).Name.toLower().compare(client->RegName.toLower())==0) {
                SendMsg("rgister fail:"+client->RegName);
                emit FailReg(client);
                return;
                }
            }
       UserRec cRec;
       //tokens=tLine.split(QRegExp("\\s"),QString::SkipEmptyParts);
       cRec.GUID=BASE.count();
       cRec.Name=client->RegName;
       cRec.Pass=client->RegPass;
       cRec.Friends.append(cRec.GUID);
       cRec.FriendNames.append(client->RegName);
       BASE.append(cRec);
       SaveBase();
       client->UGUID = cRec.GUID;
       emit SendMsg("User registered:"+cRec.Name);
       emit GoodAuth(client);
    }
    else
    if (atype==atLogin){
        for (int i=0;i<BASE.count();i++){
            if (BASE.at(i).Name.toLower().compare(client->RegName.toLower())==0) {
                if (BASE.at(i).Pass.compare(client->RegPass)==0) {
                    client->UGUID=BASE.at(i).GUID;
                    foreach (int f,BASE.at(i).Friends){
                        client->FGuids.append(f);
                        }
                    client->RegName=BASE.at(i).Name;
                    SendMsg("login success:"+client->RegName);
                    emit GoodAuth(client);
                    return;
                    }
                else { emit FailLogin(client); return; }

                }
        }
    emit FailLogin(client); // no records found.
    return;
    }

}
}
/* // absolete functions
void DBKeeper::TryLog(c_rsock *client){
    //please optimize
    for (int i=0;i<BASE.count();i++){
        if (BASE.at(i).Name.toLower().compare(client->RegName.toLower())==0) {
            if (BASE.at(i).Pass.compare(client->RegPass)==0) {
                client->UGUID=BASE.at(i).GUID;
                foreach (int f,BASE.at(i).Friends){
                    client->FGuids.append(f);
                }
                client->RegName=BASE.at(i).Name;
                emit GoodLogin(client);
                return;
                }
            else { emit FailLogin(client); return; }
           }

}
    emit FailLogin(client);
    return;
}
void DBKeeper::TryReg(c_rsock *client){
    if (!client->RegName.isEmpty()){
    SendMsg("Trying to reg name:"+client->RegName);
       QString uname=client->RegName;
       QString pass=client->RegPass;
       //please optimize
       for (int i=0;i<BASE.count();i++){
            if (BASE.at(i).Name.toLower().compare(client->RegName.toLower())==0) {
                emit FailReg(client);
                return;
            }
       }
       UserRec cRec;
       //tokens=tLine.split(QRegExp("\\s"),QString::SkipEmptyParts);
       cRec.GUID=BASE.count();
       cRec.Name=client->RegName;
       cRec.Pass=client->RegPass;
       cRec.Friends.append(cRec.GUID);
       cRec.FriendNames.append(client->RegName);
       BASE.append(cRec);
       SaveBase();
       client->UGUID = cRec.GUID;
       emit SendMsg("User registered:"+cRec.Name);
       emit GoodReg(client);
}


}

*/

void DBKeeper::DBFindFriends(QString wc,c_rsock *caller){

    //select * from eusers limit offset,count
    QString cnt="15";
    QSqlQueryModel qrFind;
    QString qst="select  * from "+TLogins+ " where "+Flogin+" like '"+ wc +"' LIMIT "+cnt;
    qrFind.setQuery(qst,MainDB);


    QStringList Results;
    for (int i=0;i<15;i++){
        if (!qrFind.record(i).value(Flogin).isNull())
            Results.append(qrFind.record(i).value(Flogin).toString());
    }
    emit SendMsg("SQL People search for:"+wc+" Found count:"+QString::number(qrFind.rowCount()));
    emit SendPeople(Results,caller);


    /*  obsolete algorithm for text base.
    QRegExp rx(wc);
    QStringList results;
    int count=15;
    for (long i=0;i<BASE.count();i++){
         if (rx.exactMatch(BASE.at(i).Name.toLower())) {
             if (BASE.at(caller->UGUID).Friends.contains(i)==false){
                results.append(BASE.at(i).Name);
                count--;
                if (count==0) {
                    //emit SendFriends(results,*caller);
                    return;
                }
             }
             return;
         }
    }
    */
}


void DBKeeper::DBAddFriend(QString name,c_rsock *caller){
    //routine to add friend
    /*
    for (long i=0;i<BASE.count();i++){
         if (BASE.at(i).Name.toLower().compare(name.toLower())==0) {
             if (BASE.at(caller->UGUID).Friends.contains(i)==false){
                 BASE[caller->UGUID].Friends.append(i);
                 BASE[caller->UGUID].FriendNames.append(BASE.at(i).Name);
                 // please solve problem with friend addition
                 SaveBase();
                 emit SendNewFriend(BASE.at(i).Name,caller);
             }
             return;
         }
    }
    */
    //-------------------------------
    QSqlQueryModel lfn;
    lfn.setQuery("SELECT * FROM "+TLogins+" WHERE "+Flogin+" = \"" + caller->RegName +"\"",MainDB);
    if (lfn.rowCount()>0) {
        QString strFlist = lfn.record(0).value(Ffriends).toString();
    LongList flist = OpenStringList(strFlist);


    QSqlQueryModel frec;
    frec.setQuery("SELECT * FROM "+TLogins+" WHERE "+Flogin+" = \"" + name+"\"",MainDB);
    /*
    if (flist.contains(frec.record(0).value("guid").toLongLong())==false){
        flist.append(frec.record(0).value("guid").toLongLong());
        }
    */

    //update friend list
    if (!flist.contains(frec.record(0).value(FMainId).toLongLong())){
    QSqlQuery addqr(0,MainDB);

    addqr.prepare("UPDATE "+TLogins+" SET "+Ffriends+"=? WHERE "+FlinkedID+"=?");
        if (strFlist.length()<1) addqr.addBindValue(QString::number(frec.record(0).value(FMainId).toLongLong()));
        else                     addqr.addBindValue(strFlist+","+QString::number(frec.record(0).value(FMainId).toLongLong()));
        addqr.addBindValue(QString::number(caller->UGUID));
        addqr.exec();

emit SendMsg("Friend added: "+strFlist);
emit SendNewFriend(frec.record(0).value(Flogin).toString(),caller);
    }
    }

emit SendMsg("-----------------");
}

void DBKeeper::DBDelFriend(QString name,c_rsock *caller){
    //QString cname=name.toLower();
    /*
    long fidx;
    for (long i=0;i<BASE.at(caller->UGUID).Friends.count();i++){
            if (BASE[caller->UGUID].FriendNames.at(i).compare(name)==0) {
                fidx=BASE.at(caller->UGUID).Friends.at(i);
                BASE[caller->UGUID].Friends.removeAt(i);
                BASE[caller->UGUID].FriendNames.removeAt(i);
                 // please solve problem with friend addition
                 SaveBase();
                 emit SendDeadFriend(BASE.at(fidx).Name,caller);
                 SendMsg("friend removal:"+BASE.at(fidx).Name);
             }
             return;
         }
*/

if (SLType==slSMF) {

    QSqlQueryModel lfn;
        // this is a stitch!
    lfn.setQuery("SELECT * FROM "+TLogins+" WHERE "+Flogin+" = \"" + caller->RegName +"\"",MainDB);
    //lfn.setQuery("SELECT * FROM "+TFriends+" WHERE "+FlinkedID+" = \"" + caller->UGUID +"\"",MainDB);
    if (lfn.rowCount()>0) {
        QString strFlist = lfn.record(0).value(Ffriends).toString();
        emit SendMsg("list found for deletion: "+strFlist);
        LongList flist = OpenStringList(strFlist);

        QSqlQueryModel frec;
        frec.setQuery("SELECT * FROM "+TLogins+" WHERE "+Flogin+" = \"" + name+"\"",MainDB);
        if (flist.contains(frec.record(0).value(FMainId).toLongLong())==true){
            flist.removeOne(frec.record(0).value(FMainId).toLongLong());
            }

        //update friend list
        emit SendMsg("list update for deletion: "+FoldLongList(flist));

        QSqlQuery addqr(0,MainDB);
        addqr.prepare("UPDATE "+TFriends+" SET "+Ffriends+"=? WHERE "+FlinkedID+"=?");
            QString foldlist=FoldLongList(flist);
            addqr.addBindValue(foldlist);
            addqr.addBindValue(QString::number(caller->UGUID));
            addqr.exec();

        }
    }


}


