#ifndef _futureData_h_
#define _futureData_h_

#include "../types.h"

namespace asyncio
{

enum WaitStatus
{
    WS_DEFERRED = static_cast< uint32_t >( boost::future_status::deferred ),
    WS_READY = static_cast< uint32_t >( boost::future_status::ready ),
    WS_TIMEOUT = static_cast< uint32_t >( boost::future_status::timeout )
};

/**
 * FutureData class is based on futures. It wraps the promise/future and take the responsibility of
 * managing the given data owned by user. ( Note : If future is boolean it will be more useful but
 * in this case does it really worth using ? It is almost a concurrency object at this point )
 */

template< class T >
class FutureData
{
    friend class FutureJob< T >;

public:

    /**
     * @param Data to be managed.
     */
    FutureData()
        : future_( promise_.get_future() )
    {}

    /**
     * @return Non-blocking and returns true if data is ready to be used.
     */
    bool isReady() const
    {
        return future_.has_value( );
    }

    /**
     * Wait function waits for the data for a given number of miliseconds.
     * @param miliseconds Wait time for the data. If 0 is given ( or no param ) it waits untils
     * data is ready.
     * @return Returns the data status.
     */
    WaitStatus wait( uint32_t miliseconds = 0 ) const
    {
        boost::posix_time::milliseconds timeout( miliseconds );
        if( miliseconds == 0 )
            future_.wait(  );
        else
            future_.timed_wait( timeout );
        return static_cast< WaitStatus >( future_.get_state() );
    }

    /**
     * Tries to get the data into the given owner data. It waits for a given number of miliseconds.
     * @param milisecondsmiliseconds Wait time for the data. If 0 is given ( or no param ) it waits untils
     * data is ready.
     * @return Returns true if data is set.
     */
    bool tryGetData( T& data, uint32_t miliseconds = 0 )
    {
        wait( miliseconds );

        if( !isReady() )
            return false;

        data = future_.get();
        return true;
    }

    /**
     * Waits until the data is set.
     */
    const T& waitData( )
    {
        return future_.get();
    }

    void setData( const T& data )
    {
        promise_.set_value( data );
    }

private:

    boost::promise< T > promise_;
    boost::unique_future< T > future_;
};

}

#endif // _futureData_h_
