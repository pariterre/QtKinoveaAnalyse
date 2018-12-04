#ifndef RESULTS_H
#define RESULTS_H

#include <limits>

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>

#include "Frame.h"

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

    void prepareWidgets();
    void resizeEvent(QResizeEvent* event = nullptr);
private slots:
    void on_NextWidget_clicked();

private:
    Ui::Results *ui;
    int _margin;
    double _aspectRatio;

    const MainWindow & GetMainWindow();
};
#include "mainwindow.h"

#endif // RESULTS_H
