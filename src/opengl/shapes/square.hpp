/*
 * Class to draw a simple square
 */

#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "src/utils/declares.hpp"
#include "src/opengl/shapes/shape.hpp"


namespace rpnoc
{
	class Square : public Shape< 4 >
	{
	public:
							Square( QVector2D iSize, QGLShaderProgram* iShaderProgram );
							~Square( void );

		void				draw(	const QMatrix4x4& iProjection,
									const QMatrix4x4& iView,
									QMatrix4x4& iModel );

		void				addUniform( QString iName, float iValue );

	protected:
		void				bindCustomUniforms( void );

	private:
		QList< QPair< QString, float > > mUniformsFloat;
	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::Square;
#endif

#endif // SQUARE_HPP
