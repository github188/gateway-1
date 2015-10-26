PROJECT(GATEWAY)

AUX_SOURCE_DIRECTORY(./ SRC_LIST)
SET(EXECUTABLE_OUTPUT_PATH ../bin)

ADD_DEFINITIONS(-g -O0 -W -Wall -DLINUX -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DAC_HAS_INFO -DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL -DTIXML_USE_STL -DHAVE_CXX_STDHEADERS -Wno-deprecated ${CMAKE_CXX_FLAGS})


INCLUDE_DIRECTORIES(
./
/usr/local/include/hiredis
/usr/local/include/json-c
../include
../include/rmq
../include/svrutil2
)

LINK_DIRECTORIES(
./
/usr/local/lib
../lib
)

ADD_EXECUTABLE(gateway ${SRC_LIST})

TARGET_LINK_LIBRARIES(gateway rmqlib boost_thread kcrypto svrutil json-c
hiredis  TradeInterfaceSDK pthread ssl crypto)
