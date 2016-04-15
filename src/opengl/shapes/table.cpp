/*
 *
 */

#include <QVector2D>
#include "src/opengl/shapes/row.hpp"
#include "src/opengl/shapes/table.hpp"


rpnoc::Table::Table( QVector2D iSize, bool iForceSize, unsigned int iMaxRows, float iSpacing )
	: mSize( iSize ), mForceSize( iForceSize ), mMaxRows( iMaxRows ), mAnimationSpeed( 0.007 ), mSpacing( iSpacing )
{
	//
}

rpnoc::Table::~Table( void )
{
	//
}

rpnoc::Row* rpnoc::Table::getRow( int iIndex )
{
	if( iIndex < mRows.length() || mRows.empty() ){
		return NULL;
	}else{
		return mRows.at( iIndex ).first;
	}
}

bool rpnoc::Table::addRow( Row* iRow )
{
	bool oRetVal = true;
		if( static_cast<int>(mMaxRows) == mRows.size() ) {
		oRetVal = false;
	} else {
		mRows.prepend( QPair< Row*, RowStatus >( iRow, ROW_INSERT ) );
	}
	
	return oRetVal;
}

bool rpnoc::Table::addRow( Row* iRow, int iIndex )
{
	bool oRetVal = true;
	if( static_cast<int>(mMaxRows) == mRows.size() ) {
		oRetVal = false;
	} else {
		mRows.insert( iIndex, QPair< Row*, RowStatus >( iRow, ROW_INSERT ) );
	}
	
	return oRetVal;
}

bool rpnoc::Table::removeRow( Row* iRow )
{
	bool oRetVal = false;
	for( int i = 0; i < mRows.length(); ++ i ){
		if( mRows.at( i ).first == iRow ){
			mRows[ i ].second = ROW_REMOVE;
			oRetVal = true;
			break;
		}
	}
	
	return oRetVal;
}

bool rpnoc::Table::removeRow( unsigned int iIndex )
{
	bool oRetVal = true;
        if( !mRows.empty() && static_cast<int>(iIndex) > mRows.size() - 1 ) {
		oRetVal = false;
	} else {
		mRows[iIndex].second = ROW_REMOVE;
	}
	
	return oRetVal;
}

void rpnoc::Table::draw( const QMatrix4x4 &iProjection, const QMatrix4x4 &iView, QMatrix4x4 iModel )
{
	QList< QPair < Row*, RowStatus > >::iterator oItrRow = mRows.begin();
	while( oItrRow != mRows.end() ) {
		Row* oCurrentRow = (*oItrRow).first;
		/**
		  *	Draw the rows that are being inserted
		  */
		if( (*oItrRow).second == ROW_INSERT ) {

			//Rows being inserted are drawn emerging from the background
			iModel.translate( 0,  0, -500.0f * ( 1.0f - oCurrentRow->getVerticalOffset() ) );

			//Rows being inserted are drawn quite a bit higher up
			iModel.translate( 0.0,
					-oCurrentRow->getSize().y() +
					oCurrentRow->getSize().y() * oCurrentRow->getVerticalOffset(),
					0.0 );

			//Rotate-matrix for that fresh look/feel
			QMatrix4x4 oRotate;
			oRotate.rotate( ( 90.0f - ( 90 * ( oCurrentRow->getVerticalOffset() ) ) ), 1.0f, 0.0f, 0.0f );

			//Update how far away from the other items this should be drawn next frame
			oCurrentRow->setVerticalOffset( oCurrentRow->getVerticalOffset() + mAnimationSpeed );

			//Translate by the height/2 of this row
			QMatrix4x4 oOffest = iModel;
			oOffest.translate( 0.0f, ( oCurrentRow->getSize().y() / 2 ), 0.0f );
			//Rotate it ( around it's center x-axis )
			oOffest *= oRotate;
			//And translate back
			oOffest.translate( 0.0f, - ( oCurrentRow->getSize().y() / 2 ), 0.0f );

			//Draw the row ( using the rotated model-matrix - we want to discard it after this,
			//in case it's not being inserted next frame (is idle ) )
			oCurrentRow->draw( iProjection, iView, oOffest );

			//Translate back to the where the item _should_ have been ( next row start )
			iModel.translate( 0, 0, 500.0f * ( 1.0f - oCurrentRow->getVerticalOffset() ) );

			if( oCurrentRow->getVerticalOffset() >= 0.98 ){
				//Snap to idle if it's reached it's destination
				(*oItrRow).second = ROW_IDLE;
				oCurrentRow->setVerticalOffset( 0.0 );
			}
			/**
			  *	Draw the rows that are being removed
			  */
		} else if( (*oItrRow).second == ROW_REMOVE ) {

			//Rows being inserted are drawn emerging from the background
			iModel.translate( 0,  0, -500.0f* oCurrentRow->getVerticalOffset() );

			//Rows being inserted are drawn quite a bit lower
			iModel.translate(
				0.0,
				oCurrentRow->getSize().y() * oCurrentRow->getVerticalOffset(),
				0.0
			);

			//Rotate-matrix for that fresh look/feel
			QMatrix4x4 oRotate;
			oRotate.rotate( ( -70 * oCurrentRow->getVerticalOffset() ), 1.0f, 0.0f, 0.0f );

			//Update how far away from the other items this should be drawn next frame
			oCurrentRow->setVerticalOffset( oCurrentRow->getVerticalOffset() + ( mAnimationSpeed ) );

			//Translate by the height/2 of this row
			QMatrix4x4 oOffest = iModel;
			oOffest.translate( 0.0f, ( oCurrentRow->getSize().y() / 2 ), 0.0f );
			//Rotate it ( around it's center x-axis )
			oOffest *= oRotate;
			//And Rotate back
			oOffest.translate( 0.0f, - ( oCurrentRow->getSize().y() / 2 ), 0.0f );

			if( oCurrentRow->getVerticalOffset() >= 0.98 ){
				oCurrentRow->setVerticalOffset( 0.0f );
				//Remove if it's reached it's destination
				oItrRow = mRows.erase( oItrRow );
			} else {
				//Or draw it if not
				oCurrentRow->draw( iProjection, iView, oOffest );
			}

			//Translate back to the where the item _should_ have been ( next row start )
			iModel.translate( 0.0,
				  oCurrentRow->getSize().y() -
				  oCurrentRow->getSize().y() * oCurrentRow->getVerticalOffset(),
				  0.0 );
			iModel.translate( 0, 0, 500.0f * oCurrentRow->getVerticalOffset() );

		} else if( (*oItrRow).second == ROW_IDLE ) {
			//Do nothing, just draw the row
			oCurrentRow->draw( iProjection, iView, iModel );
		}

		//Translate to the next row
		iModel.translate( 0.0f, oCurrentRow->getSize().y() + mSpacing, 0.0f );
		if( oItrRow != mRows.end() ) {
			++oItrRow;
		}
	}
}

int rpnoc::Table::getSize( void ) const
{
	return mRows.size();
}
