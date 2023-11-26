#ifndef DLG_USERSEARCH_H
#define DLG_USERSEARCH_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QStandardItemModel>
#include <QItemSelectionModel>

class dlg_UserSearch : public QDialog
{
    Q_OBJECT
public:
    explicit dlg_UserSearch(QWidget *parent = 0);
    QListView *EdResults;
    QStandardItemModel *ResModel;
    QItemSelectionModel *SelModel;

    QLineEdit *EdSearch;
    QPushButton *BtnSearch;
    QPushButton *BtnMore;
    QPushButton *BtnAdd;


    QGridLayout *LT;

    QString StStar;
    QString StChar;


signals:
    void AskName(QString uname);
    void AskAdd(QString uname);

public slots:
    void GetPeopleList(QStringList stl);
    void SearchClick();
    void AddClick();


};

#endif // DLG_USERSEARCH_H
