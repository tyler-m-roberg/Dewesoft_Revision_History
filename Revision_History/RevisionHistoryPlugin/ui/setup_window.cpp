#include "StdAfx.h"
#include "setup_window.h"
#include "dewesoft_bridge.h"
#include <thread>
#include <chrono>
#include <regex>

using namespace Dewesoft::MUI;
using namespace Dewesoft::RT::Core;

enum Cols
{
    ctNum,
    ctUsed,
    ctStore,
    ctColor,
    ctName,
    ctDesc,
    ctTimebase,
    ctMin,
    otLivePreview,
    ctMax,
    ctValue,
    ctStartVal,
    ctResetVal,
    otUnit,
    ctReset,
};

static Float value = 0.0;
static bool decrement = false;

SetupWindow::SetupWindow(WindowPtr ui, DewesoftBridge& bridge) : BaseSetupWindow(ui, bridge, "ui/setup_window.xml")
{
    uiRefreshTimer = Timer::Create(ui);
    uiRefreshTimer.setInterval(100);
	
    /* bind events */
    TabTest.OnChange += event(&SetupWindow::onTabChanged); // on tab clicked
    labelEdit.OnTextChanged += event(&SetupWindow::onEditTextChanged); // on textBox text change
    comboBox1.OnChange += event(&SetupWindow::onComboBoxChanged); // on combo box selected item changed   
    showHideTextBox.OnCheckedChanged += event(&SetupWindow::onCheckBoxChanged); // on checkbox clicked
    Radio1.OnCheckedChanged += event(&SetupWindow::onRadioGroupChanged); // on radio button clicked
    Radio2.OnCheckedChanged += event(&SetupWindow::onRadioGroupChanged);     
    showModal.OnClick += event(&SetupWindow::onShowModal);	
	uiRefreshTimer.OnTimer += event(&SetupWindow::onUiRefreshTimer);

    // Called when the grid needs to update row info
    channelGrid.OnCellGetProps += event(&SetupWindow::testGridGetProps);

    // Called to provide row "live value" cell info.
    channelGrid.OnCellGetLiveValue += event(&SetupWindow::testGridLiveValues);

    // Called when a cell with a dropdown/combobox is clicked to provide dropdown items
    channelGrid.OnCellGetComboItems += event(&SetupWindow::testGridComboItems);

    // Set number of gird columns and rows + 1 (1st row is the column headers row)
    channelGrid.setGridSize(10, 15);

    /* Set default column types (can be overriden in OnCellGetProps event)
     * 
     * Arguments are: 
     *     zero based index, 
     *     header name,
     *     type,
     *     default visibility,
     *     width,
     *     key/property
     */
    channelGrid.setColumn(ctNum, "#", ctNumber, RtTrue, 40, "Num");
    channelGrid.setColumn(ctUsed, "Used", ctUdButton, RtTrue, 60, "Used");
    channelGrid.setColumn(ctStore, "Store", ctUdButton, RtFalse, 50, "Store");
    channelGrid.setColumn(Cols::ctColor, "Color", DrawGridCellType::ctColor, RtTrue, 30, "Color");
    channelGrid.setColumn(ctName, "Name", ctEditText, RtTrue, 140, "Name");
    channelGrid.setColumn(ctDesc, "Description", ctEditText, RtTrue, 140, "Desc");
    channelGrid.setColumn(ctTimebase, "Timebase", ctCombobox, RtTrue, 100, "Timebase");
    channelGrid.setColumn(ctMin, "Min", ctEditNumber, RtTrue, 80, "Min");
    channelGrid.setColumn(otLivePreview, "Values", ctLiveValue, RtTrue, 130, "Value");
    channelGrid.setColumn(ctMax, "Max", ctEditNumber, RtTrue, 80, "Max");
    channelGrid.setColumn(ctValue, "Value", ctNumber, RtTrue, 80, "Value");
    channelGrid.setColumn(ctStartVal, "Default value", ctEditNumber, RtTrue, 80, "InitVal");
    channelGrid.setColumn(ctResetVal, "Value reset", ctCombobox, RtTrue, 140, "ValueRes");
    channelGrid.setColumn(otUnit, "Unit", ctEditText, RtTrue, 40, "Unit");
    channelGrid.setColumn(ctReset, "Reset value", ctButton, RtTrue, 80, "ResetVal");

    // Set default number fromat for "ctNum" (row number) to not have decimals
    channelGrid.getColumn(ctNum).setNumberFormat(cfInteger);

    // we're done setting column info
    channelGrid.applyColumns();	
	
    addButttonControlToStackPanel(ui);
}

SetupWindow::~SetupWindow()
{
   
}

void SetupWindow::setupEnter()
{
    uiRefreshTimer.setEnabled(true);
}

void SetupWindow::setupLeave()
{
    uiRefreshTimer.setEnabled(false);
}

