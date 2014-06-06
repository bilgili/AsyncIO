# Copyright (c) BBP/EPFL 2011-2014
#                        Stefan.Eilemann@epfl.ch
# All rights reserved. Do not distribute without further notice.

source_group(\\ FILES CMakeLists.txt)

set(ASYNCIO_PUBLIC_HEADERS ${COMMON_INCLUDES} types.h context.h futureData.h posixContext.h aioContext.h )
set(ASYNCIO_HEADERS)
set(ASYNCIO_SOURCES ${COMMON_SOURCES} posixContext.cpp aioContext.cpp )
set(ASYNCIO_LINK_LIBRARIES ${Boost_LIBRARIES} ${AIO_LIBRARIES})

common_library(AsyncIO)
