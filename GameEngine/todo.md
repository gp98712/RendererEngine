1. Deferred Shading (1��)
https://learnopengl.com/Advanced-Lighting/Deferred-Shading

2. FXAA (2��) (Deferred Shading���� ��Ű��ó�� �ٲٸ� MSAA ����� �Ұ�������. �׷��� FXAA�� ��Ƽ�����¡�ؾ���.)
https://blog.simonrodriguez.fr/articles/2016/07/implementing_fxaa.html

3. SSAO (2��)
https://learnopengl.com/Advanced-Lighting/SSAO

4. Lens Flare (1��)
https://www.youtube.com/watch?v=OiMRdkhvwqg

5. depth of field (�ɷ���?)
 https://developer.nvidia.com/gpugems/gpugems/part-iv-image-processing/chapter-23-depth-field-survey-techniques

6. HDRI with auto exposure (����)
https://bruop.github.io/exposure/
-----------------------------
Deferred Shading�� ���ؼ�.

G-Buffer ������

0�� �ؽ�ó 
	rgb: position
    a: metallic (0~1)
1�� �ؽ�ó
    rgb: albedo
    a: roughness
2�� �ؽ�ó
    rgb: normal
    a: �Ҵ���� ���� 0.0
3�� �ؽ�ó
    rgb: emissive
    a: �Ҵ���� ���� 0.0
4�� �ؽ�ó
    rgba: �����̸� ���ؼ� ���
5�� �ؽ�ó
    rgba: SSAO�� ���ؼ� ���

