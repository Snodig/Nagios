/**
  * The Networker-class is responsible for all network-activites, including reacting to the content of packets
  * Also holds a list of all our DataSource-pointers in a static list (sDataSources)
  */


#include "src/network/networker.hpp"
#include "src/utils/debug.hpp"
#include "src/network/profile.hpp"

QList< DataSource* > Networker::sDataSources;

rpnoc::Networker::Networker( QWidget* iParent )
{
    mSocket = new QTcpSocket( this );

    mTimeoutTimer = new QTimer( this );
    mSendPingTimer = new QTimer( this );

    connect( mSocket, SIGNAL( readyRead() ), this, SLOT( read_packet() ) );
    connect( mTimeoutTimer, SIGNAL( timeout() ), this, SLOT( disconnect() ) );
    connect( mSendPingTimer, SIGNAL ( timeout() ), this, SLOT( ping_connection() ) );

    mBytesToRead = 0;
    mTotalBytesToRead = 0;
}

rpnoc::Networker::~Networker( void )
{
	//
}

/**
  * Return a pointer to the DataSource-instance of name iName
  */
DataSource* rpnoc::Networker::getDataSource( QString iName )
{
	QList< DataSource* >::iterator oItrDataSource = sDataSources.begin();
	while( oItrDataSource != sDataSources.end() ) {
		if( (*oItrDataSource)->getName() == iName ) {
			return (*oItrDataSource);
		}
		++oItrDataSource;
	}
	
	return NULL;
}

void rpnoc::Networker::addDataSource( DataSource* iDataSource )
{
	sDataSources.append( iDataSource );
}

/**
  * Prepends the stringlength-portion of the header to a packet and sends it to the server
  */
void rpnoc::Networker::sendPacket( QByteArray oPacket ) const
{
    if( mSocket->state() != QAbstractSocket::ConnectedState ) {
        mSocket->waitForConnected();
    }

    if( mSocket->state() == QAbstractSocket::ConnectedState ) {
        QByteArray oStrlen;
        oStrlen.append( QString::number( oPacket.length() + 1 ) + ":" );

        oPacket.prepend( oStrlen );
        mSocket->write( oPacket );
    } else {
		log_e( "Attempted to send packey while not connected to server" );
    }
}

/**
  * Prepends the stringlength-portion of the header to a packet and sends it to the server
  */
void rpnoc::Networker::sendPacket( QString iPacket ) const
{
    QByteArray oBytePacket;
    oBytePacket.append( iPacket );
    sendPacket( oBytePacket );
}

/**
  * Unused slot, meant to receive status-messages from any owned modules(e.g DataSources)
  * and emit a new signal to any who may find it useful(e.g a status-window or logger)
  */
void rpnoc::Networker::on_status_message( QString iMessage, QString iSource )
{
    emit status_message( iMessage, iSource );
}

/**
  * Slot called when the login-widget finishes, attempts to connect to the server defined, using the credentials entered
  */
void rpnoc::Networker::on_login_attempt( QStringList iCredentials )
{
	QString oUser = iCredentials.at( 0 );
	QString oPassword = iCredentials.at( 1 );
	QString oServerIP = iCredentials.at( 2 );
	QString oServerport = iCredentials.at( 3 );

	//Attempts to connect
    if( mSocket->state() == QAbstractSocket::UnconnectedState ) {
        mSocket->connectToHost( oServerIP, oServerport.toUInt() );

    } else {
        log_w( "Connection already established" );
        emit status_message(  "Connection already established", "Networker" );
    }

	//In case there's a delay, we wait for the client to connect properly
    if( mSocket->state() == QAbstractSocket::ConnectingState || mSocket->state() == QAbstractSocket::UnconnectedState ) {
		mSocket->waitForConnected( 5000 );
		log_i( "Attempting to connect to server" );
        emit status_message(  "Attempting to connect to server", "Networker" );
    }

	//We try to reconnect five times (7 total) if we're unable to reach the host on either attempts
    int oCounter = 0;
    while( oCounter < 5 ){
        mSocket->waitForConnected( 3000 );
        if( mSocket->state() == QAbstractSocket::ConnectedState ) {
			log_i( "Connection established, verifying login" );
            emit status_message(  "Connection established, verifying login.", "Networker" );
            break;
        }else{
            emit status_message( QString( "Retrying connection attempt " + QString::number( ++oCounter ) ), "Networker" );
			log_w( "Retrying connecting attempt: " + QString::number( oCounter ) );
			mSocket->connectToHost( oServerIP, oServerport.toUInt() );
        }
    }

	//A simple encryption of password, just added to conceal password in logs and such
    QString oTempPW = oPassword;
    QString oEncrypted;
    for( int i = 0; i < oTempPW.length(); ++i ) {
        char tempchar = static_cast<char>( oTempPW[i].toAscii() );
        tempchar += 995511;
        oEncrypted.append( tempchar );
    }

	/**
	  * At this point, we expect to be connected, and attempt to send off our login-credentials to be recognized by the server
	  */
    QByteArray oPacket;
    oPacket.append( QString::number( rpnoc::COMMUNICATOR ) + ":" +
		QString::number( rpnoc::CONNECT ) + ":" + oUser + ":" + oEncrypted);
    
    mSocket->waitForConnected( 5000 );
    if( mSocket->state() == QAbstractSocket::ConnectedState ) {
        sendPacket( oPacket );
    }else{
        emit status_message(  "Unable to establish connection!", "Networker" );
		log_e( "Unable to establish connection!" );
    }
}

