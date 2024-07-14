#ifndef MANAGEUSERS_H
#define MANAGEUSERS_H

#include <QDialog>
#include<QSqlDatabase>
namespace Ui {
class ManageUsers;
}

class ManageUsers : public QDialog
{
    Q_OBJECT

public:
    explicit ManageUsers(QWidget *parent = nullptr);
    ~ManageUsers();
signals:
    void returntomode();//->closeButton

private slots:
    void on_refreshButton_clicked();
    void on_closeButton_clicked();

private:
    Ui::ManageUsers *ui;
    QSqlDatabase db;
        void initDatabase();
        void loadUsers();
};

#endif // MANAGEUSERS_H
