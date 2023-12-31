#include "NetServer.h"

NetServer::NetServer(QObject *parent) : QObject(parent)
{
    GlobalRoomEnabled = false;

    MainPort = 33789;
    StartSession();
    NetMode = emNone;
}

void NetServer::StartSession()
{

    // NetMode=eNone;
    QNetworkConfigurationManager manager;
    QNetworkConfiguration config = manager.defaultConfiguration();
    NetSession = new QNetworkSession(config);
    NetSession->open();

    LServer = new QTcpServer;
    RSockClientList.clear();
}

void NetServer::S_Disconnect()
{
    if (NetMode != emNone)
    {
        NetMode = emNone;
        foreach (NetSocket *client, RSockClientList)
        {
            client->Sck->close();
        }
        RSockClientList.clear();
        LServer->close();
        emit ServerOff();
    }
}

// ------------------ S E R V E R   functions -----------------
// ------------------------------------------------------------
void NetServer::SG_Start()
{
    if (NetMode == emNone)
    {
        NetMode = emGServer;
        if (!LServer->isListening())
        {
            LServer->listen(QHostAddress::Any, MainPort);
            connect(LServer, SIGNAL(newConnection()), this, SLOT(S_GetClient()));
        }

        QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        // use the first non-localhost IPv4 address
        for (int i = 0; i < ipAddressesList.size(); ++i)
        {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address())
            {
                ipAddress = ipAddressesList.at(i).toString();
                break;
            }
        }
        // emit SendStatus("Server created at:"+ipAddress);
        emit SendServIp(ipAddress);
    }
}

void NetServer::S_Start()
{
    if (NetMode == emNone)
    {
        NetMode = emServer;
        LServer = new QTcpServer;
        if (!LServer->isListening())
        {
            LServer->listen(QHostAddress::Any, MainPort);
            connect(LServer, SIGNAL(newConnection()), this, SLOT(S_GetClient()));
        }

        QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        // use the first non-localhost IPv4 address
        for (int i = 0; i < ipAddressesList.size(); ++i)
        {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address())
            {
                ipAddress = ipAddressesList.at(i).toString();
                break;
            }
        }
        // emit SendStatus("Server created at:"+ipAddress);
        emit SendServIp(ipAddress);
    }
}
void NetServer::S_Stop()
{
    LServer->close();
    NetMode = emNone;
}
void NetServer::S_GetClient()
{

    // (c1) connection step 1, put client in pool of unregistered clients.

    if (LServer->hasPendingConnections())
    {

        NetSocket *NewSock = new NetSocket;
        NewSock->Sck = LServer->nextPendingConnection();
        NewSock->init();
        connect(NewSock, SIGNAL(RSDisconnect(NetSocket *)), this, SLOT(S_KillClient(NetSocket *)));
        connect(NewSock, SIGNAL(SendDataObj(stNetHead, QByteArray, NetSocket *)), this, SLOT(S_Retranslate(stNetHead, QByteArray, NetSocket *)));
        emit SendStatus("New client");
        NewSock->Registered = false;

        if (NetMode == emServer)
        {
            G_SendData(sdLogin, SZstring(QString("Your name.")), NewSock); // send login request, ask for auth.
        }
        else if (NetMode == emGServer)
        {
            G_SendData(sdLogin, SZstring(QString("Log in to global server.")), NewSock);
        }
    }
}

void NetServer::S_UpdateUserStatus(NetSocket *user, quint8 status)
{
}

void NetServer::S_RegisterSock(NetSocket *sock)
{

    // (c3) step 3 allow registration

    if (!NameClientList.contains(sock->RegName))
    {
        RSockClientList.append(sock);
        sock->Registered = true;
        sock->listIdx = RSockClientList.count() - 1;
        G_SendData(sdLoginS, SZstring(sock->RegName), sock);
        NameClientList[sock->RegName] = sock; // link name with socket.

        G_SendUserStatus(sock, sock);
        QString Uname = sock->RegName;

        if (NetMode == emServer)
        {
            // confirm to client that he connected

            // sending your nick to everyone. and everyone's to you.
            foreach (NetSocket *client, RSockClientList)
            {
                if (client->listIdx != sock->listIdx)
                    G_SendUserStatus(sock, client);
                if (client->listIdx != sock->listIdx)
                    G_SendUserStatus(client, sock);
            }
            // Getting image if you are not only in lobby
            // please maybe some mistakke with lobby counter rclient list will be not empty when all are in roms.
            if ((GlobalRoomEnabled) && (RSockClientList.count() > 1))
            {
                //  HEAD.Hid=sdLock; // lock fucking everyone whos not in room!
                // S_ResendToAll(sdLock,QByteArray(1,c),client,true);
                G_SendData(sdReqImg, SZstring(sock->RegName), RSockClientList[0]); // please  fix for rooms ask one for image
                //  sock->blocker=true;
            }
            emit LoginS(Uname);
            emit SendStatus("NETM:New client registered:" + Uname);
        }

        else if (NetMode == emGServer)
        {

            // confirm to client that he connected
            emit LoginS(Uname);
            emit SendStatus("New client registered:" + Uname);
            GuidClientList[sock->UGUID] = sock;
            emit NeedFriends(sock->UGUID);
        }
    }

    else
    {

        S_KillClient(sock);
        //    sock->Terminate();
    }
}

