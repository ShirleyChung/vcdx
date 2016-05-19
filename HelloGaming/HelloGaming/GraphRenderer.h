#pragma once

#include "DirectXBase.h"

#include "SolidBackground.h"
#include "GradianBackground.h"
#include "BitmapBackground.h"

// 這個類別會轉譯有彩色背景的簡單文字。
ref class GraphRenderer sealed : public DirectXBase
{
public:
	GraphRenderer();
	virtual ~GraphRenderer();

	// DirectXBase 方法。
	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;

	// 用於更新時間相依物件的方法。
	void Update(float timeTotal, float timeDelta);

	// 滑鼠移上去的事件
	void PointerMoved(Windows::Foundation::Point point);

private:
	Windows::Foundation::Point m_pan;
	SolidBackground	*m_solidBackground;
	GradianBackground *m_graBackground;
	BitmapBackground *m_bmpBackground;
};
