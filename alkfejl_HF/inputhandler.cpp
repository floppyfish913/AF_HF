#include <QQuickView>
#include <QQuickItem>
#include <QObject>

QQuickView* myObject;
QQuickItem* object = myObject->rootObject();
QObject textin = object->findChild<QObject>("textInputP");
QString inputstringP = textin->property("text").toString();
