/********************************************************************************
** Form generated from reading ui file 'xmms2dprefdialog.ui'
**
** Created: Tue Jul 31 13:44:54 2007
**      by: Qt User Interface Compiler version 4.3.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_XMMS2DPREFDIALOG_H
#define UI_XMMS2DPREFDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>

class Ui_Xmms2dPrefDialog
{
public:
    QVBoxLayout *vboxLayout;
    QTreeView *treeView;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Xmms2dPrefDialog)
    {
    if (Xmms2dPrefDialog->objectName().isEmpty())
        Xmms2dPrefDialog->setObjectName(QString::fromUtf8("Xmms2dPrefDialog"));
    QSize size(600, 497);
    size = size.expandedTo(Xmms2dPrefDialog->minimumSizeHint());
    Xmms2dPrefDialog->resize(size);
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(Xmms2dPrefDialog->sizePolicy().hasHeightForWidth());
    Xmms2dPrefDialog->setSizePolicy(sizePolicy);
    Xmms2dPrefDialog->setWindowIcon(QIcon(QString::fromUtf8(":/images/esperanza.png")));
    vboxLayout = new QVBoxLayout(Xmms2dPrefDialog);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    treeView = new QTreeView(Xmms2dPrefDialog);
    treeView->setObjectName(QString::fromUtf8("treeView"));

    vboxLayout->addWidget(treeView);

    buttonBox = new QDialogButtonBox(Xmms2dPrefDialog);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

    vboxLayout->addWidget(buttonBox);


    retranslateUi(Xmms2dPrefDialog);
    QObject::connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), Xmms2dPrefDialog, SLOT(clickedBtnBox(QAbstractButton*)));

    QMetaObject::connectSlotsByName(Xmms2dPrefDialog);
    } // setupUi

    void retranslateUi(QDialog *Xmms2dPrefDialog)
    {
    Xmms2dPrefDialog->setWindowTitle(QApplication::translate("Xmms2dPrefDialog", "Esperanza - Preferences", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Xmms2dPrefDialog);
    } // retranslateUi

};

namespace Ui {
    class Xmms2dPrefDialog: public Ui_Xmms2dPrefDialog {};
} // namespace Ui

#endif // UI_XMMS2DPREFDIALOG_H
