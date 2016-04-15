/*
 *
 */

#include "src/opengl/shapes/triangle.hpp"
#include "src/utils/debug.hpp"

rpnoc::Triangle::Triangle( QGLShaderProgram* iShaderProgram,
                    float baseWidth, float height, float highVertOffset )
		: Shape< 3 >( iShaderProgram ) {
        //

//    height = -height;
    float centerXOffset = -( baseWidth / 2 );
    float centerYOffset = -( height / 2 );
    float offset = ( baseWidth / 2 ) + highVertOffset;

    GLfloat oDataVertices[ 3 ][ 3 ] = {

        { centerXOffset,            centerYOffset,          0.0f },// Bottom L
        { offset+centerXOffset,     height+centerYOffset,   0.0f },// Top
        { baseWidth+centerXOffset,  centerYOffset,          0.0f } // Bottom R
    };

    Vertex< 3 > oVertices[ 3 ] = {

        oDataVertices[ 0 ],
        oDataVertices[ 1 ],
        oDataVertices[ 2 ],
    };

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
    mBufferVertex->allocate( oVertices, sizeof( Vertex< 3 > ) * 3 );
    mBufferVertex->release();

    GLfloat oDataColors[ 3 ][ 4 ] = {
            { 0.0f, 0.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f }
    };

    Vertex< 4 > oColors[ 3 ] = {
            oDataColors[0],
            oDataColors[1],
            oDataColors[2]
    };
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
    mBufferColor->allocate( oColors, sizeof( Vertex< 4 > ) * 3 );
    mBufferColor->release();

    GLfloat oDataNormals[ 3 ][ 3 ] = {
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f }
    };
    Vertex< 3 > oVertexNormals[ 3 ] = {
            oDataNormals[ 1 ],
            oDataNormals[ 1 ],
            oDataNormals[ 2 ]
    };
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
    mBufferNormal->allocate( oVertexNormals, sizeof( Vertex< 3 > ) * 3 );
    mBufferNormal->release();

    GLfloat oDataTexCoords[ 4 ][ 2 ] = { //TODO: reset ( 2.0 -> 1.0
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

rpnoc::Triangle::~Triangle( void ) {
        //
}

void rpnoc::Triangle::draw( QMatrix4x4& iProjection, QMatrix4x4& iView, QMatrix4x4& iModel ) {

        mShaderProgram->bind();

        int oLocationPosition = mShaderProgram->attributeLocation( "in_Position" );
        int oLocationColor = mShaderProgram->attributeLocation( "in_Color" );
        int oLocationNormal = mShaderProgram->attributeLocation( "in_Normal" );
        int oLocationTexCoord = mShaderProgram->attributeLocation( "in_TexCoord" );

        int oLocationTexture = mShaderProgram->uniformLocation( "in_Texture" );
        int oLocationMatProj = mShaderProgram->uniformLocation( "in_MatProj" );
        int oLocationMatView = mShaderProgram->uniformLocation( "in_MatView" );
        int oLocationMatModel = mShaderProgram->uniformLocation( "in_MatModel" );

        // binding buffers
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

        // binding values that get uploaded to shader
        mShaderProgram->setUniformValue( oLocationTexture, 0 );
        mShaderProgram->setUniformValue( oLocationMatProj, iProjection );
        mShaderProgram->setUniformValue( oLocationMatView, iView );
        mShaderProgram->setUniformValue( oLocationMatModel, iModel );

        // drawing shader
        glDrawArrays( GL_TRIANGLE_STRIP, 0, mVertices );

        // unbinding buffers
        mShaderProgram->disableAttributeArray( oLocationPosition );
        mShaderProgram->disableAttributeArray( oLocationColor );
        mShaderProgram->disableAttributeArray( oLocationNormal );
        mShaderProgram->disableAttributeArray( oLocationTexCoord );

        mShaderProgram->release();
}
