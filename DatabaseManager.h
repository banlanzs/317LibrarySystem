#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include<QPluginLoader>
class DatabaseManager {
public:
//    static QSqlDatabase& getDatabase() {
//        static QSqlDatabase db;
//        if (!db.isValid()) {
//            db = QSqlDatabase::addDatabase("QSQLITE", "library_connection");
//            db.setDatabaseName("library.db");
//            if (!db.open()) {
//                qDebug() << "Database connection error:" << db.lastError();
//            } else {
//                qDebug() << "Database connected.";
//            }
//        }
//        return db;
//    }

    static QSqlDatabase& getDatabase() {
        static QSqlDatabase db;
        if (!db.isValid()) {
            db = QSqlDatabase::addDatabase("QMYSQL");
            db.setHostName("127.0.0.1");
            db.setUserName("root");
            db.setPassword("200504024456zsh@");
            db.setDatabaseName("library_db");

//            QPluginLoader loader;
//            loader.setFileName("D:\\ChromeDownload\\Qt\\QT\5.14.2\\mingw73_64\\plugins\\sqldrivers\\qsqlmysql.dll");// 报“找不到指定模块”
//            qDebug()<<loader.load();
//            qDebug()<<loader.errorString();
            qDebug()<<QSqlDatabase::drivers()<<endl;

            if (!db.open()) {
                qDebug() << "Database connection error:" << db.lastError()<<endl;
            } else {
                qDebug() << "Database connected."<<endl;
            }
        }
        return db;
    }
    static void closeDatabase() {
        QSqlDatabase::database("library_connection").close();
        QSqlDatabase::removeDatabase("library_connection");
    }
};
#endif // DATABASEMANAGER_H