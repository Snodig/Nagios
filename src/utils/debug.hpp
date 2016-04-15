/*
 * debug.hpp
 */

#ifndef DEBUG_HPP
#define DEBUG_HPP

/*
 * Debugging: error testing macros
 * -	Used to print out messages when something goes
 *		wrong, or when something could be of interest to
 *		the programmer. Can be turned off by not defining
 *		ENABLE_DEBUG.
 * 
 * -	Undefine ENABLE_LOG_2_FILE to not log to file.
 *
 * -	Undefine ENABLE_LOG_2_TERMINAL to not log to Terminal.
 */
#include <Qt/qdebug.h>
#include <QTextStream>
#include "src/windows/mainwindow.hpp"
#include "src/utils/debugTimer.hpp"

#define LEVEL_INFO "INFO"
#define LEVEL_WARNING "WARNING"
#define LEVEL_ERROR "ERROR"
#define ENABLE_DEBUG
#define ENABLE_LOG_2_FILE
#define ENABLE_LOG_2_TERMINAL

/*
#ifdef ENABLE_LOG_2_FILE
	//#define ENABLE_LOG_2_FILE oLog << oText; \
#else
	//#define ENABLE_LOG_2_FILE \
#endif // ENABLE_LOG_2_FILE

#ifdef ENABLE_LOG_2_TERMINAL
	//#define ENABLE_LOG_2_TERMINAL qDebug() << oText; \
#else
	//#define ENABLE_LOG_2_TERMINAL \
#endif // ENABLE_LOG_2_TERMINAL
*/

#ifdef ENABLE_DEBUG
	#define ERROR_TEST( command, text, level ) \
	{ \
		if( command == false ) { \
			QTextStream oLog( gLogFile ); \
			QString oText( level ); \
			if( oText == LEVEL_INFO  ) { \
				oText.append( "    -- " ); \
			} else if( oText == LEVEL_WARNING ) { \
				oText.append( " -- " ); \
			} else if( oText == LEVEL_ERROR ) { \
				oText.append( "   -- " ); \
			} \
			oText.prepend( "-- " ); \
			oText.append( "time '" ); oText.append( gDebugTimer.getTimeCurrent() ); oText.append( "' -- " ); \
			oText.append( "runtime '" ); oText.append( gDebugTimer.getTimeTotal() ); oText.append( "' -- " ); \
			oText.append( "file '" ); oText.append( __FILE__ ); oText.append( "' -- " ); \
			oText.append( "function '" ); oText.append( __FUNCTION__ ); oText.append( "(...)' -- " ); \
			oText.append( "line '" ); oText.append( QString::number( __LINE__ ) ); oText.append( "'\n    *   " ); \
			oText.append( text ); \
			oText.append( " *\n\n" ); \
			qDebug() << oText; \
			oLog << oText; \
		} \
	}
#else
	#define ERROR_TEST( command, text, level )
#endif // ENABLE_DEBUG

#define debug_i( command, text ) \
{ \
	ERROR_TEST( command, text, LEVEL_INFO ) \
}
	
#define log_i( text ) \
{ \
	ERROR_TEST( false, text, LEVEL_INFO ) \
}

#define debug_w( command, text ) \
{ \
	ERROR_TEST( command, text, LEVEL_WARNING ) \
}
	
#define log_w( text ) \
{ \
	ERROR_TEST( false, text, LEVEL_WARNING ) \
}

#define debug_e( command, text ) \
{ \
	ERROR_TEST( command, text, LEVEL_ERROR ) \
}

#define log_e( text ) \
{ \
	ERROR_TEST( false, text, LEVEL_ERROR ) \
}

#define log_endl QString( "\n        " )

#endif // DEBUG_HPP
