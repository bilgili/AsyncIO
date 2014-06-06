#ifndef _AIOContext_h_
#define _AIOContext_h_

#include <libaio.h>
#include <boost/atomic.hpp>
#include "context.h"

namespace asyncio
{

/**
 * The AIOContext class is used to register asyncronous read operations with given number of readers.
 * It uses the linux AIO system to submit jobs.
 */

class AIOContext : public Context
{

public:

    /**
     * @param readerCount Number of asynchronous readers.
     */
    AIOContext( size_t readerCount );

    ~AIOContext();

    /**
     * Submits a read operation. When the async read operations reaches max readers it returns an
     * empty data.
     * @param file File no.
     * @param pos Read position in the file.
     * @param size Size of the data to read.
     * @throws Throws std::exception with function name, error no when libaio system has problems
     * @return Returns the future data where user can wait on it, or get status.
     */
    virtual FutureDataPtr< AlignedUInt8Vector >::type submitIO( int32_t file,
                                                                int32_t pos,
                                                                size_t size );
private:

    void initialize_( );
    void readLoop_();

    boost::thread readThread_;
    io_context_t context_;
    size_t readerCount_;
    bool exitRequested_;
    boost::atomic< size_t > currentReaderCount_;
};

}



#endif // _AIOContext_h_
