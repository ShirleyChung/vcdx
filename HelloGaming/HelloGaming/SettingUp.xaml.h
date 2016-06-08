//
// SettingUp.xaml.h
// SettingUp 類別的宣告
//

#pragma once

#include "SettingUp.g.h"
#include "LineChart.h"

namespace HelloGaming
{
	/// <summary>
	/// 可以在本身使用或巡覽至框架內的空白頁面。
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SettingUp sealed
	{
		Windows::UI::Xaml::UIElement^ m_parent;
		std::shared_ptr<LineChart> m_lineChart;

	public:
	internal:
		SettingUp(Windows::UI::Xaml::UIElement^ parent, std::shared_ptr<LineChart> lineChart);

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	private:
		void btnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
