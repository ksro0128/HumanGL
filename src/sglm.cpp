#include "sglm.h"

namespace sglm {


float dot(const vec2& v1, const vec2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

float dot(const vec3& v1, const vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float dot(const vec4& v1, const vec4& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

vec3 cross(const vec3& v1, const vec3& v2) {
    return vec3(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

mat4 translate(const mat4& m, const vec3& v) {
    mat4 translateMat(1.0f);
    translateMat.m[3][0] = v.x;
    translateMat.m[3][1] = v.y;
    translateMat.m[3][2] = v.z;

    return m * translateMat;
}

mat4 perspective(float fovy, float aspect, float near, float far) {
    float tanHalfFovy = tan(fovy / 2.0f);

    mat4 result(0.0f);
    result.m[0][0] = 1.0f / (aspect * tanHalfFovy);
    result.m[1][1] = 1.0f / tanHalfFovy;
    result.m[2][2] = -(far + near) / (far - near);
    result.m[2][3] = -1.0f;
    result.m[3][2] = -(2.0f * far * near) / (far - near);

    return result;
}

mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) {
    vec3 f = (center - eye).normalize();
    vec3 s = cross(f, up).normalize();  
    vec3 u = cross(s, f);                 

    mat4 result(1.0f);
    result.m[0][0] = s.x;
    result.m[0][1] = u.x;
    result.m[0][2] = -f.x;

    result.m[1][0] = s.y;
    result.m[1][1] = u.y;
    result.m[1][2] = -f.y;

    result.m[2][0] = s.z;
    result.m[2][1] = u.z;
    result.m[2][2] = -f.z;

    result.m[3][0] = -dot(s, eye);
    result.m[3][1] = -dot(u, eye);
    result.m[3][2] = dot(f, eye);

    return result;
}


mat4 rotate(const mat4& m, float angle, const vec3& axis) {
    float c = cos(angle);
    float s = sin(angle);
    vec3 normAxis = axis.normalize();

    mat4 rotate(1.0f);
    rotate.m[0][0] = c + (1 - c) * normAxis.x * normAxis.x;
    rotate.m[0][1] = (1 - c) * normAxis.x * normAxis.y + s * normAxis.z;
    rotate.m[0][2] = (1 - c) * normAxis.x * normAxis.z - s * normAxis.y;

    rotate.m[1][0] = (1 - c) * normAxis.x * normAxis.y - s * normAxis.z;
    rotate.m[1][1] = c + (1 - c) * normAxis.y * normAxis.y;
    rotate.m[1][2] = (1 - c) * normAxis.y * normAxis.z + s * normAxis.x;

    rotate.m[2][0] = (1 - c) * normAxis.x * normAxis.z + s * normAxis.y;
    rotate.m[2][1] = (1 - c) * normAxis.y * normAxis.z - s * normAxis.x;
    rotate.m[2][2] = c + (1 - c) * normAxis.z * normAxis.z;

    return m * rotate;  // 열 우선 방식에서는 이 순서가 맞음
}

sglm::mat4 scale(const sglm::mat4& m, const sglm::vec3& v) {
    sglm::mat4 result(1.0f);
    result[0][0] = v.x;
    result[1][1] = v.y;
    result[2][2] = v.z;
    return result * m;
}

float* value_ptr(vec2& v) {
    return &v.x;
}

float* value_ptr(vec3& v) {
    return &v.x;
}

float* value_ptr(vec4& v) {
    return &v.x;
}

float* value_ptr(mat4& m) {
    return &m.m[0][0];
}

float* value_ptr(const vec2& v) {
    return const_cast<float*>(&v.x);
}

float* value_ptr(const vec3& v) {
    return const_cast<float*>(&v.x);
}

float* value_ptr(const vec4& v) {
    return const_cast<float*>(&v.x);
}

float* value_ptr(const mat4& m) {
    return const_cast<float*>(&m.m[0][0]);
}

vec2 normalize(const vec2& v) {
    float len = v.length();
    if (len == 0) return v;
    return vec2(v.x / len, v.y / len);
}

vec3 normalize(const vec3& v) {
    float len = v.length();
    if (len == 0) return v;
    return vec3(v.x / len, v.y / len, v.z / len);
}

vec4 normalize(const vec4& v) {
    float len = v.length();
    if (len == 0) return v;
    return vec4(v.x / len, v.y / len, v.z / len, v.w / len);
}

float radians(float degrees) {
    return degrees * 3.14159265358979323846f / 180.0f;
}

}