/*
 *
 */

#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <QMouseEvent>
#include <QTextStream>
#include <QVector2D>
#include <QtOpenGL/QGLFramebufferObject>
#include "src/opengl/window.hpp"
#include "src/network/networker.hpp"
#include "src/opengl/shapes/square.hpp"

class Layout : public QWidget {
	Q_OBJECT
public:
						Layout( Networker* iNetworker, QWidget* iParent = NULL );
						~Layout( void );
	//
	void				mouseUpdate( QMouseEvent* iEvent, QEvent::Type iType );
	void				draw( void );
	void				addWindow( QVector2D iPosition, QVector2D iSize, QString iTexBackground, QString iTexBorder, QString iDataSource = QString( "DEFAULT" ) );
	GLuint				getFBOTexture( void ) const;
	QVector2D			getFBOSize( void ) const;
	void				resize( QVector2D iSize );
	void				saveLayout( QXmlStreamWriter& iWriter );
private slots:
	void				slot_newWindow( void );
	void				slot_removeWindow( Window* iWindow );
private:
	//
	QList< QPair< Window*, bool > > mWindows;
	QAction*			mActionNewWindow;
	QAction*			mActionExit;
	QGLFramebufferObject* mFrameBuffer;
	Networker*			mNetworker;
	/*
	 * mouse position from last frame,
	 * need to store this to be able to 
	 * accurately hit windows. If we use
	 * current frame mouse position then
	 * windows will be deselected if mouse
	 * moves too fast.
	 */
	QVector2D			mPrevPosition;
	static QVector2D	sMouseReleased;
	/* 
	 * Temporary values for new window
	 * when using the QMenu to add window
	 */
	QVector2D			mNewWindowPosition;
	QVector2D			mNewWindowSize;
};

/*
 * pointer to the layouts framebuffer object, we
 * need to unbind this every time another framebuffer
 * draws, for now this is DrawableDatasource which is a
 * part of the Window class.
 * 
 * Is set at the start of the draw method, so does not
 * prevent many layouts. But you can not multithread the
 * draw function.
 */
extern QGLFramebufferObject* gLayoutBuffer;

#endif // LAYOUT_HPP
