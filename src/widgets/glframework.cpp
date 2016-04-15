/*
 * glframework.cpp
 */

#include <QDomDocument>
#include <QMouseEvent>
#include <QFile>
#include "src/opengl/layout.hpp"
#include "src/utils/debug.hpp"
#include "src/utils/texture.hpp"
#include "src/utils/shader.hpp"
#include "src/windows/mainwindow.hpp"
#include "src/opengl/shapes/format.hpp"
#include "src/opengl/shapes/table.hpp"
#include "src/opengl/shapes/cell.hpp"
#include "src/opengl/shapes/square.hpp"
#include "src/network/networker.hpp"
#include "src/opengl/drawables/drawablesourcenagios.hpp"
#include "src/opengl/drawables/drawablesourcedefault.hpp"
#include "src/opengl/drawables/drawablesourceclock.hpp"
#include "src/opengl/drawables/nagiosnewestcriticals.hpp"
#include "src/opengl/drawables/nagiosnewestunknowns.hpp"
#include "src/opengl/drawables/nagiosnewestwarnings.hpp"
#include "src/opengl/drawables/nagiosstatistics.hpp"
#include "src/utils/pluginmanager.hpp"
#include "src/widgets/glframework.hpp"

QList< QAction* > GLFramework::sDrawableDatasources;


GLFramework::GLFramework( QWidget* iParent, QGLFormat iFormat, Networker* iNetworker )
	:	QGLWidget( iFormat, iParent, this ), mReelShow( false ), mReelTraversal( 0.0f ),
		mReelTraversalDuration( 1.0f ), mReelPause( 0.0f ), mReelPauseDuration( 30000.0f ),
		mReelTraversalDelta( 0.03f ), mNetworker( iNetworker ), mShowBackground( true ),
		mShowMessages( true ), mBackgroundColor( 140, 60, 220 )
{

	TextureManager::getInstance().setContext( this );
	ShaderManager::getInstance().setContext( this );
	
	mFrameTimer = new QTimer;
	mFrameTimer->setInterval( 30 );

	mProjection.setToIdentity();
	mProjection.ortho( 0, MainWindow::getWindowSize().x(), MainWindow::getWindowSize().y(), 0, -5, 5 );
	mView.setToIdentity();
}

GLFramework::~GLFramework( void )
{
	delete mDocProfile;
}

QList< QAction* > GLFramework::getDrawableSources( void )
{
	return sDrawableDatasources;
}

void GLFramework::addDrawableSource( QAction* iAction )
{
	sDrawableDatasources.append( iAction );
	qDebug() << "Adding datasource: " << iAction->text();
}

bool GLFramework::slotSetupProfile( QString iProfile )
{
	QFile oFile( QString( "profiles/" ) + iProfile );
	if( !oFile.open( QIODevice::ReadOnly ) ) {
		log_w( "Failed to open: " + QString( "../profiles/" ) + iProfile + QString( ".xml" ) );
		return false;
	}
	
	emit signalEmitStatusMessage( QString( "Profiles/" + iProfile + " loaded" ), GLMessage::MESSAGE_INFO );
	
	QStringList oSubscriptions;
	QXmlStreamReader oReader;
	oReader.setDevice( &oFile );
	oReader.readNextStartElement();
	while( !oReader.atEnd() ) {
		if( oReader.name() == QString( "Profile" ) ) {
			// start of profile
			oReader.readNextStartElement();
			while( oReader.name() != QString( "Profile" ) ) {
				if( oReader.name() == QString( "Layout" ) ) {
					// start of Layout
					Layout* oLayout = new Layout( mNetworker );

					oReader.readNextStartElement();
					while( oReader.name() != QString( "Layout" ) ) {
						if( oReader.name() == QString( "Window" ) ) {
							// start of Window
							QVector2D oPosition;
							QVector2D oSize;
							QString oDataSource;
							QString oBackground;
							QString oBorder;

							oReader.readNextStartElement();
							while( oReader.name() != QString( "Window" ) ) {
								// reading window data
								if( oReader.name() == QString( "Position" ) ) {
									// position data
									oPosition.setX( oReader.attributes().value( "x" ).toString().toInt() );
									oPosition.setY( oReader.attributes().value( "y" ).toString().toInt() );
								} else if( oReader.name() == QString( "Size" ) ) {
									// size data
									oSize.setX( oReader.attributes().value( "x" ).toString().toInt() );
									oSize.setY( oReader.attributes().value( "y" ).toString().toInt() );
								} else if( oReader.name() == QString( "DataSource" ) ) {
									// datasource
									oDataSource = oReader.attributes().value( "name" ).toString();
									oSubscriptions.append( oDataSource );
								} else if( oReader.name() == QString( "Plane" ) ) {
									// plane data
									oBackground = oReader.attributes().value( "background" ).toString();
									oBorder = oReader.attributes().value( "border" ).toString();
								}

								oReader.readNextStartElement();
								oReader.readNextStartElement();
							} // end of window

							oLayout->addWindow( oPosition, oSize, oBackground, oBorder, oDataSource );
						}
						oReader.readNextStartElement();
					} // end of layout

					mLayouts.append( oLayout );
				}

				oReader.readNextStartElement();
			} // end of profile
		}

		oReader.readNextStartElement();
	} // end of xml file
	mNetworker->appendSubscriptions( oSubscriptions );

	return true;
}

