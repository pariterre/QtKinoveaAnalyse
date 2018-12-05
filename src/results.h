#ifndef RESULTS_H
#define RESULTS_H

#include <limits>
#include <memory>
#include <QDialog>

class QStackedWidget;
class Frame;

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
    void showEvent(QShowEvent* event = nullptr);
private slots:
    void on_NextWidget_clicked();
    void on_PreviousWidget_clicked();

    void on_PreviousWidgetFast_clicked();

    void on_NextWidgetFast_clicked();

private:
    enum Type{
        POINT_X,
        POINT_Y,
        GRF_X,
        GRF_Y
    };

    Ui::Results *ui;
    int _margin;
    double _aspectRatio;
    void resizeWidgetSubplot(QWidget*, int row, int numberRow, int col, int numberCol, double aspectRatio);
    void fillTimeFrameGraph(QStackedWidget*, const std::string &title, const std::vector<Frame>&frames, Type type);

    const MainWindow & GetMainWindow();
};
#include <mainwindow.h>

#endif // RESULTS_H
