#ifndef _Context_h_
#define _Context_h_

#include "types.h"

namespace asyncio
{

/**
 * The Context class is the base class for submitting read operations. The submitter can wait
 * on the result using futures.
 */
class Context
{

public:

    virtual ~Context() {}

    /**
     * Submits a read operation. When the read operation fails it returns an empty future data ptr.
     * @param file File no.
     * @param pos Read position in the file.
     * @param size Size of the data to read.
     * @return
     */
    virtual FutureDataPtr< AlignedUInt8Vector >::type submitIO( int32_t file,
                                                                int32_t pos,
                                                                size_t size ) = 0;

};

}


#endif // _Context_h_
