foreach(_name
        anisotropic_diffusion
        hough_circle_transform
        hough_line_transform
        hough_parameter)
    set(_test t_ext_image_processing_${_name})
    set(_target test_ext_image_processing_${_name})

    add_executable(${_target} "")
    target_sources(${_target} PRIVATE ${_name}.cpp)
    target_link_libraries(${_target}
            PRIVATE
            gil_compile_options
            gil_include_directories
            gil_dependencies)
    target_compile_definitions(${_target} PRIVATE BOOST_GIL_USE_CONCEPT_CHECK)
    add_test(NAME ${_test} COMMAND ${_target})

    unset(_name)
    unset(_target)
    unset(_test)
endforeach()
