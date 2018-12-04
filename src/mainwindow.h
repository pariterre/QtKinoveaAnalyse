#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QFileDialog"

#include "results.h"
#include "ProportionalModel.h"
#include "KinoveaReader.h"
#include "KinoMath.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QFileInfo GetKinoFile() const;
    QFileInfo GetConfigFile() const;

    const ProportionalModel& GetModel() const;
    const KinoveaReader& GetKinovea() const;

    const std::vector<Frame>& GetComi() const;
    const std::vector<Frame>& GetCom() const;
    const std::vector<Frame>& GetComVelocity() const;
    const std::vector<Frame>& GetComAcceleration() const;
    const std::vector<Frame>& GetGrf() const;

    const std::vector<Frame>& GetJointAngle() const;
    const std::vector<Frame>& GetJointVelocity() const;
    const std::vector<Frame>& GetJointAcceleration() const;

private slots:
    void on_exportedPathEdit_textChanged(const QString &arg1);
    void on_exportedConfigEdit_textChanged(const QString &arg1);
    void on_exportedPathButton_clicked();
    void on_exportedConfigButton_clicked();

    void on_computeButton_clicked();

private:
    Ui::MainWindow *ui;
    Results * results;
    KinoveaReader _kinovea;
    ProportionalModel _model;

    std::vector<Frame> _comi;
    std::vector<Frame> _com;
    std::vector<Frame> _comVelocity;
    std::vector<Frame> _comAcceleration;
    std::vector<Frame> _grf;

    std::vector<Frame> _jointAngle;
    std::vector<Frame> _jointVelocity;
    std::vector<Frame> _jointAcceleration;

    void isPathsReady();
};

#endif // MAINWINDOW_H
