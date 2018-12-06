#include <results.h>
#include <ui_results.h>

#include <QStackedWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>

#include <KinoveaReader.h>
#include <ProportionalModel.h>
#include <Frame.h>
#include <Landmark.h>
#include <Point2d.h>
#include <Joint.h>
#include <QCallout.h>

Results::Results(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Results),
    _margin(3),
    _aspectRatio(-1),
    _offsetLegends(75, 50)
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
    const std::vector<Frame>& all_comi(GetMainWindow().GetComi());

    // Fill the model
    {
        // Delete previously created stuff
        for (int i = ui->modelPositionWidget->count() - 1; i >= 0; --i)
            ui->modelPositionWidget->removeWidget(ui->modelPositionWidget->widget(i));

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
            ui->modelPositionWidget->addWidget(chartView);

            // Add the segments
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

            // Add the center of mass by segment
            QtCharts::QScatterSeries *comiSerie = new QtCharts::QScatterSeries();
            const Frame& comi(all_comi[f]);
            for (size_t p = 0; p < comi.GetNumberOfPoints2d(); ++p){
                const Point2d& pt(comi.GetPoint2d(p));
                comiSerie->append(pt.GetX(), pt.GetY());
                if (pt.GetX() < bottomLeft.GetX())
                    bottomLeft.SetX(pt.GetX());
                if (pt.GetY() < bottomLeft.GetY())
                    bottomLeft.SetY(pt.GetY());

                if (pt.GetX() > topRight.GetX())
                    topRight.SetX(pt.GetX());
                if (pt.GetY() >  topRight.GetY())
                    topRight.SetY(pt.GetY());
            }
            comiSerie->setMarkerSize(comiSerie->markerSize()/2);
            chart->addSeries(comiSerie);

            // Add the center of mass trajectory
            const std::vector<Frame>& com(GetMainWindow().GetCom());
            QtCharts::QSplineSeries *comSerie = new QtCharts::QSplineSeries();
            for (Frame frame : com)
                comSerie->append(frame.GetPoint2d(0).GetX(), frame.GetPoint2d(0).GetY());
            chart->addSeries(comSerie);

            // Add the actual center of mass
            QtCharts::QScatterSeries *actualComSerie = new QtCharts::QScatterSeries();
            actualComSerie->append(com[f].GetPoint2d(0).GetX(), com[f].GetPoint2d(0).GetY());
            actualComSerie->setMarkerSize(comiSerie->markerSize() * 1.5);
            chart->addSeries(actualComSerie);

            Callout * tooltip = new Callout(chart, _offsetLegends);
            QPointF comPoint(com[f].GetPoint2d(0).GetX(), com[f].GetPoint2d(0).GetY());
            tooltip->setText(QString("X: %1 \nY: %2 ").arg(comPoint.x()).arg(comPoint.y()));
            tooltip->setAnchor(comPoint);
            tooltip->setZValue(11);
            tooltip->updateGeometry();
            tooltip->show();

            // Make sure the size is okay
            chart->createDefaultAxes();
        }

        // Make sure the ticks are easy to read and constant
        for (int w = 0; w < ui->modelPositionWidget->count(); ++w){
            QtCharts::QChartView* chartView(static_cast<QtCharts::QChartView*>(ui->modelPositionWidget->widget(w)));
            QtCharts::QValueAxis* axisX(static_cast<QtCharts::QValueAxis*>(chartView->chart()->axisX()));
            QtCharts::QValueAxis* axisY(static_cast<QtCharts::QValueAxis*>(chartView->chart()->axisY()));
            axisX->setRange(bottomLeft.GetX() - (topRight.GetX() - bottomLeft.GetX()) * 0.05,
                            topRight.GetX() + (topRight.GetX() - bottomLeft.GetX()) * 0.05);
            axisX->setTickCount(2);
            axisY->setRange(bottomLeft.GetY() - (topRight.GetY() - bottomLeft.GetY()) * 0.05,
                            topRight.GetY() + (topRight.GetY() - bottomLeft.GetY()) * 0.05);
            axisY->setTickCount(5);

            // Compute the aspect ratio
            if (w == 0){
                double Xrange(axisX->max() - axisX->min());
                double Yrange(axisY->max() - axisY->min());
                _aspectRatio = Xrange / Yrange;
            }
        }
    }

    // Fill the dynamics
    fillTimeFrameGraph(ui->comHeightWidget, "CoM height (m)", GetMainWindow().GetCom(), POINT_Y);
    fillTimeFrameGraph(ui->comVelocityWidget, "CoM Velocity (m/s)", GetMainWindow().GetComVelocity(), POINT_Y);
    fillTimeFrameGraph(ui->comAccelerationWidget, "CoM acceleration (m/s²)", GetMainWindow().GetComAcceleration(), POINT_Y);
    fillTimeFrameGraph(ui->grfWidget, "GRF (N)", GetMainWindow().GetGrf(), GRF_Y);

    // Fill the joint angles
    {
        // Delete previously created stuff
        for (int i = ui->jointAngleWidget->count() - 1; i >= 0; --i)
            ui->jointAngleWidget->removeWidget(ui->jointAngleWidget->widget(i));

        double ymin = std::numeric_limits<double>::max();
        double ymax = std::numeric_limits<double>::min();
        for (Frame frame : GetMainWindow().GetJointAngle()){

            // Create the chart
            QtCharts::QChart *chart = new QtCharts::QChart();
            chart->setTitle("Joint angles");
            chart->legend()->detachFromChart();
            chart->legend()->setBackgroundVisible(true);
            chart->legend()->setBrush(QBrush(QColor(128, 128, 128, 128)));
            chart->legend()->setPen(QPen(QColor(192, 192, 192, 192)));

            // Add the chart to the result stackedWidget
            QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            ui->jointAngleWidget->addWidget(chartView);

            // Add the center of mass height
            size_t nJoints(GetMainWindow().GetModel().GetJoints().size());
            for (size_t j = 0; j < nJoints; ++j){
                QtCharts::QSplineSeries *serie = new QtCharts::QSplineSeries();
                serie->setName(GetMainWindow().GetModel().GetJoints()[j].GetName().c_str());
                for (Frame frame : GetMainWindow().GetJointAngle())
                    serie->append(frame.GetTime(), frame.GetJoint(j).GetAngleDegree());
                chart->addSeries(serie);

                if (frame.GetJoint(j).GetAngleDegree() < ymin)
                    ymin = frame.GetJoint(j).GetAngleDegree();
                if (frame.GetJoint(j).GetAngleDegree() > ymax)
                    ymax = frame.GetJoint(j).GetAngleDegree();
            }
        }

        // Add a time bar
        ymin -= (ymax - ymin) * 0.1;
        ymax += (ymax - ymin) * 0.1;
        for (size_t f = 0; f < GetMainWindow().GetJointAngle().size(); ++f)
        {
            QtCharts::QLineSeries *bar = new QtCharts::QLineSeries();
            bar->append(GetMainWindow().GetJointAngle()[f].GetTime(), ymin);
            bar->append(GetMainWindow().GetJointAngle()[f].GetTime(), ymax);

            QtCharts::QChartView *chartView(static_cast<QtCharts::QChartView*>(ui->jointAngleWidget->widget(static_cast<int>(f))));
            chartView->chart()->addSeries(bar);

            // Make sure the size is okay
            chartView->chart()->createDefaultAxes();
            QtCharts::QValueAxis* axisX(static_cast<QtCharts::QValueAxis*>(chartView->chart()->axisX()));
            QtCharts::QValueAxis* axisY(static_cast<QtCharts::QValueAxis*>(chartView->chart()->axisY()));
            axisX->setRange(0, axisX->max());
            axisX->setTickCount(2);
            axisY->setRange(ymin, ymax);
        }
    }
}

