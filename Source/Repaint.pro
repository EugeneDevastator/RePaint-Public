QT       += core gui network widgets
TARGET = Repaint
TEMPLATE = app

win32 {
    RC_ICONS += reicon.ico
    ICON = reicon.ico
} else {
    ICON = reicon.ico
}
# Define the base copy command depending on the platform
win32 {
    COPY_CMD = xcopy /E /I /Y
} else {
    COPY_CMD = cp -r
}

# List of directories to copy
# repaint ini also has to be copied but idk
DIRS_TO_COPY = DBrushes Noise res sets

# Loop to construct and execute copy commands for each directory
for(DIR, DIRS_TO_COPY) {
    SOURCE_DIR = $$shell_path($$PWD/$$DIR)
    RELEASE_DIR = $$shell_path($$OUT_PWD/release/$$DIR)
    DEBUG_DIR = $$shell_path($$OUT_PWD/debug/$$DIR)

    # Copy commands for release and debug
    release: system($$COPY_CMD $$SOURCE_DIR $$RELEASE_DIR)
    debug: system($$COPY_CMD $$SOURCE_DIR $$DEBUG_DIR)
}

QMAKE_CXXFLAGS += -m32
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O3
QMAKE_CXXFLAGS_RELEASE += -O0

QMAKE_CFLAGS_RELEASE -= -O1
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE -= -O3
QMAKE_CFLAGS_RELEASE += -O0

HEADERS += \
    MHPwindow.h \
    imagearray.h \
    Brushes.h \
    ctl_bparam.h \
    bctl_imagewidget.h \
    ctl_toolselector.h \
    ctl_brushsizer.h \
    bctl_dblslider.h \
    b_smartcolor.h \
    ArtMaster.h \
    dlg_login.h \
    BrushEditorPresenter.h \
    ctl_friendlist.h \
    pnl_netcontrols.h \
    Enums.h \
    ctl_usertools.h \
    c_keylink.h \
    ctl_maskselect.h \
    dlg_newcanvas.h \
    ctl_fastbrush.h \
    ctl_trueop.h \
    ctl_BMselector.h \
    pnl_layers.h \
    dlg_usersearch.h \
    pnl_quickcontrol.h \
    geomaster.h \
    strokemaster.h \
    pnl_filemenu.h \
    pnl_chat.h \
    bctl_widgetholder.h \
    ctl_colorpicker.h \
    pnl_colorpicker.h \
    ctl_assetscene.h \
    ctl_assetview.h \
    NetSocket.h \
    NetClient.h \
    NetServer.h \
    ArtThread.h \
    ActionMaster.h \
    version.h \
    layerstack.h

CONFIG +=qt thread

SOURCES += \
    MHPwindow.cpp \
    main.cpp \
    imagearray.cpp \
    Brushes.cpp \
    ctl_bparam.cpp \
    bctl_imagewidget.cpp \
    ctl_toolselector.cpp \
    ctl_brushsizer.cpp \
    bctl_dblslider.cpp \
    b_smartcolor.cpp \
    ArtMaster.cpp \
    dlg_login.cpp \
    BrushEditorPresenter.cpp \
    ctl_friendlist.cpp \
    pnl_netcontrols.cpp \
    ctl_usertools.cpp \
    c_keylink.cpp \
    ctl_maskselect.cpp \
    dlg_newcanvas.cpp \
    ctl_fastbrush.cpp \
    ctl_trueop.cpp \
    ctl_BMselector.cpp \
    pnl_layers.cpp \
    dlg_usersearch.cpp \
    pnl_quickcontrol.cpp \
    GEOmaster.cpp \
    strokemaster.cpp \
    pnl_filemenu.cpp \
    pnl_chat.cpp \
    bctl_widgetholder.cpp \
    ctl_colorpicker.cpp \
    pnl_colorpicker.cpp \
    ctl_assetscene.cpp \
    ctl_assetview.cpp \
    NetSocket.cpp \
    NetClient.cpp \
    NetServer.cpp \
    ArtThread.cpp \
    ActionMaster.cpp \
    layerstack.cpp

OTHER_FILES +=

DISTFILES += \
    Resources/BtnAbout.png \
    Resources/BtnDisc.png \
    Resources/BtnLDup.png \
    Resources/BtnPick.PNG \
    Resources/BtnRot.png \
    Resources/BtnServ.png \
    Resources/Btn_Connect.png \
    Resources/BtnlDel.png \
    Resources/ICON16.png \
    Resources/Tct_pressure.png \
    Resources/Tool_Presets.png \
    Resources/br512.png \
    Resources/br512e.png \
    Resources/btnldrop.png \
    Resources/btnlnew.png \
    Resources/btnlock.png \
    Resources/btnnew.png \
    Resources/btnopen.png \
    Resources/btnpaste.png \
    Resources/btnreframe.png \
    Resources/btnreload.png \
    Resources/btnrescale.png \
    Resources/btnsave.png \
    Resources/btnsaveas.png \
    Resources/btnsnap.png \
    Resources/ctlang.png \
    Resources/ctlbm.png \
    Resources/ctlcop.png \
    Resources/ctlcrv.png \
    Resources/ctlhue.png \
    Resources/ctllen.png \
    Resources/ctllit.png \
    Resources/ctlnz.png \
    Resources/ctlop.png \
    Resources/ctlpwr.png \
    Resources/ctlrad.png \
    Resources/ctlrrel.png \
    Resources/ctlsat.png \
    Resources/ctlscale.png \
    Resources/ctlscalerel.png \
    Resources/ctlsol.png \
    Resources/ctlsol2op.png \
    Resources/ctlspc.png \
    Resources/ctlspcjit.png \
    Resources/curDrag.cur \
    Resources/curZoom.cur \
    Resources/curZoom.png \
    Resources/curdrad_2.cur \
    Resources/fCreate.png \
    Resources/fDel.png \
    Resources/fFind.png \
    Resources/fJoin.png \
    Resources/fPart.png \
    Resources/nHeight.png \
    Resources/nWidth.png \
    Resources/repaint.jpg \
    Resources/tFbrush.png \
    Resources/tbrush.png \
    Resources/tcont.png \
    Resources/tct_acc.png \
    Resources/tct_angle.png \
    Resources/tct_dir.png \
    Resources/tct_htilt.png \
    Resources/tct_len.png \
    Resources/tct_lenpx.png \
    Resources/tct_none.png \
    Resources/tct_off.png \
    Resources/tct_relang.png \
    Resources/tct_rot.png \
    Resources/tct_tilt.png \
    Resources/tct_vel.png \
    Resources/tct_vel_i.png \
    Resources/tct_vtilt.png \
    Resources/tct_xtilt.png \
    Resources/tct_ytilt.png \
    Resources/tctstartdir.png \
    Resources/tdisp.png \
    Resources/tline.png \
    Resources/tsmudge.png \
    Resources/uBusy.png \
    Resources/uIris.png \
    Resources/uMClosed.png \
    Resources/uMfree.png \
    Resources/uOffline.png \
    Resources/uOnline.png \
    Resources/uRclosed.png \
    Resources/uRfree.png \
    Resources/urwatch.png

RESOURCES += \
    qresource.qrc
