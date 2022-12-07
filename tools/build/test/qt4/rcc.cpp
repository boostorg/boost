// (c) Copyright Juergen Hunold 2012
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE.txt or copy at
// https://www.bfgroup.xyz/b2/LICENSE.txt)

#define BOOST_TEST_MODULE QtCore
#include <QtCore>

#include <boost/test/unit_test.hpp>

std::ostream& operator<<(std::ostream& out, QString const& text)
{
    out << text.toUtf8().constData();
    return out;
}

BOOST_AUTO_TEST_CASE (check_exists)
{
    BOOST_CHECK(QFile::exists(":/test/rcc.cpp"));
}
