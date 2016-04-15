
#ifndef _BACKGROUND_HPP_
#define _BACKGROUND_HPP_

#include <QtOpenGL>
#include <QMatrix4x4>
#include "src/opengl/shapes/square.hpp"

#define WAVE_ROW 20
#define WAVE_COL 20
#define INTERVAL 5

class Background{

public:
						Background();
						~Background();

	void				init( void );
	void				render( void );

private:
	QMatrix4x4			mProjection;
	QMatrix4x4			mView;
	QMatrix4x4			mModel;
	QMatrix4x4			mWave[WAVE_ROW][WAVE_COL];
	Square*				mPlane;
};

#endif
