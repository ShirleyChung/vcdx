#include "pch.h"
#include "DirectXBase.h" 
#include <windows.ui.xaml.media.dxinterop.h>
#include <math.h>

using namespace D2D1;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Controls;

// 建構函式。
DirectXBase::DirectXBase() :
	m_dpi(-1.0f)
{
}

// 初始化必須執行的 DirectX 資源。
void DirectXBase::Initialize(CoreWindow^ window, SwapChainBackgroundPanel^ panel, float dpi)
{
	m_window = window;
	m_panel = panel;

	CreateDeviceIndependentResources();
	CreateDeviceResources();
	SetDpi(dpi);
}

// 重新建立所有裝置資源，並設回目前狀態。
void DirectXBase::HandleDeviceLost()
{
	// 重設這些成員變數，以確保 SetDpi 會重新建立所有資源。
	float dpi = m_dpi;
	m_dpi = -1.0f;
	m_windowBounds.Width = 0;
	m_windowBounds.Height = 0;
	m_swapChain = nullptr;

	CreateDeviceResources();
	SetDpi(dpi);
}

// 這些是不論哪種裝置都需要的資源。
void DirectXBase::CreateDeviceIndependentResources()
{
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
	 // 如果專案在偵錯組建中，透過 SDK 層啟用 Direct2D 偵錯。
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

	DX::ThrowIfFailed(
		D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory1),
			&options,
			&m_d2dFactory
			)
		);

	DX::ThrowIfFailed(
		DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			&m_dwriteFactory
			)
		);

	DX::ThrowIfFailed(
		CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&m_wicFactory)
			)
		);
}

// 這些是因裝置而異的資源。
void DirectXBase::CreateDeviceResources()
{
	// 這個旗標會加入支援色彩頻道順序異於預設
	// 應用程式開發介面的表面。必須這樣才能與 Direct2D 相容。
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	ComPtr<IDXGIDevice> dxgiDevice;

#if defined(_DEBUG)
	// 如果專案在偵錯組建中，請使用這個旗標透過 SDK 層啟用偵錯。
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 這個陣列定義這個應用程式將支援的一組 DirectX 硬體功能層級。
	// 注意，應保留順序。
	// 不要忘記在描述中宣告應用程式的最小必要功能
	// 層級。除非另外指定，所有應用程式都必須支援 9.1。
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// 建立 Direct3D 11 應用程式開發介面裝置物件和對應的內容。
	ComPtr<ID3D11Device> d3dDevice;
	ComPtr<ID3D11DeviceContext> d3dContext;
	DX::ThrowIfFailed(
		D3D11CreateDevice(
			nullptr, // 指定 nullptr 以使用預設配接器。
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			creationFlags, // 設定偵錯和 Direct2D 相容性旗標。
			featureLevels, // 這個應用程式可支援的功能層級清單。
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION, // 若是 Windows 市集應用程式，這一項一定設為 D3D11_SDK_VERSION。
			&d3dDevice, // 傳回建立的 Direct3D 裝置。
			&m_d3dFeatureLevel, // 傳回建立的裝置功能層級。
			&d3dContext // 傳回裝置直接內容。
			)
		);

	// 取得 Direct3D 11.1 應用程式開發介面裝置和內容介面。
	DX::ThrowIfFailed(
		d3dDevice.As(&m_d3dDevice)
		);

	DX::ThrowIfFailed(
		d3dContext.As(&m_d3dContext)
		);

	// 取得 Direct3D 裝置的基礎 DXGI 裝置。
	DX::ThrowIfFailed(
		m_d3dDevice.As(&dxgiDevice)
		);

	// 建立 Direct2D 裝置物件和對應的內容。
	DX::ThrowIfFailed(
		m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice)
		);

	DX::ThrowIfFailed(
		m_d2dDevice->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&m_d2dContext
			)
		);
}

// 這個方法是在 LogicalDpiChanged 事件的事件處理常式中呼叫。
void DirectXBase::SetDpi(float dpi)
{
	if (dpi != m_dpi)
	{
		// 儲存更新的 DPI 值。
		m_dpi = dpi;
		
		// 更新 Direct2D 的預存 DPI。
		m_d2dContext->SetDpi(m_dpi, m_dpi);

		// DPI 變更通常表示視窗大小變更。在某些情況下，Windows 會發出
		// 大小變更事件和 DPI 變更事件。如發生這種狀況，所產生的界限 
		// 不會變更，而且視窗調整大小程式碼只會執行一次。
		UpdateForWindowSizeChange();
	}
}

// 這個方法是在 SizeChanged 事件的事件處理常式中呼叫。
void DirectXBase::UpdateForWindowSizeChange()
{
	// 只有在沒有暫止的 DPI 變更時才處理變更的視窗大小。
	if (m_dpi != DisplayProperties::LogicalDpi)
	{
		return;
	}

	if (m_window->Bounds.Width  != m_windowBounds.Width ||
		m_window->Bounds.Height != m_windowBounds.Height ||
		m_orientation != DisplayProperties::CurrentOrientation)
	{
		ID3D11RenderTargetView* nullViews[] = {nullptr};
		m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		m_d2dContext->SetTarget(nullptr);
		m_d2dTargetBitmap = nullptr;
		m_d3dRenderTargetView = nullptr;
		m_d3dDepthStencilView = nullptr;
		m_d3dContext->Flush();
		CreateWindowSizeDependentResources();
	}
}

