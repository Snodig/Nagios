/*
 *
 */

#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QDialog>

#include "src/utils/declares.hpp"
#include "src/network/newuser.hpp"

class QPushButton;
class QLabel;
class QLineEdit;
class QXmlStreamReader;


namespace rpnoc
{

	class LoginScreen : public QDialog
	{
		Q_OBJECT
	public:
		explicit				LoginScreen( QWidget* iParent = NULL );
	
	public slots:
		void					slotLoadUserData( void );
		void					slotCreateNewUser( void );
		void					slotCancel( void );
	
	signals:
		void					signalLoginUser( QString iUser );
		void					signalCancel( QString iReason );
	
	private:
		void					readXml( const QString& iFilename, const QString& iPassword );
		void					clearTextFields( void );
	
	private:
		QPushButton*			mButtonCancel;
		QPushButton*			mButtonNewUser;
		QPushButton*			mButtonLogin;
		QPushButton*			mButtonSettings;
		QLabel*					mLabelUsername;
		QLabel*					mLabelPassword;
		QLineEdit*				mLineEditUsername;
		QLineEdit*				mLineEditPassword;
	
		NewUser*				mNewUser;
	};

} // namespace rpnoc

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::LoginScreen;
#endif

#endif // LOGINSCREEN_H
