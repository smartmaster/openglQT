#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>

#include <glm/glm.hpp>

#include "SMLAxisSystem.h"

class QTimer;

class MyOglWidget : public QOpenGLWidget, public QOpenGLFunctions_4_5_Core
{
    Q_OBJECT

public:
    explicit MyOglWidget(QWidget *parent = nullptr);
    virtual ~MyOglWidget() override;

signals:


public:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    //void on_frameSwapped();
    void on_timeout();

private:
    void CreateProgram(const GLchar * const vertSource, const GLchar *const  fragSource);
    void ResetEye();

private:
    void DebugPoc(GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar *message);
    static void APIENTRY DEBUGPROC(GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar *message,
                const void *userParam);

private:
    GLuint _programId{GLuint(-1)};
    GLuint _vao{GLuint(-1)};

    GLuint _vboPos{GLuint(-1)};
    GLuint _vboColor{GLuint(-1)};
    GLuint _vboElemet{GLuint(-1)};

    GLuint _vboPosLine{GLuint(-1)};
    GLuint _vboColorLine{GLuint(-1)};
    GLuint _vboElemetLine{GLuint(-1)};



    GLint _mvpLocation{-1};

    QTimer* _updateTimer{nullptr};


    glm::mat4 _eyeAxis{1.0f};
    glm::vec3 _eye{0.0f, 0.0f, 0.0f};
    float _offsetZ{0.0f};
    bool _dirInc{false};

    bool _axisInited{false};

   inline static constexpr int posLocation = 0;
   inline static constexpr int colorLocation = 1;


    SmartLib::AxisSystem<float> _axisModel;
    glm::mat4 _frustum;


};


