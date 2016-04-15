#include "src/opengl/shapes/format.hpp"
#include "src/opengl/shapes/square.hpp"
#include "src/utils/texture.hpp"

float charmap[255][8];
Square* gSquare;

rpnoc::TextFormat::TextFormat( QString fontMapLocation,
				int columns, int rows, int spacing,
				int charHeight, int charWidth,
				QVector4D color )
{
	mShader         = ShaderManager::getInstance().getShader( "default" );
	mFontMap        = fontMapLocation;
	mColumns        = columns;
	mRows           = rows;
	mSpacing		= spacing;
	mCharHeight     = charHeight;
	mCharWidth      = charWidth;
	mColor          = color;
}

rpnoc::TextFormat::~TextFormat()
{
	// empty
}

void rpnoc::TextFormat::setShaderProgram( QGLShaderProgram* shaderProgram )
{
    mShader = shaderProgram;
}

QGLShaderProgram* rpnoc::TextFormat::getShaderProgram() const
{
    return mShader;
}

void rpnoc::TextFormat::setColumns( int columns )
{
    mColumns = columns;
}

int rpnoc::TextFormat::getColumns() const
{
    return mColumns;
}

void rpnoc::TextFormat::setRows( int rows )
{
    mRows = rows;
}

int rpnoc::TextFormat::getRows() const
{
    return mRows;
}

void rpnoc::TextFormat::setSpacing( int spacing )
{
	mSpacing = spacing;
}

int rpnoc::TextFormat::getSpacing() const
{
	return mSpacing;
}

void rpnoc::TextFormat::setCharHeight( int charHeight )
{
    mCharHeight = charHeight;
}

int rpnoc::TextFormat::getCharHeight() const
{
    return mCharHeight;
}

void rpnoc::TextFormat::setCharWidth( int charWidth )
{
    mCharWidth = charWidth;
}

int rpnoc::TextFormat::getCharWidth() const
{
    return mCharWidth;
}

void rpnoc::TextFormat::setFontMapLocation( QString fontMapLocation )
{
    mFontMap = fontMapLocation;
}

QString rpnoc::TextFormat::getFontMapLocation() const
{
    return mFontMap;
}

void rpnoc::TextFormat::setColor( QVector4D color )
{
    mColor = color;
}

QVector4D rpnoc::TextFormat::getColor( void ) const
{
    return mColor;
}

/**
  *	Method initializes the mapping for character-lookup in the spritesheets when drawing text
  */
void rpnoc::initCharMaps()
{
	/*
	 * initialize the square
	 */
	gSquare = new Square( QVector2D( 100, 100 ), ShaderManager::getInstance().getShader("default") );
	
	/*
	 * letters are fetchable by ratios
	 */
	float oBlockSizeX = 1.0f / 38.0f;
	float oBlockSizeY = 1.0f / 4.0f;
	
	/*
	 * setting all characters to draw unknown character
	 */
	for( int i = 0; i < 255; ++i ) {
		charmap[ i ][0] = ( 35 * oBlockSizeX ) + oBlockSizeX;
		charmap[ i ][1] = ( 3.0f * oBlockSizeY );
		charmap[ i ][2] = ( 35 * oBlockSizeX ) + oBlockSizeX;
		charmap[ i ][3] = ( 4.0f * oBlockSizeY );
		charmap[ i ][4] = ( 35 * oBlockSizeX );
		charmap[ i ][5] = ( 3.0f * oBlockSizeY );
		charmap[ i ][6] = ( 35 * oBlockSizeX );
		charmap[ i ][7] = ( 4.0f * oBlockSizeY );
	}
	
	/*
	 * adding a-z and A-Z
	 */
	for( int i = 97; i < ( 97 + 29 ); ++i ) {
		charmap[i][0] = ( ( i - 97 ) * oBlockSizeX ) + oBlockSizeX;
		charmap[i][1] = ( 0.0f * oBlockSizeY );
		charmap[i][2] = ( ( i - 97 ) * oBlockSizeX ) + oBlockSizeX;
		charmap[i][3] = ( 1.0f * oBlockSizeY );
		charmap[i][4] = ( i - 97 ) * oBlockSizeX;
		charmap[i][5] = ( 0.0f * oBlockSizeY );
		charmap[i][6] = ( i - 97 ) * oBlockSizeX;
		charmap[i][7] = ( 1.0f * oBlockSizeY );
	
		charmap[i-32][0] = ( ( i - 97 ) * oBlockSizeX ) + oBlockSizeX;
		charmap[i-32][1] = ( 1.0f * oBlockSizeY );
		charmap[i-32][2] = ( ( i - 97 ) * oBlockSizeX ) + oBlockSizeX;
		charmap[i-32][3] = ( 2.0f * oBlockSizeY );
		charmap[i-32][4] = ( i - 97 ) * oBlockSizeX;
		charmap[i-32][5] = ( 1.0f * oBlockSizeY );
		charmap[i-32][6] = ( i - 97 ) * oBlockSizeX;
		charmap[i-32][7] = ( 2.0f * oBlockSizeY );
	}
	
	/*
	 * adding numbers
	 */
	for( int i = 48; i < ( 48 + 10 ); ++i ) {
		charmap[i][0] = ( ( i - 48 ) * oBlockSizeX ) + oBlockSizeX;
		charmap[i][1] = ( 2.0f * oBlockSizeY );
		charmap[i][2] = ( ( i - 48 ) * oBlockSizeX ) + oBlockSizeX;
		charmap[i][3] = ( 3.0f * oBlockSizeY );
		charmap[i][4] = ( i - 48 ) * oBlockSizeX;
		charmap[i][5] = ( 2.0f * oBlockSizeY );
		charmap[i][6] = ( i - 48 ) * oBlockSizeX;
		charmap[i][7] = ( 3.0f * oBlockSizeY );
	}
	
	/*
	 * adding special characters
	 */
	int oKeys[37] = {
		33, // !
		34, // "
		35, // #
		254, // ¤
		37, // %
		38, // &
		47, // /
		40, // (
		41, // )
		61, // =
		63, // ?
		92, // '\'
		96, // `
		44, // ,
		46, // .
		59, // ;
		58, // :
		39, // '
		42, // *
		94, // ^
		126, // ~
		60, // <
		62, // >
		64, // @
		156, // £
		36, // $
		123, // {
		125, // }
		91, // [
		93, // ]
		43, // +
		45, // -
		95, // _
		124, // |
		253, // §
		178, // used as unvalid key 
		32 // space
	};
	for( int i = 0; i < 37; ++i ) {
		charmap[ oKeys[ i ] ][0] = ( i * oBlockSizeX ) + oBlockSizeX;
		charmap[ oKeys[ i ] ][1] = ( 3.0f * oBlockSizeY );
		charmap[ oKeys[ i ] ][2] = ( i * oBlockSizeX ) + oBlockSizeX;
		charmap[ oKeys[ i ] ][3] = ( 4.0f * oBlockSizeY );
		charmap[ oKeys[ i ] ][4] = ( i * oBlockSizeX );
		charmap[ oKeys[ i ] ][5] = ( 3.0f * oBlockSizeY );
		charmap[ oKeys[ i ] ][6] = ( i * oBlockSizeX );
		charmap[ oKeys[ i ] ][7] = ( 4.0f * oBlockSizeY );
	}
}

