#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QDomNode>
#include <QFile>
#include <QDialogButtonBox>
#include <QPushButton>

namespace Ui {
class addDialog;
}

class addDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDialog(QWidget *parent = 0, int state = 0, QString _contractNumber = QString::null);
    ~addDialog();
    QDomElement contract(QDomDocument  &domDoc,
                         const QString &Code,
                         const QString &DateOfReceipt,
                         const QString &DateTransferLaboratory,
                         const QString &DateReceiptResults,
                         const QString &AccountNumber,
                         const QString &DatePay,
                         const QString &Urgent,
                         const QString &SentToCustomer,
                         const QString &Comment
                       );
    QDomElement makeElement( QDomDocument& domDoc,
                            const QString& strName,
                            const QString& strAttr = QString::null,
                            const QString& strText = QString::null
                           );
    int state;          // 0 - добавление
                        // 1 - изменение
                        // 2 - удаление
                        // 3 - копирование
    QString contractNumber;

    QDomElement findNecessaryNode(const QDomNode& node,const QString& necessaryName,const QString& number);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void stateControl(QString);    // Контроль состояния формы ввода

private:
    Ui::addDialog *ui;

};

#endif // ADDDIALOG_H
