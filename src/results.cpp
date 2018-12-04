#include "results.h"
#include "ui_results.h"

Results::Results(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Results),
    _margin(10)
{
    ui->setupUi(this);
}

Results::~Results()
{
    delete ui;
}

void Results::prepareWidgets()
{
    // Add the center of mass trajectory
    const std::vector<Frame>& com(GetMainWindow().GetCom());
    QtCharts::QSplineSeries *comSerie = new QtCharts::QSplineSeries();
    for (Frame frame : com)
        comSerie->append(frame.GetPoint2d(0).GetX(), frame.GetPoint2d(0).GetY());

    // Populate the Chart
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->legend()->hide();
    chart->addSeries(comSerie);
    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");

    // Add the chart to the result window
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->stackedWidget->addWidget(chartView);

    // Make sure the size is okay
    resizeEvent();
}

void Results::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);

    // Make sure the ticks are easy to read
    QtCharts::QChartView* chartView(static_cast<QtCharts::QChartView*>(ui->stackedWidget->widget(0)));
    QtCharts::QValueAxis* axisX(static_cast<QtCharts::QValueAxis*>(chartView->chart()->axisX()));
    QtCharts::QValueAxis* axisY(static_cast<QtCharts::QValueAxis*>(chartView->chart()->axisY()));
    axisX->applyNiceNumbers();
    axisY->applyNiceNumbers();

    // Reposition the next button
    QSize buttonRect(ui->NextWidget->size());
    ui->NextWidget->setGeometry(size().width() - buttonRect.width() - _margin,
                                size().height() - buttonRect.height() - _margin,
                                buttonRect.width(), buttonRect.height());

    // Keep the aspect ratio of the graph
    double Xrange(axisX->max() - axisX->min());
    double Yrange(axisY->max() - axisY->min());
    double aspectRatio(Yrange / Xrange);

    // Resize the stacked widget
    double rectHeight(ui->NextWidget->geometry().top() - _margin);
    double rectWidth(size().width() - _margin);
    QRect stackedSize;
    if (rectHeight * aspectRatio <= rectWidth){
        double finalWidth(rectHeight * aspectRatio);
        stackedSize = QRect(static_cast<int>(rectWidth/2 - finalWidth/2 + _margin),
                            _margin,
                            static_cast<int>(finalWidth - _margin),
                            static_cast<int>(rectHeight));
    }
    else{
        double finalHeight(rectWidth / aspectRatio);
        stackedSize = QRect(_margin,
                            static_cast<int>(rectHeight/2 - finalHeight/2 + _margin),
                            static_cast<int>(rectWidth - _margin),
                            static_cast<int>(finalHeight));
    }
    ui->stackedWidget->setGeometry(stackedSize);
}

const MainWindow & Results::GetMainWindow()
{
    return *dynamic_cast<MainWindow*>(this->parent());
}



void Results::on_NextWidget_clicked()
{
    if (ui->stackedWidget->count() != 0)
        ui->stackedWidget->setCurrentIndex((ui->stackedWidget->currentIndex() + 1) % ui->stackedWidget->count());
}

