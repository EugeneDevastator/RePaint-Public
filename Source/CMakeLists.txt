cmake_minimum_required(VERSION 3.27)
project(Source VERSION 1.0.0 LANGUAGES CXX)
set(Qt5_DIR "d:\\QT5\\5.12.12\\mingw73_64\\lib\\cmake\\Qt5\\Qt5Config.cmake")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
set(CMAKE_PREFIX_PATH "d:\\QT5\\5.12.12\\mingw73_64\\lib\\cmake\\")
find_package(Qt5 COMPONENTS Core Gui Network Widgets REQUIRED)

include_directories(.)

add_executable(Source
        SharedNetCode/NetStructures.h
        DBrushes/dBlank.png
        DBrushes/Disp_TrueBlank.bmp
        DBrushes/Waves.png
        Noise/clouds.png
        Noise/clouds.png_
        Noise/dots.png
        Noise/noise.png
        Noise/noise4k.png
        res/br512.png
        res/br512e.png
        res/Btn_Connect.png
        res/BtnAbout.png
        res/BtnDisc.png
        res/BtnlDel.png
        res/btnldrop.png
        res/BtnLDup.png
        res/btnlnew.png
        res/btnlock.png
        res/btnnew.png
        res/btnopen.png
        res/btnpaste.png
        res/BtnPick.PNG
        res/btnpin.png
        res/btnreframe.png
        res/btnreload.png
        res/btnrescale.png
        res/BtnRot.png
        res/btnsave.png
        res/btnsaveas.png
        res/BtnServ.png
        res/btnsnap.png
        res/ctlang.png
        res/ctlbm.png
        res/ctlcop.png
        res/ctlcrv.png
        res/ctlhue.png
        res/ctllen.png
        res/ctllit.png
        res/ctlnz.png
        res/ctlop.png
        res/ctlpwr.png
        res/ctlrad.png
        res/ctlrrel.png
        res/ctlsat.png
        res/ctlscale.png
        res/ctlscalerel.png
        res/ctlsol.png
        res/ctlsol2op.png
        res/ctlspc.png
        res/ctlspcjit.png
        res/curdrad_2.cur
        res/curDrag.cur
        res/curZoom.cur
        res/curZoom.png
        res/fCreate.png
        res/fDel.png
        res/fFind.png
        res/fJoin.png
        res/fPart.png
        res/ICON16.png
        res/nHeight.png
        res/nWidth.png
        res/repaint.jpg
        res/splash.png
        res/tbrush.png
        res/tcont.png
        res/tct_acc.png
        res/tct_angle.png
        res/tct_dir.png
        res/tct_htilt.png
        res/tct_len.png
        res/tct_lenpx.png
        res/tct_none.png
        res/tct_off.png
        res/Tct_pressure.png
        res/tct_relang.png
        res/tct_rot.png
        res/tct_tilt.png
        res/tct_vel.png
        res/tct_vel_i.png
        res/tct_vtilt.png
        res/tct_xtilt.png
        res/tct_ytilt.png
        res/tctstartdir.png
        res/tdisp.png
        res/tFbrush.png
        res/tline.png
        res/Tool_Presets.png
        res/tsmudge.png
        res/uBusy.png
        res/uIris.png
        res/uMClosed.png
        res/uMfree.png
        res/uOffline.png
        res/uOnline.png
        res/uRclosed.png
        res/uRfree.png
        res/urwatch.png
        sets/Presets.dat
        sets/Presets.dat_
        sets/Presets.dat_old
        sets/sizes.dat
        ReMath/ReMath.hpp
        NetworkDTO/NetworkDTO.hpp
        BrushEngine/NetBrushStamp.hpp
        # reverse it:
        BrushEngine/ClientBrushStamp.hpp
        BrushEngine/LegacySharedBrush.hpp
        abrush.bmp
        ActionMaster.cpp
        ActionMaster.h
        ArtMaster.cpp
        ArtMaster.h
        ArtThread.cpp
        ArtThread.h
        b_smartcolor.cpp
        b_smartcolor.h
        bctl_dblslider.cpp
        bctl_dblslider.h
        bctl_hoverbrush.cpp
        bctl_hoverbrush.h
        bctl_imagewidget.cpp
        bctl_imagewidget.h
        bctl_widgetholder.cpp
        bctl_widgetholder.h
        Brushes.cpp
        Brushes.h
        brushparam.cpp
        brushparam.h
        c_keylink.cpp
        c_keylink.h
        ctl_assetscene.cpp
        ctl_assetscene.h
        ctl_assetview.cpp
        ctl_assetview.h
        ctl_BMselector.cpp
        ctl_BMselector.h
        ctl_bparam.cpp
        ctl_bparam.h
        ctl_brushsizer.cpp
        ctl_brushsizer.h
        ctl_colorpicker.cpp
        ctl_colorpicker.h
        #ctl_colorwheel.cpp #legacy unused
        #ctl_colorwheel.h
        ctl_fastbrush.cpp
        ctl_fastbrush.h
        ctl_friendlist.cpp
        ctl_friendlist.h
        ctl_maskselect.cpp
        ctl_maskselect.h
        ctl_toolselector.cpp
        ctl_toolselector.h
        ctl_trueop.cpp
        ctl_trueop.h
        ctl_usertools.cpp
        ctl_usertools.h
        disp.bmp
        dlg_login.cpp
        dlg_login.h
        dlg_newcanvas.cpp
        dlg_newcanvas.h
        dlg_usersearch.cpp
        dlg_usersearch.h
        Enums.h
        GEOmaster.cpp
        geomaster.h
        icon16.png
        imagearray.cpp
        imagearray.h
        #imagebox.cpp unused
        #imagebox.h
        LayerActionQueue.cpp
        LayerActionQueue.h
        main.cpp
        MHPwindow.cpp
        MHPwindow.h
        NetClient.cpp
        NetClient.h
        NetServer.cpp
        NetServer.h
        NetSocket.cpp
        NetSocket.h
        BrushEditorPresenter.cpp
        BrushEditorPresenter.h
        pnl_chat.cpp
        pnl_chat.h
        pnl_colorpicker.cpp
        pnl_colorpicker.h
        pnl_filemenu.cpp
        pnl_filemenu.h
        LayersPanelPresenter.cpp
        LayersPanelPresenter.h
        pnl_netcontrols.cpp
        pnl_netcontrols.h
        pnl_quickcontrol.cpp
        pnl_quickcontrol.h
        reicon.ico
        smudge.bmp
        smudge.jpg
        strokemaster.cpp
        strokemaster.h
        version.h
        BrushModel.h
        BrushModel.cpp
        BrushEngine/DialModel.hpp
)

qt5_add_resources(SOURCE_FILES qresource.qrc)

target_link_libraries(Source Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network)