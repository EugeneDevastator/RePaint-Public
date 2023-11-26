#include "NetSocket.h"

QByteArray sAuth::Serialize()
{
    QByteArray *sa = new QByteArray;
    QDataStream tds(sa, QIODevice::ReadWrite);
    tds << aType;
    tds << uname;
    tds << upass;

    return *sa;
}

sAuth sAuth::DeSerialize(QByteArray ba)
{
    sAuth dsz;
    QDataStream tds(&ba, QIODevice::ReadWrite);
    tds >> dsz.aType;
    tds >> dsz.uname;
    tds >> dsz.upass;
    return dsz;
}

//----------------- NETROOM --------

NetRoom::NetRoom()
{
    RCList.clear();
}
void NetRoom::AddUser(NetSocket *client)
{

    RCList.append(client);
    client->RoomIdx = RCList.count() - 1;
    client->Room = this;
    client->inroom = true;
}

void NetRoom::DelUser(NetSocket *client)
{

    RCList.removeAt(client->RoomIdx);
    for (int i = 0; i < RCList.size(); i++)
    {
        RCList.at(i)->RoomIdx = i;
    }
    client->inroom = false;
    client->Room = NULL;
}

// ----------------- NETHEAD

void stNetHead::Serialize(QDataStream *ds)
{
    *ds << Hid;
    *ds << Hsize;
}

void stNetHead::DeSerialize(QDataStream *ds)
{
    *ds >> Hid;
    *ds >> Hsize;
}

quint8 stNetHead::GetSSize()
{
    QByteArray tmphead;
    QDataStream ds(&tmphead, QIODevice::ReadWrite);
    Serialize(&ds);
    return tmphead.count();
}

//---------USERSTATE

QByteArray stUserState::Serialize()
{
    QByteArray *ba = new QByteArray(1, 'c');
    QDataStream tds(ba, QIODevice::ReadWrite);
    tds << Ustate;
    tds << Uname;
    return *ba;
}

void stUserState::DeSerialize(QByteArray ba)
{
    QDataStream tds(&ba, QIODevice::ReadOnly);
    tds >> Ustate;
    tds >> Uname;
}

// ----------------------  additional functions
void G_SendData(quint8 Hid, QByteArray data, NetSocket *sock)
{
    stNetHead HEAD;
    HEAD.Hid = Hid; // please fix
    HEAD.Hsize = data.count();
    HEAD.Serialize(sock->SockStream);
    sock->Sck->write(data);
}
void G_SendUserStatus(NetSocket *user, NetSocket *dest)
{

    // note fix this shit!
    /*
    QString compstr=user->RegName;
    compstr.append(user->uStatus);
    QByteArray ldata=SZstring(user->RegName);
    ldata.append(user->uStatus);
    G_SendData("US",ldata,dest);
*/
    stUserState us;
    us.Uname = user->RegName;
    us.Ustate = user->uStatus;
    G_SendData(sdUserStat, us.Serialize(), dest);
}

void G_SendUserStatus(QString uname, int status, NetSocket *dest)
{
    stUserState us;
    us.Uname = uname;
    us.Ustate = status;
    G_SendData(sdUserStat, us.Serialize(), dest);
}

//------------------------ MAIN CLASS DEFINITION -----------------------

NetSocket::NetSocket(QObject *parent) : QObject(parent)
{
    RegName = "nullname";
    RegPass = "nullpass";

    Hid = "ZZ";
    Sck = new QTcpSocket;
    HeadSize = HEAD.GetSSize();
    // HeadSize=3;
    Role = srGlobal;
    RDataSize = HeadSize;
    DataState = sdNONE;
    DataBuffer.clear();
    Registered = true;
    Role = srNONE;
    inroom = false;
    uStatus = suOn;
}

void NetSocket::ParseData()
{
    // int a =0;
    if (!reading)
    {

        while (Sck->bytesAvailable() > 0)
        {
            reading = true;
            qDebug() << "Waiting amount:" + QString::number(HeadSize) + "Got Data:" + QString::number(Sck->bytesAvailable());
            if (DataState == sdNONE)
            { // waiting for head to come in
                if (Sck->bytesAvailable() >= HeadSize)
                {

                    HEAD.DeSerialize(SockStream);
                    RDataSize = HEAD.Hsize;
                    DataState = HEAD.Hid;
                    DataBuffer.clear();
                    // if (DataState>sdSTOP)
                    //         this->Terminate(); //wrong head id = kill socket;
                }
            }
            if (DataState != sdNONE)
            { // reading data packet; and in case it arrived in one piece continue to read.
                if (Sck->bytesAvailable() >= RDataSize)
                {
                    DataBuffer.append(this->Sck->read(RDataSize));
                }

                if ((DataBuffer.size() == RDataSize) &
                    (DataState != sdNONE)) // retranslating the packet;
                {                          // when full data was read

                    if (Registered)
                    {
                        emit SendDataObj(HEAD, DataBuffer, this); // this should be rewritten
                    }
                    else if (DataState == sdAuth)
                    {
                        emit SendDataObj(HEAD, DataBuffer, this);
                    }
                    // for client socket
                    DataState = sdNONE;
                    RDataSize = HeadSize;
                    DataBuffer.clear();
                }
            }
            QApplication::processEvents(QEventLoop::AllEvents, 2);
        }
        reading = false;
    }
}

void NetSocket::init()
{
    reading = false;
    blocker = false;
    imageowner = false;
    connect(this->Sck, SIGNAL(readyRead()), this, SLOT(ParseData()));
    connect(this->Sck, SIGNAL(disconnected()), this, SLOT(Terminate()));
    SockStream = new QDataStream(this->Sck);
    emit SendMsg("rsock:new client connected");
}

void NetSocket::Terminate()
{
    emit SendMsg("Terminator called.");
    emit RSDisconnect(this);
    this->Sck->close();
    this->deleteLater();
    delete SockStream;
}
