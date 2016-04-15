/*
 *	A very simple clock
 */

#ifndef DRAWABLESOURCECLOCK_HPP
#define DRAWABLESOURCECLOCK_HPP

#include <QObject>
#include "drawablesource.hpp"

#include "src/utils/declares.hpp"
#include "src/opengl/shapes/cell.hpp"
#include "src/opengl/shapes/format.hpp"


namespace rpnoc
{
	class DrawableSourceClock : public DrawableSource
	{
	public:
										DrawableSourceClock( float iWindowSize, QWidget* iParent = NULL );
										~DrawableSourceClock( void );

		virtual void					init( void );
		virtual void					draw( void );

	private:
		rpnoc::TextCell*				mTime;
		rpnoc::TextFormat*				mTextFormat;
	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::DrawableSourceClock;
#endif

#endif // DRAWABLESOURCECLOCK_HPP
