#include "src/opengl/drawables/drawablesource.hpp"

#include "src/network/networker.hpp"
#include "src/widgets/glframework.hpp"
#include "src/opengl/layout.hpp"
#include "src/utils/shader.hpp"
#include "src/utils/texture.hpp"
#include "src/windows/mainwindow.hpp"

rpnoc::DrawableSource::DrawableSource( QString name, QString datasourceName, float windowSize, QWidget *parent )
	: QWidget( parent),
	mName( name ), mDataSourceName( datasourceName ), mWindowSize( windowSize ), mActionOnClick( NULL )
{
	//
}

rpnoc::DrawableSource::~DrawableSource()
{
	//
}

void rpnoc::DrawableSource::init( void )
{
	mActionOnClick = new QAction( mName, this );
	GLFramework::addDrawableSource( mActionOnClick );
	mFrameBuffer = new QGLFramebufferObject( QSize( mWindowSize, mWindowSize ) );
	mDataSource = Networker::getDataSource( mDataSourceName );
}

QString rpnoc::DrawableSource::getName() const
{
    return mName;
}

void rpnoc::DrawableSource::setName( QString name )
{
    name = mName;
}

QString rpnoc::DrawableSource::getSourceName( void ) const
{
	return mDataSourceName;
}

/**
  *	Sets the pointer to the DataSource-class we'll be getting data from
  */
bool rpnoc::DrawableSource::setSourcePointer( QString iName )
{
	mDataSource = Networker::getDataSource( iName );
	if( mDataSource != NULL ) {
		return true;
	}else{
		return false;
	}
}

/**
  *	Binds this Drawable to the framebuffer we'll be drawing to
  */
void rpnoc::DrawableSource::bindBuffer( void )
{
	glViewport( 0, 0, mFrameBuffer->width(), mFrameBuffer->height() );
	
	gLayoutBuffer->release();
	mFrameBuffer->bind();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	mProjection.setToIdentity();
	mProjection.perspective( 90, (float)mFrameBuffer->size().width() / mFrameBuffer->size().height(), 0.0, 2000.0 );
	mView.setToIdentity();
	mView.lookAt( QVector3D( 0.0f, 0.0f, 0.0f ), QVector3D( 0.0f, 0.0f, -1.0f ), QVector3D( 0.0f, 1.0f, 0.0f ) );
	mModel.setToIdentity();
	mModel.translate( -mFrameBuffer->width() / 2, -mFrameBuffer->width() / 2, -mFrameBuffer->width() / 2 ); 
}

void rpnoc::DrawableSource::unbindBuffer( void )
{
	mFrameBuffer->release();
	gLayoutBuffer->bind();
	glViewport( 0, 0, MainWindow::getWindowSize().x(), MainWindow::getWindowSize().y() );
}

/**
  *	The texture we're pasting the framebufferobject onto, to display it
  */
GLuint rpnoc::DrawableSource::getFBOTexture() const
{
    return mFrameBuffer->texture();
}