void GLFramework::switchLayout( void )
{
	mCurrentLayout = getNextLayout();
	emit signalEmitStatusMessage( QString( "Switching layout" ), GLMessage::MESSAGE_INFO );
}

void GLFramework::slotAddLayout( void )
{
	Layout* oNewLayout = new Layout( mNetworker );
	mLayouts.append( oNewLayout );
	emit signalEmitStatusMessage( QString( "added a new layout to profile, remember to save!" ), GLMessage::MESSAGE_INFO );
	mCurrentLayout = oNewLayout;
}

void GLFramework::slotRemoveLayout( void )
{
	Layout* oTempLayout = mCurrentLayout;
	switchLayout();
	if( oTempLayout != mCurrentLayout ) {
		// remove current layout
		mLayouts.removeOne( oTempLayout );
		emit signalEmitStatusMessage( QString( "removed layout" ), GLMessage::MESSAGE_INFO );
		delete oTempLayout;
	} else {
		// not allowed to delete the last layout
		emit signalEmitStatusMessage( QString( "Tried to delete the last layout" ), GLMessage::MESSAGE_WARNING );
	}
}

void GLFramework::slotShowBackground( bool iShowBackground )
{
	mShowBackground = iShowBackground;
}

bool GLFramework::saveLayouts( void ) 
{
	qDebug() << "profiles/" + mNetworker->getProfile();
	QFile oFile( "profiles/" + mNetworker->getProfile() );
	if( !oFile.open(QFile::ReadWrite | QFile::Text | QFile::Truncate ) ) {
		log_w( QString( "Failed to open file: '" ) + qPrintable( mNetworker->getProfile() ) + "'" + log_endl +
			+ qPrintable( oFile.errorString() ) );
		return false;
	}
	
	QXmlStreamWriter oWriter( &oFile );
	oWriter.setAutoFormatting( true );
	oWriter.writeStartDocument();
	oWriter.writeStartElement( QString( "Profile" ) );
	QList< Layout* >::iterator oItrLayout = mLayouts.begin();
	while( oItrLayout != mLayouts.end() ) {
		(*oItrLayout)->saveLayout( oWriter );
		++oItrLayout;
	}
	
	oWriter.writeEndElement(); // Profile
	oWriter.writeEndDocument();
	
	oFile.close();
	if( oFile.error() ) {
		log_w( QString( "Failed to write to file: '" ) + qPrintable( mNetworker->getProfile() ) + "'" + log_endl +
			+ qPrintable( oFile.errorString() ) );
		return false;
	} else {
		log_i( "Saved profile: 'profiles/" + mNetworker->getProfile() + "'" );
	}
	
	return true;
}

void GLFramework::slotToggleReelShow( void )
{
	if( mReelShow ) {
		mReelShow = false;
	} else {
		mReelShow = true;
		mReelTraversal = 0.0f;
		mReelPause = 0.0f;
	}
}

void GLFramework::slotSetReelSpeed( float iSpeed )
{
	mReelTraversalDelta = iSpeed / 1000.0f;
}

void GLFramework::slotSetReelPauseTime( float iMS )
{
	mReelPauseDuration = iMS;
}

void GLFramework::setBackgroundColor( QVector3D iColor )
{
	mBackgroundColor = iColor;
}

void GLFramework::mouseMoveEvent( QMouseEvent *iEvent )
{
	if(!mReelShow ) {
		mCurrentLayout->mouseUpdate( iEvent, QEvent::MouseMove );
	}
}

void GLFramework::mousePressEvent( QMouseEvent *iEvent )
{
	if(!mReelShow ) {
		mCurrentLayout->mouseUpdate( iEvent, QEvent::MouseButtonPress );
	}
}

