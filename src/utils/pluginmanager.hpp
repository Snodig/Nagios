/*
 * Manager to retreive DrawableSources ( so that we can display them in windows )
 */

#ifndef PLUGINMANAGER_HPP
#define PLUGINMANAGER_HPP

#include "src/utils/declares.hpp"
#include "src/opengl/drawables/drawablesource.hpp"


namespace rpnoc
{
	class PluginManager
	{
	public:
		static PluginManager&	getInstance( void );

		bool					addPlugin( rpnoc::DrawableSource* iDrawableSource );
		void					remPlugin( rpnoc::DrawableSource* iDrawableSource );
		rpnoc::DrawableSource*			getPlugin( QString iName );
		void					initPlugins( void );
	private:
								PluginManager( void );
								~PluginManager( void );
								PluginManager( const PluginManager& iPluginManager );
		const PluginManager&	operator=( const PluginManager& iPluginManager );
	private:
		QVector< rpnoc::DrawableSource* > mPlugins;
	};
}

extern rpnoc::PluginManager& gPluginManager;

#endif // PLUGINMANAGER_HPP
