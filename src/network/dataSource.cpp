#include "dataSource.hpp"

rpnoc::DataSource::DataSource(	QString datatype, QString serverIP, quint16 serverPort,
								QObject* parent )
 : QObject( parent )
{
    mName = datatype;
    mIP = serverIP;
    mPort = serverPort;
    mRunning = false;
}

rpnoc::DataSource::~DataSource()
{
	//
}

void rpnoc::DataSource::setIP( QString IP )
{
	mIP = IP;
}

void rpnoc::DataSource::setName( QString name )
{
	mName = name;
}

void rpnoc::DataSource::setPort( quint16 port )
{
	mPort = port;
}

QString rpnoc::DataSource::getIP() const
{
    return mIP;
}

QString rpnoc::DataSource::getName() const
{
	return mName;
}

quint16 rpnoc::DataSource::getPort() const
{
    return mPort;
}

bool rpnoc::DataSource::isRunning() const
{
	return mRunning;
}

QString rpnoc::DataSource::toString() const
{
	return ( mIP + ":" + QString::number( mPort ) + ":" + mName );
}

int rpnoc::DataSource::getTimeSinceLastUpdate()
{
	if( mLastUpdateTime.isNull() ){
		return -1;
	}else{
		return mLastUpdateTime.elapsed();
	}
}

void rpnoc::DataSource::setServerData( QString data )
{
    QByteArray byteData;
	byteData.append( data );
	setServerData( byteData );
}

void rpnoc::DataSource::setServerData( QByteArray data )
{
    mServerData = data;
    parseServerData();
	mLastUpdateTime.start();
}
