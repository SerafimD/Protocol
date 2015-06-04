#include "addDialog.h"
#include "ui_addDialog.h"
#include "mainwindow.h"

addDialog::addDialog(QWidget *parent, int state, int _currentRow, QTableWidget *_table) :
    QDialog(parent),
    ui(new Ui::addDialog)
{
    ui->setupUi(this);
    this->state = state;

    currentRow = _currentRow;
    table = _table;

    if(state == 0)
    {
        // добавление
        this->setWindowTitle("Add contract");

        ui->dateEdit_DateOfReceipt->setDate(QDate::currentDate ());
        ui->dateEdit_DatePay->setDate(QDate::currentDate ());
        ui->dateEdit_DateReceiptResults->setDate(QDate::currentDate ());
        ui->dateEdit_DateTransferLaboratory->setDate(QDate::currentDate ());

        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else if(state == 1)
    {
        // изменение
        this->setWindowTitle("Change contract");

    }
    else if(state == 2)
    {
        // удаление
        this->setWindowTitle("Delete contract");

        ui->lineEdit_ContractNumber->setText(table->item(currentRow,0)->text());
        ui->lineEdit_Code->setText(table->item(currentRow,1)->text());
        ui->dateEdit_DateOfReceipt->setDate(QDate::fromString(table->item(currentRow,2)->text(),"dd.MM.yy"));
        ui->dateEdit_DateTransferLaboratory->setDate(QDate::fromString(table->item(currentRow,3)->text(),"dd.MM.yy"));
        ui->dateEdit_DateReceiptResults->setDate(QDate::fromString(table->item(currentRow,4)->text(),"dd.MM.yy"));
        ui->lineEdit_AccountNumber->setText(table->item(currentRow,5)->text());
        ui->dateEdit_DatePay->setDate(QDate::fromString(table->item(currentRow,6)->text(),"dd.MM.yy"));
        ui->checkBox_Urgent->setChecked(!QString::compare(table->item(currentRow,7)->text(),"ДА"));
        ui->checkBox_SentToCustomer->setChecked(!QString::compare(table->item(currentRow,8)->text(),"ДА"));
        ui->textEdit_Comment->setText(table->item(currentRow,9)->text());

        ui->lineEdit_ContractNumber->setEnabled(false);
        ui->lineEdit_Code->setEnabled(false);
        ui->dateEdit_DateOfReceipt->setEnabled(false);
        ui->dateEdit_DateTransferLaboratory->setEnabled(false);
        ui->dateEdit_DateReceiptResults->setEnabled(false);
        ui->lineEdit_AccountNumber->setEnabled(false);
        ui->dateEdit_DatePay->setEnabled(false);
        ui->checkBox_Urgent->setEnabled(false);
        ui->checkBox_SentToCustomer->setEnabled(false);
        ui->textEdit_Comment->setEnabled(false);

    }
    else if(state == 3)
    {
        // копирование
        this->setWindowTitle("Copy contract");

    }

    // установка даты по умолчанию
    ui->dateEdit_DateOfReceipt->setCalendarPopup(true);
    ui->dateEdit_DatePay->setCalendarPopup(true);
    ui->dateEdit_DateReceiptResults->setCalendarPopup(true);
    ui->dateEdit_DateTransferLaboratory->setCalendarPopup(true);



    connect(ui->lineEdit_Code,SIGNAL(textChanged(QString)),SLOT(stateControl(QString)));
    connect(ui->lineEdit_ContractNumber,SIGNAL(textChanged(QString)),SLOT(stateControl(QString)));
}

addDialog::~addDialog()
{
    delete ui;
}

QDomElement addDialog::contract(QDomDocument  &domDoc,
                                const QString &Code,
                                const QString &DateOfReceipt,
                                const QString &DateTransferLaboratory,
                                const QString &DateReceiptResults,
                                const QString &AccountNumber,
                                const QString &DatePay,
                                const QString &Urgent,
                                const QString &SentToCustomer,
                                const QString &Comment
                                )
{
    const QString nNumber = this->ui->lineEdit_ContractNumber->text();

    qDebug() << "Urgent = " << Urgent;
    qDebug() << "SentToCustomer = " << SentToCustomer;

    QDomElement domElement = makeElement(domDoc,
                                         "contract",
                                         nNumber
                                        );
    domElement.appendChild(makeElement(domDoc, "Code", "", Code));
    domElement.appendChild(makeElement(domDoc, "DateOfReceipt", "", DateOfReceipt));
    domElement.appendChild(makeElement(domDoc, "DateTransferLaboratory", "", DateTransferLaboratory));
    domElement.appendChild(makeElement(domDoc, "DateReceiptResults", "", DateReceiptResults));
    domElement.appendChild(makeElement(domDoc, "AccountNumber", "", AccountNumber));
    domElement.appendChild(makeElement(domDoc, "DatePay", "", DatePay));
    domElement.appendChild(makeElement(domDoc, "Urgent", "", Urgent));
    domElement.appendChild(makeElement(domDoc, "SentToCustomer", "", SentToCustomer));
    domElement.appendChild(makeElement(domDoc, "Comment", "", Comment));

    return domElement;
}

QDomElement addDialog::makeElement(QDomDocument &domDoc, const QString &strName, const QString &strAttr, const QString &strText)
{
    QDomElement domElement = domDoc.createElement(strName);

        if (!strAttr.isEmpty()) {
            QDomAttr domAttr = domDoc.createAttribute("number");
            domAttr.setValue(strAttr);
            domElement.setAttributeNode(domAttr);
        }

        if (!strText.isEmpty()) {
            QDomText domText = domDoc.createTextNode(strText);
            domElement.appendChild(domText);
        }
        return domElement;
}

void addDialog::on_buttonBox_accepted()
{
    QFile file("contracts.xml");

    switch (state) {
    case 0:
        if (!file.exists()){

            qDebug() << "Not create";
            // файл бд не создан, создаём его
            QDomDocument doc("contracts");
            QDomElement  domElement = doc.createElement("contracts");
            doc.appendChild(domElement);

            QString nUrgent;
            QString nSentToCustomer;

            if (ui->checkBox_Urgent->isChecked())   nUrgent = "дб";
            else                                    nUrgent = "оеф";

            if (ui->checkBox_SentToCustomer->isChecked()) nSentToCustomer = "дб";
            else                                          nSentToCustomer = "оеф";

            QDomElement cnt =
                contract(doc,
                         ui->lineEdit_Code->text(),
                         ui->dateEdit_DateOfReceipt->text(),
                         ui->dateEdit_DateTransferLaboratory->text(),
                         ui->dateEdit_DateReceiptResults->text(),
                         ui->lineEdit_AccountNumber->text(),
                         ui->dateEdit_DatePay->text(),
                         nUrgent,
                         nSentToCustomer,
                         ui->textEdit_Comment->toPlainText()
                         );

            domElement.appendChild(cnt);

            if(file.open(QIODevice::WriteOnly)) {
                QTextStream(&file) << doc.toString();
                file.close();
            }
        }
        else
        {
            qDebug() << "Created";
            // файл создан, пишем в него
            if(file.open(QIODevice::ReadWrite)) {
                QDomDocument doc("contracts");
                doc.setContent(&file);
                file.close();
                QDomElement  domElement = doc.documentElement();
                //doc.appendChild(domElement);

                QString nUrgent;
                QString nSentToCustomer;

                if (ui->checkBox_Urgent->isChecked())   nUrgent = "ДА";
                else                                    nUrgent = "НЕТ";

                if (ui->checkBox_SentToCustomer->isChecked()) nSentToCustomer = "ДА";
                else                                          nSentToCustomer = "НЕТ";

                QDomElement cnt =
                    contract(doc,
                             ui->lineEdit_Code->text(),
                             ui->dateEdit_DateOfReceipt->text(),
                             ui->dateEdit_DateTransferLaboratory->text(),
                             ui->dateEdit_DateReceiptResults->text(),
                             ui->lineEdit_AccountNumber->text(),
                             ui->dateEdit_DatePay->text(),
                             nUrgent,
                             nSentToCustomer,
                             ui->textEdit_Comment->toPlainText()
                             );

                domElement.appendChild(cnt);

                if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) file.close();

                if(file.open(QIODevice::WriteOnly)) {
                    QTextStream(&file) << doc.toString();
                    file.close();
                }
            }
        }
        break;
    case 1:
        break;
    case 2:
        qDebug() << "Deleted";

        if(file.open(QIODevice::ReadWrite)) {
            QDomDocument doc("contracts");
            doc.setContent(&file);
            file.close();
            QDomElement  domElement = doc.documentElement();

            QDomElement element = findNecessaryNode(domElement,"contract",ui->lineEdit_ContractNumber->text());
            domElement.removeChild(element);

            if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) file.close();

            if(file.open(QIODevice::WriteOnly)) {
                QTextStream(&file) << doc.toString();
                file.close();
            }
        }
        break;
    case 3:
        break;
    default:
        break;
    }

}

