#pragma once

#include "../simple_library/public/simple_library.h"
#include "Debug/Log/SimpleLog.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include "Rendering/DirectX12/d3dx12.h"
#include "Rendering/DirectX12/DDSTextureLoader.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <vector>

using namespace Microsoft::WRL;
using namespace std;
