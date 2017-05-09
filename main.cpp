#include "include/Angel.h"
#include <cmath>
#include <deque>
#include <algorithm>
#include <map>
#include <set>
#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>

#include "drawable.h"
#include "point.h"
#include "line.h"
#include "square.h"
#include "circle.h"
using namespace std;
std::ifstream f("traffic");
const GLfloat WindowWidth = 900;
const GLfloat WindowHeight = 600;
int frameCount = 0;
//some basic colors
color3 blue  = color3(0,0,1);
color3 red   = color3(1,0,0);
color3 green = color3(0,1,0);
color3 black = color3(0,0,0);
color3 white = color3(1,1,1);
color3 gray = color3(109/255.0,109/255.0,109/255.0);
color3 darkgreen = color3(0,100/255.0,0);


deque<Drawable*> shapes;
map<int,FilledSquare*> cars;
map<int,GLfloat> car_speed;
map<int , size_t> count_points;
deque<int> v;
set<int> leftlane;
map<int ,unsigned long long> cars_time;
//----------------------------------------------------------------------------
void
build_world()
{

    Drawable* road_arc1 = new FilledCircle(vec2(-140,0),275,100,gray,true);
    shapes.push_back(road_arc1);

    Drawable* road_segment_1 = new FilledSquare(vec2(-150,200),vec2(-150,275),vec2(450,200),vec2(450,275),gray);
    shapes.push_back(road_segment_1);

    Drawable* road_segment_2 = new FilledSquare(vec2(-150,-200),vec2(-150,-275),vec2(450,-200),vec2(450,-275),gray);
    shapes.push_back(road_segment_2);

    GLfloat left_roadsegment =-120;
    GLfloat current_seg=450;
    while(current_seg > left_roadsegment){
        current_seg-=450/20.0;
        Drawable* block = new FilledSquare(vec2(current_seg,239.5),vec2(current_seg,235.5),vec2(current_seg+(10.0),239.5),vec2(current_seg+(10.0),235.5),white);
        shapes.push_back(block);
        Drawable* block2 = new FilledSquare(vec2(current_seg,-239.5),vec2(current_seg,-235.5),vec2(current_seg+(10.0),-239.5),vec2(current_seg+(10.0),-235.5),white);
        shapes.push_back(block2);
    }

    Drawable* road_arc2 = new FilledCircle(vec2(-140,0),200,100,darkgreen);
    shapes.push_back(road_arc2);
    for(int i =0 ; i < 1200; i++){
        Drawable* road_arcx = new Circle(vec2(-140,0),(200.0+275.0+(i*.005))/2.0,75,white,true);
        shapes.push_back(road_arcx);
    }

    Drawable* circle = new FilledCircle(vec2(-400,250),30,100,green);
    shapes.push_back(circle);
}
//----------------------------------------------------------------------------
int cont=0;
void
build_cars()
{

    srand (time(NULL));
    GLfloat scale,speed;
    unsigned long long entry_time;
    char lane;
    while(f){
        f>>scale>>entry_time>>speed>>lane;
        GLfloat right=450.0f,top_height=240.0,bottom_height=210.0,width =30*scale,height=15*scale;
        color3 temp =color3((rand()%255)/255.0,(rand()%255)/255.0,(rand()%255)/255.0);
        if(lane == 'l'){
            cars.insert(make_pair(cont,new FilledSquare(vec2(right,bottom_height),
                                                        vec2(right,bottom_height+height),
                                                        vec2(right+(width),bottom_height),
                                                        vec2(right+(width),bottom_height+height),temp)));
            leftlane.insert(cont);
            v.push_back(cont);
            car_speed.insert(make_pair(cont,speed/100.0));
            count_points.insert(make_pair(cont,0));
            cars_time.insert(make_pair(cont,entry_time));
            cont++;
        }else if(lane == 'r'){
            cars.insert(make_pair(cont,new FilledSquare(vec2(right,top_height),
                                                        vec2(right,top_height+height),
                                                        vec2(right+(width),top_height),
                                                        vec2(right+(width),top_height+height),temp)));
            v.push_back(cont);
            car_speed.insert(make_pair(cont,speed/100.0));
            cars_time.insert(make_pair(cont,entry_time));
            count_points.insert(make_pair(cont,0));
            cont++;
        }
    }
}

//-----------------------------------------------------------
void
cleanup()
{
    glUseProgram(0);
    glDeleteProgram(Drawable::shaderProgram);
    //destroy shapes and its vertices
    for (size_t i=0;i<shapes.size();i++){
        delete shapes[i];
    }

}

//-----------------------------------------------------------
void
init( void )
{
    // Load shaders and use the resulting shader program
    Drawable::shaderProgram = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( Drawable::shaderProgram );
    build_world();
    build_cars();
}

