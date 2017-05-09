#version 130
in vec4 vPosition;
in vec3 vColor;
out vec3 outColor;
void
main()
{
    gl_Position = vec4(vPosition.x/450.0,vPosition.y/300.0,0,1);
    outColor = vColor;
}
