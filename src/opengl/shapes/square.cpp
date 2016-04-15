/*
 *
 */

#include "src/opengl/shapes/square.hpp"
#include "src/utils/debug.hpp"

rpnoc::Square::Square( QVector2D iSize, QGLShaderProgram* iShaderProgram )
	:	Shape< 4 >( iShaderProgram )
{

	//Arrays to hold our vertices
	GLfloat oDataVertices[ 4 ][ 3 ] = {
		{ iSize.x(),	0.0f,		0.0f },
		{ iSize.x(),	iSize.y(),	0.0f },
		{ 0.0f,			0.0f,		0.0f },
		{ 0.0f,			iSize.y(),	0.0f }
	};
	Vertex< 3 > oVertexVertices[ 4 ] = {
		oDataVertices[0],
		oDataVertices[1],
		oDataVertices[2],
		oDataVertices[3]
	};

	//Allocate a buffer for our vertices ( and pass them to the GPU )
	mBufferVertex = new QGLBuffer( QGLBuffer::VertexBuffer );
	ERROR_TEST( 
		mBufferVertex->create(),
		"Failed to create vertex buffer",
		LEVEL_ERROR
	)
	ERROR_TEST(
		mBufferVertex->bind(),
		"Failed to bind vertex buffer",
		LEVEL_ERROR
	)

	mBufferVertex->setUsagePattern( QGLBuffer::DynamicDraw );
	mBufferVertex->allocate( oVertexVertices, sizeof( Vertex< 3 > ) * 4 );
	mBufferVertex->release();
	
	//Arrays to hold our colors
	GLfloat oDataColors[ 4 ][ 4 ] = {
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f }
	};
	Vertex< 4 > oVertexColors[ 4 ] = {
		oDataColors[0],
		oDataColors[1],
		oDataColors[2],
		oDataColors[3]
	};

	//Allocate a buffer for our colors ( and pass them to the GPU )
	mBufferColor = new QGLBuffer( QGLBuffer::VertexBuffer );
	ERROR_TEST( 
		mBufferColor->create(),
		"Failed to create color buffer",
		LEVEL_ERROR
	)
	ERROR_TEST( 
		mBufferColor->bind(),
		"Failed to bind color buffer",
		LEVEL_ERROR
	)

	mBufferColor->setUsagePattern( QGLBuffer::DynamicDraw );
	mBufferColor->allocate( oVertexColors, sizeof( Vertex< 4 > ) * 4 );
	mBufferColor->release();
	
	//Arrays to hold our normals
	GLfloat oDataNormals[ 4 ][ 3 ] = {
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f }
	};
	Vertex< 3 > oVertexNormals[ 4 ] = {
		oDataNormals[ 1 ],
		oDataNormals[ 1 ],
		oDataNormals[ 2 ],
		oDataNormals[ 3 ]
	};

	//Allocate a buffer for our normals ( and pass them to the GPU )
	mBufferNormal = new QGLBuffer( QGLBuffer::VertexBuffer );
	ERROR_TEST( 
		mBufferNormal->create(),
		"Failed to create normal buffer",
		LEVEL_ERROR
	)
	ERROR_TEST(
		mBufferNormal->bind(),
		"Failed to bind normal buffer",
		LEVEL_ERROR
	)

	mBufferNormal->setUsagePattern( QGLBuffer::DynamicDraw );
	mBufferNormal->allocate( oVertexNormals, sizeof( Vertex< 3 > ) * 4 );
	mBufferNormal->release();
	
	//Arrays to hold our texture-coordinates
	GLfloat oDataTexCoords[ 4 ][ 2 ] = {
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, 1.0f }
	};
	Vertex< 2 > oVertexTexCords[ 4 ] = {
		oDataTexCoords[ 0 ],
		oDataTexCoords[ 1 ],
		oDataTexCoords[ 2 ],
		oDataTexCoords[ 3 ]
	};

	//Allocate a buffer for our texture-coordinates ( and pass them to the GPU )
	mBufferTexCoord = new QGLBuffer( QGLBuffer::VertexBuffer );
	ERROR_TEST(
		mBufferTexCoord->create(),
		"Failed to create texcoord buffer",
		LEVEL_ERROR
	)
	ERROR_TEST(
		mBufferTexCoord->bind(),
		"Failed to bind texcoord buffer",
		LEVEL_ERROR
	)

	mBufferTexCoord->setUsagePattern( QGLBuffer::DynamicDraw );
	mBufferTexCoord->allocate( oVertexTexCords, sizeof( Vertex< 2 > ) * 4 );
	mBufferTexCoord->release();
}

