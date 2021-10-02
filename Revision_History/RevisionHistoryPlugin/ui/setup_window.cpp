#include "StdAfx.h"
#include "setup_window.h"
#include "dewesoft_bridge.h"
#include <thread>
#include <chrono>
#include <regex>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace Dewesoft::MUI;
using namespace Dewesoft::RT::Core;

SetupWindow::SetupWindow(WindowPtr ui, DewesoftBridge& bridge) : BaseSetupWindow(ui, bridge, "ui/setup_window.xml")
{
    uiRefreshTimer = Timer::Create(ui);
    uiRefreshTimer.setInterval(100);
	
    /* bind events */
	uiRefreshTimer.OnTimer += event(&SetupWindow::onUiRefreshTimer);

    SetupWindow::memoTxt = Memo::Connect(ui, "revisionHistoryMemo");
    memoTxt.setText("");

}

SetupWindow::~SetupWindow()
{
   
}

void SetupWindow::setupEnter()
{
    uiRefreshTimer.setEnabled(true);

    memoTxt.setText("");

    std::istringstream iss(bridge.revisionHistory);
    std::string token;

    while (std::getline(iss, token, '\n'))
    {
        memoTxt.addLine(token);
    }
}

void SetupWindow::setupLeave()
{
    uiRefreshTimer.setEnabled(false);
    std::string tempString = memoTxt.getText();
    tempString.erase(std::remove(tempString.begin(), tempString.end(), '\r'), tempString.end());
    bridge.revisionHistory = tempString;
}

void SetupWindow::onUiRefreshTimer(Timer& ctrl, Dewesoft::MUI::EventArgs& args)
{

}