void Results::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    std::cout << size().width() << "; " << size().height() << std::endl;

    // Reposition the next button
    QSize previousButtonRectangle(ui->PreviousWidget->size());
    ui->PreviousWidget->setGeometry(size().width()/2 - previousButtonRectangle.width() - _margin,
                                size().height() - previousButtonRectangle.height() - _margin,
                                previousButtonRectangle.width(), previousButtonRectangle.height());

    QSize previousFastButtonRectangle(ui->PreviousWidgetFast->size());
    ui->PreviousWidgetFast->setGeometry(size().width()/2 - previousFastButtonRectangle.width() - ui->PreviousWidget->geometry().width() - 2*_margin ,
                                size().height() - previousFastButtonRectangle.height() - _margin,
                                previousFastButtonRectangle.width(), previousFastButtonRectangle.height());

    QSize nextButtonRectangle(ui->NextWidget->size());
    ui->NextWidget->setGeometry(size().width()/2 + _margin,
                                size().height() - nextButtonRectangle.height() - _margin,
                                nextButtonRectangle.width(), nextButtonRectangle.height());

    QSize nextFastButtonRectangle(ui->NextWidgetFast->size());
    ui->NextWidgetFast->setGeometry(size().width()/2 + ui->NextWidget->geometry().width() + 2*_margin,
                                    size().height() - nextFastButtonRectangle.height() - _margin,
                                    nextFastButtonRectangle.width(), nextFastButtonRectangle.height());


    // Resize the stacked widget
    resizeWidgetSubplot(ui->modelPositionWidget, 0, 1, 0, 4, _aspectRatio);
    double aspectRatioTimeGraph(1.1);
    resizeWidgetSubplot(ui->comHeightWidget, 0, 2, 1, 4, aspectRatioTimeGraph);
    resizeWidgetSubplot(ui->comVelocityWidget, 1, 2, 1, 4, aspectRatioTimeGraph);
    resizeWidgetSubplot(ui->comAccelerationWidget, 0, 2, 2, 4, aspectRatioTimeGraph);
    resizeWidgetSubplot(ui->grfWidget, 1, 2, 2, 4, aspectRatioTimeGraph);
    resizeWidgetSubplot(ui->jointAngleWidget, 0, 1, 3, 4, _aspectRatio);

    // Reposition the legend of angles
    QStackedWidget* w(static_cast<QStackedWidget*>(ui->jointAngleWidget));
    QtCharts::QChartView* cv(static_cast<QtCharts::QChartView*>(w->widget(w->currentIndex())));
    QtCharts::QLegend* legend(cv->chart()->legend());
    legend->setGeometry(_offsetLegends.x(), _offsetLegends.y(), 0, 0);
    legend->adjustSize();
}

