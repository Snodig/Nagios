/*
 *	Simple 'cells' - e.g cells that contain text or images
 */

#ifndef CELL_HPP
#define CELL_HPP

#include <QVector2D>
#include "src/utils/declares.hpp"
#include "src/opengl/shapes/panel.hpp"
#include "src/opengl/shapes/format.hpp"

class QGLShaderProgram;
class QMatrix4x4;


namespace rpnoc
{
	class Cell
	{
	public:
							Cell( QVector2D iSize, QVector2D iPadding,
								QString iImageBackground = "backgrounds/white.png", QString iImageBorder = "borders/round_corners.png");
		virtual				~Cell( void );

		void				setPanelBackground( QString iBackground );
		void				setPanelBorder( QString iBorder );
		void				setBackGroundColor( QVector4D iColor );
		QVector2D			getSize( void ) const;
		QVector4D			getBackgroundColor( void ) const;

		virtual void		resize( QVector2D iSize )=0;
		virtual void		draw( const QMatrix4x4& iProjection, const QMatrix4x4& iView, QMatrix4x4 iModel )=0;

	protected:
		QVector2D			mSize;
		QVector2D			mPadding;
		QVector4D			mBackgroundColor;
		Panel*				mPanel;
	};

	class TextCell : public Cell
	{
	public:
							TextCell( QVector2D iSize, QVector2D iPadding, TextFormat iTextFormat,
									QString iText );
							~TextCell( void );

		void				resize( QVector2D iSize );
		void				draw( const QMatrix4x4& iProjection, const QMatrix4x4& iView, QMatrix4x4 iModel );
		void				setText( QString iText );
		QString				getText( void ) const;

	private:
		TextFormat			mTextFormat;
		QString				mText;
	};

	class ImageCell : public Cell {
	public:
							ImageCell( QVector2D iSize, QString iName );
							~ImageCell( void );
		void				resize( QVector2D iSize );
		void				draw( const QMatrix4x4& iProjection, const QMatrix4x4& iView, QMatrix4x4 iModel );
	private:
		QString				mName;
		Square*				mSquare;
	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::Cell;
	using rpnoc::TextCell;
	using rpnoc::ImageCell;
#endif

#endif // CELL_HPP
