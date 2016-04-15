/*
 *
 */

#include <QtOpenGL>
#include "src/opengl/layout.hpp"
#include "src/windows/mainwindow.hpp"
#include "src/utils/debug.hpp"
#include "src/utils/shader.hpp"
#include "src/utils/texture.hpp"

/*
 * tells update if mouse has been released
 */
QVector2D Layout::sMouseReleased = QVector2D( -666666, -666666 );
QGLFramebufferObject* gLayoutBuffer;


Layout::Layout( Networker* iNetworker, QWidget* iParent ) : QWidget( iParent ) {
	mNetworker = iNetworker;
	mActionNewWindow = new QAction( "&New Window", this );
	mActionNewWindow->setShortcut( QString( "Ctrl+n" ) );
	mActionNewWindow->setStatusTip( "Creates a new static window" );
	connect( mActionNewWindow, SIGNAL( triggered() ),
		this, SLOT( slot_newWindow() ) );
	
	mActionExit = new QAction( "&Exit", this );
	mActionExit->setShortcut( QKeySequence::Quit );
	mActionExit->setStatusTip( "Exit the application" );
	connect( mActionExit, SIGNAL( triggered() ),
		this, SLOT( close() ) );
		
	mFrameBuffer = new QGLFramebufferObject( 
		QSize( MainWindow::getWindowSize().x(), MainWindow::getWindowSize().y() ) 
	);
	
	TextureManager::getInstance().addTexture( "window.png", "PNG" );
	mPrevPosition = sMouseReleased;
}

Layout::~Layout( void ) {
	delete mActionNewWindow;
	delete mActionExit;
}

void Layout::mouseUpdate( QMouseEvent* iEvent, QEvent::Type iType ) {
	QList< QPair< Window*, bool > >::iterator oItrWindow = mWindows.begin();
	
	/*
	 * send signal to window that is hit by mouse pointer
	 */
	oItrWindow = mWindows.begin();
	while( oItrWindow != mWindows.end() ) {
		/*
		 * See if mouse previous position needs to be reset, will be reset if mouse was released
		 * or used right-clicked something.
		 */
		if( ( ( mPrevPosition.x() == sMouseReleased.x() ) && ( mPrevPosition.y() == sMouseReleased.y() ) )
				|| iEvent->buttons() & Qt::RightButton ) {
			mPrevPosition = QVector2D( iEvent->x(), iEvent->y() );
		}
		
		/*
		 * Check if mouse hit window
		 */
		if( oItrWindow->first->checkCollision( QVector2D( mPrevPosition.x(), mPrevPosition.y() ) ) ) {
			if( ( iType == QEvent::MouseButtonPress ) && ( iEvent->buttons() & Qt::LeftButton ) ) {
				oItrWindow->second = true;
			}
			
			/*
			 * moves window to the front if is selected on click
			 */
			if( oItrWindow->first->clicked( iEvent, oItrWindow->second ) ) {
				int oIndex = mWindows.indexOf( (*oItrWindow) );
				for( int i = 0; i < oIndex; ++i ) {
					mWindows.move( oIndex - i, i );
				}
			}
			
			break; // break while loop if we hit a window
		}
		
		++oItrWindow;
	}
	
	/*
	 * if no window is hit, then send signal to layout
	 */
	if( oItrWindow == mWindows.end() ) {
		if( iEvent->buttons() & Qt::RightButton ) {
			mNewWindowPosition = QVector2D( iEvent->x(), iEvent->y() );
			mNewWindowSize = QVector2D( 300, 200 );
			
			QMenu menu( this );
			menu.addAction( mActionNewWindow );
			menu.addSeparator();
			menu.addAction( mActionExit );
			menu.exec( iEvent->globalPos() );
		}
	}
	
	/*
	 * if button released, clear selected status
	 */
	if( iEvent->type() == QEvent::MouseButtonRelease ) {
		oItrWindow = mWindows.begin();
		while( oItrWindow != mWindows.end() ) {
			oItrWindow->second = false;
			oItrWindow->first->reset();
			++oItrWindow;
		}
		
		/*
		 * Reset the mPrevPosition 
		 */
		 mPrevPosition = sMouseReleased;
	} else {
		mPrevPosition = QVector2D( iEvent->x(), iEvent->y() );
	}
}

void Layout::addWindow( QVector2D iPosition, QVector2D iSize, QString iTexBackground, QString iTexBorder, QString iDataSource ) {
	mWindows.append( QPair< Window*, bool >( new Window( iPosition, iSize, iDataSource, iTexBackground, iTexBorder, this ), false ) );
	qDebug() << "Added Window at: " << iPosition.x() << ", " << iPosition.y();
	connect( mWindows.back().first, SIGNAL( signal_closeWindow(Window*) ),
		this, SLOT( slot_removeWindow( Window* ) ) );
	connect( mWindows.back().first, SIGNAL( signal_subscription_attempted( QString ) ), mNetworker, SLOT( subscribe( QString ) ) );
}

void Layout::resize( QVector2D iSize ) {
	delete mFrameBuffer;
	mFrameBuffer = new QGLFramebufferObject( QSize( iSize.x(), iSize.y() ) );
}

void Layout::draw( void ) {
	gLayoutBuffer = mFrameBuffer;
	mFrameBuffer->bind();
	
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	/*
	 * WARNING: going through list backwards
	 *	Qt does not provide backward iterators.
	 */
	QList< QPair< Window*, bool > >::iterator oItrWindow = mWindows.end();
	while( oItrWindow != mWindows.begin() ) {
		--oItrWindow;
		oItrWindow->first->draw();
	}
	
	mFrameBuffer->release();
}

void Layout::slot_newWindow( void ) {
	addWindow( mNewWindowPosition, mNewWindowSize, QString( "backgrounds/white.png" ), QString( "borders/round_corners.png" ) );
}

void Layout::slot_removeWindow( Window* iWindow ) {
	QList< QPair< Window*, bool > >::iterator oItrWindow = mWindows.begin();
	while( oItrWindow != mWindows.end() ) {
		if( iWindow->settings().getID() == oItrWindow->first->settings().getID() ) {
			oItrWindow = mWindows.erase( oItrWindow );
			// TODO: why does this crash the application?
			// delete iWindow;
		} else {
			++oItrWindow;
		}
	}
}

GLuint Layout::getFBOTexture( void ) const {
	return mFrameBuffer->texture();
}

QVector2D Layout::getFBOSize( void ) const {
	return QVector2D( mFrameBuffer->size().width(), mFrameBuffer->size().width() );
}

void Layout::saveLayout( QXmlStreamWriter& iWriter )
{
	iWriter.writeStartElement( "Layout" );
	
	QList< QPair< Window*, bool > >::iterator oItrWindow = mWindows.begin();
	while( oItrWindow != mWindows.end() ) {
		oItrWindow->first->saveWindow( iWriter );
		++oItrWindow;
	}
	
	iWriter.writeEndElement(); // Layout
}
