#include "StdAfx.h"
#include "dewesoft_bridge.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace Dewesoft::Utils::Dcom::InputChannel;
using namespace Dewesoft::Utils::Dcom::OutputChannel;
using namespace Dewesoft::Utils::Dcom::Utils;

DewesoftBridge::DewesoftBridge(InputManagerImpl& inputManager, OutputFactoryImpl& outputFactory, const IAppPtr app)
    : inputManager(inputManager)
    , outputFactory(outputFactory)
    , app(app)
    , pluginGroup(nullptr)
    , sineGenerator(outputFactory)
{
}

void DewesoftBridge::setPluginProps(const IAppPtr app, const _bstr_t pluginGuid, IPluginGroup* pluginGroup)
{
    this->app = app;
    this->pluginGuid = pluginGuid;
    this->pluginGroup = pluginGroup;
}

bool DewesoftBridge::checkSupportedDewesoftVersion(std::string& errorMessage) const
{
    long major, minor, revision;
    app->GetInterfaceVersion(&major, &minor, &revision);

    // If function returns false, this message will be displayed on Dewesoft startup.
    errorMessage = "Current Test plugin version is supported from DEWESoftX 2020.1 onwards.";

    return true;
}

void DewesoftBridge::onEnterHardwareSetup()
{
}

void DewesoftBridge::onUpdateSettings(NodePtr node)
{
    if (node->isWrite())
        sineGenerator.saveSettings(node);
    else
        sineGenerator.loadSettings(node);
}

void DewesoftBridge::onInitiateHardware()
{
}

void DewesoftBridge::onSetupEnter(const bool analysisMode)
{
    setupWindow->setupEnter();
}

void DewesoftBridge::onSetupLeave(const bool analysisMode)
{
	setupWindow->setupLeave();
}

void DewesoftBridge::onClearSetup()
{
    sineGenerator.clear();
}

void DewesoftBridge::onNewSetup()
{
    revisionHistory = "";
    sineGenerator.addSineWave(1.0, 0.0);
    sineGenerator.addSineWave(1.0, M_PI / 2.0, 1.0);
    sineGenerator.addSineWave(2.0, M_PI, 10.0);
}

void DewesoftBridge::onLoadSetup(NodePtr node, bool dataFile)
{
    sineGenerator.loadSetup(node);
    node->read(u8"RevisionHistory", revisionHistory, "");
}

void DewesoftBridge::onSaveSetup(NodePtr node, bool dataFile)
{
    sineGenerator.saveSetup(node);
    node->write(u8"RevisionHistory", revisionHistory);
}

void DewesoftBridge::onPreInitiate()
{
}

void DewesoftBridge::onStartData()
{
}

void DewesoftBridge::onGetData(const AcquiredDataInfo& acquiredDataInfo)
{
    const double sampleRate = inputManager.getCurrentSampleRate();
    const double startTime = acquiredDataInfo.beginPos / sampleRate;
    const size_t numSamples = acquiredDataInfo.endPos - acquiredDataInfo.beginPos;

    sineGenerator.getData(startTime, sampleRate, numSamples);
}

void DewesoftBridge::onStopData()
{
}

void DewesoftBridge::onStartStoring()
{
}

void DewesoftBridge::onStopStoring()
{
}

void DewesoftBridge::onPrepareAnalysis()
{
    
}

void DewesoftBridge::onStartAnalysis()
{
}

bool DewesoftBridge::isRecalculationRequired() const
{
    return false;
}

bool DewesoftBridge::showSettings(Dewesoft::MUI::IWindow* window)
{
    try
    {
        settingsWindow = std::make_unique<SettingsWindow>(window, *this);
    }
    catch (const std::exception& e)
    {
        app->WriteErrorMessage(e.what());
        return false;
    }
    return true;
}

void DewesoftBridge::hideSettings()
{
    settingsWindow.reset(nullptr);
}

bool DewesoftBridge::showSetup(Dewesoft::MUI::IWindow* window)
{
    try
    {
        setupWindow = std::make_unique<SetupWindow>(window, *this);
    }
    catch (const std::exception& e)
    {
        std::string msg = e.what();
        app->WriteErrorMessage(e.what());
        return false;
    }
    return true;
}

void DewesoftBridge::hideSetup()
{
    setupWindow.reset(nullptr);
}
