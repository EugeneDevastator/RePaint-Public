#include "NetSocket.h"
#include "SharedNetCode/NetStructures.h"

QByteArray AuthPacket::Serialize()
{
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::ReadWrite);
    stream << aType;
    stream << uname;
    stream << upass;
    return bytes;
}

// proper way of stream handling by gpt:
/*
 * QByteArray AuthPacket::SerializeInto()
{
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::ReadWrite);

    // Set the version for compatibility
    stream.setVersion(QDataStream::Qt_5_15); // Replace with your Qt version

    // Specify the endianness
    stream.setByteOrder(QDataStream::LittleEndian); // or QDataStream::BigEndian

    // Serialization
    stream << aType;
    if (stream.status() != QDataStream::Ok) {
        // Handle error
    }

    stream << uname;
    if (stream.status() != QDataStream::Ok) {
        // Handle error
    }

    stream << upass;
    if (stream.status() != QDataStream::Ok) {
        // Handle error
    }

    return bytes;
}
 */

AuthPacket AuthPacket::DeSerialize(QByteArray ba)
{
    AuthPacket data;
    QDataStream stream(&ba, QIODevice::ReadWrite);
    stream >> data.aType;
    stream >> data.uname;
    stream >> data.upass;
    return data;
}

//----------------- NETROOM --------

NetRoom::NetRoom()
{
    RemoteClients.clear();
}
void NetRoom::AddUser(NetSocket *client)
{

    RemoteClients.append(client);
    client->RoomIdx = RemoteClients.count() - 1;
    client->Room = this;
    client->IsInRoom = true;
}

void NetRoom::DelUser(NetSocket *client)
{

    RemoteClients.removeAt(client->RoomIdx);
    for (int i = 0; i < RemoteClients.size(); i++)
    {
        RemoteClients.at(i)->RoomIdx = i;
    }
    client->IsInRoom = false;
    client->Room = NULL;
}

// ----------------- NETHEAD

void NetPacketHeader::Serialize(QDataStream *ds)
{
    *ds << Id;
    *ds << Size;
}

// better realization:
//void NetPacketHeader::DeSerialize(QDataStream &ds)
//{
//    ds >> Id;
//    ds >> Size;
//}
void NetPacketHeader::DeSerialize(QDataStream *ds)
{
    *ds >> Id;
    *ds >> Size;
}

quint8 NetPacketHeader::GetSSize()
{
    QByteArray tmphead;
    QDataStream ds(&tmphead, QIODevice::ReadWrite);
    Serialize(&ds);
    return tmphead.count();
}

//---------USERSTATE

QByteArray NetUserState::Serialize()
{
    QByteArray *arry = new QByteArray(1, 'c');
    QDataStream tds(arry, QIODevice::ReadWrite);
    tds << UserState;
    tds << UserName;
    return *arry;
}

void NetUserState::DeSerialize(QByteArray ba)
{
    QDataStream tds(&ba, QIODevice::ReadOnly);
    tds >> UserState;
    tds >> UserName;
}

// ----------------------  additional functions
void G_SendData(quint8 Hid, QByteArray data, NetSocket *sock)
{
    NetPacketHeader HEAD;
    HEAD.Id = Hid; // please fix
    HEAD.Size = data.count();
    HEAD.Serialize(sock->SockStream);
    sock->Sck->write(data);
}
void G_SendUserStatus(NetSocket *user, NetSocket *dest)
{

    // note fix this shit!
    /*
    QString compstr=user->RegName;
    compstr.append(user->UserStateKind);
    QByteArray ldata=SZstring(user->RegName);
    ldata.append(user->UserStateKind);
    G_SendData("US",ldata,dest);
*/
    NetUserState us;
    us.UserName = user->RegName;
    us.UserState = user->UserStateKind;
    G_SendData(sdUserStat, us.Serialize(), dest);
}

void G_SendUserStatus(QString uname, int status, NetSocket *dest)
{
    NetUserState us;
    us.UserName = uname;
    us.UserState = status;
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
    HeaderKind = sdNONE;
    DataBuffer.clear();
    IsRegistered = true;
    Role = srNONE;
    IsInRoom = false;
    UserStateKind = suOn;
}

void NetSocket::ParseData()
{
    // int a =0;
    if (!IsReading)
    {

        while (Sck->bytesAvailable() > 0)
        {
            IsReading = true;
            qDebug() << "Waiting amount:" + QString::number(HeadSize) + "Got Data:" + QString::number(Sck->bytesAvailable());
            if (HeaderKind == sdNONE)
            { // waiting for head to come in
                if (Sck->bytesAvailable() >= HeadSize)
                {

                    HEAD.DeSerialize(SockStream);
                    RDataSize = HEAD.Size;
                    HeaderKind = HEAD.Id;
                    DataBuffer.clear();
                    // if (HeaderKind>sdSTOP)
                    //         this->Terminate(); //wrong head id = kill socket;
                }
            }
            if (HeaderKind != sdNONE)
            { // IsReading data packet; and in case it arrived in one piece continue to read.
                if (Sck->bytesAvailable() >= RDataSize)
                {
                    DataBuffer.append(this->Sck->read(RDataSize));
                }

                if ((DataBuffer.size() == RDataSize) &
                    (HeaderKind != sdNONE)) // retranslating the packet;
                {                          // when full data was read

                    if (IsRegistered)
                    {
                        emit SendDataObj(HEAD, DataBuffer, this); // this should be rewritten
                    }
                    else if (HeaderKind == sdAuth)
                    {
                        emit SendDataObj(HEAD, DataBuffer, this);
                    }
                    // for client socket
                    HeaderKind = sdNONE;
                    RDataSize = HeadSize;
                    DataBuffer.clear();
                }
            }
            QApplication::processEvents(QEventLoop::AllEvents, 2);
        }
        IsReading = false;
    }
}

void NetSocket::init()
{
    IsReading = false;
    blocker = false;
    IsImageOwner = false;
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
