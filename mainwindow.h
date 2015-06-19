#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QItemDelegate>
#include <QDate>
#include "addDialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    addDialog *dlg;

public slots:
    void addSlot();                 // Добавить контракт
    void changeSlot();              // Изменить контракт
    void deleteSlot();              // Удалить контракт
    void copySlot();                // Скопировать контракт
    void updateTables();            // Обновить таблицы
    void updateDownTable();         // Обновление подчинённой таблицы
    void setDelayColor(int row);    // Покраска строки таблицы
    void setSendColor(int row);
    void setDelayColorTable();
    void menuControl();

signals:
    void delayContract(int);
    void sendContract(int);
};

class NonEditTableColumnDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    NonEditTableColumnDelegate(QObject * parent = 0) : QItemDelegate(parent) {}
    virtual QWidget * createEditor ( QWidget *, const QStyleOptionViewItem &,
                                     const QModelIndex &) const
    {
        return 0;
    }
};

#endif // MAINWINDOW_H