// 配置發生 SizeChanged 視窗事件時變更的所有記憶體資源。
void DirectXBase::CreateWindowSizeDependentResources()
{
	// 存放視窗界限，使得下次收到 SizeChanged 事件時，如果大小
	// 相同，可以避免重建所有內容。
	m_windowBounds = m_window->Bounds;

	// 計算需要的交換鏈結，並以像素為單位呈現目標大小。
	float windowWidth = ConvertDipsToPixels(m_windowBounds.Width);
	float windowHeight = ConvertDipsToPixels(m_windowBounds.Height);

	// 交換鏈結的寬度和高度必須以視窗的橫向
	// 寬度和高度為基礎。如果視窗是縱向，
	// 維度將反轉。
	m_orientation = DisplayProperties::CurrentOrientation;
	bool swapDimensions =
		m_orientation == DisplayOrientations::Portrait ||
		m_orientation == DisplayOrientations::PortraitFlipped;
	m_d3dRenderTargetSize.Width = swapDimensions ? windowHeight : windowWidth;
	m_d3dRenderTargetSize.Height = swapDimensions ? windowWidth : windowHeight;

	if (m_swapChain != nullptr)
	{
		// 如果交換鏈結已經存在，即調整它的大小。
		HRESULT hr = m_swapChain->ResizeBuffers(
			2, // 雙重緩衝的交換鏈結。
			static_cast<UINT>(m_d3dRenderTargetSize.Width),
			static_cast<UINT>(m_d3dRenderTargetSize.Height),
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0
			);

		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			HandleDeviceLost();
			return;
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
	else
	{
		// 否則，使用與現有 Direct3D 裝置相同的配接器建立新的。
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
		swapChainDesc.Width = static_cast<UINT>(m_d3dRenderTargetSize.Width); // 比對視窗的大小。
		swapChainDesc.Height = static_cast<UINT>(m_d3dRenderTargetSize.Height);
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // 這是最常見的交換鏈結格式。
		swapChainDesc.Stereo = false; 
		swapChainDesc.SampleDesc.Count = 1; // 不使用多重取樣。
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2; // 使用雙重緩衝將延遲降到最低。
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // 所有 Windows 市集應用程式都必須使用這個 SwapEffect。
		swapChainDesc.Flags = 0;

		ComPtr<IDXGIDevice1> dxgiDevice;
		DX::ThrowIfFailed(
			m_d3dDevice.As(&dxgiDevice)
			);

		ComPtr<IDXGIAdapter> dxgiAdapter;
		DX::ThrowIfFailed(
			dxgiDevice->GetAdapter(&dxgiAdapter)
			);

		ComPtr<IDXGIFactory2> dxgiFactory;
		DX::ThrowIfFailed(
			dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory))
			);

		DX::ThrowIfFailed(
			dxgiFactory->CreateSwapChainForComposition(
				m_d3dDevice.Get(),
				&swapChainDesc,
				nullptr,
				&m_swapChain
				)
			);

		// 將新的交換鏈結與 SwapChainBackgroundPanel 項目產生關聯。
		ComPtr<ISwapChainBackgroundPanelNative> panelNative;
		DX::ThrowIfFailed(
			reinterpret_cast<IUnknown*>(m_panel)->QueryInterface(IID_PPV_ARGS(&panelNative))
			);
		
		DX::ThrowIfFailed(
			panelNative->SetSwapChain(m_swapChain.Get())
			);

		// 確定 DXGI 不會一次將多個框架排入佇列。這樣可同時降低延遲，並
		// 確保應用程式只會在每次 VSync 後轉譯，使耗電降到最低。
		DX::ThrowIfFailed(
			dxgiDevice->SetMaximumFrameLatency(1)
			);
	}

	// 為交換鏈結設定正確的方向，並產生 2D 和
	// 3D 矩陣轉換，以轉譯成旋轉的交換鏈結。
	// 注意，2D 和 3D 轉換的旋轉角度不同。
	// 這是座標空間不同所致。此外，
	// 3D 矩陣是明確指定，以避免進位誤差。
	DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;
	switch (m_orientation)
	{
		case DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			m_orientationTransform2D = Matrix3x2F::Identity();
			m_orientationTransform3D = XMFLOAT4X4( // 0 度 Z 旋轉
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);
			break;

		case DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			m_orientationTransform2D = 
				Matrix3x2F::Rotation(270.0f) *
				Matrix3x2F::Translation(0.0f, m_windowBounds.Width);
			m_orientationTransform3D = XMFLOAT4X4( // 90 度 Z 旋轉
				0.0f, 1.0f, 0.0f, 0.0f,
				-1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);
			break;

		case DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			m_orientationTransform2D = 
				Matrix3x2F::Rotation(180.0f) *
				Matrix3x2F::Translation(m_windowBounds.Width, m_windowBounds.Height);
			m_orientationTransform3D = XMFLOAT4X4( // 180 度 Z 旋轉
				-1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, -1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);
			break;

		case DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			m_orientationTransform2D = 
				Matrix3x2F::Rotation(90.0f) *
				Matrix3x2F::Translation(m_windowBounds.Height, 0.0f);
			m_orientationTransform3D = XMFLOAT4X4( // 270 度 Z 旋轉
				0.0f, -1.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
				);
			break;

		default:
			throw ref new Platform::FailureException();
	}

	DX::ThrowIfFailed(
		m_swapChain->SetRotation(rotation)
		);

	// 建立交換鏈結背景緩衝區的 Direct3D 轉譯目標檢視。
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
		);

	DX::ThrowIfFailed(
		m_d3dDevice->CreateRenderTargetView(
			backBuffer.Get(),
			nullptr,
			&m_d3dRenderTargetView
			)
		);

	// 建立深度樣板檢視，以視需要用於 3D 轉譯。
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT, 
		static_cast<UINT>(m_d3dRenderTargetSize.Width),
		static_cast<UINT>(m_d3dRenderTargetSize.Height),
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
		);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(
		m_d3dDevice->CreateTexture2D(
			&depthStencilDesc,
			nullptr,
			&depthStencil
			)
		);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateDepthStencilView(
			depthStencil.Get(),
			&depthStencilViewDesc,
			&m_d3dDepthStencilView
			)
		);

	// 將 3D 轉譯檢視區的目標設為整個視窗。
	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		m_d3dRenderTargetSize.Width,
		m_d3dRenderTargetSize.Height
		);

	m_d3dContext->RSSetViewports(1, &viewport);

	// 建立與交換鏈結背景緩衝區關聯的 Direct2D
	// 目標點陣圖，並將它設為目前目標。
	D2D1_BITMAP_PROPERTIES1 bitmapProperties = 
		BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
			m_dpi,
			m_dpi
			);

	ComPtr<IDXGISurface> dxgiBackBuffer;
	DX::ThrowIfFailed(
		m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateBitmapFromDxgiSurface(
			dxgiBackBuffer.Get(),
			&bitmapProperties,
			&m_d2dTargetBitmap
			)
		);

	m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());

	// 建議所有 Windows 市集應用程式都使用灰階文字消除鋸齒。
	m_d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
}

