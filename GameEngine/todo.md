1. 구현해야할것

[] Materials xml에 정보 저장하여 사용하도록 수정
[] Volumatric Light 추가 (Screen-Space God rays는 제거)
[] Killer Feature PCSS 그림자

PCSS 대안 
https://developer.nvidia.com/gpugems/gpugems3/part-ii-light-and-shadows/chapter-8-summed-area-variance-shadow-maps

PCSS는 Area Light 개념을 차용한것인데 이건 뭔가 다르다!?!?

[] Killer Feature 스켈레톤 애니메이션
[] Killer Feature 자연 애니메이션 (잔디)
[] Killer Feature 자연 애니메이션 (나무)
[] Killer Feature VCT
http://simonstechblog.blogspot.com/2013/01/implementing-voxel-cone-tracing.html
https://andrew-pham.blog/2019/07/29/voxel-cone-tracing/
https://jose-villegas.github.io/post/deferred_voxel_shading/


VCT 대안

LPV
https://www.slideshare.net/slideshow/light-propagation-volumepdf/265376977
https://ericpolman.com/author/ericpolman/

GI using Progressive Refinement Radiosity
https://developer.nvidia.com/gpugems/gpugems2/part-v-image-oriented-computing/chapter-39-global-illumination-using-progressive

High Quality Global Illumination Rendering Using Rasterization
https://developer.nvidia.com/gpugems/gpugems2/part-v-image-oriented-computing/chapter-38-high-quality-global-illumination

----------------
GPU Gems

Sufels GI GPU Gems 2
https://developer.nvidia.com/gpugems/GPUGems2/gpugems2_chapter14.html

Motion Blur GPU Gems 3
https://developer.nvidia.com/gpugems/gpugems3/part-iv-image-effects/chapter-27-motion-blur-post-processing-effect

Depth of Field GPU Gems 3
https://developer.nvidia.com/gpugems/gpugems3/part-iv-image-effects/chapter-28-practical-post-process-depth-field

---

해결해야할 이슈 들

2. 그림자가 이상함 많이 고장났음

Directional Light도 그냥 Perspective Projection으로 계산해도 될것같은데...


3. PCSS 적용전 PCF 적용시 효과가 미미한점 이것이 정상인지 검증

4. PCSS 적용


