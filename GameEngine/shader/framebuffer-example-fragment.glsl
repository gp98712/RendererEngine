#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float exposure;

void main()
{ 
    vec3 color = texture(screenTexture, TexCoords).rgb;

    // Reinhard tone mapping
    vec3 mapped = vec3(1.0) - exp(-color * exposure);


    /*
    glEnable(GL_FRAMEBUFFER_SRGB);�� ����ϰ� �־ ������ ó���� �ϰ����� ����.
    ���� �������� ó���ϰ�ʹٸ�

    ����

    const flaot gamma = 2.2;

    ...

    mapped = pow(mapped, vec3(1.0 / gamma));

    ���� ������ 1/2.2 ó���� �������.
    */
    FragColor = vec4(mapped, 1.0);
}