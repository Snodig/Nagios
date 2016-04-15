/*
 *	Class to draw a simple triangle
 */
 
#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "src/utils/declares.hpp"
#include "src/opengl/shapes/shape.hpp"


namespace rpnoc
{
	class Triangle : public Shape< 3 >
	{
	public:
					Triangle( QGLShaderProgram* iShaderProgram,
								float baseWidth, float height, float highVertOffset=0 );
					~Triangle( void );

	void
					draw(	QMatrix4x4& iProjection,
							QMatrix4x4& iView,
							QMatrix4x4& iModel );
	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::Triangle;
#endif

#endif // TRIANGLE_HPP
