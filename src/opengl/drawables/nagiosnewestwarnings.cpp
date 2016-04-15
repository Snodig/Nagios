#include "src/opengl/drawables/nagiosnewestwarnings.hpp"

#include "src/utils/pluginmanager.hpp"
#include "src/widgets/glframework.hpp"
#include "src/opengl/layout.hpp"
#include "src/opengl/shapes/format.hpp"
#include "src/network/networker.hpp"
#include "src/network/nagiosSource.hpp"

rpnoc::NagiosNewestWarnings::NagiosNewestWarnings( float windowSize, QWidget *parent )
	: DrawableSource( "Nagios - Newest Warnings", "NAGIOS", windowSize, parent )
{
	//
}

rpnoc::NagiosNewestWarnings::~NagiosNewestWarnings( void )
{
	//
}

void rpnoc::NagiosNewestWarnings::init( void )
{
	DrawableSource::init(); //Must be called

	/**
	  *	From here we set up the text-formats, cells and rows that will be drawn
	  */
	mFormatOwner = new TextFormat(
			QString( "charset.png" ),
			8,
			1,
			-10,
			33,
			33,
			QVector4D( 1.0f, 1.0f, 1.0f, 1.0f )
	);

	mFormatStatus = new TextFormat(
			QString( "charset.png" ),
			1240,
			1,
			-30,
			60,
			60,
			QVector4D( 0.0f, 0.0f, 0.0f, 1.0f )
	);

	mFormatMessage = new TextFormat(
			QString( "charset.png" ),
			1240,
			1,
			-20,
			35,
			35,
			QVector4D( 0.0f, 0.0f, 0.0f, 1.0f )
	);

	//The table to hold our rows of cells
	mTable = new Table( QVector2D( mFrameBuffer->width(), mFrameBuffer->height() ));

	//10 rows of cells
	for( int i = 0; i < 10; ++i ) {
		mRow[i] = new Row( QVector2D( mFrameBuffer->width(), mFrameBuffer->height() / 10 ) );

		//3 cells to a row
		//Column 1
		mImageCellOwner[i] = new ImageCell( QVector2D( mFrameBuffer->height() / 4, mFrameBuffer->height() / 10 ),
			QString( "ubuntu.png" ) );
		//Column 2
		mTextCellStatus[i] = new TextCell( QVector2D( mFrameBuffer->height() / 4, mFrameBuffer->height() / 10 ), QVector2D( 20, 20 ), *mFormatStatus,
										   QString( "" ) );
		//Column 3
		mTextCellMessage[i] = new TextCell( QVector2D( mFrameBuffer->height() / 2, mFrameBuffer->height() / 10 ), QVector2D( 20, 20 ), *mFormatMessage,
											QString( "" ) );

		mRow[i]->addCell( mTextCellMessage[i] );
		mRow[i]->addCell( mTextCellStatus[i] );
		mRow[i]->addCell( mImageCellOwner[i] );

		mTable->addRow( mRow[i], 0 );
	}
}

void rpnoc::NagiosNewestWarnings::draw()
{
	if( mDataSource->getServerData() == NULL ){
		//To make sure we don't try to fetch data from a NULL-pointer array
		return;
	}

	for( int i = 0; i < mTable->getSize(); ++i ) {
		//Set the text for the cells per row
		mTextCellStatus[i]->setText(
				((NagiosSource*)mDataSource)->getAllOfStatus("WARNING")[i][NagiosSource::SERVER_STATUS] );
		mTextCellMessage[i]->setText(
				((NagiosSource*)mDataSource)->getAllOfStatus("WARNING")[i][NagiosSource::OUTPUT] );

		//Color me teal
		if( mTextCellStatus[i]->getText() == "CRITICAL" ) {
			mImageCellOwner[i]->setBackGroundColor( QVector4D( 1.0, 0.0, 0.0, 1.0 ) );
		} else if( mTextCellStatus[i]->getText() ==  "WARNING" ) {
			mImageCellOwner[i]->setBackGroundColor( QVector4D( 0.0, 1.0, 1.0, 1.0 ) );
		} else if( mTextCellStatus[i]->getText() ==  "OK" ) {
			mImageCellOwner[i]->setBackGroundColor( QVector4D( 0.0, 1.0, 0.0, 1.0 ) );
		} else if( mTextCellStatus[i]->getText() == "UNKNOWN" ) {
			mImageCellOwner[i]->setBackGroundColor( QVector4D( 0.7, 0.1, 0.2, 1.0 ) );
		}
	}

	//Cycle the conveyer-belt table, this is what triggers animation
	static int oRow = 0;
	mTable->removeRow( mTable->getSize() - 1 );
	if( mTable->addRow( mRow[oRow], 0 ) ) {
		oRow++;

		if( oRow == 10 ) oRow = 0;
	}

	//Display ( and animate ) the table
	mTable->draw( mProjection, mView, mModel );
}
