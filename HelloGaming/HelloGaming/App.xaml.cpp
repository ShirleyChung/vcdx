//
// App.xaml.cpp
// App 類別的實作。
//

#include "pch.h"
#include "DirectXPage.xaml.h"

using namespace HelloGaming;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

/// <summary>
/// 初始化單一應用程式物件。這是第一行執行之撰寫程式碼，
/// 而且其邏輯相當於 main() 或 WinMain()。
/// </summary>
App::App()
{
	InitializeComponent();
	Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
}

/// <summary>
/// 在應用程式由使用者正常啟動時叫用。其他進入點
/// 將在啟動應用程式以開啟特定檔案時使用，以顯示
/// 搜尋結果等。
/// </summary>
/// <param name="args">關於啟動要求和處理序的詳細資料。</param>
void App::OnLaunched(LaunchActivatedEventArgs^ args)
{
	m_directXPage = ref new DirectXPage();

	if (args->PreviousExecutionState == ApplicationExecutionState::Terminated)
	{
		m_directXPage->LoadInternalState(ApplicationData::Current->LocalSettings->Values);
	}

	// 將頁面放在目前視窗中，並確定它是作用中。
	Window::Current->Content = m_directXPage;
	Window::Current->Activate();
}

/// <summary>
/// 在應用程式正要暫停時叫用。
/// </summary>
/// <param name="sender">關於事件起源的詳細資料。</param>
/// <param name="args">關於暫停事件的詳細資料。</param>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ args)
{
	(void) sender; // 未使用的參數。
	(void) args; // 未使用的參數。

	m_directXPage->SaveInternalState(ApplicationData::Current->LocalSettings->Values);
}