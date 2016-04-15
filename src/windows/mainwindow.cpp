/*
 * mainwindow.hpp
 */

#include <QtGui>
#include <QtOpenGL>
#include <QGLWidget>
#include <QGLFormat>
#include "src/widgets/glframework.hpp"
#include "src/network/networker.hpp"
#include "src/utils/debug.hpp"
#include "src/network/loginscreen.hpp"
#include "src/network/settingsdialog.hpp"
#include "src/network/profile.hpp"
#include "src/windows/mainwindow.hpp"


QVector2D MainWindow::mWindowSize = QVector2D( 1200, 900 );
QFile* gLogFile = NULL;


MainWindow::MainWindow( QWidget* iParent )
	: QMainWindow( iParent ) 
{
	gLogFile = new QFile( 
		"log/log-" + 
		QDate::currentDate().toString( "yyyy-MM-dd_" ) + 
		DebugTimer::getInstance().getTimeCurrent() + ".log" 
	);
	
	debug_e( gLogFile->open( QIODevice::WriteOnly | QIODevice::Text ), "Failed to open logfile" );
	
	log_i( "starting program" );
	
	mWidgetMain = new QWidget;
	setCentralWidget( mWidgetMain );
	mNetworker = new Networker( this );
	setupOpenGL();
	// 87.238.46.18
	// 45454
}

MainWindow::~MainWindow( void )
{	
	delete gLogFile;
	delete mWidgetMain;
	delete mNetworker;
	delete mProfile;
	delete mLoginScreen;
	delete mSettingsDialog;
	delete mGLFramework;
	
	delete mScrollWidgetMain;
	delete mLabelInfo;
	
	delete mMenuFile;
	delete mMenuHelp;
	delete mMenuLayout;
	delete mMenuSettings;
	
	delete mActionAbout;
	delete mActionAboutQt;
	delete mActionExit;
	delete mActionLogin;
	delete mActionNewLayout;
	delete mActionProfile;
	delete mActionRemLayout;
	delete mActionSaveLayout;
	delete mActionSettings;
	delete mActionToggleReel;
	
	qApp->quit();
}

void MainWindow::keyPressEvent( QKeyEvent* iEvent )
{
	switch( iEvent->modifiers() ) {
		case Qt::ControlModifier :
			switch( iEvent->key() ) {
				case Qt::Key_Tab :
					mGLFramework->switchLayout();
					break;
				case Qt::Key_Minus :
					mGLFramework->slotRemoveLayout();
					break;
				case Qt::Key_Plus :
					mGLFramework->slotAddLayout();
					break;
			}
			break;
		default :
			switch( iEvent->key() ) {
				case Qt::Key_F2 :
					toggleMenuBar();
					break;
				case Qt::Key_F1 :
					toggleFullscreen();
					break;
				case Qt::Key_F3 :
					mGLFramework->slotToggleReelShow();
					break;
				case Qt::Key_Escape :
					slotExitProgram( QString( "User pressed 'ESCAPE'" ) );
					break;
				default :
					break;
			}
			break;
	}
}

void MainWindow::about( void )
{
	QMessageBox::about( this, "About Menu", "The <b>Northug</b> uber pulk!" ); 
}

void MainWindow::aboutQt( void )
{
	// TODO: what goes on here?
}

void MainWindow::toggleFullscreen( void )
{
	if( QWidget::isFullScreen() ) {
		QWidget::showNormal();
	} else {
		QWidget::showFullScreen();
	}
}

void MainWindow::toggleMenuBar( void )
{
	if( menuBar()->isHidden() ) {
		menuBar()->show();
	} else {
		menuBar()->hide();
	}
}

