#include "manageusers.h"
#include "ui_manageusers.h"
#include"mainwindow.h"
#include"DatabaseManager.h"
#include"emailyanshi.h"
#include "ui_emailyanshi.h"//没有这个，getUI()->sendButton会出错
#include<QSqlQuery>
#include<QSqlError>
#include<QDebug>
#include<QDateTime>
#include <ctime>    // For time()
#include<QTimer>
ManageUsers::ManageUsers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageUsers),
    emailWidget(new emailyanshi(this)),
    m_pTimer(new QTimer(this)),
        m_nCountdown(60)// 初始化倒计时为60秒
{
    ui->setupUi(this);
    initDatabase();// Initialize database connection
        loadUsers();// Load users data into the UI
       //connect(发送对象，信号，接收对象，槽函数)，其中发送信号和槽函数需要用 SIGNAL() 和 SLOT() 来进行声明。
        connect(ui->usersTable, &QTableWidget::cellClicked, this, &ManageUsers::onUserSelectionChanged);
            connect(ui->refreshButton, &QPushButton::clicked, this, &ManageUsers::on_refreshButton_clicked);
            connect(ui->remindreturnButton, &QPushButton::clicked, emailWidget->getUI()->sendButton,&QPushButton::click);
            // 设置计时器槽函数
                connect(m_pTimer, &QTimer::timeout, this, &ManageUsers::updateButtonText);
}

ManageUsers::~ManageUsers()
{
    delete ui;
    delete emailWidget;
    delete m_pTimer;
}

void ManageUsers::initDatabase(){
//    db = QSqlDatabase::addDatabase("QSQLITE", "manage_connection");
//        db.setDatabaseName("library.db");//多次创建会导致重复出错
    db = DatabaseManager::getDatabase();
        if (!db.open()) {
            qDebug() << "Error: Failed to connect database." << db.lastError();
        } else {
            qDebug() << "Database connected.";
        }
}

void ManageUsers::loadUsers()
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users");
    if (!query.exec()) {
        qDebug() << "Error loading users:" << query.lastError();
        return;
    }

    ui->usersTable->setRowCount(0); // Clear existing rows
    int row = 0;
    while (query.next()) {
        QString userid = query.value("xuehao").toString();
        QString pwd=query.value("password").toString();
        QString email = query.value("email").toString();
        QString createTime = query.value("create_time").toString();
        bool isOnline = query.value("is_online").toBool();

        // Insert row into the table
        ui->usersTable->insertRow(row);
        ui->usersTable->setItem(row, 0, new QTableWidgetItem(userid));
        ui->usersTable->setItem(row, 1, new QTableWidgetItem(pwd));
        ui->usersTable->setItem(row, 2, new QTableWidgetItem(email));
        ui->usersTable->setItem(row, 3, new QTableWidgetItem(createTime));
        ui->usersTable->setItem(row, 4, new QTableWidgetItem(isOnline ? "Online" : "Offline"));

        ++row;
    }
}

void ManageUsers::onUserSelectionChanged()
{
    int row = ui->usersTable->currentRow();
    QString xuehao = ui->usersTable->item(row, 0)->text();
    loadborrow(xuehao);
}

void ManageUsers::loadborrow(const QString&xuehao){
    QSqlQuery query(db);
    query.prepare("SELECT book_id,title,borrow_time, created_at FROM borrow WHERE xuehao = :xuehao");
        query.bindValue(":xuehao", xuehao);
        if (!query.exec()) {
            qDebug() << "Error loading borrowing data:" << query.lastError();
            return;
        }
        ui->borrowTableWidget->setRowCount(0); // Clear existing rows
            int row = 0;
            QString borrowdate = query.value("borrow_time").toString();
            QDateTime borrow_date=QDateTime::currentDateTime();
            QDateTime returndate=borrow_date.addMonths(1);
            QString return_date=returndate.toString("yyyy-MM-dd");
            while (query.next()) {
                ui->borrowTableWidget->insertRow(row);
                ui->borrowTableWidget->setItem(row, 0, new QTableWidgetItem(query.value("book_id").toString()));
                ui->borrowTableWidget->setItem(row, 1, new QTableWidgetItem(query.value("title").toString()));
                ui->borrowTableWidget->setItem(row, 2, new QTableWidgetItem(query.value("borrow_time").toString()));
                ui->borrowTableWidget->setItem(row, 3, new QTableWidgetItem(return_date));
                row++;
            }
}

//刷新
void ManageUsers::on_refreshButton_clicked()
{
    loadUsers(); // Reload users data when refresh button is clicked
    int row = ui->usersTable->currentRow();
        if (row >= 0) {
            QString xuehao = ui->usersTable->item(row, 0)->text();
            loadborrow(xuehao);
        }
}

//返回
void ManageUsers::on_closeButton_clicked()
{
//    this->close(); // Close the manage users dialog
//    MainWindow*m=new MainWindow();
//    m->show();
    emit returntomode();  // Emit the signal to return to the Mode window
    this->hide();  // Hide the ManageUsers window
}

//提醒归还
void ManageUsers::on_remindreturnButton_clicked()
{

}

// 更新按钮文本
void ManageUsers::updateButtonText()
{
    if (m_nCountdown > 0) {
        ui->remindreturnButton->setText(QString::number(m_nCountdown) + "秒后重试");
        m_nCountdown--;
    } else {
        m_pTimer->stop();
        ui->remindreturnButton->setText("提醒归还");
        ui->remindreturnButton->setEnabled(true);
    }
}