void GLFramework::mouseReleaseEvent( QMouseEvent *iEvent )
{
	if(!mReelShow ) {
		mCurrentLayout->mouseUpdate( iEvent, QEvent::MouseButtonRelease );
	}
}

void GLFramework::initializeGL( void )
{
	qglClearColor( QColor( Qt::darkMagenta ) );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glEnable( GL_ALPHA_TEST );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glAlphaFunc( GL_GREATER, 0 );
	glDepthFunc( GL_LEQUAL );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	if( TextureManager::getInstance().initTextures() ) {
		log_i( "Texture Manager succesfully initialized" );
	} else {
		log_e( "Failed to initialize Texture Manager" );
	}
	
	if( ShaderManager::getInstance().initShaders() ) {
		log_i( "Shader Manager succesfully initialized" );
	} else {
		log_e( "Failed to initialize Shader Manager" );
	}
	
	mMessageHandler = new GLMessageHandler;
	mBackground = new Background;
	connect( this, SIGNAL( signalEmitStatusMessage( QString, GLMessage::MessageLevel ) ),
		mMessageHandler, SLOT( slotAddMessage( QString, GLMessage::MessageLevel ) ) );
	mBackground->init();
	
	mSquare = new Square(
		QVector2D( width(), height() ),
		ShaderManager::getInstance().getShader( "default" )
	);

	initCharMaps();
	registerSources();
	
	mCurrentLayout = new Layout( mNetworker );
}

void GLFramework::slotSetupLayouts( QString iXml )
{
	QList< Layout* >::iterator oItrLayout = mLayouts.begin();
	while( oItrLayout != mLayouts.end() ) {
		mLayouts.removeOne( (*oItrLayout) );
		++oItrLayout;
	}
	mCurrentLayout = NULL;
	
	slotSetupProfile( iXml );
	if( mLayouts.length() < 1 ) {
		mCurrentLayout = new Layout( mNetworker );
		mLayouts.append( mCurrentLayout );
	} else {
		log_i( "Profile: '" + iXml + "' has been loaded" );
		mCurrentLayout = mLayouts.at( 0 );
	}
	mFrameTimer->start();
	connect( mFrameTimer, SIGNAL( timeout() ), this, SLOT( updateGL() ) );
}

void GLFramework::slotStatusMessage( QString iMessage, QString iSender )
{
	emit signalEmitStatusMessage( iMessage, GLMessage::MESSAGE_INFO );
}

void GLFramework::slotStatusMessage( QString iMessage, GLMessage::MessageLevel iLevel )
{
	emit signalEmitStatusMessage( iMessage, iLevel );
}

void GLFramework::resizeGL( int iWidth, int iHeight )
{
	glViewport( 0, 0, iWidth, iHeight );
	MainWindow::setWindowSize( QVector2D( iWidth, iHeight ) );
	mProjection.setToIdentity();
	mProjection.ortho( 0, iWidth, iHeight, 0, -5, 5 );

	GLfloat oNewVertices[4][3] = {
		{ iWidth, 0.0f, 0.0f },
		{ iWidth, iHeight, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, iHeight, 0.0f }
	};
	Vertex<3> oNewData[4] = {
		oNewVertices[0],
		oNewVertices[1],
		oNewVertices[2],
		oNewVertices[3]
	};
	mSquare->updateVertices( oNewData );

	QList< Layout* >::iterator oItrLayout = mLayouts.begin();
	while( oItrLayout != mLayouts.end() ) {
		(*oItrLayout)->resize( QVector2D( iWidth, iHeight ) );
		++oItrLayout;
	}
}

void GLFramework::paintGL( void )
{
	qglClearColor( QColor( mBackgroundColor.x(), mBackgroundColor.y(), mBackgroundColor.z() ) );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	setCursor( Qt::ArrowCursor );
	if( mShowBackground ) {
		mBackground->render();
	}

	if( mReelShow ) {
		paintReel();
	} else {
		paintStatic();
	}
	
	QMatrix4x4 oMessageView;
	oMessageView.translate( MainWindow::getWindowSize().x() - 410.0f, MainWindow::getWindowSize().y() - 105.0f, 0.0f );
	QMatrix4x4 oModel;
	
	if( mShowMessages ) {
		mMessageHandler->draw( mProjection, oMessageView, oModel );
	}
}