QDomElement addDialog::findNecessaryNode(const QDomNode& node,const QString& necessaryName,const QString& number)
{
    //qDebug() << 1;
    if(node.isElement())
    {
        QDomElement element = node.toElement();
        if(element.tagName() == necessaryName && element.attribute("number") == number)
            return element;
    }

    //qDebug() << 2;

    QDomNode siblingNode = node.nextSiblingElement();
    while(!siblingNode.isNull()){
        QDomElement res = findNecessaryNode( siblingNode, necessaryName, number);
        if(!res.isNull())
            return siblingNode.toElement();
        siblingNode = siblingNode.nextSiblingElement();
    }

    //qDebug() << 3;

    QDomNode childNode = node.firstChild();
    if(!childNode.isNull()){
        QDomElement res = findNecessaryNode( childNode, necessaryName, number);
        if(!res.isNull()) return res;
    }

    //return ;
}

void addDialog::on_buttonBox_rejected()
{
    // ничего не делаем
}

void addDialog::stateControl(QString text)
{

    QPushButton *btn_OK;
    btn_OK = ui->buttonBox->button(QDialogButtonBox::Ok);
    if(!ui->lineEdit_ContractNumber->text().isEmpty() and !ui->lineEdit_Code->text().isEmpty()) btn_OK->setEnabled(true);
    else btn_OK->setEnabled(false);
}
