/*
 *	Class to draw a simple circle ( or ellipse )
 */

#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "src/utils/declares.hpp"
#include "src/opengl/shapes/shape.hpp"


namespace rpnoc
{
	class Circle : public Shape< 65 >
	{
	public:
					explicit Circle( QGLShaderProgram* iShaderProgram, float radius );

					~Circle( void );

	void			draw(	QMatrix4x4& iProjection,
							QMatrix4x4& iView,
							QMatrix4x4& iModel );

	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::Circle;
#endif

#endif // CIRCLE_HPP
