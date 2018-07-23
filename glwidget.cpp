#include "glwidget.h"
#include <QGLWidget>
#include <QtOpenGL/QtOpenGL>
///////////

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>


////////
#include "bad.h"
#include "GL/glu.h"
#include "uarmext.h"

#define farend 300

extern long pos2;
extern int cameraposZ;
extern int posx;
extern Bad *Gear2;
extern Bad *main_gear;
extern Bad *main_gear3;
extern Bad *main_bar;
extern Bad *arm1;
extern Bad *arm2;
extern Bad *arm3;
extern Bad *small_gear1;
extern Bad *small_gear2;
extern Bad *small_gear3;
extern Bad *upper_triangle;
extern Bad *motor1;
extern Bad *motor2;
extern Bad *motor3;
extern Bad *horizontal;
extern Bad *base;
extern Bad *bottom_plate;
extern Bad *left1;
extern Bad *rigth1;
extern Bad *palanca2;
extern Bad *sphere;
extern Bad *secondtriangle;
extern Bad *SG90;
extern Bad *SG90holder;
extern Bad *plane;
extern Bad *esfera;

extern float angle;
float aspect;

extern bool ORTHO;
extern bool PROJECTED;

GLfloat light0_ambient[] =  {0.4f, 0.4f, 0.4f, 1.0f};
GLfloat light0_diffuse[] =  {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light0_specular[]=  {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light0_position[] = {-10.0f, 1.0f, 1.0f, 0.0f};



static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = vec4(1, 0, 0, 1);\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";


void GLWidget::initializeGL()
{



                         // glMatrixMode(GL_PROJECTION);
                         // glLoadIdentity(); //reset projection matrix
                         // gluPerspective(50.0f, 640/480, 1.0f, 1000.0f);
                          glMatrixMode(GL_MODELVIEW); //set modelview matrix
                          glLoadIdentity(); //reset modelview matrix
                          glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
                          glEnable(GL_LIGHTING);
                          glEnable(GL_LIGHT0);
                          glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
                          glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
                          glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
                          glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
                          glEnable(GL_DEPTH_TEST);
                          glEnable(GL_COLOR_MATERIAL);

/*
                          glClearColor(0,0,0,1);
                            glEnable(GL_DEPTH_TEST);
                            glEnable(GL_MULTISAMPLE);
                            glEnable(GL_COLOR_MATERIAL);
                            glEnable(GL_LIGHTING);
                            glEnable(GL_LIGHT0);
                            glEnable(GL_LIGHT1);
                            glEnable(GL_NORMALIZE);
                            glEnable(GL_SMOOTH);
                            glShadeModel(GL_SMOOTH);
                          //glEnable( GL_POLYGON_SMOOTH );
                          //glEnable(GL_BLEND);
                         // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




                          glEnable(GL_MULTISAMPLE);
                          GLint bufs;
                            GLint samples;
                            glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
                            glGetIntegerv(GL_SAMPLES, &samples);
                            qDebug("Have %d buffers and %d samples", bufs, samples);
 */

    glClearColor(0.70f, 0.8f, 0.8f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

/*
    glEnable(GL_COLOR_MATERIAL);
       glEnable(GL_MULTISAMPLE);
*/
    /*   glEnable (GL_POINT_SMOOTH);
       glHint(GL_POINT_SMOOTH, GL_NICEST);
       glEnable(GL_POINT_SMOOTH);
       glEnable(GL_LINE_SMOOTH);
       glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
       glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);

*/
    // Create Shader (Do not release until VAO is created)
   /*     m_program = new QOpenGLShaderProgram(this);
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
        m_program->link();
        m_posAttr = m_program->attributeLocation("posAttr");
        m_colAttr = m_program->attributeLocation("colAttr");
        m_matrixUniform = m_program->attributeLocation("matrix");

        m_program->release();

*/

}

void GLWidget::paintGL()
{
            glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

            ///////////////
            int renderMode;
            glGetIntegerv(GL_RENDER_MODE, &renderMode);


        if (renderMode != GL_SELECT) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45.0, aspect, 1.0, 1500.0);
        }
        if(ORTHO==true){

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-farend, farend, -farend, farend, 650.0f, -650.0f);
        }
        else{
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45.0, aspect, 1.0, 1500.0);
        }

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(0-posx,650-pos2,50+cameraposZ, 0-posx,0,50+cameraposZ, 0,0,1 );//pos2 is the depth of the camera from the UI
            glEnable(GL_DEPTH_TEST);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            glEnable(GL_MULTISAMPLE);
            glEnable(GL_LINE_SMOOTH);


            angle =  (float) posx;
            if (angle >= 360.0f)
            {
                    angle = 0.0f;
            }

            if(DARM)
            {
             Gear2->draw();
             main_gear->draw();
             main_gear3->draw();
             main_bar->draw();
             arm1->draw();
             arm2->draw();
             arm3->draw();
             small_gear1->draw();
             small_gear2->draw();
             small_gear3->draw();
             upper_triangle->draw();
             motor1->draw();
             motor2->draw();
             motor3->draw();
             horizontal->draw();
             base->draw();
             bottom_plate->draw();
             left1->draw();
             rigth1->draw();
             palanca2->draw();
             sphere->draw();
             secondtriangle->draw();
             SG90->draw();
             SG90holder->draw();

            }
             if(esfera!=NULL){
                 esfera->draw();
                 //qDebug()<<"in draw";
             }
            if(UARM)
            {
                base01->draw();
                base02->draw();
                base03->draw();
                base05->draw();
                base06->draw();
                base07->draw();
                base08->draw();
                base09->draw();
                base10->draw();
                base11->draw();
                base12->draw();
                base13->draw();
                base14->draw();
                bearing->draw();
                bearing_f5x9x012->draw();
                board1->draw();
                bolt->draw();
                bolt22->draw();
                bolt23->draw();
                bolt27->draw();
                bolt39->draw();
                DJ001->draw();
                DJ002->draw();
                Dj9g->draw();
                DP->draw();
                DP001->draw();
                DP002->draw();
                link1->draw();
                link2->draw();
                link_3->draw();
                link3->draw();
                link004->draw();
                link4->draw();
                link005->draw();
                link5->draw();
                link006->draw();
                link6->draw();
                link7->draw();
                link8->draw();
                s1->draw();
                s002->draw();
                s2->draw();
                STM4_22->draw();
            }
            plane->draw();

             //glPopMatrix();
             glFlush();

            update();

}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w/h, 0.01 ,1500.0);
    aspect=(float)w/h;

}


