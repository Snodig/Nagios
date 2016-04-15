/*
 *
 */

#include "src/opengl/shapes/panel.hpp"
#include "src/opengl/shapes/format.hpp"
#include "src/utils/texture.hpp"
#include "cell.hpp"

rpnoc::Cell::Cell( QVector2D iSize, QVector2D iPadding,
			QString iImageBackground, QString iImageBorder )
	:	mSize( iSize ), mPadding( iPadding )
{
	mPanel = new Panel( iSize, ShaderManager::getInstance().getShader( "default" ), iImageBackground, iImageBorder );
}

rpnoc::Cell::~Cell( void )
{
	delete mPanel;
}

QVector2D rpnoc::Cell::getSize( void ) const
{
	return mSize;
}

QVector4D rpnoc::Cell::getBackgroundColor( void ) const
{
	return mBackgroundColor;
}

void rpnoc::Cell::setPanelBackground( QString iBackground )
{
	mPanel->setBackground( iBackground );
	
}

void rpnoc::Cell::setPanelBorder( QString iBorder )
{
	mPanel->setBorder( iBorder );
}

void rpnoc::Cell::setBackGroundColor( QVector4D iColor )
{
	mBackgroundColor = iColor;
    GLfloat color[4] = { iColor.x(), iColor.y(), iColor.z(), iColor.w() };
    Vertex< 4 > oVertex[4] = { color, color, color, color };

    mPanel->setColor( oVertex );
}

rpnoc::TextCell::TextCell( QVector2D iSize, QVector2D iPadding, TextFormat iTextFormat,
						   QString iText )
	: Cell( iSize, iPadding ), mTextFormat( iTextFormat ), mText( iText )
{
	if( iText.size() <= 0 ) {
		iText = "NO TEXT ENTERED";
	}

	mTextFormat.setColumns( 
		(int)( mSize.x() - ( mPadding.x() * 2 ) ) / 
		(int)( mTextFormat.getCharWidth() + mTextFormat.getSpacing() ) 
	);
	
	mTextFormat.setRows(
		(int)( mSize.y() - ( mPadding.y() * 2 ) ) /
		(int)( mTextFormat.getCharHeight() + mTextFormat.getSpacing() )
	);
	
	if( iText.length() < mTextFormat.getRows() * mTextFormat.getColumns() ) {
		if( iText.length() < mTextFormat.getColumns() ) {
			mTextFormat.setColumns( iText.length() );
		}
		
		mTextFormat.setRows( iText.length() / mTextFormat.getColumns() + 1 );
	}
}

rpnoc::TextCell::~TextCell( void )
{
	//
}

void rpnoc::TextCell::draw( const QMatrix4x4& iProjection, const QMatrix4x4& iView, QMatrix4x4 iModel )
{
	mPanel->draw( iProjection, iView, iModel );
	
	float oCentralX = ( 
		mSize.x() - mPadding.x() - 30.0f -
		( mTextFormat.getColumns() * ( mTextFormat.getCharWidth() + mTextFormat.getSpacing() ) )
	) / 2;
	float oCentralY = (
		mSize.y() - mPadding.y() - 30.0f -
		( mTextFormat.getRows() * ( mTextFormat.getCharHeight() + mTextFormat.getSpacing() ) )
	) / 2;
	iModel.translate( mPadding.x() + oCentralX, mPadding.y() + oCentralY, 0.0f );
	
	drawText( mText, mTextFormat, iProjection, iView, iModel );
}

void rpnoc::TextCell::resize( QVector2D iSize )
{
	mSize = iSize;

	mTextFormat.setColumns( 
		(int)( mSize.x() - ( mPadding.x() * 2 ) ) / 
		(int)( mTextFormat.getCharWidth() + mTextFormat.getSpacing() ) 
	);

	mTextFormat.setRows(
		(int)( mSize.y() - ( mPadding.y() * 2 ) ) /
		(int)( mTextFormat.getCharHeight() + mTextFormat.getSpacing() )
	);
	
	mPanel->setSize( mSize );
}

void rpnoc::TextCell::setText( QString iText )
{
	mText = iText;

	if( mText.isEmpty() ) {
		mText = "NO TEXT ENTERED";
	}
	
	mTextFormat.setColumns( 
		(int)( mSize.x() - ( mPadding.x() * 2 ) ) / 
		(int)( mTextFormat.getCharWidth() + mTextFormat.getSpacing() ) 
	);
	
	mTextFormat.setRows(
		(int)( mSize.y() - ( mPadding.y() * 2 ) ) /
		(int)( mTextFormat.getCharHeight() + mTextFormat.getSpacing() )
	);
	
	if( iText.length() < mTextFormat.getRows() * mTextFormat.getColumns() ) {
		if( iText.length() < mTextFormat.getColumns() ) {
			mTextFormat.setColumns( iText.length() );
		}
		
		/*
		 * ???: removed the +1 from mTextFormat.getColumns(), not sure if
		 * this might break something.
		 */
		mTextFormat.setRows( iText.length() / mTextFormat.getColumns() );
	}
}

QString rpnoc::TextCell::getText( void ) const
{
	return mText;
}

rpnoc::ImageCell::ImageCell( QVector2D iSize, QString iName )
	: Cell( iSize, QVector2D( 20.0f, 20.0f ) ), mName( iName )
{
	mSquare = new Square( iSize - ( mPadding + mPadding ), ShaderManager::getInstance().getShader( "default" ) );
}

rpnoc::ImageCell::~ImageCell( void )
{
	delete mSquare;
}

void rpnoc::ImageCell::resize( QVector2D iSize )
{
	mSize = iSize;
	mPanel->setSize( mSize );
}

void rpnoc::ImageCell::draw( const QMatrix4x4& iProjection, const QMatrix4x4& iView, QMatrix4x4 iModel )
{
	mPanel->draw( iProjection, iView, iModel );
	iModel.translate( mPadding.x(), mPadding.y(), 0.0 );
	
	TextureManager::getInstance().bindTexture( mName );
	mSquare->draw( iProjection, iView, iModel );
}
