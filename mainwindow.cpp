#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"register.h"//跳转注册窗口头文件
#include"rewritepwd.h"//跳转重置密码窗口头文件
#include"usermode.h"//跳转用户窗口
#include"mode.h"//跳转管理员窗口
#include "DatabaseManager.h"
#include<QLineEdit>
#include<QSqlError>
#include <QSqlQuery>    //操作数据库
#include <QDebug>   //输出错误信息
#include <QMessageBox>//消息盒子
#include <QSettings>    //读写配置文件
#include"emailyanshi.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initDatabase(); // 初始化数据库连接
}

MainWindow::~MainWindow()
{
    delete ui;
    DatabaseManager::closeDatabase();
}

void MainWindow::initDatabase(){
//    db = QSqlDatabase::addDatabase("QSQLITE", "library_connection");
//    db.setDatabaseName("library.db");
    db = DatabaseManager::getDatabase();
    if(!db.open()){
        qDebug() << db.lastError();
        QMessageBox::warning(this, "error", "MainWindow未连接到数据库");
        return;
    } else {
        qDebug() << "MainWindow Database connected.";
    }
}

//登录
bool MainWindow::validateUser(const QString &userid, const QString &password)
{
        QSqlQuery query(db);
        if (userid == "admin") {
                // 管理员账户验证
                query.prepare("SELECT * FROM users WHERE xuehao = ? AND password = ?");
                query.addBindValue(userid);
                query.addBindValue(password);
            } else {
                // 普通用户验证
                query.prepare("SELECT * FROM users WHERE xuehao = ? AND password = ? AND email IS NOT NULL");
                query.addBindValue(userid);
                query.addBindValue(password);
            }

            if(query.exec()) {
                if(query.next()) {
                    // 执行登录成功后的操作，打开主窗口
                    if (userid == "admin") {
                                // 打开管理员界面
                                Mode *adminWindow = new Mode();
                                adminWindow->show();
                            } else {
                                // 打开普通用户界面
                                UserMode *userWindow = new UserMode();
                                userWindow->show();
                            }
                    QSqlQuery updateQuery(db);
                            updateQuery.prepare("UPDATE users SET is_online = 1 WHERE xuehao = ?");
                            updateQuery.addBindValue(userid);
                            updateQuery.exec();//更新在线状态
                    this->close();
                    return true; // 用户存在且密码正确
                }
            } else {
                qDebug() << "Login query error:" << query.lastError();
            }

            return false;
}

//注册（转到注册界面）
void MainWindow::on_RegisterButton_clicked()
{
    Register*r=new Register;
    this->close();
    r->show();//去注册
}

//忘记密码
void MainWindow::on_Forgetpwd_clicked()
{
    rewritepwd*r=new rewritepwd;
    this->close();
    r->show();
}

//登录
void MainWindow::on_LoginButton_clicked()
{
    qDebug()<<"LoginButton clicked.";
    QString userid=ui->userID->text();//获取学号
    QString pwd=ui->password->text();//获取输入密码
    //判断学号或密码是否为空，为空则提示
    if (userid.isEmpty() || pwd.isEmpty()) {
            QMessageBox::warning(this, "错误", "学号或密码不能为空");
            return;
        }
    //检查学号是否符合规范(9位数字长度)，10位会超大小(管理员除外)
    if(userid!="admin"&&(userid.length()!=9||!userid.toUInt())){
        QMessageBox::warning(this,"error","学号必须是9位数字。");
        return;
    }

    if(validateUser(userid, pwd)) {
            QMessageBox::information(this, "Success", "登录成功！");
        } else {
            QMessageBox::warning(this, "Error", "学号或密码错误！");
            ui->userID->clear();
            ui->userID->setFocus();
            ui->password->clear();
        }
}

////转到邮箱测试
//void MainWindow::on_pushButton_clicked()
//{
//    this->close();
//    emailyanshi*e=new emailyanshi();
//    e->show();
//}

//退出
void MainWindow::on_out_clicked()
{
    this->close();
    DatabaseManager::closeDatabase();
}

////填充账号密码
//void MainWindow::fillCredentials(const QString &userid, const QString &password) {
//    // 假设有QLineEdit对象用于输入账号和密码
//    ui->userID->setText(userid);
//    ui->password->setText(password);
//}