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

DX12初始化流程:
DXGIFactory
CommandAllocator:命令存储队列
GraphicsCommandList:创建命令
    ResourceBarrier
    RSSetViewports
    RSSetScissorRects
    ClearRenderTargetView
    ClearDepthStencilView
    OMSetRenderTargets
    Close
CommandQueue
    ExecuteCommandLists:执行命令队列
    Signal:信号量同步，比如CPU执行一个操作，信号是1传给GPU，GPU操作完成以后，将信号改成2回传给CPU，这样CPU继续执行，否则进入等待。但是这种方法并不高效。
D3dDevice:图形驱动，用来创建上述命令
    CreateCommandList
    CreateCommandQueue
    CreateCommandAllocators
    CheckFeatureSupport
    CreateFence
    CreateDepthStencilView
    CreateCommittedResource
Fence
    GPU和CPU的同步问题
交换链:主要前后交换链，避免画面撕裂。本质交换两个指针。当然还有三层交换链...
资源描述符:主要设置
    CBV:常量缓冲区视图
    SRV:着色器资源视图
    UAV:无序访问视图 
    RTV:渲染目标视图资源
    DSV:深度模版视图资源
    SAMPLER:采样器视图
WaitGPUCommandQueueComplete:结合Fence来做CPU和GPU之间的同步
SwapChainBuffer:交换链的Buff，前台和后台缓冲区不断交换来渲染
ResourceDesc:资源的描述(宽高，层级，格式等等)
深度缓冲区:范围0~1，从近到远，其实就是深度Buff
多重采样:MSAA，抗锯齿，修复走样，SSAA(超采样)，