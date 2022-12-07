include(BoostTest OPTIONAL RESULT_VARIABLE HAVE_BOOST_TEST)

if(HAVE_BOOST_TEST)
    boost_test(TYPE run NAME convert_test_callable SOURCES callable.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_fallbacks SOURCES fallbacks.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_spirit_converter SOURCES spirit_converter.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_stream_converter SOURCES stream_converter.cpp LINK_LIBRARIES Boost::convert Boost::included_unit_test_framework)
    boost_test(TYPE run NAME convert_test_printf_converter SOURCES printf_converter.cpp LINK_LIBRARIES Boost::convert Boost::included_unit_test_framework)
    boost_test(TYPE run NAME convert_test_strtol_converter SOURCES strtol_converter.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_lcast_converter SOURCES lcast_converter.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_encryption SOURCES encryption.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_user_type SOURCES user_type.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_str_to_int SOURCES str_to_int.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_sfinae SOURCES sfinae.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_has_member SOURCES has_member.cpp LINK_LIBRARIES Boost::convert)

    # these tests require C++17
    boost_test(TYPE run NAME convert_test_charconv_converter SOURCES charconv_converter.cpp LINK_LIBRARIES Boost::convert Boost::included_unit_test_framework COMPILE_FEATURES cxx_std_17)
endif()

function(boost_exe)
    cmake_parse_arguments("BOOST_EXE" "" "NAME" "SOURCES;LINK_LIBRARIES;COMPILE_DEFINITIONS;COMPILE_OPTIONS;COMPILE_FEATURES;INCLUDE_DIRECTORIES" ${ARGN})

    add_executable(${BOOST_EXE_NAME} EXCLUDE_FROM_ALL ${BOOST_EXE_SOURCES})
    target_link_libraries(${BOOST_EXE_NAME} PRIVATE ${BOOST_EXE_LINK_LIBRARIES})
    target_include_directories(${BOOST_EXE_NAME} PRIVATE ${BOOST_EXE_INCLUDE_DIRECTORIES})
    target_compile_definitions(${BOOST_EXE_NAME} PRIVATE ${BOOST_EXE_COMPILE_DEFINITIONS})
    target_compile_options(${BOOST_EXE_NAME} PRIVATE ${BOOST_EXE_COMPILE_OPTIONS})
    target_compile_features(${BOOST_EXE_NAME} PRIVATE ${BOOST_EXE_COMPILE_FEATURES})
endfunction()

boost_exe(NAME convert_test_performance SOURCES performance.cpp LINK_LIBRARIES Boost::convert Boost::timer Boost::chrono)
boost_exe(NAME convert_test_performance_spirit SOURCES performance_spirit.cpp LINK_LIBRARIES Boost::convert INCLUDE_DIRECTORIES ${Boost_SOURCE_DIR} COMPILE_FEATURES cxx_std_17)
