// newuser.h

#ifndef NEWUSER_H
#define NEWUSER_H

#include <QDialog>
#include "src/utils/declares.hpp"


class QPushButton;
class QLabel;
class QLineEdit;
class QStringListModel;
class QListView;
class QXmlStreamWriter;
class QXmlStreamReader;
class QStringList;

namespace rpnoc
{
	
	class NewUser : public QDialog
	{
		Q_OBJECT
	public:
		explicit				NewUser( QWidget* iParent = NULL );
	
		QString					getUserName( void ) const;
		void					setUserName( QString name );
	
	public slots:
		void					saveUserData( void );
	
	private:
		void					writeIndexEntry( QXmlStreamWriter* iXmlWriter, const QStringList* iItem );
		bool					writeXml( const QString& iFilename, QStringList* iItem );
		bool					validateCredentias( void );
		void					clearTextFields( void );
	
	private:
		QPushButton*			mButtonCancel;
		QPushButton*			mButtonCreateUser;
		QLabel*					mLabelUsername;
		QLabel*					mLabelPassword;
		QLabel*					mLabelConfirmPassword;
		QLineEdit*				mLineEditUsername;
		QLineEdit*				mLineEditPassword;
		QLineEdit*				mLineEditConfirmPassword;
		QLabel*					mLabelIpAddress;
		QLabel*					mLabelPort;
		QLineEdit*				mLineEditIpAddress;
		QLineEdit*				mLineEditPort;
		QStringList				mList;
		QString					mUserName;
	};

} // namespace rpnoc

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::NewUser;
#endif

#endif // NEWUSER_H
