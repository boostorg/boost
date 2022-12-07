// (c) Copyright Juergen Hunold 2012
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE.txt or copy at
// https://www.bfgroup.xyz/b2/LICENSE.txt)

#define BOOST_TEST_MODULE QtScriptTools

#include <QScriptEngine>

#include <QScriptEngineDebugger>

#include <QCoreApplication>

#include <boost/test/unit_test.hpp>

#include <iostream>

namespace utf = boost::unit_test::framework;

std::ostream&
operator << (std::ostream& stream, QString const& string)
{
    stream << qPrintable(string);
    return stream;
}

BOOST_AUTO_TEST_CASE( defines)
{
    BOOST_CHECK_EQUAL(BOOST_IS_DEFINED(QT_SCRIPTTOOLS_LIB), true);
}

BOOST_AUTO_TEST_CASE( script )
{
   QCoreApplication app(utf::master_test_suite().argc,
                        utf::master_test_suite().argv);

   QScriptEngine myEngine;
   QScriptValue three = myEngine.evaluate("1 + 2");

   QScriptEngineDebugger debugger;
   debugger.attachTo(&myEngine);

   BOOST_CHECK_EQUAL(three.toNumber(), 3);
   BOOST_CHECK_EQUAL(three.toString(), QLatin1String("3"));

   debugger.detach();
}
