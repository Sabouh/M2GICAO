#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool open(QString url);
    bool openFilename();

private:
    Ui::MainWindow *ui;
    QImage* img;
    QString filename;
};

#endif // MAINWINDOW_H