/**
  * Reads the first portion of a packets header, returning the packets length 
  * (as well as setting the member mTotalBytesToRead to the same value)
  */
int rpnoc::Networker::getPacketLengthFromHeader( void )
{
    for( int i = 0; i < mPacket.byteData.length(); ++i ){
        if( mPacket.byteData.at( i ) == ':' ){
            QByteArray oTemp = mPacket.byteData;
            oTemp.remove( i, oTemp.length() );
            int oRetval = oTemp.toInt();

            mPacket.byteData.remove( 0, i+1 );
            mTotalBytesToRead = oRetval;
            
            return oRetval;
        }
    }

    return -1;
}

/**
  * Entered if more than one packet is available simultaneously on the socket, reading and processing them one by one
  */
void rpnoc::Networker::readMultiPacket( void )
{
    while( mBytesToRead != 0 && mPacket.byteData.length() != 0 ){
        RawData tempPacket;
		tempPacket.byteData.append( mPacket.byteData, mTotalBytesToRead - 1 );

        handlepacket( tempPacket );

        mPacket.byteData.remove( 0, mTotalBytesToRead - 1 );
        mBytesToRead = getPacketLengthFromHeader();
    }
}

/**
  * Resets variables we use to read packets (used when we're done reading off the socket)
  */
void rpnoc::Networker::resetPacketHeader( void )
{
    mTotalBytesToRead = 0;
    mBytesToRead = 0;
    mPacket.byteData.clear();
    mPacket.subtype = 0;
    mPacket.type = 0;
}

/**
  * Triggered when data is available on the socket (signal readyRead)
  * This will triggered several times if a packet is larger than what QTcpSocket lets us buffer each time
  */
void rpnoc::Networker::read_packet( void ) {
    if( mSocket->bytesAvailable() > 0 ) {

		//Reads off bytes available now
        int bytesAvailable = mSocket->bytesAvailable();
        mPacket.byteData.append( mSocket->readAll() );

		//Reads the header, if not read in an earlier iteration
        if( mBytesToRead == 0 ) {
            mBytesToRead = getPacketLengthFromHeader();
            bytesAvailable -= QString::number( mBytesToRead ).length();
        }

		//Implies we have more than one packet available on the socket simultaneously
        if( mBytesToRead < bytesAvailable ) {
            readMultiPacket();
            resetPacketHeader();
            return;
        }

        mBytesToRead -= bytesAvailable;

		//We're done, there's no more data inbound for this (single) packet
        if( mBytesToRead == 0 ) {
            handlepacket( mPacket );
            resetPacketHeader();
            return;
        }else{
			//There's more data coming, we wait for the signal to trigger this method again
            return;
        }
    }else{
		//Failsafe, if bytesavailable is <= 0
        resetPacketHeader();
        return;
    }

	//Failsafe
    resetPacketHeader();
    return;
}

/**
  * Function is called when a packet is ready for processing, and is handled according to type and subtype defined in its header
  */
void rpnoc::Networker::handlepacket( RawData packet )
{
	//Read off the base-type, and move it to the type-variable
    packet.type = static_cast<QString>( packet.byteData.at( 0 ) ).toUInt();
    packet.byteData.remove( 0, 2 );

	//Implies a system-message
    if( packet.type == 1 ) {

		//Read off the sub-type, and move it to the subtype-variable
        packet.subtype = static_cast<QString>( packet.byteData.at( 0 ) ).toUInt();
        packet.byteData.remove( 0, 2 );

		QString oSource;
		QString oIP;

        //Switch to find out what type of message this was
        switch( static_cast<rpnoc::CommunicatorType>( packet.subtype ) ) {
			case rpnoc::PING:
				//Keep alive message
				log_i( "Me <- Pong! <- Server" );
				mTimeoutTimer->start( 20000 );
				return;
				
			case rpnoc::CONNECT:
				//Login-verification
				if( packet.byteData.contains( "OK" ) ) {
					mSendPingTimer->start( 10000 );
					
					log_i( "Login successful" );
					emit status_message(  "Login successful!", "Networker" );
					subscribeToAll();
	
				} else {
					log_i( QString( "Login reply from server" + packet.byteData + " ( != OK )" ) );
					emit status_message( QString( "Login reply from server" +
						packet.byteData + " ( != OK )" ), "Networker");
						
					disconnect();
				}
				break;
				
			case rpnoc::SUBSCRIBE:
				//Subscription-verification
				emit status_message( QString("Subscription verified: " +
					packet.byteData), "Networker" );
	
				oSource = packet.byteData.split( ':' ).at( 0 );
				oIP = packet.byteData.split( ':' ).at( 1 );
				log_i( "Subscription verified: " + oSource );
				
				for( int i = 0; i < sDataSources.length(); ++i ) {
					if( sDataSources.at( i )->getName() == oSource ) {
						sDataSources.at( i )->setIP( oIP );
					}
				}
				break;
				
			case rpnoc::UNSUBSCRIBE:
				//Unsubscription-verification
				emit status_message( QString("Unubscription verified: " +
					packet.byteData), "Networker" );
				log_i( "Unsubscription verified: " + packet.byteData );
				break;
				
			case rpnoc::SOURCE_UNAVAILABLE:
				//Invalid subscription, or service offline
				log_w( "Datasource unavailable: " + packet.byteData );
				emit status_message( QString("Datasource unavailable: "
					+ packet.byteData), "Networker");
				break;
	
			case rpnoc::NO_DATA:
				//No data available for selected service
				log_w( "No data available: " + packet.byteData );
				emit status_message( QString("No data available: "
					+ packet.byteData), "Networker");
				break;
				
			default:
				break;
		}
        return; //Was Communication, not data, so we return after handling
    }

	//Read what service this data was from (e.g Nagios)
    QString oSource;
    for( int i = 0; i < packet.byteData.length(); ++i ) {
        if( packet.byteData.at( i ) == ':' ){
            QString oTemp = packet.byteData;
            oTemp.remove( i, oTemp.length() );
            packet.byteData.remove( 0, i+1 );
            oSource = oTemp;
            break;
        }
    }

	//Update the data in our DataSource-instance by that name
    for( int i = 0; i < sDataSources.length(); ++i ) {
        if( sDataSources.at( i )->getName() == oSource ){
            sDataSources.at( i )->setServerData( packet.byteData );

			log_i( "Updated datasource: " + oSource );
            emit status_message( QString( "Updated datasource: " + oSource ), "Networker" );
            break;
        }
    }
}

