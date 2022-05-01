/*
    Actiona
    Copyright (C) 2005 Jonathan Mercier-Ganady

    Actiona is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

    Actiona is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact: jmgr@jmgr.info
*/

#include "backend/backend.hpp"
#include "mainwindow.hpp"

#include <QApplication>
#include <QDialog>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    // Initialize the backend
    Backend::Instance::initialize();

    MainWindow mainWindow;
    mainWindow.show();

    QDialog dummyWindow;
    dummyWindow.show();
    auto pos = mainWindow.pos();
    auto size = mainWindow.size();
    pos.setX(pos.x() + size.width());
    pos.setY(pos.y() + size.height());
    dummyWindow.move(pos);

    QObject::connect(&mainWindow, &MainWindow::closed, &dummyWindow, &QDialog::close);

	return app.exec();
}
