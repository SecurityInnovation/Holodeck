//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		MainFrame.h
//
// DESCRIPTION: Contains definition for the class MainFrame
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 07 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <stdio.h>
#include <vcclr.h>

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>
#using <GuiSupport.dll>
#using <HoloScript.dll>
#using <CustomTestProjectGenerator.dll>

#include "Registry.h"
#include "SiString.h"
#include "SplashScreen.h"
#include "ProjectPaneDataArray.h"
#include "ResourcePaneDataArray.h"
#include "DataEventRegistry.h"
#include "UIEventRegistry.h"
#include "ImageListManager.h"
#include "LogPaneDataArray.h"
#include "LogPane.h"
#include "ResourcePane.h"
#include "NonDockablePaneManager.h"
#include "FaultPaneDataArray.h"
#include "ReportPaneDataArray.h"
#include "RecordedSessionDataArray.h"
#include "ScheduledTestDataArray.h"
#include "ResourceTestDataArray.h"
#include "TestGenerationDataArray.h"
#include "NetworkCorruptionDataArray.h"
#include "FileCorruptionDataArray.h"
#include "HelpPane.h"
#include "printer.h"
#include "StressTestGenerationDlg.h"
#include "CodeCoverageTestGenerationDlg.h"
#include "NewProjectWizard.h"
#include "FindNextDialog.h"
#include "Project.h"
#include "LaunchProgressDialog.h"
#include "NetworkLogFindNextDialog.h"
#include "FaultyUserLibrariesDialog.h"
#include "defines.h"
#include "ProjectPane.h"
#include "PropertiesPane.h"
#include "LimitPane.h"
#include "ReportPane.h"
#include "FaultPane.h"
#include "ScheduledTestPane.h"
#include "AboutBox.h"
#include "FieldChooser.h"
#include "NewTestWizard.h"
#include "NewResourceTestWizard.h"
#include "NetworkCorruptionPane.h"
#include "ResourceTestPane.h"
#include "NetworkCorruptionWizard.h"
#include "FileCorruptionPane.h"
#include "FileCorruptionWizard.h"
#include "FileCorruptionDetailsPane.h"
#include "FileCorruptionChangesParser.h"
#include "SavedFileCorruptionPane.h"
#include "AttachApplicationForm.h"
#include "UserNotification.h"
#include "RegistrationMgr.h"
#include "RegistrationDialog.h"
#include "ProjectFileWriter.h"
#include "ProjectFileReader.h"
#include "HtmlDialog.h"
#include "NetworkLogPane.h"
#include "NetworkMessageDetailsPane.h"
#include "ExceptionPane.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Common;
using namespace Crownwood::Magic::Docking;
using namespace Crownwood::Magic::Menus;
using namespace System::Threading;
using namespace Microsoft::Win32;
using namespace SiUtils;
using namespace UIData;
using namespace HoloScript;
using namespace FunctionDatabase;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Xml::XPath;
using namespace System::IO;
using namespace System::Security;
using namespace CustomTestProjectGenerator;
using namespace System::Reflection;

namespace HolodeckGui
{
	public __delegate void StartupUserNotificationDelegate();

	//*********************************************************************
	// defines a new process' displayed pages
	//*********************************************************************
	public __gc struct NewProcessTabPage
	{
		DisplayableDataNode *node;
		bool makeActive;
	};

