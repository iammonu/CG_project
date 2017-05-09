#include "square.h"

Square::Square(vec2 p1,vec2 p2,vec2 p3,vec2 p4,color3 color) {
    pointsNum = 4;
    points = new vec2[pointsNum];
    colors = new color3[pointsNum];
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    points[3] = p4;
    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    this->init();
}

void Square::render() {
    glBindVertexArray( vao );
    glDrawArrays( GL_LINE_LOOP , 0, pointsNum);
}

Square::~Square() {
    delete[] points;
    delete[] colors;
}

//-------------
// FILLED SQUARE CLASS

void FilledSquare::render() {
    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, pointsNum);
}

void FilledSquare::update(float xx) {
    for(int i=0;i<pointsNum;++i){
        points[i].x-=xx;
    }

    glBufferData( GL_ARRAY_BUFFER, (sizeof(vec2) + sizeof(vec3)) * pointsNum, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec2) * pointsNum, points);
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec2) * pointsNum, sizeof(vec3) * pointsNum, colors);
    glBindVertexArray( vao );
    // ATTRIBUTES
    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( shaderProgram, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0));

    // Initialize the vertex color attribute from the vertex shader
    GLuint loc2 = glGetAttribLocation( shaderProgram, "vColor" );
    glEnableVertexAttribArray( loc2 );
    glVertexAttribPointer( loc2, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(vec2) * pointsNum) );
    //// it works no idea why :
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec2) * pointsNum, points);
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec2) * pointsNum, sizeof(vec3) * pointsNum, colors);
    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, pointsNum);

}
void FilledSquare::update2(float theta) {
    mat2 m;
    m[0][0]=cos(theta);   m[0][1]=-sin(theta);
    m[1][0]=sin(theta);   m[1][1]=cos(theta);

    for(int i =0;i<4;++i){
        points[i].x += 140;
    }
    for(int i =0;i<4;++i)
        points[i] = m * points[i];

    for(int i =0;i<4;++i){
        points[i].x -= 140;
    }


    glBufferData( GL_ARRAY_BUFFER, (sizeof(vec2) + sizeof(vec3)) * pointsNum, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec2) * pointsNum, points);
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec2) * pointsNum, sizeof(vec3) * pointsNum, colors);
    glBindVertexArray( vao );
    // ATTRIBUTES
    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( shaderProgram, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,BUFFER_OFFSET(0));

////     Initialize the vertex color attribute from the vertex shader
    GLuint loc2 = glGetAttribLocation( shaderProgram, "vColor" );
    glEnableVertexAttribArray( loc2 );
    glVertexAttribPointer( loc2, 3, GL_FLOAT, GL_FALSE, 0,BUFFER_OFFSET(sizeof(vec2) * pointsNum) );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, pointsNum);
}
