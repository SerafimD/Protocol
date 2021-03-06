#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dlg = 0;

    this->ui->centralWidget->setLayout(ui->verticalLayout);

    this->ui->mainToolBar->addAction(style()->standardIcon(QStyle::SP_TrashIcon),"Удалить отправленные протоколы",this,SLOT(clearSendContracts()));
    this->ui->mainToolBar->addAction(style()->standardIcon(QStyle::SP_BrowserReload),"Обновить",this,SLOT(updateTables()));

    this->ui->actionQuit->setIcon(style()->standardIcon(QStyle::SP_MessageBoxCritical));

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
    ui->upperTable->setItemDelegateForColumn(10, new NonEditTableColumnDelegate());
    ui->upperTable->setItemDelegateForColumn(11, new NonEditTableColumnDelegate());
    ui->upperTable->setItemDelegateForColumn(12, new NonEditTableColumnDelegate());

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
    connect(this,SIGNAL(delayContract(int)),this,SLOT(setDelayColor(int)));
    connect(this,SIGNAL(sendContract(int)),this,SLOT(setSendColor(int)));
    connect(ui->actionQuit,SIGNAL(triggered()),this,SLOT(quitSlot()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(viewAboutDialog()));
    connect(ui->actionAllContracts,SIGNAL(changed()),this,SLOT(updateTables()));
    connect(ui->actionTodayContracts,SIGNAL(changed()),this,SLOT(updateTables()));
    connect(ui->actionPaidContracts,SIGNAL(changed()),this,SLOT(updateTables()));
    connect(ui->actionNotPaidContracts,SIGNAL(changed()),this,SLOT(updateTables()));


    connect(ui->actionAllContracts,SIGNAL(triggered()),this,SLOT(menuControl()));
    connect(ui->actionTodayContracts,SIGNAL(toggled(bool)),this,SLOT(updateTables()));
    connect(ui->actionPaidContracts,SIGNAL(toggled(bool)),this,SLOT(updateTables()));
    connect(ui->actionNotPaidContracts,SIGNAL(toggled(bool)),this,SLOT(updateTables()));


    updateTables();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::quitSlot()
{
    //qDebug() << "quitSlot();";
    this->close();
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

     //ui->upperTable->clear();
     ui->upperTable->setRowCount(0);
     int tableRowCount = 1;

     QList<QAction*> actions = ui->menuDocs->actions();

     //bool actionAll = false;
     bool actionToday = false;
     bool actionPaid = false;
     bool actionNotPaid = false;

     for(int i = 0 ; i < actions.count() ; i++)
     {
        //qDebug() << actions.at(i)->iconText();
        //qDebug() << actions.at(i)->isChecked();
        /*
        if(!QString::compare("Все договора",actions.at(i)->iconText()) && actions.at(i)->isChecked())
        {
            actionAll = true;
        }
        */
        if(!QString::compare("К выдаче сегодня",actions.at(i)->iconText()) && actions.at(i)->isChecked())
        {
            actionToday = true;
        }
        if(!QString::compare("Оплаченные договора",actions.at(i)->iconText()) && actions.at(i)->isChecked())
        {
            actionPaid = true;
        }
        if(!QString::compare("Не оплаченные договора",actions.at(i)->iconText()) && actions.at(i)->isChecked())
        {
            actionNotPaid = true;
        }
        //qDebug() << actionAll;
        //qDebug() << actionToday;
        //qDebug() << actionPaid;
        //qDebug() << actionNotPaid;
     }

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

     //ui->upperTable->setRowCount(childs.length());

     if(actionPaid)
     {
         for (int i = 0; i < childs.length(); i ++)
         {
             QDomElement child = childs.at(i).toElement();

             QDomAttr a = child.attributeNode("number");
             QString NumberContract = a.value();

             QDomAttr b = child.attributeNode("code");
             QString Code = b.value();

             QDomNodeList payNode = child.elementsByTagName("Pay");
             QString pay = payNode.at(0).toElement().text();
             qDebug() << "pay = " << pay;

             QDomNodeList dateNode = child.elementsByTagName("DatePay");
             QString payDate = dateNode.at(0).toElement().text();
             qDebug() << "payDate = " << payDate;

             if(!QString::compare("ДА",pay))
             {
                 QDomNodeList hosts = child.childNodes();
                 ui->upperTable->setRowCount(tableRowCount);

                 for(int j = 0 ; j < hosts.length(); j++)
                 {
                     QDomElement host = hosts.at(j).toElement();
                     ui->upperTable->setItem(tableRowCount-1,j+2,new QTableWidgetItem(host.text()));
                     ui->upperTable->setItem(tableRowCount-1,0,new QTableWidgetItem(NumberContract));
                     ui->upperTable->setItem(tableRowCount-1,1,new QTableWidgetItem(Code));
                 }
                 tableRowCount++;
             }
             else continue;
        }
     }

     if(actionNotPaid)
     {
         for (int i = 0; i < childs.length(); i ++)
         {
             QDomElement child = childs.at(i).toElement();

             QDomAttr a = child.attributeNode("number");
             QString NumberContract = a.value();

             QDomAttr b = child.attributeNode("code");
             QString Code = b.value();

             QDomNodeList payNode = child.elementsByTagName("Pay");
             QString pay = payNode.at(0).toElement().text();
             qDebug() << "pay = " << pay;

             QDomNodeList dateNode = child.elementsByTagName("DatePay");
             QString payDate = dateNode.at(0).toElement().text();
             qDebug() << "payDate = " << payDate;

             if(!QString::compare("НЕТ",pay))
             {
                 QDomNodeList hosts = child.childNodes();
                 ui->upperTable->setRowCount(tableRowCount);

                 for(int j = 0 ; j < hosts.length(); j++)
                 {
                     QDomElement host = hosts.at(j).toElement();
                     ui->upperTable->setItem(tableRowCount-1,j+2,new QTableWidgetItem(host.text()));
                     ui->upperTable->setItem(tableRowCount-1,0,new QTableWidgetItem(NumberContract));
                     ui->upperTable->setItem(tableRowCount-1,1,new QTableWidgetItem(Code));

                 }
                 tableRowCount++;
             }
             else continue;
        }
     }

     if(actionToday)
     {
         for (int i = 0; i < childs.length(); i ++)
         {
             QDomElement child = childs.at(i).toElement();

             QDomAttr a = child.attributeNode("number");
             QString NumberContract = a.value();

             QDomAttr b = child.attributeNode("code");
             QString Code = b.value();

             QDomNodeList payNode = child.elementsByTagName("Pay");
             QString pay = payNode.at(0).toElement().text();
             qDebug() << "pay = " << pay;

             QDomNodeList dateNode = child.elementsByTagName("DatePay");
             QString payDate = dateNode.at(0).toElement().text();
             qDebug() << "payDate = " << payDate;

             QDomNodeList urgentNode = child.elementsByTagName("Urgent");
             QString urgentStatus = urgentNode.at(0).toElement().text();
             qDebug() << "urgentStatus = " << urgentStatus;

             QDomNodeList sendNode = child.elementsByTagName("SentToCustomer");
             QString sendStatus = sendNode.at(0).toElement().text();

             // Проверять соблюдение условия здесь олачен и выдать сегодня/просрочен
             if(!QString::compare("ДА",pay) && getRowStatus(payDate, urgentStatus) && !QString::compare("НЕТ",sendStatus))
             {
                 QDomNodeList hosts = child.childNodes();
                 ui->upperTable->setRowCount(tableRowCount);

                 for(int j = 0 ; j < hosts.length(); j++)
                 {
                     QDomElement host = hosts.at(j).toElement();
                     ui->upperTable->setItem(tableRowCount-1,j+2,new QTableWidgetItem(host.text()));
                     ui->upperTable->setItem(tableRowCount-1,0,new QTableWidgetItem(NumberContract));
                     ui->upperTable->setItem(tableRowCount-1,1,new QTableWidgetItem(Code));

                 }
                 tableRowCount++;
             }
             else continue;
        }
     }

     if(!actionToday && !actionPaid && !actionNotPaid)
     {
         for (int i = 0; i < childs.length(); i ++)
         {
             QDomElement child = childs.at(i).toElement();

             QDomAttr a = child.attributeNode("number");
             QString NumberContract = a.value();

             QDomAttr b = child.attributeNode("code");
             QString Code = b.value();

             QDomNodeList payNode = child.elementsByTagName("Pay");
             QString pay = payNode.at(0).toElement().text();
             qDebug() << "pay = " << pay;

             QDomNodeList dateNode = child.elementsByTagName("DatePay");
             QString payDate = dateNode.at(0).toElement().text();
             qDebug() << "payDate = " << payDate;

             QDomNodeList urgentNode = child.elementsByTagName("Urgent");
             QString urgentStatus = urgentNode.at(0).toElement().text();
             qDebug() << "urgentStatus = " << urgentStatus;


                 QDomNodeList hosts = child.childNodes();
                 ui->upperTable->setRowCount(tableRowCount);

                 for(int j = 0 ; j < hosts.length(); j++)
                 {
                     QDomElement host = hosts.at(j).toElement();
                     ui->upperTable->setItem(tableRowCount-1,j+2,new QTableWidgetItem(host.text()));
                     ui->upperTable->setItem(tableRowCount-1,0,new QTableWidgetItem(NumberContract));
                     ui->upperTable->setItem(tableRowCount-1,1,new QTableWidgetItem(Code));

                 }
                 tableRowCount++;
        }
     }

    setDelayColorTable();

    this->ui->upperTable->resizeColumnsToContents();
    this->ui->upperTable->resizeRowsToContents();
    this->ui->downTable->resizeColumnsToContents();
    this->ui->downTable->resizeRowsToContents();
}

void MainWindow::updateDownTable()
{
    int curentRow = ui->upperTable->currentRow();

    // если протокол не срочный
    if(ui->upperTable->item(curentRow,10)->text() == "НЕТ")
    {
        // получаем текущую дату оплаты
        QDate datePaid = QDate::fromString(ui->upperTable->item(curentRow,9)->text(),"dd.MM.yyyy");

        // получаем дату выдачи
        QDate dateOfIssue = datePaid.addDays(14);

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
            if(ui->upperTable->item(curentRow,11)->text() != "ДА") emit delayContract(curentRow);
        }

    }
    // если срочный
    else
    {
        // получаем текущую дату оплаты
        QDate datePaid = QDate::fromString(ui->upperTable->item(curentRow,9)->text(),"dd.MM.yyyy");

        QDate dateOfIssue;

        // получаем дату выдачи
        if(datePaid.dayOfWeek() != 5)
        {
            dateOfIssue = datePaid.addDays(1);
        }
        else
        {
            dateOfIssue = datePaid.addDays(3);
        }

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
            if(ui->upperTable->item(curentRow,11)->text() != "ДА") emit delayContract(curentRow);
        }
    }
}

