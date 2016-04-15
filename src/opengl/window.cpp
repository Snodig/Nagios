/*
 *
 */

#include "src/opengl/shapes/vertex.hpp"
#include "src/widgets/glframework.hpp"
#include "src/utils/debug.hpp"
#include "src/utils/shader.hpp"
#include "src/utils/texture.hpp"
#include "src/windows/mainwindow.hpp"
#include "src/utils/pluginmanager.hpp"
#include "src/opengl/window.hpp"


QVector2D Window::sResizeArea = QVector2D( 20.0, 20.0 );


Window::Window( QVector2D iPosition, QVector2D iSize, QString iDataSource, QString iBackground, QString iBorder, QWidget* iParent )
	:	QWidget( iParent ), mMouseLastPosition( QVector2D( 0.0, 0.0 ) ),
		mSelected( false ), mResize( false ), mResizeStatus( RESIZE_NONE ), mMinimumSize( QVector2D( 300, 200 ) ) {

	mMenuBackground = new QMenu( QString( "Background" ) );
	mMenuBorder = new QMenu( QString( "Border" ) );
	QList<QString> oTextures = TextureManager::getInstance().getTextureNames();
		for( int i = 0; i < oTextures.length(); ++i ) {
			if( QString( oTextures.at( i ) ).contains( QString( "backgrounds/" ) ) ) {
				mMenuBackground->addAction( oTextures.at( i ) );
			} else if( QString( oTextures.at( i ) ).contains( QString( "borders/" ) ) ) {
				mMenuBorder->addAction( oTextures.at( i ) );
		}
	}

	mActionRemoveWindow = new QAction( QString( "Close" ), this );

	mSettings = new WindowSettings( iPosition, iSize );
	mPanel = new Panel(
		iSize,
		ShaderManager::getInstance().getShader( "default" ),
		iBackground,
		iBorder
	);

	mSquare = new Square(
		QVector2D( mSettings->getWindowSize().x() - 40.0, mSettings->getWindowSize().y() - 40.0 ),
		ShaderManager::getInstance().getShader( "default" )
	);

	mDataSource = gPluginManager.getPlugin( iDataSource );
}

Window::~Window( void ) {
	delete mPanel;
	delete mMenuBackground;
	delete mMenuBorder;
	delete mSettings;
	delete mSquare;
}

void Window::draw( void ) {
	QMatrix4x4 oProjection;
	oProjection.ortho( 0, MainWindow::getWindowSize().x(), 0, MainWindow::getWindowSize().y(), -5, 5 );
	QMatrix4x4 oView;
	QMatrix4x4 oModel;
	oModel.translate( mSettings->getWindowPosition().x(), mSettings->getWindowPosition().y(), 0 );

	mPanel->draw( oProjection, oView, oModel );

	if( mDataSource != NULL ) {
		mDataSource->bindBuffer();
		mDataSource->draw();
		mDataSource->unbindBuffer();

		glBindTexture( GL_TEXTURE_2D, mDataSource->getFBOTexture() );
		oModel.translate( 20.0, 20.0, 0.0 );
		mSquare->draw( oProjection, oView, oModel );
	}
}

void Window::saveWindow( QXmlStreamWriter &iWriter )
{
	iWriter.writeStartElement( QString( "Window" ) );
	
	iWriter.writeStartElement( QString( "Position" ) );
	iWriter.writeAttribute( QString( "x" ), QString::number( mSettings->getWindowPosition().x() ) );
	iWriter.writeAttribute( QString( "y" ), QString::number( mSettings->getWindowPosition().y() ) );
	iWriter.writeEndElement(); // Position
	
	iWriter.writeStartElement( QString( "Size" ) );
	iWriter.writeAttribute( QString( "x" ), QString::number( mSettings->getWindowSize().x() ) );
	iWriter.writeAttribute( QString( "y" ), QString::number( mSettings->getWindowSize().y() ) );
	iWriter.writeEndElement(); // Size
	
	iWriter.writeStartElement( QString( "DataSource" ) );
	iWriter.writeAttribute( QString( "name" ), QString( mDataSource->getName() ) );
	iWriter.writeEndElement(); // DataSource
	
	iWriter.writeStartElement( QString( "Plane" ) );
	iWriter.writeAttribute( QString( "background" ), QString( mPanel->getBackground() ) );
	iWriter.writeAttribute( QString( "border" ), QString( mPanel->getBorder() ) );
	iWriter.writeEndElement(); // Plane
	
	iWriter.writeEndElement(); // Window
}

