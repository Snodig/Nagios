// loginscreen.cpp

#include <QtGui>
#include <QtXml/QtXml>
#include <QDebug>
#include <iostream>

#include "src/utils/debug.hpp"
#include "src/network/loginscreen.hpp"
#include "src/network/settingsdialog.hpp"
#include "src/network/profile.hpp"


rpnoc::LoginScreen::LoginScreen( QWidget* iParent )
	:	QDialog( iParent )
{
	mNewUser = new NewUser( this );

	mButtonCancel = new QPushButton( "Cancel" );
	mButtonNewUser = new QPushButton( "New User" );
	mButtonLogin = new QPushButton( "Login" );

	connect( mButtonCancel, SIGNAL( clicked() ), this, SLOT( slotCancel( void ) ) );
	connect( mButtonNewUser, SIGNAL( clicked() ), this, SLOT( createNewUser() ) );
	connect( mButtonLogin, SIGNAL( clicked() ), this, SLOT( slotLoadUserData() ) );

	mLabelUsername = new QLabel( "Username" );
	mLabelPassword = new QLabel( "Password" );

	mLineEditUsername = new QLineEdit;
	mLineEditPassword = new QLineEdit;
	mLineEditPassword->setEchoMode( QLineEdit::Password );

	QHBoxLayout* oUsernameLayout = new QHBoxLayout;
	oUsernameLayout->addWidget( mLabelUsername );
	oUsernameLayout->addSpacing( 67 );
	oUsernameLayout->addWidget( mLineEditUsername );

	QHBoxLayout* oPasswordLayout = new QHBoxLayout;
	oPasswordLayout->addWidget( mLabelPassword );
	oPasswordLayout->addSpacing( 69 );
	oPasswordLayout->addWidget( mLineEditPassword );

	QHBoxLayout* oButtonLayout = new QHBoxLayout;
	oButtonLayout->addWidget( mButtonCancel );
	oButtonLayout->addWidget( mButtonNewUser );
	oButtonLayout->addWidget( mButtonLogin );

	QVBoxLayout* oMainLayout = new QVBoxLayout;
	oMainLayout->addLayout( oUsernameLayout );
	oMainLayout->addLayout( oPasswordLayout );
	oMainLayout->addLayout( oButtonLayout );

	setFixedSize( 380, 120 );
	setLayout( oMainLayout );
	setWindowTitle( "Northug Login" );
	mButtonLogin->setDefault( true );
	mButtonLogin->setAutoDefault( true );
}

void rpnoc::LoginScreen::slotLoadUserData( void )
{
	QString oUsername = mLineEditUsername->text();
	QString oPassword = mLineEditPassword->text();

	if( oUsername.isEmpty() || oPassword.isEmpty() ) {
		log_e( "Username or Password field is empty." );
		// show error message
		QMessageBox oMessage;
		oMessage.setText( "Username or password field is empty." );
		oMessage.setStandardButtons( QMessageBox::Ok );
		oMessage.setDefaultButton( QMessageBox::Ok );
		oMessage.setIcon( QMessageBox::Information );
		oMessage.exec();
	}else{
		readXml( oUsername, oPassword );
	}
}

void rpnoc::LoginScreen::readXml( const QString& iUsername, const QString& iPassword )
{
	log_i( "Reading profile XML." );
	QString oUser, oPassword, oAddress, oPort;
	QString oUserfile = mLineEditUsername->text();
	QString oFileName = iUsername.toLower();
	QFile oFile( "profiles/" + oUserfile + "/" + oFileName + ".xml" );

	if( !oFile.open( QIODevice::ReadOnly ) ) {
		log_e( "Failed to open file '" + oFile.fileName() + "'," + log_endl + oFile.errorString() );
	}

	QXmlStreamReader oReader;
	oReader.setDevice( &oFile );

	oReader.readNext();
	while( !oReader.atEnd() ) {
		if( oReader.isStartElement() ) {
			if( oReader.name() == "personalia" ) {
				oReader.readNext();
				while( !oReader.atEnd()){
					if(oReader.isEndElement()){
						oReader.readNext();
					}
					if(oReader.isStartElement()){
						if(oReader.name() == "user"){
							oUser = oReader.attributes().value("username").toString();
							qDebug() << oUser;
							oReader.readNext();
						}else if(oReader.name() == "pass"){
							oPassword = oReader.attributes().value("password").toString();
							qDebug() << oPassword;
							oReader.readNext();
						}else if(oReader.name() == "addr"){
							oAddress = oReader.attributes().value("address").toString();
							qDebug() << oAddress;
							oReader.readNext();
						}else if(oReader.name() == "port"){
							oPort = oReader.attributes().value("portnr").toString();
							qDebug() << oPort;
							oReader.readNext();
						}
					}else{
						oReader.readNext();
					}
				}
			}else{
				oReader.raiseError(QObject::tr("Not a user file"));
			}
		}else{
			oReader.readNext();
		}
	}

	oFile.close();

	if( oUser == iUsername && iPassword == oPassword ) {
		// TODO: Login! TODO TODO TODO TODO --- LOAD PROFILE SELECTION SCREEN
		qDebug() << "username and password checked";
		this->done( 1 );
		emit signalLoginUser( oUser );
		clearTextFields();
	}else{
		// show error message box
		qDebug() << "password or username does not exist";
		QMessageBox message;
		message.setText("Username and password does not match.");
		message.setInformativeText("If this is your first time logging in, you have to create a new user.");
		message.setStandardButtons(QMessageBox::Ok);
		message.setDefaultButton(QMessageBox::Ok);
		message.setIcon(QMessageBox::Information);
		message.exec();
		clearTextFields();
	}
}

void rpnoc::LoginScreen::slotCreateNewUser( void )
{
	qDebug() << "createNewUser slot called";
	clearTextFields();

	mNewUser->show();
	mNewUser->raise();
	mNewUser->activateWindow();
}

void rpnoc::LoginScreen::clearTextFields( void )
{
	mLineEditPassword->clear();
	mLineEditUsername->clear();
}

void rpnoc::LoginScreen::slotCancel( void )
{
	emit signalCancel( QString( "User pressed cancel at loginscreen" ) );
	this->close();
}
