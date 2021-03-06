#include "directx.h"

#include <Windows.h>
#include <hash_map>
#include <algorithm>
#include <iostream>
#include <sstream>

#include "DirectDraw.h"
#include "DirectDrawCollection.h"

#include "D3DHooks.h"
#include "D3DMenu.h"

#include "detours.h"

#pragma comment(lib, "detours")

#define COM_METHOD(TYPE, METHOD) TYPE STDMETHODCALLTYPE METHOD

CDirectDrawCollection gDDC;
D3DMenu menu;

void UpdateHack()
{
	while(true)
	{
		if(GetAsyncKeyState(VK_END) && 0x8000) menu.Toggle();
		
		menu.KeyInput();
		Sleep(100);
	}
}

int InitializeHook(void)
{
	menu.Initialize();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((PVOID*)&Direct3DCreate9Next, Direct3DCreate9Callback);

	if( DetourTransactionCommit() == NO_ERROR)
		OutputDebugString("Directx Hook Success");
	else
		OutputDebugString("Directx Hook Failed");

	return 0;
}

//------------------------------------------------------------------------
// hook interface IDirect3DDevice9
class CDirect3DDevice9Hook : public IDirect3DDevice9
{
private:
	IDirect3DDevice9* m_ptr;

public:
	CDirect3DDevice9Hook(IDirect3DDevice9* ptr) : m_ptr(ptr) {}

public:
	/*** IUnknown methods ***/
	COM_METHOD(HRESULT, QueryInterface)(THIS_ REFIID riid, void** ppvObj) { /*TODO*/ return m_ptr->QueryInterface(riid, ppvObj); }
	COM_METHOD(ULONG, AddRef)(THIS) { /*TODO*/ return m_ptr->AddRef(); }
	COM_METHOD(ULONG, Release)(THIS) 
	{
		gDDC.Remove(m_ptr);
		return m_ptr->Release(); 
	}

