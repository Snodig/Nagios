#ifndef DECLARES_HPP
#define DECLARES_HPP

#include <QString>
#include <QStringList>

#define INCLUDE_ALL_MODULES

//Globally used enums, shared across the applications
//These are used to identify packets and also serve as a simply way
//of structuring system-messages
//v[0.3]

//Packets:

//Ping: "strlen:1:3"

//Con:  "strlen:1:4:Username:Password"
//Sub:  "strlen:1:1:'name'"
//Unsub:"strlen:1:2:'name'"

//Data: "strlen:2:VALIDATOR:Data"

//strlen : sendData-methods append this automatically now

namespace rpnoc
{
    enum CommunicatorType
    {
        SUBSCRIBE = 1,
        UNSUBSCRIBE,
        PING,
        CONNECT,
        DISCONNECT,
        SOURCE_UNAVAILABLE,
        NO_DATA,
        SAFEMODE
    };

    enum PacketType
    {
        COMMUNICATOR = 1,
        DATAID
    };

    struct RawData
    {
        unsigned int type;
        unsigned int subtype;
        QByteArray byteData;
    };
}

#ifdef INCLUDE_ALL_MODULES
	using rpnoc::CommunicatorType;
	using rpnoc::PacketType;
	using rpnoc::RawData;
#endif

#endif // DECLARES_HPP
