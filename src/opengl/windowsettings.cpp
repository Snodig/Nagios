/*
 *
 */

#include "src/opengl/windowsettings.hpp"


WindowSettings::WindowSettings( QVector2D iPosition, QVector2D iSize )
	:	mWindowPosition( iPosition ), mWindowSize( iSize ) {
	static int oIdCounter = 0;
	mID = ++oIdCounter;
}

WindowSettings::~WindowSettings( void ) {
	//
}

QVector2D WindowSettings::getWindowPosition( void ) const {
	return mWindowPosition;
}

QVector2D WindowSettings::getWindowSize( void ) const {
	return mWindowSize;
}

void WindowSettings::setWindowPosition( QVector2D iPosition ) {
	mWindowPosition = iPosition;
}

void WindowSettings::setWindowSize( QVector2D iSize ) {
	mWindowSize = iSize;
}

int WindowSettings::getID( void ) const {
	return mID;
}

bool WindowSettings::checkCollision( QVector2D iPoint ) {
	if( iPoint.x() < mWindowPosition.x() ) return false;
	if( iPoint.x() > mWindowPosition.x() + mWindowSize.x() ) return false;
	if( iPoint.y() < mWindowPosition.y() ) return false;
	if( iPoint.y() > mWindowPosition.y() + mWindowSize.y() ) return false;
	
	return true;
}
