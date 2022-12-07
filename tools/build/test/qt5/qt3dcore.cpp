// (c) Copyright Juergen Hunold 2015
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE.txt or copy at
// https://www.bfgroup.xyz/b2/LICENSE.txt)

#define BOOST_TEST_MODULE Qt3DCore
#include <Qt3DCore>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE (defines)
{
    BOOST_CHECK_EQUAL(BOOST_IS_DEFINED(QT_3DCORE_LIB), true);
}

BOOST_AUTO_TEST_CASE ( sample_code )
{
    Qt3DCore::QTransform torusTransform;
    torusTransform.setScale3D(QVector3D(1.5, 1, 0.5));
    torusTransform.setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 45.0f));
}