	//*********************************************************************
	// defines the primary window for the Holodeck UI
	//*********************************************************************
	public __gc class MainFrame : public System::Windows::Forms::Form
	{
	private:
		ArrayList *commandLineArgs;						// command line arguments

	public:
		MainFrame(StartupUserNotificationDelegate *notificationDelegate);
		~MainFrame();

		void ProcessCommandLine();

		__property ArrayList *get_CommandLineArgs() { return commandLineArgs; }
		__property void set_CommandLineArgs(ArrayList *value) { commandLineArgs = value; }


		void launchNewIntercepts();

		static String* ConstructTabTitle(DisplayableDataNode *dataNode);
		void CheckForFaultyUserLibraries();

	protected:
		virtual void OnLoad(EventArgs* e);

	private: // UI COMPONENT MEMBERS //
		__delegate void xRestartProjectDelegate(bool pauseOnStart);		//delegate for x-thread marshalling
		xRestartProjectDelegate *xRestartProjectHandler;

		__delegate void DisplayNewProcessDelegate(DisplayableDataNode *dataNode, Boolean makeTabActive);  // delegate for x-thread tab page displaying
		DisplayNewProcessDelegate *displayNewProcessHandler;

		__property bool get_IsProjectActive() { return (currentProject && currentProject->IsActive); }
		__property void set_IsProjectActive(bool value) { currentProject->IsActive = value; }

		String *recordMenuItemText;
		String *replayMenuItemText;
		String *codeCoverageMenuItemText;
		String *stressMenuItemText;
		DockingManager *dockManager;
		ToolBar *buttonBar;
		ProjectPaneDataArray *projectPaneData;
		DataEventRegistry *dataEventRegistry;
		TestGenerationDataArray *testGenerationData;
		UIEventRegistry *uiEventRegistry;
		HoloScriptRegistry *holoScriptRegistry;
		FindNextDialog * findNextDialog;
		NetworkLogFindNextDialog *netLogFindNextDialog;
		bool codeCoverageRunning;
		bool isRunning;
		Hashtable *createdThreadsTable;
		Mutex *threadCreatedMutex;

		DeleteNetworkLogPaneDelegate *DeleteNetworkLogPaneHandler;
		SaveNetworkLogPaneDelegate *SaveNetworkLogPaneHandler;
		DeleteLogPaneDelegate *DeleteLogPaneHandler;
		SaveLogPaneDelegate *SaveLogPaneHandler;
		SaveResourcePaneDelegate *SaveResourcePaneHandler;
		DoubleClickDelegate *DoubleClickHandler;
		HelpPaneLinkClickedDelegate *HelpPaneLinkClickedHandler;
		DeleteReportDelegate *DeleteReportHandler;
		CreateReportDelegate *CreateReportHandler;
		StartCreateTestWizardDelegate *StartCreateTestWizardHandler;
		StartModifyTestWizardDelegate *StartModifyTestWizardHandler;
		StartNetworkCorruptionWizardDelegate *StartNetworkCorruptionWizardHandler;
		StartModifyNetworkCorruptionWizardDelegate *StartModifyNetworkCorruptionWizardHandler;
		StartFileCorruptionWizardDelegate *StartFileCorruptionWizardHandler;
		StartModifyFileCorruptionWizardDelegate *StartModifyFileCorruptionWizardHandler;
		StartCreateResourceTestWizardDelegate *StartCreateResourceTestWizardHandler;
		StartModifyResourceTestWizardDelegate *StartModifyResourceTestWizardHandler;
		ReportModifiedDelegate *ReportModifiedHandler;
		SaveReportDelegate *SaveReportHandler;
		DeleteRecordedSessionDelegate *DeleteRecordedSessionHandler;
		CreateRecordedSessionDelegate *CreateRecordedSessionHandler;
		RecordedSessionModifiedDelegate *RecordedSessionModifiedHandler;
		RecordedSessionReplayStartDelegate *RecordedSessionReplayStartHandler;
		RecordedSessionReplayStopDelegate *RecordedSessionReplayStopHandler;
		RecordedSessionStartDelegate *RecordedSessionStartHandler;
		RecordedSessionStopDelegate *RecordedSessionStopHandler;
		RecordedSessionReplayStartedDelegate *RecordedSessionReplayStartedHandler;
		RecordedSessionReplayStoppedDelegate *RecordedSessionReplayStoppedHandler;
		RecordedSessionStartedDelegate *RecordedSessionStartedHandler;
		RecordedSessionStoppedDelegate *RecordedSessionStoppedHandler;
		RestartProjectDelegate *RestartProjectHandler;
		SaveProjectDelegate *SaveProjectHandler;
		CodeCoverageTestGenerationStoppedDelegate *CCTestGenStoppedHandler;
		StressTestGenerationStoppedDelegate *StressTestGenStoppedHandler;
		FocusChangedDelegate *FocusChangedHandler;
		ProjectPaneFocusChangedDelegate * ProjectPaneFocusChangedHandler;
		URLClickedDelegate *URLClickedHandler;
		ShowDialogDelegate *ShowDialogHandler;
		ShowCommonDialogDelegate *ShowCommonDialogHandler;
		CopyToClipboardDelegate *CopyToClipBoardHandler;
		PauseProcessDelegate *PauseProcessHandler;
		ProcessPausedDelegate *ProcessPausedHandler;
		ResumeProcessDelegate *ResumeProcessHandler;
		TerminateProcessDelegate *TerminateProcessHandler;
		RestartProcessDelegate *RestartProcessHandler;
		RemoveProcessDelegate *RemoveProcessHandler;
		ShowFileCorruptionDetailsPaneDelegate *ShowFileCorruptionDetailsPaneHandler;
		ShowNetworkCorruptionDetailsPaneDelegate *ShowNetworkCorruptionDetailsPaneHandler;

		Content *projectPane;
		Content *propertiesPane;
		Content *helpPane;
		Content *resourcePane;
		Content *limitPane;
		Content *fileCorruptDetailsPane;
		Content *networkMessageDetailsPane;

		ResourcePane *resPane;
		LimitPane *limPane;
		FileCorruptionDetailsPane *fcDetailsPane;
		NetworkMessageDetailsPane *nmDetailsPane;
		HelpPane *dynamicHelpPane;
		Printer *holoPrinter;

		ToolBarButton *projectPaneButton;
		ToolBarButton *resourcePaneButton;
		ToolBarButton *limitsPaneButton;
		ToolBarButton *fileCorruptDetailsPaneButton;
		ToolBarButton *networkMessageDetailsPaneButton;
		ToolBarButton *propertiesPaneButton;
		ToolBarButton *helpPaneButton;

		MenuCommand *viewProjectPaneMenuItem;
		MenuCommand *viewResourcePaneMenuItem;
		MenuCommand *viewLimitsPaneMenuItem;
		MenuCommand *viewFileCorruptDetailsPaneMenuItem;
		MenuCommand *viewNetworkMessageDetailsPaneMenuItem;
		MenuCommand *viewPropertiesPaneMenuItem;
		MenuCommand *viewHelpPaneMenuItem;
		MenuCommand *sessionRecordSessionMenuItem;
		MenuCommand *sessionReplaySessionMenuItem;
		MenuCommand *sessionCodeCoverageTestGeneratorMenuItem;
		MenuCommand *sessionStressTestGeneratorMenuItem;
		MenuCommand *sessionPerThreadMenuItem;
		
		MenuCommand *applicationCopySelectedItemMenuItem;
		MenuCommand *applicationPauseMenuItem;
		MenuCommand *applicationStopMenuItem;
		MenuCommand *applicationRestartMenuItem;
		MenuCommand *applicationRemoveMenuItem;
		MenuCommand *applicationAttachDebuggerMenuItem;
		MenuCommand *applicationLogFirstChanceMenuItem;
		MenuCommand *applicationInheritSettingsMenuItem;

		Project *currentProject;
		bool silentMode;
		StressTestGenerationDlg *stressTestGenerationDlg;
		CodeCoverageTestGenerationDlg *codeCoverageTestGenerationDlg;
		Queue *pendingServicesToStart;
		Queue *pendingTabPagesToDisplay;
		Threading::Thread *displayTabPagesThread;
		String *startPageUrl;
		Crownwood::Magic::Controls::TabPage *startPage;
		FormStateAndSizeSaver *stateAndSizeSaver;

		NonDockablePaneManager *nonDockablePaneManager;

		NewProjectWizard *wizardToLaunchAppFrom;
		LaunchProgressDialog *launchProgressDlg;
		HoloScriptApplication *hsAppToLaunch;
		HoloScriptApplication *hsAppToRestart;

		VisualStyle visualStyle;

		void InitializeUI();
		MenuControl *MakeMenuBar();
		ToolBar *MakeButtonBar();
		void fileNewProjectMenuItemClicked(Object *sender, EventArgs *args);
		void fileNewTestApplicationMenuItemClicked(Object *sender, EventArgs *args);
		void fileOpenProjectMenuItemClicked(Object *sender, EventArgs *args);
		void fileCloseMenuItemClicked(Object *sender, EventArgs *args);
		void fileExportLogToFileMenuItemClicked (Object *sender, EventArgs *args);
		void fileExportResourcesToFileMenuItemClicked (Object *sender, EventArgs *args);
		void fileSaveProjectMenuItemClicked(Object *sender, EventArgs *args);
		void fileSaveProjectAsMenuItemClicked(Object *sender, EventArgs *args);
		void filePrintLogMenuItemClicked(Object *sender, EventArgs *args);
		void fileExitMenuItemClicked(Object *sender, EventArgs *args);
		void applicationCopySelectedItemMenuItemClicked(Object *sender, EventArgs *args);
		void applicationPauseMenuItemClicked(Object *sender, EventArgs *args);
		void applicationStopMenuItemClicked(Object *sender, EventArgs *args);
		void applicationRestartMenuItemClicked(Object *sender, EventArgs *args);
		void applicationRemoveMenuItemClicked(Object *sender, EventArgs *args);
		void applicationCreateATestMenuItemClicked(Object *sender, EventArgs *args);
		void applicationAttachDebuggerMenuItemClicked(Object *sender, EventArgs *args);
		void applicationLogFirstChanceMenuItemClicked(Object *sender, EventArgs *args);
		void applicationInheritSettingsMenuItemClicked(Object *sender, EventArgs *args);
		void applicationCreateAResourceTestMenuItemClicked(Object *sender, EventArgs *args);
		void applicationNewFaultMenuItemClicked(Object *sender, EventArgs *args);
		void applicationNewNetworkCorruptionMenuItemClicked(Object *sender, EventArgs *args);
		void applicationNewFileCorruptionMenuItemClicked(Object *sender, EventArgs *args);
		void sessionCodeCoverageTestGeneratorMenuItemClicked(Object *sender, EventArgs *args);
		void sessionStressTestGeneratorMenuItemClicked(Object *sender, EventArgs *args);
		void sessionRecordSessionMenuItemClicked(Object *sender, EventArgs *args);
		void sessionReplaySessionMenuItemClicked(Object *sender, EventArgs *args);
		void sessionCreateReportMenuItemClicked(Object *sender, EventArgs *args);
		void sessionPerThreadMenuItemClicked(Object *sender, EventArgs *args);
		void logFindMenuItemClicked(Object *sender, EventArgs *args);
		void logFindNextMenuItemClicked(Object *sender, EventArgs *args);
		void logFilterMenuItemClicked(Object *sender, EventArgs *args);
		void toolsCustomWrapperLibraryMenuItemClicked(Object *sender, EventArgs *args);
		void toolsAddHolodeckInterceptsMenuItemClicked(Object *sender, EventArgs *args);
		void toolsHolodeckInterceptsManagerMenuItemClicked(Object *sender, EventArgs *args);
		void viewFieldChooserMenuItemClicked(Object *sender, EventArgs *args);
		void viewProjectPaneMenuItemClicked(Object *sender, EventArgs *args);
		void viewResourcePaneMenuItemClicked(Object *sender, EventArgs *args);
		void viewLimitsPaneMenuItemClicked(Object *sender, EventArgs *args);
		void viewFileCorruptDetailsPaneMenuItemClicked(Object *sender, EventArgs *args);
		void viewNetworkMessageDetailsPaneMenuItemClicked(Object *sender, EventArgs *args);
		void viewPropertiesPaneMenuItemClicked(Object *sender, EventArgs *args);
		void viewHelpPaneMenuItemClicked(Object *sender, EventArgs *args);
		void helpHelpTopicsMenuItemClicked(Object *sender, EventArgs *args);
		void helpPDFHelpTopicsMenuItemClicked(Object *sender, EventArgs *args);
		void helpHolodeckSiteMenuItemClicked(Object *sender, EventArgs *args);
		void helpReportABugMenuItemClicked(Object *sender, EventArgs *args);
		void helpRegisterMenuItemClicked(Object *sender, EventArgs *args);
		void helpAboutHolodeckMenuItemClicked(Object *sender, EventArgs *args);

		bool OpenProjectDlg ();
		void LaunchSavedApp (SortedList *procInfo, SortedList *procFaults, SortedList *procLimits);
		void CloseProject();
		void UpdateApplicationMenuItems();
		void CloseAllNonDockablePanes();

		void toolBarButtonClicked(Object *sender, ToolBarButtonClickEventArgs *args);
		void refreshMenuItemAndButtonBarState();

		void onPauseProcess(unsigned int processID);
		void onResumeProcess(unsigned int processID);
		void onTerminateProcess(unsigned int processID);
		void onRemoveProcess(unsigned int processID);
		void onRestartProcess(unsigned int processID);
		void onHeatProcessTerminated(unsigned int processID);
		void onHeatProcessPaused(unsigned int processID);
		void onHeatProcessResumed(unsigned int processID);
		void onDisplayNewProcess(DisplayableDataNode *dataNode, Boolean makeTabActive);
		void onProcessPaused(unsigned int processID);
		void onPaneVisibilityChanged(Content *c, EventArgs *args);
		void onWindowClosing(Object *sender, CancelEventArgs *args);
		void onDeletePane(DisplayableDataNode *dataNode);
		void onSaveLogPane(DisplayableDataNode *dataNode);
		void onSaveNetworkLogPane(DisplayableDataNode *dataNode);
		void onSaveResourcePane();
		void onDoubleClick(DisplayableDataNode *dataNode);
		void onHelpPaneLinkClicked(Object *sender, String *url);
		void onStartCreateTestWizard(DisplayableDataNode *dataNode);
		void onStartModifyTestWizard(DisplayableDataNode *dataNode);
		void onStartNetworkCorruptionWizard(DisplayableDataNode *dataNode);
		void onStartModifyNetworkCorruptionWizard(DisplayableDataNode *dataNode);
		void onStartFileCorruptionWizard(DisplayableDataNode *dataNode);
		void onStartModifyFileCorruptionWizard(DisplayableDataNode *dataNode);
		void onStartCreateResourceTestWizard(ResourceTestDataNode *dataNode);
		void onStartModifyResourceTestWizard(ResourceTestDataNode *dataNode);
		void onCreateReport(DisplayableDataNode *dataNode);
		void onDeleteRecordedSession(DisplayableDataNode *dataNode);
		void onInitializeAppForProcessChaining(HoloScriptApplication *parentHoloScriptApp, HoloScriptApplication *newHoloScriptApp);
		void onProcessChainingAttachComplete(HoloScriptApplication *parentHoloScriptApp, HoloScriptApplication *newHoloScriptApp);
		void onThreadCreated(unsigned int pid, unsigned int tid);
		void onUrlLinkClicked(String *url);
		System::Windows::Forms::DialogResult onShowDialog(Form *formToDisplay);
		System::Windows::Forms::DialogResult onShowCommonDialog(CommonDialog *formToDisplay);
		void onCantCreateLogFile();
		void onResourcesAndLimitWindowSelectionChanged(Object *sender, EventArgs *e);
		void onResourcesAndLimitsTabControlCreated(Crownwood::Magic::Controls::TabControl *tc);
		void onLoggedNewThreadID(unsigned int processID, unsigned int threadID);
		void onShowFileCorruptionDetailsPane();
		void onShowNetworkCorruptionDetailsPane();

		void CCTestGenStopped();
		void StressTestGenStopped();
		void RecordedSessionReplayStart(DisplayableDataNode *dataNode);
		void RecordedSessionReplayStop(DisplayableDataNode *dataNode);
		void RecordedSessionStart(DisplayableDataNode *dataNode);
		void RecordedSessionStop(DisplayableDataNode *dataNode);
		void RecordedSessionReplayStarted(DisplayableDataNode *dataNode);
		void RecordedSessionReplayStopped(DisplayableDataNode *dataNode);
		void RecordedSessionStarted(DisplayableDataNode *dataNode);
		void RecordedSessionStopped(DisplayableDataNode *dataNode);
		void ModifyDataArray(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
		void onSaveReport(DisplayableDataNode *dataNode);
		void saveDockablePaneConfigs(Byte byteData[]);
		void loadDockablePaneConfigs();
		void resetPaneSettings();

		bool IsServiceInstalled(String *serviceName);
		void DisplayStartPage();
		bool runNewTestApplicationWizard();
		bool runNewProjectWizard(NewProjectWizard * defaultProjectWizard);
		bool NewTestApplicationWorker(NewProjectWizard *wizard);
		void NewTestApplicationThreadFunction();
		void ApplicationRestartThreadFunction();
		Crownwood::Magic::Controls::TabPage* GetTabPage(DisplayableDataNode *dataNode);
		void DisplayTabPage(DisplayableDataNode* dataNode, bool tabSelected);
		void onRestartProject(bool pauseOnStart);		//used for x-thread marshalling
		void RestartProject(bool pauseOnStart);
		void onSaveProject();
		void StressTestGenerationStart(GenerationType generationType, Intensity intensity);
		void CodeCoverageTestGenerationStart(Intensity intensity, bool restartProject, int timeout);
		void StressTestGenerationStop();
		void CodeCoverageTestGenerationStop();
		DisplayableDataNode *GetActiveNode(bool displayWarning);		//gets active pID and tID and returns in a datanode
		void DisplayTabPagesThreadFunction();
		void DisplayNewProcess(DisplayableDataNode *logDataNode, DisplayableDataNode *faultDataNode, bool makeTabActive);
		void CreateDataArrays(HoloScriptApplication *holoScriptApp);
		void onFocusChanged(DisplayableDataNode *dataNode);
		void onProjectPaneFocusChanged(DisplayableDataNode *dataNode);
		void DisplayTrialDisabledNotice(String *helpUrl);
		void DisplayTrialTestLimitExceededNotice();
		void ProcessRemoved(unsigned int processIDToRemove, bool removeTabPages);
		ArrayList * GetDataArraysForProcess(unsigned int processID, bool includeLogs);
		void InheritSettingsFromParentProcess(HoloScriptApplication *parentHoloScriptApp, HoloScriptApplication *newHoloScriptApp);
		String* getFocusedControlText(Control::ControlCollection * controls);
		String* getSelectedTextFromControl(Control * c);
		void onCopyToClipboard();
		void UpdateHolodeckWindowTitle(String * projectTitle);
	};
}
