#include "histodialog.h"
#include "ui_histodialog.h"

HistoDialog::HistoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoDialog)
{
    ui->setupUi(this);
}

HistoDialog::~HistoDialog()
{
    delete ui;
}
