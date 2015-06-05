#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QDomNode>
#include <QFile>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTableWidget>

namespace Ui {
class addDialog;
}

class addDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDialog(QWidget *parent = 0, int state = 0, int _currentRow = 0, QTableWidget *_table = 0);
    ~addDialog();
    QDomElement contract(QDomDocument  &domDoc,
                         //const QString &Code,
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
                            const QString& strAttr1 = QString::null,
                            const QString& strAttr2 = QString::null,
                            const QString& strText = QString::null
                           );
    int state;          // 0 - Добавление
                        // 1 - Изменение
                        // 2 - Удаление
                        // 3 - Копирование

    int currentRow;
    QTableWidget *table;

    QDomElement findNecessaryNode(const QDomNode& node,
                                  const QString& necessaryName,
                                  const QString& number,
                                  const QString& code);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void stateControl(QString);    // Контроль состояния формы

private:
    Ui::addDialog *ui;

};

#endif // ADDDIALOG_H