void GLFramework::paintStatic( void )
{
	mCurrentLayout->draw();

	QMatrix4x4 oModel;

	glBindTexture( GL_TEXTURE_2D, mCurrentLayout->getFBOTexture() );
	mSquare->draw( mProjection, mView, oModel );

	{	// Testing drawText and format
		QMatrix4x4 oView;
		TextFormat oFormat(
			QString( "charset.png" ),
			15, 2,
			-40,
			72, 72,
			QVector4D( 1.0f, 0.0f, 0.0f, 1.0f )
		);
		drawText( QString( "Redpill" ), oFormat, mProjection, oView, oModel );
		oFormat.setColor( QVector4D( 1.0f, 1.0f, 1.0f, 1.0f ) );
		oModel.translate( 232.0f, 0.0f, 0.0f );
		drawText( QString( "Linpro" ), oFormat, mProjection, oView, oModel );
		oModel.translate(  200.0f, 0.0f, 0.0f );
		oFormat.setColor( QVector4D( 0.2f, 1.0f, 0.1f, 1.0f ) );
		drawText( QString( "[Edit Mode]" ), oFormat, mProjection, oView, oModel );

		oModel.translate( -420.0f, 60.0f, 0.0f );
		oFormat.setCharWidth( 40 );
		oFormat.setCharHeight( 40 );
		oFormat.setRows( 1 );
		oFormat.setSpacing( -10 );
		oFormat.setColumns( 30 );
		oFormat.setColor( QVector4D( 1.0f, 1.0f, 1.0f, 0.3f ) );
		drawText( QString( "Northug pulk server ###" ), oFormat, mProjection, oView, oModel );
	}

	ERROR_TEST( ( glGetError() == GL_NO_ERROR ), "OpenGL Error", LEVEL_WARNING )
}

void GLFramework::paintReel( void )
{
	QMatrix4x4 oModel;
	QMatrix4x4 oView;

	if( mReelPause == 0.0f && mLayouts.size() > 1 ) {
		/*
		 * traversal between two layouts [ pushing layouts to the left <-- ]
		 */
		mReelTraversal += mReelTraversalDelta;
		Layout* oLayoutStart = mCurrentLayout;
		Layout* oLayoutEnd = getNextLayout();


		oView.translate( -( mReelTraversal / mReelTraversalDuration ) * ( oLayoutStart->getFBOSize().x() ), 0.0f, 0.0f );
		oLayoutStart->draw();
		glBindTexture( GL_TEXTURE_2D, oLayoutStart->getFBOTexture() );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		mSquare->draw( mProjection, oView, oModel );

		oView.translate( oLayoutStart->getFBOSize().x(), 0.0f, 0.0f );
		oLayoutEnd->draw();
		glBindTexture( GL_TEXTURE_2D, oLayoutEnd->getFBOTexture() );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		mSquare->draw( mProjection, oView, oModel );

		if( mReelTraversal > mReelTraversalDuration ) {
			mReelTraversal = 0.0f;
			mReelPause += 30.0f;
			mCurrentLayout = oLayoutEnd;
		}
	} else {
		/*
		 * pausing effect between slides
		 */
		mCurrentLayout->draw();
		glBindTexture( GL_TEXTURE_2D, mCurrentLayout->getFBOTexture() );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		mSquare->draw( mProjection, oView, oModel );

		 mReelPause += 30.0f;
		 if( mReelPause > mReelPauseDuration ) {
			mReelPause = 0.0f;
		 }
	}
}

Layout* GLFramework::getNextLayout( void )
{
	Layout* oRetVal = mCurrentLayout;
	if( mLayouts.length() > 1 ) {
		QList< Layout* >::iterator oItrLayout = qFind( mLayouts.begin(), mLayouts.end(), mCurrentLayout );
		if( ++oItrLayout == mLayouts.end() ) {
			oRetVal = mLayouts.at( 0 );
		} else {
			oRetVal = (*oItrLayout);
		}
	}

	return oRetVal;
}

void GLFramework::registerSources( void )
{
	Networker::addDataSource( new rpnoc::NagiosSource );
	gPluginManager.addPlugin( new rpnoc::DrawableSourceDefault( 1200 ) );
	gPluginManager.addPlugin( new rpnoc::DrawableSourceNagios( 1200 ) );
	gPluginManager.addPlugin( new rpnoc::DrawableSourceClock( 500 ) );
	gPluginManager.addPlugin( new rpnoc::NagiosStatistics( 1200 ) );
	gPluginManager.addPlugin( new rpnoc::NagiosNewestCriticals( 1200 ) );
	gPluginManager.addPlugin( new rpnoc::NagiosNewestWarnings( 1200 ) );
	gPluginManager.addPlugin( new rpnoc::NagiosNewestUnknowns( 1200 ) );
	gPluginManager.initPlugins();
}
