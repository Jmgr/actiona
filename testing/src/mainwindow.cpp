#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "backend/backend.hpp"
#include "backend/windowing.hpp"
#include "backend/windowhandle.hpp"
#include "actiontools/windowhandle.hpp"

#include <QKeyEvent>
#include <QDBusInterface>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->choosePositionPushButton, &ActionTools::ChoosePositionPushButton::positionChosen, this, [this](const QPoint &position){
        ui->choosePositionLabel->setText(QStringLiteral("%1:%2").arg(position.x()).arg(position.y()));
    });
    connect(ui->choosePositionPushButton, &ActionTools::ChoosePositionPushButton::errorOccurred, this, [this](const QString &error){
        ui->choosePositionLabel->setText(error);
    });
    connect(ui->chooseWindowPushButton, &ActionTools::ChooseWindowPushButton::windowChosen, this, [this](const ActionTools::WindowHandle &window){
        try
        {
            ui->chooseWindowLabel->setText(window.title());
        }
        catch(const Backend::BackendError &e)
        {
            ui->chooseWindowLabel->setText(e.what());
        }
    });
    connect(ui->chooseWindowPushButton, &ActionTools::ChooseWindowPushButton::errorOccurred, this, [this](const QString &error){
        ui->chooseWindowLabel->setText(error);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape && mDisableEscape)
        return;

    QMainWindow::keyPressEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit closed();

    QMainWindow::closeEvent(event);
}

void MainWindow::on_chooseRectangleGnomePushButton_clicked()
{
    auto connectionInterface = new QDBusInterface(QStringLiteral("org.gnome.Shell.Screenshot"), QStringLiteral("/org/gnome/Shell/Screenshot"),
                                             QStringLiteral("org.gnome.Shell.Screenshot"),
                                             QDBusConnection::sessionBus(), this);
    if(!connectionInterface->isValid())
    {
        qDebug() << "invalid gnome shell screenshot DBus interface";
        return;
    }

    auto result = connectionInterface->call(QStringLiteral("SelectArea"));
    if(result.type() == QDBusMessage::ErrorMessage)
    {
        qDebug() << "call error" << result.errorMessage();
        return;
    }

    auto args = result.arguments();
    if(args.size() != 4)
    {
        qDebug() << "invalid result arguments count" << args.size();
        return;
    }

    ui->rectangleGnomeLabel->setText(QStringLiteral("Rectangle: (%1:%2)(%3:%4)").arg(args[0].toInt()).arg(args[1].toInt()).arg(args[2].toInt()).arg(args[3].toInt()));
}

void MainWindow::on_chooseAreaPushButton_clicked()
{
    auto areaChooser = Backend::Instance::windowing().createAreaChooser(this);
    connect(areaChooser, &Backend::AreaChooser::done, this, [this, areaChooser](const QRect &area){
        ui->chooseAreaLabel->setText(QStringLiteral("%1:%2 %3:%4").arg(area.x()).arg(area.y()).arg(area.width()).arg(area.height()));
        areaChooser->deleteLater();
    });
    connect(areaChooser, &Backend::AreaChooser::canceled, this, [this, areaChooser]{
        ui->chooseAreaLabel->setText(QStringLiteral("canceled"));
        areaChooser->deleteLater();
    });
    connect(areaChooser, &Backend::AreaChooser::errorOccurred, this, [this, areaChooser](const Backend::BackendError &e){
        ui->chooseAreaLabel->setText(QStringLiteral("error: %1").arg(e.what()));
        areaChooser->deleteLater();
    });
    areaChooser->choose();
}

void MainWindow::on_chooseAreaIntegratedPushButton_clicked()
{

}

