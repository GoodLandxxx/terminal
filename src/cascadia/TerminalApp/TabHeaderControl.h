// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "winrt/Microsoft.UI.Xaml.Controls.h"

#include "TabHeaderControl.g.h"

namespace winrt::TerminalApp::implementation
{
    struct TabHeaderControl : TabHeaderControlT<TabHeaderControl>
    {
        TabHeaderControl();
        void BeginRename();

        void RenameBoxLostFocusHandler(const winrt::Windows::Foundation::IInspectable& sender,
                                       const winrt::Windows::UI::Xaml::RoutedEventArgs& e);

        bool InRename();

        til::event<TerminalApp::TitleChangeRequestedArgs> TitleChangeRequested;
        til::typed_event<> RenameEnded;

        til::property_changed_event PropertyChanged;
        WINRT_OBSERVABLE_PROPERTY(winrt::hstring, Title, PropertyChanged.raise);
        WINRT_OBSERVABLE_PROPERTY(double, RenamerMaxWidth, PropertyChanged.raise);
        WINRT_OBSERVABLE_PROPERTY(winrt::TerminalApp::TerminalTabStatus, TabStatus, PropertyChanged.raise);

    private:
        bool _receivedKeyDown{ false };
        bool _renameCancelled{ false };
        // 标记本次重命名已主动结束(回车提交或 Esc 取消)。置位后随后的 LostFocus
        // 不再重复 raise 提交(回车路径已直接 raise)。
        bool _renameCommitted{ false };
        // BeginRename 的时刻(GetTickCount64 毫秒)。失焦时若距此很近(<=300ms),
        // 判定为调用方 context flyout 关闭的焦点抖动,静默忽略(不关闭、不重新 Focus),
        // 避免"一闪而过";也不重新 Focus 以免触发闪烁循环。
        uint64_t _renameStartedTick{ 0 };

        void _CloseRenameBox();
    };
}

namespace winrt::TerminalApp::factory_implementation
{
    BASIC_FACTORY(TabHeaderControl);
}
