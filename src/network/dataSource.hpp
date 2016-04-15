/**
  *	This baseclass handles storage and retreival of the data to be vizualised by the DrawableSource-class,
  * as well as some conveniance-functions
  */

#ifndef DATASOURCE_HPP
#define DATASOURCE_HPP

#include <QtNetwork>
#include "src/utils/declares.hpp"


namespace rpnoc
{
	class DataSource : public QObject
	{
		Q_OBJECT

	public:
		DataSource( QString datatype, QString serverIP, quint16 serverPort, QObject* parent = 0 );
		virtual				~DataSource();

		void							setIP( QString IP );
		void							setName( QString name );
		void							setPort( quint16 port );
		QString							getIP( void )						const;
		QString							getName( void )						const;
		quint16							getPort( void )						const;
		bool							isRunning( void )					const;
		QString							toString( void )					const;
		int								getTimeSinceLastUpdate( void );

		/**
		  *	Saves retrieved data to internal array
		  */
		void							setServerData( QString data );
		void							setServerData( QByteArray data );

		/**
		  *	These are the only methods that need reimplementation for new plugins
		  */
		virtual void					parseServerData( void )		 =0;
		virtual QString**				getServerData( void )		const=0;

	signals:
		/**
		  *	A signal used for error-messages and general feedback
		  */
		void							signalStatusMessage( QString message, QString source = "Base: DataSource" );

	protected:
		QString							mName;
		QString							mIP;
		quint16							mPort;
		bool							mRunning;
		QTime							mLastUpdateTime;
		QByteArray						mServerData;
	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::DataSource;
#endif

#endif // DATASOURCE_HPP
