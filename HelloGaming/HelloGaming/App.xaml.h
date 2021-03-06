﻿//
// App.xaml.h
// App 類別的宣告。
//

#pragma once

#include "App.g.h"
#include "DirectXPage.xaml.h"

namespace HelloGaming
{
	/// <summary>
	/// 提供應用程式專屬行為以補充預設的應用程式類別。
	/// </summary>
	ref class App sealed
	{
	public:
		App();
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ args) override;

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
		DirectXPage^ m_directXPage;
	};
}
