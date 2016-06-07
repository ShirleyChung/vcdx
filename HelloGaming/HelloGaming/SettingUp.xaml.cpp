//
// SettingUp.xaml.cpp
// SettingUp 類別的實作
//

#include "pch.h"
#include "SettingUp.xaml.h"

using namespace HelloGaming;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// 空白頁項目範本已記錄在 http://go.microsoft.com/fwlink/?LinkId=234238

SettingUp::SettingUp(Windows::UI::Xaml::UIElement^ p)
	:m_parent(p)
{
	InitializeComponent();
}

/// <summary>
/// 在此頁面即將顯示在框架中時叫用。
/// </summary>
/// <param name="e">描述如何到達此頁面的事件資料。Parameter
/// 屬性通常用來設定頁面。</param>
void SettingUp::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void) e;	// 未使用的參數
}


void HelloGaming::SettingUp::btnClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Window::Current->Content = m_parent;
	Window::Current->Activate();
}