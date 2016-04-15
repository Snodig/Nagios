/**
  * A custom subclass to store and parse json-objects with data from a Nagios-server
  */

#ifndef NAGIOSSOURCE_HPP
#define NAGIOSSOURCE_HPP

#include <QObject>
#include <QScriptEngine>

#include "src/utils/declares.hpp"
#include "src/network/dataSource.hpp"

namespace rpnoc
{
	class NagiosSource : public rpnoc::DataSource
	{
		Q_OBJECT

	public:
		enum DataField
		{
			HOSTNAME,
			HOSTGROUP,
			SERVICE_DESCRIPTION,
			DURATION,
			OUTPUT,
			OWNERS,
			TEAM,
			SERVER_STATUS,
			HOST_STATUS,
			OBJECT_ID
		};

									NagiosSource( QObject *parent = 0 );
									~NagiosSource();

		int							getCountCritical( void )			const;
		int							getCountWarning( void )				const;
		int							getCountOK( void )					const;
		int							getCountUnknown( void )				const;
		int							getCountOfStatus( QString status )	const;
		QList< QString* >			getAllOfStatus( QString status )	const;

		virtual void				parseServerData( void );
		QString**					getServerData( void )				const;

	private:
		QString**					mParsedData;
		QString**					mOldParsedData;
		int							mParsedDataCount;
		int							mOldParsedDataCount;
		int							mCountCritical;
		int							mCountUnknown;
		int							mCountWarning;
		int							mCountOK;

	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::NagiosSource;
#endif

#endif // NAGIOSSOURCE_HPP
