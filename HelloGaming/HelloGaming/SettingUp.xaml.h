//
// SettingUp.xaml.h
// SettingUp 類別的宣告
//

#pragma once

#include "SettingUp.g.h"

namespace HelloGaming
{
	/// <summary>
	/// 可以在本身使用或巡覽至框架內的空白頁面。
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SettingUp sealed
	{
		Windows::UI::Xaml::UIElement^ m_parent;

	public:
		SettingUp(Windows::UI::Xaml::UIElement^ parent);

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	};
}
