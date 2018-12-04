#include "results.h"
#include "ui_results.h"

Results::Results(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Results),
    _margin(10),
    _aspectRatio(-1)
{
    ui->setupUi(this);
}

Results::~Results()
{
    delete ui;
}

void Results::prepareWidgets()
{
    // Aliases
    const std::vector<Frame>& kino(GetMainWindow().GetKinovea().GetFrames());
    const std::vector<Landmark>& stickLinks(GetMainWindow().GetModel().GetStickLink());

    Point2d bottomLeft(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    Point2d topRight(std::numeric_limits<double>::min(), std::numeric_limits<double>::min());
    for (size_t f = 0; f < kino.size(); ++f){
        std::string title("Model at frame : ");
        title += std::to_string(f);

        // Create the chart
        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->setTitle(title.c_str());
        chart->legend()->hide();

        // Add the chart to the result stackedWidget
        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        ui->stackedWidget->addWidget(chartView);

        // Add the model
        QtCharts::QLineSeries *kinoSerie = new QtCharts::QLineSeries();
        for (Landmark landmark : stickLinks){
            const Point2d& pt(kino[f].GetPoint2d(landmark.GetName()));
            kinoSerie->append(pt.GetX(), pt.GetY());
            if (pt.GetX() < bottomLeft.GetX())
                bottomLeft.SetX(pt.GetX());
            if (pt.GetY() < bottomLeft.GetY())
                bottomLeft.SetY(pt.GetY());

            if (pt.GetX() > topRight.GetX())
                topRight.SetX(pt.GetX());
            if (pt.GetY() >  topRight.GetY())
                topRight.SetY(pt.GetY());
        }
        chart->addSeries(kinoSerie);

        // Add the center of mass trajectory
        const std::vector<Frame>& com(GetMainWindow().GetCom());
        QtCharts::QSplineSeries *comSerie = new QtCharts::QSplineSeries();
        for (Frame frame : com)
            comSerie->append(frame.GetPoint2d(0).GetX(), frame.GetPoint2d(0).GetY());
        chart->addSeries(comSerie);

        // Make sure the size is okay
        chart->createDefaultAxes();
    }

    // Make sure the ticks are easy to read and constant
    for (int w = 0; w < ui->stackedWidget->count(); ++w){
        QtCharts::QChartView* chartView(static_cast<QtCharts::QChartView*>(ui->stackedWidget->widget(w)));
        QtCharts::QValueAxis* axisX(static_cast<QtCharts::QValueAxis*>(chartView->chart()->axisX()));
        QtCharts::QValueAxis* axisY(static_cast<QtCharts::QValueAxis*>(chartView->chart()->axisY()));
        axisX->applyNiceNumbers();
        axisX->setRange(static_cast<int>(bottomLeft.GetX() - (topRight.GetX() - bottomLeft.GetX()) * 0.05),
                        static_cast<int>(topRight.GetX() + (topRight.GetX() - bottomLeft.GetX()) * 0.05));
        axisX->setTickCount(2);
        axisY->setRange(static_cast<int>(bottomLeft.GetY() - (topRight.GetY() - bottomLeft.GetY()) * 0.05),
                        static_cast<int>(topRight.GetY() + (topRight.GetY() - bottomLeft.GetY()) * 0.05));
        axisY->setTickCount(5);

        // Compute the aspect ratio
        if (w == 0){
            double Xrange(axisX->max() - axisX->min());
            double Yrange(axisY->max() - axisY->min());
            _aspectRatio = Xrange / Yrange;
        }
    }



    resizeEvent();
}

void Results::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);

    // Reposition the next button
    QSize buttonRect(ui->NextWidget->size());
    ui->NextWidget->setGeometry(size().width() - buttonRect.width() - _margin,
                                size().height() - buttonRect.height() - _margin,
                                buttonRect.width(), buttonRect.height());

    // Resize the stacked widget
    double rectHeight(ui->NextWidget->geometry().top() - _margin);
    double rectWidth(size().width() - _margin);
    QRect stackedSize;
    if (rectHeight * _aspectRatio <= rectWidth){
        double finalWidth(rectHeight * _aspectRatio);
        stackedSize = QRect(static_cast<int>(rectWidth/2 - finalWidth/2 + _margin),
                            _margin,
                            static_cast<int>(finalWidth - _margin),
                            static_cast<int>(rectHeight));
    }
    else{
        double finalHeight(rectWidth / _aspectRatio);
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