void MainWindow::setDelayColor(int row)
{
    qDebug() << "Строка будет покрашена в красный = " << row;
    ui->upperTable->item(row,0)->setBackground(Qt::red);
    ui->upperTable->item(row,1)->setBackground(Qt::red);
    ui->upperTable->item(row,2)->setBackground(Qt::red);
    ui->upperTable->item(row,3)->setBackground(Qt::red);
    ui->upperTable->item(row,4)->setBackground(Qt::red);
    ui->upperTable->item(row,5)->setBackground(Qt::red);
    ui->upperTable->item(row,6)->setBackground(Qt::red);
    ui->upperTable->item(row,7)->setBackground(Qt::red);
    ui->upperTable->item(row,8)->setBackground(Qt::red);
    ui->upperTable->item(row,9)->setBackground(Qt::red);
    ui->upperTable->item(row,10)->setBackground(Qt::red);
    ui->upperTable->item(row,11)->setBackground(Qt::red);
}

void MainWindow::setSendColor(int row)
{
    qDebug() << "Строка будет покрашена в зелёный = " << row;
    ui->upperTable->item(row,0)->setBackground(Qt::green);
    ui->upperTable->item(row,1)->setBackground(Qt::green);
    ui->upperTable->item(row,2)->setBackground(Qt::green);
    ui->upperTable->item(row,3)->setBackground(Qt::green);
    ui->upperTable->item(row,4)->setBackground(Qt::green);
    ui->upperTable->item(row,5)->setBackground(Qt::green);
    ui->upperTable->item(row,6)->setBackground(Qt::green);
    ui->upperTable->item(row,7)->setBackground(Qt::green);
    ui->upperTable->item(row,8)->setBackground(Qt::green);
    ui->upperTable->item(row,9)->setBackground(Qt::green);
    ui->upperTable->item(row,10)->setBackground(Qt::green);
    ui->upperTable->item(row,11)->setBackground(Qt::green);
}



