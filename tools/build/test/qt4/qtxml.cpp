// (c) Copyright Juergen Hunold 2008
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE.txt or copy at
// https://www.bfgroup.xyz/b2/LICENSE.txt)

#define BOOST_TEST_MODULE QtXml

#include <QtXml>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( defines)
{
    BOOST_CHECK_EQUAL(BOOST_IS_DEFINED(QT_CORE_LIB), true);
    BOOST_CHECK_EQUAL(BOOST_IS_DEFINED(QT_XML_LIB), true);
}

BOOST_AUTO_TEST_CASE( reader_construct)
{
    QXmlStreamReader reader;
    BOOST_CHECK_EQUAL(reader.atEnd(), false);
}

BOOST_AUTO_TEST_CASE( writer_construct)
{
    QXmlStreamWriter writer;
    BOOST_CHECK_EQUAL(writer.device(), static_cast<QIODevice*>(0));
}