void Results::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    resizeEvent();
}

void Results::resizeWidgetSubplot(QWidget * widget, int row, int numberRow, int col, int numberCol, double aspectRatio){
    double rectHeight(ui->NextWidget->geometry().top() * 1.0/static_cast<double>(numberRow));
    double rectWidth(size().width() * 1.0/static_cast<double>(numberCol));
    QRect newSize;
    if (rectHeight * aspectRatio <= rectWidth){
        double finalWidth(rectHeight * aspectRatio);
        newSize = QRect(static_cast<int>(rectWidth/2 - finalWidth/2 + _margin + rectWidth * static_cast<double>(col)),
                        static_cast<int>(_margin + rectHeight * static_cast<double>(row)),
                        static_cast<int>(finalWidth - _margin),
                        static_cast<int>(rectHeight));
    }
    else{
        double finalHeight(rectWidth / aspectRatio);
        newSize = QRect(static_cast<int>(_margin + rectWidth * static_cast<double>(col)),
                        static_cast<int>(rectHeight/2 - finalHeight/2 + _margin + rectHeight * static_cast<double>(row)),
                        static_cast<int>(rectWidth - _margin),
                        static_cast<int>(finalHeight));
    }
    widget->setGeometry(newSize);
}

void Results::fillTimeFrameGraph(QStackedWidget * widget, const std::string& title, const std::vector<Frame> &frames, Type type)
{
    // Delete previously created stuff
    for (int i = widget->count() - 1; i >= 0; --i)
        widget->removeWidget(widget->widget(i));

    for (Frame frame : frames){
        // Create the chart
        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->setTitle(title.c_str());
        chart->legend()->hide();

        // Add the chart to the result stackedWidget
        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        widget->addWidget(chartView);

        QtCharts::QSplineSeries *serie = new QtCharts::QSplineSeries();
        double ymin = std::numeric_limits<double>::max();
        double ymax = std::numeric_limits<double>::min();
        for (Frame frame : frames){
            double yval(0);
            if (type == Type::POINT_X)
                yval = frame.GetPoint2d(0).GetX();
            if (type == Type::POINT_Y)
                yval = frame.GetPoint2d(0).GetY();
            if (type == Type::GRF_X)
                yval = frame.GetGrf().GetX();
            if (type == Type::GRF_Y)
                yval = frame.GetGrf().GetY();
            serie->append(frame.GetTime(), yval);

            if (yval < ymin)
                ymin = yval;
            if (yval > ymax)
                ymax = yval;
        }
        chart->addSeries(serie);

        // Add a time bar
        ymin -= (ymax - ymin) * 0.1;
        ymax += (ymax - ymin) * 0.1;
        QtCharts::QLineSeries *bar = new QtCharts::QLineSeries();
        bar->append(frame.GetTime(), ymin);
        bar->append(frame.GetTime(), ymax);
        chart->addSeries(bar);

        // Make sure the size is okay
        chart->createDefaultAxes();
        QtCharts::QValueAxis* axisX(static_cast<QtCharts::QValueAxis*>(chartView->chart()->axisX()));
        QtCharts::QValueAxis* axisY(static_cast<QtCharts::QValueAxis*>(chartView->chart()->axisY()));
        axisX->setRange(0, axisX->max());
        axisX->setTickCount(2);
        axisY->setRange(ymin, ymax);

   }
}

