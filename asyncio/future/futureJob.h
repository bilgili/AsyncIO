#ifndef _futureJob_h_
#define _futureJob_h_

#include "../types.h"
#include "futureData.h"

namespace asyncio
{

enum JobStatus
{
    JS_NONE,
    JS_SUBMITTED,
    JS_FAILED,
    JS_READY
};

const std::uint64_t INVALID_JOBID = -1;

template< class T >
class FutureJob
{

public:

    typedef boost::function< bool( const FutureJob< T >&, typename FutureDataPtr< T >::Ptr ) > JobFunction;
    typedef boost::function< void( const FutureJob< T >&, typename FutureDataPtr< T >::ConstPtr ) > JobFinishFunction;

    /**
     * FutureJob is used to submit a function which sets the future data.
     * and notifies the submitter when job is done.
     * @param function Function that sets the future data.
     * @param jobFinishFunction When function is done, jobFinishFunction is executed.
     */
    FutureJob( const JobFunction& function,
               const JobFinishFunction& jobFinishFunction = 0 )
        : futureDataPtr_( new FutureData< T >( ) ),
          jobFunction_( function ),
          jobFinishFunction_( jobFinishFunction ),
          status_( JS_NONE ),
          jobId_( INVALID_JOBID )
    {
    }

    ~FutureJob()
    {}


    /**
     * @return Returns the status of the operation success.
     */
    JobStatus operator()()
    {
        setJobID_();
        status_ = JS_SUBMITTED;

        bool ret = jobFunction_( *this, futureDataPtr_ );
        if( ret )
        {
            status_ = JS_READY;
        }
        else
            status_ = JS_FAILED;

        if( jobFinishFunction_ )
            jobFinishFunction_( *this, futureDataPtr_ );

        return status_;
    }

    /**
     * @return Returns the status of the job.
     */
    JobStatus getStatus() const { return status_; }

    /**
      * @return Returns the jobID set by the JobPool
      */
    std::uint64_t getJobID() const { return jobId_; }

    /**
     * @return Returns the future data.
     */
    const typename FutureDataPtr< T >::Ptr getFutureData( ) const { return futureDataPtr_; }

private:

    void setJobID_() { jobId_ = reinterpret_cast< std::uint64_t >( this ); }

    typename FutureDataPtr< T >::Ptr futureDataPtr_;
    JobFunction jobFunction_;
    JobFinishFunction jobFinishFunction_;
    JobStatus status_;
    std::uint64_t jobId_;

};

}

#endif // _futureJob_h_
