#include "Human.h"

HumanUPtr Human::Create() {
    auto human = HumanUPtr(new Human());
    human->Init();
    return std::move(human);
}
// part = {translate, rotate, scale, size, color}
void Human::Init() {
    m_body = {sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(1.0f, 1.0f, 0.3f), sglm::vec3(1.0f, 0.0f, 0.0f)};

    //body 자식
    m_head = {sglm::vec3(0.0f, 0.75f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.5f, 0.5f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};

    //body 자식
    m_leftUpperArm = {sglm::vec3(-0.65f, 0.15f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.3f, 0.7f, 0.3f), sglm::vec3(1.0f, 0.0f, 0.0f)};
    m_rightUpperArm = {sglm::vec3(0.65f, 0.15f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.3f, 0.7f, 0.3f), sglm::vec3(1.0f, 0.0f, 0.0f)};

    //body 자식 - UpperArm 자식
    m_leftLowerArm = {sglm::vec3(0.0f, -0.7f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.3f, 0.7f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};
    m_rightLowerArm = {sglm::vec3(0.0f, -0.7f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.3f, 0.7f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};

    //body 자식
    m_leftUpperLeg = {sglm::vec3(-0.25f, -0.875f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.45f, 0.75f, 0.3f), sglm::vec3(0.0f, 0.0f, 1.0f)};
    m_rightUpperLeg = {sglm::vec3(0.25f, -0.875f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.45f, 0.75f, 0.3f), sglm::vec3(0.0f, 0.0f, 1.0f)};

    //body 자식 - UpperLeg 자식
    m_leftLowerLeg = {sglm::vec3(0.0f, -0.75f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.45f, 0.75f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};
    m_rightLowerLeg = {sglm::vec3(0.0f, -0.75f, 0.0f), sglm::vec3(0.0f, 0.0f, 0.0f), sglm::vec3(1.0f, 1.0f, 1.0f), sglm::vec3(0.45f, 0.75f, 0.3f), sglm::vec3(0.767f, 0.634f, 0.591f)};


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

void Human::Draw(const Mesh* mesh, const Program* program, const sglm::mat4& transform) const {
    DrawNode(m_root, mesh, program, transform);
}

// 제일 처음 오는 scale만 적용하고 나머지는 scale을 적용하지 않는다.
// 그렇게 (projection * view * translate * rotate) * scale * translate * rotate * scale 또는 자식단에서는
// (projection * view * translate * rotate * scale) * translate * rotate * translate * rotate * scale 이렇게

// 왜 이런 잘못된것을 사용했나 -> tranlate을 scale적용한 것으로 사용하면 사람이 보기에 직관적이지가 않아서.
void Human::DrawNode(const Node& node, const Mesh* mesh, const Program* program, const sglm::mat4& transform) const {
    
    sglm::mat4 translateModel = sglm::translate(sglm::mat4(1.0f), node.part->translate);
    sglm::mat4 rotateModel = sglm::rotate(sglm::mat4(1.0f), sglm::radians(node.part->rotate.x), sglm::vec3(1.0f, 0.0f, 0.0f));
    rotateModel = sglm::rotate(rotateModel, sglm::radians(node.part->rotate.y), sglm::vec3(0.0f, 1.0f, 0.0f));
    rotateModel = sglm::rotate(rotateModel, sglm::radians(node.part->rotate.z), sglm::vec3(0.0f, 0.0f, 1.0f));
    sglm::mat4 scaleModel = sglm::scale(sglm::mat4(1.0f), node.part->scale);
    sglm::mat4 sizeModel = sglm::scale(sglm::mat4(1.0f), node.part->size);
    sglm::mat4 childTransform = transform * translateModel * rotateModel * scaleModel;
    sglm::mat4 newTransform = childTransform * sizeModel;

    program->Use();
    program->SetUniform("objectColor", sglm::vec4(node.part->color.x, node.part->color.y, node.part->color.z, 1.0f));
    program->SetUniform("transform", newTransform);
    mesh->Draw(program);
    for (const auto& child : node.children) {
        DrawNode(child, mesh, program, childTransform);
    }
}