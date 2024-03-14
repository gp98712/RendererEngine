#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH 1280
#endif

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 960
#endif

/*
* 0: ť����� ��ī�̹ڽ� ����
* 1: ť����� ���� �� ����
* 2: ť����� �̸���� �� ����.
* 3: brdf��� �ؽ�ó
* 
* 4: directional light �׸��ڸ� ����
* 5: point light �׸��� ť��� ����.
* 6: point light �׸��� ť��� ����.
* 7: spot light �׸��ڸ� ����.
* 8: spot light �׸��ڸ� ����.
* 
* ���߿� �׸��ڸ� ��Ʋ�󽺷� �����Ҽ��� ����.
* 
* ���� ����� 
*/
#ifndef TEXTURE_SKYBOX_OFFSET
#define TEXTURE_SKYBOX_OFFSET 4
#endif


#ifndef TEXTURE_START
#define TEXTURE_START 9
#endif

// ����!!! OPENGL ES <- �ؽ�ó 8���ۿ� ��� ����, OpenGL 3.x �̻� �������� �ּ� 16���� ������ �ؽ�ó�� �����Ѵٰ���.
// �� ��ǻ�Ϳ��� ��ȸ������� 32������ �����Ѵٰ� ����.
// �ؽ�ó �ø��°� ��������!

#ifndef BIAS
#define BIAS 0.005f
#endif