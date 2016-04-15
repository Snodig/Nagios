/*
 *	Container-class for Rows of Cells, visualizeable as an animated spreadsheet
 */

#ifndef TABLE_HPP
#define TABLE_HPP

#include <QList>
#include <QPair>
#include <QMatrix4x4>
#include <QVector2D>
#include <QObject>
#include "src/utils/declares.hpp"
#include "src/opengl/shapes/row.hpp"


namespace rpnoc
{
	class Table : public QObject
	{
		Q_OBJECT
	public:
							Table( QVector2D iSize, bool iForceSize = true,
								   unsigned int iMaxRows = 10, float iSpacing = 15.0f );
							~Table( void );

		bool				addRow( rpnoc::Row* iRow );
		bool				addRow( rpnoc::Row* iRow, int iIndex );
		bool				removeRow( rpnoc::Row* iRow );
		bool				removeRow( unsigned int iIndex );
		Row*				getRow( int iIndex );
		int					getSize( void ) const;

		void				draw( const QMatrix4x4& iProjection, const QMatrix4x4& iView, const QMatrix4x4 iModel );

	private:
			enum RowStatus
			{
				ROW_INSERT,
				ROW_IDLE,
				ROW_REMOVE
			};

			QVector2D			mSize;
			bool				mForceSize;
			unsigned int		mMaxRows;
			float				mAnimationSpeed;
			float				mSpacing;
			QList< QPair< rpnoc::Row*, RowStatus > > mRows;
	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::Table;
#endif

#endif // TABLE_HPP
