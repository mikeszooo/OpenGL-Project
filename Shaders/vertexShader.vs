#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aColor;

out vec3 ourColor;
out vec4 pos;
out vec2 TexCoord;
uniform float offset;
uniform mat4 model;
 uniform mat4 view;
 uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(aPos.x,aPos.y,aPos.z, 1.0);
    pos = gl_Position;
    ourColor = aColor;
    TexCoord = aTexCoord;
}