//-----------------------------------------------------------
int cnt =0;
float speed_scaler=2.0;
int coll=0;
void
display( void )
{
    frameCount++;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor(0,100/255.0,0,0);
    // clear the window
    for (size_t i=0;i<shapes.size();i++){
        shapes[i]->render();
    }

    for (size_t i : v){
        if(cars_time[i] <= glutGet( GLUT_ELAPSED_TIME )){
            for(size_t j : v){
                if(cars_time[j] <= glutGet( GLUT_ELAPSED_TIME ) && i!=j){
                    if ( (((((FilledSquare *)cars[i])->points[0].y < ((FilledSquare *)cars[j])->points[3].y &&
                            ((FilledSquare *)cars[j])->points[0].y < ((FilledSquare *)cars[i])->points[3].y)
                           ||
                           ((FilledSquare *)cars[i])->points[0].y < ((FilledSquare *)cars[j])->points[3].y && ((FilledSquare *)cars[j])->points[3].y < ((FilledSquare *)cars[i])->points[3].y)) &&
                         (((leftlane.find(i)!=leftlane.end()) &&(leftlane.find(j)!=leftlane.end())) || (((leftlane.find(i)==leftlane.end()) &&(leftlane.find(j)==leftlane.end()))))

                         && ((((FilledSquare *)cars[i])->points[1].x < ((FilledSquare *)cars[j])->points[2].x &&
                              ((FilledSquare *)cars[j])->points[1].x < ((FilledSquare *)cars[i])->points[2].x)
                             ||
                             ((FilledSquare *)cars[i])->points[1].x < ((FilledSquare *)cars[j])->points[2].x && ((FilledSquare *)cars[j])->points[2].x < ((FilledSquare *)cars[i])->points[2].x))

                    {                        
                        car_speed[i]=0;
                        car_speed[j]=0;
                        coll+=1;
                        cout<<"Accident between car " << i<< " and car " <<j <<" at "<< glutGet( GLUT_ELAPSED_TIME ) <<" ms\n";
                    }
                }
            }
            cnt++;
            if(count_points[i]<=1000*(speed_scaler/car_speed[i]) && cars[i]->points[0].x<=-140){
                ((FilledSquare *)cars[i])->update2((speed_scaler*3.1415)/(1000*(1/car_speed[i])));
                count_points[i]+=1;
            }
            else if(cars[i]->points[0].y>0){
                ((FilledSquare *)cars[i])->update(speed_scaler*car_speed[i]);
            }else if(cars[i]->points[0].y<0){
                ((FilledSquare *)cars[i])->update(speed_scaler*-car_speed[i]);
            }
            if((cars[i]->points[3].x>450 && cars[i]->points[3].y < 0) || car_speed[i]==0 ){
                v.erase(std::remove(v.begin(), v.end(), i), v.end());
                cars.erase(i);
                count_points.erase (i);
                car_speed.erase(i);
                cars_time.erase(i);
            }
        }
    }
    glutSwapBuffers();
}

//----------------------------------------------------------------------------
int parity=0;
void
keyboard( unsigned char key, int x, int y )
{

    x=y;
    y=x;
    if ( key == 033) {
        exit( EXIT_SUCCESS );
    }else if (key =='w'|| key == GLUT_KEY_UP){
        speed_scaler*=2.0;
        parity=0;
        glUniform1i(0,parity);
    }else if(key =='s' || key == GLUT_KEY_DOWN){
        if(speed_scaler>0){
            speed_scaler*=.5;
        }else{
            parity=1;
            glUniform1i(0,parity);
        }
    }else if (key ==' '|| key == 'x'){
        if(parity==0){
            parity=1;
            cout<<"Simulation time : "<<(glutGet( GLUT_ELAPSED_TIME ));
            cout<<"\nNumber of Cars : "<<v.size();
            cout<<"\nNumber of Cars : "<<v.size();
            cout<<"\nNumber of accidents : "<<coll<<endl ;
            cout<<"Rate of accidents : "<<coll/float(glutGet( GLUT_ELAPSED_TIME )/(1.0*frameCount))<<endl ;

            speed_scaler=0;
            glUniform1i(0,parity);
        }else{
            parity=0;
            speed_scaler=2.0;
            glUniform1i(0,parity);
        }
    }

}

//----------------------------------------------------------------------------

void timerFunc(int value) {
    if (value != 0){ //not the first call
        stringstream ss;
        ss << "FPS: " << frameCount;
        glutSetWindowTitle(ss.str().c_str());
    }
    //reset frame count to 0
//    frameCount = 0;
    glutTimerFunc ( 500, timerFunc, 1 );
}
//----------------------------------------------------------------------------
int old_Time=0;
void
idle()
{
    glutPostRedisplay();

}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( WindowWidth, WindowHeight);

    glutCreateWindow( "Running" );

    glewInit();

    init();
    glutIdleFunc( idle );
    //    glutTimerFunc( 500, timerFunc, 0 );
    glutDisplayFunc( display );
    glutIdleFunc(idle );
    glutKeyboardFunc( keyboard );

    glutCloseFunc(cleanup);

    glutMainLoop();
    return 0;
}
