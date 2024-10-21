#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "mesh.h"
#include "Human.h"

CLASS_PTR(Context)
class Context {
public:
    static ContextUPtr Create();
    void Render();
    void ProcessInput(GLFWwindow* window);
    void Reshape(int width, int height);
    void MouseMove(double x, double y);
    void MouseButton(int button, int action, double x, double y);

private:
    Context() {}
    bool Init();
    ProgramUPtr m_program;
    ProgramUPtr m_simpleProgram;
    MeshUPtr m_box;
    HumanUPtr m_human;


    // animation
    bool m_animation { true };

    // clear color
    sglm::vec4 m_clearColor { sglm::vec4(0.1f, 0.2f, 0.3f, 0.0f) };

    // light parameter
    struct Light {
        sglm::vec3 position { sglm::vec3(2.0f, 2.0f, 2.0f) };
        sglm::vec3 direction { sglm::vec3(-1.0f, -1.0f, -1.0f) };
        sglm::vec2 cutoff { sglm::vec2(20.0f, 5.0f) };
        float distance { 32.0f };
        sglm::vec3 ambient { sglm::vec3(0.1f, 0.1f, 0.1f) };
        sglm::vec3 diffuse { sglm::vec3(0.5f, 0.5f, 0.5f) };
        sglm::vec3 specular { sglm::vec3(1.0f, 1.0f, 1.0f) };
    };
    Light m_light;
    bool m_flashLightMode { false };


    // camera parameters
    bool m_cameraControl { false };
    sglm::vec2 m_prevMousePos { sglm::vec2(0.0f) };
    float m_cameraPitch { 0.0f };
    float m_cameraYaw { 0.0f };
    sglm::vec3 m_cameraPos { sglm::vec3(0.0f, 0.0f, 3.0f) };
    sglm::vec3 m_cameraFront { sglm::vec3(0.0f, 0.0f, -1.0f) };
    sglm::vec3 m_cameraUp { sglm::vec3(0.0f, 1.0f, 0.0f) };
    int m_width {WINDOW_WIDTH};
    int m_height {WINDOW_HEIGHT};

    // model transform
    sglm::vec3 m_modelTranslate { sglm::vec3(0.0f) };
    sglm::vec3 m_modelRotate { sglm::vec3(0.0f, 180.0f, 0.0f) };
    sglm::vec3 m_modelScale { sglm::vec3(1.0f) };

};

#endif // __CONTEXT_H__