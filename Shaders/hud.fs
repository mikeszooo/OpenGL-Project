// hud.fs
#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D gunTex;

void main()
{
    vec4 col = texture(gunTex, TexCoord);
    // odrzucamy w pełni przezroczyste fragmenty (opcjonalnie)
    if(col.a < 0.1) discard;
    FragColor = col;
}
