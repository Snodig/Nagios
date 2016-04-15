/**
  *	This baseclass handles vizualisation of data retreived from the affiliatedDataSource-class,
  * as well as some conveniance-functions
  */

#ifndef DRAWABLESOURCE_HPP
#define DRAWABLESOURCE_HPP

#include <QObject>
#include <QtOpenGL>
#include <QAction>

#include "src/utils/declares.hpp"
#include "src/network/dataSource.hpp"

namespace rpnoc
{
	class DrawableSource : public QWidget
	{
		Q_OBJECT

	public:
		explicit DrawableSource( QString name, QString datasourceName, float windowSize,
								 QWidget *parent = 0);

		virtual ~DrawableSource();

		QString                         getName( void )				const;
		void                            setName( QString name );
		QString							getSourceName( void )		const;
		bool							setSourcePointer( QString iName );

		/**
		  *	Access-functions for the FrameBuffer-Object( oGL )
		  */
		void							bindBuffer( void );
		void							unbindBuffer( void );
		GLuint                          getFBOTexture()				const;

		/**
		*	All subclasses must call the base::init() !
		*/
		virtual void					init( void )=0;
		virtual void                    draw( void )=0;

	protected:
		QGLFramebufferObject*           mFrameBuffer;

		QMatrix4x4						mProjection;
		QMatrix4x4						mView;
		QMatrix4x4						mModel;

		QString                         mName;
		QString							mDataSourceName;
		float							mWindowSize;
		DataSource*						mDataSource;

		//This QAction* is used to add the Drawable to the menu-lists
		QAction*						mActionOnClick;
	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::DrawableSource;
#endif

#endif // DRAWABLESOURCE_HPP
