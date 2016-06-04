#include "pch.h"
#include "Pane.h"


Pane::Pane(void)
	:m_bkcolor(D2D1::ColorF::Black, 0.5f)
{
}


Pane::~Pane(void)
{
}

void Pane::CreateDeviceDependentResources(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context, IWICImagingFactory2 *wicFactory, LPCWSTR filename)
{
	DX::ThrowIfFailed( context->CreateSolidColorBrush(m_bkcolor, &m_bkBrush) );
/*
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
*/
}

void Pane::SetRect(D2D1_RECT_F rt)
{
	m_rect = rt; 
}

void Pane::Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> context)
{
	context->FillRectangle(m_rect, m_bkBrush.Get());
//	context->DrawBitmap(m_pBmp, &m_rect);
}
