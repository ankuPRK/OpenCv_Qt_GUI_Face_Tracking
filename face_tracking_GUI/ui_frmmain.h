/********************************************************************************
** Form generated from reading UI file 'frmmain.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRMMAIN_H
#define UI_FRMMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_frmmain
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *lblChosenFile;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnOpenWebCam;
    QPushButton *btnOpenFile;
    QPushButton *btnPlayPause;
    QPushButton *btnStop;
    QPushButton *btnToggleDots;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lblOriginal;
    QLabel *lblCanny;

    void setupUi(QMainWindow *frmmain)
    {
        if (frmmain->objectName().isEmpty())
            frmmain->setObjectName(QStringLiteral("frmmain"));
        frmmain->resize(710, 377);
        centralWidget = new QWidget(frmmain);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lblChosenFile = new QLabel(centralWidget);
        lblChosenFile->setObjectName(QStringLiteral("lblChosenFile"));
        QFont font;
        font.setPointSize(11);
        lblChosenFile->setFont(font);
        lblChosenFile->setAutoFillBackground(true);

        verticalLayout->addWidget(lblChosenFile);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnOpenWebCam = new QPushButton(centralWidget);
        btnOpenWebCam->setObjectName(QStringLiteral("btnOpenWebCam"));
        btnOpenWebCam->setFont(font);

        horizontalLayout->addWidget(btnOpenWebCam);

        btnOpenFile = new QPushButton(centralWidget);
        btnOpenFile->setObjectName(QStringLiteral("btnOpenFile"));
        btnOpenFile->setFont(font);

        horizontalLayout->addWidget(btnOpenFile);

        btnPlayPause = new QPushButton(centralWidget);
        btnPlayPause->setObjectName(QStringLiteral("btnPlayPause"));
        btnPlayPause->setEnabled(false);
        btnPlayPause->setFont(font);

        horizontalLayout->addWidget(btnPlayPause);

        btnStop = new QPushButton(centralWidget);
        btnStop->setObjectName(QStringLiteral("btnStop"));
        btnStop->setEnabled(false);
        btnStop->setFont(font);

        horizontalLayout->addWidget(btnStop);

        btnToggleDots = new QPushButton(centralWidget);
        btnToggleDots->setObjectName(QStringLiteral("btnToggleDots"));
        btnToggleDots->setEnabled(false);
        btnToggleDots->setFont(font);

        horizontalLayout->addWidget(btnToggleDots);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        lblOriginal = new QLabel(centralWidget);
        lblOriginal->setObjectName(QStringLiteral("lblOriginal"));
        lblOriginal->setAutoFillBackground(true);
        lblOriginal->setFrameShape(QFrame::Box);
        lblOriginal->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(lblOriginal);

        lblCanny = new QLabel(centralWidget);
        lblCanny->setObjectName(QStringLiteral("lblCanny"));
        lblCanny->setAutoFillBackground(true);
        lblCanny->setFrameShape(QFrame::Box);
        lblCanny->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(lblCanny);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        gridLayout->setRowStretch(1, 1);
        frmmain->setCentralWidget(centralWidget);

        retranslateUi(frmmain);

        QMetaObject::connectSlotsByName(frmmain);
    } // setupUi

    void retranslateUi(QMainWindow *frmmain)
    {
        frmmain->setWindowTitle(QApplication::translate("frmmain", "frmmain", 0));
        lblChosenFile->setText(QString());
        btnOpenWebCam->setText(QApplication::translate("frmmain", "Open WebCam", 0));
        btnOpenFile->setText(QApplication::translate("frmmain", "Open Video File", 0));
        btnPlayPause->setText(QApplication::translate("frmmain", "Play/Pause", 0));
        btnStop->setText(QApplication::translate("frmmain", "Stop", 0));
        btnToggleDots->setText(QApplication::translate("frmmain", "Toggle Dots", 0));
        lblOriginal->setText(QString());
        lblCanny->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class frmmain: public Ui_frmmain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMMAIN_H
