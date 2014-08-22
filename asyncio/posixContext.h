#ifndef _posixContext_h_
#define _posixContext_h_

#include "context.h"

namespace asyncio
{

namespace detail
{
    class PosixContext;
}

/**
 * The PosixContext class is used to register asyncronous read operations with given number of readers.
 * It uses the regular posix API to register read operations.
 */

class PosixContext : public Context
{

public:

    /**
     * @param readerCount Number of asynchronous readers.
     */
    PosixContext( size_t readerCount );
    virtual ~PosixContext( );

    /**
     * Submits a read operation. When the async read operations reaches max readers it returns an
     * empty data.
     * @param file File no.
     * @param pos Read position in the file.
     * @param size Size of the data to read.
     * @return Returns the future data where user can wait on it, or get status.
     */
    virtual FutureDataPtr< AlignedUInt8Vector >::ConstPtr submitIO( int32_t file,
                                                                    int32_t pos,
                                                                    size_t size );

private:

    detail::PosixContext* impl_;
};

}

#endif // _posixContext_h_
