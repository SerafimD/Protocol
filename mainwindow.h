#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
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
    void addSlot();
    void changeSlot();
    void deleteSlot();
    void copySlot();
    void updateTables();
};

#endif // MAINWINDOW_H