rpnoc::Square::~Square( void )
{
	//
}

void rpnoc::Square::draw( const QMatrix4x4& iProjection, const QMatrix4x4& iView, QMatrix4x4& iModel )
{

	//Bind the GLSL-shader we want to use ( submitted in ctr' )
	mShaderProgram->bind();

	//Fetch locations for GLSL-attributes
	int oLocationPosition = mShaderProgram->attributeLocation( "in_Position" );
	int oLocationColor = mShaderProgram->attributeLocation( "in_Color" );
	int oLocationNormal = mShaderProgram->attributeLocation( "in_Normal" );
	int oLocationTexCoord = mShaderProgram->attributeLocation( "in_TexCoord" );

	//Fetch locations for GLSL-uniforms
	int oLocationTexture = mShaderProgram->uniformLocation( "in_Texture" );
	int oLocationMatProj = mShaderProgram->uniformLocation( "in_MatProj" );
	int oLocationMatView = mShaderProgram->uniformLocation( "in_MatView" );
	int oLocationMatModel = mShaderProgram->uniformLocation( "in_MatModel" );

	//Bind our buffers to these locations
	ERROR_TEST( mBufferVertex->bind(), "Failed to bind vertex buffer", LEVEL_ERROR )
	mShaderProgram->enableAttributeArray( oLocationPosition );
	mShaderProgram->setAttributeBuffer( oLocationPosition, GL_FLOAT, 0, 3, sizeof( Vertex< 3 > ) );
	
	ERROR_TEST( mBufferColor->bind(), "Failed to bind color buffer", LEVEL_ERROR )
	mShaderProgram->enableAttributeArray( oLocationColor );
	mShaderProgram->setAttributeBuffer( oLocationColor, GL_FLOAT, 0, 4, sizeof( Vertex< 4 > ) );
	
	ERROR_TEST( mBufferNormal->bind(), "Failed to bind normal buffer", LEVEL_ERROR )
	mShaderProgram->enableAttributeArray( oLocationNormal );
	mShaderProgram->setAttributeBuffer( oLocationNormal, GL_FLOAT, 0, 3, sizeof( Vertex< 3 > ) );
	
	ERROR_TEST( mBufferTexCoord->bind(), "Failed to bind texcoord buffer", LEVEL_ERROR )
	mShaderProgram->enableAttributeArray( oLocationTexCoord );
	mShaderProgram->setAttributeBuffer( oLocationTexCoord, GL_FLOAT, 0, 2, sizeof( Vertex< 2 > ) );
	
	bindCustomUniforms();

	//Bind uniforms that get uploaded to shader
	mShaderProgram->setUniformValue( oLocationTexture, 0 );
	mShaderProgram->setUniformValue( oLocationMatProj, iProjection );
	mShaderProgram->setUniformValue( oLocationMatView, iView );
	mShaderProgram->setUniformValue( oLocationMatModel, iModel );

	//Drawing geometry
	glDrawArrays( GL_TRIANGLE_STRIP, 0, mVertices );

	//Unbinding buffers
	mShaderProgram->disableAttributeArray( oLocationPosition );
	mShaderProgram->disableAttributeArray( oLocationColor );
	mShaderProgram->disableAttributeArray( oLocationNormal );
	mShaderProgram->disableAttributeArray( oLocationTexCoord );

	//Releasing the shader ( resetting for the next object to be drawn )
	mShaderProgram->release();
}

void rpnoc::Square::bindCustomUniforms( void )
{
	QList< QPair< QString, float > >::iterator oIterFloatUniforms = mUniformsFloat.begin();
	while( oIterFloatUniforms != mUniformsFloat.end() ) {
		/*
		 * Try to hook up the uniform to the shader
		 */
		QString oUniform = QString( oIterFloatUniforms->first );
		int oLocation = mShaderProgram->uniformLocation( oUniform );
		mShaderProgram->setUniformValue( oLocation, oIterFloatUniforms->second );
		
		oIterFloatUniforms = mUniformsFloat.erase( oIterFloatUniforms );
	}
}

void rpnoc::Square::addUniform( QString iName, float iValue )
{
	mUniformsFloat.append( QPair< QString, float >( iName, iValue ) );
}


