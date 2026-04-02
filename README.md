# MiniRender
本人很佛，不定期更新。
PSO:
    1、类似对比PS中的图层(Layer)，比如有AO，反射，透明，阴影，背景，UI等等。
    2、每个PSO上都有Shader，而Shader上则包含了顶点和像素着色器。
    3、自定义修改PSO就可以自定义渲染管线。

RHI:
    1、渲染指令的提交线程，统一不同平台的渲染指令提交，如DX，OpenGL，Valkan等。

Fence:
    1、CPU和GPU的同步方式。

RTV:渲染画布描述(Render Taget View)
DSV:深度模版缓冲区。
指定GPU的地址，这样CPU更新以后GPU也会更新。

根签名:和寄存器做绑定。

定义资源描述:
    交换链+RTV+Light+Normal+SSAO+....

最终的画面：
    PSO1->PSO2->PSO3->......PSO:多个PSO叠加成最后的渲染画面。