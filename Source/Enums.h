#ifndef ENUMS_H
#define ENUMS_H

enum NetworkRoleKind{
    srNONE, srServ, srClient, srGlobal, srSTOP
};
enum UserStateKind{
    suZERO,suOff,suOn,suRoomFree,suRoomClosed,suMemberFree,suMemberClosed,suMemberSame,suSTOP
};
enum AuthKind{
    atLogin, atRegister, atRename, atSTOP
};

#endif // ENUMS_H
