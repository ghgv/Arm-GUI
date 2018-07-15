#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <qtopengl\QtOpenGL>
#include <QGLFormat>



class GLWidget : public QOpenGLWidget
{
public:
    GLWidget(QWidget* parent) :
        QOpenGLWidget(parent)
    {
setFormat(QSurfaceFormat::defaultFormat());
    }
  // QGLWidget::setFormat(QGLFormat(QGL::SampleBuffers));

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

    //////////
    GLuint m_posAttr;
        GLuint m_colAttr;
        GLuint m_matrixUniform;
        QOpenGLShaderProgram *m_program;


    /////////
};

#endif // GLWIDGET_H