	/*** IDirect3DDevice9 methods ***/
	COM_METHOD(HRESULT, TestCooperativeLevel)(THIS) { /*TODO*/ return m_ptr->TestCooperativeLevel(); }
	COM_METHOD(UINT, GetAvailableTextureMem)(THIS) { /*TODO*/ return m_ptr->GetAvailableTextureMem(); }
	COM_METHOD(HRESULT, EvictManagedResources)(THIS) { /*TODO*/ return m_ptr->EvictManagedResources(); }
	COM_METHOD(HRESULT, GetDirect3D)(THIS_ IDirect3D9** ppD3D9) { /*TODO*/ return m_ptr->GetDirect3D(ppD3D9); }
	COM_METHOD(HRESULT, GetDeviceCaps)(THIS_ D3DCAPS9* pCaps) { /*TODO*/ return m_ptr->GetDeviceCaps(pCaps); }
	COM_METHOD(HRESULT, GetDisplayMode)(THIS_ UINT iSwapChain,D3DDISPLAYMODE* pMode) { /*TODO*/ return m_ptr->GetDisplayMode(iSwapChain, pMode); }
	COM_METHOD(HRESULT, GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS *pParameters) { /*TODO*/ return m_ptr->GetCreationParameters(pParameters); }
	COM_METHOD(HRESULT, SetCursorProperties)(THIS_ UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap) { /*TODO*/ return m_ptr->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap); }
	COM_METHOD(void, SetCursorPosition)(THIS_ int X,int Y,DWORD Flags) { /*TODO*/ return m_ptr->SetCursorPosition(X, Y, Flags); }
	COM_METHOD(BOOL, ShowCursor)(THIS_ BOOL bShow) { /*TODO*/ return m_ptr->ShowCursor(bShow); }
	COM_METHOD(HRESULT, CreateAdditionalSwapChain)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain) { /*TODO*/ return m_ptr->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain); }
	COM_METHOD(HRESULT, GetSwapChain)(THIS_ UINT iSwapChain,IDirect3DSwapChain9** pSwapChain) { /*TODO*/ return m_ptr->GetSwapChain(iSwapChain, pSwapChain); }
	COM_METHOD(UINT, GetNumberOfSwapChains)(THIS) { /*TODO*/ return m_ptr->GetNumberOfSwapChains(); }
	COM_METHOD(HRESULT, Reset)(THIS_ D3DPRESENT_PARAMETERS* pPresentationParameters) 
	{
		menu.Clear();
		return m_ptr->Reset(pPresentationParameters);
	}
	COM_METHOD(HRESULT, Present)(THIS_ CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion) { /*TODO*/ return m_ptr->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion); }
	COM_METHOD(HRESULT, GetBackBuffer)(THIS_ UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer) { /*TODO*/ return m_ptr->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer); }
	COM_METHOD(HRESULT, GetRasterStatus)(THIS_ UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus) { /*TODO*/ return m_ptr->GetRasterStatus(iSwapChain, pRasterStatus); }
	COM_METHOD(HRESULT, SetDialogBoxMode)(THIS_ BOOL bEnableDialogs) { /*TODO*/ return m_ptr->SetDialogBoxMode(bEnableDialogs); }
	COM_METHOD(void, SetGammaRamp)(THIS_ UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp) { /*TODO*/ return m_ptr->SetGammaRamp(iSwapChain, Flags, pRamp); }
	COM_METHOD(void, GetGammaRamp)(THIS_ UINT iSwapChain,D3DGAMMARAMP* pRamp) { /*TODO*/ return m_ptr->GetGammaRamp(iSwapChain, pRamp); }
	COM_METHOD(HRESULT, CreateTexture)(THIS_ UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) { /*TODO*/ return m_ptr->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle); }
	COM_METHOD(HRESULT, CreateVolumeTexture)(THIS_ UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle) { /*TODO*/ return m_ptr->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle); }
	COM_METHOD(HRESULT, CreateCubeTexture)(THIS_ UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle) { /*TODO*/ return m_ptr->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle); }
	COM_METHOD(HRESULT, CreateVertexBuffer)(THIS_ UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle) { /*TODO*/ return m_ptr->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle); }
	COM_METHOD(HRESULT, CreateIndexBuffer)(THIS_ UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle) { /*TODO*/ return m_ptr->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle); }
	COM_METHOD(HRESULT, CreateRenderTarget)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) { /*TODO*/ return m_ptr->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle); }
	COM_METHOD(HRESULT, CreateDepthStencilSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) { /*TODO*/ return m_ptr->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle); }
	COM_METHOD(HRESULT, UpdateSurface)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint) { /*TODO*/ return m_ptr->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint); }
	COM_METHOD(HRESULT, UpdateTexture)(THIS_ IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture) { /*TODO*/ return m_ptr->UpdateTexture(pSourceTexture, pDestinationTexture); }
	COM_METHOD(HRESULT, GetRenderTargetData)(THIS_ IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface) { /*TODO*/ return m_ptr->GetRenderTargetData(pRenderTarget, pDestSurface); }
	COM_METHOD(HRESULT, GetFrontBufferData)(THIS_ UINT iSwapChain,IDirect3DSurface9* pDestSurface) { /*TODO*/ return m_ptr->GetFrontBufferData(iSwapChain, pDestSurface); }
	COM_METHOD(HRESULT, StretchRect)(THIS_ IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter) { /*TODO*/ return m_ptr->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter); }
	COM_METHOD(HRESULT, ColorFill)(THIS_ IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color) { /*TODO*/ return m_ptr->ColorFill(pSurface, pRect, color); }
	COM_METHOD(HRESULT, CreateOffscreenPlainSurface)(THIS_ UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) { /*TODO*/ return m_ptr->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle); }
	COM_METHOD(HRESULT, SetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget) { /*TODO*/ std::cout << "Render Target: " << RenderTargetIndex << std::endl; return m_ptr->SetRenderTarget(RenderTargetIndex, pRenderTarget); }
	COM_METHOD(HRESULT, GetRenderTarget)(THIS_ DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget) { /*TODO*/ return m_ptr->GetRenderTarget(RenderTargetIndex, ppRenderTarget); }
	COM_METHOD(HRESULT, SetDepthStencilSurface)(THIS_ IDirect3DSurface9* pNewZStencil) { /*TODO*/ return m_ptr->SetDepthStencilSurface(pNewZStencil); }
	COM_METHOD(HRESULT, GetDepthStencilSurface)(THIS_ IDirect3DSurface9** ppZStencilSurface) { /*TODO*/ return m_ptr->GetDepthStencilSurface(ppZStencilSurface); }
	COM_METHOD(HRESULT, BeginScene)(THIS) 
	{
		menu.CreateFont(m_ptr, "Consolas");
		menu.CreateSprite(m_ptr);
		//GenerateTexture(m_ptr, D3DCOLOR_ARGB(255,255,0,0));
		return m_ptr->BeginScene();
	}
	COM_METHOD(HRESULT, EndScene)(THIS) 
	{
		menu.DrawMenu(m_ptr);
		return m_ptr->EndScene();
	}
	COM_METHOD(HRESULT, Clear)(THIS_ DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil) { /*TODO*/ return m_ptr->Clear(Count, pRects, Flags, Color, Z, Stencil); }
	COM_METHOD(HRESULT, SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix) { /*TODO*/ return m_ptr->SetTransform(State, pMatrix); }
	COM_METHOD(HRESULT, GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix) { /*TODO*/ return m_ptr->GetTransform(State, pMatrix); }
	COM_METHOD(HRESULT, MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE Type,CONST D3DMATRIX* pD3DMatrix) { /*TODO*/ return m_ptr->MultiplyTransform(Type, pD3DMatrix); }
	COM_METHOD(HRESULT, SetViewport)(THIS_ CONST D3DVIEWPORT9* pViewport) { /*TODO*/ return m_ptr->SetViewport(pViewport); }
	COM_METHOD(HRESULT, GetViewport)(THIS_ D3DVIEWPORT9* pViewport) { /*TODO*/ return m_ptr->GetViewport(pViewport); }
	COM_METHOD(HRESULT, SetMaterial)(THIS_ CONST D3DMATERIAL9* pMaterial) { /*TODO*/ return m_ptr->SetMaterial(pMaterial); }
	COM_METHOD(HRESULT, GetMaterial)(THIS_ D3DMATERIAL9* pMaterial) { /*TODO*/ return m_ptr->GetMaterial(pMaterial); }
	COM_METHOD(HRESULT, SetLight)(THIS_ DWORD Index,CONST D3DLIGHT9* pD3DLight9) { /*TODO*/ return m_ptr->SetLight(Index, pD3DLight9); }
	COM_METHOD(HRESULT, GetLight)(THIS_ DWORD Index,D3DLIGHT9* pD3DLight9) { /*TODO*/ return m_ptr->GetLight(Index, pD3DLight9); }
	COM_METHOD(HRESULT, LightEnable)(THIS_ DWORD Index,BOOL Enable) { /*TODO*/ return m_ptr->LightEnable(Index, Enable); }
	COM_METHOD(HRESULT, GetLightEnable)(THIS_ DWORD Index,BOOL* pEnable) { /*TODO*/ return m_ptr->GetLightEnable(Index, pEnable); }
	COM_METHOD(HRESULT, SetClipPlane)(THIS_ DWORD Index,CONST float* pPlane) { /*TODO*/ return m_ptr->SetClipPlane(Index, pPlane); }
	COM_METHOD(HRESULT, GetClipPlane)(THIS_ DWORD Index,float* pPlane) { /*TODO*/ return m_ptr->GetClipPlane(Index, pPlane); }
	COM_METHOD(HRESULT, SetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD Value) { /*TODO*/ return m_ptr->SetRenderState(State, Value); }
	COM_METHOD(HRESULT, GetRenderState)(THIS_ D3DRENDERSTATETYPE State,DWORD* pValue) { /*TODO*/ return m_ptr->GetRenderState(State, pValue); }
	COM_METHOD(HRESULT, CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB) { /*TODO*/ return m_ptr->CreateStateBlock(Type, ppSB); }
	COM_METHOD(HRESULT, BeginStateBlock)(THIS) { /*TODO*/ return m_ptr->BeginStateBlock(); }
	COM_METHOD(HRESULT, EndStateBlock)(THIS_ IDirect3DStateBlock9** ppSB) { /*TODO*/ return m_ptr->EndStateBlock(ppSB); }
	COM_METHOD(HRESULT, SetClipStatus)(THIS_ CONST D3DCLIPSTATUS9* pClipStatus) { /*TODO*/ return m_ptr->SetClipStatus(pClipStatus); }
	COM_METHOD(HRESULT, GetClipStatus)(THIS_ D3DCLIPSTATUS9* pClipStatus) { /*TODO*/ return m_ptr->GetClipStatus(pClipStatus); }
	COM_METHOD(HRESULT, GetTexture)(THIS_ DWORD Stage,IDirect3DBaseTexture9** ppTexture) { /*TODO*/ return m_ptr->GetTexture(Stage, ppTexture); }
	COM_METHOD(HRESULT, SetTexture)(THIS_ DWORD Stage,IDirect3DBaseTexture9* pTexture) { /*TODO*/ return m_ptr->SetTexture(Stage, pTexture); }
	COM_METHOD(HRESULT, GetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue) { /*TODO*/ return m_ptr->GetTextureStageState(Stage, Type, pValue); }
	COM_METHOD(HRESULT, SetTextureStageState)(THIS_ DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value) { /*TODO*/ return m_ptr->SetTextureStageState(Stage, Type, Value); }
	COM_METHOD(HRESULT, GetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue) { /*TODO*/ return m_ptr->GetSamplerState(Sampler, Type, pValue); }
	COM_METHOD(HRESULT, SetSamplerState)(THIS_ DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value) { /*TODO*/ return m_ptr->SetSamplerState(Sampler, Type, Value); }
	COM_METHOD(HRESULT, ValidateDevice)(THIS_ DWORD* pNumPasses) { /*TODO*/ return m_ptr->ValidateDevice(pNumPasses); }
	COM_METHOD(HRESULT, SetPaletteEntries)(THIS_ UINT PaletteNumber,CONST PALETTEENTRY* pEntries) { /*TODO*/ return m_ptr->SetPaletteEntries(PaletteNumber, pEntries); }
	COM_METHOD(HRESULT, GetPaletteEntries)(THIS_ UINT PaletteNumber,PALETTEENTRY* pEntries) { /*TODO*/ return m_ptr->GetPaletteEntries(PaletteNumber, pEntries); }
	COM_METHOD(HRESULT, SetCurrentTexturePalette)(THIS_ UINT PaletteNumber) { /*TODO*/ return m_ptr->SetCurrentTexturePalette(PaletteNumber); }
	COM_METHOD(HRESULT, GetCurrentTexturePalette)(THIS_ UINT *PaletteNumber) { /*TODO*/ return m_ptr->GetCurrentTexturePalette(PaletteNumber); }
	COM_METHOD(HRESULT, SetScissorRect)(THIS_ CONST RECT* pRect) { /*TODO*/ return m_ptr->SetScissorRect(pRect); }
	COM_METHOD(HRESULT, GetScissorRect)(THIS_ RECT* pRect) { /*TODO*/ return m_ptr->GetScissorRect(pRect); }
	COM_METHOD(HRESULT, SetSoftwareVertexProcessing)(THIS_ BOOL bSoftware) { /*TODO*/ return m_ptr->SetSoftwareVertexProcessing(bSoftware); }
	COM_METHOD(BOOL, GetSoftwareVertexProcessing)(THIS) { /*TODO*/ return m_ptr->GetSoftwareVertexProcessing(); }
	COM_METHOD(HRESULT, SetNPatchMode)(THIS_ float nSegments) { /*TODO*/ return m_ptr->SetNPatchMode(nSegments); }
	COM_METHOD(float, GetNPatchMode)(THIS) { /*TODO*/ return m_ptr->GetNPatchMode(); }
	COM_METHOD(HRESULT, DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount) { /*TODO*/ return m_ptr->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount); }
	COM_METHOD(HRESULT, DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount) 
	{
		/*TODO*/ 
		Wallhack(m_ptr, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		HRESULT ret = m_ptr->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		return ret;
	}
	COM_METHOD(HRESULT, DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) { /*TODO*/ return m_ptr->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride); }
	COM_METHOD(HRESULT, DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride) { /*TODO*/ return m_ptr->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride); }
	COM_METHOD(HRESULT, ProcessVertices)(THIS_ UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags) { /*TODO*/ return m_ptr->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags); }
	COM_METHOD(HRESULT, CreateVertexDeclaration)(THIS_ CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl) { /*TODO*/ return m_ptr->CreateVertexDeclaration(pVertexElements, ppDecl); }
	COM_METHOD(HRESULT, SetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9* pDecl) { /*TODO*/ return m_ptr->SetVertexDeclaration(pDecl); }
	COM_METHOD(HRESULT, GetVertexDeclaration)(THIS_ IDirect3DVertexDeclaration9** ppDecl) { /*TODO*/ return m_ptr->GetVertexDeclaration(ppDecl); }
	COM_METHOD(HRESULT, SetFVF)(THIS_ DWORD FVF) { /*TODO*/ return m_ptr->SetFVF(FVF); }
	COM_METHOD(HRESULT, GetFVF)(THIS_ DWORD* pFVF) { /*TODO*/ return m_ptr->GetFVF(pFVF); }
	COM_METHOD(HRESULT, CreateVertexShader)(THIS_ CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader) { /*TODO*/ return m_ptr->CreateVertexShader(pFunction, ppShader); }
	COM_METHOD(HRESULT, SetVertexShader)(THIS_ IDirect3DVertexShader9* pShader) { /*TODO*/ return m_ptr->SetVertexShader(pShader); }
	COM_METHOD(HRESULT, GetVertexShader)(THIS_ IDirect3DVertexShader9** ppShader) { /*TODO*/ return m_ptr->GetVertexShader(ppShader); }
	COM_METHOD(HRESULT, SetVertexShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) { /*TODO*/ return m_ptr->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
	COM_METHOD(HRESULT, GetVertexShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount) { /*TODO*/ return m_ptr->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
	COM_METHOD(HRESULT, SetVertexShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) { /*TODO*/ return m_ptr->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
	COM_METHOD(HRESULT, GetVertexShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount) { /*TODO*/ return m_ptr->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
	COM_METHOD(HRESULT, SetVertexShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) { /*TODO*/ return m_ptr->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount); }
	COM_METHOD(HRESULT, GetVertexShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount) { /*TODO*/ return m_ptr->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount); }
	COM_METHOD(HRESULT, SetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride) { /*TODO*/ return m_ptr->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride); }
	COM_METHOD(HRESULT, GetStreamSource)(THIS_ UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride) { /*TODO*/ return m_ptr->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride); }
	COM_METHOD(HRESULT, SetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT Setting) { /*TODO*/ return m_ptr->SetStreamSourceFreq(StreamNumber, Setting); }
	COM_METHOD(HRESULT, GetStreamSourceFreq)(THIS_ UINT StreamNumber,UINT* pSetting) { /*TODO*/ return m_ptr->GetStreamSourceFreq(StreamNumber, pSetting); }
	COM_METHOD(HRESULT, SetIndices)(THIS_ IDirect3DIndexBuffer9* pIndexData) { /*TODO*/ return m_ptr->SetIndices(pIndexData); }
	COM_METHOD(HRESULT, GetIndices)(THIS_ IDirect3DIndexBuffer9** ppIndexData) { /*TODO*/ return m_ptr->GetIndices(ppIndexData); }
	COM_METHOD(HRESULT, CreatePixelShader)(THIS_ CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader) { /*TODO*/ return m_ptr->CreatePixelShader(pFunction, ppShader); }
	COM_METHOD(HRESULT, SetPixelShader)(THIS_ IDirect3DPixelShader9* pShader) { /*TODO*/ return m_ptr->SetPixelShader(pShader); }
	COM_METHOD(HRESULT, GetPixelShader)(THIS_ IDirect3DPixelShader9** ppShader) { /*TODO*/ return m_ptr->GetPixelShader(ppShader); }
	COM_METHOD(HRESULT, SetPixelShaderConstantF)(THIS_ UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) { /*TODO*/ return m_ptr->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
	COM_METHOD(HRESULT, GetPixelShaderConstantF)(THIS_ UINT StartRegister,float* pConstantData,UINT Vector4fCount) { /*TODO*/ return m_ptr->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
	COM_METHOD(HRESULT, SetPixelShaderConstantI)(THIS_ UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) { /*TODO*/ return m_ptr->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
	COM_METHOD(HRESULT, GetPixelShaderConstantI)(THIS_ UINT StartRegister,int* pConstantData,UINT Vector4iCount) { /*TODO*/ return m_ptr->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
	COM_METHOD(HRESULT, SetPixelShaderConstantB)(THIS_ UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) { /*TODO*/ return m_ptr->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount); }
	COM_METHOD(HRESULT, GetPixelShaderConstantB)(THIS_ UINT StartRegister,BOOL* pConstantData,UINT BoolCount) { /*TODO*/ return m_ptr->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount); }
	COM_METHOD(HRESULT, DrawRectPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo) { /*TODO*/ return m_ptr->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo); }
	COM_METHOD(HRESULT, DrawTriPatch)(THIS_ UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo) { /*TODO*/ return m_ptr->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo); }
	COM_METHOD(HRESULT, DeletePatch)(THIS_ UINT Handle) { /*TODO*/ return m_ptr->DeletePatch(Handle); }
	COM_METHOD(HRESULT, CreateQuery)(THIS_ D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery) { /*TODO*/ return m_ptr->CreateQuery(Type, ppQuery); }


};
//------------------------------------------------------------------------



