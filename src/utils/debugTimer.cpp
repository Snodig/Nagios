/*
 * debugTimer.cpp
 */

#include <boost/date_time/gregorian/gregorian.hpp>
using boost::gregorian::to_simple_string;
#include <boost/date_time/posix_time/posix_time.hpp>
using boost::posix_time::ptime;
using boost::posix_time::microsec_clock;
using boost::posix_time::microseconds;
#include "src/utils/debugTimer.hpp"
#include "src/utils/debug.hpp"

// DebugTimer instance
DebugTimer& gDebugTimer = DebugTimer::getInstance(); 
 
DebugTimer::DebugTimer( void ) 
	:	mStartTick( microsec_clock::local_time() ),
		mLastTick( microsec_clock::local_time() ), 
		mCurrentTick( microsec_clock::local_time() ),
		mTimeDelta( 0 ) 
{
	//
}

DebugTimer::~DebugTimer( void ) 
{
	//
}

DebugTimer& DebugTimer::getInstance( void ) 
{
	static DebugTimer oInstance;
	return oInstance;
}

void DebugTimer::update( void ) 
{
	mLastTick = mCurrentTick;
	mCurrentTick = microsec_clock::local_time();
	
	/*
		ptime - ptime = time_duration.
		We skip the process of making a
		temp time_duration variable here,
		and rather just use parenthesis.
	*/
	mTimeDelta = ( mCurrentTick - mLastTick ).total_microseconds();
}

QString DebugTimer::getTimeTotal( void ) 
{
	/*
		ptime - ptime = time_duration.
	*/
	update();
	const char* oOutput = to_simple_string( -(mStartTick - mCurrentTick) ).c_str();
	return QString( oOutput );
}

QString DebugTimer::getTimeCurrent( void ) const 
{
	/*
		ptime - ptime = time_duration.
	*/
	const char* oOutput = to_simple_string( 
		mCurrentTick.time_of_day() 
	).c_str();
	
	return QString( oOutput );
}
