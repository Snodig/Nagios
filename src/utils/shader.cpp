/*
 *
 */

#include "src/utils/shader.hpp"
#include "src/utils/debug.hpp"


rpnoc::ShaderManager::ShaderManager( void ) {
	//
}

rpnoc::ShaderManager::~ShaderManager( void ) {
	//
}

rpnoc::ShaderManager& rpnoc::ShaderManager::getInstance( void ) {
	static ShaderManager oInstance;
	return oInstance;
}

/**
  *	Adds a shader to our list of available programs, taking
  * the name we want to retreive it by, the filename for the vertex- and fragment-shader as arguments
  */
bool rpnoc::ShaderManager::addShader( QString iName, QString iVertex, QString iFragment ) {
	QMap< QString, QGLShaderProgram* >::const_iterator oItrShader = mShaders.find( iName );
	if( oItrShader == mShaders.end() ) {
		QGLShader* oShaderVert = new QGLShader( QGLShader::Vertex, mContext );
		QGLShader* oShaderFrag = new QGLShader( QGLShader::Fragment, mContext );
		
		/**
		  *	We attempt to compile our shaders (GLSL) and debug the output should it fail
		  */
		debug_w( oShaderVert->compileSourceFile( QString( "shaders/" ) + iVertex + QString( ".vert" ) ), "Failed to compile vertex shader" );
		debug_w( oShaderFrag->compileSourceFile( QString( "shaders/" ) + iFragment + QString( ".frag" ) ), "Failed to compile fragment shader" );
		
		/**
		  *	Add the shaders to the list of available programs
		  */
		QGLShaderProgram* oProgram = new QGLShaderProgram( mContext );
		debug_w( oProgram->addShader( oShaderVert ), "Failed to add vertex shader to program" );
		debug_w( oProgram->addShader( oShaderFrag ), "Failed to add fragment shader to program" );
		
		debug_w( oProgram->link(), "Failed to link program" );
		
		//TODO: Make cleaner code
		mShaders.insert( iName, oProgram );
	} else {
		log_w( "Shader already exists. name: [ " + iName + " ]"  );
		return false;
	}
	
	
	return true;
}

/**
  *	Returns a pointer to a shaderprogram iName, NULL if it cannot find it
  */
QGLShaderProgram* rpnoc::ShaderManager::getShader( QString iName ) {
	QMap< QString, QGLShaderProgram* >::const_iterator oItrShader = mShaders.find( iName );
	if( oItrShader != mShaders.end() ) {
		return oItrShader.value();
	} else {
		log_w( "Failed to find shaderprogram" );
		oItrShader = mShaders.find( QString( "default" ) );
		return oItrShader.value();
	}
}

//TODO: Implement
void rpnoc::ShaderManager::removeShader( QString iName ) {
	QMap< QString, QGLShaderProgram* >::const_iterator oItrShader = mShaders.find( iName );
	if( oItrShader != mShaders.end() ) {
		//oItrShader.value().removeAllShaders();
	}
}


/**
  *	All shaders added here will be added to the list of available programs
  */
bool rpnoc::ShaderManager::initShaders( void ) {
	bool oRetVal = true;
	if( !addShader( "default", "default", "default" ) ) oRetVal = false;
	if( !addShader( "border_x", "default", "border_x" ) ) oRetVal = false;
	if( !addShader( "border_y", "default", "border_y" ) ) oRetVal = false;
	if( !addShader( "fancy", "fancy", "fancy" ) ) oRetVal = false;
	return oRetVal;
}

void rpnoc::ShaderManager::setContext( QGLWidget *iContext ) {
	mContext = iContext;
}
