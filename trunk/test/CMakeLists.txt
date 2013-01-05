# CMakeLists.txt for Test 

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(test)

#============================= check =========================================================
IF (WIN32)
        IF (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
           MESSAGE(FATAL_ERROR "
           CMake generation for ${PROJECT_NAME} is not allowed within the #source directory!
           Remove the CMakeCache.txt file and try again from another folder, e.g.:

           RD /S /Q CMakeFiles
           DEL /Q CMakeCache.txt
           mkdir cmake-make (or mkdir build)
           cd cmake-make (or cd build)
           cmake -G \"Visual Studio 8 2005\" -DCMAKE_BUILD_TYPE=string:Release ../
           (or cmake -G \"Visual Studio 8 2005\" -DCMAKE_BUILD_TYPE=string:Debug ../)
           ")
        ENDIF (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
ELSE (WIN32)
        IF (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
           MESSAGE(FATAL_ERROR "
           CMake generation for ${PROJECT_NAME} not allowed within the #source directory!
           Remove the CMakeCache.txt file and try again from another folder, e.g.:

           rm -rf CMakeFiles CMakeCache.txt
           mkdir cmake-make (or mkdir build)
           cd cmake-make (or cd build)
           cmake -G \"Unix Makefiles\" ../
           ")
        ENDIF (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
ENDIF (WIN32)

SET(CMAKE_VERBOSE_MAKEFILE TRUE)

SET(CMAKE_USE_RELATIVE_PATHS TRUE)

IF (NOT CMAKE_BUILD_TYPE)

   MESSAGE(STATUS "set default as MINSIZEREL")

   SET(CMAKE_BUILD_TYPE MINSIZEREL)

   SET(CMAKE_CONFIGURARION_TYPES "MINSIZEREL")

ENDIF(NOT CMAKE_BUILD_TYPE)

#============================= sources ========================================================

SET(PROJECT_ALL_FILES
./test.cxx
)

INCLUDE_DIRECTORIES(
../src
../util
)
#=============================== build =========================================================
IF (WIN32)

  IF (CMAKE_BUILD_TYPE MATCHES Debug)

    SET(CMAKE_CONFIGURATION_TYPES "Debug")

    ADD_DEFINITIONS(
		    -DWIN32
		    -D_DEBUG
		    -D_USRDLL
		    -D_USE_32BIT_TIME_T
		    -DUSE_TUTIL_DLL
		    -DHAVE_LIBPTHREAD
		    -DUSE_DOWNLOADENGINE_DLL
		    -DUSE_9X9DOWNLOAD_DLL
		    -DNO_USE_JSONC_BOOLEAN
		   )


    LINK_DIRECTORIES(
		    ../../contrib/TUtil/lib
		    ../../contrib/json-c/lib
		    ../../contrib/downloadlib/lib
		    ../../contrib/CrashRpt/lib
		    ../../DownloadClient/lib
                   )

    ADD_EXECUTABLE(${PROJECT_NAME} WIN32 ${PROJECT_ALL_FILES})

    TARGET_LINK_LIBRARIES(${PROJECT_NAME} Ws2_32.lib TUtil-vc80-d.lib CrashRptd.lib 9x9downloadlib-vc80-d.lib DownloadEngine-vc80-d.lib libjson-c-vc80-d.lib)

    SET(LIBRARY_OUTPUT_PATH ../cmake_lib)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
			  COMPILE_FLAGS "/D \"_UNICODE\" /D \"UNICODE\" /Gm /EHsc /RTC1 /MDd /W3 /nologo /c /Wp64 /ZI /TP"
			  OUTPUT_NAME ${PROJECT_NAME})

  ELSE (CMAKE_BUILD_TYPE MATCHES Debug)

     SET(CMAKE_CONFIGURATION_TYPES "Release")

     ADD_DEFINITIONS(
		     -DWIN32
		     -DNDEBUG
		     -D_USE_32BIT_TIME_T
		     -DHAVE_LIBPTHREAD
		     -D_STATIC_CPPLIB
		     -DNO_USE_JSONC_BOOLEAN
                    )

     LINK_DIRECTORIES(../../contrib/TUtil/lib
		      ../../contrib/json-c/lib
		      ../../contrib/sqlite-3.6.20/win32/Sqllite/lib
		      ../../contrib/downloadlib/lib
		      ../../contrib/CrashRpt/lib
		      ../../DownloadClient/lib
                      )
   

     ADD_EXECUTABLE(${PROJECT_NAME} WIN32 ${PROJECT_ALL_FILES})

     TARGET_LINK_LIBRARIES(${PROJECT_NAME}
			Ws2_32.lib wldap32.lib Iphlpapi.lib
			Sensapi.lib Wininet.lib pthreadvc2.lib
			Sqllite.lib CrashRpt.lib libjson-c-vc80-s.lib
			libTUtil-vc80-s.lib libDownloadEngine-vc80-s.lib 9x9downloadlib.lib)

     SET(LIBRARY_OUTPUT_PATH ../cmake_lib)
     SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
			COMPILE_FLAGS "/D \"_UNICODE\" /D \"UNICODE\" /GF /FD /EHsc /MT /O1 /Ob1 /W3 /nologo /c /Wp64 /Zi /TP"
			OUTPUT_NAME ${PROJECT_NAME})

  ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

ELSE (WIN32)

  IF (CMAKE_BUILD_TYPE MATCHES Debug)
     MESSAGE(STATUS "${PROJECT_NAME} Debug")
     SET(CMAKE_CONFIGURATION_TYPES "Debug")

     ADD_DEFINITIONS(
                    )

    LINK_DIRECTORIES(../lib/debug)
    ADD_EXECUTABLE(${PROJECT_NAME} ${PROJECT_ALL_FILES})

     IF(CMAKE_HOST_APPLE)
        SET(CMAKE_FIND_LIBRARY_SUFFIXES .dylib)
     ELSE()
        SET(CMAKE_FIND_LIBRARY_SUFFIXES .so)
     ENDIF()


     FIND_LIBRARY(LOG4CPLUS_LIB NAMES log4cplus PATHS /usr/local/lib )
     MESSAGE(STATUS "log4cplus lib path: ${LOG4CPLUS_LIB}")
     
     FIND_LIBRARY(EVENT_LIB NAMES event PATHS /usr/local/lib )
     MESSAGE(STATUS "libevent lib path: ${EVENT_LIB}")

     FIND_LIBRARY(GOOGLE_PROFILE_LIB NAMES profiler PATHS /usr/local/lib )
     MESSAGE(STATUS "google profile lib path: ${GOOGLE_PROFILE_LIB}")

     #FIND_LIBRARY(LIGHTNINGSERVER_LIB NAMES LightningServer PATHS ../lib/debug )
     #MESSAGE(STATUS "lightningserver lib path: ${LIGHTNINGSERVER_LIB}")
     
     SET(Boost_USE_STATIC_LIBS OFF)
     SET(Boost_USE_MULTITHREADED ON)
     FIND_PACKAGE(Boost COMPONENTS system date_time thread)   
     IF(Boost_FOUND)
       MESSAGE(STATUS "boost lib path: ${Boost_LIBRARIES}")
       
       TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                            ${Boost_LIBRARIES}
                            ${LOG4CPLUS_LIB}
                            ${EVENT_LIB}
                            LightningServer#${LIGHTNINGSERVER_LIB}
                            ${GOOGLE_PROFILE_LIB}
                            stdc++
                            )
     ENDIF()

     SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
     SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O0 -g -Wall" OUTPUT_NAME ${PROJECT_NAME})

  ELSE (CMAKE_BUILD_TYPE MATCHES Debug)
    MESSAGE(STATUS "${PROJECT_NAME} ${CMAKE_BUILD_TYPE}")

     SET(CMAKE_CONFIGURATION_TYPES "Release")

     ADD_DEFINITIONS(
                    )

    LINK_DIRECTORIES(../lib/release)
    ADD_DEFINITIONS(
                    )

    ADD_EXECUTABLE(${PROJECT_NAME} ${PROJECT_ALL_FILES} )

    SET(CMAKE_FIND_LIBRARY_SUFFIXES .a)

     FIND_LIBRARY(LOG4CPLUS_LIB NAMES log4cplus PATHS /usr/local/lib )
     MESSAGE(STATUS "log4cplus lib path: ${LOG4CPLUS_LIB}")
     
     FIND_LIBRARY(EVENT_LIB NAMES event PATHS /usr/local/lib )
     MESSAGE(STATUS "libevent lib path: ${EVENT_LIB}")

     FIND_LIBRARY(GOOGLE_PROFILE_LIB NAMES profiler PATHS /usr/local/lib )
     MESSAGE(STATUS "google profile lib path: ${GOOGLE_PROFILE_LIB}")

     #FIND_LIBRARY(LIGHTNINGSERVER_LIB NAMES LightningServer PATHS ../lib/debug )
     #MESSAGE(STATUS "lightningserver lib path: ${LIGHTNINGSERVER_LIB}")
     
     SET(Boost_USE_STATIC_LIBS ON)
     SET(Boost_USE_MULTITHREADED ON)
     FIND_PACKAGE(Boost COMPONENTS system date_time thread)   
     IF(Boost_FOUND)
       MESSAGE(STATUS "boost lib path: ${Boost_LIBRARIES}")
       
       TARGET_LINK_LIBRARIES(${PROJECT_NAME}
           #${Boost_LIBRARIES}
           #                ${LOG4CPLUS_LIB}
           #                ${EVENT_LIB}
                            LightningServer#${LIGHTNINGSERVER_LIB}
                            ${GOOGLE_PROFILE_LIB}
                            stdc++
                            )
      ENDIF()

    SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O0 -static" OUTPUT_NAME ${PROJECT_NAME})

  ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

ENDIF (WIN32)
