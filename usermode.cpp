#include "usermode.h"
#include "ui_usermode.h"
#include"mainwindow.h"
#include"borrowbook.h"
#include"returnbook.h"
#include"reservation.h"
#include"managebooks.h"
UserMode::UserMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserMode),
    borrowbookWindow(new borrowbook),
    returnbookWindow(new returnbook),
    reservationWindow(new reservation),
    managebooksWindow(new managebooks)
{
    ui->setupUi(this);
    // Connect the return signal from ManageReaders to the slot in Mode
        connect(borrowbookWindow, &borrowbook::returntomode, this, &UserMode::on_returnFromborrowbook);
        connect(returnbookWindow,&returnbook::returntomode,this,&UserMode::on_returnFromreturnbook);
        connect(reservationWindow,&reservation::returntomode,this,&UserMode::on_returnFromreservation);
        connect(borrowbookWindow,&borrowbook::bookBorrowed,managebooksWindow,&managebooks::on_refreshButton_clicked);
}

UserMode::~UserMode()
{
    delete ui;
    delete borrowbookWindow;
    delete returnbookWindow;
    delete reservationWindow;
    delete managebooksWindow;
}

//退出
void UserMode::on_backButton_clicked()
{
    MainWindow*m=new MainWindow();
    this->close();
    m->show();
}

//进入“借书”页面
void UserMode::on_borrowButton_clicked()
{
    this->hide();
    borrowbookWindow->show();
}
//从“借书”返回
void UserMode::on_returnFromborrowbook(){
    borrowbookWindow->hide();
    this->show();
}

//进入“预约”页面
void UserMode::on_reservationButton_clicked()
{
    this->hide();
    reservationWindow->show();
}
//从“预约”返回
void UserMode::on_returnFromreservation(){
    reservationWindow->hide();
    this->show();
}

//进入“还书”
void UserMode::on_returnBookButton_clicked()
{
    this->hide();
    returnbookWindow->show();
}
//从“还书”返回
void UserMode::on_returnFromreturnbook(){
    returnbookWindow->hide();
    this->show();
}
