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

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        m_human->SetState(1);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        m_human->SetState(2);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        m_human->SetState(3);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        m_animation = !m_animation;
    }
    
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        m_human->Initialize();
    }
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

    const float cameraRotSpeed = 0.1f;
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
    m_human->SetState(1);

    glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
    return true;
}

void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // //imgui
    if (ImGui::Begin("Human Control")) {
        ImGui::Text("Animation");
        ImGui::Checkbox("Animation", &m_animation);
        if (ImGui::Button("Reset")) {
            m_human->Initialize();
        }
        ImGui::Text("Body");
        ImGui::SliderFloat3("Body Scale", &m_human->m_body.scale[0], 0.1f, 2.0f);
        ImGui::SliderFloat3("Body Size", &m_human->m_body.size[0], 0.1f, 2.0f);
        ImGui::ColorEdit3("Body Color", &m_human->m_body.color[0]);

        ImGui::Text("Head");
        ImGui::SliderFloat3("Translate Head", &m_human->m_head.translate[0], -5.0f, 5.0f);
        ImGui::SliderFloat3("Rotate Head", &m_human->m_head.rotate[0], -180.0f, 180.0f);
        ImGui::SliderFloat3("Scale Head", &m_human->m_head.scale[0], 0.1f, 2.0f);
        ImGui::SliderFloat3("Size Head", &m_human->m_head.size[0], 0.1f, 2.0f);
        ImGui::ColorEdit3("Color Head", &m_human->m_head.color[0]);

        ImGui::Text("Left Upper Arm");
        ImGui::SliderFloat3("Translate Left Upper Arm", &m_human->m_leftUpperArm.translate[0], -5.0f, 5.0f);
        ImGui::SliderFloat3("Rotate Left Upper Arm", &m_human->m_leftUpperArm.rotate[0], -180.0f, 180.0f);
        ImGui::SliderFloat3("Scale Left Upper Arm", &m_human->m_leftUpperArm.scale[0], 0.1f, 2.0f);
        ImGui::SliderFloat3("Size Left Upper Arm", &m_human->m_leftUpperArm.size[0], 0.1f, 2.0f);
        ImGui::ColorEdit3("Color Left Upper Arm", &m_human->m_leftUpperArm.color[0]);

        ImGui::Text("Left Lower Arm");
        ImGui::SliderFloat3("Translate Left Lower Arm", &m_human->m_leftLowerArm.translate[0], -5.0f, 5.0f);
        ImGui::SliderFloat3("Rotate Left Lower Arm", &m_human->m_leftLowerArm.rotate[0], -180.0f, 180.0f);
        ImGui::SliderFloat3("Scale Left Lower Arm", &m_human->m_leftLowerArm.scale[0], 0.1f, 2.0f);
        ImGui::SliderFloat3("Size Left Lower Arm", &m_human->m_leftLowerArm.size[0], 0.1f, 2.0f);
        ImGui::ColorEdit3("Color Left Lower Arm", &m_human->m_leftLowerArm.color[0]);

        ImGui::Text("Right Upper Arm");
        ImGui::SliderFloat3("Translate Right Upper Arm", &m_human->m_rightUpperArm.translate[0], -5.0f, 5.0f);
        ImGui::SliderFloat3("Rotate Right Upper Arm", &m_human->m_rightUpperArm.rotate[0], -180.0f, 180.0f);
        ImGui::SliderFloat3("Scale Right Upper Arm", &m_human->m_rightUpperArm.scale[0], 0.1f, 2.0f);
        ImGui::SliderFloat3("Size Right Upper Arm", &m_human->m_rightUpperArm.size[0], 0.1f, 2.0f);
        ImGui::ColorEdit3("Color Right Upper Arm", &m_human->m_rightUpperArm.color[0]);

        ImGui::Text("Right Lower Arm");
        ImGui::SliderFloat3("Translate Right Lower Arm", &m_human->m_rightLowerArm.translate[0], -5.0f, 5.0f);
        ImGui::SliderFloat3("Rotate Right Lower Arm", &m_human->m_rightLowerArm.rotate[0], -180.0f, 180.0f);
        ImGui::SliderFloat3("Scale Right Lower Arm", &m_human->m_rightLowerArm.scale[0], 0.1f, 2.0f);
        ImGui::SliderFloat3("Size Right Lower Arm", &m_human->m_rightLowerArm.size[0], 0.1f, 2.0f);
        ImGui::ColorEdit3("Color Right Lower Arm", &m_human->m_rightLowerArm.color[0]);

        ImGui::Text("Left Upper Leg");
        ImGui::SliderFloat3("Translate Left Upper Leg", &m_human->m_leftUpperLeg.translate[0], -5.0f, 5.0f);
        ImGui::SliderFloat3("Rotate Left Upper Leg", &m_human->m_leftUpperLeg.rotate[0], -180.0f, 180.0f);
        ImGui::SliderFloat3("Scale Left Upper Leg", &m_human->m_leftUpperLeg.scale[0], 0.1f, 2.0f);
        ImGui::SliderFloat3("Size Left Upper Leg", &m_human->m_leftUpperLeg.size[0], 0.1f, 2.0f);
        ImGui::ColorEdit3("Color Left Upper Leg", &m_human->m_leftUpperLeg.color[0]);

        ImGui::Text("Left Lower Leg");
        ImGui::SliderFloat3("Translate Left Lower Leg", &m_human->m_leftLowerLeg.translate[0], -5.0f, 5.0f);
        ImGui::SliderFloat3("Rotate Left Lower Leg", &m_human->m_leftLowerLeg.rotate[0], -180.0f, 180.0f);
        ImGui::SliderFloat3("Scale Left Lower Leg", &m_human->m_leftLowerLeg.scale[0], 0.1f, 2.0f);
        ImGui::SliderFloat3("Size Left Lower Leg", &m_human->m_leftLowerLeg.size[0], 0.1f, 2.0f);
        ImGui::ColorEdit3("Color Left Lower Leg", &m_human->m_leftLowerLeg.color[0]);

        ImGui::Text("Right Upper Leg");
        ImGui::SliderFloat3("Translate Right Upper Leg", &m_human->m_rightUpperLeg.translate[0], -5.0f, 5.0f);
        ImGui::SliderFloat3("Rotate Right Upper Leg", &m_human->m_rightUpperLeg.rotate[0], -180.0f, 180.0f);
        ImGui::SliderFloat3("Scale Right Upper Leg", &m_human->m_rightUpperLeg.scale[0], 0.1f, 2.0f);
        ImGui::SliderFloat3("Size Right Upper Leg", &m_human->m_rightUpperLeg.size[0], 0.1f, 2.0f);
        ImGui::ColorEdit3("Color Right Upper Leg", &m_human->m_rightUpperLeg.color[0]);

        ImGui::Text("Right Lower Leg");
        ImGui::SliderFloat3("Translate Right Lower Leg", &m_human->m_rightLowerLeg.translate[0], -5.0f, 5.0f);
        ImGui::SliderFloat3("Rotate Right Lower Leg", &m_human->m_rightLowerLeg.rotate[0], -180.0f, 180.0f);
        ImGui::SliderFloat3("Scale Right Lower Leg", &m_human->m_rightLowerLeg.scale[0], 0.1f, 2.0f);
        ImGui::SliderFloat3("Size Right Lower Leg", &m_human->m_rightLowerLeg.size[0], 0.1f, 2.0f);
        ImGui::ColorEdit3("Color Right Lower Leg", &m_human->m_rightLowerLeg.color[0]);
    }
    ImGui::End();


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
    sglm::mat4 translateModel = sglm::translate(sglm::mat4(1.0f), m_modelTranslate);
    sglm::mat4 rotateModel = sglm::mat4(1.0f);
    rotateModel = sglm::rotate(rotateModel, sglm::radians(m_modelRotate.x), sglm::vec3(1.0f, 0.0f, 0.0f));
    rotateModel = sglm::rotate(rotateModel, sglm::radians(m_modelRotate.y), sglm::vec3(0.0f, 1.0f, 0.0f));
    rotateModel = sglm::rotate(rotateModel, sglm::radians(m_modelRotate.z), sglm::vec3(0.0f, 0.0f, 1.0f));
    sglm::mat4 scaleModel = sglm::scale(sglm::mat4(1.0f), m_modelScale);
    auto transform = projection * view * translateModel * rotateModel * scaleModel;

    if (m_animation)
        m_human->Update();
    m_human->Draw(m_box.get(), m_program.get(), transform);
}