#ifndef ENUMS_H
#define ENUMS_H

enum rStates{
    srNONE, srServ, srClient, srGlobal, srSTOP
};
enum uStates{
    suZERO,suOff,suOn,suRoomFree,suRoomClosed,suMemberFree,suMemberClosed,suMemberSame,suSTOP
};
enum aTypes{
    atLogin, atRegister, atRename, atSTOP
};

#endif // ENUMS_H
