/**
  *	This source shows simple statistics for nagios:
  *	The Nagios-source IP, update-latency and counts of status-types
  */

#ifndef NAGIOSSTATISTICS_HPP
#define NAGIOSSTATISTICS_HPP

#include "src/opengl/drawables/drawablesource.hpp"
#include <QObject>

#include "src/utils/declares.hpp"
#include "src/opengl/shapes/cell.hpp"
#include "src/opengl/shapes/row.hpp"
#include "src/opengl/shapes/table.hpp"


namespace rpnoc
{
	class NagiosStatistics : public DrawableSource
	{
		Q_OBJECT

	public:
										NagiosStatistics( float windowSize, QWidget *parent = 0 );
										~NagiosStatistics( void );

		virtual void					init( void );
		virtual void					draw( void );

	private:
		rpnoc::Row*						mRowUpdateTime;
		rpnoc::Row*						mRowCounts;
		rpnoc::TextCell*				mTextCellUpdateTime;
		rpnoc::TextCell*				mTextCellCounts[3];
		rpnoc::TextFormat*				mFormatUpdateTime;
		rpnoc::TextFormat*				mFormatCounts;
	};
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::NagiosStatistics;
#endif

#endif // NAGIOSSTATISTICS_HPP
