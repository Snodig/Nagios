#include "src/opengl/shapes/row.hpp"

rpnoc::Row::Row( QVector2D iSize )
	: mSize( iSize ), mVerticalOffset( 0.0f )
{
	//
}

rpnoc::Row::~Row( void )
{
	//
}

bool rpnoc::Row::addCell( Cell* iCell )
{
	mCells.append( iCell );
	return true;
}

bool rpnoc::Row::addCell( Cell* iCell, QVector2D iSize ) {
	mCells.append( iCell );
        return true;
}

bool rpnoc::Row::addCell( Cell* iCell, unsigned int iIndex, QVector2D iSize ) {
	bool oRetVal = true;
        if( static_cast<int>(iIndex) > mCells.size() - 1 ) {
		oRetVal = false;
	} else {
		mCells.insert( iIndex, iCell );
	}
	
	return oRetVal;
}

QVector2D rpnoc::Row::getSize( void ) const
{
	return mSize;
}

/**
  *	The vertical offset is used to animate rows ( e.g in a table )
  */
float rpnoc::Row::getVerticalOffset( void ) const
{
    return mVerticalOffset;
}

void rpnoc::Row::setVerticalOffset( float iVertOffset )
{
    mVerticalOffset = iVertOffset;
}

void rpnoc::Row::draw( const QMatrix4x4 &iProjection, const QMatrix4x4 &iView, QMatrix4x4 iModel )
{
	QList< Cell* >::iterator oItrCell = mCells.end();
	while( oItrCell != mCells.begin() ) {
			--oItrCell;
	(*oItrCell)->draw( iProjection, iView, iModel );
	iModel.translate( (*oItrCell)->getSize().x(), 0.0f, 0.0f );
	}
}
