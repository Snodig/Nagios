/*
 *	Class to define a vertex
 */
 
#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QtOpenGL/QGLBuffer>
#include <QtOpenGL>

#include "src/utils/declares.hpp"


namespace rpnoc
{
	template< int size = 3 >
	class Vertex
	{
	public:
							Vertex( void );
							Vertex( GLfloat iData[size] );
		GLfloat				operator[]( int iIndex );
	private:
		GLfloat				mData[size];
	};

	template< int size >
	Vertex< size >::Vertex( void ) {
		for( int i = 0; i < size; ++i ) {
			mData[i] = 0.0f;
		}
	}

	template< int size >
	Vertex< size >::Vertex( float iData[size] ) {
		for( int i = 0; i < size; ++i ) {
				mData[i] = iData[i];
		}
	};

	template< int size >
	GLfloat Vertex< size >::operator[]( int iIndex ) {
		return mData[ iIndex ];
	}
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::Vertex;
#endif

#endif // VERTEX_HPP


