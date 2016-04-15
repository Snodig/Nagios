/*
 *
 */

#include <QDir>
#include <QtGui>
#include <QDebug>
#include <iostream>
#include <QtXml/QtXml>

#include "src/utils/debug.hpp"
#include "src/network/loginscreen.hpp"
#include "src/network/newuser.hpp"


rpnoc::NewUser::NewUser( QWidget* iParent ) 
	:	QDialog( iParent )
{
	mButtonCancel = new QPushButton( "Cancel" );
	mButtonCreateUser = new QPushButton( "Create User" );

	connect( mButtonCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
	connect( mButtonCreateUser, SIGNAL( clicked() ), this, SLOT( saveUserData() ) );

	mLabelUsername = new QLabel( tr( "Username" ) );
	mLabelPassword = new QLabel( tr( "Password" ) );
	mLabelConfirmPassword = new QLabel( tr( "Confirm Password" ) );
	mLabelIpAddress = new QLabel( "IP Address" );
	mLabelPort = new QLabel( "Port" );

	mLineEditUsername = new QLineEdit;
	mLineEditPassword = new QLineEdit;
	mLineEditPassword->setEchoMode( QLineEdit::Password );
	mLineEditConfirmPassword = new QLineEdit;
	mLineEditConfirmPassword->setEchoMode( QLineEdit::Password );
	mLineEditIpAddress = new QLineEdit;
	mLineEditPort = new QLineEdit;

	mLabelUsername->setBuddy( mLineEditUsername );
	mLabelPassword->setBuddy( mLineEditPassword );
	mLabelConfirmPassword->setBuddy( mLineEditConfirmPassword );

	QHBoxLayout* oHorizontalOne = new QHBoxLayout;
	oHorizontalOne->addWidget( mLabelUsername );
	oHorizontalOne->setSpacing( 67 );
	oHorizontalOne->addWidget( mLineEditUsername );

	QHBoxLayout* oHorizontalTwo = new QHBoxLayout;
	oHorizontalTwo->addWidget( mLabelPassword );
	oHorizontalTwo->setSpacing( 69 );
	oHorizontalTwo->addWidget( mLineEditPassword );

	QHBoxLayout* oHorizontalThree = new QHBoxLayout;
	oHorizontalThree->addWidget( mLabelConfirmPassword );
	oHorizontalThree->setSpacing( 11 );
	oHorizontalThree->addWidget( mLineEditConfirmPassword );

	QHBoxLayout* oNetworkLayout = new QHBoxLayout;
	oNetworkLayout->addWidget( mLabelIpAddress );
	oNetworkLayout->addWidget( mLineEditIpAddress );
	oNetworkLayout->addWidget( mLabelPort );
	oNetworkLayout->addWidget( mLineEditPort );

	QHBoxLayout* oButtonLayout = new QHBoxLayout;
	oButtonLayout->addWidget( mButtonCancel );
	oButtonLayout->addWidget( mButtonCreateUser );

	QVBoxLayout* oMainLayout = new QVBoxLayout;
	oMainLayout->addLayout( oHorizontalOne );
	oMainLayout->addLayout( oHorizontalTwo );
	oMainLayout->addLayout( oHorizontalThree );
	oMainLayout->addLayout( oNetworkLayout );
	oMainLayout->addLayout( oButtonLayout );

	setFixedSize( 380, 180 );
	setLayout( oMainLayout );
	setWindowTitle( "Create new user" );
}

void rpnoc::NewUser::saveUserData( void )
{
	// write data to xml - create user
	if( validateCredentias() ) {
		QDir oDirectory( "profiles/" );
		if( !oDirectory.exists() ) {
			log_e( "Directory does not exists: " + log_endl + oDirectory.absolutePath() );
		}

		oDirectory.mkdir( mList.at( 0 ).toLower() );
		setUserName( mList.at( 0 ) );

		if( !oDirectory.exists() ) {
			log_e( "Failed to create directory: " + log_endl + oDirectory.absolutePath() );
		}

		log_i( "Wrote new user xml: " + log_endl + oDirectory.absolutePath() );
		writeXml( mList.at( 0 ) + ".xml", &mList );

		clearTextFields();
		this->close();
	}
}

void rpnoc::NewUser::writeIndexEntry( QXmlStreamWriter* iWriter, const QStringList* iItem )
{
	iWriter->writeStartElement( "user" );
	iWriter->writeAttribute( "username", iItem ->at( 0 ) );
	iWriter->writeEndElement();
	iWriter->writeStartElement( "pass" );
	iWriter->writeAttribute( "password", iItem ->at( 1 ) );
	iWriter->writeEndElement();
	iWriter->writeStartElement( "addr" );
	iWriter->writeAttribute( "address", iItem ->at( 2 ) );
	iWriter->writeEndElement();
	iWriter->writeStartElement( "port" );
	iWriter->writeAttribute( "portnr", iItem ->at( 3 ) );
	iWriter->writeEndElement();
}

bool rpnoc::NewUser::writeXml( const QString& iFilename, QStringList* iItem )
{
	QString oFileName = iFilename.toLower();
	QFile oFile( "profiles/" + iItem->at( 0 ) + "/" + oFileName );

	if( !oFile.open( QFile::ReadWrite | QFile::Text ) ){
		log_e( "Failed to open file '" + oFileName + "', " + log_endl + oFile.errorString() );
		return false;
	}

	QXmlStreamWriter oWriter( &oFile );
	oWriter.setAutoFormatting( true );
	oWriter.writeStartDocument();
	oWriter.writeStartElement( "personalia" );
	writeIndexEntry( &oWriter, iItem );
	oWriter.writeEndDocument();

	oFile.close();
	if( oFile.error() ) {
		log_e( "Failed to close file '" + oFileName + "', " + log_endl + oFile.errorString() );
		return false;
	}
	
	return true;
}

void rpnoc::NewUser::clearTextFields( void )
{
	mLineEditUsername->clear();
	mLineEditPassword->clear();
	mLineEditConfirmPassword->clear();
	mLineEditIpAddress->clear();
	mLineEditPort->clear();
}

bool rpnoc::NewUser::validateCredentias()
{
	QString oUsername = mLineEditUsername->text();
	QString oPassword = mLineEditPassword->text();
	QString oConfirm = mLineEditConfirmPassword->text();
	QString oIPAddress = mLineEditIpAddress->text();
	QString oPort = mLineEditPort->text();

	if( oUsername.isEmpty() || oPassword.isEmpty() || oConfirm.isEmpty() ) {
		log_e( "Unable to validate because of empty fields" );
		QMessageBox oMessage;
		oMessage.setText( "There are empty fields." );
		oMessage.setInformativeText( "Make sure you have filled in all required fields." );
		oMessage.setStandardButtons( QMessageBox::Ok );
		oMessage.setDefaultButton( QMessageBox::Ok );
		oMessage.setIcon( QMessageBox::Information );
		oMessage.exec();
		return false;
	}

	if( oConfirm != oPassword ) {
		log_e( "Unable to conenct, invalid password." )
		QMessageBox oMessage;
		oMessage.setText( "Passwords do not match." );
		oMessage.setInformativeText( "Make sure you type the same password in both fields." );
		oMessage.setStandardButtons( QMessageBox::Ok );
		oMessage.setDefaultButton( QMessageBox::Ok );
		oMessage.setIcon( QMessageBox::Information );
		oMessage.exec();
		return false;
	}

	if( oPassword.count() <= 5 || oPassword.count() >= 21 ){
		QMessageBox oMessage;
		oMessage.setText( "Password length error." );
		oMessage.setInformativeText( "Your password needs to be between 6 and 20 characters." );
		oMessage.setStandardButtons( QMessageBox::Ok );
		oMessage.setDefaultButton( QMessageBox::Ok );
		oMessage.setIcon( QMessageBox::Information );
		oMessage.exec();
		return false;
	}

	if( oUsername.count() <= 2 || oUsername.count() >= 50 ) {
		QMessageBox oMessage;
		oMessage.setText( "Username length error." );
		oMessage.setInformativeText( "Your username needs to be between 2 and 50 characters." );
		oMessage.setStandardButtons( QMessageBox::Ok );
		oMessage.setDefaultButton( QMessageBox::Ok );
		oMessage.setIcon( QMessageBox::Information );
		oMessage.exec();
		return false;
	}

	mList << oUsername << oPassword << oIPAddress << oPort;
	return true;
}

QString rpnoc::NewUser::getUserName( void ) const
{
	return mUserName;
}

void rpnoc::NewUser::setUserName( QString iName )
{
	mUserName = iName;
}







































