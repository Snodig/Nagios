#include "src/opengl/drawables/nagiosstatistics.hpp"

#include "src/opengl/shapes/format.hpp"
#include "src/network/nagiosSource.hpp"


rpnoc::NagiosStatistics::NagiosStatistics( float windowSize, QWidget *parent )
	: DrawableSource( "Nagios - Statistics", "NAGIOS", windowSize, parent )
{
	//
}

rpnoc::NagiosStatistics::~NagiosStatistics( void )
{
	//
}

void rpnoc::NagiosStatistics::init( void )
{
	//All subclasses must call the base::init()
	DrawableSource::init();

	/**
	  *	From here we set up the text-formats, cells and rows that will be drawn
	  */

	mFormatCounts = new TextFormat(
			QString( "charset.png" ),
			8,
			1,
			-10,
			33,
			33,
			QVector4D( 1.0f, 1.0f, 1.0f, 1.0f )
	);

	mFormatUpdateTime = new TextFormat(
			QString( "charset.png" ),
			1240,
			1,
			-30,
			60,
			60,
			QVector4D( 0.0f, 0.0f, 0.0f, 1.0f )
	);

	//Row 1
	mRowUpdateTime = new Row( QVector2D( mFrameBuffer->width(), mFrameBuffer->height() / 2 ) );

	mTextCellUpdateTime = new TextCell( QVector2D( mFrameBuffer->height(), mFrameBuffer->height() / 2 ),
										QVector2D( 20, 20 ), *mFormatUpdateTime,
										QString( "" ) );

	mRowUpdateTime->addCell( mTextCellUpdateTime );

	//Row 2
	mRowCounts = new Row( QVector2D( mFrameBuffer->width(), mFrameBuffer->height() / 2 ) );

	//Column 1
	mTextCellCounts[0] = new TextCell(	QVector2D( mFrameBuffer->height() / 3, mFrameBuffer->height() / 2 ),
										QVector2D( 20, 20 ), *mFormatCounts,
										QString( "" ) );
	//Column 2
	mTextCellCounts[1] = new TextCell(	QVector2D( mFrameBuffer->height() / 3, mFrameBuffer->height() / 2 ),
										QVector2D( 20, 20 ), *mFormatCounts,
										QString( "" ) );
	//Column 3
	mTextCellCounts[2] = new TextCell(	QVector2D( mFrameBuffer->height() / 3, mFrameBuffer->height() / 2 ),
										QVector2D( 20, 20 ), *mFormatCounts,
										QString( "" ) );

	mRowCounts->addCell( mTextCellCounts[0] );
	mRowCounts->addCell( mTextCellCounts[1] );
	mRowCounts->addCell( mTextCellCounts[2] );
}

void rpnoc::NagiosStatistics::draw()
{
	if( mDataSource->getServerData() == NULL ){

		//To make sure we don't try to fetch data from a NULL-pointer array
		return;
	}

	mTextCellUpdateTime->setText( "IP:" + mDataSource->getIP() + " - " + QString::number( mDataSource->getTimeSinceLastUpdate() ) + " ms" );

	mRowUpdateTime->draw( mProjection, mView, mModel ); //Row 1

	//Column 1
	mTextCellCounts[0]->setText(	QString( "Critical: " +
									QString::number( ( (NagiosSource*)mDataSource)->getCountCritical() ) ) );
	mTextCellCounts[0]->setBackGroundColor( QVector4D( 1.0, 0.0, 0.0, 1.0 ) );
	//Column 2
	mTextCellCounts[1]->setText(	QString( "Warnings: " +
									QString::number( ( (NagiosSource*)mDataSource)->getCountWarning() ) ) );
	mTextCellCounts[1]->setBackGroundColor( QVector4D( 0.0, 1.0, 1.0, 1.0 ) );
	//Column 3
	mTextCellCounts[2]->setText(	QString( "Unknowns: " +
									QString::number( ( (NagiosSource*)mDataSource)->getCountUnknown() ) ) );
	mTextCellCounts[2]->setBackGroundColor( QVector4D( 0.7, 0.1, 0.2, 1.0 ) );


	mModel.translate( 0.0f, mRowUpdateTime->getSize().y(), 0.0f );
	mRowCounts->draw( mProjection, mView, mModel );					//Row 2

	//Translate back for Row 1 in the next cycle of draw()
	mModel.translate( 0.0f, -mRowUpdateTime->getSize().y(), 0.0f );
}
