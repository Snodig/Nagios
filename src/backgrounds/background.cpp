
#include <qmath.h>
#include "background.hpp"
#include "src/utils/debug.hpp"
#include "src/utils/texture.hpp"
#include "src/utils/shader.hpp"
#include "src/windows/mainwindow.hpp"

Background::Background() {
	for(int rows = 0; rows < WAVE_ROW; ++rows){
		for(int col = 0; col < WAVE_COL; ++col){
			mWave[rows][col].translate(INTERVAL * col, INTERVAL * rows, 0);
			mWave[rows][col].translate(-110.0, -40.0, 0.0);
			mWave[rows][col].translate(0.0, 0.0, (qCos(col / 2) * 2));
			mWave[rows][col].rotate( QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 0.0), qCos(col / 2) * 50));
		}
	}
	
	for(int oOffset = 0; oOffset < 40; ++oOffset ) {
		for(int rows = 0; rows < WAVE_ROW; ++rows){
			for(int col = 0; col < WAVE_COL; ++col){
				mWave[rows][col].rotate(QQuaternion::fromAxisAndAngle(QVector3D(1.0, 1.0, 1.0), qTan( 0.2 ) * 5));
				if( 0 == 0 ) {
					mWave[rows][col].rotate( QQuaternion::fromAxisAndAngle( QVector3D( 0.2, -0.5, 0.0 ), qTan( 0.6 ) * 10 ) );
					static float oTest = 0.0f;
					static bool oDir = true;
					if( oTest > 2.0f ) {
						oDir = false;
					} else if( oTest < -2.0f ) {
						oDir = true;
					}
					if( oDir ) {
						oTest += 0.00002f;
						} else {
						oTest -= 0.00002f;
					}
					
					mWave[rows][col].translate( 0.0, 0.0, -qCos( oTest * (rows*col) / 300 ) * -qSin( oTest * (rows*col) / 300 ) );
					mWave[rows][col].translate( -qSin( oTest * (rows*col) / 300 ), -oTest * (rows*col) / 300, 0.0 );
				}
			}
		}
	}
	
	mView.lookAt( QVector3D(40.0, -20.0f, 30.0f), QVector3D(40.0f, 40.0f, -40.0f), QVector3D(0.0f, 1.0f, 0.0f) );
	mProjection.perspective( 90.0f, MainWindow::getWindowSize().x() / MainWindow::getWindowSize().y(), 0.0f, 100.0f );
	//mProjection.frustum( 0, 1024, 0, 900, -100, 1000 );
}

Background::~Background() {

}

void Background::init() {
	mPlane = new Square( QVector2D(5.0, 5.0) , ShaderManager::getInstance().getShader( "fancy" ) );
}

void Background::render() {
	TextureManager::getInstance().bindTexture("Dot.png");
	
	for( int rows = 0; rows < WAVE_ROW; ++rows ) {
		for( int col = 0; col < WAVE_COL; ++col ) {
			QMatrix4x4 oMatrix = mWave[rows][col];
			oMatrix.scale( 1.0 - (double)rows / WAVE_ROW );
			mWave[rows][col].rotate( QQuaternion::fromAxisAndAngle( QVector3D( 0.1, 0.0, 0.3 ), 1 ) );
			//mWave[rows][col].translate( sin( rows*col / M_PI * M_El ) * acos( rows*col / M_PI * M_El ), sin( rows*col / M_PI * M_El ), cos( rows*col / M_PI * M_El ) );
			mWave[rows][col].rotate( QQuaternion::fromAxisAndAngle( QVector3D( 0.1, 0.0, 0.3 ), qTan( 0.6 ) * 1 ) );
			
			if( 0 == 0 ) {
					mWave[rows][col].rotate( QQuaternion::fromAxisAndAngle( QVector3D( 0.2, -0.5, 0.0 ), qTan( 0.6 ) * 1 ) );
					static float oTest = 0.0f;
					static bool oDir = true;
					if( oTest > 2.0f ) {
						oDir = false;
					} else if( oTest < -2.0f ) {
						oDir = true;
					}
					if( oDir ) {
						oTest += 0.0000002f;
						} else {
						oTest -= 0.0000002f;
					}
					
					mWave[rows][col].translate( 0.0, 0.0, -qCos( oTest * (rows*M_El) / 300 ) * -qSin( oTest * (rows*col) / 300 ) );
					mWave[rows][col].translate( -qSin( oTest * (rows*col) / 300 ), -oTest * (rows*col) / 300, 0.0 );
			}
			mPlane->draw(mProjection, mView, oMatrix );
		}
	}
}
