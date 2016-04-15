/*
 * This class defines how text is displayed when sent as an argument for drawText()
 */

#ifndef FORMAT_HPP
#define FORMAT_HPP

#include <QtCore>
#include <QGLShader>

#include "src/utils/declares.hpp"
#include "src/opengl/shapes/vertex.hpp"
#include "src/utils/shader.hpp"
#include "src/utils/debug.hpp"


namespace rpnoc
{
	class TextFormat
	{
	public:
							TextFormat( QString fontMapLocation = "charset.png",
										int columns = 16, int rows = 1, int spacing = -15,
										int charHeight = 38, int charWidth = 38,
										QVector4D color = QVector4D( 1.0, 0.4, 0.0, 1.0 ) );

							~TextFormat();

		void                setShaderProgram( QGLShaderProgram* shaderProgram );
		QGLShaderProgram*   getShaderProgram( void ) const;

		void                setColumns( int columns );
		int					getColumns( void ) const;

		void                setRows( int rows );
		int					getRows( void ) const;

		void				setSpacing( int spacing );
		int					getSpacing( void ) const;

		void                setCharHeight( int charHeight );
		int					getCharHeight( void ) const;

		void                setCharWidth( int charWidth );
		int					getCharWidth( void ) const;

		void                setFontMapLocation( QString fontMap );
		QString             getFontMapLocation( void ) const;

		void                setColor( QVector4D color );
		QVector4D           getColor( void ) const;

	private:

		int					mColumns;
		int					mRows;
		int					mSpacing;
		int					mCharHeight;
		int					mCharWidth;
		QString             mFontMap;
		QVector4D           mColor;
		QGLShaderProgram*   mShader;

	};

	/**
	  * Global methods to draw text-to-screen
	  */
	void					initCharMaps( void );
	void					drawText(	QString text, TextFormat format,
										const QMatrix4x4& iProjection, const QMatrix4x4& iView, QMatrix4x4 iModel );
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::TextFormat;
	using rpnoc::drawText;
	using rpnoc::initCharMaps;
#endif

#endif // FORMAT_HPP
