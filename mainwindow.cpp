#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dlg = 0;

    this->ui->centralWidget->setLayout(ui->verticalLayout);

    // Создаём контекстное меню для верхней таблицы
    QAction *addAction = new QAction(tr("&Add..."), this);
    //addAction->setShortcuts(QKeySequence::Open);
    addAction->setStatusTip(tr("New contract"));
    connect(addAction, SIGNAL(triggered()), this, SLOT(addSlot()));

    QAction *changeAction = new QAction(tr("&Change..."), this);
    //changeAction->setShortcuts(QKeySequence::Open);
    changeAction->setStatusTip(tr("Change current contract"));
    connect(changeAction, SIGNAL(triggered()), this, SLOT(changeSlot()));

    QAction *deleteAction = new QAction(tr("&Delete..."), this);
    //deleteAction->setShortcuts(QKeySequence::Open);
    deleteAction->setStatusTip(tr("delete contract"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteSlot()));

    QAction *copyAction = new QAction(tr("&Copy..."), this);
    //copyAction->setShortcuts(QKeySequence::Open);
    copyAction->setStatusTip(tr("copy contract"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copySlot()));

    QAction *updateAction = new QAction(tr("&Update..."), this);
    //copyAction->setShortcuts(QKeySequence::Open);
    copyAction->setStatusTip(tr("update tables"));
    connect(updateAction, SIGNAL(triggered()), this, SLOT(updateTables()));

    ui->upperTable->addAction(addAction);
    ui->upperTable->addAction(changeAction);
    ui->upperTable->addAction(deleteAction);
    ui->upperTable->addAction(copyAction);
    ui->upperTable->addAction(updateAction);
    ui->upperTable->setContextMenuPolicy(Qt::ActionsContextMenu);

    updateTables();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addSlot()
{
    qDebug() << "add slot activated" << ui->upperTable->currentRow();

    addDialog *dlg = new addDialog(this,0);
    connect(dlg,SIGNAL(accepted()),this,SLOT(updateTables()));
    this->dlg = dlg;
    dlg->exec();
}

void MainWindow::changeSlot()
{
    qDebug() << "change slot activated" << ui->upperTable->currentRow();

    addDialog *dlg = new addDialog(this,1);
    dlg->exec();
}

void MainWindow::deleteSlot()
{
    qDebug() << "delete slot activated" << ui->upperTable->currentRow();

    addDialog *dlg = new addDialog(this,2);
    dlg->exec();
}

void MainWindow::copySlot()
{
    qDebug() << "copy slot activated" << ui->upperTable->currentRow();

    addDialog *dlg = new addDialog(this,3);
    dlg->exec();
}

void MainWindow::updateTables()
{
     qDebug() << "Tables will be updated";
     // Читаем содержимое файла и грузим в дерево
     QDomDocument doc("contracts");
     QFile file("contracts.xml");
     if (!file.open(QIODevice::ReadOnly))
         return;
     if (!doc.setContent(&file)) {
         file.close();
         return;
     }
     file.close();

     QDomElement root = doc.documentElement();
     QDomNodeList childs = root.childNodes();
     ui->upperTable->setRowCount(childs.length());

     for (int i = 0; i < childs.length(); i ++) {
         QDomElement child = childs.at(i).toElement();
         QDomAttr a = child.attributeNode("number");
         QString NumberContract = a.value();
         ui->upperTable->setItem(i,0,new QTableWidgetItem(NumberContract));
         QDomNodeList hosts = child.childNodes();
         for(int j = 0 ; j < hosts.length(); j++)
         {
             QDomElement host = hosts.at(j).toElement();
             ui->upperTable->setItem(i,j+1,new QTableWidgetItem(host.text()));
         }
    }
    this->ui->upperTable->resizeColumnsToContents();
     this->ui->upperTable->resizeRowsToContents();
    this->ui->downTable->resizeColumnsToContents();
     this->ui->downTable->resizeRowsToContents();
}
