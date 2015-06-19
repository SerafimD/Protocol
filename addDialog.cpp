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

        ui->lineEdit_ContractNumber->setText(table->item(currentRow,0)->text());
        ui->lineEdit_Code->setText(table->item(currentRow,1)->text());
        ui->lineEdit_Company->setText(table->item(currentRow,2)->text());
        ui->lineEdit_Sample->setText(table->item(currentRow,3)->text());
        ui->dateEdit_DateOfReceipt->setDate(QDate::fromString(table->item(currentRow,4)->text(),"dd.MM.yyyy"));
        ui->dateEdit_DateTransferLaboratory->setDate(QDate::fromString(table->item(currentRow,5)->text(),"dd.MM.yyyy"));
        ui->dateEdit_DateReceiptResults->setDate(QDate::fromString(table->item(currentRow,6)->text(),"dd.MM.yyyy"));
        ui->lineEdit_AccountNumber->setText(table->item(currentRow,7)->text());
        ui->checkBox_Pay->setChecked(!QString::compare(table->item(currentRow,8)->text(),"ДА"));
        ui->dateEdit_DatePay->setDate(QDate::fromString(table->item(currentRow,9)->text(),"dd.MM.yyyy"));
        ui->checkBox_Urgent->setChecked(!QString::compare(table->item(currentRow,10)->text(),"ДА"));
        ui->checkBox_SentToCustomer->setChecked(!QString::compare(table->item(currentRow,11)->text(),"ДА"));
        ui->textEdit_Comment->setText(table->item(currentRow,12)->text());

    }
    else if(state == 2)
    {
        // удаление
        this->setWindowTitle("Delete contract");

        ui->lineEdit_ContractNumber->setText(table->item(currentRow,0)->text());
        ui->lineEdit_Code->setText(table->item(currentRow,1)->text());
        ui->lineEdit_Company->setText(table->item(currentRow,2)->text());
        ui->lineEdit_Sample->setText(table->item(currentRow,3)->text());
        ui->dateEdit_DateOfReceipt->setDate(QDate::fromString(table->item(currentRow,4)->text(),"dd.MM.yyyy"));
        ui->dateEdit_DateTransferLaboratory->setDate(QDate::fromString(table->item(currentRow,5)->text(),"dd.MM.yyyy"));
        ui->dateEdit_DateReceiptResults->setDate(QDate::fromString(table->item(currentRow,6)->text(),"dd.MM.yyyy"));
        ui->lineEdit_AccountNumber->setText(table->item(currentRow,7)->text());
        ui->checkBox_Pay->setChecked(!QString::compare(table->item(currentRow,8)->text(),"ДА"));
        ui->dateEdit_DatePay->setDate(QDate::fromString(table->item(currentRow,9)->text(),"dd.MM.yyyy"));
        ui->checkBox_Urgent->setChecked(!QString::compare(table->item(currentRow,10)->text(),"ДА"));
        ui->checkBox_SentToCustomer->setChecked(!QString::compare(table->item(currentRow,11)->text(),"ДА"));
        ui->textEdit_Comment->setText(table->item(currentRow,12)->text());

        ui->lineEdit_ContractNumber->setEnabled(false);
        ui->lineEdit_Code->setEnabled(false);
        ui->lineEdit_Company->setEnabled(false);
        ui->lineEdit_Sample->setEnabled(false);
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

        ui->lineEdit_ContractNumber->setText(table->item(currentRow,0)->text());
        ui->lineEdit_Code->setText(table->item(currentRow,1)->text());
        ui->lineEdit_Company->setText(table->item(currentRow,2)->text());
        ui->lineEdit_Sample->setText(table->item(currentRow,3)->text());
        ui->dateEdit_DateOfReceipt->setDate(QDate::fromString(table->item(currentRow,4)->text(),"dd.MM.yyyy"));
        ui->dateEdit_DateTransferLaboratory->setDate(QDate::fromString(table->item(currentRow,5)->text(),"dd.MM.yyyy"));
        ui->dateEdit_DateReceiptResults->setDate(QDate::fromString(table->item(currentRow,6)->text(),"dd.MM.yyyy"));
        ui->lineEdit_AccountNumber->setText(table->item(currentRow,7)->text());
        ui->checkBox_Pay->setChecked(!QString::compare(table->item(currentRow,8)->text(),"ДА"));
        ui->dateEdit_DatePay->setDate(QDate::fromString(table->item(currentRow,9)->text(),"dd.MM.yyyy"));
        ui->checkBox_Urgent->setChecked(!QString::compare(table->item(currentRow,10)->text(),"ДА"));
        ui->checkBox_SentToCustomer->setChecked(!QString::compare(table->item(currentRow,11)->text(),"ДА"));
        ui->textEdit_Comment->setText(table->item(currentRow,12)->text());
    }

    // установка даты по умолчанию
    ui->dateEdit_DateOfReceipt->setCalendarPopup(true);
    ui->dateEdit_DatePay->setCalendarPopup(true);
    ui->dateEdit_DateReceiptResults->setCalendarPopup(true);
    ui->dateEdit_DateTransferLaboratory->setCalendarPopup(true);


    connect(ui->lineEdit_Code,SIGNAL(textChanged(QString)),SLOT(stateControl(QString)));
    connect(ui->lineEdit_ContractNumber,SIGNAL(textChanged(QString)),SLOT(stateControl(QString)));
    connect(ui->checkBox_Pay,SIGNAL(clicked()),SLOT(PayState()));
}

