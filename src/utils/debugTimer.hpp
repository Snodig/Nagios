/*
 * debugTimer.hpp
 */

#ifndef DEGUBTIMER_HPP
#define DEGUBTIMER_HPP

#ifdef WIN32
	#pragma warning( disable : 4996 )
#endif

/*
 * Debugging: DebugTimer
 * -	Timer used to generate different kinds
 *		of timestamps.
 */
#include <boost/date_time/posix_time/posix_time.hpp>
#include <Qt/qstring.h>

class DebugTimer {
public:
	typedef boost::posix_time::ptime	ptime;
	
	static DebugTimer&					getInstance( void );
	// returns time since program was started
	QString								getTimeTotal( void );
	// returns the current local time
	QString								getTimeCurrent( void ) const;
private:
										DebugTimer( void );
										~DebugTimer( void );
										DebugTimer( const DebugTimer& iTimer );
	DebugTimer&							operator=( const DebugTimer& iTimer );
private:
	void								update( void );
	
	ptime								mStartTick;
	ptime								mLastTick;
	ptime								mCurrentTick;
	unsigned long long					mTimeDelta;
};

// handler to DebugTimer class
extern DebugTimer& gDebugTimer;

#endif // DEGUBTIMER_HPP
