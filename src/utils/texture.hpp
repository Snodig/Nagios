/*
 *	Manager to add, setup and retreive textures we want to use on our geometry
 */

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <QGLWidget>
#include <QImage>
#include "src/utils/declares.hpp"


namespace rpnoc
{
	class TextureManager
	{
	public:
		static TextureManager&	getInstance( void );
		QList<QString>			getTextureNames( void );
		void					bindTexture( QString iTexture ) const;
		bool					addTexture( QString iTexture, QString iFormat );
		bool					removeTexture( QString iTexture );
		void					setContext( QGLWidget* iContext );
		bool					initTextures( void );

	private:
								TextureManager( void );
								TextureManager( const TextureManager& iTextureManager );
		TextureManager			operator=( const TextureManager& iTextureManager );
								~TextureManager( void );

		QMap< QString, GLuint >	mTextures;
		QGLWidget*				mContext;
	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::TextureManager;
#endif

#endif // TEXTURE_HPP
