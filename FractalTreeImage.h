#ifndef FRACTALTREEPIXMAP_H
#define FRACTALTREEPIXMAP_H

#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <math.h>
#include <QPoint>
#include <QVector2D>
#include <QList>
#include <QPen>
#include <QColor>
#include <QPaintEngine>
#include <time.h>

using namespace std;

class FractalTreeImage : public QImage
{
public:
    explicit FractalTreeImage(int width = 100, int height = 100, int numBranches = 2, int recursionDepth = 1, int rootWidth = 4,
                              float leafSize = 0.1, unsigned int seed = 0, QColor treeColor = Qt::black, QColor leafColor = QColor(0, 198, 0, 200));

    explicit FractalTreeImage(int width = 100, int height = 100, int numBranches = 2, int recursionDepth = 1, int rootWidth = 4,
                              float leafSize = 0.1, unsigned int seed = 0, QColor treeColor = Qt::black, QList<QColor> leafColors = QList<QColor>());
    unsigned int getSeed();

protected:
    struct Endpoint {
        QPoint point;
        QVector2D vec;
    };

    double branches;
    int maxDepth;
    int rootWidth;
    unsigned int seed;
    float leafSize;
    QList<QColor> leafColors;
    QColor treeColor;

    void drawTree();

    void drawLine(QPoint start, QPoint end, QPainter &painter, float thickness = 1);
    void drawLeaf(Endpoint point, int length, QPainter &painter);
    QPoint add(QPoint p, QVector2D v);
    QVector2D rotate(QVector2D vec, double angle);


};

#endif // FRACTALTREEPIXMAP_H
