#ifndef _PosixContextDetail_h_
#define _PosixContextDetail_h_

#include "../future/futureData.h"
#include "../future/futureJobPool.h"

namespace asyncio
{

namespace detail
{

class PosixContext
{

public:

    PosixContext( size_t readerCount );
    ~PosixContext( );

    FutureDataPtr< AlignedUInt8Vector >::ConstPtr submitIO( int32_t file,
                                                                    int32_t pos,
                                                                    size_t size );

private:

    FutureJobPool< AlignedUInt8Vector > futureJobPool_;
};

}

}


#endif // _PosixContextDetail_h_
