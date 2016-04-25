#pragma once

#include "DirectXHelper.h"

// 初始化 2D 和 3D 轉譯之 DirectX 應用程式開發介面的協助程式類別。
// 如果只使用 2D 或只使用 3D 轉譯，可略過這個類別中的一些程式碼。
ref class DirectXBase abstract
{
internal:
	DirectXBase();

public:
	virtual void Initialize(Windows::UI::Core::CoreWindow^ window, Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^ panel, float dpi);
	virtual void HandleDeviceLost();
	virtual void CreateDeviceIndependentResources();
	virtual void CreateDeviceResources();
	virtual void SetDpi(float dpi);
	virtual void CreateWindowSizeDependentResources();
	virtual void UpdateForWindowSizeChange();
	virtual void Render() = 0;
	virtual void Present();
	virtual float ConvertDipsToPixels(float dips);
	void ValidateDevice();


protected private:
	Platform::Agile<Windows::UI::Core::CoreWindow> m_window;
	Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^ m_panel;

	// DirectWrite 和 Windows 影像處理元件物件。
	Microsoft::WRL::ComPtr<IDWriteFactory1> m_dwriteFactory;
	Microsoft::WRL::ComPtr<IWICImagingFactory2> m_wicFactory;

	// DirectX 核心物件。2D 和 3D 的必要項。
	Microsoft::WRL::ComPtr<ID3D11Device1> m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_d3dRenderTargetView;

	// Direct2D 轉譯物件。2D 的必要項。
	Microsoft::WRL::ComPtr<ID2D1Factory1> m_d2dFactory;
	Microsoft::WRL::ComPtr<ID2D1Device> m_d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> m_d2dContext;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_d2dTargetBitmap;

	// Direct3D 轉譯物件。3D 的必要項。
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_d3dDepthStencilView;

	// 快取的轉譯屬性。
	D3D_FEATURE_LEVEL m_d3dFeatureLevel;
	Windows::Foundation::Size m_d3dRenderTargetSize;
	Windows::Foundation::Rect m_windowBounds;
	float m_dpi;
	Windows::Graphics::Display::DisplayOrientations m_orientation;

	// 用於顯示方向的轉換。
	D2D1::Matrix3x2F m_orientationTransform2D;
	DirectX::XMFLOAT4X4 m_orientationTransform3D;
};
