/*
 *
 */

#include "pluginmanager.hpp"


rpnoc::PluginManager& gPluginManager = rpnoc::PluginManager::getInstance();


rpnoc::PluginManager::PluginManager( void )
{
	//
}

rpnoc::PluginManager::~PluginManager( void )
{
	//
}

rpnoc::PluginManager& rpnoc::PluginManager::getInstance( void )
{
	static PluginManager oInstance;
	return oInstance;
}

/**
  *	Adds a plugin to the managers lists, so that we can retreive it later, and add it to a window
  */
bool rpnoc::PluginManager::addPlugin( rpnoc::DrawableSource* iDrawableSource )
{
	bool oRetVal = true;
	QVector< rpnoc::DrawableSource* >::iterator oItrPlugin = mPlugins.begin();
	while( oItrPlugin != mPlugins.end() ) {
		if( (*oItrPlugin) == iDrawableSource ) break;
		++oItrPlugin;
	}
	
	if( oItrPlugin == mPlugins.end() ) {
		mPlugins.append( iDrawableSource );
	} else {
		oRetVal = false;
	}
	
	return oRetVal;
}

void rpnoc::PluginManager::remPlugin( rpnoc::DrawableSource* iDrawableSource )
{
	//TODO: Implement
}

/**
  *	Returns a pointer to plugin iName, NULL if it cannot find it
  */
rpnoc::DrawableSource* rpnoc::PluginManager::getPlugin( QString iName )
{
	QVector< rpnoc::DrawableSource* >::iterator oItrPlugin = mPlugins.begin();
	while( oItrPlugin != mPlugins.end() ) {
		if( (*oItrPlugin)->getName() == iName ) {
			return (*oItrPlugin);
		} 
		++oItrPlugin;
	}
	
	// TODO: return a default plugin if it fails to find plugin
	return NULL;
}

/**
  *	Calls init for all DrawableSources added
  */
void rpnoc::PluginManager::initPlugins( void )
{
	QVector< rpnoc::DrawableSource* >::iterator oItrDrawSource = mPlugins.begin();
	while( oItrDrawSource != mPlugins.end() ) {
		(*oItrDrawSource)->init();
		++oItrDrawSource;
	}
}