void SetupWindow::onUiRefreshTimer(Timer& ctrl, Dewesoft::MUI::EventArgs& args)
{
    if (channelGrid.assigned())
        channelGrid.invalidate();
}

void SetupWindow::onTabChanged(TabControl& ctrl, Dewesoft::MUI::EventArgs& args)
{
	
}

void SetupWindow::testGridComboItems(DSDrawGrid& grid, DrawGridComboItemsArgs& args)
{
    Int col = args.getColumn();

    Int index;
    switch (col)
    {
        case ctTimebase:
            index = args.add("Async");
            index = args.add("Single value");
            break;
        case ctResetVal:
            index = args.add("On start measurement");
            index = args.add("On load setup");
            index = args.add("Always keep last value");
            break;
        default:
            break;
    }
}

void SetupWindow::testGridLiveValues(DSDrawGrid& grid, DrawGridLiveValueArgs& args)
{
    args.setLimitMaximum(500);
    args.setLimitMinimum(0);

    args.setCurrentMinimum(value);
    args.setCurrentMaximum(value);

    args.setOverloadMinimum(0);
    args.setOverloadMaximum(500);
    args.setShowOverloadWarning(true);
    args.setUnit("Cpp");
    args.setPrecision(3);
}

void SetupWindow::testGridGetProps(DSDrawGrid& grid, DrawGridCellPropsArgs& args)
{
    Int row = args.getRow();
    Int col = args.getColumn();

    switch (col)
    {
        case ctNum:
            args.setNumber(static_cast<Float>(row));
            break;
        case ctUsed:
            args.setText("Used");
            args.setIsButtonDown(RtTrue);
            break;
        case ctStore:
            args.setText("Store");
            args.setIsButtonDown(RtTrue);
            break;
        case Cols::ctColor:
            args.setCellColor(0xff0000);
            break;
        case ctName:
            args.setText("Channel name C++");
            break;
        case ctDesc:
            args.setText("Channel description C++");
            break;
        case ctTimebase:
            args.setText("Async C++");
            break;
        case ctMin:
            args.setNumber(0.0);
            break;
        case ctMax:
            args.setNumber(100.0);
            break;
        case ctValue:
            if (value > 600)
            {
                decrement = true;
            }
            else if (value < -100)
            {
                decrement = false;
            }

            if (decrement)
            {
                value--;
            }
            else
            {
                value++;
            }

            args.setNumber(value);
            break;
        case ctStartVal:
            args.setNumber(0.0);
            break;
        case ctResetVal:
        {
            args.setText("On start measurement C++");
            break;
        }
        case otUnit:
            args.setText("");
            break;
        case ctReset:
            args.setText("Reset");
            break;
        default:
            args.setText("");
            break;
    }
}

void SetupWindow::onEditTextChanged(TextBox& editBox, Dewesoft::MUI::EventArgs& args)
{
    textBoxLabel.setText(editBox.getText());
}

void SetupWindow::onSpinEditChanged(TextBox& editBox, Dewesoft::MUI::EventArgs& args)
{
    if (editBox.getName() == "MySpinEdit")
        std::string str = editBox.getText().toStdString();
}

void SetupWindow::onComboBoxChanged(ComboBox& button, Dewesoft::MUI::EventArgs& args)
{
    std::string text = "Selected item: " + button.getSelectedItem().toStdString();
    selectedItemLabel.setText(text);
}

void SetupWindow::onButtonClicked(Button& btn, Dewesoft::MUI::EventArgs& args)
{
    comboBox1->setIsEnabled(!comboBox1.getIsEnabled());
}

void SetupWindow::onCheckBoxChanged(CheckBox& checkBox, Dewesoft::MUI::EventArgs& args)
{
    Visibility visibility = labelEdit.getVisibility();
    if (visibility == vVisible)
    {
        labelEdit.setVisibility(Visibility::vHidden);
    }
    else
    {
        labelEdit.setVisibility(Visibility::vVisible);
    }
}

void SetupWindow::onRadioGroupChanged(RadioButton& radioGrup, Dewesoft::MUI::EventArgs& args)
{
    std::string text = "Selected item: " + radioGrup.getLabel().toStdString();

    SampleText.setText(text);
}

void SetupWindow::addButttonControlToStackPanel(Dewesoft::MUI::WindowPtr ui)
{
    Dewesoft::MUI::Button enableDisableButton;
    enableDisableButton = Button::Create(ui);
    enableDisableButton.setCaption("Enable/Disable");
    enableDisableStackPanel.addControl(enableDisableButton);
    enableDisableButton.OnClick += event(&SetupWindow::onButtonClicked);
}

void SetupWindow::onShowModal(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args)
{
    std::unique_ptr<ModalWindow> addNotificationWindow = std::make_unique<ModalWindow>(Window::Create(window));
    DialogResult result = addNotificationWindow->showDialog();

    if (result == DialogResult::OK)
    {
        // do something when closed   
    }
}
