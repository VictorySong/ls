/********************************************************************************
** Form generated from reading UI file 'arpactdev.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARPACTDEV_H
#define UI_ARPACTDEV_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_arpactdev
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QTableWidget *mactableview;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *ipcombox;
    QPushButton *getmacbutton;
    QPushButton *pushButton;
    QLabel *dev_tip;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *tcpport;
    QPushButton *listentcpport;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QVBoxLayout *connecteddev;
    QListView *listView;
    QVBoxLayout *verticalLayout_2;
    QLabel *udpget;
    QLabel *tcpget;
    QPushButton *newtcpconnect;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *lineEditx;
    QLabel *label_5;
    QLineEdit *lineEdity;
    QPushButton *pushButton_2;
    QLabel *statusLabel;

    void setupUi(QWidget *arpactdev)
    {
        if (arpactdev->objectName().isEmpty())
            arpactdev->setObjectName(QStringLiteral("arpactdev"));
        arpactdev->resize(691, 484);
        gridLayout = new QGridLayout(arpactdev);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        mactableview = new QTableWidget(arpactdev);
        if (mactableview->columnCount() < 3)
            mactableview->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        mactableview->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        mactableview->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        mactableview->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        mactableview->setObjectName(QStringLiteral("mactableview"));
        mactableview->setMinimumSize(QSize(380, 229));
        mactableview->horizontalHeader()->setDefaultSectionSize(120);

        verticalLayout->addWidget(mactableview);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(arpactdev);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMaximumSize(QSize(60, 16777215));

        horizontalLayout->addWidget(label);

        ipcombox = new QComboBox(arpactdev);
        ipcombox->addItem(QString());
        ipcombox->setObjectName(QStringLiteral("ipcombox"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ipcombox->sizePolicy().hasHeightForWidth());
        ipcombox->setSizePolicy(sizePolicy1);
        ipcombox->setMinimumSize(QSize(120, 0));

        horizontalLayout->addWidget(ipcombox);

        getmacbutton = new QPushButton(arpactdev);
        getmacbutton->setObjectName(QStringLiteral("getmacbutton"));

        horizontalLayout->addWidget(getmacbutton);

        pushButton = new QPushButton(arpactdev);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);

        dev_tip = new QLabel(arpactdev);
        dev_tip->setObjectName(QStringLiteral("dev_tip"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dev_tip->sizePolicy().hasHeightForWidth());
        dev_tip->setSizePolicy(sizePolicy2);
        dev_tip->setMinimumSize(QSize(0, 50));

        verticalLayout->addWidget(dev_tip);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(arpactdev);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy3);
        label_2->setMinimumSize(QSize(100, 0));
        label_2->setMaximumSize(QSize(300, 16777215));
        label_2->setBaseSize(QSize(300, 0));

        horizontalLayout_2->addWidget(label_2);

        tcpport = new QLineEdit(arpactdev);
        tcpport->setObjectName(QStringLiteral("tcpport"));
        tcpport->setEnabled(false);
        tcpport->setMaxLength(4);

        horizontalLayout_2->addWidget(tcpport);

        listentcpport = new QPushButton(arpactdev);
        listentcpport->setObjectName(QStringLiteral("listentcpport"));

        horizontalLayout_2->addWidget(listentcpport);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_4 = new QLabel(arpactdev);
        label_4->setObjectName(QStringLiteral("label_4"));
        QSizePolicy sizePolicy4(QSizePolicy::Maximum, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy4);
        label_4->setMinimumSize(QSize(0, 20));
        label_4->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_4->addWidget(label_4);

        connecteddev = new QVBoxLayout();
        connecteddev->setObjectName(QStringLiteral("connecteddev"));
        listView = new QListView(arpactdev);
        listView->setObjectName(QStringLiteral("listView"));

        connecteddev->addWidget(listView);


        horizontalLayout_4->addLayout(connecteddev);


        gridLayout->addLayout(horizontalLayout_4, 1, 1, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        udpget = new QLabel(arpactdev);
        udpget->setObjectName(QStringLiteral("udpget"));
        sizePolicy3.setHeightForWidth(udpget->sizePolicy().hasHeightForWidth());
        udpget->setSizePolicy(sizePolicy3);
        udpget->setMinimumSize(QSize(60, 0));

        verticalLayout_2->addWidget(udpget);

        tcpget = new QLabel(arpactdev);
        tcpget->setObjectName(QStringLiteral("tcpget"));

        verticalLayout_2->addWidget(tcpget);

        newtcpconnect = new QPushButton(arpactdev);
        newtcpconnect->setObjectName(QStringLiteral("newtcpconnect"));

        verticalLayout_2->addWidget(newtcpconnect);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(arpactdev);
        label_3->setObjectName(QStringLiteral("label_3"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy5);
        label_3->setMinimumSize(QSize(50, 20));
        label_3->setBaseSize(QSize(0, 0));

        horizontalLayout_3->addWidget(label_3);

        lineEditx = new QLineEdit(arpactdev);
        lineEditx->setObjectName(QStringLiteral("lineEditx"));

        horizontalLayout_3->addWidget(lineEditx);

        label_5 = new QLabel(arpactdev);
        label_5->setObjectName(QStringLiteral("label_5"));
        sizePolicy3.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy3);
        label_5->setMinimumSize(QSize(50, 0));

        horizontalLayout_3->addWidget(label_5);

        lineEdity = new QLineEdit(arpactdev);
        lineEdity->setObjectName(QStringLiteral("lineEdity"));

        horizontalLayout_3->addWidget(lineEdity);

        pushButton_2 = new QPushButton(arpactdev);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_3->addWidget(pushButton_2);


        verticalLayout_2->addLayout(horizontalLayout_3);


        gridLayout->addLayout(verticalLayout_2, 0, 2, 1, 1);

        statusLabel = new QLabel(arpactdev);
        statusLabel->setObjectName(QStringLiteral("statusLabel"));

        gridLayout->addWidget(statusLabel, 1, 2, 1, 1);


        retranslateUi(arpactdev);

        QMetaObject::connectSlotsByName(arpactdev);
    } // setupUi

    void retranslateUi(QWidget *arpactdev)
    {
        arpactdev->setWindowTitle(QApplication::translate("arpactdev", "Form", nullptr));
        QTableWidgetItem *___qtablewidgetitem = mactableview->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("arpactdev", "ip", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = mactableview->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("arpactdev", "mac", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = mactableview->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("arpactdev", "tcp\350\277\236\346\216\245\347\212\266\346\200\201", nullptr));
        label->setText(QApplication::translate("arpactdev", "\345\217\257\347\224\250\347\232\204ip:", nullptr));
        ipcombox->setItemText(0, QApplication::translate("arpactdev", "\346\227\240", nullptr));

        getmacbutton->setText(QApplication::translate("arpactdev", "\350\216\267\345\217\226\345\217\257\347\224\250\347\232\204mac", nullptr));
        pushButton->setText(QApplication::translate("arpactdev", "\345\271\277\346\222\255\345\275\223\345\211\215ip\345\217\212\347\253\257\345\217\243", nullptr));
        dev_tip->setText(QString());
        label_2->setText(QApplication::translate("arpactdev", "\350\256\276\347\275\256\346\234\254\345\234\260tcp\347\253\257\345\217\243\357\274\232", nullptr));
        tcpport->setText(QApplication::translate("arpactdev", "7777", nullptr));
        tcpport->setPlaceholderText(QString());
        listentcpport->setText(QApplication::translate("arpactdev", "\347\233\221\345\220\254\350\257\245\347\253\257\345\217\243", nullptr));
        label_4->setText(QApplication::translate("arpactdev", "\351\200\211\346\213\251\345\267\262\350\277\236\346\216\245\350\256\276\345\244\207\357\274\232", nullptr));
        udpget->setText(QString());
        tcpget->setText(QString());
        newtcpconnect->setText(QApplication::translate("arpactdev", "\345\217\221\350\265\267tcp\350\277\236\346\216\245", nullptr));
        label_3->setText(QApplication::translate("arpactdev", "\344\275\215\347\275\256x\357\274\232", nullptr));
        label_5->setText(QApplication::translate("arpactdev", "\344\275\215\347\275\256y\357\274\232", nullptr));
        pushButton_2->setText(QApplication::translate("arpactdev", "\345\217\221\351\200\201\344\277\241\346\201\257", nullptr));
        statusLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class arpactdev: public Ui_arpactdev {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARPACTDEV_H
