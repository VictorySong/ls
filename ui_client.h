/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_client
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QLineEdit *multicastip;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *udpport;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *serverip;
    QLabel *label_2;
    QLineEdit *serverport;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QLineEdit *localip;
    QLabel *label_8;
    QLineEdit *localport;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLineEdit *x;
    QLabel *label_4;
    QLineEdit *y;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QTableWidget *tableWidget;

    void setupUi(QWidget *client)
    {
        if (client->objectName().isEmpty())
            client->setObjectName(QStringLiteral("client"));
        client->resize(531, 413);
        gridLayout = new QGridLayout(client);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_6 = new QLabel(client);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_6->addWidget(label_6);

        multicastip = new QLineEdit(client);
        multicastip->setObjectName(QStringLiteral("multicastip"));
        multicastip->setEnabled(false);

        horizontalLayout_6->addWidget(multicastip);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_5 = new QLabel(client);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_5->addWidget(label_5);

        udpport = new QLineEdit(client);
        udpport->setObjectName(QStringLiteral("udpport"));
        udpport->setEnabled(false);

        horizontalLayout_5->addWidget(udpport);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(client);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(60, 0));

        horizontalLayout->addWidget(label);

        serverip = new QLineEdit(client);
        serverip->setObjectName(QStringLiteral("serverip"));
        serverip->setEnabled(false);

        horizontalLayout->addWidget(serverip);

        label_2 = new QLabel(client);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(70, 0));

        horizontalLayout->addWidget(label_2);

        serverport = new QLineEdit(client);
        serverport->setObjectName(QStringLiteral("serverport"));
        serverport->setEnabled(false);

        horizontalLayout->addWidget(serverport);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_7 = new QLabel(client);
        label_7->setObjectName(QStringLiteral("label_7"));
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);
        label_7->setMinimumSize(QSize(60, 0));

        horizontalLayout_7->addWidget(label_7);

        localip = new QLineEdit(client);
        localip->setObjectName(QStringLiteral("localip"));
        localip->setEnabled(false);

        horizontalLayout_7->addWidget(localip);

        label_8 = new QLabel(client);
        label_8->setObjectName(QStringLiteral("label_8"));
        sizePolicy.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy);
        label_8->setMinimumSize(QSize(70, 0));

        horizontalLayout_7->addWidget(label_8);

        localport = new QLineEdit(client);
        localport->setObjectName(QStringLiteral("localport"));
        localport->setEnabled(false);

        horizontalLayout_7->addWidget(localport);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_3 = new QLabel(client);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMinimumSize(QSize(60, 0));

        horizontalLayout_2->addWidget(label_3);

        x = new QLineEdit(client);
        x->setObjectName(QStringLiteral("x"));

        horizontalLayout_2->addWidget(x);

        label_4 = new QLabel(client);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setMinimumSize(QSize(70, 0));

        horizontalLayout_2->addWidget(label_4);

        y = new QLineEdit(client);
        y->setObjectName(QStringLiteral("y"));

        horizontalLayout_2->addWidget(y);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        pushButton_2 = new QPushButton(client);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_3->addWidget(pushButton_2);

        pushButton = new QPushButton(client);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_3->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        pushButton_3 = new QPushButton(client);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout_4->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(client);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout_4->addWidget(pushButton_4);


        verticalLayout->addLayout(horizontalLayout_4);

        tableWidget = new QTableWidget(client);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        verticalLayout->addWidget(tableWidget);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(client);

        QMetaObject::connectSlotsByName(client);
    } // setupUi

    void retranslateUi(QWidget *client)
    {
        client->setWindowTitle(QApplication::translate("client", "Form", nullptr));
        label_6->setText(QApplication::translate("client", "udp\345\271\277\346\222\255\345\234\260\345\235\200\357\274\232", nullptr));
        multicastip->setText(QApplication::translate("client", "239.255.43.21", nullptr));
        label_5->setText(QApplication::translate("client", "udp\347\233\221\345\220\254\347\253\257\345\217\243\357\274\232", nullptr));
        udpport->setText(QApplication::translate("client", "4567", nullptr));
        label->setText(QApplication::translate("client", "\346\234\215\345\212\241\345\231\250ip\357\274\232", nullptr));
        label_2->setText(QApplication::translate("client", "\346\234\215\345\212\241\345\231\250\347\253\257\345\217\243\357\274\232", nullptr));
        label_7->setText(QApplication::translate("client", "\345\275\223\345\211\215ip\357\274\232", nullptr));
        label_8->setText(QApplication::translate("client", "\345\275\223\345\211\215\347\253\257\345\217\243\357\274\232", nullptr));
        label_3->setText(QApplication::translate("client", "\344\275\215\347\275\256x\357\274\232", nullptr));
        label_4->setText(QApplication::translate("client", "\344\275\215\347\275\256y\357\274\232", nullptr));
        pushButton_2->setText(QApplication::translate("client", "\346\226\255\345\274\200\350\277\236\346\216\245", nullptr));
        pushButton->setText(QApplication::translate("client", "\345\217\221\350\265\267tcp\350\277\236\346\216\245", nullptr));
        pushButton_3->setText(QApplication::translate("client", "\345\217\221\351\200\201\344\275\215\347\275\256", nullptr));
        pushButton_4->setText(QApplication::translate("client", "\347\224\237\346\210\220\351\232\217\346\234\272\344\275\215\347\275\256\345\271\266\345\217\221\351\200\201", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("client", "ip", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("client", "\347\253\257\345\217\243", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("client", "\346\250\252\345\235\220\346\240\207x", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("client", "\347\272\265\345\235\220\346\240\207y", nullptr));
    } // retranslateUi

};

namespace Ui {
    class client: public Ui_client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
