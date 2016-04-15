/*
 * Container-class for cells
 */

#ifndef ROW_HPP
#define ROW_HPP

#include <QList>
#include <QPair>
#include <QVector2D>

#include "src/utils/declares.hpp"
#include "src/opengl/shapes/cell.hpp"


namespace rpnoc
{
	class Row
	{
	public:
								Row( QVector2D iSize );
								~Row( void );

		bool					addCell( Cell* iCell );
		bool					addCell( Cell* iCell, QVector2D iSize );
		bool					addCell( Cell* iCell, unsigned int iIndex, QVector2D iSize );
	
		QVector2D				getSize( void ) const;
		float					getVerticalOffset( void ) const;
		void					setVerticalOffset( float iVertOffset );

		void					draw( const QMatrix4x4& iProjection, const QMatrix4x4& iView, QMatrix4x4 iModel );
	private:
		//
		QVector2D				mSize;
		/*
		 * Used to animate insertion and removal
		 */
		float					mVerticalOffset;
		QList< rpnoc::Cell* >	mCells;
	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::Row;
#endif

#endif // ROW_HPP
