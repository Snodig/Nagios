/**
  *	Visualizes the 10 latest critical-status nagios-items
  */

#ifndef NAGIOSNEWESTCRITICALS_HPP
#define NAGIOSNEWESTCRITICALS_HPP

#include <QObject>
#include "src/opengl/drawables/drawablesource.hpp"

#include "src/utils/declares.hpp"
#include "src/opengl/shapes/cell.hpp"
#include "src/opengl/shapes/row.hpp"
#include "src/opengl/shapes/table.hpp"


namespace rpnoc
{
	class NagiosNewestCriticals : public DrawableSource
	{
		Q_OBJECT

	public:
										NagiosNewestCriticals( float windowSize, QWidget *parent = 0 );
										~NagiosNewestCriticals( void );

		virtual void					init( void );
		virtual void					draw( void );

	private:
		rpnoc::Row*						mRow[10];
		rpnoc::Table*					mTable;
		rpnoc::TextCell*				mTextCellStatus[10];
		rpnoc::TextCell*				mTextCellMessage[10];
		rpnoc::ImageCell*				mImageCellOwner[10];
		rpnoc::TextFormat*				mFormatStatus;
		rpnoc::TextFormat*				mFormatMessage;
		rpnoc::TextFormat*				mFormatOwner;
	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::NagiosNewestCriticals;
#endif

#endif // NAGIOSNEWESTCRITICALS_HPP
