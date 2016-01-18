/********************************************************************************
** Form generated from reading UI file 'GeneralSettings.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERALSETTINGS_H
#define UI_GENERALSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GeneralSettings
{
public:
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBoxSvp;
    QCheckBox *checkBoxSfp;
    QCheckBox *checkBoxSup;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GeneralSettings)
    {
        if (GeneralSettings->objectName().isEmpty())
            GeneralSettings->setObjectName(QStringLiteral("GeneralSettings"));
        GeneralSettings->resize(215, 206);
        verticalLayout = new QVBoxLayout(GeneralSettings);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        checkBoxSvp = new QCheckBox(GeneralSettings);
        checkBoxSvp->setObjectName(QStringLiteral("checkBoxSvp"));

        verticalLayout->addWidget(checkBoxSvp);

        checkBoxSfp = new QCheckBox(GeneralSettings);
        checkBoxSfp->setObjectName(QStringLiteral("checkBoxSfp"));

        verticalLayout->addWidget(checkBoxSfp);

        checkBoxSup = new QCheckBox(GeneralSettings);
        checkBoxSup->setObjectName(QStringLiteral("checkBoxSup"));

        verticalLayout->addWidget(checkBoxSup);

        buttonBox = new QDialogButtonBox(GeneralSettings);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(GeneralSettings);
        QObject::connect(buttonBox, SIGNAL(accepted()), GeneralSettings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), GeneralSettings, SLOT(reject()));

        QMetaObject::connectSlotsByName(GeneralSettings);
    } // setupUi

    void retranslateUi(QDialog *GeneralSettings)
    {
        GeneralSettings->setWindowTitle(QApplication::translate("GeneralSettings", "Dialog", 0));
        checkBoxSvp->setText(QApplication::translate("GeneralSettings", "Start webcam paused.", 0));
        checkBoxSfp->setText(QApplication::translate("GeneralSettings", "Start video file paused.", 0));
        checkBoxSup->setText(QApplication::translate("GeneralSettings", "Start URL video paused.", 0));
    } // retranslateUi

};

namespace Ui {
    class GeneralSettings: public Ui_GeneralSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENERALSETTINGS_H
