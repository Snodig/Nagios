#include "src/opengl/drawables/drawablesourcedefault.hpp"
#include "src/opengl/shapes/format.hpp"
#include "src/opengl/shapes/cell.hpp"

rpnoc::DrawableSourceDefault::DrawableSourceDefault( float windowSize, QWidget *parent )
	: DrawableSource( "DEFAULT", "NONE", windowSize, parent )
{
	//
}

rpnoc::DrawableSourceDefault::~DrawableSourceDefault( void )
{
	//
}

void rpnoc::DrawableSourceDefault::init( void )
{
	DrawableSource::init();
}

void rpnoc::DrawableSourceDefault::draw()
{
	TextFormat oHeader1Format(
		QString( "charset.png" ),
		1240,
		1,
		-10,
		60,
		60,
		QVector4D( 1.0f, 0.0f, 0.0f, 1.0f )
	);
	
	TextCell oStatus(
		QVector2D( mFrameBuffer->size().width(), mFrameBuffer->size().height() ),
		QVector2D( 20.0f, 20.0f ),
		oHeader1Format,
		QString( "NOTHING SELECTED" )
	);

	oStatus.draw( mProjection, mView, mModel );
}
