#include "posixContext.h"
#include "detail/posixContext.h"

namespace asyncio
{


PosixContext::PosixContext( size_t readerCount )
    : impl_( new detail::PosixContext( readerCount ) )
{
}

PosixContext::~PosixContext()
{
    delete impl_;
}

FutureDataPtr< AlignedUInt8Vector >::ConstPtr PosixContext::submitIO( int32_t file,
                                                                      int32_t pos,
                                                                      size_t size )
{
    return impl_->submitIO( file, pos, size );
}

}