/**
  * Disconnects the socket
  */
void rpnoc::Networker::disconnect( void )
{
    mSocket->disconnectFromHost();
    log_i( "Disconnected from host" );
	emit status_message( "Disconnected.", "Networker" );
}

/**
  * Sends a keep-alive packet (automated)
  */
void rpnoc::Networker::ping_connection( void )
{
    QByteArray oPacket;
    oPacket.append( QString::number( rpnoc::COMMUNICATOR ) + ":"
		+ QString::number( rpnoc::PING ) );

	log_i( "Me -> Ping! -> Server" );
    sendPacket( oPacket );
    mSendPingTimer->start( 10000 );
}

/**
  * If no keep-alive signal is received for a period of time, this slot automatically disconnects us
  */
void rpnoc::Networker::on_ping_timeout( void )
{
	log_i( "Timed out from server" );
    disconnect();
}

/**
  * Updates the list of subscriptions we've attempted (is saved to the profile-xml)
  */
void rpnoc::Networker::appendSubscriptions( QStringList iSubscriptions ) {
	mAttemptedSubscriptions << iSubscriptions;
}

/**
  * Attempts to subscribe to all services read from xml or attempted this session
  */
void rpnoc::Networker::subscribeToAll( void )
{
	log_i( "Subscribing to everything" );
	for( int i = 0; i < mAttemptedSubscriptions.length(); ++i ) {
		if( mAttemptedSubscriptions.at( i ) != "NONE" &&
			mAttemptedSubscriptions.at( i ).length() != 0 ) {
			subscribe( mAttemptedSubscriptions.at( i ) );
		}
	}
}

/**
  * Sends a subscribtion to service iType to the server
  */
void rpnoc::Networker::subscribe( QString iType )
{
	//To avoid subscribing to "", if a drawableSource is defined to use "" as its source(i.e it doesn't need one)
    if( iType.length() == 0 ) {
        return;
    }

	if( !mAttemptedSubscriptions.contains(iType) ) {
		mAttemptedSubscriptions << iType;
	}

    QByteArray oPacket;
    oPacket.append( QString::number( rpnoc::COMMUNICATOR ) + ":"
		+ QString::number( rpnoc::SUBSCRIBE ) + ":" + iType );

    if( mSocket->state() == QAbstractSocket::ConnectedState ){
        sendPacket( oPacket );
        for( int i = 0; i < sDataSources.length(); ++i ) {
            if( sDataSources.at( i )->getName() == iType ) {
                return; //already setup
            }
        }
    } else {
        log_w( "Subscription attempted without being connected" );
        emit status_message( QString( "Subscription for " + iType +
			" attempted while not connected!" ), "Networker" );
    }
}

/**
  * Sends a unsubscribtion to service iType to the server
  */
void rpnoc::Networker::unsubscribe( QString iType )
{
    QByteArray oPacket;
    oPacket.append( QString::number( rpnoc::COMMUNICATOR ) + ":"
		+ QString::number( rpnoc::UNSUBSCRIBE ) + ":" + iType );

    sendPacket( oPacket );
}

/**
  * Sets the profile we're using to iProfile
  */
void rpnoc::Networker::selectedProfile( QString iProfile )
{
	mProfile = iProfile;
}

/**
  * Returns the profile we're using
  */
QString rpnoc::Networker::getProfile( void ) const
{
	return mProfile;
}
