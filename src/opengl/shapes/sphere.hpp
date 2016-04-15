/*
 *	Class to draw a 3-dimensional sphere ( warning: looks crummy )
 */

#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "src/utils/declares.hpp"
#include "src/opengl/shapes/shape.hpp"


namespace rpnoc
{
	class Sphere : public Shape< (180 / 10) * (360 / 10) * 4  >
	{
	public:
	explicit	Sphere( QGLShaderProgram* iShaderProgram,
						 float radius );

				~Sphere( void );

	void
				draw(	QMatrix4x4& iProjection,
						QMatrix4x4& iView,
						QMatrix4x4& iModel );

	GLuint		texture[1];
	double		angle;
	int			VertexCount;

	typedef struct
	{
		int X;
		int Y;
		int Z;

	} VERTICES;


	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::Sphere;
#endif

#endif // SPHERE_HPP
