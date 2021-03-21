#pragma once

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

namespace Backend
{
    class WindowHandle;
    class TargetWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void positionChooseStarted();
    void positionChosen(QPointF position);
    void foundValidWindow(const Backend::WindowHandle &handle);
    void windowSearchEnded(const Backend::WindowHandle &handle);
    void on_chooseRectanglePushButton_clicked();

    void on_chooseRectangleGnomePushButton_clicked();

    void on_choosePositionNewPushButton_clicked();

    void on_chooseAreaPushButton_clicked();

    void on_chooseWindowNewPushButton_clicked();

private:
    Ui::MainWindow *ui;
    Backend::TargetWindow *mTargetWindow{};
    bool mDisableEscape{false};
};

