#include "FractalTreeImage.h"

#define PI 3.14159265359

FractalTreeImage::FractalTreeImage(int width, int height, int numBranches, int recursionDepth, int rootWidth) :
    QImage(width, height, QImage::Format_ARGB32)
{
    branches = numBranches;
    maxDepth = recursionDepth;
    this->rootWidth = rootWidth;

    drawTree();
}

void FractalTreeImage::drawTree()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    int width = this->width();
    int height = this->height();
    double baseLength = min(width, height) / 3.5;
    double delta = 180.0 / (2 * branches);
    QList<Endpoint> endpoints;

    //Draw background
    painter.fillRect(rect(), Qt::transparent);
//    painter.fillRect(rect(), Qt::white);

    //Draw base line
    QPoint start(width / 2, height - (height / 10));
    QVector2D baseVec(0, -1);
    QPoint end = add(start,baseLength * baseVec);

    drawLine(start, end, painter, rootWidth);

    //insert endpoint of baseline into endpoints list
    endpoints.push_back(Endpoint{end, baseVec});

    //draw branches
    for (double n = 1; n < maxDepth; n++) {
        double lineWidth = rootWidth - rootWidth * (n/maxDepth);
        lineWidth = max(lineWidth, 1.0);
        double length = baseLength / (n + 1.0);

        QList<Endpoint> newEndpoints;

        for (Endpoint endpoint : endpoints) {

            double angle = delta - 90;

            for (int i = 0; i < branches; i++) {

                double gitter = drand48() * delta - delta / 2.0; //use rand between +- delta/4
                QPoint start = endpoint.point;
                QVector2D direction = rotate(endpoint.vec, angle + gitter);
                QPoint end = add(start, length * direction);
                drawLine(start, end, painter, lineWidth);

                newEndpoints.append(Endpoint{end, direction});

                angle += 2 * delta;
            }

        }
        endpoints.clear();
        endpoints.append(newEndpoints);
    }

    //draw leafs
    for (Endpoint p : endpoints) {
        drawLeaf(p, QColor(0, 198, 0, 200), baseLength / maxDepth, painter);
    }
}

void FractalTreeImage::drawLine(QPoint start, QPoint end, QPainter &painter, float thickness, QColor color) {
    QPen pen(color, thickness, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);

    painter.setPen(pen);

    QVector2D vec(end-start);
    QPoint cp = add(start, vec / 2 + rotate(vec, pow(-1, rand()%2) * 90) / 8);

    QPainterPath path(start);
    path.quadTo(cp, end);

    painter.drawPath(path);
}

void FractalTreeImage::drawLeaf(Endpoint point, QColor color, int length, QPainter &painter) {
    double width = length * 0.6;

    QVector2D vec = rotate(point.vec, rand() % 40 - 20);
    QPoint start = point.point;
    QPoint end = add(start, length * vec);
    QPoint cp1 = add(start, length * vec / 2 + width * rotate(vec, 90));
    QPoint cp2 = add(start, length * vec / 2 + width * rotate(vec, -90));

    QPainterPath path(start);
    path.quadTo(cp1, end);
    path.quadTo(cp2, start);

    painter.fillPath(path,QBrush(color));
}

QPoint FractalTreeImage::add(QPoint p, QVector2D v) {
    return p + v.toPoint();
}

//angle is conterclockwise as usual
QVector2D FractalTreeImage::rotate(QVector2D vec, double angle) {
    double a = -angle * PI / 180.0;
    float x = vec.x() * cos(a) - vec.y() * sin(a);
    float y = vec.x() * sin(a) + vec.y() * cos(a);

    return QVector2D(x, y);
}