void rpnoc::drawText( QString text, TextFormat format, const QMatrix4x4& iProjection,
					  const QMatrix4x4& iView , QMatrix4x4 iModel )
{
	for( int i = 0; i < text.length(); ++i ){
		// see if we should break to a new line
		if( ( i != 0 )  && ( ( i % format.getColumns() ) == 0 ) ) {
			/*
			 * charWidth & Height behave in a such a way that we have to cast to int before we
			 * make it a negative (or we lose byte-precision).
			 */
			int oBacktrack = format.getColumns() * ( format.getCharWidth() + format.getSpacing() );
			iModel.translate(  -oBacktrack, format.getCharHeight() + format.getSpacing() );
		}
		
		/*
		 * work out the coordinate for the
		 * current letter in 2D.
		 */
		float* oCoords = NULL;
		if( i > ( format.getColumns() * format.getRows() ) - 1 ) {
			// skip past this letter if all columns and rows are used already
			continue;
		} else if( ( text.length() > format.getColumns() * format.getRows() ) &&
			( i > ( format.getColumns() * format.getRows() ) - 4 ) ) {
			/* 
			 * if the string contains more letters than we allow, and we
			 * are at the three last letters, convert them to dots ( ... )
			 */
			oCoords = charmap[ '.' ];
		} else {
			/*
			 * get the texcoords for the current letter ( default )
			 */
			int oKey = text.at( i ).toAscii();
			if( oKey > 255 || oKey < 0 ) {
				switch( oKey ) {
					case static_cast<int>( '¤' ) :
						oKey = 254;
						break;
					case static_cast<int>( '§' ) :
						oKey = 253;
						break;
					default :
						oKey = 178;
						break;
				}
			}
			oCoords = charmap[ oKey ];
		}
		
		/*
		 * upload the texcoord to the square that is 
		 * about to draw the current letter.
		 */
		Vertex<2> oNewTex[4] = {
			&oCoords[0],
			&oCoords[2],
			&oCoords[4],
			&oCoords[6]
		};
		
		gSquare->updateTexCoords( oNewTex );
		
		GLfloat oVertex[4][3] = {
			{ format.getCharWidth(), 0.0f, 0.0f },
			{ format.getCharWidth(), format.getCharHeight(), 0.0f },
			{ 0.0f, 0.0f, 0.0f  },
			{ 0.0f, format.getCharHeight(), 0.0f }
		};
		Vertex<3> oVertexData[4] = {
			oVertex[0],
			oVertex[1],
			oVertex[2],
			oVertex[3]
		};
		gSquare->updateVertices( oVertexData );
		
		/*
		 * upload the color to the square that is 
		 * about to draw the current letter
		 */
		GLfloat oColor[4] = {
			format.getColor().x(),
			format.getColor().y(),
			format.getColor().z(),
			format.getColor().w()
		};
		Vertex<4> oColorData[4] = {
			oColor,
			oColor,
			oColor,
			oColor
		};
		gSquare->updateColors( oColorData );

		
		/*
		 * bind the texture, draw the letter,
		 * and translates to the next letter position.
		 */
		TextureManager::getInstance().bindTexture( "charset.png" );
		gSquare->draw( iProjection, iView, iModel );
		iModel.translate( format.getCharWidth() + format.getSpacing(), 0, 0 );
	}
}
