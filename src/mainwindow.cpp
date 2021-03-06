#include <mainwindow.h>
#include <ui_mainwindow.h>

#include <Joint.h>
#include <Frame.h>
#include <Point2d.h>
#include <Landmark.h>
#include <Segment.h>
#include <ProportionalModel.h>
#include <KinoveaReader.h>
#include <KinoMath.h>
#include <results.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _results(new Results(this)),
    _kinovea(new KinoveaReader()),
    _model(new ProportionalModel())
{
    ui->setupUi(this);
    isPathsReady();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QFileInfo MainWindow::GetKinoFile() const
{
    return ui->exportedPathEdit->text();
}

QFileInfo MainWindow::GetConfigFile() const
{
    return ui->exportedConfigEdit->text();
}

const ProportionalModel &MainWindow::GetModel() const
{
    return *_model;
}

const KinoveaReader &MainWindow::GetKinovea() const
{
    return *_kinovea;
}

const std::vector<Frame> &MainWindow::GetComi() const
{
    return _comi;
}

const std::vector<Frame> &MainWindow::GetCom() const
{
    return _com;
}

const std::vector<Frame> &MainWindow::GetComVelocity() const
{
    return _comVelocity;
}

const std::vector<Frame> &MainWindow::GetComAcceleration() const
{
    return _comAcceleration;
}

const std::vector<Frame> &MainWindow::GetGrf() const
{
    return _grf;
}

const std::vector<Frame> &MainWindow::GetJointAngle() const
{
    return _jointAngle;
}

const std::vector<Frame> &MainWindow::GetJointVelocity() const
{
    return _jointVelocity;
}

const std::vector<Frame> &MainWindow::GetJointAcceleration() const
{
    return _jointAcceleration;
}

void MainWindow::on_exportedPathButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Fichier exporté  de Kinovea"), "",
            tr("Fichier XML (*.xml);;All Files (*)"));
    ui->exportedPathEdit->setText(fileName);
}
void MainWindow::on_exportedPathEdit_textChanged(const QString &arg1)
{
    ui->exportedPathEdit->setText(arg1);
    isPathsReady();
}

void MainWindow::on_exportedConfigButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Fichier de configuration"), "",
            tr("Configuration (*.xml);;All Files (*)"));
    ui->exportedConfigEdit->setText(fileName);
}
void MainWindow::on_exportedConfigEdit_textChanged(const QString &arg1)
{
    ui->exportedConfigEdit->setText(arg1);
    isPathsReady();
}

void MainWindow::isPathsReady()
{
    QFileInfo kinoFile(GetKinoFile());
    QFileInfo configFile(GetConfigFile());
    double mass(getMassValue());
    if (kinoFile.exists() && kinoFile.isFile() && configFile.exists() && configFile.isFile() && mass > 0)
        ui->computeButton->setEnabled(true);
    else
        ui->computeButton->setEnabled(false);
}


void MainWindow::on_computeButton_clicked()
{
    _model->readXml(GetConfigFile().absoluteFilePath().toStdString());
    _kinovea->readXml(GetKinoFile().absoluteFilePath().toStdString(), *_model);
    _comi = KinoMath::computeCoMi(*_model, *_kinovea);
    _com =  KinoMath::computeCoM(*_model, *_kinovea, _comi);
    _comVelocity = KinoMath::computeDerivative(_com);
    _comAcceleration = KinoMath::computeDerivative(_comVelocity);
    _grf = KinoMath::computeGrf(_comAcceleration, getMassValue());

    _jointAngle = KinoMath::computeJointAngles(*_model, *_kinovea);
    _jointVelocity = KinoMath::computeDerivative(_jointAngle);
    _jointAcceleration = KinoMath::computeDerivative(_jointVelocity);

    _results->setModal(true);
    _results->prepareWidgets();
    _results->show();
}

void MainWindow::on_subjectMassEdit_editingFinished()
{
    double mass(getMassValue());
    if (mass < 0)
        ui->subjectMassEdit->setText("");
    else
        ui->subjectMassEdit->setText(KinoMath::to_string_with_precision(mass, 1).c_str());
    isPathsReady();
}

double MainWindow::getMassValue()
{
    double mass(-1.0);
    try {
        mass = KinoMath::stod(ui->subjectMassEdit->text().toStdString());
    } catch (std::invalid_argument) {
        mass = -1.0;
    }
    return mass;
}