addDialog::~addDialog()
{
    delete ui;
}

QDomElement addDialog::contract(QDomDocument  &domDoc,
                                //const QString &Code,
                                const QString &Company,
                                const QString &Sample,
                                const QString &DateOfReceipt,
                                const QString &DateTransferLaboratory,
                                const QString &DateReceiptResults,
                                const QString &AccountNumber,
                                const QString &Pay,
                                const QString &DatePay,
                                const QString &Urgent,
                                const QString &SentToCustomer,
                                const QString &Comment
                                )
{
    const QString nNumber = this->ui->lineEdit_ContractNumber->text();
    const QString nCode = this->ui->lineEdit_Code->text();

    qDebug() << "Urgent = " << Urgent;
    qDebug() << "SentToCustomer = " << SentToCustomer;

    QDomElement domElement = makeElement(domDoc,
                                         "contract",
                                         nNumber,
                                         nCode
                                        );
    //domElement.appendChild(makeElement(domDoc, "Code", "", Code));
    domElement.appendChild(makeElement(domDoc, "Company","","", Company));
    domElement.appendChild(makeElement(domDoc, "Sample","","", Sample));
    domElement.appendChild(makeElement(domDoc, "DateOfReceipt", "", "", DateOfReceipt));
    domElement.appendChild(makeElement(domDoc, "DateTransferLaboratory", "", "", DateTransferLaboratory));
    domElement.appendChild(makeElement(domDoc, "DateReceiptResults", "", "", DateReceiptResults));
    domElement.appendChild(makeElement(domDoc, "AccountNumber", "", "", AccountNumber));
    domElement.appendChild(makeElement(domDoc, "Pay", "", "", Pay));
    domElement.appendChild(makeElement(domDoc, "DatePay", "", "", DatePay));
    domElement.appendChild(makeElement(domDoc, "Urgent", "", "", Urgent));
    domElement.appendChild(makeElement(domDoc, "SentToCustomer", "", "", SentToCustomer));
    domElement.appendChild(makeElement(domDoc, "Comment", "", "", Comment));

    return domElement;
}

