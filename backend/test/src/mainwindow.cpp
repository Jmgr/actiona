#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "backend/backend.hpp"
#include "backend/windowing.hpp"
#include "backend/windowhandle.hpp"

#include <QKeyEvent>

#include <QDBusInterface>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

void MainWindow::on_chooseWindowNewPushButton_pressed()
{
    auto windowChooser = Backend::Instance::windowing().createWindowChooser(this);
    connect(windowChooser, &Backend::WindowChooser::done, this, [this, windowChooser](const WId &window){
        try
        {
            ui->chooseWindowNewLabel->setText(Backend::WindowHandle(window).title());
        }
        catch(const Backend::BackendError &e)
        {
            ui->chooseWindowNewLabel->setText(QStringLiteral("%1 (%2)").arg(window).arg(e.what()));
        }

        windowChooser->deleteLater();
    });
    connect(windowChooser, &Backend::WindowChooser::canceled, this, [this, windowChooser]{
        ui->chooseWindowNewLabel->setText(QStringLiteral("canceled"));
        windowChooser->deleteLater();
    });
    connect(windowChooser, &Backend::AreaChooser::errorOccurred, this, [this, windowChooser](const Backend::BackendError &e){
        ui->chooseWindowNewLabel->setText(QStringLiteral("error: %1").arg(e.what()));
        windowChooser->deleteLater();
    });
    windowChooser->choose();
}

void MainWindow::on_choosePositionNewPushButton_pressed()
{
    auto posChooser = Backend::Instance::windowing().createPositionChooser(this);
    connect(posChooser, &Backend::PositionChooser::done, this, [this, posChooser](const QPoint &position){
        ui->choosePositionNewLabel->setText(QStringLiteral("%1:%2").arg(position.x()).arg(position.y()));
        posChooser->deleteLater();
    });
    connect(posChooser, &Backend::PositionChooser::canceled, this, [this, posChooser]{
        ui->choosePositionNewLabel->setText(QStringLiteral("canceled"));
        posChooser->deleteLater();
    });
    connect(posChooser, &Backend::AreaChooser::errorOccurred, this, [this, posChooser](const Backend::BackendError &e){
        ui->choosePositionNewLabel->setText(QStringLiteral("error: %1").arg(e.what()));
        posChooser->deleteLater();
    });
    posChooser->choose();
}
