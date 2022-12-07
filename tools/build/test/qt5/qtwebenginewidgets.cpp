// (c) Copyright Juergen Hunold 2016
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE.txt or copy at
// https://www.bfgroup.xyz/b2/LICENSE.txt)

#define BOOST_TEST_MODULE QtWebEngineWidgets

#include <QtWebEngineWidgets>

#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineScript>

#include <QApplication>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( defines)
{
    BOOST_CHECK_EQUAL(BOOST_IS_DEFINED(QT_CORE_LIB), true);
    BOOST_CHECK_EQUAL(BOOST_IS_DEFINED(QT_WIDGETS_LIB), true);
    BOOST_CHECK_EQUAL(BOOST_IS_DEFINED(QT_WEBENGINE_LIB), true);
    BOOST_CHECK_EQUAL(BOOST_IS_DEFINED(QT_WEBENGINECORE_LIB), true);
    BOOST_CHECK_EQUAL(BOOST_IS_DEFINED(QT_WEBENGINEWIDGETS_LIB), true);
}

/*!
  Also tests the core library
 */
BOOST_AUTO_TEST_CASE( webengine_widgets )
{
    QApplication app(boost::unit_test::framework::master_test_suite().argc,
                     boost::unit_test::framework::master_test_suite().argv);

    QWebEngineSettings *defaultSettings = QWebEngineSettings::globalSettings();
    QWebEngineProfile *defaultProfile = QWebEngineProfile::defaultProfile();

    defaultSettings->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
    defaultProfile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
}
