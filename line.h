#ifndef __LINE_H__
#define __LINE_H__

#include "drawable.h"

class Line : public Drawable {
    public:

        //constructors
        Line(vec2 s,vec2 e,color3 color);
        Line(GLfloat x1,GLfloat y1,GLfloat x2,GLfloat y2,color3 color);

        //methods
        void render();

        //destructor
        ~Line();
};

#endif
