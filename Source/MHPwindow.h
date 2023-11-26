#ifndef MHPWINDOW_H
#define MHPWINDOW_H


#include <QtWidgets/QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QTabletEvent>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QSettings>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QMainWindow>
#include "Brushes.h"
#include "layerstack.h"
#include "ArtMaster.h"
#include "ArtThread.h"
#include "ActionMaster.h"
#include "b_smartcolor.h"
#include "bctl_imagewidget.h"
#include "bctl_dblslider.h"

#include "c_keylink.h"
#include "ctl_assetview.h"
#include "ctl_assetscene.h"
#include "ctl_BMselector.h"
#include "ctl_bparam.h"
#include "ctl_brushsizer.h"
#include "ctl_colorwheel.h"
#include "ctl_fastbrush.h"
#include "ctl_friendlist.h"
#include "ctl_maskselect.h"
#include "ctl_toolselector.h"
#include "ctl_trueop.h"
#include "ctl_colorpicker.h"
#include "ctl_usertools.h"

#include "dlg_login.h"
#include "dlg_newcanvas.h"
#include "dlg_usersearch.h"
#include "strokemaster.h"

#include "Enums.h"
#include "geomaster.h"
#include "imagearray.h"
#include "NetClient.h"
#include "NetServer.h"

#include "pnl_bcontorls.h"
#include "pnl_netcontrols.h"
#include "pnl_layers.h"
#include "pnl_quickcontrol.h"
#include "pnl_filemenu.h"
#include "pnl_chat.h"



QT_BEGIN_NAMESPACE
class QLabel;
class QPixmap;
class QPushButton;
class QTcpSocket;
class QNetworkSession;
class QHBoxLayout;
class QVBoxLayout;
class QLineEdit;


class ArtMaster;
class b_smartcolor;
class bctl_imagewidget;
class bctl_dblslider;
class c_keylink;
class ctl_BMselector;
class ctl_bparam;
class ctl_brushsizer;
class ctl_colorwheel;
class ctl_fastbrush;
class ctl_friendlist;
class ctl_maskselect;
class ctl_toolselector;
class ctl_trueop;
class ctl_usertools;
class dlg_login;
class dlg_newcanvas;
class dlg_usersearch;

class geomaster;
class imagearray;
class NetClient;

class pnl_bcontorls;
class pnl_netcontrols;
class pnl_layers;
class pnl_quickcontrol;
QT_END_NAMESPACE


class MHPwindow : public QWidget
{
    Q_OBJECT

public slots:



    void SendMsg();

    void GetPoly(QPolygonF Poly);
    void GetTStroke(d_Stroke Strk,d_StrokePars stpars);
    void GetAction(d_Action act);
    void GetClient();
    void GetMsg(QString msg);
    void GetServIp(QString msg);
    void ConnectAddr();
    void ImgOpen();
    void RedrawCol();
    void GetColor(QColor col);
    void SendChatMsg();
   // void GetChatMsg(QString Msg);
    void GetLogin(QString user,QString pass);
    void GrabKB();
    void RelKB();
    void SaveLogin();
    void LoadLogin();
    void OpenImage();
    void ReOpenImage();
    void SnapImage();
    void SaveImg();
    void SaveImgAs();
    void NewImage(QSize sz);
    void PasteImage();
    void RealignQP();
    void ShowQP();
    void HideQP();

    void SwitchPinPanels();

    void LockCanvas(qint8 lk);
    void GrabImg(QString asker);



    void ExecLayerAction(d_LAction lact);
    void ConfirmImage(QByteArray ba);
    void ConfirmAct(d_Action act);


public:
    MHPwindow();
    qint8 lock;
    bool StrokingEN;
    int ActiveLayer;

   ActionMaster *ACM;
    pnl_Layers *LayersPanel;
    QPointF lastpos;

 //   QMdiArea *MdiArea;
    bool Dedicated;

    QString WorkFilePath;
    int SnapIdx;

    QString *LocalName;
    NetClient *NET;
    NetServer *sNET;
    c_KeyLink *KBLINK;

    ArtThread *ARTM;
    QSettings *AppSettings;

    //bctl_HoverBrush *HoverBrush;
    d_RealBrush *g_Brush;
    b_SmartColor *g_PaintColor;
    b_SmartColor *g_EraseColor;
    pnl_QuickControl *QuickPanel;
    pnl_FileMenu *FileMenu;
        ctl_UserTools *PRESETS;
    //QPicture *ViewCanvas;
    QLabel *Label1;
    QLabel *LblServIp;
    QLabel *Lb1;

    QSplitter *MCSplitter;
    QList <int> MCsizes;
    QList <int> TMsizes;
    QList <QWidget *> AllPanels;
    bool PansHidden;
QGridLayout *MainLayout;
    QRect MainGeometry;

   // QPushButton *BtnAbout;
    //    QPushButton *BtnLogin;

    pnl_NetControls *NetControls;
pnl_Chat *CHAT;
    ctl_friendlist *FriendList;


    //QDialog *DlgCol;
    QDialog *DlgAbout;

    dlg_login *DlgLogin;
    dlg_NewCanvas *DlgNew;
    ctl_ColorWheel *CtlCol;
    float SaveDist;

    d_Brush LocalBrush;
    d_Action LocalAction;

    //Layout defs
    QHBoxLayout *LtUpperToolbar;
    QHBoxLayout *LtUpperRightToolbar;
    QVBoxLayout *LtLeftToolbar;
    QWidget *Leftpanel;
//    QSplitter *TMSplitter;


    // network defs

    ImageArray *MainImage;

    QToolButton *btnTool;
    QToolBar *TB1;


    pnl_bcontorls *BControls;
    QVBoxLayout *LtChat;
QWidget *wChat;

StrokeMaster *STM;

bool PanelsPinned;

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);

    void wheelEvent(QWheelEvent *event);
    //void MainImage::mouseMoveEvent(QMouseEvent *event);
};
#endif // MHPWINDOW_H


