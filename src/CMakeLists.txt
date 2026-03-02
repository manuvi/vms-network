set(VMS_NETWORK_LIBRARY_TYPE STATIC)
if(VMS_NETWORK_BUILD_SHARED)
    set(VMS_NETWORK_LIBRARY_TYPE SHARED)
endif()

set(VMS_NETWORK_TARGET_NAME vms-network)

# Keep this list explicit: when you add/remove a source file, update it here.
set(VMS_NETWORK_SOURCES
    vms-network/NetAddress.cpp
    vms-network/NetPacket.cpp
    vms-network/NetSocket.cpp
    vms-network/TCPSocket.cpp
    vms-network/UDPSocket.cpp
)

add_library(${VMS_NETWORK_TARGET_NAME} ${VMS_NETWORK_LIBRARY_TYPE} ${VMS_NETWORK_SOURCES})

target_compile_features(${VMS_NETWORK_TARGET_NAME} PUBLIC cxx_std_17)

target_include_directories(
    ${VMS_NETWORK_TARGET_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(${VMS_NETWORK_TARGET_NAME} PROPERTIES OUTPUT_NAME "vms-network")
