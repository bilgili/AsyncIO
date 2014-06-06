

set(ASYNCIO_REPO_URL https://github.com/bilgili/AsyncIO.git)
set(ASYNCIO_DEPENDS REQUIRED Boost AIO)
set(ASYNCIO_BOOST_COMPONENTS "unit_test_framework system thread filesystem program_options thread serialization")



if(CI_BUILD_COMMIT)
  set(ASYNCIO_REPO_TAG ${CI_BUILD_COMMIT})
else()
  set(ASYNCIO_REPO_TAG master)
endif()
set(ASYNCIO_FORCE_BUILD ON)
set(ASYNCIO_SOURCE ${CMAKE_SOURCE_DIR})
