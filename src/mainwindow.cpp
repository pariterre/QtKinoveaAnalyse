#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    results(new Results(this))
{
    ui->setupUi(this);
    results->setWindowModality(Qt::ApplicationModal);
    isPathsReady();
    on_computeButton_clicked();
}

MainWindow::~MainWindow()
{
    delete results;
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
    return _model;
}

const KinoveaReader &MainWindow::GetKinovea() const
{
    return _kinovea;
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
    if (kinoFile.exists() && kinoFile.isFile() && configFile.exists() && configFile.isFile())
        ui->computeButton->setEnabled(true);
    else
        ui->computeButton->setEnabled(false);
}


void MainWindow::on_computeButton_clicked()
{
    _model.readXml(GetConfigFile().absoluteFilePath().toStdString());
    _kinovea.readXml(GetKinoFile().absoluteFilePath().toStdString(), _model);
    _comi = KinoMath::computeCoMi(_model, _kinovea);
    _com =  KinoMath::computeCoM(_model, _kinovea, _comi);
    _comVelocity = KinoMath::computeDerivative(_com);
    _comAcceleration = KinoMath::computeDerivative(_comVelocity);
    _grf = KinoMath::computeGrf(_comAcceleration, 70);

    _jointAngle = KinoMath::computeJointAngles(_model, _kinovea);
    _jointVelocity = KinoMath::computeDerivative(_jointAngle);
    _jointAcceleration = KinoMath::computeDerivative(_jointVelocity);

    results->prepareWidgets();
    results->show();
}
