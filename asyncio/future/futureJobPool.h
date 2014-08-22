#ifndef _futureJobPool_h_
#define _futureJobPool_h_

#include "../types.h"

#include "futureJob.h"
#include <deque>

namespace asyncio
{

template< class T >
class FutureJobPool
{
public:

    /**
     * FutureJobPool creates a pool of threads that can run FutureJobs.
     * @param readerCount Number of resident threads.
     */
    FutureJobPool( size_t readerCount )
        : continueReading_( true )
    {
        for( uint32_t i = 0; i < readerCount; ++i )
        {
            boost::thread *thread = new boost::thread();
            threadPool_.add_thread( thread );
            *thread = boost::thread( &FutureJobPool<T>::readLoop_, this );
        }
    }

    /**
     * Submits a job with given function and generates a future job.
     * @param function Function that sets the future data.
     * @param jobFinishFunction When function is done, jobFinishFunction is executed.
     * @return Returns the future job.
     */
    typename FutureJobPtr< T >::Ptr submitJob(
            const typename FutureJob< T >::JobFunction& function,
            const typename FutureJob< T >::JobFinishFunction& jobFinishFunction = 0 )
    {
        typename FutureJobPtr< T >::Ptr futureJob(
                    new FutureJob< T >( function, jobFinishFunction ) );
        {
            boost::mutex::scoped_lock lock( mutex_ );
            futureJobQueue_.push_back( futureJob );
            queueEmpty_.notify_all();
        }
        return futureJob;
    }

    virtual ~FutureJobPool()
    {
        continueReading_ = false;
        queueEmpty_.notify_all();
        threadPool_.join_all();
    }

protected:

    void readLoop_( )
    {
        while( continueReading_ )
        {
             typename FutureJobPtr< T >::Ptr futureJob;
             {
                boost::mutex::scoped_lock lock( mutex_ );
                if( futureJobQueue_.empty() )
                    queueEmpty_.wait( lock );

                futureJob = futureJobQueue_.back();
                futureJobQueue_.pop_back();
             }

             (*futureJob)();
        }
    }

    boost::thread_group threadPool_;
    boost::condition_variable queueEmpty_;
    boost::mutex mutex_;
    bool continueReading_;
    std::deque< typename FutureJobPtr< T >::Ptr > futureJobQueue_;
};

}

#endif // _futureJobPool_h_
