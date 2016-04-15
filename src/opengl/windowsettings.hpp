/*
 *
 */

#ifndef WINDOWSETTINGS_HPP
#define WINDOWSETTINGS_HPP

#include <QVector2D>

class WindowSettings {
public:
						WindowSettings( QVector2D iPosition, QVector2D iSize );
						~WindowSettings( void );
						
	void				setWindowSize( QVector2D iSize );
	void				setWindowPosition( QVector2D iPosition );
	
	bool				checkCollision( QVector2D iPoint );
	
	int					getID( void ) const;
	QVector2D			getWindowSize( void ) const;
	QVector2D			getWindowPosition( void ) const;
private:
	bool				mFocus;
	QVector2D			mWindowPosition;
	QVector2D			mWindowSize;
	int					mID;
};

#endif // WINDOWSETTINGS_HPP
