#include "point.h"

Point::Point(vec2 p,color3 color) {
    pointsNum = 1 ;
    points = new vec2[pointsNum];
    colors = new color3[pointsNum];
    points[0] = p;
    colors[0] = color;
    this->init();
}


Point::Point(GLfloat x,GLfloat y,color3 color) {
    pointsNum = 1 ;
    points = new vec2[pointsNum];
    colors = new color3[pointsNum];
    points[0] = vec2(x,y);
    colors[0] = color;
    this->init();
}


void Point::render() {
    glBindVertexArray( vao );
    glDrawArrays( GL_POINTS , 0, pointsNum);
}

Point::~Point() {
    delete[] points;
    delete[] colors;
}
