/*
 * main.cpp
 */

#include <QtGui/QApplication>
#include "src/windows/mainwindow.hpp"

int main( int iArgc, char** iArgv ) {
	QApplication oApplication(iArgc, iArgv);
	MainWindow oWindow;
	oWindow.show();
	
	return oApplication.exec();
}
