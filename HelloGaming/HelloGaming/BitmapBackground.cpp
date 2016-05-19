#include "pch.h"
#include "BitmapBackground.h"


BitmapBackground::BitmapBackground(void)
{
}


BitmapBackground::~BitmapBackground(void)
{
	m_pBmp->Release();
}

void BitmapBackground::CreateDeviceDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context, IWICImagingFactory2 *wicFactory, LPCWSTR filename)
{
	IWICBitmapDecoder *pDecoder = nullptr;
	DX::ThrowIfFailed( wicFactory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder) );

	IWICBitmapFrameDecode *pFrame = nullptr;
	DX::ThrowIfFailed( pDecoder->GetFrame(0, &pFrame) );

	IWICFormatConverter *pConvtSourceBmp;
	DX::ThrowIfFailed( wicFactory->CreateFormatConverter(&pConvtSourceBmp) );
	DX::ThrowIfFailed( pConvtSourceBmp->Initialize( pFrame, GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom ) );

	DX::ThrowIfFailed( context->CreateBitmapFromWicBitmap( pConvtSourceBmp, &m_pBmp ) );

	pDecoder->Release();
	pFrame->Release();
	pConvtSourceBmp->Release();
}

void BitmapBackground::CreateWindowSizeDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
	m_rect = D2D1::RectF(0, 0, context->GetSize().width, context->GetSize().height);
}

void BitmapBackground::Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
	context->DrawBitmap(m_pBmp, &m_rect);
}
