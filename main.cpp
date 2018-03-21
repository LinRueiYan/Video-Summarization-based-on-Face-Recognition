#include "facerecognition.h"
#include <qapplication.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FaceRecognition w;
	w.show();
	return a.exec();

}
