#include "line.h"

Line::Line(vec2 s,vec2 e,color3 color) {
    pointsNum = 2;
    points = new vec2[pointsNum];
    colors = new color3[pointsNum];
    points[0] = s;
    points[1] = e;
    colors[0] = color;
    colors[1] = color;
    this->init();
}

Line::Line(GLfloat x1,GLfloat y1,GLfloat x2,GLfloat y2,color3 color) {
    pointsNum = 2;
    points = new vec2[pointsNum];
    colors = new color3 [pointsNum];
    points[0] = vec2(x1,y1);
    points[1] = vec2(x2,y2);
    colors[0] = color;
    colors[1] = color;
    this->init();
}

void Line::render() {
    glBindVertexArray( vao );
    glDrawArrays( GL_LINES , 0, pointsNum);
}

Line::~Line() {
    delete[] points;
    delete[] colors;
}
