/*
 *
 */

#include "glmessage.hpp"


GLMessage::GLMessage( QString iMessage, MessageLevel iLevel ) : mLifespan( 100.0f ),
	mLevel( iLevel )
{
	QVector2D oMaxSize( 400, 275 );
	QVector4D oColor( 1.0f, 1.0f, 1.0f, 1.0f );
	switch( iLevel ) {
		case MESSAGE_ERROR :
			oColor = QVector4D( 1.0f, 0.0f, 0.0f, 1.0f );
			break;
		case MESSAGE_CRITICAL :
			oColor = QVector4D( 0.9f, 0.3f, 0.3f, 1.0f );
			break;
		case MESSAGE_WARNING :
			oColor = QVector4D( 1.0f, 1.0f, 0.0f, 1.0f );
			break;
		case MESSAGE_INFO :
			oColor = QVector4D( 1.0f, 1.0f, 1.0f, 1.0f );
			break;
	}
	
	TextFormat oFormat( QString( "charset.png" ), 22, 22, -10, 15, 18 );
	oFormat.setColor( QVector4D( 0.0f, 0.0f, 1.0f, 1.0f ) );
	QVector2D oSize( 400, oFormat.getSpacing() + oFormat.getColumns() + 60.0f );
	mTextCell = new TextCell( oSize, QVector2D( 20, 20 ), oFormat, iMessage );
	mTextCell->resize( oSize );
	mTextCell->setText( iMessage );
	mTextCell->setBackGroundColor( oColor );
	mTextCell->setPanelBackground( "backgrounds/white.png" );
	mTextCell->setPanelBorder( "borders/round_corners.png" );
}

GLMessage::~GLMessage( void )
{
	delete mTextCell;
}

float GLMessage::getLife( void ) const
{
	return mLifespan;
}

GLMessage::MessageLevel GLMessage::getLevel( void ) const
{
	return mLevel;
}

QVector2D GLMessage::getSize( void ) const
{
	return mTextCell->getSize();
}

void GLMessage::draw( QMatrix4x4 &iProjection, QMatrix4x4 &iView, QMatrix4x4 &iModel )
{
	mLifespan -= 0.4f;
	if( mLifespan < 50.0f ) {
		QVector4D oNewColor( mTextCell->getBackgroundColor() );
		oNewColor.setW( oNewColor.w() - 0.007f );
		mTextCell->setBackGroundColor( oNewColor );
	}
	mTextCell->draw( iProjection, iView, iModel );
}


GLMessageHandler::GLMessageHandler( void )
{
	//
}


GLMessageHandler::~GLMessageHandler( void )
{
	//
}

void GLMessageHandler::draw( QMatrix4x4 &iProjection, QMatrix4x4 &iView, QMatrix4x4 &iModel )
{
	/*
	 * go through list and see if any messages should be
	 * deleted, and paint them.
	 */
	QList< GLMessage* >::iterator oItrMessage = mMessages.begin();
	while( oItrMessage != mMessages.end() ) {
		if( (*oItrMessage)->getLife() < 0.0f ) {
			oItrMessage = mMessages.erase( oItrMessage );
		} else {
			(*oItrMessage)->draw( iProjection, iView, iModel );
			iView.translate( 0.0f, -(*oItrMessage)->getSize().y() - 5.0f, 0.0f );
			++oItrMessage;
		}
	}
}

void GLMessageHandler::slotAddMessage( QString iMessage, GLMessage::MessageLevel iLevel )
{
	mMessages.append( new GLMessage( iMessage, iLevel ) );
}