// 將最終影像傳遞到顯示器的方法。
void DirectXBase::Present()
{
	// 應用程式可選擇指定 "dirty" 或 "scroll"
	// rects，以改善某些狀況的效能。
	DXGI_PRESENT_PARAMETERS parameters = {0};
	parameters.DirtyRectsCount = 0;
	parameters.pDirtyRects = nullptr;
	parameters.pScrollRect = nullptr;
	parameters.pScrollOffset = nullptr;
	
	// 第一個引數會指示 DXGI 在 VSync 之前封鎖住，使應用程式
	// 直到下一次 VSync 前都保持睡眠狀態。這樣可確保不會浪費任何循環轉譯
	// 絕不會顯示在螢幕上的畫面。
	HRESULT hr = m_swapChain->Present1(1, 0, &parameters);

	// 捨棄轉譯目標的內容。
	// 只有在即將完全覆寫現有內容時，這才是有效的
	// 作業。如果使用 dirty 或 scroll rects，應移除這個呼叫。
	m_d3dContext->DiscardView(m_d3dRenderTargetView.Get());

	// 捨棄深度樣板的內容。
	m_d3dContext->DiscardView(m_d3dDepthStencilView.Get());

	// 如果因中斷連接或驅動程式升級而移除裝置，就 
	// 必須重新建立所有裝置資源。
	if (hr == DXGI_ERROR_DEVICE_REMOVED)
	{
		HandleDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}

// 將以裝置獨立畫素 (DIP) 為單位的長度轉換成實體像素長度的方法。
float DirectXBase::ConvertDipsToPixels(float dips)
{
	static const float dipsPerInch = 96.0f;
	return floor(dips * m_dpi / dipsPerInch + 0.5f); // 四捨五入到最近的整數。
}


void DirectXBase::ValidateDevice()
{
	ComPtr<IDXGIDevice1> dxgiDevice;
	ComPtr<IDXGIAdapter> deviceAdapter;
	DXGI_ADAPTER_DESC deviceDesc;
	DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));
	DX::ThrowIfFailed(dxgiDevice->GetAdapter(&deviceAdapter));
	DX::ThrowIfFailed(deviceAdapter->GetDesc(&deviceDesc));

	ComPtr<IDXGIFactory2> dxgiFactory;
	ComPtr<IDXGIAdapter1> currentAdapter;
	DXGI_ADAPTER_DESC currentDesc;
	DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));
	DX::ThrowIfFailed(dxgiFactory->EnumAdapters1(0, &currentAdapter));
	DX::ThrowIfFailed(currentAdapter->GetDesc(&currentDesc));

	if (deviceDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart ||
		deviceDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart ||
		FAILED(m_d3dDevice->GetDeviceRemovedReason()))
	{
		dxgiDevice = nullptr;
		deviceAdapter = nullptr;

		HandleDeviceLost();
	}
}
