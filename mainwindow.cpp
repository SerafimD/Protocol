#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dlg = 0;

    this->ui->centralWidget->setLayout(ui->verticalLayout);


    QAction *addAction = new QAction(tr("&Добавить"), this);
    //addAction->setShortcuts(QKeySequence::Open);
    addAction->setStatusTip(tr("Новый договор"));
    connect(addAction, SIGNAL(triggered()), this, SLOT(addSlot()));

    QAction *changeAction = new QAction(tr("&Изменить"), this);
    //changeAction->setShortcuts(QKeySequence::Open);
    changeAction->setStatusTip(tr("Изменить текущий договор"));
    connect(changeAction, SIGNAL(triggered()), this, SLOT(changeSlot()));

    QAction *deleteAction = new QAction(tr("&Удалить"), this);
    //deleteAction->setShortcuts(QKeySequence::Open);
    deleteAction->setStatusTip(tr("Удалить договор"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteSlot()));

    QAction *copyAction = new QAction(tr("&Копировать"), this);
    //copyAction->setShortcuts(QKeySequence::Open);
    copyAction->setStatusTip(tr("Скопировать договор"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copySlot()));

    QAction *updateAction = new QAction(tr("&Обновить"), this);
    //copyAction->setShortcuts(QKeySequence::Open);
    updateAction->setStatusTip(tr("Обновить представление"));
    connect(updateAction, SIGNAL(triggered()), this, SLOT(updateTables()));

    ui->upperTable->addAction(addAction);
    ui->upperTable->addAction(changeAction);
    ui->upperTable->addAction(deleteAction);
    ui->upperTable->addAction(copyAction);
    ui->upperTable->addAction(updateAction);
    ui->upperTable->setContextMenuPolicy(Qt::ActionsContextMenu);

    ui->upperTable->setItemDelegateForColumn(0, new NonEditTableColumnDelegate());
    ui->upperTable->setItemDelegateForColumn(1, new NonEditTableColumnDelegate());
    ui->upperTable->setItemDelegateForColumn(2, new NonEditTableColumnDelegate());
    ui->upperTable->setItemDelegateForColumn(3, new NonEditTableColumnDelegate());
    ui->upperTable->setItemDelegateForColumn(4, new NonEditTableColumnDelegate());
    ui->upperTable->setItemDelegateForColumn(5, new NonEditTableColumnDelegate());
    ui->upperTable->setItemDelegateForColumn(6, new NonEditTableColumnDelegate());
    ui->upperTable->setItemDelegateForColumn(7, new NonEditTableColumnDelegate());
    ui->upperTable->setItemDelegateForColumn(8, new NonEditTableColumnDelegate());
    ui->upperTable->setItemDelegateForColumn(9, new NonEditTableColumnDelegate());

    ui->upperTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->upperTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->upperTable->setFocusPolicy(Qt::NoFocus);

    ui->downTable->setRowCount(1);
    ui->downTable->setItemDelegateForColumn(0, new NonEditTableColumnDelegate());
    ui->downTable->setItemDelegateForColumn(1, new NonEditTableColumnDelegate());
    ui->downTable->setItemDelegateForColumn(2, new NonEditTableColumnDelegate());

    ui->downTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->downTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->downTable->setFocusPolicy(Qt::NoFocus);

    connect(ui->upperTable,SIGNAL(cellClicked(int,int)),this,SLOT(updateDownTable()));

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

    this->dlg = dlg;
    dlg->exec();
    updateTables();
}

void MainWindow::changeSlot()
{
    qDebug() << "change slot activated" << ui->upperTable->currentRow();

    if(ui->upperTable->currentRow() != -1)
    {
        addDialog *dlg = new addDialog(this,1,ui->upperTable->currentRow(),ui->upperTable);
        dlg->exec();
        updateTables();
    }
}

void MainWindow::deleteSlot()
{
    qDebug() << "delete slot activated" << ui->upperTable->currentRow();

    if(ui->upperTable->currentRow() != -1)
    {
        addDialog *dlg = new addDialog(this,2,ui->upperTable->currentRow(),ui->upperTable);
        dlg->exec();
        updateTables();
    }
}

void MainWindow::copySlot()
{
    qDebug() << "copy slot activated" << ui->upperTable->currentRow();

    if(ui->upperTable->currentRow() != -1)
    {
        addDialog *dlg = new addDialog(this,3,ui->upperTable->currentRow(),ui->upperTable);
        dlg->exec();
        updateTables();
    }
}

void MainWindow::updateTables()
{
     qDebug() << "Tables will be updated";

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

void MainWindow::updateDownTable()
{
    int curentRow = ui->upperTable->currentRow();

    // если протокол не срочный
    if(ui->upperTable->item(curentRow,7)->text() == "НЕТ")
    {
        // получаем текущую дату оплаты
        QDate datePaid = QDate::fromString(ui->upperTable->item(curentRow,6)->text(),"dd.MM.yyyy");

        // получаем дату выдачи
        QDate dateOfIssue = datePaid.addDays(10);

        // Получаем разность между датой выдачи и текущей датой (системное время)
        QString today = QDate::currentDate().toString("dd.MM.yyyy");
        int daysToEnd = QDate::fromString(today,"dd.MM.yyyy").daysTo(dateOfIssue);

        ui->downTable->setItem(0,0,new QTableWidgetItem(dateOfIssue.toString("dd.MM.yyyy")));
        ui->downTable->setItem(0,1,new QTableWidgetItem(QString::number(daysToEnd)));

        if(daysToEnd >= 0)
        {
            ui->downTable->setItem(0,2,new QTableWidgetItem(QString("НЕТ")));
        }
        else
        {
            int daysDelay = QDate::fromString(today,"dd.MM.yyyy").daysTo(dateOfIssue) * -1;
            ui->downTable->setItem(0,2,new QTableWidgetItem(QString::number(daysDelay)));
            ui->downTable->setItem(0,1,new QTableWidgetItem(QString("0")));
        }

    }
    // если срочный
    else
    {
        // получаем текущую дату оплаты
        QDate datePaid = QDate::fromString(ui->upperTable->item(curentRow,6)->text(),"dd.MM.yyyy");

        // получаем дату выдачи
        QDate dateOfIssue = datePaid.addDays(1);

        // Получаем разность между датой выдачи и текущей датой (системное время)
        QString today = QDate::currentDate().toString("dd.MM.yyyy");
        int daysToEnd = QDate::fromString(today,"dd.MM.yyyy").daysTo(dateOfIssue);

        ui->downTable->setItem(0,0,new QTableWidgetItem(dateOfIssue.toString("dd.MM.yyyy")));
        ui->downTable->setItem(0,1,new QTableWidgetItem(QString::number(daysToEnd)));

        if(daysToEnd >= 0)
        {
            ui->downTable->setItem(0,2,new QTableWidgetItem(QString("НЕТ")));
        }
        else
        {
            int daysDelay = QDate::fromString(today,"dd.MM.yyyy").daysTo(dateOfIssue) * -1;
            ui->downTable->setItem(0,2,new QTableWidgetItem(QString::number(daysDelay)));
            ui->downTable->setItem(0,1,new QTableWidgetItem(QString("0")));
        }
    }
}
