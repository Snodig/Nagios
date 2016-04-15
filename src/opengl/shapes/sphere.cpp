#include "sphere.hpp"
#include "src/utils/debug.hpp"


rpnoc::Sphere::Sphere( QGLShaderProgram* iShaderProgram, float radius)
	 :	Shape< (180 / 10) * (360 / 10) * 4 >( iShaderProgram ){

    angle = 0;
	const int space = 10; //Spacing between each layer

    int i;
    double a;
    double b;

	//Arrays to hold our vertices
    GLfloat oDataVertices[ mVertices ][ 3 ];
    Vertex< 3 > oVertices[ mVertices ];

    GLfloat oDataTexCoords[ mVertices ][ 2 ];
    Vertex< 2 > oVertexTexCords [ mVertices ];

    i = 0;

	//Define the sphere by these vertices
    for( b = 0; b <= 180 - space; b += space){
        for( a = 0; a <= 360 - space; a += space){

            oDataVertices[i][0] = radius * sin((a) / 180 * M_PI) * sin((b) / 180 * M_PI);
            oDataVertices[i][1] = radius * cos((a) / 180 * M_PI) * sin((b) / 180 * M_PI);
            oDataVertices[i][2] = radius * cos((b) / 180 * M_PI);

            oDataTexCoords[i][0] = b / 360;
            oDataTexCoords[i][1] = (a) / 360;

            oVertices[i] = oDataVertices[i];
            oVertexTexCords[i] = oDataTexCoords[i];

			++i;

            oDataVertices[i][0] = radius * sin((a) / 180 * M_PI) * sin((b + space) / 180 * M_PI);
            oDataVertices[i][1] = radius * cos((a) / 180 * M_PI) * sin((b + space) / 180 * M_PI);
            oDataVertices[i][2] = radius * cos((b + space) / 180 * M_PI);

            oDataTexCoords[i][0] = (b + space) / 360;
            oDataTexCoords[i][1] = (a) / 360;

            oVertices[i] = oDataVertices[i];
            oVertexTexCords[i] = oDataTexCoords[i];

			 ++i;

            oDataVertices[i][0] = radius * sin((a + space) / 180 * M_PI) * sin((b) / 180 * M_PI);
            oDataVertices[i][1] = radius * cos((a + space) / 180 * M_PI) * sin((b) / 180 * M_PI);
            oDataVertices[i][2] = radius * cos((b) / 180 * M_PI);

            oDataTexCoords[i][0] = b / 360;
            oDataTexCoords[i][1] = (a + space) / 360;

            oVertices[i] = oDataVertices[i];
            oVertexTexCords[i] = oDataTexCoords[i];

			 ++i;

            oDataVertices[i][0] = radius * sin((a + space) / 180 * M_PI) * sin((b + space) / 180 * M_PI);
            oDataVertices[i][1] = radius * cos((a + space) / 180 * M_PI) * sin((b + space) / 180 * M_PI);
            oDataVertices[i][2] = radius * cos((b + space) / 180 * M_PI);

            oDataTexCoords[i][0] = (b + space) / 360;
            oDataTexCoords[i][1] = (a + space) / 360;

            oVertices[i] = oDataVertices[i];
            oVertexTexCords[i] = oDataTexCoords[i];

			++i;
        }
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
    mBufferVertex->allocate( oVertices, sizeof( Vertex< 3 > ) * mVertices );
    mBufferVertex->release();

	//Arrays to hold our colors
    GLfloat oDataColors[ mVertices ][ 4 ];
	Vertex< 4 > oColors[ mVertices ];

	for( int i = 0; i < mVertices; ++i ){

		//Color around the surface of the sphere
        oDataColors[i][0] = 1.0f;
        oDataColors[i][1] = 0.0f;
        oDataColors[i][2] = 1.0f;
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
    mBufferColor->allocate( oColors, sizeof( Vertex< 4 > ) * mVertices );
    mBufferColor->release();

	//Arrays to hold our normals
    GLfloat oDataNormals[ mVertices ][ 3 ];
    Vertex< 3 > oVertexNormals [ mVertices ];

    for( int i = 0; i < mVertices; ++i ){

		//Normals shoot from the center out through all vertices
		oDataNormals[i][0] = oDataVertices[i][0];
		oDataNormals[i][1] = oDataVertices[i][1];
		oDataNormals[i][2] = oDataVertices[i][2];

        oVertexNormals[i] = oDataVertices[i];
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
    mBufferNormal->allocate( oVertexNormals, sizeof( Vertex< 3 > ) * mVertices );
    mBufferNormal->release();

	//Allocate a buffer for our colors ( and pass them to the GPU )
	//These were set alongside the vertices up above
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
    mBufferTexCoord->allocate( oVertexTexCords, sizeof( Vertex< 2 > ) * mVertices );
    mBufferTexCoord->release();
}


rpnoc::Sphere::~Sphere( void ) {
        //
}

void rpnoc::Sphere::draw( QMatrix4x4& iProjection, QMatrix4x4& iView, QMatrix4x4& iModel ) {

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