void GLWidget::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble xmin, xmax, ymin, ymax;

    ymax = zNear * tan( fovy * M_PI / 360.0 );
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    //glFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}

void GLWidget::mousePressEvent(QMouseEvent* ev){
    qDebug("mouse clicked\n");
    qDebug()<<"x: "<< ev->x()<< " Y:"<<ev->y();
    // Selection buffer
    std::fill(selectBuffer.begin(), selectBuffer.end(), 0);
    glSelectBuffer(selectBufferSize, &selectBuffer[0]);

    // Draw for selection buffer
    glRenderMode(GL_SELECT);

    // Matrix setting
    glMatrixMode(GL_PROJECTION);
     glPushMatrix();
        glLoadIdentity();
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        gluPickMatrix(ev->x(), height() - ev->y(), 5, 5, viewport);
        const float aspect = static_cast<float>(viewport[2]) / viewport[3];
        gluPerspective(45.0, aspect, 1.0, 1500.0);

        // Draw
   //     paintGL();
        // Reset matrix setting
        glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // Revert render mode
    int hits = glRenderMode(GL_RENDER);

    // Show selection
    qDebug()<<"hits "<< hits;
    if (hits > 0) {
        int id = 0;
        for (int i = 0; i < hits; i++) {
            printf("Level: %u\n", selectBuffer[id + 0]);
            printf("  Min: %f\n", (double)selectBuffer[id + 1] / UINT_MAX);
            printf("  Max: %f\n", (double)selectBuffer[id + 2] / UINT_MAX);
            printf("   ID: %u\n", selectBuffer[id + 3]);
            id += 4;
        }
    }
}
