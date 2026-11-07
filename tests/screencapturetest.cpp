#include <actiontools/screenshotwizard.hpp>
#include <actiontools/screenshotwizardpage.hpp>
#include <actiontools/targetwindow.hpp>

#include <QPushButton>
#include <QSignalSpy>
#include <QTest>
#include <QTimer>
#include <QWindow>

class ScreenCaptureTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
#ifdef Q_OS_UNIX
        if(QGuiApplication::platformName() != QStringLiteral("xcb"))
            QSKIP("The screenshot wizard requires X11; run with xvfb-run on headless systems.");
#endif
    }

    void selection_data()
    {
        QTest::addColumn<QPoint>("start");
        QTest::addColumn<QPoint>("end");
        QTest::addColumn<bool>("cancel");
        QTest::addColumn<bool>("complete");

        QTest::newRow("escape") << QPoint(100, 100) << QPoint(300, 250) << true << false;
        QTest::newRow("click") << QPoint(100, 100) << QPoint(100, 100) << false << false;
        QTest::newRow("horizontal") << QPoint(100, 100) << QPoint(300, 100) << false << false;
        QTest::newRow("quick-drag") << QPoint(100, 100) << QPoint(300, 250) << false << true;
        QTest::newRow("reverse-quick-drag") << QPoint(300, 250) << QPoint(100, 100) << false << true;
    }

    void selection()
    {
        QFETCH(QPoint, start);
        QFETCH(QPoint, end);
        QFETCH(bool, cancel);
        QFETCH(bool, complete);

        QWidget mainWindow;
        mainWindow.show();
        ActionTools::ScreenshotWizard wizard(nullptr, true, &mainWindow);
        wizard.show();
        QVERIFY(QTest::qWaitForWindowExposed(&wizard));

        auto page = wizard.findChild<ActionTools::ScreenshotWizardPage *>();
        QVERIFY(page);
        auto captureButton = page->findChild<QPushButton *>(QStringLiteral("captureScreenPartPushButton"));
        QVERIFY(captureButton);

        // Repeat to check that cancellation and capture leave the wizard usable.
        for(int attempt = 0; attempt < 2; ++attempt)
        {
            captureButton->click();
            ActionTools::TargetWindow *target = nullptr;
            for(auto widget : QApplication::topLevelWidgets())
            {
                if(auto candidate = qobject_cast<ActionTools::TargetWindow *>(widget))
                {
                    target = candidate;
                    break;
                }
            }
            QVERIFY(target);
            QSignalSpy selected(target, &ActionTools::TargetWindow::rectangleSelected);
            if(cancel)
                QTest::keyClick(target, Qt::Key_Escape);
            else
            {
                // Deliver release before the geometry update timer runs.
                QTest::mousePress(target, Qt::LeftButton, Qt::NoModifier, target->mapFromGlobal(start));
                QTest::mouseRelease(target, Qt::LeftButton, Qt::NoModifier, target->mapFromGlobal(end));
            }
            QCOMPARE(selected.count(), 1);
            QRect expected(QPoint(qMin(start.x(), end.x()), qMin(start.y(), end.y())),
                           QSize(qAbs(end.x() - start.x()), qAbs(end.y() - start.y())));
#ifdef Q_OS_UNIX
            expected = expected.marginsRemoved(QMargins(2, 2, 2, 2));
#endif
            if(cancel || expected.isEmpty())
                expected = {};
            QCOMPARE(selected.at(0).at(0).toRect(), expected);

            QTRY_VERIFY(mainWindow.windowHandle()->isExposed());
            QTRY_VERIFY(wizard.windowHandle()->isExposed());
            QTRY_COMPARE(mainWindow.windowHandle()->opacity(), 1.0);
            QTRY_COMPARE(wizard.windowHandle()->opacity(), 1.0);
            QTRY_COMPARE(wizard.button(QWizard::NextButton)->isEnabled(), complete);
            QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
        }

        QSignalSpy rejected(&wizard, &QDialog::rejected);
        QTest::keyClick(page, Qt::Key_Escape);
        QCOMPARE(rejected.count(), 1);
    }

    void modalCapture_data()
    {
        QTest::addColumn<bool>("wholeScreen");
        QTest::addColumn<bool>("cancel");
        QTest::newRow("area") << false << false;
        QTest::newRow("cancel-area") << false << true;
        QTest::newRow("whole-screen") << true << false;
    }

    void modalCapture()
    {
        QFETCH(bool, wholeScreen);
        QFETCH(bool, cancel);

        QWidget mainWindow;
        mainWindow.show();
        ActionTools::ScreenshotWizard wizard(nullptr, true, &mainWindow);
        auto page = wizard.findChild<ActionTools::ScreenshotWizardPage *>();
        QVERIFY(page);
        auto captureButton = page->findChild<QPushButton *>(wholeScreen
            ? QStringLiteral("captureWholeScreenPushButton")
            : QStringLiteral("captureScreenPartPushButton"));
        QVERIFY(captureButton);
        bool completed = false;
        bool restored = false;

        QTimer::singleShot(0, &wizard, [&](){
            captureButton->click();

            // Return to exec() between steps: a nested qWait() here would keep
            // the wizard alive and mask premature termination when it is hidden.
            QTimer::singleShot(50, &wizard, [&](){
                if(!wholeScreen)
                {
                    for(auto widget : QApplication::topLevelWidgets())
                    {
                        if(auto target = qobject_cast<ActionTools::TargetWindow *>(widget))
                        {
                            if(cancel)
                                QTest::keyClick(target, Qt::Key_Escape);
                            else
                            {
                                QTest::mousePress(target, Qt::LeftButton, Qt::NoModifier,
                                                  target->mapFromGlobal(QPoint(100, 100)));
                                QTest::mouseRelease(target, Qt::LeftButton, Qt::NoModifier,
                                                    target->mapFromGlobal(QPoint(300, 250)));
                            }
                            break;
                        }
                    }
                }

                QTimer::singleShot(350, &wizard, [&](){
                    completed = wizard.button(QWizard::NextButton)->isEnabled() == !cancel;
                    restored = mainWindow.windowHandle()->isExposed()
                        && wizard.windowHandle()->isExposed()
                        && mainWindow.windowHandle()->opacity() == 1.0
                        && wizard.windowHandle()->opacity() == 1.0;
                    wizard.reject();
                });
            });
        });

        wizard.exec();
        // Clean up an orphaned selector if exec() returned too early.
        for(auto widget : QApplication::topLevelWidgets())
            if(auto target = qobject_cast<ActionTools::TargetWindow *>(widget))
                delete target;
        QVERIFY2(completed, "The modal wizard returned before capture completed");
        QVERIFY(restored);
    }
};

QTEST_MAIN(ScreenCaptureTest)
#include "screencapturetest.moc"
