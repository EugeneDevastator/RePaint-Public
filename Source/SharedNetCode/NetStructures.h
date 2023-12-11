//
// Created by Eugene on 12/10/2023.
//

#ifndef SOURCE_NETSTRUCTURES_H
#define SOURCE_NETSTRUCTURES_H

enum NetworkAppOperationMode {
    emServer,
    emGServer,
    emClient,
    emNone
    };

enum HeaderKind{
    sdNONE,         //0
    sdAction,       //AA 1
    sdSection,   //2
    sdLAction,      //LA
    sdGetName,      //GN
    sdGetPass,      //GP
    sdGetMsg,       //CM 5
    sdLogin,        //SL
    sdLoginS,       //LS 7
    sdUserAdded,    //UA    like saying hello to everyone
    sdUserDel,      //DU    Goodbye everyone;
    sdUserStat,     //US 10 status, "he everyone, i a killer"
    sdRoomJoin,     //RJ
    sdRoomPart,     //RP
    sdRoomCreate,   //RC
    sdAuth,         //    new one for combined auth;
    sdReg,          // RR
    sdLog,          // LL 15
    sdFriendSeek,    // FS
    sdFriendAdd,
    sdFriendDel,    //FD
    sdLock,
    sdUnlock,
    sdGetImg,
    sdReqImg,
    sdConfirmImg,
    sdPeopleResults,

    sdMisc,
    sdFAIL,
    sdSTOP
};
#endif //SOURCE_NETSTRUCTURES_H
