#ifndef __HUMAN_H__
#define __HUMAN_H__

#include "common.h"
#include "mesh.h"

struct Part {
    std::string name;
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
    void Draw(const Mesh* mesh, const Program* program, const sglm::mat4& transform);
    void SetState(int state);
    void Update();
    void Initialize();
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
private:
    Node m_root;

    int32_t m_state = 0; // 1 : idle, 2 : walk, 3 : jump
    int32_t m_idleTime = 0;
    int32_t m_walkTime = 0;
    int32_t m_jumpTime = 0;

    Human() {}
    void Init();
    void DrawNode(const Node& node, const Mesh* mesh, const Program* program, const sglm::mat4& transform);
    void Idle();
    void Walk();
    void Jump();
};


#endif
