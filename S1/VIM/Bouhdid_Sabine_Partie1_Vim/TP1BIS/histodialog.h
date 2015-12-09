#ifndef HISTODIALOG_H
#define HISTODIALOG_H

#include <QDialog>

namespace Ui {
class HistoDialog;
}

class HistoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistoDialog(QWidget *parent = 0);
    ~HistoDialog();
    int* histR;
    int* histG;
    int* histB;

private:
    Ui::HistoDialog *ui;
};

#endif // HISTODIALOG_H
