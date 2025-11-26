#version 330 core
out vec4 FragColor;
uniform sampler2D ourTexture;
in vec3 outColor;
in vec2 outUV;
void main() {
    FragColor = vec4(outUV,0.0,1.0);
    FragColor = texture(ourTexture, outUV);
}