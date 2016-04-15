/**
  *	Visualizes the 10 latest warning-status nagios-items
  */

#ifndef NAGIOSNEWESTWARNINGS_HPP
#define NAGIOSNEWESTWARNINGS_HPP

#include <QObject>
#include "src/opengl/drawables/drawablesource.hpp"

#include "src/utils/declares.hpp"
#include "src/opengl/shapes/cell.hpp"
#include "src/opengl/shapes/row.hpp"
#include "src/opengl/shapes/table.hpp"


namespace rpnoc
{
	class NagiosNewestWarnings : public DrawableSource
	{
		Q_OBJECT

	public:
										NagiosNewestWarnings( float windowSize, QWidget *parent = 0 );
										~NagiosNewestWarnings( void );

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
	using rpnoc::NagiosNewestWarnings;
#endif

#endif // NAGIOSNEWESTWARNINGS_HPP