void MainWindow::setupWindow( void )
{
	{	// setup dialog windows
		connect( mLoginScreen, SIGNAL( signalLoginUser( QString ) ), this, SLOT( slotStartProfileDialog( QString ) ) );
		connect( mLoginScreen, SIGNAL( rejected() ), this, SLOT( slotExitProgram() ) );
		connect( mLoginScreen, SIGNAL( signalCancel( QString ) ), this, SLOT( slotExitProgram( QString ) ) );
		connect( mProfile, SIGNAL( sendProfile( QString ) ), this, SLOT( slotSetupLayouts( QString ) ) );
		connect( mProfile, SIGNAL( rejected() ), this, SLOT( slotExitProgram() ) );
		connect( mProfile, SIGNAL( signalCancel( QString ) ), this, SLOT( slotExitProgram( QString ) ) );
		connect( mProfile, SIGNAL( sendCreds( QStringList ) ), mNetworker, SLOT( on_login_attempt( QStringList ) ) );
		connect( mProfile, SIGNAL( signalChangeUser() ), this, SLOT( slotChangeUser() ) );
		connect( mNetworker, SIGNAL( status_message( QString, QString ) ), mGLFramework, SLOT( slotStatusMessage( QString, QString ) ) );
	}
	{	// setting up actions
		mActionExit = new QAction( "&Exit", this );
		mActionExit->setShortcut( QKeySequence::Quit );
		mActionExit->setStatusTip( "Exit the application" );
		connect( mActionExit, SIGNAL( triggered() ),
			this, SLOT( close() ) );
		
		mActionAbout = new QAction( "&About", this );
		connect( mActionAbout, SIGNAL( triggered() ),
			this, SLOT( about() ) );
			
		mActionLogin = new QAction( "&Login", this );
		connect( mActionLogin, SIGNAL( triggered() ),
			this, SLOT( showLoginScreen() ) );
		
		mActionProfile = new QAction( "&Profile", this );
		connect( mActionProfile, SIGNAL( triggered() ), mProfile, SLOT( exec() ) );

		mActionSettings = new QAction( "&Settings", this );
		connect(mActionSettings, SIGNAL( triggered() ),
			this, SLOT( showSettingsDialog() ) );
		
		connect( mSettingsDialog, SIGNAL( signalSettingsUpdated( ProgramSettings ) ), this, SLOT( slotUpdateWindowSettings( ProgramSettings ) ) );
			
		mActionNewLayout = new QAction( "&New Layout", this );
		connect( mActionNewLayout, SIGNAL( triggered() ),
			this, SLOT( about() ) );
		
		mActionAboutQt = new QAction( "About &Qt", this );
		connect( mActionAboutQt, SIGNAL( triggered() ),
			qApp, SLOT( aboutQt() ) );
		connect( mActionAboutQt, SIGNAL( triggered() ),
			this, SLOT( aboutQt() ) );

		mActionSaveLayout = new QAction( "Save Layouts", this );
		connect( mActionSaveLayout, SIGNAL( triggered() ), this, SLOT( saveLayouts() ) );
		
		mActionToggleReel = new QAction( QString( "&Edit/Reel" ), this );
		connect( mActionToggleReel, SIGNAL( triggered() ), mGLFramework, SLOT( slotToggleReelShow() ) );
		
		mActionNewLayout = new QAction( QString( "&Add Layout" ), this );
		connect( mActionNewLayout, SIGNAL( triggered() ), mGLFramework, SLOT( slotAddLayout() ) );
		
		mActionRemLayout = new QAction( QString( "&Remove Layout" ), this );
		connect( mActionRemLayout, SIGNAL( triggered() ), mGLFramework, SLOT( slotRemoveLayout() ) );
	}	//

	{	// setting up menues
		mMenuFile = menuBar()->addMenu( "&File" );
		mMenuFile->addAction( mActionLogin ); 
		mMenuFile->addAction( mActionProfile );
		mMenuFile->addAction( mActionExit );
		
		mMenuLayout = menuBar()->addMenu( "&Layout" );
		mMenuLayout->addAction( mActionNewLayout );
		mMenuLayout->addAction( mActionRemLayout );
		mMenuLayout->addAction( mActionSaveLayout );
		
		mMenuSettings = menuBar()->addMenu( "&Settings" );
		mMenuSettings->addAction( mActionSettings );

		menuBar()->addAction( mActionToggleReel );
		
		mMenuHelp = menuBar()->addMenu( "&Help" );
		mMenuHelp->addAction( mActionAbout );
		mMenuHelp->addAction( mActionAboutQt );
	}	//
}

