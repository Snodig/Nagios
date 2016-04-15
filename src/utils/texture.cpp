/*
 *
 */

#include <QDebug>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "src/utils/debug.hpp"
#include "src/utils/texture.hpp"

using namespace boost::filesystem;

rpnoc::TextureManager::TextureManager( void )
{
	// private 
}

rpnoc::TextureManager::~TextureManager( void )
{
	// private
}

rpnoc::TextureManager& rpnoc::TextureManager::getInstance( void )
{
	static TextureManager oInstance;
	return oInstance;
}

/**
  *	Returns a list of all textures available
  */
QList<QString> rpnoc::TextureManager::getTextureNames( void )
{
	return mTextures.uniqueKeys();
}

/**
  *	Binds a texture to be used on geomoetry until unbound
  */
void rpnoc::TextureManager::bindTexture( QString iTexture ) const
{
	QMap< QString, GLuint >::const_iterator oItrTexture = mTextures.find( iTexture );
	if( oItrTexture != mTextures.end() ) {
		glBindTexture( GL_TEXTURE_2D, oItrTexture.value() );
	} else {
		log_e( "Tried to bind a texture that does not exist: '" + iTexture + "'." );
		oItrTexture = mTextures.find( QString( "error.png" ) );
		glBindTexture( GL_TEXTURE_2D, oItrTexture.value() );
	}

	// better alpha on textures, but less nice
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

/**
  *	Adds and formats a new texture for use
  */
bool rpnoc::TextureManager::addTexture( QString iTexture, QString iFormat )
{
	QImage oImage;
	if( !oImage.load( QString( "images/" + iTexture ) , iFormat.toStdString().c_str() ) ) {
		return false;
	}
	
	GLuint oTexID = mContext->bindTexture( 
			oImage, GL_TEXTURE_2D, 
			GL_RGBA, 
			QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption 
	);
	
	log_i( QString( "Added Texture: '" + iTexture + "', id: '" + QString::number( oTexID ) + "'." ) );
	mTextures.insert( iTexture, oTexID );
	return true;
}

bool rpnoc::TextureManager::removeTexture( QString iTexture )
{
	QMap< QString, GLuint >::const_iterator oItrTexture = mTextures.find( iTexture );
	if( oItrTexture != mTextures.end() ) {
		// TODO: write rest of remove texture procedure
		log_i( "RemoveTexture function not implemented." );
	}
	return true;
}

/**
  *	Sets the oGL-context we're working for/with
  */
void rpnoc::TextureManager::setContext( QGLWidget *iContext )
{
	mContext = iContext;
}

/**
  *	Loads all textures in images-folder hierarchy
  */
bool rpnoc::TextureManager::initTextures( void )
{
	bool oRetVal = true;
	
	/*
	 * Load all the border images found in the 'images/borders/' folder
	 */
	path oPathBorders( initial_path<path>().string() + "/images/borders" );
	if( exists( oPathBorders ) ) {
		log_i( "Borders folder found at: '" + QString( oPathBorders.string().c_str() ) + "'." );
		/*
		 * Loop through all the files in the directory and try to load them
		 */
		directory_iterator oItrEnd;
		for( directory_iterator oItrDir( oPathBorders ); oItrDir != oItrEnd; ++oItrDir ) {
			if( is_regular_file( oItrDir->status() ) ) {
				qDebug() << "		" << oItrDir->path().filename().c_str();
				// TODO: add support for more than just png files
				addTexture( QString( "borders/" ) + oItrDir->path().filename().c_str(), QString( "PNG" ) );
			}
		}
	} else {
		log_e( "Could not find '/images/borders' folder" + log_endl + QString( oPathBorders.string().c_str() ) );
	}
	
	/*
	 * Load all the background images found in the 'images/backgrounds/' folder
	 */
	path oPathBackgrounds( initial_path<path>().string() + "/images/backgrounds" );
	if( exists( oPathBackgrounds ) ) {
		log_i( "Backgrounds folder found at: '" + QString( oPathBackgrounds.string().c_str() ) + "'." );
		
		/*
		 * Loop through all the files in the directory and try to load them
		 */
		 directory_iterator oItrEnd;
		 for( directory_iterator oItrDir( oPathBackgrounds ); oItrDir != oItrEnd; ++oItrDir ) {
			qDebug() << "		" << oItrDir->path().filename().c_str();
			// TODO: add support for more than just png files
			addTexture( QString( "backgrounds/" ) + oItrDir->path().filename().c_str(), QString( "PNG" ) );
		 }
	} else {
		log_e( "Could not find '/images/backgrounds' folder" + log_endl + QString( oPathBackgrounds.string().c_str() ) );
	}
	
	/*
	 * Adding a few default textures that will be used by the program
	 */
	if( !addTexture( QString( "error.png" ), QString( "PNG" ) ) ) oRetVal = false;
	if( !addTexture( QString( "transparent.png" ), QString( "PNG" ) ) ) oRetVal = false;
	if( !addTexture( QString( "panel_background_0.png" ), QString( "PNG" ) ) ) oRetVal = false;
	if( !addTexture( QString( "Cyan_Water.png" ), QString( "PNG" ) ) ) oRetVal = false;
	if( !addTexture( QString( "panel_border_0.png" ), QString( "PNG" ) ) ) oRetVal = false;
	if( !addTexture( QString( "Red-Dot.png" ), QString( "PNG" ) ) ) oRetVal = false;
	if( !addTexture( QString( "charset.png" ), QString( "PNG" ) ) ) oRetVal = false;
	if( !addTexture( QString( "Dot.png" ), QString( "PNG" ) ) ) oRetVal = false;
	if( !addTexture( QString( "ubuntu.png" ), QString( "PNG" ) ) ) oRetVal = false;
	
	return oRetVal;
}
