#include "manageusers.h"
#include "ui_manageusers.h"
#include"mainwindow.h"
#include"DatabaseManager.h"
#include<QSqlQuery>
#include<QSqlError>
#include<QDebug>
ManageUsers::ManageUsers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageUsers)
{
    ui->setupUi(this);
    initDatabase();// Initialize database connection
        loadUsers();// Load users data into the UI
}

ManageUsers::~ManageUsers()
{
    delete ui;
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

//刷新
void ManageUsers::on_refreshButton_clicked()
{
    loadUsers(); // Reload users data when refresh button is clicked
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
