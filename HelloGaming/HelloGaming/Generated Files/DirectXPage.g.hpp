﻿

//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"
#include "DirectXPage.xaml.h"




void ::HelloGaming::DirectXPage::InitializeComponent()
{
    if (_contentLoaded)
        return;

    _contentLoaded = true;

    // Call LoadComponent on ms-appx:///DirectXPage.xaml
    ::Windows::UI::Xaml::Application::LoadComponent(this, ref new ::Windows::Foundation::Uri(L"ms-appx:///DirectXPage.xaml"), ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);

    // Get the SwapChainBackgroundPanel named 'SwapChainPanel'
    SwapChainPanel = safe_cast<::Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"SwapChainPanel"));
    // Get the TextBlock named 'SimpleTextBlock'
    SimpleTextBlock = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"SimpleTextBlock"));
}

void ::HelloGaming::DirectXPage::Connect(int connectionId, Platform::Object^ target)
{
    switch (connectionId)
    {
    case 1:
        (safe_cast<::Windows::UI::Xaml::UIElement^>(target))->PointerMoved +=
            ref new ::Windows::UI::Xaml::Input::PointerEventHandler(this, (void (::HelloGaming::DirectXPage::*)(Platform::Object^, Windows::UI::Xaml::Input::PointerRoutedEventArgs^))&DirectXPage::OnPointerMoved);
        (safe_cast<::Windows::UI::Xaml::UIElement^>(target))->PointerReleased +=
            ref new ::Windows::UI::Xaml::Input::PointerEventHandler(this, (void (::HelloGaming::DirectXPage::*)(Platform::Object^, Windows::UI::Xaml::Input::PointerRoutedEventArgs^))&DirectXPage::OnPointerReleased);
        break;
    case 2:
        (safe_cast<::Windows::UI::Xaml::Controls::Primitives::ButtonBase^>(target))->Click +=
            ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::HelloGaming::DirectXPage::*)(Platform::Object^, Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::OnNextColorPressed);
        break;
    case 3:
        (safe_cast<::Windows::UI::Xaml::Controls::Primitives::ButtonBase^>(target))->Click +=
            ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::HelloGaming::DirectXPage::*)(Platform::Object^, Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::OnPreviousColorPressed);
        break;
    }
    (void)connectionId; // Unused parameter
    (void)target; // Unused parameter
    _contentLoaded = true;
}