void NetServer::S_GetFriends(long DGUID, QList<long> fguids, QList<QString> fnames)
{

    NetSocket *user = GuidClientList.value(DGUID);
    user->FGuids.clear();
    for (int k = 0; k < fguids.count(); k++)
    {
        NetSocket *sfriend;
        if (GuidClientList.contains(fguids.at(k)))
        {
            sfriend = GuidClientList.value(fguids.at(k));
            G_SendUserStatus(sfriend, user); // send statuses of friends to local user
            G_SendUserStatus(user, sfriend); // and localuser to friends.
        }
        else
        {
            G_SendUserStatus(fnames.at(k), suOff, user);
        }
        user->FGuids.append(fguids.at(k));
    }
}

void NetServer::S_GetPeople(QStringList pList, NetSocket *asker)
{

    // please
    QString FoldList;
    foreach (QString st, pList)
    {
        FoldList = FoldList + st + ",";
    }
    FoldList.chop(1);

    G_SendData(sdPeopleResults, SZstring(FoldList), asker);
}

void NetServer::S_AddToRoom(NetSocket *client, QString rname)
{

    NetRoom *NR = NameClientList.value(rname)->Room;
    // new room is created in previous step.

    // find room by user's name
    // if (NClientList.value(rname)->inroom==true & client->inroom==false){
    if (client->inroom == false)
    {
        /*foreach (NetSocket*sck,caller->Room->RCList){
            G_SendData("RP",QByteArray(caller->RegName.toAscii()),sck); //user lefts room
           }*/

        NR->AddUser(client);
        client->Role = srServ;
        foreach (NetSocket *sck, NR->RCList)
        {
            G_SendData(sdRoomJoin, SZstring(client->RegName), sck); // send self status to everyone in room (+self)
            // and get all the roommates except self.
            if (client->listIdx != sck->listIdx)
            {
                G_SendData(sdRoomJoin, SZstring(sck->RegName), client);
                // G_SendUserStatus(client->RegName,suMemberFree,sck);
            }
        }
        if (NR->RCList.count() > 1)
            S_SetUserStatus(client, suMemberFree);
        else
            S_SetUserStatus(client, suRoomFree);
    }
}

void NetServer::S_DelFromRoom(NetSocket *client)
{
    if (client->inroom == true)
    {
        NetRoom *NR = client->Room;
        QString DelName = client->RegName;
        foreach (NetSocket *sck, client->Room->RCList)
            G_SendData(sdRoomPart, SZstring(DelName), sck); // message for roommates to delete parting one

        client->Room->DelUser(client);

        if (client->Role == srServ)
        { // transit server role to first in line
            client->Role = srClient;
            if (NR->RCList.count() > 0)
            {
                NR->RCList[0]->Role = srServ;
                NR->RCList[0]->imageowner = true;
                /*stUserState us;
                us.Uname=(NR->RCList.at(0)->RegName);
                us.Ustate=suRoomFree;*/
                S_SetUserStatus(NR->RCList[0], suRoomFree); // marking new owner
                // foreach (NetSocket *sck,NR->RCList)
                // G_SendData(sdUserStat,us.Serialize(),sck);
                //   G_SendUserStatus(NR->RCList.at(0)->RegName,suRoomFree,sck);
            }

            else
                delete NR;
        }
    }
}
void NetServer::S_SetUserStatus(NetSocket *client, int status)
{
    client->uStatus = status;
    /*//in any case send to roommates.
    if (client->inroom==true)
        foreach (NetSocket *sck,client->Room->RCList) G_SendUserStatus(client,sck);
*/
    if (NetMode == emServer)
        foreach (NetSocket *sck, RSockClientList)
            G_SendUserStatus(client, sck);

    else if (NetMode == emGServer)
    {
        foreach (int fg, client->FGuids)
        {
            if (GuidClientList.value(fg) != NULL)
                G_SendUserStatus(client, GuidClientList.value(fg));
        }
        if (client->inroom)
        {
            foreach (NetSocket *sck, client->Room->RCList)
                G_SendUserStatus(client, sck);
        }
    }
}

