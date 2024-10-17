#include "mesh.h"

MeshUPtr Mesh::Create(
    const std::vector<Vertex>& vertices,
    const std::vector<uint32_t>& indices,
    uint32_t primitiveType) {
    auto mesh = MeshUPtr(new Mesh());
    mesh->Init(vertices, indices, primitiveType);
    return std::move(mesh);
}

void Mesh::Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t primitiveType) {
    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData(
        GL_ARRAY_BUFFER, GL_STATIC_DRAW,
        vertices.data(), sizeof(Vertex), vertices.size());
    m_indexBuffer = Buffer::CreateWithData(
        GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW,
        indices.data(), sizeof(uint32_t), indices.size());
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, false,
        sizeof(Vertex), 0);
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, false,
        sizeof(Vertex), offsetof(Vertex, normal));
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, false,
        sizeof(Vertex), offsetof(Vertex, texCoord));
}

void Mesh::Draw(const Program* program) const {
  m_vertexLayout->Bind();
  glDrawElements(m_primitiveType, m_indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
}

MeshUPtr Mesh::CreateBox() {
    std::vector<Vertex> vertices = {
        Vertex { sglm::vec3(-0.5f, -0.5f, -0.5f), sglm::vec3( 0.0f,  0.0f, -1.0f), sglm::vec2(0.0f, 0.0f) },
        Vertex { sglm::vec3( 0.5f, -0.5f, -0.5f), sglm::vec3( 0.0f,  0.0f, -1.0f), sglm::vec2(1.0f, 0.0f) },
        Vertex { sglm::vec3( 0.5f,  0.5f, -0.5f), sglm::vec3( 0.0f,  0.0f, -1.0f), sglm::vec2(1.0f, 1.0f) },
        Vertex { sglm::vec3(-0.5f,  0.5f, -0.5f), sglm::vec3( 0.0f,  0.0f, -1.0f), sglm::vec2(0.0f, 1.0f) },

        Vertex { sglm::vec3(-0.5f, -0.5f,  0.5f), sglm::vec3( 0.0f,  0.0f,  1.0f), sglm::vec2(0.0f, 0.0f) },
        Vertex { sglm::vec3( 0.5f, -0.5f,  0.5f), sglm::vec3( 0.0f,  0.0f,  1.0f), sglm::vec2(1.0f, 0.0f) },
        Vertex { sglm::vec3( 0.5f,  0.5f,  0.5f), sglm::vec3( 0.0f,  0.0f,  1.0f), sglm::vec2(1.0f, 1.0f) },
        Vertex { sglm::vec3(-0.5f,  0.5f,  0.5f), sglm::vec3( 0.0f,  0.0f,  1.0f), sglm::vec2(0.0f, 1.0f) },

        Vertex { sglm::vec3(-0.5f,  0.5f,  0.5f), sglm::vec3(-1.0f,  0.0f,  0.0f), sglm::vec2(1.0f, 0.0f) },
        Vertex { sglm::vec3(-0.5f,  0.5f, -0.5f), sglm::vec3(-1.0f,  0.0f,  0.0f), sglm::vec2(1.0f, 1.0f) },
        Vertex { sglm::vec3(-0.5f, -0.5f, -0.5f), sglm::vec3(-1.0f,  0.0f,  0.0f), sglm::vec2(0.0f, 1.0f) },
        Vertex { sglm::vec3(-0.5f, -0.5f,  0.5f), sglm::vec3(-1.0f,  0.0f,  0.0f), sglm::vec2(0.0f, 0.0f) },

        Vertex { sglm::vec3( 0.5f,  0.5f,  0.5f), sglm::vec3( 1.0f,  0.0f,  0.0f), sglm::vec2(1.0f, 0.0f) },
        Vertex { sglm::vec3( 0.5f,  0.5f, -0.5f), sglm::vec3( 1.0f,  0.0f,  0.0f), sglm::vec2(1.0f, 1.0f) },
        Vertex { sglm::vec3( 0.5f, -0.5f, -0.5f), sglm::vec3( 1.0f,  0.0f,  0.0f), sglm::vec2(0.0f, 1.0f) },
        Vertex { sglm::vec3( 0.5f, -0.5f,  0.5f), sglm::vec3( 1.0f,  0.0f,  0.0f), sglm::vec2(0.0f, 0.0f) },

        Vertex { sglm::vec3(-0.5f, -0.5f, -0.5f), sglm::vec3( 0.0f, -1.0f,  0.0f), sglm::vec2(0.0f, 1.0f) },
        Vertex { sglm::vec3( 0.5f, -0.5f, -0.5f), sglm::vec3( 0.0f, -1.0f,  0.0f), sglm::vec2(1.0f, 1.0f) },
        Vertex { sglm::vec3( 0.5f, -0.5f,  0.5f), sglm::vec3( 0.0f, -1.0f,  0.0f), sglm::vec2(1.0f, 0.0f) },
        Vertex { sglm::vec3(-0.5f, -0.5f,  0.5f), sglm::vec3( 0.0f, -1.0f,  0.0f), sglm::vec2(0.0f, 0.0f) },

        Vertex { sglm::vec3(-0.5f,  0.5f, -0.5f), sglm::vec3( 0.0f,  1.0f,  0.0f), sglm::vec2(0.0f, 1.0f) },
        Vertex { sglm::vec3( 0.5f,  0.5f, -0.5f), sglm::vec3( 0.0f,  1.0f,  0.0f), sglm::vec2(1.0f, 1.0f) },
        Vertex { sglm::vec3( 0.5f,  0.5f,  0.5f), sglm::vec3( 0.0f,  1.0f,  0.0f), sglm::vec2(1.0f, 0.0f) },
        Vertex { sglm::vec3(-0.5f,  0.5f,  0.5f), sglm::vec3( 0.0f,  1.0f,  0.0f), sglm::vec2(0.0f, 0.0f) },
    };

    std::vector<uint32_t> indices = {
        0,  2,  1,  2,  0,  3,
        4,  5,  6,  6,  7,  4,
        8,  9, 10, 10, 11,  8,
        12, 14, 13, 14, 12, 15,
        16, 17, 18, 18, 19, 16,
        20, 22, 21, 22, 20, 23,
    };

    return Create(vertices, indices, GL_TRIANGLES);
}

