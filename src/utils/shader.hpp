/*
 * Manager to add, retreive and setup shaderprograms
 */

#ifndef SHADER_HPP
#define SHADER_HPP

#include "src/utils/declares.hpp"
#include <QGLShaderProgram>

namespace rpnoc
{
	class ShaderManager
	{
	public:
		static ShaderManager&	getInstance( void );
		bool					addShader( QString iName, QString iVertex, QString iFragment );
		QGLShaderProgram*		getShader( QString iName );
		void					removeShader( QString iName );
		bool					initShaders( void );
		void					setContext( QGLWidget* iContext );
	private:
								ShaderManager( void );
								ShaderManager( const ShaderManager& iSM );
		ShaderManager			operator=( const ShaderManager& iSM );
								~ShaderManager( void );

		QMap< QString, QGLShaderProgram* > mShaders;
		QGLWidget*				mContext;
	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::ShaderManager;
#endif

#endif // SHADER_HPP
