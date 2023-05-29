/** Statistic analyzer sourse-main file
 * 
 * Copyright (C) Gubaydullin Nurislam, School 21. All Rights Reserved
 * 
 * If the code of this project has helped you in any way,
 * please thank me with a cup of beer.
 * 
 * gmail:    gubaydullin.nurislam@gmail.com
 * telegram: @tonitaga
 */

#include "./View/laboratoryview.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    LaboratoryView w;
    w.show();
    return a.exec();
}
