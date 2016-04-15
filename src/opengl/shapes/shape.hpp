/*
 *	A base-class for all geometricals
 */

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <QMatrix4x4>
#include <QGLBuffer>
#include <QGLShaderProgram>
#include "src/utils/declares.hpp"
#include "src/opengl/shapes/vertex.hpp"


namespace rpnoc
{
	template< int size >
	class Shape
	{
	public:
							Shape( QGLShaderProgram* iShaderProgram );
		virtual				~Shape( void );

		virtual void		draw(	const QMatrix4x4& iProjection,
									const QMatrix4x4& iView,
									QMatrix4x4& iModel ) = 0;

		void				updateVertices( Vertex<3> iVertices[ size ] );
		void				updateColors( Vertex<4> iColors[ size ] );
		void				updateNormals( Vertex<3> iNormals[ size ] );
		void				updateTexCoords( Vertex<2> iTexCoords[ size ] );

		int					count( void ) const;
		void				setShader( QGLShaderProgram* iShaderProgram );
		QGLShaderProgram*	getShader( void );
	protected:
		QGLBuffer*			mBufferVertex;
		QGLBuffer*			mBufferColor;
		QGLBuffer*			mBufferNormal;
		QGLBuffer*			mBufferTexCoord;
		QGLShaderProgram*	mShaderProgram;
		int					mVertices;
	};

	template< int size >
	Shape< size >::Shape( QGLShaderProgram* iShaderProgram )
		:	mShaderProgram( iShaderProgram ), mVertices( size ) {
		//
	}

	template< int size >
	Shape< size >::~Shape( void ) {
		delete mBufferVertex;
		delete mBufferColor;
		delete mBufferNormal;
		delete mBufferTexCoord;
	}

	/**
	  *	Writes new vertex-array to the GPU
	  */
	template< int size >
	void Shape< size >::updateVertices( Vertex<3> iVertices[ size ] ) {
		mBufferVertex->bind();
		mBufferVertex->write( 0, iVertices, size * sizeof( Vertex<3> ) );
		mBufferVertex->release();
	}

	/**
	  *	Writes new color-array to the GPU
	  */
	template< int size >
	void Shape< size >::updateColors( Vertex<4> iColors[ size ] ) {
		mBufferColor->bind();
		mBufferColor->write( 0, iColors, size * sizeof( Vertex<4> ) );
		mBufferColor->release();
	}

	/**
	  *	Writes new normal-array to the GPU
	  */
	template< int size >
	void Shape< size >::updateNormals( Vertex<3> iNormals[ size ] ) {
		mBufferNormal->bind();
		mBufferNormal->write( 0, iNormals, size * sizeof( Vertex<3> ) );
		mBufferNormal->release();
	}

	/**
	  *	Writes new texcoord-array to the GPU
	  */
	template< int size >
	void Shape< size >::updateTexCoords( Vertex<2> iTexCoords[ size ] ) {
		mBufferTexCoord->bind();
		mBufferTexCoord->write( 0, iTexCoords, size * sizeof( Vertex<2> ) );
		mBufferTexCoord->release();
	}

	template< int size >
	int Shape< size >::count( void ) const {
		return mVertices;
	}

	template< int size >
	void Shape< size >::setShader( QGLShaderProgram* iShaderProgram ) {
		mShaderProgram = iShaderProgram;
	}

	template< int size >
	QGLShaderProgram* Shape< size >::getShader( void ) {
		return mShaderProgram;
	}
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::Shape;
#endif

#endif // SHAPE_HPP
