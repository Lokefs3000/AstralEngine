#version 450

layout(location = 0) in vec4 v_Vertex;
layout(location = 1) in vec4 v_Color;

layout(location = 0) out vec2 f_TexCoord;
layout(location = 1) out vec4 f_Color;

void main() {
    gl_Position = vec4(v_Vertex.xy, 0.0, 1.0);
    
    f_TexCoord = v_Vertex.zw;
    f_Color = v_Color;
}