bool Window::clicked( QMouseEvent *iEvent, bool iSelected ) {
	bool oRetVal = false;
	if( iEvent->buttons() & Qt::RightButton && !iSelected ) {
		QMenu oMenu( this );
		QList< QAction* > oDrawableList = GLFramework::getDrawableSources();
		QList< QAction* >::iterator oItrAction = oDrawableList.begin();
		while( oItrAction != oDrawableList.end() ) {
			oMenu.addAction( (*oItrAction) );
			++oItrAction;
		}
		oMenu.addSeparator();
		oMenu.addMenu( mMenuBackground );
		oMenu.addMenu( mMenuBorder );
		oMenu.addSeparator();
		oMenu.addAction( mActionRemoveWindow );
		QAction* oAction = oMenu.exec( iEvent->globalPos() );
		if( oAction != NULL ) {
			if( gPluginManager.getPlugin( oAction->text() ) != NULL ) {
				// if a datasource was selected
				mDataSource = gPluginManager.getPlugin( oAction->text() );
				emit signal_subscription_attempted( mDataSource->getSourceName() );
			} else if( oAction->text() == mActionRemoveWindow->text() ) {
				// if close window was selected
				emit signal_closeWindow( this );
			} else if( QString( oAction->text() ).contains( "borders/" ) ) {
				// if a border was selected
				mPanel->setBorder( oAction->text() );
			} else if( QString( oAction->text() ).contains( "backgrounds/" ) ) {
				// if a background was selected
				mPanel->setBackground( oAction->text() );
			}
			qDebug() << "Action pressed: " << oAction->text();
		} else {
			// no action was clicked
		}
	}

	/*
	 * see if window is selected( iSelected ), and that the window itself is selected
	 * with a left-button click( mSelected ).
	 */
	if( iEvent->buttons() & Qt::LeftButton && iSelected && mSelected ) {
		if( mResize ) {
			QVector2D oResize = QVector2D( iEvent->x(), iEvent->y() ) - mMouseLastPosition;
			resizeWindow( oResize );
		} else {
			QApplication::setOverrideCursor( Qt::ClosedHandCursor );
			QVector2D oMove = QVector2D( iEvent->x(), iEvent->y() ) - mMouseLastPosition;
			mSettings->setWindowPosition( mSettings->getWindowPosition() + oMove );
		}
	} else if( iEvent->button() & Qt::LeftButton && iSelected ) {
		mSelected =	true;
		oRetVal = true;

		QVector2D oResizeArea = mSettings->getWindowPosition() + mSettings->getWindowSize();
		oResizeArea -= sResizeArea;
		if( ( oResizeArea.x() < iEvent->x() || oResizeArea.y() < iEvent->y() ) &&
				( oResizeArea.x() > 150 ) && ( oResizeArea.y() > 150 ) ) {
			mResize = true;
		}
	}

	mMouseLastPosition = QVector2D( iEvent->x(), iEvent->y() );
	return oRetVal;
}

void Window::reset( void ) {
	QApplication::setOverrideCursor( Qt::ArrowCursor);
	mSelected = false;
	mResize = false;
}

bool Window::checkCollision( QVector2D iPoint ) {
	return mSettings->checkCollision( iPoint );
}

void Window::resizeWindow( QVector2D iResize ) {
	if( mSettings->getWindowSize().x() + iResize.x() > 300 &&
		mSettings->getWindowSize().x() + iResize.y() > 200 ) {
		mSettings->setWindowSize( mSettings->getWindowSize() + iResize );
	}
	/*
	 * resizing window forces us to upload new
	 * vertices to the GPU, triangle strip (1-2-3)(2-3-4)
	 *
	 * 4----2
	 * |	|
	 * |	|
	 * 3----1
	 *
	 */
	GLfloat oNewWindowVertices[ 4 ][ 3 ] = {
		{ mSettings->getWindowSize().x(), 0.0f, 0.0f },
		{ mSettings->getWindowSize().x(), mSettings->getWindowSize().y(), 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, mSettings->getWindowSize().y(), 0.0f }
	};

	/*
	 * sending the new window size to panel
	 * buffer.
	 */
	QVector2D oNewPanelSize( mSettings->getWindowSize().x(), mSettings->getWindowSize().y() );
	mPanel->setSize( oNewPanelSize );

	/*
	 * resizeing the square that datasource
	 * is drawn on.
	 */
	oNewWindowVertices[0][0] -= 40.0f;
	oNewWindowVertices[1][0] -= 40.0f;
	oNewWindowVertices[1][1] -= 40.0f;
	oNewWindowVertices[3][1] -= 40.0f;
	Vertex< 3 > oSquareVertices[ 4 ] = {
		oNewWindowVertices[0],
		oNewWindowVertices[1],
		oNewWindowVertices[2],
		oNewWindowVertices[3]
	};
	mSquare->updateVertices( oSquareVertices );

	QApplication::setOverrideCursor( Qt::SizeFDiagCursor );
}

const WindowSettings& Window::settings( void ) {
	return (*mSettings);
}


