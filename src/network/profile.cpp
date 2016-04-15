
#include <QtGui>
#include <QtXml/QtXml>
#include <QDir>
#include <QDebug>
#include <iostream>

#include "src/utils/debug.hpp"
#include "src/network/loginscreen.hpp"
#include "src/network/settingsdialog.hpp"
#include "src/network/profile.hpp"


rpnoc::Profile::Profile( QWidget *parent) 
	: QDialog( parent )
{
	buttonLoadProfile = new QPushButton( "Load" );
	buttonNewProfile = new QPushButton( "New" );
	buttonDeleteProfile = new QPushButton( "Delete" );
	buttonCancel = new QPushButton( "Cancel" );
	buttonSwitchUser = new QPushButton( "Change User" );

	model = new QStringListModel( this );

	listView = new QListView;
	listView->setModel( model );
	listView->setEditTriggers( QAbstractItemView::NoEditTriggers );

	connect( buttonLoadProfile, SIGNAL( clicked() ), this, SLOT( loadProfile() ) );
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( slotCancel() ) );
	connect( buttonNewProfile, SIGNAL( clicked() ), this, SLOT( newProfile() ) );
	connect( buttonSwitchUser, SIGNAL( clicked() ), this, SLOT( changeUser() ) );
	connect( buttonDeleteProfile, SIGNAL( clicked() ), this, SLOT( deleteProfile() ) );

	QVBoxLayout* buttonLayoutOne = new QVBoxLayout;
	buttonLayoutOne->addWidget( buttonLoadProfile );
	buttonLayoutOne->addWidget( buttonNewProfile );
	buttonLayoutOne->addWidget( buttonDeleteProfile );
	buttonLayoutOne->addStretch();
	buttonLayoutOne->addWidget( buttonSwitchUser );
	buttonLayoutOne->addStretch();
	buttonLayoutOne->addWidget( buttonCancel );

	QVBoxLayout* listLayout = new QVBoxLayout;
	listLayout->addWidget( listView );

	QHBoxLayout* mainLayout = new QHBoxLayout;
	mainLayout->addLayout( buttonLayoutOne );
	mainLayout->addLayout( listLayout );

	setLayout( mainLayout );
	setWindowTitle( "Profiles" );
}

void rpnoc::Profile::loadProfile( void )
{
	// selection from listview
	QString selection = model->data( listView->currentIndex(), 0 ).toString();
	if( !selection.isEmpty() ) {
		QString tempUser = getUser();
		
		// get user credentials
		QStringList toNetwork = readXml(tempUser);
	
		// send credentials tonetwork
		emit sendCreds(toNetwork);
		emit sendProfile( getUser() + "/" + selection);
		hide();
	}
}

void rpnoc::Profile::deleteProfile( void )
{
	QVariant oTemp = model->data( listView->currentIndex(), 0 );
	model->removeRows( listView->currentIndex().row(), 1 );

	QString oTempUser = getUser();
	QString oFilename = oTemp.toString();
	
	QFile oFile;
	oFile.remove( "profiles/" + oTempUser + "/" + oFilename );
	log_i( "Removed profile 'profiles/" + oTempUser + "/" + oFilename + "'." ); 

	profileList.removeOne( oFilename );
}

void rpnoc::Profile::newProfile( void )
{
	ProfileNameDialog oProfileDialog;
	oProfileDialog.exec();

	QString oFilename = oProfileDialog.getFilename();
	QString oTempUser = getUser();
	QString oFile( "profiles/" + oTempUser + "/" + oFilename + ".xml" );
	writeXml( oFile, &profileList );
	profileList.append( oFilename + ".xml" );
	log_i( "New profile Created '" + oFile + "'." );

	showProfiles();
}

void rpnoc::Profile::changeUser( void )
{
	this->done( 1 );
	emit signalChangeUser();
}

void rpnoc::Profile::slotCancel( void )
{
	emit signalCancel( QString( "User pressed cancel at profile window" ) );
	this->close();
}

