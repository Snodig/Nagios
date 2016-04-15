/*
 * glframework.hpp
 */

#ifndef GLFRAMEWORK_HPP
#define GLFRAMEWORK_HPP

#include <QGLWidget>
#include <QVector4D>
#include <QStateMachine>
#include <QState>
#include "src/opengl/shapes/square.hpp"
#include "src/network/networker.hpp"
#include "src/backgrounds/background.hpp"
#include "src/opengl/glmessage.hpp"
#include "src/opengl/shapes/format.hpp"

class Layout;
class QDomDocument;
class QDomNode;


class GLFramework : public QGLWidget {
	Q_OBJECT
public:
						GLFramework( QWidget* iParent, QGLFormat iFormat, Networker* iNetworker );
						~GLFramework( void );
	
	/*
	 * functions used by the mainwindow's menu, and
	 * key presses.
	 */
	void				switchLayout( void );
	void				toggleBackground( bool iStatus );
	void				setBackgroundColor( QVector3D iColor );
	bool				saveLayouts( void );
	
	static QList< QAction* >	getDrawableSources( void );
	static void					addDrawableSource( QAction* iAction );
	
public slots:
	void				slotToggleReelShow( void );
	void				slotAddLayout( void );
	void				slotRemoveLayout( void );
	void				slotShowBackground( bool iShowBackground );
	void				slotSetReelSpeed( float iSpeed );
	void				slotSetReelPauseTime( float iMS );
	bool				slotSetupProfile( QString iProfile );
	void				slotSetupLayouts( QString iXml );
	void				slotStatusMessage( QString iMessage, QString iSender );
	void				slotStatusMessage( QString iMessage, GLMessage::MessageLevel iLevel );
	
signals:
	void				signalEmitStatusMessage( QString iMessage, GLMessage::MessageLevel iLevel );
	
protected:
	void				mouseMoveEvent( QMouseEvent* iEvent );
	void				mousePressEvent( QMouseEvent* iEvent );
	void				mouseReleaseEvent( QMouseEvent* iEvent );
	
	void				initializeGL( void );
	void				resizeGL( int iWidth, int iHeight );
	void				paintGL( void );
	void				paintStatic( void );
	void				paintReel( void );
private:
	Layout*				getNextLayout( void );
	void				registerSources( void );
	
	//
	QDomDocument*		mDocProfile;
	Networker*			mNetworker;
	bool				mShowBackground;
	bool				mShowMessages;
	QVector3D			mBackgroundColor;
	QTimer*				mFrameTimer;
	
	/*
	 * 
	 */
	Square*				mSquare;
	GLMessageHandler*	mMessageHandler;
	QList< Layout* >	mLayouts;
	Layout*				mCurrentLayout;
	Background*			mBackground;
	
	/*
	 * Camera variables
	 */
	QMatrix4x4			mProjection;
	QMatrix4x4			mView;
	
	/*
	 * Layout switching variables
	 * - TraversalDelta decides the speed of the
	 *	 show reel. ( goes from [ 1 - 100 ] )
	 *   higher means faster.
	 *
	 * - PauseDuration sets the pause duration
	 *   in form of milliseconds.
	 *   ( goes from [ 0 - 3000000 ] )
	 */
	bool				mReelShow;
	float				mReelTraversal;
	float				mReelTraversalDelta;
	float				mReelTraversalDuration;
	float				mReelPause;
	float				mReelPauseDuration;
	
	
	/*
	 * List of all the extended drawable datasources,
	 * new plugins must be added to this list for it
	 * to be listed in the dropdown menu.
	 */
	static QList< QAction* > sDrawableDatasources;
};

#endif // GLFRAMEWORK_HPP
