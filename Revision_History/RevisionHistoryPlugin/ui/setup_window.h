#pragma once
#include <mui/ds_window.h>
#include <mui/controls.h>
#include <mui/layout.h>
#include "base_setup_window.h"
#include "modal_window.h"
#include <mui/controls/timer_ptr.h>

class DewesoftBridge;

class SetupWindow : public BaseSetupWindow
{
public:
    explicit SetupWindow(Dewesoft::MUI::WindowPtr ui, DewesoftBridge& bridge);
    ~SetupWindow();
	
    void setupEnter();
    void setupLeave();

    Dewesoft::MUI::Memo memoTxt;

    Dewesoft::MUI::TextBox majorTxt;
    Dewesoft::MUI::TextBox minorTxt;
    Dewesoft::MUI::TextBox revTxt;
    Dewesoft::MUI::TextBox buildTxt;
    Dewesoft::MUI::Label usedSetupLbl;

private:
	Dewesoft::MUI::Timer uiRefreshTimer;
    DewesoftBridge& bridge;

    void onEditTextChanged(Dewesoft::MUI::Memo& memoBox, Dewesoft::MUI::EventArgs& args);

    void onMajorTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args);
    void onMinorTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args);
    void onRevisionTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args);
    void onBuildTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args);

};

