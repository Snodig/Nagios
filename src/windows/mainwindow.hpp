/*
 * mainwindow.hpp
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtGui/QMainWindow>
#include <QVector2D>
#include <QFile>
#include "src/network/networker.hpp"
#include "src/network/settingsdialog.hpp"
#include "src/network/profile.hpp"

class QAction;
class QLabel;
class QScrollArea;
class QMenu;
class QKeyMenu;
class GLFramework;
class LoginScreen;
class SettingsDialog;

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
						MainWindow( QWidget* iParent = NULL );
						~MainWindow( void );
private slots:
	void				about( void );
	void				aboutQt( void );
	void				toggleFullscreen( void );
	void				toggleMenuBar( void );
	void				showLoginScreen( void );
	void				showSettingsDialog( void );
	void				saveLayouts( void );
	void				slotUpdateWindowSettings( ProgramSettings iSettings );
	void				slotStartProfileDialog( QString iUsername );
	void				slotSetupLayouts( QString iXml );
	void				slotExitProgram( QString iReason );
	void				slotExitProgram( void );
	void				slotChangeUser( void );
	
protected:
	bool				event( QEvent* iEvent );	

private:
	void				setupWindow( void );
	void				setupOpenGL( void );
	void				rebindConnects( void );
	void				keyPressEvent( QKeyEvent* iEvent );
	
	QMenu*				mMenuFile;
	QMenu*				mMenuLayout;
	QMenu*				mMenuSettings;
	QMenu*				mMenuHelp;
	
	QAction*			mActionAbout;
	QAction*			mActionAboutQt;
	QAction*			mActionExit;
	QAction*			mActionLogin;
	QAction*			mActionProfile;
	QAction*			mActionSettings;
	QAction*			mActionNewLayout;
	QAction*			mActionRemLayout;
	QAction*			mActionSaveLayout;
	QAction*			mActionToggleReel;
	
	QLabel*				mLabelInfo;
	QWidget*			mWidgetMain;
	QScrollArea*		mScrollWidgetMain;
	GLFramework*		mGLFramework;

	Networker*			mNetworker;
	LoginScreen*		mLoginScreen;
	SettingsDialog*		mSettingsDialog;
	Profile*			mProfile;

public:
	static QVector2D	mWindowSize;
	static QVector2D	getWindowSize( void ) { return mWindowSize; }
	static void			setWindowSize( QVector2D iSize ) { mWindowSize = iSize; }
};

extern QFile* gLogFile;

#endif // MAINWINDOW_HPP
