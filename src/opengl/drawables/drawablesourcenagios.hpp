/**
  *	Visualizes a very general overview of the latest nagios-related items
  */

#ifndef DRAWABLESOURCENAGIOS_HPP
#define DRAWABLESOURCENAGIOS_HPP

#include <QObject>
#include "src/opengl/drawables/drawablesource.hpp"

#include "src/utils/declares.hpp"
#include "src/opengl/shapes/cell.hpp"
#include "src/opengl/shapes/row.hpp"
#include "src/opengl/shapes/table.hpp"


namespace rpnoc
{
	class DrawableSourceNagios : public DrawableSource
	{
		Q_OBJECT

	public:
										DrawableSourceNagios( float windowSize, QWidget *parent = 0 );
										~DrawableSourceNagios( void );

		virtual void					init( void );
		virtual void					draw( void );

	private:
		rpnoc::Row*						mRow[5];
		rpnoc::Table*					mTable;
		rpnoc::TextCell*				mTextCellStatus[5];
		rpnoc::TextCell*				mTextCellMessage[5];
		rpnoc::ImageCell*				mImageCellOwner[5];
		rpnoc::TextFormat*				mFormatStatus;
		rpnoc::TextFormat*				mFormatMessage;
		rpnoc::TextFormat*				mFormatOwner;

	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::DrawableSourceNagios;
#endif

#endif // DRAWABLESOURCENAGIOS_HPP
