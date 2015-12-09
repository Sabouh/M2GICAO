/********************************************************************************
** Form generated from reading UI file 'histodialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISTODIALOG_H
#define UI_HISTODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_HistoDialog
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;

    void setupUi(QDialog *HistoDialog)
    {
        if (HistoDialog->objectName().isEmpty())
            HistoDialog->setObjectName(QStringLiteral("HistoDialog"));
        HistoDialog->resize(400, 300);
        gridLayout_2 = new QGridLayout(HistoDialog);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        graphicsView = new QGraphicsView(HistoDialog);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(HistoDialog);

        QMetaObject::connectSlotsByName(HistoDialog);
    } // setupUi

    void retranslateUi(QDialog *HistoDialog)
    {
        HistoDialog->setWindowTitle(QApplication::translate("HistoDialog", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class HistoDialog: public Ui_HistoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTODIALOG_H