void rpnoc::Profile::setUser( QString iUser ) 
{
	mUser = iUser;
	QStringList oFilters;
	oFilters << "*.xml";

	QDir oDir( "profiles/" + getUser() );
	if( !oDir.exists() ) {
		log_e( "Directory does not exist: '" + oDir.currentPath() + "'." );
	}

	oDir.setFilter( QDir::Files );
	oDir.setNameFilters( oFilters );
	oDir.setSorting( QDir::Name );

	QFileInfoList oFileInfoList = oDir.entryInfoList();
	for(int i = 0; i < oFileInfoList.size(); ++i){
		QFileInfo oFileInfo = oFileInfoList.at(i);
		if(oFileInfo.fileName() != (getUser() + ".xml")){
			profileList << ( QString( "%1" ).arg( oFileInfo.fileName() ) );
		}
	}
	showProfiles();
}

QStringList rpnoc::Profile::readXml( const QString &iUsername )
{
	QString oUser, oPass, oAddr, oPort;
	QString oUserName = iUsername.toLower();
	QFile oFile("profiles/" + oUserName + "/" + oUserName + ".xml");

	if( !oFile.open( QIODevice::ReadOnly ) ) {
		log_e( "Failed to open file '" + oFile.errorString() + "' for reading." );
	}

	QXmlStreamReader oReader;
	oReader.setDevice( &oFile );

	oReader.readNext();
	while(!oReader.atEnd()){
		if(oReader.isStartElement()){
			if(oReader.name() == "personalia"){
				oReader.readNext();
				while(!oReader.atEnd()){
					if(oReader.isEndElement()){
						oReader.readNext();
					}
					if(oReader.isStartElement()){
						if(oReader.name() == "user"){
							oUser = oReader.attributes().value("username").toString();
							oReader.readNext();
						}else if(oReader.name() == "pass"){
							oPass = oReader.attributes().value("password").toString();
							oReader.readNext();
						}else if(oReader.name() == "addr"){
							oAddr = oReader.attributes().value("address").toString();
							oReader.readNext();
						}else if(oReader.name() == "port"){
							oPort = oReader.attributes().value("portnr").toString();
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

	QStringList oStringList;
	oStringList << oUser << oPass << oAddr << oPort;
	return oStringList;
}

void rpnoc::Profile::writeIndexEntry( QXmlStreamWriter *oWriter, const QStringList *iItem )
{
	oWriter->writeStartElement("Layout");
	oWriter->writeEndElement();
}

bool rpnoc::Profile::writeXml( const QString &iFilename, QStringList *iItem )
{
	QString oFileName = iFilename.toLower();
	QFile oFile( oFileName );
	if( !oFile.open( QFile::ReadWrite | QFile::Text ) ) {
		log_e( "Failed to open file '" + iFilename + "'." + log_endl + oFile.errorString() );
		return false;
	}

	QXmlStreamWriter oWriter( &oFile );
	oWriter.setAutoFormatting( true );
	oWriter.writeStartDocument();
	oWriter.writeStartElement( "Profile" );
	writeIndexEntry( &oWriter, iItem );
	oWriter.writeEndDocument();
	
	oFile.close();
	if( oFile.error() ) {
		log_e( "Failed to close file '" + iFilename + "'." + log_endl + oFile.errorString() );
		return false;
	}
	return true;
}

void rpnoc::Profile::showProfiles( void )
{
	model->setStringList( profileList );
}



// name new profile dialog
rpnoc::ProfileNameDialog::ProfileNameDialog( QWidget *iParent )
	: QDialog( iParent )
{
	buttonCancel = new QPushButton( "Cancel" );
	buttonOk = new QPushButton( "Ok" );
	labelName = new QLabel( "Name" );
	lineEditName = new QLineEdit;

	labelName->setBuddy( lineEditName );

	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
	connect( buttonOk, SIGNAL( clicked() ), this, SLOT( filename() ) );

	QHBoxLayout* nameLayout = new QHBoxLayout;
	nameLayout->addWidget( labelName );
	nameLayout->addWidget( lineEditName );

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget( buttonCancel );
	buttonLayout->addWidget( buttonOk );

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout( nameLayout );
	mainLayout->addLayout( buttonLayout );

	setLayout( mainLayout );
	setFixedSize( 220, 110 );
	setWindowTitle( "Enter profile name" );
}

void rpnoc::ProfileNameDialog::filename( void )
{
	setFilename( lineEditName->text() );
	this->done( 1 );
}