const MainWindow & Results::GetMainWindow()
{
    return *dynamic_cast<MainWindow*>(this->parent());
}

void Results::on_NextWidget_clicked()
{
    if (ui->modelPositionWidget->count() != 0){
        ui->modelPositionWidget->setCurrentIndex(ui->modelPositionWidget->currentIndex() + 1);
        ui->comHeightWidget->setCurrentIndex(ui->modelPositionWidget->currentIndex());
        ui->comVelocityWidget->setCurrentIndex(ui->modelPositionWidget->currentIndex());
        ui->comAccelerationWidget->setCurrentIndex(ui->modelPositionWidget->currentIndex());
        ui->grfWidget->setCurrentIndex(ui->modelPositionWidget->currentIndex());
        ui->jointAngleWidget->setCurrentIndex(ui->modelPositionWidget->currentIndex());
    }
    resizeEvent();
}

void Results::on_PreviousWidget_clicked()
{
    if (ui->modelPositionWidget->count() != 0){
        ui->modelPositionWidget->setCurrentIndex(ui->modelPositionWidget->currentIndex() - 1);
        ui->comHeightWidget->setCurrentIndex(ui->modelPositionWidget->currentIndex());
        ui->comVelocityWidget->setCurrentIndex(ui->modelPositionWidget->currentIndex());
        ui->comAccelerationWidget->setCurrentIndex(ui->modelPositionWidget->currentIndex());
        ui->grfWidget->setCurrentIndex(ui->modelPositionWidget->currentIndex());
        ui->jointAngleWidget->setCurrentIndex(ui->modelPositionWidget->currentIndex());
    }
    resizeEvent();
}

void Results::on_PreviousWidgetFast_clicked()
{
    for (size_t i = 0; i<5; ++i)
        on_PreviousWidget_clicked();
}

void Results::on_NextWidgetFast_clicked()
{
    for (size_t i = 0; i<5; ++i)
        on_NextWidget_clicked();
}