//------------------------------------------------------------------------
// hook interface IDirect3D9
class CDirect3D9Hook : public IDirect3D9
{
private:
	IDirect3D9* m_ptr;

public:
	CDirect3D9Hook(IDirect3D9* ptr) : m_ptr(ptr) {}

public:
	/*** IUnknown methods ***/
	COM_METHOD(HRESULT, QueryInterface)(THIS_ REFIID riid, void** ppvObj) { /*TODO*/ return m_ptr->QueryInterface(riid, ppvObj); }
	COM_METHOD(ULONG, AddRef)(THIS) { /*TODO*/ return m_ptr->AddRef(); }
	COM_METHOD(ULONG, Release)(THIS) { /*TODO*/ return m_ptr->Release(); }

	/*** IDirect3D9 methods ***/
	COM_METHOD(HRESULT, RegisterSoftwareDevice)(THIS_ void* pInitializeFunction) { /*TODO*/ return m_ptr->RegisterSoftwareDevice(pInitializeFunction); }
	COM_METHOD(UINT, GetAdapterCount)(THIS) { /*TODO*/ return m_ptr->GetAdapterCount(); }
	COM_METHOD(HRESULT, GetAdapterIdentifier)(THIS_ UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier) { /*TODO*/ return m_ptr->GetAdapterIdentifier(Adapter, Flags, pIdentifier); }
	COM_METHOD(UINT, GetAdapterModeCount)(THIS_ UINT Adapter,D3DFORMAT Format) { /*TODO*/ return m_ptr->GetAdapterModeCount(Adapter, Format); }
	COM_METHOD(HRESULT, EnumAdapterModes)(THIS_ UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode) { /*TODO*/ return m_ptr->EnumAdapterModes(Adapter, Format, Mode, pMode); }
	COM_METHOD(HRESULT, GetAdapterDisplayMode)(THIS_ UINT Adapter,D3DDISPLAYMODE* pMode) { /*TODO*/ return m_ptr->GetAdapterDisplayMode(Adapter, pMode); }
	COM_METHOD(HRESULT, CheckDeviceType)(THIS_ UINT Adapter,D3DDEVTYPE DevType,D3DFORMAT AdapterFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed) { /*TODO*/ return m_ptr->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed); }
	COM_METHOD(HRESULT, CheckDeviceFormat)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat) { /*TODO*/ return m_ptr->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat); }
	COM_METHOD(HRESULT, CheckDeviceMultiSampleType)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels) { /*TODO*/ return m_ptr->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels); }
	COM_METHOD(HRESULT, CheckDepthStencilMatch)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat) { /*TODO*/ return m_ptr->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat); }
	COM_METHOD(HRESULT, CheckDeviceFormatConversion)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat) { /*TODO*/ return m_ptr->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat); }
	COM_METHOD(HRESULT, GetDeviceCaps)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps) { /*TODO*/ return m_ptr->GetDeviceCaps(Adapter, DeviceType, pCaps); }
	COM_METHOD(HMONITOR, GetAdapterMonitor)(THIS_ UINT Adapter) { /*TODO*/ return m_ptr->GetAdapterMonitor(Adapter); }
	COM_METHOD(HRESULT, CreateDevice)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface) 
	{
		HRESULT hr = m_ptr->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface); 

		if(SUCCEEDED(hr))
		{
			gDDC.Add(*ppReturnedDeviceInterface, new CDirectDraw(*ppReturnedDeviceInterface));

			*ppReturnedDeviceInterface = static_cast<IDirect3DDevice9*>(new CDirect3DDevice9Hook(*ppReturnedDeviceInterface));
		}

		return hr;
	}

};

static IDirect3D9* (WINAPI* Direct3DCreate9Next)(UINT SDKVersion)
		= Direct3DCreate9;

IDirect3D9* WINAPI Direct3DCreate9Callback(UINT SDKVersion)
{
	IDirect3D9* pv = Direct3DCreate9Next(SDKVersion);

	if(pv)
	{
		pv = static_cast<IDirect3D9*>(new CDirect3D9Hook(pv));
	}

	return pv;
}