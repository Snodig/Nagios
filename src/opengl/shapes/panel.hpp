/*
 *	A Square with borders
 */

#ifndef PANEL_H
#define PANEL_H

#include "src/utils/declares.hpp"
#include "src/opengl/shapes/square.hpp"


namespace rpnoc
{
	class Panel
	{
	public:
							Panel( QVector2D iSize,
								   QGLShaderProgram* iShaderProgram,
								   QString iTexBackground,
								   QString iTexBorderLayout );
							~Panel( void );

		void				draw(	const QMatrix4x4& iProjection,
									const QMatrix4x4& iView,
									QMatrix4x4& iModel );

		void				setBackground( QString iBackground );
		void				setBorder( QString iBorder );
		void				setSize( QVector2D iVertices );
		void				setColor( Vertex<4> iVertices[4] );
		void				setNorm( Vertex<3> iNormals[4] );
		QString				getBackground( void ) const;
		QString				getBorder( void ) const;
	private:
		Square*				mWindow[9];
		QString				mTexBorder;
		QString				mTexBackground;
		QVector2D			mSize;
	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::Panel;
#endif

#endif // PANEL_H
