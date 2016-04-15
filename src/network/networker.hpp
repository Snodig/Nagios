#ifndef NETWORKER_HPP
#define NETWORKER_HPP

#include "src/network/loginscreen.hpp"
#include "src/utils/declares.hpp"
#include "src/network/dataSource.hpp"
#include "src/network/nagiosSource.hpp"


namespace rpnoc
{
	class Networker : public QObject{
		Q_OBJECT
	public:
									Networker(QWidget* parent=0);
									~Networker( void );
	
		void						unsubscribe( QString type );
		static void					addDataSource( DataSource* iDataSource );
		static DataSource*			getDataSource( QString iName );
	
		void						appendSubscriptions( QStringList iSubscriptions );
		void						subscribeToAll();
		QString						getProfile() const;
	
	public slots:
		void						selectedProfile( QString profile );
		
	private slots:
		void						subscribe( QString type );
		void						on_login_attempt( QStringList credentials);
		void						on_status_message( QString message, QString source = "NOT_SPECIFIED" );
		int							getPacketLengthFromHeader();
		void						resetPacketHeader();
		void						readMultiPacket();
		void						read_packet( void );
		void						ping_connection( void );
		void						on_ping_timeout( void );
		void						disconnect( void );
	
	signals:
		void						status_message( QString message, QString source );
	
	private:
		void						sendPacket( QByteArray packet )    const;
		void						sendPacket( QString packet )       const;
		void						handlepacket( rpnoc::RawData packet );
		
		QTcpSocket*                 mSocket;
		QTimer*                     mTimeoutTimer;
		QTimer*                     mSendPingTimer;
		RawData						mPacket;
		int                         mBytesToRead;
		int                         mTotalBytesToRead;
		QStringList					mAttemptedSubscriptions;
		static QList< DataSource* > sDataSources;
		QString						mProfile;
	};

} // namespace rpnoc

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::Networker;
#endif

#endif // NETWORKER_HPP
