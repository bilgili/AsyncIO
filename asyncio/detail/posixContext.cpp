#include <unistd.h>

#include "posixContext.h"

namespace asyncio
{

namespace detail
{

struct ReadJob
{
    ReadJob( int32_t file,
             int32_t pos,
             size_t size )
            : file_( file ),
              pos_( pos ),
              size_( size )
    {}

    bool readData( const FutureJob< AlignedUInt8Vector >&, FutureDataPtr< AlignedUInt8Vector >::Ptr futureData )
    {
        AlignedUInt8Vector data( size_ );
        const size_t readDataSize = pread64( file_, &data[ 0 ], size_, pos_ );
        if( readDataSize == data.size() )
        {
            futureData->setData( data );
            return true;
        }
        return false;
    }

    int32_t file_;
    int32_t pos_;
    size_t size_;
};

typedef boost::shared_ptr< ReadJob > ReadJobPtr ;
std::map< std::uint64_t, ReadJobPtr > jobMap;
boost::mutex mutex;

void addReadJob( const FutureJob<AlignedUInt8Vector>& futureJob,
                 ReadJobPtr readJob )
{
    boost::mutex::scoped_lock( mutex_ );
    jobMap[ futureJob.getJobID() ] = readJob;
}

void removeReadJob( const FutureJob< AlignedUInt8Vector>& futureJob,
                    FutureDataPtr< AlignedUInt8Vector >::ConstPtr )
{
    boost::mutex::scoped_lock( mutex_ );
    jobMap.erase( futureJob.getJobID() );
}

PosixContext::PosixContext( size_t readerCount )
    : futureJobPool_( readerCount )
{
}

PosixContext::~PosixContext()
{
}

FutureDataPtr< AlignedUInt8Vector >::ConstPtr PosixContext::submitIO( int32_t file,
                                                                      int32_t pos,
                                                                      size_t size )
{
    ReadJobPtr readJob( new ReadJob( file, pos, size ) );
    FutureJob< AlignedUInt8Vector >::JobFunction readFunction =
            boost::bind( &ReadJob::readData, readJob, _1, _2 );

    FutureJob< AlignedUInt8Vector >::JobFinishFunction readFinishFunction =
            boost::bind( &removeReadJob, _1, _2 );

    FutureJobPtr< AlignedUInt8Vector >::Ptr job =
            futureJobPool_.submitJob( readFunction, readFinishFunction );

    addReadJob( *job, readJob );
    return job->getFutureData();
}

}

}

