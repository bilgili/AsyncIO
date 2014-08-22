
/* Copyright (c) 2013-2014, EPFL/Blue Brain Project
 *                          Ahmet Bilgili <ahmet.bilgili@epfl.ch>
 */
#include <stdio.h>

#include "../asyncio/types.h"
#include "../asyncio/future/futureData.h"
#include "../asyncio/posixContext.h"

#define BOOST_TEST_MODULE posixContext
#include <boost/test/unit_test.hpp>

const size_t dataSize = 1024 * 1024;
const uint32_t nbReaders = 10;

BOOST_AUTO_TEST_CASE(posixReadTest)
{
    FILE *fp = fopen( "/home/ahmetbilgili/Data/Volume/JeanPierre/merged/Btest/Btest.raw", "r" );
    const int fno = fileno( fp );

    asyncio::PosixContext context( nbReaders );
    for( size_t i = 0; i < nbReaders; ++i )
    {
        asyncio::FutureDataPtr< asyncio::AlignedUInt8Vector >::ConstPtr futureData =
                context.submitIO( fno, 0, dataSize );
        futureData->wait();
        BOOST_CHECK( futureData->isReady() );
    }
}
