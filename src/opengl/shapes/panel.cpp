/*
 *
 */

#include <QVector2D>
#include "src/opengl/shapes/panel.hpp"
#include "src/utils/texture.hpp"
#include "src/utils/shader.hpp"


rpnoc::Panel::Panel( QVector2D iSize,
			  QGLShaderProgram* iShaderProgram,
			  QString iTexBackground,
			  QString iTexBorderLayout ) 
	:	mTexBorder( iTexBorderLayout ), mTexBackground( iTexBackground ), mSize( iSize )
{
	/*
	 * _________________________________________
	 * | mWindow[0] |  mWindow[1]  | mWindow[2] |
	 * |________________________________________|
	 * | mWindow[3] |  mWindow[4]  | mWindow[5] |
	 * |________________________________________|
	 * | mWindow[6] |  mWindow[7]  | mWindow[8] |
	 * ------------------------------------------
	 *
	 * mWindow[0/2/6/8] = corners
	 * mWindow[1,3,5,7] = horizontal and vertical borders
	 * mWindow[4]		= background texture
	 *
	 * Using iTexBorderLayout to texturize corners and
	 * border. Doing this to prevent texture smearing
	 * when resizing panel.
	 */
	
	// border rations
	const float WINDOW_WIDTH = 24.0f;
	const float WINDOW_HEIGHT = 12.0f;
	const float BLOCK_SIZE = 4.0f;
	const float BIG_OFFSET = 6.0f;
	
	// Top Left Corner
	mWindow[6] = new Square( QVector2D( 20.0, 20.0 ), iShaderProgram );
	GLfloat oTexCoordTopLeftCorner[4][2] = {
		{ ( ( 0.0f * BIG_OFFSET ) + ( 0.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 3.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 0.0f * BIG_OFFSET ) + ( 0.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 2.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 0.0f * BIG_OFFSET ) + ( 1.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 2.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 0.0f * BIG_OFFSET ) + ( 1.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 3.0f * BLOCK_SIZE ) / WINDOW_HEIGHT }
	};
	Vertex<2> oTexCoordTopLeftCornerData[4] = {
		oTexCoordTopLeftCorner[2],
		oTexCoordTopLeftCorner[3],
		oTexCoordTopLeftCorner[1],
		oTexCoordTopLeftCorner[0]
	};
	mWindow[6]->updateTexCoords( oTexCoordTopLeftCornerData );
	
	// Bottom Middle Border
	mWindow[7] = new Square( QVector2D( iSize.x() - 40.0, 20.0 ), ShaderManager::getInstance().getShader( "border_x" ) );
	GLfloat oTexCoordMiddleTopBorder[4][2] = {
		{ ( ( 1.0f * BIG_OFFSET ) + ( 1.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 3.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 1.0f * BIG_OFFSET ) + ( 1.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 2.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 1.0f * BIG_OFFSET ) + ( 2.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 2.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 1.0f * BIG_OFFSET ) + ( 2.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 3.0f * BLOCK_SIZE ) / WINDOW_HEIGHT }
	};
	Vertex<2> oTexCoordTopMiddleBorderData[4] = {
		oTexCoordMiddleTopBorder[2],
		oTexCoordMiddleTopBorder[3],
		oTexCoordMiddleTopBorder[1],
		oTexCoordMiddleTopBorder[0]
	};
	mWindow[7]->updateTexCoords( oTexCoordTopMiddleBorderData );
	
	// Top Right Corner
	mWindow[8] = new Square( QVector2D( 20.0, 20.0 ), iShaderProgram );
	GLfloat oTexCoordTopRightCorner[4][2] = {
		{ ( ( 2.0f * BIG_OFFSET ) + ( 2.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 3.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 2.0f * BIG_OFFSET ) + ( 2.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 2.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 2.0f * BIG_OFFSET ) + ( 3.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 2.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 2.0f * BIG_OFFSET ) + ( 3.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 3.0f * BLOCK_SIZE ) / WINDOW_HEIGHT }
	};
	Vertex<2> oTexCoordTopRightCornerData[4] = {
		oTexCoordTopRightCorner[2],
		oTexCoordTopRightCorner[3],
		oTexCoordTopRightCorner[1],
		oTexCoordTopRightCorner[0]
	};
	mWindow[8]->updateTexCoords( oTexCoordTopRightCornerData );
	
	// Middle Left Border
	mWindow[3] = new Square( QVector2D( 20.0, iSize.y() - 40.0 ), ShaderManager::getInstance().getShader( "border_y" ) );
	GLfloat oTexCoordMiddleLeftBorder[4][2] = {
		{ ( ( 0.0f * BIG_OFFSET ) + ( 0.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 2.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 0.0f * BIG_OFFSET ) + ( 0.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 1.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 0.0f * BIG_OFFSET ) + ( 1.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 1.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 0.0f * BIG_OFFSET ) + ( 1.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 2.0f * BLOCK_SIZE ) / WINDOW_HEIGHT }
	};
	Vertex<2> oTexCoordMiddleLeftBorderData[4] = {
		oTexCoordMiddleLeftBorder[2],
		oTexCoordMiddleLeftBorder[3],
		oTexCoordMiddleLeftBorder[1],
		oTexCoordMiddleLeftBorder[0]
	};
	mWindow[3]->updateTexCoords( oTexCoordMiddleLeftBorderData );
	
	// Middle Background Texture
	mWindow[4] = new Square( QVector2D( iSize.x() - 40.0, iSize.y() - 40.0 ), iShaderProgram );
	
	// Middle Right Border
	mWindow[5] = new Square( QVector2D( 20.0, iSize.y() - 40.0 ), ShaderManager::getInstance().getShader( "border_y" ) );
	GLfloat oTexCoordMiddleRightBorder[4][2] = {
		{ ( ( 2.0f * BIG_OFFSET ) + ( 2.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 2.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 2.0f * BIG_OFFSET ) + ( 2.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 1.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 2.0f * BIG_OFFSET ) + ( 3.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 1.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 2.0f * BIG_OFFSET ) + ( 3.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 2.0f * BLOCK_SIZE ) / WINDOW_HEIGHT }
	};
	Vertex<2> oTexCoordMiddleRightBorderData[4] = {
		oTexCoordMiddleRightBorder[2],
		oTexCoordMiddleRightBorder[3],
		oTexCoordMiddleRightBorder[1],
		oTexCoordMiddleRightBorder[0]
	};
	mWindow[5]->updateTexCoords( oTexCoordMiddleRightBorderData );
	
	// Bottom Left Corner
	mWindow[0] = new Square( QVector2D( 20, 20 ), iShaderProgram );
	GLfloat oTexCoordBottomLeftCorner[4][2] = {
		{ ( ( 0.0f * BIG_OFFSET ) + ( 0.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 1.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 0.0f * BIG_OFFSET ) + ( 0.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 0.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 0.0f * BIG_OFFSET ) + ( 1.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 0.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 0.0f * BIG_OFFSET ) + ( 1.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 1.0f * BLOCK_SIZE ) / WINDOW_HEIGHT }
	};
	Vertex<2> oTexCoordButtomLeftCornerData[4] = {
		oTexCoordBottomLeftCorner[2],
		oTexCoordBottomLeftCorner[3],
		oTexCoordBottomLeftCorner[1],
		oTexCoordBottomLeftCorner[0]
	};
	mWindow[0]->updateTexCoords( oTexCoordButtomLeftCornerData );
	
	// Top Middle Border
	mWindow[1] = new Square( QVector2D( iSize.x() - 40.0f, 20.0 ), ShaderManager::getInstance().getShader( "border_x" ) );
	GLfloat oTexCoordBottomMiddleBorder[4][2] = {
		{ ( ( 1.0f * BIG_OFFSET ) + ( 1.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 1.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 1.0f * BIG_OFFSET ) + ( 1.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 0.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 1.0f * BIG_OFFSET ) + ( 2.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 0.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 1.0f * BIG_OFFSET ) + ( 2.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 1.0f * BLOCK_SIZE ) / WINDOW_HEIGHT }
	};
	Vertex<2> oTexCoordBottomMiddleBorderData[4] = {
		oTexCoordBottomMiddleBorder[2],
		oTexCoordBottomMiddleBorder[3],
		oTexCoordBottomMiddleBorder[1],
		oTexCoordBottomMiddleBorder[0]
	};
	mWindow[1]->updateTexCoords( oTexCoordBottomMiddleBorderData );
	
	// Bottom Right Corner
	mWindow[2] = new Square( QVector2D( 20.0, 20.0 ), iShaderProgram );
	GLfloat oTexCoordBottomRightCorner[4][2] = {
		{ ( ( 2.0f * BIG_OFFSET ) + ( 2.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 1.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 2.0f * BIG_OFFSET ) + ( 2.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 0.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 2.0f * BIG_OFFSET ) + ( 3.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 0.0f * BLOCK_SIZE ) / WINDOW_HEIGHT },
		{ ( ( 2.0f * BIG_OFFSET ) + ( 3.0f * BLOCK_SIZE ) ) / WINDOW_WIDTH, ( 1.0f * BLOCK_SIZE ) / WINDOW_HEIGHT }
	};
	Vertex<2> oTexCoordBottomRightCornerData[4] = {
		oTexCoordBottomRightCorner[2],
		oTexCoordBottomRightCorner[3],
		oTexCoordBottomRightCorner[1],
		oTexCoordBottomRightCorner[0]
	};
	mWindow[2]->updateTexCoords( oTexCoordBottomRightCornerData );
	
	setSize( QVector2D( iSize.x(), iSize.y() ) );
}

rpnoc::Panel::~Panel( void )
{
        delete mWindow[0];
        delete mWindow[1];
        delete mWindow[2];
        delete mWindow[3];
        delete mWindow[4];
        delete mWindow[5];
        delete mWindow[6];
        delete mWindow[7];
        delete mWindow[8];
}

void rpnoc::Panel::draw( const QMatrix4x4& iProjection, const QMatrix4x4& iView, QMatrix4x4 &iModel )
{
	QMatrix4x4 oModelOffest = iModel;
	
	TextureManager::getInstance().bindTexture( mTexBorder );
	
	// Top Left Corner
	mWindow[0]->draw( iProjection, iView, oModelOffest );
	
	oModelOffest.translate( 20.0, 0.0, 0.0 );
	// Top Middle Border
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	mWindow[1]->addUniform( QString( "in_Scale" ), ( mSize.x() / 50.0f ) );
	mWindow[1]->draw( iProjection, iView, oModelOffest );
	
	oModelOffest.translate( mSize.x() - 40.0, 0.0, 0.0 );
	// Top Right Corner
	mWindow[2]->draw( iProjection, iView, oModelOffest );
	
	oModelOffest = iModel;
	oModelOffest.translate( 0.0, 20.0, 0.0 );
	
	// Middle Left Border
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	mWindow[3]->addUniform( QString( "in_Scale" ), ( mSize.y() / 50.0f ) );
	mWindow[3]->draw( iProjection, iView, oModelOffest );
	
	
	// Background
	oModelOffest.translate( 20.0, 0.0, 0.0 );
	TextureManager::getInstance().bindTexture( mTexBackground );
	mWindow[4]->draw( iProjection, iView, oModelOffest );
	
	
	oModelOffest.translate( mSize.x() - 40.0, 0.0, 0.0 );
	TextureManager::getInstance().bindTexture( mTexBorder );
	
	// Middle Right Border
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	mWindow[5]->addUniform( QString( "in_Scale" ), ( mSize.y() / 50.0f ) );
	mWindow[5]->draw( iProjection, iView, oModelOffest );
	
	oModelOffest = iModel;
	oModelOffest.translate( 0.0, mSize.y() - 20.0, 0.0 );
	
	// Bottom Left Corner
	mWindow[6]->draw( iProjection, iView, oModelOffest );
	
	oModelOffest.translate( 20.0, 0.0, 0.0 );
	
	// Bottom Middle Border
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	mWindow[7]->addUniform( QString( "in_Scale" ), ( mSize.x() / 50.0f ) );
	mWindow[7]->draw( iProjection, iView, oModelOffest );
	
	oModelOffest.translate( mSize.x() - 40.0, 0.0, 0.0 );
	
	// Bottom Right Corner
	mWindow[8]->draw( iProjection, iView, oModelOffest );
}

void rpnoc::Panel::setBackground( QString iBackground )
{
	mTexBackground = iBackground;
}

void rpnoc::Panel::setBorder( QString iBorder )
{
	mTexBorder = iBorder;
}

void rpnoc::Panel::setColor( Vertex< 4 > iVertices [4 ] )
{
    for( int oWindow = 0; oWindow < 9; ++oWindow ) {
        mWindow[oWindow]->updateColors( iVertices );
    }
}

QString rpnoc::Panel::getBackground( void ) const
{
	return mTexBackground;
}

QString rpnoc::Panel::getBorder( void ) const
{
	return mTexBorder;
}

void rpnoc::Panel::setSize( QVector2D iSize )
{
	mSize = iSize;
	
	// Background
	GLfloat oBackground[4][3] = {
		{ mSize.x() - 40.0, 0.0, 0.0 },
		{ mSize.x() - 40.0, mSize.y() - 40.0, 0.0 },
		{ 0.0, 0.0, 0.0 },
		{ 0.0, mSize.y() - 40.0, 0.0 }
	};
	Vertex<3> oBackgroundData[4] = {
		oBackground[0],
		oBackground[1],
		oBackground[2],
		oBackground[3]
	};
	mWindow[4]->updateVertices( oBackgroundData );
	
	// Top Middle & Bottom Middle-Border
	GLfloat oHorizontal[4][3] = {
		{ mSize.x() - 40.0, 0.0, 0.0 },
		{ mSize.x() - 40.0, 20.0, 0.0 },
		{ 0.0, 0.0, 0.0 },
		{ 0.0, 20.0, 0.0 }
	};
	Vertex<3> oHorizontalData[4] = {
		oHorizontal[0],
		oHorizontal[1],
		oHorizontal[2],
		oHorizontal[3]
	};
	mWindow[1]->updateVertices( oHorizontalData );
	mWindow[7]->updateVertices( oHorizontalData );
	
	// Middle Left & Right-Border
	GLfloat oVertical[4][3] = {
		{ 20.0, 0.0, 0.0 },
		{ 20.0, mSize.y() - 40.0, 0.0 },
		{ 0.0, 0.0, 0.0 },
		{ 0.0, mSize.y() - 40.0, 0.0 }
	};
	Vertex<3> oVerticalData[4] = {
		oVertical[0],
		oVertical[1],
		oVertical[2],
		oVertical[3]
	};
	mWindow[3]->updateVertices( oVerticalData );
	mWindow[5]->updateVertices( oVerticalData );
}
