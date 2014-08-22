#ifndef _AIOContextDetail_h_
#define _AIOContextDetail_h_

#include <libaio.h>
#include <boost/atomic.hpp>

#include "../future/futureData.h"

namespace asyncio
{

namespace detail
{

class AIOContext
{
public:

    /**
     * @param readerCount Number of asynchronous readers.
     */
    AIOContext( size_t readerCount );

    ~AIOContext();

    FutureDataPtr< AlignedUInt8Vector >::ConstPtr submitIO( int32_t file,
                                                            int32_t pos,
                                                            size_t size );

    void initialize_( );
    void readLoop_();

    boost::thread readThread_;
    io_context_t context_;
    size_t readerCount_;
    bool exitRequested_;
    boost::atomic< size_t > currentReaderCount_;
};

}

}
#endif // _AIOContextDetail_h_
