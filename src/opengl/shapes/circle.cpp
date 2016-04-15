#include "circle.hpp"
#include "src/utils/debug.hpp"

//Coordinate struct
struct Point {
   Point(float x, float y) : x(x), y(y) {}
   float x, y;
};

rpnoc::Circle::Circle( QGLShaderProgram* iShaderProgram, float radius)
     :	Shape< 65 >( iShaderProgram ){

    std::vector<Point> points;
    const float step = 0.1;

	points.push_back( Point( 0, 0 ) ); //Center of the circle

    for ( float a = 0 ; a < ( 2 * M_PI ); a += step) {
		//Add all points along the circumference of the circle
		points.push_back( Point( cos( a ) * radius, sin( a ) * radius ) );
    }
	points.push_back( points.at( 1 ) ); //Closes the circle

	//Arrays to hold our vertices
	GLfloat oDataVertices[ points.size() ][ 2 ];
    Vertex< 2 > oVertices[ points.size() ];

    for( unsigned int i = 0; i < points.size(); ++i ){

        oDataVertices[i][0] = points.at( i ).x;
        oDataVertices[i][1] = points.at( i ).y;

        oVertices[i] = oDataVertices[i];
    }

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
    mBufferVertex->allocate( oVertices, sizeof( Vertex< 2 > ) * points.size() );
    mBufferVertex->release();

	//Arrays to hold our colors
    GLfloat oDataColors[ points.size() ][ 4 ];
    Vertex< 4 > oColors[ points.size() ];

    //Colors for the center of the sphere (will crossfade to edges)
    oDataColors[0][0] = 0.0f;
    oDataColors[0][1] = 0.0f;
    oDataColors[0][2] = 0.0f;
    oDataColors[0][3] = 0.0f;

    oColors[0] = oDataColors[0];

    for( unsigned int i = 1; i < points.size(); ++i ){

		//Color around the circumference
        oDataColors[i][0] = 0.0f;
        oDataColors[i][1] = 0.0f;
        oDataColors[i][2] = 0.0f;
        oDataColors[i][3] = 1.0f;

        oColors[i] = oDataColors[i];
    }

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
    mBufferColor->allocate( oColors, sizeof( Vertex< 4 > ) * points.size() );
    mBufferColor->release();

	//Arrays to hold our normals ( used for shading )
    GLfloat oDataNormals[ points.size() ][ 3 ];
    Vertex< 2 > oVertexNormals [ points.size() ];

    for( unsigned int i = 0; i < points.size(); ++i ){

		//Since this is a 2D-object, all normals are along the same axis
        oDataNormals[i][0] = 0.0f;
        oDataNormals[i][1] = 0.0f;
        oDataNormals[i][2] = 1.0f;

        oVertexNormals[i] = oDataNormals[i];
	}

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
    mBufferNormal->allocate( oVertexNormals, sizeof( Vertex< 2 > ) * 3 );
    mBufferNormal->release();

	//Arrays to hold our texture-coordinates ( essentialy where we paste any texture we draw on this circle )
	GLfloat oDataTexCoords[ 4 ][ 2 ] = {
            { 2.0f, 0.0f },
            { 2.0f, 2.0f },
            { 0.0f, 0.0f },
            { 0.0f, 2.0f }
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


rpnoc::Circle::~Circle( void ) {
        //
}

void rpnoc::Circle::draw( QMatrix4x4& iProjection, QMatrix4x4& iView, QMatrix4x4& iModel ) {

	//Bind the GLSL-shader we want to use ( submitted in ctr' )
	mShaderProgram->bind();

	//Fetch locations for GLSL-attributes
	int oLocationPosition	= mShaderProgram->attributeLocation( "in_Position" );
	int oLocationColor		= mShaderProgram->attributeLocation( "in_Color" );
	int oLocationNormal		= mShaderProgram->attributeLocation( "in_Normal" );
	int oLocationTexCoord	= mShaderProgram->attributeLocation( "in_TexCoord" );

	//Fetch locations for GLSL-uniforms
	int oLocationTexture	= mShaderProgram->uniformLocation( "in_Texture" );
	int oLocationMatProj	= mShaderProgram->uniformLocation( "in_MatProj" );
	int oLocationMatView	= mShaderProgram->uniformLocation( "in_MatView" );
	int oLocationMatModel	= mShaderProgram->uniformLocation( "in_MatModel" );

	//Bind our buffers to these locations
	ERROR_TEST( mBufferVertex->bind(), "Failed to bind vertex buffer", LEVEL_ERROR )
	mShaderProgram->enableAttributeArray( oLocationPosition );
	mShaderProgram->setAttributeBuffer( oLocationPosition, GL_FLOAT, 0, 2, sizeof( Vertex< 2 > ) );

	ERROR_TEST( mBufferColor->bind(), "Failed to bind color buffer", LEVEL_ERROR )
	mShaderProgram->enableAttributeArray( oLocationColor );
	mShaderProgram->setAttributeBuffer( oLocationColor, GL_FLOAT, 0, 4, sizeof( Vertex< 4 > ) );

	ERROR_TEST( mBufferNormal->bind(), "Failed to bind normal buffer", LEVEL_ERROR )
	mShaderProgram->enableAttributeArray( oLocationNormal );
	mShaderProgram->setAttributeBuffer( oLocationNormal, GL_FLOAT, 0, 3, sizeof( Vertex< 3 > ) );

	ERROR_TEST( mBufferTexCoord->bind(), "Failed to bind texcoord buffer", LEVEL_ERROR )
	mShaderProgram->enableAttributeArray( oLocationTexCoord );
	mShaderProgram->setAttributeBuffer( oLocationTexCoord, GL_FLOAT, 0, 2, sizeof( Vertex< 2 > ) );

	//Bind uniforms that get uploaded to shader
	mShaderProgram->setUniformValue( oLocationTexture, 0 );
	mShaderProgram->setUniformValue( oLocationMatProj, iProjection );
	mShaderProgram->setUniformValue( oLocationMatView, iView );
	mShaderProgram->setUniformValue( oLocationMatModel, iModel );

	//Drawing geometry
	glDrawArrays( GL_TRIANGLE_FAN, 0, mVertices );

	//Unbinding buffers
	mShaderProgram->disableAttributeArray( oLocationPosition );
	mShaderProgram->disableAttributeArray( oLocationColor );
	mShaderProgram->disableAttributeArray( oLocationNormal );
	mShaderProgram->disableAttributeArray( oLocationTexCoord );

	//Releasing the shader ( resetting for the next object to be drawn )
	mShaderProgram->release();
}
