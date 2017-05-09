#ifndef __POINT_H__
#define __POINT_H__

#include "drawable.h"
class Point : public Drawable {
    public:
        //constructors
        Point(vec2 p,color3 color);
        Point(GLfloat x,GLfloat y,color3 color);

        //methods
        void render();

        //destructor
        ~Point();
};

#endif
