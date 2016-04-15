#include "nagiosSource.hpp"

#include <QtScript>

rpnoc::NagiosSource::NagiosSource( QObject *parent )
	: rpnoc::DataSource( "NAGIOS", "0.0.0.0", 0, parent )
{
    mOldParsedData = NULL;
    mParsedData = NULL;

	mCountOK = -1;
    mCountCritical = -1;
    mCountWarning = -1;
    mCountUnknown = -1;
}

rpnoc::NagiosSource::~NagiosSource()
{
	//
}

/**
  * The compare-method used to filter all items by descending durations
  */
int compareDurations( const void* jsonA, const void* jsonB )
{
	QString* a = *(QString**)jsonA;
	QString* b = *(QString**)jsonB;

	if( a[rpnoc::NagiosSource::DURATION].toInt() >= b[rpnoc::NagiosSource::DURATION].toInt() ){
		return true;
	}else{
		return false;
	}
}

int rpnoc::NagiosSource::getCountCritical( void ) const
{
	return mCountCritical;
}

int rpnoc::NagiosSource::getCountWarning( void ) const
{
	return mCountWarning;
}

int rpnoc::NagiosSource::getCountOK( void ) const
{
	return mCountOK;
}

int rpnoc::NagiosSource::getCountUnknown( void ) const
{
	return mCountUnknown;
}

int rpnoc::NagiosSource::getCountOfStatus( QString status ) const
{
	if( status == "CRITICAL" ){
		return mCountCritical;
	}else if( status == "WARNING" ){
		return mCountWarning;
	}else if( status == "OK" ){
		return mCountOK;
	}else if( status == "UNKNOWN" ){
		return mCountUnknown;
	}

	return 0;
}

QList< QString* > rpnoc::NagiosSource::getAllOfStatus( QString status ) const
{
	QList< QString*> retData;

	for( int i = 0; i < mParsedDataCount; ++i ){
		if( mParsedData[i][SERVER_STATUS] == status ){
			retData.append( mParsedData[i] );
		}
	}

	return retData;
}

QString** rpnoc::NagiosSource::getServerData( void ) const
{
	return mParsedData;
}

/**
  *	Parses the raw data-string to a format we can use ( a 2D-array of enum-accessible strings - mParsedData )
  */
void rpnoc::NagiosSource::parseServerData( void )
{
	mCountOK = 0;
    mCountCritical = 0;
    mCountWarning = 0;
	mCountUnknown = 0;

	//If we've receieved data from the server
	if( !mServerData.isEmpty() ){
		//If we have saved data already, copy it
		if( mParsedData != NULL ){
			//If we already have a copy, delete it
			if( mOldParsedData != NULL ){
                for( int i = 0; i < mOldParsedDataCount; ++i ){

                    delete []mOldParsedData[i];
                }
                delete []mOldParsedData;
            }
			//Transfer the counter
			mOldParsedDataCount = mParsedDataCount;

			//Allocate a fresh array for the copy and replace
            mOldParsedData = new QString*[mOldParsedDataCount];
            mOldParsedData = mParsedData;
        }

        QScriptEngine mScriptEngine;
        QString result = mServerData;
		QScriptValue sc;

		sc = mScriptEngine.evaluate(  "(" + result + ")", QString( "Log/nagiosError.log" ) );
		
		//Indicates a syntax-error within the script
		if( mScriptEngine.hasUncaughtException() ) {
                        qDebug() << mScriptEngine.uncaughtException().toString();
                        qDebug() << mServerData;
		}
		
        QScriptValueIterator itr( sc );
        mParsedDataCount = 0;

		//First, count the number of items
        while( itr.hasNext() ){

            itr.next();
            ++mParsedDataCount;
        }
		
		//Allocate the suited amount of memory
        mParsedData = new QString*[mParsedDataCount];

		//Reset the count, just to use as an iterator
		mParsedDataCount = 0;
        itr.toFront();

        while( itr.hasNext() ){
            itr.next();
            mParsedData[mParsedDataCount] = new QString[10];

			//Save the fields we want
			mParsedData[mParsedDataCount][rpnoc::NagiosSource::HOSTNAME] = itr.value().property("host").toString();
			mParsedData[mParsedDataCount][rpnoc::NagiosSource::HOSTGROUP] = itr.value().property("hostgroup").toString();
			mParsedData[mParsedDataCount][rpnoc::NagiosSource::SERVICE_DESCRIPTION] = itr.value().property("service").toString();
			mParsedData[mParsedDataCount][rpnoc::NagiosSource::DURATION] = itr.value().property("duration").toString();
			mParsedData[mParsedDataCount][rpnoc::NagiosSource::OUTPUT] = itr.value().property("plugin_output").toString();
			mParsedData[mParsedDataCount][rpnoc::NagiosSource::OWNERS] = itr.value().property("pri").toString() + " : "
                                    + itr.value().property("sec").toString();
			mParsedData[mParsedDataCount][rpnoc::NagiosSource::TEAM] = itr.value().property("team").toString();
			mParsedData[mParsedDataCount][rpnoc::NagiosSource::SERVER_STATUS] = itr.value().property("status").toString();
			mParsedData[mParsedDataCount][rpnoc::NagiosSource::HOST_STATUS] = itr.value().property("host_status").toString();

			//Setup a semi-unique checksum for this item
			QString checkstring = mParsedData[mParsedDataCount][rpnoc::NagiosSource::HOSTNAME] +
								  mParsedData[mParsedDataCount][rpnoc::NagiosSource::HOSTGROUP] +
								  mParsedData[mParsedDataCount][rpnoc::NagiosSource::SERVICE_DESCRIPTION];

			quint16 checksum = qChecksum( checkstring.toStdString().c_str(),
										  qstrlen( checkstring.toStdString().c_str() ) );
			mParsedData[mParsedDataCount][rpnoc::NagiosSource::OBJECT_ID] = QString::number( checksum );
            
            QString status = mParsedData[mParsedDataCount][SERVER_STATUS];
            
			//Counters for our conveniance-methods
            if( status == "CRITICAL" ){
				++mCountCritical;
			}else if( status == "WARNING" ){
				++mCountWarning;
			}else if( status == "OK" ){
				++mCountOK;
			}else if( status == "UNKNOWN" ){
				++mCountUnknown;
            }

            ++mParsedDataCount;
		}

		//Finally, sort all items by duration (descending)
		qsort( (QString*)mParsedData, mParsedDataCount, sizeof( *mParsedData ), compareDurations );
    }
}




