void NetServer::S_Retranslate(stNetHead HEAD, QByteArray fulldata, NetSocket *caller)
{

    if (HEAD.Hid == sdAuth)
    {
        // (c2) client has sent auth, check it.
        sAuth auth;
        auth = auth.DeSerialize(fulldata);
        auth.aType = atLogin;
        caller->RegName = auth.uname;
        caller->RegPass = auth.upass; // GenForumHash(auth.uname,auth.upass);

        if (NetMode == emGServer)
        {
            emit SendStatus("attempt to login: " + caller->RegName);
            // S_RegisterSock(caller);
            emit SendAuth(auth.aType, caller); // ask db manager for parsing.
        }

        else if (NetMode == emServer)
        {
            int a = 1;
            QString Basename = caller->RegName;
            for (int i = 0; i < RSockClientList.size(); i++)
            {
                if (RSockClientList.at(i)->RegName.toLower().compare(caller->RegName.toLower()) == 0)
                {
                    caller->RegName = Basename + "_" + QString::number(a);
                    i = 0;
                    a++;
                }
            }
            S_RegisterSock(caller);
        }
    }

    // retranslating only communication messages
    if (HEAD.Hid == sdFriendSeek)
    { // friend search;
        if (NetMode == emGServer)
        {
            QString fname = DSZstring(fulldata); //(fulldata.right(fulldata.size()-caller->HeadSize));
            SendStatus("Friend search for:" + fname);
            emit DBFindUser(fname, caller);
        }
    }
    if (HEAD.Hid == sdFriendAdd)
    { // friend search;
        if (NetMode == emGServer)
        {
            QString fname = DSZstring(fulldata); //(fulldata.right(fulldata.size()-caller->HeadSize));
            SendStatus("Friend add for:" + fname);
            emit DBAddFriend(fname, caller);
        }
    }
    if (HEAD.Hid == sdFriendDel)
    { // friend delete;
        if (NetMode == emGServer)
        {
            QString fname = DSZstring(fulldata);
            SendStatus("dead fiend search for:" + fname);
            emit DBDelFriend(fname, caller);
        }
    }
    /////////////////  Create Room  //////////////
    if (HEAD.Hid == sdRoomCreate)
    {
        // presume user can create room only if he is stranded.
        if (caller->inroom == false)
        {
            caller->Room = new NetRoom;
            caller->Role = srServ;
            S_AddToRoom(caller, caller->RegName);
        }
    }
    ///////////////// Join Room //////////////

    if (HEAD.Hid == sdRoomJoin)
    {                                        // join room
        QString uname = DSZstring(fulldata); // name of dest room
        if (uname.compare(caller->RegName) != 0)
        {
            if (NameClientList.contains(uname))
                if (NameClientList.value(uname)->Room != NULL)
                    if ((caller->inroom == false) & (caller->RegName != uname))
                    {
                        caller->Role = srClient;
                        S_SetUserStatus(caller, suMemberFree);
                        S_AddToRoom(caller, uname);
                        G_SendData(sdReqImg, SZstring(caller->RegName), caller->Room->RCList[0]);
                    }
        }
    }
    if (HEAD.Hid == sdRoomPart)
    { // room part
        S_SetUserStatus(caller, suOn);
        S_DelFromRoom(caller);
    }

    if (((HEAD.Hid == sdAction) | (HEAD.Hid == sdSection)))
    {
        if (NetMode == emServer)
            S_ResendToRest(HEAD, fulldata, caller, caller->inroom);

        else if (NetMode == emGServer)
        {
            if (caller->inroom)
                S_ResendToRest(HEAD, fulldata, caller, caller->inroom);
        }
    }

    if ((HEAD.Hid == sdLAction) | (HEAD.Hid == sdGetMsg))
    {
        if (NetMode == emServer)
            S_ResendToAll(HEAD, fulldata, caller, caller->inroom);

        else if (NetMode == emGServer)
        {
            if (caller->inroom)
                S_ResendToAll(HEAD, fulldata, caller, caller->inroom);
        }
    }

    if (HEAD.Hid == sdConfirmImg)
    {
        // unlock dependent users;
    }
    if (HEAD.Hid == sdGetImg)
    {
        QDataStream ds(&fulldata, QIODevice::ReadWrite);
        QByteArray ELI(1, 'c');
        QString asker;
        ds >> asker;
        ds >> ELI;
        // assume this blockk kwill be only executed on client;
        if (NameClientList.contains(asker))
        {
            G_SendData(sdGetImg, ELI, NameClientList.value(asker));
        }
    }
}
// ------------- end of retranslation routine ------------

void NetServer::S_FoundFriend(QString name, NetSocket *caller)
{
    if (NameClientList.contains(name))
    {
        G_SendUserStatus(NameClientList.value(name), caller);
    }
    else
        G_SendUserStatus(name, suOff, caller);
    emit SendStatus("transfered friend to userlist: " + name);
}

