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

signals:
    void closed();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_chooseRectangleGnomePushButton_clicked();
    void on_chooseAreaPushButton_clicked();
    void on_chooseAreaIntegratedPushButton_clicked();

private:
    Ui::MainWindow *ui;
    bool mDisableEscape{false};
};

