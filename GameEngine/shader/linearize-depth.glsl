uniform float near = 0.1;
uniform float far = 100.0;

/*
see 1. http://web.archive.org/web/20130426093607/http://www.songho.ca/opengl/gl_projectionmatrix.html
see 2. http://web.archive.org/web/20130416194336/http://olivers.posterous.com/linear-depth-in-glsl-for-real

�˾ƾ� �ϴ� ���� ����.

1. A,B�� perspective Matrix�� 3,4��° ���� �ִ� ���̴�.

[
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, A, B,
	0, 0, -1, 0
]; (������ 0,�� 1�� �ƴϴ�. see1. ��ȣ���� ��α׿��� 3rd row of GL_PROJECTION matrix like this. �Ʒ� �ִ� ��Ʈ������ Ȯ���϶�.)

2. OpenGL������ z���� perspectiveMatrix�� ��� ��Ų �� w ������Ʈ�� ������. ( w = -z )

�̸� ǥ���� ������ zn = ze / we
(���⼭ zn�� NDC�� �ִ� z, ze�� eye space�� �ִ� w�̴�.)

��Ŀ��� ������ �����ϸ� zn = (A * ze + B*we) / - ze�̴�.

A�� ��� - (f+n) / (f-n) �̰�, B�� ��� - (2fn) / (f-n) �̴�.

------------------------------------------------------------------------------------------------------
1. ���� z���� NDC�� �����ؾ��Ѵ�. (see below) �����ϸ� �� ���� zn�̴�.
2. ze�� ���ϴ� ������ �������� ��������.. (see below)

ze = (B*we)/(zn - A) ( ���⼭ eye space���� w������Ʈ ���� 1�̱⿡ ���� �� ����)
ze = B / (zn-A)

������ �����ϸ�

(2.0 * near * far) / (fajk
r + near - z * (far - near)); �̷��Ѱ��� ���´�.


*/
float LinearizeDepth(float depth){
	float z = depth * 2.0 - 1.0; // Back to NDC
	return (2.0 * near * far) / (far + near - z * (far - near));	
}
