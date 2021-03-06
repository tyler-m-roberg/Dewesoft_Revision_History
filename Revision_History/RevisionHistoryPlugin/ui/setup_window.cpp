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

SetupWindow::SetupWindow(WindowPtr ui, DewesoftBridge& bridge)
    : BaseSetupWindow(ui, bridge, "ui/setup_window.xml")
    , bridge(bridge)
{
    uiRefreshTimer = Timer::Create(ui);
    uiRefreshTimer.setInterval(100);

    /* bind events */

    SetupWindow::memoTxt = Memo::Connect(ui, "revisionHistoryMemo");
    memoTxt.OnChange += event(&SetupWindow::onEditTextChanged);
    memoTxt.setText("");

    SetupWindow::majorTxt = TextBox::Connect(ui, "txtMajor");
    SetupWindow::minorTxt = TextBox::Connect(ui, "txtMinor");
    SetupWindow::revTxt = TextBox::Connect(ui, "txtRevision");
    SetupWindow::buildTxt = TextBox::Connect(ui, "txtBuild");
    SetupWindow::usedSetupLbl = Label::Connect(ui, "usedSetupLbl");

    majorTxt.OnTextChanged += event(&SetupWindow::onMajorTextChanged);
    minorTxt.OnTextChanged += event(&SetupWindow::onMinorTextChanged);
    revTxt.OnTextChanged += event(&SetupWindow::onRevisionTextChanged);
    buildTxt.OnTextChanged += event(&SetupWindow::onBuildTextChanged);

    majorTxt.setText("");
    minorTxt.setText("");
    revTxt.setText("");
    buildTxt.setText("");
}

SetupWindow::~SetupWindow()
{
}

void SetupWindow::setupEnter()
{
    uiRefreshTimer.setEnabled(true);

    memoTxt.clear();
    std::istringstream iss(bridge.revisionHistory);
    std::string token;

    while (std::getline(iss, token, '\n'))
    {
        memoTxt.addLine(token);
    }

    majorTxt.setText(std::to_string(bridge.majorVer));
    minorTxt.setText(std::to_string(bridge.minorVer));
    revTxt.setText(std::to_string(bridge.revVer));
    buildTxt.setText(std::to_string(bridge.buildVer));
    usedSetupLbl.setText(bridge.bridgeUsedSetupFile);
}

void SetupWindow::setupLeave()
{
    uiRefreshTimer.setEnabled(false);
    std::string tempString = memoTxt.getText();
    tempString.erase(std::remove(tempString.begin(), tempString.end(), '\r'), tempString.end());
    bridge.revisionHistory = tempString;

}

void SetupWindow::onEditTextChanged(Dewesoft::MUI::Memo& memoBox, Dewesoft::MUI::EventArgs& args)
{
    uiRefreshTimer.setEnabled(false);
    std::string tempString = memoTxt.getText();
    tempString.erase(std::remove(tempString.begin(), tempString.end(), '\r'), tempString.end());
    bridge.revisionHistory = tempString;
}

void SetupWindow::onMajorTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args)
{
    if (majorTxt.getText() != "")
        bridge.majorVer = std::stoi(majorTxt.getText().toStdString());
}

void SetupWindow::onMinorTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args)
{
    if (minorTxt.getText() != "")
        bridge.minorVer = std::stoi(minorTxt.getText().toStdString());
}

void SetupWindow::onRevisionTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args)
{
    if (revTxt.getText() != "")
        bridge.revVer = std::stoi(revTxt.getText().toStdString());
}

void SetupWindow::onBuildTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args)
{
    if (buildTxt.getText() != "")
        bridge.buildVer = std::stoi(buildTxt.getText().toStdString());
}
