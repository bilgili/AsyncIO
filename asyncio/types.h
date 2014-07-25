#ifndef _types_h_
#define _types_h_

#include <boost/shared_ptr.hpp>

#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>

#include <fstream>
#include "alignedAllocator.h"

namespace asyncio
{

class Context;

template< class T >
class FutureJob;

template< class T >
class FutureData;

class PosixContext;
class AIOContext;

template< class T >
struct FutureJobPtr
{
    typedef boost::shared_ptr< FutureJob< T > > Ptr;
    typedef boost::shared_ptr< const FutureJob< T > > ConstPtr;
};

template< class T >
struct FutureDataPtr
{
    typedef boost::shared_ptr< FutureData< T > > Ptr;
    typedef boost::shared_ptr< const FutureData< T > > ConstPtr;
};

typedef std::vector< uint8_t, util::aligned_allocator< uint8_t, 512 > > AlignedUInt8Vector;

}

#endif
