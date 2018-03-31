/********************************************************************************
** Form generated from reading UI file 'selectip.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTIP_H
#define UI_SELECTIP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_selectip
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;
    QLabel *dev_tip;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *ipcomboBox;
    QPushButton *pushButton;
    QProgressBar *progressBar;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;

    void setupUi(QWidget *selectip)
    {
        if (selectip->objectName().isEmpty())
            selectip->setObjectName(QStringLiteral("selectip"));
        selectip->resize(482, 410);
        gridLayout = new QGridLayout(selectip);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableWidget = new QTableWidget(selectip);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->horizontalHeader()->setDefaultSectionSize(150);

        verticalLayout->addWidget(tableWidget);

        dev_tip = new QLabel(selectip);
        dev_tip->setObjectName(QStringLiteral("dev_tip"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dev_tip->sizePolicy().hasHeightForWidth());
        dev_tip->setSizePolicy(sizePolicy);
        dev_tip->setMinimumSize(QSize(0, 50));

        verticalLayout->addWidget(dev_tip);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(selectip);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMaximumSize(QSize(60, 16777215));

        horizontalLayout->addWidget(label);

        ipcomboBox = new QComboBox(selectip);
        ipcomboBox->setObjectName(QStringLiteral("ipcomboBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ipcomboBox->sizePolicy().hasHeightForWidth());
        ipcomboBox->setSizePolicy(sizePolicy2);
        ipcomboBox->setMinimumSize(QSize(100, 0));

        horizontalLayout->addWidget(ipcomboBox);

        pushButton = new QPushButton(selectip);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);

        progressBar = new QProgressBar(selectip);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setEnabled(false);
        progressBar->setValue(1);

        verticalLayout->addWidget(progressBar);

        textEdit = new QTextEdit(selectip);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setEnabled(false);
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy3);
        textEdit->setMaximumSize(QSize(16777215, 70));

        verticalLayout->addWidget(textEdit);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_3 = new QPushButton(selectip);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout_2->addWidget(pushButton_3);

        pushButton_2 = new QPushButton(selectip);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(selectip);

        QMetaObject::connectSlotsByName(selectip);
    } // setupUi

    void retranslateUi(QWidget *selectip)
    {
        selectip->setWindowTitle(QApplication::translate("selectip", "Form", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("selectip", "ip", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("selectip", "mac", nullptr));
        dev_tip->setText(QString());
        label->setText(QApplication::translate("selectip", "\345\217\257\347\224\250\347\232\204ip:", nullptr));
        pushButton->setText(QApplication::translate("selectip", "\350\216\267\345\217\226\346\264\273\345\212\250mac", nullptr));
        textEdit->setHtml(QApplication::translate("selectip", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\345\234\250\346\255\244\347\225\214\351\235\242\351\200\211\346\213\251ip </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\346\263\250\346\204\217\357\274\232</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">     \345\256\242\346\210\267\347\253\257ip\350\246\201\345\222\214\346\234\215\345\212\241\347\253\257ip\345\234\250\345\220\214\344\270\200\345\261\200\345\237\237\347\275\221\344\270\255\357\274\214"
                        "\346\243\200\346\265\213\346\226\271\346\263\225\345\215\263\344\270\272\347\202\271\345\207\273\342\200\234\350\216\267\345\217\226\346\264\273\345\212\250mac&quot; </p></body></html>", nullptr));
        pushButton_3->setText(QApplication::translate("selectip", "\346\211\223\345\274\200\346\234\215\345\212\241\347\253\257", nullptr));
        pushButton_2->setText(QApplication::translate("selectip", "\346\211\223\345\274\200\345\256\242\346\210\267\347\253\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class selectip: public Ui_selectip {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTIP_H
