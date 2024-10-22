#include "Human.h"

HumanUPtr Human::Create() {
    auto human = HumanUPtr(new Human());
    human->Init();
    return std::move(human);
}
// part = {translate, rotate, scale, size, color}
void Human::Init() {
    m_body = {"body", sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(1.0f, 1.0f, 0.3f), sglm::vec3(1.0f, 0.0f, 0.0f)};

    //body 자식
    m_head = {"head", sglm::vec3(0.0f, 0.75f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.5f, 0.5f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};

    //body 자식
    m_leftUpperArm = {"leftUpperArm", sglm::vec3(-0.65f, 0.15f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.3f, 0.7f, 0.3f), sglm::vec3(1.0f, 0.0f, 0.0f)};
    m_rightUpperArm = {"rightUpperArm", sglm::vec3(0.65f, 0.15f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.3f, 0.7f, 0.3f), sglm::vec3(1.0f, 0.0f, 0.0f)};

    //body 자식 - UpperArm 자식
    m_leftLowerArm = {"leftLowerArm", sglm::vec3(0.0f, -0.7f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.3f, 0.7f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};
    m_rightLowerArm = {"leftLowerArm", sglm::vec3(0.0f, -0.7f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.3f, 0.7f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};

    //body 자식
    m_leftUpperLeg = {"leftUpperLeg", sglm::vec3(-0.25f, -0.875f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.45f, 0.75f, 0.3f), sglm::vec3(0.0f, 0.0f, 1.0f)};
    m_rightUpperLeg = {"leftUpperLeg", sglm::vec3(0.25f, -0.875f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.45f, 0.75f, 0.3f), sglm::vec3(0.0f, 0.0f, 1.0f)};

    //body 자식 - UpperLeg 자식
    m_leftLowerLeg = {"leftLowerLeg", sglm::vec3(0.0f, -0.75f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.45f, 0.75f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};
    m_rightLowerLeg = {"leftLowerLeg", sglm::vec3(0.0f, -0.75f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.45f, 0.75f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};


    Node head = {&m_head, {}};
    Node leftLowerArm = {&m_leftLowerArm, {}};
    Node rightLowerArm = {&m_rightLowerArm, {}};
    Node leftLowerLeg = {&m_leftLowerLeg, {}};  
    Node rightLowerLeg = {&m_rightLowerLeg, {}};

    Node leftUpperArm = {&m_leftUpperArm, {leftLowerArm}};
    Node rightUpperArm = {&m_rightUpperArm, {rightLowerArm}};
    Node leftUpperLeg = {&m_leftUpperLeg, {leftLowerLeg}};
    Node rightUpperLeg = {&m_rightUpperLeg, {rightLowerLeg}};
    m_root = {&m_body, {head, leftUpperArm, rightUpperArm, leftUpperLeg, rightUpperLeg}};
}

void Human::Draw(const Mesh* mesh, const Program* program, const sglm::mat4& transform) {
    DrawNode(m_root, mesh, program, transform);
}

void Human::DrawNode(const Node& node, const Mesh* mesh, const Program* program, const sglm::mat4& transform) {
    sglm::vec3 rotationPivot;
    if (node.part->name == "body") {
        rotationPivot = sglm::vec3(0.0f, 0.0f, 0.0f);
    }
    else if (node.part->name == "head") {
        rotationPivot = sglm::vec3(0.0f, - node.part->scale.y * node.part->size.y * 0.5, 0.0f);
    }
    else if (node.part->name == "leftUpperArm" || node.part->name == "rightUpperArm") {
        rotationPivot = sglm::vec3(0.0f, node.part->scale.y * node.part->size.y * 0.9 * 0.5, 0.0f);
    }
    else {
        rotationPivot = sglm::vec3(0.0f, node.part->scale.y * node.part->size.y * 0.5, 0.0f);
    }
    
    sglm::mat4 pivotTransform = sglm::translate(sglm::mat4(1.0f), -1 * rotationPivot);
    sglm::mat4 pivotTransformBack = sglm::translate(sglm::mat4(1.0f), rotationPivot);
    sglm::mat4 translateModel = sglm::translate(sglm::mat4(1.0f), node.part->translate);
    sglm::mat4 rotateModel = sglm::rotate(sglm::mat4(1.0f), sglm::radians(node.part->rotate.x), sglm::vec3(1.0f, 0.0f, 0.0f));
    rotateModel = sglm::rotate(rotateModel, sglm::radians(node.part->rotate.y), sglm::vec3(0.0f, 1.0f, 0.0f));
    rotateModel = sglm::rotate(rotateModel, sglm::radians(node.part->rotate.z), sglm::vec3(0.0f, 0.0f, 1.0f));
    sglm::mat4 scaleModel = sglm::scale(sglm::mat4(1.0f), node.part->scale);
    sglm::mat4 sizeModel = sglm::scale(sglm::mat4(1.0f), node.part->size);
    sglm::mat4 childTransform = transform * translateModel * pivotTransformBack * rotateModel * pivotTransform * scaleModel;
    sglm::mat4 newTransform = childTransform * sizeModel;

    program->Use();
    program->SetUniform("objectColor", sglm::vec4(node.part->color.x, node.part->color.y, node.part->color.z, 1.0f));
    program->SetUniform("transform", newTransform);
    mesh->Draw(program);
    for (const auto& child : node.children) {
        DrawNode(child, mesh, program, childTransform);
    }
}

void Human::SetState(int state) {
    if (m_state == 3)
        return ;
    m_state = state;
}

void Human::Update() {
    Idle();
    Walk();
    Jump();
}

void Human::Idle() {
    if (m_state != 1 && m_idleTime == 0) {
        return ;
    }
    if (m_state == 1 && m_walkTime != 0) {
        return ;
    }
    m_idleTime++;
    if (m_idleTime > 100) {
        m_idleTime = 0;
    }
    float headAngle = 5.0f * sin(sglm::radians(m_idleTime * 1.8f));
    m_head.rotate = sglm::vec3(headAngle - 1.0f, m_head.rotate.y, m_head.rotate.z);
    if (m_idleTime < 50) {
        m_head.translate = sglm::vec3(m_head.translate.x, 0.75f + m_idleTime * 0.0006, m_head.translate.z);
        m_body.size = sglm::vec3((1.0f + m_idleTime * 0.001), (1.0f + m_idleTime * 0.001), m_body.size.z);
        m_leftUpperArm.translate = sglm::vec3(-0.65f - m_idleTime * 0.0006, 0.15f + m_idleTime * 0.0001, m_leftUpperArm.translate.z);
        m_rightUpperArm.translate = sglm::vec3(0.65f + m_idleTime * 0.0006, 0.15f + m_idleTime * 0.0001, m_rightUpperArm.translate.z);
    }
    else {
        m_head.translate = sglm::vec3(m_head.translate.x, 0.75f + (100 - m_idleTime) * 0.0006, m_head.translate.z);
        m_body.size = sglm::vec3((1.0f + (100 - m_idleTime) * 0.001), (1.0f + (100 - m_idleTime) * 0.001), m_body.size.z);
        m_leftUpperArm.translate = sglm::vec3(-0.65f - (100 - m_idleTime) * 0.0006, 0.15f + (100 - m_idleTime) * 0.0001, m_leftUpperArm.translate.z);
        m_rightUpperArm.translate = sglm::vec3(0.65f + (100 - m_idleTime) * 0.0006, 0.15f + (100 - m_idleTime) * 0.0001, m_rightUpperArm.translate.z);
    }
}

void Human::Walk() {
    if (m_state != 2 && m_walkTime == 0) {
        return ;
    }
    m_walkTime++;
    if (m_walkTime > 200) {
        m_walkTime = 0;
    }

    float frontLegAngle = 30.0f * sin(sglm::radians(m_walkTime * 1.8f));
    float backLegAngle = 15.0f * sin(sglm::radians(m_walkTime * 1.8f));
    float lowerLegAngle;
    float bodyTranslate = sin(sglm::radians(m_walkTime * 3.6f)) * 0.05f;
    m_body.translate = sglm::vec3(m_body.translate.x, 0 + bodyTranslate, m_body.translate.z);
    float armAngle = 30.0f * sin(sglm::radians(m_walkTime * 1.8f));
    m_leftUpperArm.rotate = sglm::vec3(-armAngle, m_leftUpperArm.rotate.y, m_leftUpperArm.rotate.z);
    m_rightUpperArm.rotate = sglm::vec3(armAngle, m_rightUpperArm.rotate.y, m_rightUpperArm.rotate.z);

    if (m_walkTime < 100) {
        m_leftUpperLeg.rotate = sglm::vec3(frontLegAngle, m_leftUpperLeg.rotate.y, m_leftUpperLeg.rotate.z);
        m_rightUpperLeg.rotate = sglm::vec3(-backLegAngle, m_rightUpperLeg.rotate.y, m_rightUpperLeg.rotate.z);
        lowerLegAngle = -15.0f * sin(sglm::radians(m_walkTime * 1.8f));
    }
    else {
        m_leftUpperLeg.rotate = sglm::vec3(backLegAngle, m_leftUpperLeg.rotate.y, m_leftUpperLeg.rotate.z); 
        m_rightUpperLeg.rotate = sglm::vec3(-frontLegAngle, m_rightUpperLeg.rotate.y, m_rightUpperLeg.rotate.z); 
        lowerLegAngle = -15.0f * sin(sglm::radians((m_walkTime - 100) * 1.8f));
    }
    m_leftLowerLeg.rotate = sglm::vec3(lowerLegAngle, m_leftLowerLeg.rotate.y, m_leftLowerLeg.rotate.z);
    m_rightLowerLeg.rotate = sglm::vec3(lowerLegAngle, m_rightLowerLeg.rotate.y, m_rightLowerLeg.rotate.z);
}

void Human::Jump() {
    if (m_state != 3 && m_jumpTime == 0) {
        return ;
    }
    if (m_walkTime != 0 || m_idleTime != 0) {
        return ;
    }
    m_jumpTime++;
    if (m_jumpTime > 400) {
        m_jumpTime = 0;
        m_state = 1;
        return ;
    }

    float velocity = 0.06f;
    float gravity = -0.0006f;
    float time = m_jumpTime - 100.0f;
    float jumpHeight = velocity * time + 0.5f * gravity * time * time;


    if (m_jumpTime < 100) {
        m_body.translate = sglm::vec3(m_body.translate.x, 0.0f - sin(sglm::radians(m_jumpTime * 0.9f)) * 0.5f, m_body.translate.z);
        m_body.rotate = sglm::vec3(- sin(sglm::radians(m_jumpTime * 0.9f)) * 45.0f, m_body.rotate.y, m_body.rotate.z);
        m_leftUpperArm.rotate = sglm::vec3(- sin(sglm::radians(m_jumpTime * 0.9f)) * 30.0f, m_leftUpperArm.rotate.y, m_leftUpperArm.rotate.z);
        m_rightUpperArm.rotate = sglm::vec3(- sin(sglm::radians(m_jumpTime * 0.9f)) * 30.0f, m_rightUpperArm.rotate.y, m_rightUpperArm.rotate.z);
        m_leftLowerArm.rotate = sglm::vec3(sin(sglm::radians(m_jumpTime * 0.9f)) * 20.0f, m_leftLowerArm.rotate.y, m_leftLowerArm.rotate.z);
        m_rightLowerArm.rotate = sglm::vec3(sin(sglm::radians(m_jumpTime * 0.9f)) * 20.0f, m_rightLowerArm.rotate.y, m_rightLowerArm.rotate.z);
        m_leftUpperLeg.rotate = sglm::vec3(sin(sglm::radians(m_jumpTime * 0.9f)) * 100.0f, m_leftUpperLeg.rotate.y, m_leftUpperLeg.rotate.z);
        m_rightUpperLeg.rotate = sglm::vec3(sin(sglm::radians(m_jumpTime * 0.9f)) * 100.0f, m_rightUpperLeg.rotate.y, m_rightUpperLeg.rotate.z);
        m_leftLowerLeg.rotate = sglm::vec3(- sin(sglm::radians(m_jumpTime * 0.9f)) * 90.0f, m_leftLowerLeg.rotate.y, m_leftLowerLeg.rotate.z);
        m_rightLowerLeg.rotate = sglm::vec3(- sin(sglm::radians(m_jumpTime * 0.9f)) * 90.0f, m_rightLowerLeg.rotate.y, m_rightLowerLeg.rotate.z);
    }
    else if (m_jumpTime < 200) {
        m_body.translate = sglm::vec3(m_body.translate.x, -0.5f + jumpHeight, m_body.translate.z);
        m_body.rotate = sglm::vec3(-45.0f + sin(sglm::radians((m_jumpTime - 100) * 0.9f)) * 45.0f, m_body.rotate.y, m_body.rotate.z);
        m_leftUpperArm.rotate = sglm::vec3(-30.0f + sin(sglm::radians((m_jumpTime - 100) * 0.9f)) * 210.0f, m_leftUpperArm.rotate.y, m_leftUpperArm.rotate.z);
        m_rightUpperArm.rotate = sglm::vec3(-30.0f + sin(sglm::radians((m_jumpTime - 100) * 0.9f)) * 210.0f, m_rightUpperArm.rotate.y, m_rightUpperArm.rotate.z);
        m_leftUpperLeg.rotate = sglm::vec3(100.0f - sin(sglm::radians((m_jumpTime - 100) * 0.9f)) * 100.0f, m_leftUpperLeg.rotate.y, m_leftUpperLeg.rotate.z);
        m_rightUpperLeg.rotate = sglm::vec3(100.0f - sin(sglm::radians((m_jumpTime - 100) * 0.9f)) * 100.0f, m_rightUpperLeg.rotate.y, m_rightUpperLeg.rotate.z);
        m_leftLowerLeg.rotate = sglm::vec3(-90.0f + sin(sglm::radians((m_jumpTime - 100) * 0.9f)) * 90.0f, m_leftLowerLeg.rotate.y, m_leftLowerLeg.rotate.z);
        m_rightLowerLeg.rotate = sglm::vec3(-90.0f + sin(sglm::radians((m_jumpTime - 100) * 0.9f)) * 90.0f, m_rightLowerLeg.rotate.y, m_rightLowerLeg.rotate.z);
    }
    else if (m_jumpTime < 300) {
        m_body.translate = sglm::vec3(m_body.translate.x, -0.5f + jumpHeight, m_body.translate.z);
        m_body.rotate = sglm::vec3(0.0f - sin(sglm::radians((m_jumpTime - 200) * 0.9f)) * 45.0f, m_body.rotate.y, m_body.rotate.z);
        m_leftUpperArm.rotate = sglm::vec3(180.0f - sin(sglm::radians((m_jumpTime - 200) * 0.9f)) * 210.0f, m_leftUpperArm.rotate.y, m_leftUpperArm.rotate.z);
        m_rightUpperArm.rotate = sglm::vec3(180.0f - sin(sglm::radians((m_jumpTime - 200) * 0.9f)) * 210.0f, m_rightUpperArm.rotate.y, m_rightUpperArm.rotate.z);
        m_leftUpperLeg.rotate = sglm::vec3(0.0f + sin(sglm::radians((m_jumpTime - 200) * 0.9f)) * 100.0f, m_leftUpperLeg.rotate.y, m_leftUpperLeg.rotate.z);
        m_rightUpperLeg.rotate = sglm::vec3(0.0f + sin(sglm::radians((m_jumpTime - 200) * 0.9f)) * 100.0f, m_rightUpperLeg.rotate.y, m_rightUpperLeg.rotate.z);
        m_leftLowerLeg.rotate = sglm::vec3(0.0f - sin(sglm::radians((m_jumpTime - 200) * 0.9f)) * 90.0f, m_leftLowerLeg.rotate.y, m_leftLowerLeg.rotate.z);
        m_rightLowerLeg.rotate = sglm::vec3(0.0f - sin(sglm::radians((m_jumpTime - 200) * 0.9f)) * 90.0f, m_rightLowerLeg.rotate.y, m_rightLowerLeg.rotate.z);
    }
    else {
        m_body.translate = sglm::vec3(m_body.translate.x, -0.5f + sin(sglm::radians((m_jumpTime - 300) * 0.9f)) * 0.5f, m_body.translate.z);
        m_body.rotate = sglm::vec3(-45.0f + sin(sglm::radians((m_jumpTime - 300) * 0.9f)) * 45.0f, m_body.rotate.y, m_body.rotate.z);
        m_leftUpperArm.rotate = sglm::vec3(-30.0f + sin(sglm::radians((m_jumpTime - 300) * 0.9f)) * 30.0f, m_leftUpperArm.rotate.y, m_leftUpperArm.rotate.z);
        m_rightUpperArm.rotate = sglm::vec3(-30.0f + sin(sglm::radians((m_jumpTime - 300) * 0.9f)) * 30.0f, m_rightUpperArm.rotate.y, m_rightUpperArm.rotate.z);
        m_leftLowerArm.rotate = sglm::vec3(20 - sin(sglm::radians((m_jumpTime - 300) * 0.9f)) * 20.0f, m_leftLowerArm.rotate.y, m_leftLowerArm.rotate.z);
        m_rightLowerArm.rotate = sglm::vec3(20 - sin(sglm::radians((m_jumpTime - 300) * 0.9f)) * 20.0f, m_rightLowerArm.rotate.y, m_rightLowerArm.rotate.z);
        m_leftUpperLeg.rotate = sglm::vec3(100.0f - sin(sglm::radians((m_jumpTime - 300) * 0.9f)) * 100.0f, m_leftUpperLeg.rotate.y, m_leftUpperLeg.rotate.z);
        m_rightUpperLeg.rotate = sglm::vec3(100.0f - sin(sglm::radians((m_jumpTime - 300) * 0.9f)) * 100.0f, m_rightUpperLeg.rotate.y, m_rightUpperLeg.rotate.z);
        m_leftLowerLeg.rotate = sglm::vec3(-90.0f + sin(sglm::radians((m_jumpTime - 300) * 0.9f)) * 90.0f, m_leftLowerLeg.rotate.y, m_leftLowerLeg.rotate.z);
        m_rightLowerLeg.rotate = sglm::vec3(-90.0f + sin(sglm::radians((m_jumpTime - 300) * 0.9f)) * 90.0f, m_rightLowerLeg.rotate.y, m_rightLowerLeg.rotate.z);
    }
}

void Human::Initialize() {
    m_state = 1;
    m_idleTime = 0;
    m_walkTime = 0;
    m_jumpTime = 0;
    m_body = {"body", sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(1.0f, 1.0f, 0.3f), sglm::vec3(1.0f, 0.0f, 0.0f)};

    //body 자식
    m_head = {"head", sglm::vec3(0.0f, 0.75f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.5f, 0.5f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};

    //body 자식
    m_leftUpperArm = {"leftUpperArm", sglm::vec3(-0.65f, 0.15f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.3f, 0.7f, 0.3f), sglm::vec3(1.0f, 0.0f, 0.0f)};
    m_rightUpperArm = {"rightUpperArm", sglm::vec3(0.65f, 0.15f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.3f, 0.7f, 0.3f), sglm::vec3(1.0f, 0.0f, 0.0f)};

    //body 자식 - UpperArm 자식
    m_leftLowerArm = {"leftLowerArm", sglm::vec3(0.0f, -0.7f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.3f, 0.7f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};
    m_rightLowerArm = {"leftLowerArm", sglm::vec3(0.0f, -0.7f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.3f, 0.7f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};

    //body 자식
    m_leftUpperLeg = {"leftUpperLeg", sglm::vec3(-0.25f, -0.875f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.45f, 0.75f, 0.3f), sglm::vec3(0.0f, 0.0f, 1.0f)};
    m_rightUpperLeg = {"leftUpperLeg", sglm::vec3(0.25f, -0.875f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.45f, 0.75f, 0.3f), sglm::vec3(0.0f, 0.0f, 1.0f)};

    //body 자식 - UpperLeg 자식
    m_leftLowerLeg = {"leftLowerLeg", sglm::vec3(0.0f, -0.75f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.45f, 0.75f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};
    m_rightLowerLeg = {"leftLowerLeg", sglm::vec3(0.0f, -0.75f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.45f, 0.75f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};
}