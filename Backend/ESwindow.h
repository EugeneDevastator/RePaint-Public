#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
#include <QtGui>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabletEvent>
#include <QToolBar>
#include <QComboBox>
#include <QDialog>
#include "dbkeeper.h"
#include <QTextEdit>

QT_BEGIN_NAMESPACE
class QLabel;
class QPixmap;
class QPushButton;
class QTcpSocket;
class QNetworkSession;
class QHBoxLayout;
class QVBoxLayout;
class QLineEdit;
class QTableView;
class QSqlDatabase;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
    
public:
    MainWindow();
    c_Netmaster *NET;

    ~MainWindow();
    QPushButton *Btn1;
    QHBoxLayout *LT;
    QTableView *UTable;
    QSqlDatabase *UBase;
    QLabel *TestLBL;
    DBKeeper *DBK;
    QTextEdit *TextView;

private:
public slots:
    void GetMsg(QString msg);


};

#endif // MAINWINDOW_H
