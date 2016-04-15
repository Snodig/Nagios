/*
 *
 */

#include <QDateTime>
#include "drawablesourceclock.hpp"

DrawableSourceClock::DrawableSourceClock( float iWindowSize, QWidget* iParent )
	: DrawableSource( "CLOCK", "", iWindowSize, iParent ) {
	/*
	 * this class wont be using any datasources, will be using
	 * the Qt library to show the time.
	 */
	mDataSource = NULL;
}

DrawableSourceClock::~DrawableSourceClock( void ) {
	//
}

void DrawableSourceClock::draw( void ) {
	mTime->setText( QDateTime::currentDateTime().toString( "hh:mm:ss" ) );
	mTime->draw( mProjection, mView, mModel );
}

void DrawableSourceClock::init( void ) {
	DrawableSource::init();
	
	mTextFormat = new TextFormat(
		QString( "charset.png" ),
		16,
		1,
		-30,
		400,
		75,
		QVector4D( 1.0f, 1.0f, 0.0f, 1.0f )
	);
	
	mTime = new TextCell(
		QVector2D( mFrameBuffer->width(), mFrameBuffer->height() ),
		QVector2D( 0.0f, 0.0f ),
		*mTextFormat,
		QString( ";:." )
	); 	
	mTime->setPanelBorder( QString( "transparent.png" ) );
	mTime->setPanelBackground( QString( "transparent.png" ) );
}
