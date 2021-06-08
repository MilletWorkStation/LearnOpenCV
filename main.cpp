#include "mainwindow.h"
#include <QApplication>

#include "BasicConceptsAndOperations.h"
#include "basicgraphicsdrawing.h"
#include "ImageReadAndSave.h"
#include "imageroi.h"
#include "imagesplitandmerge.h"
#include "contrastandbrightness.h"
#include "filterwindow.h"
#include "morphologywindow.h"
#include "imageidentifywindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();


    // BasicConceptsAndOperations();

    // ImageReadAndSave();

    // BasicGraphicsDrawing();

    //ImageROI();

    // ImageSplitAndMerge();

//    ContrastAndBrightness w;
//    w.show();

//    FilterWindow w;
//    w.show();

//    MorphologyWindow w;
//    w.show();

    ImageIdentifyWindow w;
    w.show();

    return a.exec();
}
