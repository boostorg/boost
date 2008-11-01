if (BUILD_DOCUMENTATION_HTML)
  # Install style sheets and the main Boost logo
  install(FILES boostbook.css docutils.css reference.css ../../boost.png
    DESTINATION share/boost-${BOOST_VERSION}/html)

  # Install images
  install(DIRECTORY images
    DESTINATION share/boost-${BOOST_VERSION}/html
    COMPONENT Core
    PATTERN "CVS" EXCLUDE
    PATTERN ".svn" EXCLUDE)
endif ()

