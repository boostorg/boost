// Copyright Vladimir Prus 2005.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt
// or copy at https://www.bfgroup.xyz/b2/LICENSE.txt)

#include "arrow.h"

#include <QApplication>
#include <QTimer>

int main(int ac, char* av[])
{
    QApplication app(ac, av);
    Arrow_widget* w = new Arrow_widget;
    w->resize(1100, 480);

    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()),
                     w, SLOT(slotChangeColor()));

    timer.start(2000);

    w->show();
    app.exec();
    return 0;
}

