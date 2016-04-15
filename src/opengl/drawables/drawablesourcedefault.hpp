/*
 *	A placeholder for windows, when no data is bound to be displayed
 */

#ifndef DRAWABLESOURCEDEFAULT_HPP
#define DRAWABLESOURCEDEFAULT_HPP

#include <QObject>
#include "src/utils/declares.hpp"
#include "src/opengl/drawables/drawablesource.hpp"

namespace rpnoc
{
	class DrawableSourceDefault : public DrawableSource
	{

	public:
										DrawableSourceDefault( float windowSize, QWidget *parent = 0 );
										~DrawableSourceDefault( void );

		virtual void					init( void );
		virtual void					draw( void );
	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::DrawableSourceDefault;
#endif

#endif // DRAWABLESOURCEDEFAULT_HPP
