#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 overlayColor; // New overlay color

void main() {
    vec4 textureColor = texture(ourTexture, TexCoord);
    
    // Mix texture color with overlay color
    FragColor = mix(textureColor, overlayColor, overlayColor.a);
}