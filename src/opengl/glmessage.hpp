/*
 *
 */

#ifndef GLMESSAGE_HPP
#define GLMESSAGE_HPP

#include <QObject>
#include "src/opengl/shapes/cell.hpp"

using rpnoc::TextCell;


class GLMessage 
{
public:
	enum MessageLevel
	{
		MESSAGE_ERROR		= 0x00001,
		MESSAGE_WARNING		= 0x00010,
		MESSAGE_CRITICAL	= 0x00100,
		MESSAGE_INFO		= 0x01000
	};
						GLMessage( QString iMessage, MessageLevel iLevel );
						~GLMessage( void );
	
	float				getLife( void ) const;
	MessageLevel		getLevel( void ) const;
	QVector2D			getSize( void ) const;
	void				draw( QMatrix4x4& iProjection, QMatrix4x4& iView, QMatrix4x4& iModel );
	
private:
	TextCell*			mTextCell;
	float				mLifespan;
	MessageLevel		mLevel;
};

class GLMessageHandler : public QObject
{
	Q_OBJECT
public:
						GLMessageHandler( void );
						~GLMessageHandler( void );
	
	void				draw( QMatrix4x4& iProjection, QMatrix4x4& iView, QMatrix4x4& iModel );

public slots:
	void				slotAddMessage( QString iMessage, GLMessage::MessageLevel iLevel );
private:
	QList< GLMessage* >	mMessages;
};

#endif // GLMESSAGE_HPP
