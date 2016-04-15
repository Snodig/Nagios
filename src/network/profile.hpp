/*
 *
 */

#ifndef PROFILE_H
#define PROFILE_H

#include <QDialog>

#include "src/utils/declares.hpp"

class QPushButton;
class QLineEdit;
class QLabel;
class QXmlStreamWriter;
class QListView;
class QStringListModel;


namespace rpnoc
{

class Profile : public QDialog
{
    Q_OBJECT
public:
	explicit Profile( QWidget *parent = 0);

	void					setUser( QString user );

signals:
	void					sendCreds( QStringList networkList );
	void					sendProfile( QString select );
	void					signalChangeUser( void );
	void					signalCancel( QString iReason );

public slots:
	void					loadProfile( void );
	void					newProfile( void );
	void					deleteProfile( void );
	void					changeUser( void );
	void					slotCancel( void );

private:
	QStringList				readXml( const QString &username );
	void					writeIndexEntry( QXmlStreamWriter* xmlWriter, const QStringList* item );
	bool					writeXml( const QString &filename, QStringList* item );

	QString					getUser( void ) const	{ return mUser; }
	void					showProfiles( void );

private:
	QPushButton*			buttonLoadProfile;
	QPushButton*			buttonDeleteProfile;
	QPushButton*			buttonNewProfile;
	QPushButton*			buttonSwitchUser;
	QPushButton*			buttonCancel;
	QStringList				profileList;

	QListView*				listView;
	QStringListModel*		model;

	QString					mUser;
};

class ProfileNameDialog : public QDialog
{
	Q_OBJECT
public:
	ProfileNameDialog( QWidget* parent = NULL );

private slots:
	void					filename( void );

public:
	void					setFilename( const QString name ) { mName = name; }
	QString					getFilename( void ) const { return mName; }

private:
	QPushButton*			buttonCancel;
	QPushButton*			buttonOk;
	QLabel*					labelName;
	QLineEdit*				lineEditName;
	QString					mName;
};

} // namespace rpnoc

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::Profile;
#endif

#endif // PROFILE_H