void NetServer::S_DeleteFriend(QString name, NetSocket *caller)
{
    if (NetMode == emGServer)
    {
        G_SendData(sdUserDel, SZstring(name), caller);
    }
}

void NetServer::S_ResendToRest(stNetHead HEAD, QByteArray fulldata, NetSocket *caller, bool inroom)
{
    if (inroom)
    {
        for (int i = 0; i < caller->Room->RCList.size(); i++)
        {
            if (i != caller->RoomIdx)
            {
                // caller->Room->RCList.at(i)->Sck->write(fulldata);
                HEAD.Hsize = fulldata.count();
                HEAD.Serialize(caller->Room->RCList[i]->SockStream);
                caller->Room->RCList[i]->Sck->write(fulldata);
            }
        }
    }
    else if (GlobalRoomEnabled)
        for (int i = 0; i < RSockClientList.size(); i++)
        {
            if (i != caller->listIdx)
                if (RSockClientList.at(i)->inroom == false)
                {
                    HEAD.Hsize = fulldata.count();
                    HEAD.Serialize(RSockClientList[i]->SockStream);
                    RSockClientList[i]->Sck->write(fulldata);
                }
        }
    QApplication::processEvents(QEventLoop::AllEvents, 2);
}

void NetServer::S_ResendToAll(stNetHead HEAD, QByteArray fulldata, NetSocket *caller, bool inroom)
{
    if (inroom)
        for (int i = 0; i < caller->Room->RCList.size(); i++)
        {
            HEAD.Hsize = fulldata.count();
            HEAD.Serialize(caller->Room->RCList[i]->SockStream);
            caller->Room->RCList[i]->Sck->write(fulldata);
        }
    else
        for (int i = 0; i < RSockClientList.size(); i++)
        {
            if (RSockClientList.at(i)->inroom == false)
            {

                HEAD.Hsize = fulldata.count();
                HEAD.Serialize(RSockClientList[i]->SockStream);
                RSockClientList[i]->Sck->write(fulldata);
            }
        }
    QApplication::processEvents(QEventLoop::AllEvents, 2);
}

void NetServer::S_KillClient(NetSocket *caller)
{
    // please optimize for multiclientism

    QString DelName = caller->RegName;
    if (caller->Registered == true)
    {
        if (NetMode == emGServer)
            GuidClientList.remove(caller->UGUID);

        NameClientList.remove(DelName);
        RSockClientList.removeAt(caller->listIdx);
        for (int i = 0; i < RSockClientList.size(); i++)
        {
            RSockClientList.at(i)->listIdx = i;
        }

        if (NetMode == emServer)
        { // delete from main chat in case of LAN server.
            foreach (NetSocket *sck, RSockClientList)
                G_SendData(sdUserDel, SZstring(DelName), sck);
            if (caller->imageowner)
                RSockClientList[0]->imageowner = true;
        }
        // remove client from room
        // if caller->inroom
        S_DelFromRoom(caller);

        emit ClientEnd(DelName);
    }
    // emit ClientEnd(DelName);
    // emit SendStatus("Client killed, new size:"+QString::number(RClientList.size()));
    // delete caller;
}

// --------------------- I N P U T   functions --------------------
void NetServer::S_SendFail(NetSocket *caller)
{

    G_SendData(sdFAIL, SZstring(QString("Attempt to enter Failed")), caller);
    S_KillClient(caller);
}

/*
  ----------- hashing for smf forums -----------
QString NetClient::GenForumHash(QString uname, QString upass){
    QByteArray ba;
    QByteArray cst;
    cst.append(uname.toLower().toLatin1());
    cst.append(upass.toLatin1());
    ba=QCryptographicHash::hash(cst,QCryptographicHash::Sha1);
    return QString(ba.toHex());
}
*/

// $this->user_salt = "$1$".substr($this->user_info[user_code], 0, 8)."$";
//	  $user_salt = "$1$".substr($signup_code, 0, 8)."$";
// $crypt_password = crypt($signup_password, $user_salt);
//$1$LKUcDhZo$KW12l/EcrhV.4dWKBFGw0.
// KW12l/EcrhV.4dWKBFGw0.

QString NetServer::GenForumHash(QString uname, QString upass)
{

    /*
   QString str;

   QString ALPHABET="./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   QHash alphakeys;
   for (int i=0;i<ALPHABET.length();i++)
       alphakeys.insert(ALPHABET.at(i),i);
   }
    */

    QByteArray ba;
    QByteArray cst;
    cst.append(uname);
    // cst.append(upass);
    ba = QCryptographicHash::hash(cst, QCryptographicHash::Md5);
    return QString(ba.toHex());
}
