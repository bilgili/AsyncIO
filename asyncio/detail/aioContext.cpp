#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <boost/atomic.hpp>

#include "aioContext.h"

namespace asyncio
{

namespace detail
{

class libaio_exception : public std::exception
{
public:
    libaio_exception( const std::string& errorFunctionName, int32_t errorNb )
    {
        _error << "Error in libaio function : " << errorFunctionName
               << " Error number: " << errorNb << std::endl;
    }

    libaio_exception( const libaio_exception& excep )
    {
        _error << excep._error;
    }

    virtual ~libaio_exception() throw() {}

    virtual const char* what() const throw() { return _error.str().c_str(); }

private:
    std::stringstream _error;
};

struct IOCBSubmitter : public iocb
{
    IOCBSubmitter()
    {}

    void submit( FutureDataPtr< AlignedUInt8Vector >::Ptr fd,
                 io_context_t context,
                 int32_t file,
                 int32_t pos,
                 size_t size )
    {
        futureData = fd;
        iocb* iocbs[1] = { 0 };
        iocbs[ 0 ] = this;
        data.resize( size );
        io_prep_pread( this, file, &data[ 0 ], size, pos );
        int32_t result = io_submit( context, 1, iocbs ) ; // will throw
        if( result < 0 )
            throw libaio_exception( "io_submit", result );
    }

    FutureDataPtr< AlignedUInt8Vector >::Ptr futureData;
    AlignedUInt8Vector data;
};

AIOContext::AIOContext( size_t readerCount )
    : context_( 0 ),
      readerCount_( readerCount ),
      exitRequested_( false ),
      currentReaderCount_( 0 )
{
    initialize_();
}

AIOContext::~AIOContext()
{
    exitRequested_ = true;
    readThread_.join();
    io_destroy( context_ );
}

FutureDataPtr< AlignedUInt8Vector >::ConstPtr AIOContext::submitIO( int32_t file,
                                                                    int32_t pos,
                                                                    size_t size )
{
    if( currentReaderCount_ == readerCount_ )
        return FutureDataPtr< AlignedUInt8Vector >::Ptr();

    ++currentReaderCount_;

    FutureDataPtr< AlignedUInt8Vector >::Ptr futureData( new FutureData< AlignedUInt8Vector > );

    // Will be deleted by the reading thread
    IOCBSubmitter *submitter = new IOCBSubmitter();
    submitter->submit( futureData, context_, file, size, pos );

    return futureData;
}

void AIOContext::readLoop_( )
{
    struct io_event* events = new io_event[ readerCount_ ];

    timespec time;
    time.tv_sec = 10;

    while( !exitRequested_ )
    {
        const int32_t numEvents = io_getevents( context_, 0, readerCount_, events, &time );
        if( numEvents < 0 )
        {
            delete events;
            throw libaio_exception( "io_getevents", numEvents );
        }

        for ( int32_t i = 0; i < numEvents; i++ )
        {
            const io_event& event = events[ i ];
            IOCBSubmitter* iocb = static_cast< IOCBSubmitter* >( event.obj );
            FutureDataPtr< AlignedUInt8Vector >::Ptr futureData = iocb->futureData;
            futureData->setData( iocb->data );
            delete iocb;
            --currentReaderCount_;
        }
    }

    delete events;
}



void AIOContext::initialize_( )
{
    if( io_setup( readerCount_, &context_ ) < 0 )
        return;

    readThread_ = boost::thread( &AIOContext::readLoop_, this );
    readThread_.start_thread();
}

}

}
