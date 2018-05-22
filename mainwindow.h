#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_exportedPathEdit_textChanged(const QString &arg1);

    void on_exportedPathButton_clicked();

    void on_exportedConfigButton_clicked();

    void on_exportedConfigEdit_textChanged(const QString &arg1);
private:
    Ui::MainWindow *ui;

    void checkForPathsReady();
};

#endif // MAINWINDOW_H
