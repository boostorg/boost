#
# Copyright Troy D. Straszheim
#
# Distributed under the Boost Software License, Version 1.0.
# See http://www.boost.org/LICENSE_1_0.txt
#
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