void MainWindow::setDelayColorTable()
{
    for(int i = 0 ; i < ui->upperTable->rowCount() ; i++)
    {
        // если протокол не срочный
        if(ui->upperTable->item(i,10)->text() == "НЕТ")
        {
            // получаем текущую дату оплаты
            QDate datePaid = QDate::fromString(ui->upperTable->item(i,9)->text(),"dd.MM.yyyy");

            // получаем дату выдачи
            QDate dateOfIssue = datePaid.addDays(14);

            // Получаем разность между датой выдачи и текущей датой (системное время)
            QString today = QDate::currentDate().toString("dd.MM.yyyy");
            int daysToEnd = QDate::fromString(today,"dd.MM.yyyy").daysTo(dateOfIssue);

            if(daysToEnd < 0) emit delayContract(i);

        }
        // если срочный
        else
        {
            // получаем текущую дату оплаты
            QDate datePaid = QDate::fromString(ui->upperTable->item(i,9)->text(),"dd.MM.yyyy");

            QDate dateOfIssue;

            // получаем дату выдачи
            if(datePaid.dayOfWeek() != 5) dateOfIssue = datePaid.addDays(1);
            else dateOfIssue = datePaid.addDays(3);

            // Получаем разность между датой выдачи и текущей датой (системное время)
            QString today = QDate::currentDate().toString("dd.MM.yyyy");
            int daysToEnd = QDate::fromString(today,"dd.MM.yyyy").daysTo(dateOfIssue);

            if(daysToEnd < 0) emit delayContract(i);

        }

        if(ui->upperTable->item(i,11)->text() == "ДА") emit sendContract(i);
    }
}