void MainWindow::setupOpenGL( void )
{
	/*
	 * Make sure we get the right opengl
	 * format, and check that everything
	 * is supported on the running system.
	 * - Double buffering
	 * - Depth buffering
	 * - Alpha buffering
	 * - Size of buffers
	 * - Opengl 3.1 ( TODO: support higher versions too )
	 */
	QGLFormat oGLFormat;
	oGLFormat.setDoubleBuffer( true ); 
	oGLFormat.setDepth( true );
	oGLFormat.setAlpha( true );
	
	oGLFormat.setRedBufferSize( 8 );
	oGLFormat.setGreenBufferSize( 8 );
	oGLFormat.setBlueBufferSize( 8 );
	oGLFormat.setAlphaBufferSize( 8 );
	oGLFormat.setDepthBufferSize( 16 );
	
	oGLFormat.setVersion( 3, 1 );
	oGLFormat.setProfile( QGLFormat::CoreProfile );
	
	mGLFramework = new GLFramework( this, oGLFormat, mNetworker );
	debug_e( mGLFramework->format().depth(), "Failed to set Depth Buffer" );
	debug_e( mGLFramework->format().alpha(), "Failed to set Alpha Buffer" );
	debug_e( ( mGLFramework->format().majorVersion() == 3 ) && ( mGLFramework->format().minorVersion() == 1 ), "Opengl Version 3.1 not supported" );
	debug_e( ( mGLFramework->format().profile() == QGLFormat::CoreProfile ), "Opengl not set to Core Profile ( old commands depricated )" );
	
	log_i( 
		"opengl succesfully initialized" + log_endl + "- Currently using opengl version: " + 
		QString::number( mGLFramework->format().majorVersion() ) + "." + QString::number( mGLFramework->format().minorVersion() ) 
	);
	
	mScrollWidgetMain = new QScrollArea;
	mScrollWidgetMain->setWidget( mGLFramework );
	mScrollWidgetMain->setWidgetResizable( true );
	mScrollWidgetMain->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	mScrollWidgetMain->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	mScrollWidgetMain->setSizePolicy( 
		QSizePolicy::Ignored, 
		QSizePolicy::Ignored 
	);
	
	mScrollWidgetMain->setMinimumSize( MainWindow::getWindowSize().x(), MainWindow::getWindowSize().y() );
	
	QVBoxLayout* oMainLayout = new QVBoxLayout;
	oMainLayout->setMargin( 0 );
	oMainLayout->addWidget( mScrollWidgetMain, 0, 0 );
	mWidgetMain->setLayout( oMainLayout );
	
	resize( MainWindow::getWindowSize().x(), MainWindow::getWindowSize().y() );
}

void MainWindow::rebindConnects( void )
{
	disconnect( mLoginScreen, SIGNAL( signalCancel( QString ) ), this, SLOT( slotExitProgram( QString ) ) );
	disconnect( mProfile, SIGNAL( signalCancel( QString ) ), this, SLOT( slotExitProgram( QString ) ) );
	connect( mActionLogin, SIGNAL( triggered() ), mLoginScreen, SLOT( exec() ) );
}

void MainWindow::showLoginScreen()
{
	mLoginScreen->raise();
	mLoginScreen->exec();
}

void MainWindow::showSettingsDialog()
{
	mSettingsDialog->raise();
	mSettingsDialog->exec();
}

void MainWindow::saveLayouts( void ) 
{
	if( mGLFramework->saveLayouts() ) {
		mGLFramework->slotStatusMessage( QString( "Profile Saved" ), GLMessage::MESSAGE_INFO );
	} else {
		mGLFramework->slotStatusMessage( QString( "Failed to save profile" ), GLMessage::MESSAGE_ERROR );
	}
}

void MainWindow::slotUpdateWindowSettings( ProgramSettings iSettings )
{
	mGLFramework->slotShowBackground( iSettings.mShowBackground );
	mGLFramework->setBackgroundColor( iSettings.mBackgroundColor );
	mGLFramework->slotSetReelPauseTime( iSettings.mReelPauseTime );
	mGLFramework->slotSetReelSpeed( iSettings.mReelTraversalTime );
}

void MainWindow::slotStartProfileDialog( QString iUsername )
{
	mProfile->setUser( iUsername );
	mProfile->raise();
	mProfile->exec();
}

void MainWindow::slotSetupLayouts( QString iXml )
{
	mNetworker->selectedProfile( iXml );
	mGLFramework->slotSetupLayouts( iXml );
}

void MainWindow::slotExitProgram( QString iReason )
{
	log_i( "closing program, reason '" + iReason + "'." );
	gLogFile->close();
	exit( 0 );
}

void MainWindow::slotExitProgram( void )
{
	log_i( "closing program, user hit 'ESCAPE'." );
	gLogFile->close();
	exit( 0 );
}

void MainWindow::slotChangeUser( void )
{
	mProfile->done( 1 );
	mLoginScreen->done( 1 );
	mLoginScreen->exec();
}

bool MainWindow::event( QEvent* iEvent )
{
	bool oRetVal = true;
	switch( iEvent->type() ) {
		case QEvent::Show :
			mLoginScreen = new LoginScreen;
			mSettingsDialog = new SettingsDialog;
			mProfile = new Profile;
		
			setupWindow();
			setWindowTitle( "Northug" );
			setMinimumSize( 1024, 768 );
			
			mLoginScreen->exec();
			
			rebindConnects();
			resize( MainWindow::getWindowSize().x(), MainWindow::getWindowSize().y() );
			break;
		case QEvent::KeyPress :
			keyPressEvent( static_cast<QKeyEvent*>( iEvent ) );
			break;
		default :
			break;
	}
	
	return oRetVal;
}
