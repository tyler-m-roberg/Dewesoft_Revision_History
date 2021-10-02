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

private:
	Dewesoft::MUI::Timer uiRefreshTimer;
	
    void onTabChanged(Dewesoft::MUI::TabControl& ctrl, Dewesoft::MUI::EventArgs& args);
	void onUiRefreshTimer(Dewesoft::MUI::Timer& ctrl, Dewesoft::MUI::EventArgs& args);
    void testGridComboItems(Dewesoft::MUI::DSDrawGrid& grid, Dewesoft::MUI::DrawGridComboItemsArgs& args);
    void testGridLiveValues(Dewesoft::MUI::DSDrawGrid& grid, Dewesoft::MUI::DrawGridLiveValueArgs& args);
    void testGridGetProps(Dewesoft::MUI::DSDrawGrid& grid, Dewesoft::MUI::DrawGridCellPropsArgs& args);
    void onEditTextChanged(Dewesoft::MUI::TextBox& editBox, Dewesoft::MUI::EventArgs& args);
    void onSpinEditChanged(Dewesoft::MUI::TextBox& editBox, Dewesoft::MUI::EventArgs& args);
    void onComboBoxChanged(Dewesoft::MUI::ComboBox& button, Dewesoft::MUI::EventArgs& args);
    void onButtonClicked(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args);
    void onCheckBoxChanged(Dewesoft::MUI::CheckBox& checkBox, Dewesoft::MUI::EventArgs& args);
    void onRadioGroupChanged(Dewesoft::MUI::RadioButton& radioGrup, Dewesoft::MUI::EventArgs& args);
    void addButttonControlToStackPanel(Dewesoft::MUI::WindowPtr ui);
    void onShowModal(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args);
};

