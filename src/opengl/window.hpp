/*
 *
 */

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QMouseEvent>
#include <QVector2D>
#include "src/widgets/glframework.hpp"
#include "src/opengl/windowsettings.hpp"
#include "src/opengl/shapes/panel.hpp"
#include "src/opengl/drawables/drawablesource.hpp"

class Window : public QWidget {
	Q_OBJECT
public:
							Window( 
								QVector2D iPosition, 
								QVector2D iSize,
								QString	iDataSource,
								QString iBackground,
								QString iBorder,
								QWidget* iParent = NULL );
							~Window( void );
	bool					clicked( QMouseEvent* iEvent, bool iSelected );
	void					reset( void );
	const WindowSettings&	settings( void );
	bool					checkCollision( QVector2D iPoint );
	void					draw( void );
	void					saveWindow( QXmlStreamWriter& iWriter );
	
	char*					getDataSource( void );
	void					setDataSource( char* iDatasource );
signals:
	void					signal_closeWindow( Window* );
	void					signal_subscription_attempted( QString );
private:
	void					resizeWindow( QVector2D oResize );
private:
	enum Resize {
		RESIZE_NONE,
		RESIZE_X,
		RESIZE_Y,
		RESIZE_XY
	};
	static QVector2D		sResizeArea;
	
	QAction*				mActionNewSource;
	QAction*				mActionRemoveWindow;
	QMenu*					mMenuBackground;
	QMenu*					mMenuBorder;
	
	Panel*					mPanel;
	Square*					mSquare;
	WindowSettings*			mSettings;
	DrawableSource*			mDataSource;
	QVector2D				mMouseLastPosition;
	bool					mSelected;
	bool					mResize;
	Resize					mResizeStatus;
	QVector2D				mMinimumSize;
};

#endif // WINDOW_HPP
