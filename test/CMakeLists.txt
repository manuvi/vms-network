# Keep this list explicit: when you add/remove a test file, update it here.
set(VMS_NETWORK_TEST_SOURCES
    net_packet_test.cpp
    net_address_test.cpp
    net_socket_test.cpp
    udp_socket_test.cpp
    tcp_socket_test.cpp
)

if(NOT VMS_NETWORK_TEST_SOURCES)
    message(STATUS "No unit tests found in ${CMAKE_CURRENT_SOURCE_DIR}")
    return()
endif()

foreach(TEST_SOURCE IN LISTS VMS_NETWORK_TEST_SOURCES)
    get_filename_component(TEST_NAME "${TEST_SOURCE}" NAME_WE)
    set(TEST_TARGET "test_${TEST_NAME}")

    add_executable(${TEST_TARGET} "${TEST_SOURCE}")
    target_link_libraries(${TEST_TARGET} PRIVATE vms-network)
    target_compile_features(${TEST_TARGET} PRIVATE cxx_std_17)

    add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
endforeach()
