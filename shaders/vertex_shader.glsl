#version 330 core

layout (location = 0) in vec3 aPos;   // Vertex position
layout (location = 1) in vec2 aTexCoord; // Texture coordinates

out vec2 TexCoord; // Pass to fragment shader

// Uniforms for transformation
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Final position = projection * view * model * vertex position
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
