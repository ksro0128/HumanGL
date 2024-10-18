#include "context.h"

#include <imgui.h>

ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}

void Context::ProcessInput(GLFWwindow* window) {
    if (!m_cameraControl)
        return;
    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * m_cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)   
        m_cameraPos -= cameraSpeed * m_cameraFront;

    auto cameraRight = sglm::normalize(sglm::cross(m_cameraUp, -1.f *  m_cameraFront));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * cameraRight;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * cameraRight;    

    auto cameraUp = sglm::normalize(sglm::cross(-1.f * m_cameraFront, cameraRight));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * cameraUp;
}

void Context::Reshape(int width, int height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
}

void Context::MouseMove(double x, double y) {
    if (!m_cameraControl)
        return;
    auto pos = sglm::vec2((float)x, (float)y);
    auto deltaPos = pos - m_prevMousePos;

    const float cameraRotSpeed = 0.4f;
    m_cameraYaw -= deltaPos.x * cameraRotSpeed;
    m_cameraPitch -= deltaPos.y * cameraRotSpeed;

    if (m_cameraYaw < 0.0f)   m_cameraYaw += 360.0f;
    if (m_cameraYaw > 360.0f) m_cameraYaw -= 360.0f;

    if (m_cameraPitch > 89.0f)  m_cameraPitch = 89.0f;
    if (m_cameraPitch < -89.0f) m_cameraPitch = -89.0f;

    m_prevMousePos = pos;
}

void Context::MouseButton(int button, int action, double x, double y) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
        // 마우스 조작 시작 시점에 현재 마우스 커서 위치 저장
            m_prevMousePos = sglm::vec2((float)x, (float)y);
            m_cameraControl = true;
        }
        else if (action == GLFW_RELEASE) {
            m_cameraControl = false;
        }
    }
}

bool Context::Init() {
    m_box = Mesh::CreateBox();

    m_program = Program::Create("./shader/shader.vs", "./shader/shader.fs");
    if (!m_program)
        return false;

    m_human = Human::Create();

    glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
    return true;
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    sglm::vec4 tmp =
        sglm::rotate(sglm::mat4(1.0f), sglm::radians(m_cameraYaw), sglm::vec3(0.0f, 1.0f, 0.0f)) *
        sglm::rotate(sglm::mat4(1.0f), sglm::radians(m_cameraPitch), sglm::vec3(1.0f, 0.0f, 0.0f)) *
        sglm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

    m_cameraFront = sglm::vec3(tmp.x, tmp.y, tmp.z);

    auto projection = sglm::perspective(sglm::radians(45.0f), (float)m_width / (float)m_height, 0.01f, 100.0f);
    auto view = sglm::lookAt(
        m_cameraPos,
        m_cameraPos + m_cameraFront,
        m_cameraUp);
    sglm::mat4 translateModel = sglm::mat4(1.0f);
    sglm::mat4 rotateModel = sglm::mat4(1.0f);
    static float angle = 0.0f;
    angle += 1.0f;
    if (angle >= 360.0f)
        angle = 0.0f;
    rotateModel = sglm::rotate(rotateModel, sglm::radians(angle), sglm::vec3(0.0f, 1.0f, 0.0f));
    sglm::mat4 scaleModel = sglm::mat4(1.0f);
    static float scale = 1.0f;
    static float dir = 0.01f;
    scale += dir;
    if (scale >= 2.0f)
        dir = -0.01f;
    if (scale <= 0.5f)
        dir = 0.01f;
    scaleModel = sglm::scale(scaleModel, sglm::vec3(scale, scale, scale));
    auto transform = projection * view * translateModel * rotateModel * scaleModel;

    m_human->Draw(m_box.get(), m_program.get(), transform);
}