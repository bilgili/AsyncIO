#include "aioContext.h"
#include "detail/aioContext.h"

namespace asyncio
{

AIOContext::AIOContext( size_t readerCount )
    : impl_( new detail::AIOContext( readerCount ) )
{
}

AIOContext::~AIOContext()
{
    delete impl_;
}

FutureDataPtr< AlignedUInt8Vector >::ConstPtr AIOContext::submitIO( int32_t file,
                                                                    int32_t pos,
                                                                    size_t size )
{
    return impl_->submitIO( file, pos, size );
}

}
