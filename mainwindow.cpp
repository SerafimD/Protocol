#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dlg = 0;
    currentRow = 0;
    contractNumber = QString::null;

    this->ui->centralWidget->setLayout(ui->verticalLayout);

    // ������� ����������� ���� ��� ������� �������
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

    // ���������� �������� � ������
    connect(this->ui->upperTable,SIGNAL(cellClicked(int,int)),this, SLOT(RowSelected(int, int)));

    updateTables();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addSlot()
{
    qDebug() << "add slot activated" << ui->upperTable->currentRow();

    addDialog *dlg = new addDialog(this,0,currentRow,ui->upperTable);
    //connect(dlg,SIGNAL(accepted()),this,SLOT(updateTables()));
    this->dlg = dlg;
    dlg->exec();
    updateTables();
}

void MainWindow::changeSlot()
{
    qDebug() << "change slot activated" << ui->upperTable->currentRow();

    addDialog *dlg = new addDialog(this,1,currentRow,ui->upperTable);
    dlg->exec();
    updateTables();
}

void MainWindow::deleteSlot()
{
    qDebug() << "delete slot activated" << ui->upperTable->currentRow();

    addDialog *dlg = new addDialog(this,2,currentRow,ui->upperTable);
    dlg->exec();
    updateTables();
}

void MainWindow::copySlot()
{
    qDebug() << "copy slot activated" << ui->upperTable->currentRow();

    addDialog *dlg = new addDialog(this,3,currentRow,ui->upperTable);
    dlg->exec();
    updateTables();
}

void MainWindow::updateTables()
{
     qDebug() << "Tables will be updated";
     // ������ ���������� ����� � ������ � ������
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

void MainWindow::RowSelected(int row, int col)
{
    this->currentRow = row;
    contractNumber = this->ui->upperTable->item(row,0)->text();
    qDebug() << "row = " << currentRow;
}
