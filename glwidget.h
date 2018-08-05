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
        QSurfaceFormat format;
            format.setSamples(8);    // These three lines enable the antialiasing
            setFormat(format);

    }


    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
    void mousePressEvent(QMouseEvent* ev) override;
    void glStencilFunc();
private:
    using uint = unsigned int;
    static const int selectBufferSize = 100;
    std::vector<uint> selectBuffer = std::vector<uint>(selectBufferSize);
    //////////
    GLuint m_posAttr;
        GLuint m_colAttr;
        GLuint m_matrixUniform;
        QOpenGLShaderProgram *m_program;


    /////////
};

#endif // GLWIDGET_H

