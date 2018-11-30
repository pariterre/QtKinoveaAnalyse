#ifndef RESULTS_H
#define RESULTS_H

#include <QDialog>

namespace Ui {
class Results;
}
class MainWindow;

class Results : public QDialog
{
    Q_OBJECT

public:
    explicit Results(QWidget *parent = nullptr);
    ~Results();

private slots:
    void on_NextWidget_clicked();

private:
    Ui::Results *ui;
    const MainWindow & GetMainWindow();
};
#include "mainwindow.h"

#endif // RESULTS_H
