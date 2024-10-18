#ifndef __HUMAN_H__
#define __HUMAN_H__

#include "common.h"
#include "mesh.h"

struct Part {
    sglm::vec3 translate;
    sglm::vec3 rotate;
    sglm::vec3 scale;
    sglm::vec3 size;
    sglm::vec3 color;
};

struct Node {
    Part* part;
    std::vector<Node> children;
};

struct Transform {
    sglm::mat4& projection;
    sglm::mat4& view;
    sglm::mat4& translateModel;
    sglm::mat4& rotateModel;
    sglm::mat4& scaleModel;
};


CLASS_PTR(Human)
class Human {
public:
    static HumanUPtr Create();
    void Draw(const Mesh* mesh, const Program* program, const sglm::mat4& transform) const;
private:
    Part m_body;
    Part m_head;
    Part m_leftUpperArm;
    Part m_rightUpperArm;
    Part m_leftUpperLeg;
    Part m_rightUpperLeg;
    Part m_leftLowerArm;
    Part m_rightLowerArm;
    Part m_leftLowerLeg;
    Part m_rightLowerLeg;
    Node m_root;

    Human() {}
    void Init();
    void DrawNode(const Node& node, const Mesh* mesh, const Program* program, const sglm::mat4& transform) const;
};


#endif