QDomElement addDialog::makeElement(QDomDocument &domDoc,
                                   const QString &strName,
                                   const QString &strAttr1,
                                   const QString &strAttr2,
                                   const QString &strText)
{
    QDomElement domElement = domDoc.createElement(strName);

        if (!strAttr1.isEmpty()) {
            QDomAttr domAttr = domDoc.createAttribute("number");
            domAttr.setValue(strAttr1);
            domElement.setAttributeNode(domAttr);
        }
        if (!strAttr2.isEmpty()) {
            QDomAttr domAttr = domDoc.createAttribute("code");
            domAttr.setValue(strAttr2);
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
            QString nPay;

            if (ui->checkBox_Urgent->isChecked())   nUrgent = "ДА";
            else                                    nUrgent = "НЕТ";

            if (ui->checkBox_SentToCustomer->isChecked()) nSentToCustomer = "ДА";
            else                                          nSentToCustomer = "НЕТ";

            if (ui->checkBox_Pay->isChecked()) nPay = "ДА";
            else                               nPay = "НЕТ";

            QDomElement cnt =
                contract(doc,
                         //ui->lineEdit_Code->text(),
                         ui->lineEdit_Company->text(),
                         ui->lineEdit_Sample->text(),
                         ui->dateEdit_DateOfReceipt->text(),
                         ui->dateEdit_DateTransferLaboratory->text(),
                         ui->dateEdit_DateReceiptResults->text(),
                         ui->lineEdit_AccountNumber->text(),
                         nPay,
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
                QString nPay;

                if (ui->checkBox_Urgent->isChecked())   nUrgent = "ДА";
                else                                    nUrgent = "НЕТ";

                if (ui->checkBox_SentToCustomer->isChecked()) nSentToCustomer = "ДА";
                else                                          nSentToCustomer = "НЕТ";

                if (ui->checkBox_Pay->isChecked()) nPay = "ДА";
                else                               nPay = "НЕТ";

                QDomElement cnt =
                    contract(doc,
                             //ui->lineEdit_Code->text(),
                             ui->lineEdit_Company->text(),
                             ui->lineEdit_Sample->text(),
                             ui->dateEdit_DateOfReceipt->text(),
                             ui->dateEdit_DateTransferLaboratory->text(),
                             ui->dateEdit_DateReceiptResults->text(),
                             ui->lineEdit_AccountNumber->text(),
                             nPay,
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
        qDebug() << "Updated";

        if(file.open(QIODevice::ReadWrite)) {
            QDomDocument doc("contracts");
            doc.setContent(&file);
            file.close();
            QDomElement  domElement = doc.documentElement();

            QDomElement element = findNecessaryNode(domElement,
                                                    "contract",
                                                    ui->lineEdit_ContractNumber->text(),
                                                    ui->lineEdit_Code->text());
            domElement.removeChild(element);

            QString nUrgent;
            QString nSentToCustomer;
            QString nPay;

            if (ui->checkBox_Urgent->isChecked())   nUrgent = "ДА";
            else                                    nUrgent = "НЕТ";

            if (ui->checkBox_SentToCustomer->isChecked()) nSentToCustomer = "ДА";
            else                                          nSentToCustomer = "НЕТ";

            if (ui->checkBox_Pay->isChecked()) nPay = "ДА";
            else                               nPay = "НЕТ";

            QDomElement cnt =
                contract(doc,
                         //ui->lineEdit_Code->text(),
                         ui->lineEdit_Company->text(),
                         ui->lineEdit_Sample->text(),
                         ui->dateEdit_DateOfReceipt->text(),
                         ui->dateEdit_DateTransferLaboratory->text(),
                         ui->dateEdit_DateReceiptResults->text(),
                         ui->lineEdit_AccountNumber->text(),
                         nPay,
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

        break;
    case 2:
        qDebug() << "Deleted";

        if(file.open(QIODevice::ReadWrite)) {
            QDomDocument doc("contracts");
            doc.setContent(&file);
            file.close();
            QDomElement  domElement = doc.documentElement();

            QDomElement element;
            element = findNecessaryNode(domElement,
                                        "contract",
                                        ui->lineEdit_ContractNumber->text(),
                                        ui->lineEdit_Code->text());
            qDebug() << "find element for delete has attribute = " << element.attribute("number");
            qDebug() << "address element = " << &element;
            domElement.removeChild(element);

            if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) file.close();

            if(file.open(QIODevice::WriteOnly)) {
                QTextStream(&file) << doc.toString();
                file.close();
            }
        }
        break;
    case 3:
        qDebug() << "Copy";

        if(file.open(QIODevice::ReadWrite)) {
            QDomDocument doc("contracts");
            doc.setContent(&file);
            file.close();
            QDomElement  domElement = doc.documentElement();
            //doc.appendChild(domElement);

            QString nUrgent;
            QString nSentToCustomer;
            QString nPay;

            if (ui->checkBox_Urgent->isChecked())   nUrgent = "ДА";
            else                                    nUrgent = "НЕТ";

            if (ui->checkBox_SentToCustomer->isChecked()) nSentToCustomer = "ДА";
            else                                          nSentToCustomer = "НЕТ";

            if (ui->checkBox_Pay->isChecked()) nPay = "ДА";
            else                               nPay = "НЕТ";

            QDomElement cnt =
                contract(doc,
                         //ui->lineEdit_Code->text(),
                         ui->lineEdit_Company->text(),
                         ui->lineEdit_Sample->text(),
                         ui->dateEdit_DateOfReceipt->text(),
                         ui->dateEdit_DateTransferLaboratory->text(),
                         ui->dateEdit_DateReceiptResults->text(),
                         ui->lineEdit_AccountNumber->text(),
                         nPay,
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
        break;
    default:
        break;
    }

}

QDomElement addDialog::findNecessaryNode(const QDomNode& node,
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

void addDialog::PayState()
{
    qDebug() << "addDialog::PayState()";
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