void MainWindow::menuControl()
{
    qDebug() << "MainWindow::menuControl(bool state)";
    ui->actionTodayContracts->setChecked(false);
    ui->actionPaidContracts->setChecked(false);
    ui->actionNotPaidContracts->setChecked(false);
    updateTables();
}

QDomElement MainWindow::findNecessaryNode(const QDomNode& node,
                                         const QString& necessaryName,
                                         const QString& number,
                                         const QString& code)
{
    qDebug() << 1;
    if(node.isElement())
    {
        QDomElement element = node.toElement();

        qDebug() << "element.attribute(\"number\") = " << element.attribute("number");
        qDebug() << "number = " << number;

        if(element.tagName() == necessaryName
                && element.attribute("number") == number
                && element.attribute("code") == code)
        {
            qDebug() << "RETURNED ELEMENT HAS ATTRIBUTE = " << element.attribute("number");
            qDebug() << "address element = " << &element;
            return element;
        }
    }

    qDebug() << 2;

    QDomNode siblingNode = node.nextSiblingElement();
    while(!siblingNode.isNull()){
        QDomElement res = findNecessaryNode( siblingNode, necessaryName, number, code);
        if(!res.isNull())
            //return siblingNode.toElement();
            return res;
        siblingNode = siblingNode.nextSiblingElement();
    }

    qDebug() << 3;

    QDomNode childNode = node.firstChild();
    if(!childNode.isNull()){
        QDomElement res = findNecessaryNode( childNode, necessaryName, number, code);
        if(!res.isNull()) return res;
    }

    //return ;
}

void MainWindow::clearSendContracts()
{
    QFile file("contracts.xml");
    QMessageBox msgBox;
    msgBox.setWindowTitle("Очистка");
    msgBox.setText("Из системы будут удалены протоколы\nотправленные заказчикам.");
    msgBox.setInformativeText("Вы уверены?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Warning);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Ok:

        for(int i = 0 ; i < ui->upperTable->rowCount() ; i++)
        {

            if(!QString::compare("ДА",ui->upperTable->item(i,11)->text()))
            {
                if(file.open(QIODevice::ReadWrite)) {
                    QDomDocument doc("contracts");
                    doc.setContent(&file);
                    file.close();
                    QDomElement  domElement = doc.documentElement();

                    QDomElement element;
                    element = findNecessaryNode(domElement,
                                                "contract",
                                                //ui->lineEdit_ContractNumber->text(),
                                                //ui->lineEdit_Code->text());
                                                ui->upperTable->item(i,0)->text(),
                                                ui->upperTable->item(i,1)->text());

                    qDebug() << "find element for delete has attribute = " << element.attribute("number");
                    qDebug() << "address element = " << &element;

                    domElement.removeChild(element);

                    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) file.close();

                    if(file.open(QIODevice::WriteOnly)) {
                        QTextStream(&file) << doc.toString();
                        file.close();
                    }
                }
            }
        }
        updateTables();
          break;
      case QMessageBox::Cancel:
          // Cancel was clicked
          break;
      default:
          // should never be reached
          break;
    }


}

bool MainWindow::getRowStatus(QString currentPayDate, QString urgent)
{
        // если протокол не срочный
        if(urgent == "НЕТ")
        {
            // получаем текущую дату оплаты
            QDate datePaid = QDate::fromString(currentPayDate,"dd.MM.yyyy");

            // получаем дату выдачи
            QDate dateOfIssue = datePaid.addDays(14);

            // Получаем разность между датой выдачи и текущей датой (системное время)
            QString today = QDate::currentDate().toString("dd.MM.yyyy");
            int daysToEnd = QDate::fromString(today,"dd.MM.yyyy").daysTo(dateOfIssue);

            if(daysToEnd <= 0) return true;
        }
        // если срочный
        else
        {
            // получаем текущую дату оплаты
            QDate datePaid = QDate::fromString(currentPayDate,"dd.MM.yyyy");

            QDate dateOfIssue;

            // получаем дату выдачи
            if(datePaid.dayOfWeek() != 5) dateOfIssue = datePaid.addDays(1);
            else dateOfIssue = datePaid.addDays(3);

            // Получаем разность между датой выдачи и текущей датой (системное время)
            QString today = QDate::currentDate().toString("dd.MM.yyyy");
            int daysToEnd = QDate::fromString(today,"dd.MM.yyyy").daysTo(dateOfIssue);

            if(daysToEnd <= 0) return true;
        }
        return false;
}

void MainWindow::viewAboutDialog()
{
    //qDebug() << "MainWindow::viewAboutDialog()";
    AboutDialog *about;
    about = new AboutDialog();
    about->exec();
}


