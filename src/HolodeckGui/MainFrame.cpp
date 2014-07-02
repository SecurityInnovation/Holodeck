//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		MainFrame.cpp
//
// DESCRIPTION: Contains implementation for the class MainFrame
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 07 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "mainframe.h"
#include "CLRIntercept.h"

#using <NewIntercepts.dll>
using namespace AddNewIntercepts;

using namespace HolodeckRegistration;

//undef Win32 version of CreateDirectory to use .net version
#ifdef CreateDirectory
#undef CreateDirectory
#endif

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		MainFrame
	// Description: Constructor for the MainFrame class
	//
	// Parameters:
	//	notificationDelegate - the delegate to be invoked on need for a user
	//	 notification message
	//
	// Return Value: None
	//*************************************************************************
	MainFrame::MainFrame(StartupUserNotificationDelegate *notificationDelegate)
	{
		currentProject = new Project();
		stressTestGenerationDlg = new StressTestGenerationDlg();
		codeCoverageTestGenerationDlg = new CodeCoverageTestGenerationDlg();
		pendingServicesToStart = new Queue();
		createdThreadsTable = new Hashtable();
		threadCreatedMutex = new Mutex();

		stateAndSizeSaver = new FormStateAndSizeSaver();
		stateAndSizeSaver->Parent = this;
		stateAndSizeSaver->RegistryPath = String::Concat(ROOT_PATH, "\\", MAIN_FRAME_STATE_KEY);

		isRunning = true;
		pendingTabPagesToDisplay = new Queue();
		ThreadStart *pendingTabPagesThreadStart = new ThreadStart(this, DisplayTabPagesThreadFunction);
		displayTabPagesThread = new Threading::Thread(pendingTabPagesThreadStart);
		displayTabPagesThread->IsBackground = true;
		displayTabPagesThread->Start();

		dataEventRegistry = DataEventRegistry::getInstance();
		uiEventRegistry = UIEventRegistry::getInstance();

		DeleteReportHandler = new DeleteReportDelegate(this, onDeletePane);
		dataEventRegistry->OnDeleteReport += DeleteReportHandler;
		CreateReportHandler = new CreateReportDelegate(this, onCreateReport);
		dataEventRegistry->OnCreateReport += CreateReportHandler;
		ReportModifiedHandler = new ReportModifiedDelegate(this, ModifyDataArray);
		dataEventRegistry->OnReportModified += ReportModifiedHandler;
		SaveReportHandler = new SaveReportDelegate(this, onSaveReport);
		dataEventRegistry->OnSaveReport += SaveReportHandler;

		StartCreateTestWizardHandler = new StartCreateTestWizardDelegate(this, onStartCreateTestWizard);
		uiEventRegistry->OnStartCreateTestWizard += StartCreateTestWizardHandler;
		StartModifyTestWizardHandler = new StartModifyTestWizardDelegate(this, onStartModifyTestWizard);
		uiEventRegistry->OnStartModifyTestWizard += StartModifyTestWizardHandler;
		StartNetworkCorruptionWizardHandler = new StartNetworkCorruptionWizardDelegate(this, onStartNetworkCorruptionWizard);
		uiEventRegistry->OnStartNetworkCorruptionWizard += StartNetworkCorruptionWizardHandler;
		StartModifyNetworkCorruptionWizardHandler = new StartModifyNetworkCorruptionWizardDelegate(this, onStartModifyNetworkCorruptionWizard);
		uiEventRegistry->OnStartModifyNetworkCorruptionWizard += StartModifyNetworkCorruptionWizardHandler;
		StartFileCorruptionWizardHandler = new StartFileCorruptionWizardDelegate(this, onStartFileCorruptionWizard);
		uiEventRegistry->OnStartFileCorruptionWizard += StartFileCorruptionWizardHandler;
		StartModifyFileCorruptionWizardHandler = new StartModifyFileCorruptionWizardDelegate(this, onStartModifyFileCorruptionWizard);
		uiEventRegistry->OnStartModifyFileCorruptionWizard += StartModifyFileCorruptionWizardHandler;
		StartCreateResourceTestWizardHandler = new StartCreateResourceTestWizardDelegate(this, onStartCreateResourceTestWizard);
		uiEventRegistry->OnStartCreateResourceTestWizard += StartCreateResourceTestWizardHandler;
		StartModifyResourceTestWizardHandler = new StartModifyResourceTestWizardDelegate(this, onStartModifyResourceTestWizard);
		uiEventRegistry->OnStartModifyResourceTestWizard += StartModifyResourceTestWizardHandler;
		CopyToClipBoardHandler = new CopyToClipboardDelegate(this, onCopyToClipboard);
		uiEventRegistry->OnCopyToClipboard += CopyToClipBoardHandler;
		PauseProcessHandler = new PauseProcessDelegate(this, onPauseProcess);
		uiEventRegistry->OnPauseProcess += PauseProcessHandler;
		ProcessPausedHandler = new ProcessPausedDelegate(this, onProcessPaused);
		uiEventRegistry->OnProcessPaused += ProcessPausedHandler;
		ResumeProcessHandler = new ResumeProcessDelegate(this, onResumeProcess);
		uiEventRegistry->OnResumeProcess += ResumeProcessHandler;
		TerminateProcessHandler = new TerminateProcessDelegate(this, onTerminateProcess);
		uiEventRegistry->OnTerminateProcess += TerminateProcessHandler;
		RestartProcessHandler = new RestartProcessDelegate(this, onRestartProcess);
		uiEventRegistry->OnRestartProcess += RestartProcessHandler;
		RemoveProcessHandler = new RemoveProcessDelegate(this, onRemoveProcess);
		uiEventRegistry->OnRemoveProcess += RemoveProcessHandler;


		DeleteRecordedSessionHandler = new DeleteRecordedSessionDelegate(this, onDeleteRecordedSession);
		dataEventRegistry->OnDeleteRecordedSession += DeleteRecordedSessionHandler;
		CreateRecordedSessionHandler = new CreateRecordedSessionDelegate(this, RecordedSessionStart);
		dataEventRegistry->OnCreateRecordedSession += CreateRecordedSessionHandler;
		RecordedSessionModifiedHandler = new RecordedSessionModifiedDelegate(this, ModifyDataArray);
		dataEventRegistry->OnRecordedSessionModified += RecordedSessionModifiedHandler;

		DeleteLogPaneHandler = new DeleteLogPaneDelegate(this, onDeletePane);
		dataEventRegistry->OnDeleteLogPane += DeleteLogPaneHandler;
		SaveLogPaneHandler = new SaveLogPaneDelegate(this, onSaveLogPane);
		dataEventRegistry->OnSaveLogPane += SaveLogPaneHandler;

		DeleteNetworkLogPaneHandler = new DeleteNetworkLogPaneDelegate(this, onDeletePane);
		dataEventRegistry->OnDeleteNetworkLogPane += DeleteNetworkLogPaneHandler;
		SaveNetworkLogPaneHandler = new SaveNetworkLogPaneDelegate(this, onSaveNetworkLogPane);
		dataEventRegistry->OnSaveNetworkLogPane += SaveNetworkLogPaneHandler;

		SaveResourcePaneHandler = new SaveResourcePaneDelegate(this, onSaveResourcePane);
		dataEventRegistry->OnSaveResourcePane += SaveResourcePaneHandler;

		RestartProjectHandler = new RestartProjectDelegate(this, onRestartProject);
		dataEventRegistry->OnRestartProject += RestartProjectHandler;
		xRestartProjectHandler = new xRestartProjectDelegate(this, RestartProject);		//allows x-thread restarts
		SaveProjectHandler = new SaveProjectDelegate(this, onSaveProject);
		dataEventRegistry->OnSaveProject += SaveProjectHandler;
		displayNewProcessHandler = new DisplayNewProcessDelegate(this, onDisplayNewProcess);

		CCTestGenStoppedHandler = new CodeCoverageTestGenerationStoppedDelegate(this, CCTestGenStopped);
		dataEventRegistry->OnCodeCoverageTestGenerationStopped += CCTestGenStoppedHandler;
		
		StressTestGenStoppedHandler = new StressTestGenerationStoppedDelegate(this, StressTestGenStopped);
		dataEventRegistry->OnStressTestGenerationStopped += StressTestGenStoppedHandler;

		DoubleClickHandler = new DoubleClickDelegate(this, onDoubleClick);
		uiEventRegistry->OnDoubleClick += DoubleClickHandler;
		RecordedSessionReplayStartHandler = new RecordedSessionReplayStartDelegate(this, RecordedSessionReplayStart);
		uiEventRegistry->OnRecordedSessionReplayStart += RecordedSessionReplayStartHandler;
		RecordedSessionReplayStopHandler = new RecordedSessionReplayStopDelegate(this, RecordedSessionReplayStop);
		uiEventRegistry->OnRecordedSessionReplayStop += RecordedSessionReplayStopHandler;
		RecordedSessionStartHandler = new RecordedSessionStartDelegate(this, RecordedSessionStart);
		uiEventRegistry->OnRecordedSessionStart += RecordedSessionStartHandler;
		RecordedSessionStopHandler = new RecordedSessionStopDelegate(this, RecordedSessionStop);
		uiEventRegistry->OnRecordedSessionStop += RecordedSessionStopHandler;
		RecordedSessionReplayStartedHandler = new RecordedSessionReplayStartedDelegate(this, RecordedSessionReplayStarted);
		uiEventRegistry->OnRecordedSessionReplayStarted += RecordedSessionReplayStartedHandler;
		RecordedSessionReplayStoppedHandler = new RecordedSessionReplayStoppedDelegate(this, RecordedSessionReplayStopped);
		uiEventRegistry->OnRecordedSessionReplayStopped += RecordedSessionReplayStoppedHandler;
		RecordedSessionStartedHandler = new RecordedSessionStartedDelegate(this, RecordedSessionStarted);
		uiEventRegistry->OnRecordedSessionStarted += RecordedSessionStartedHandler;
		RecordedSessionStoppedHandler = new RecordedSessionStoppedDelegate(this, RecordedSessionStopped);
		uiEventRegistry->OnRecordedSessionStopped += RecordedSessionStoppedHandler;
		FocusChangedHandler = new FocusChangedDelegate(this, onFocusChanged);
		uiEventRegistry->OnFocusChanged += FocusChangedHandler;
		ProjectPaneFocusChangedHandler = new ProjectPaneFocusChangedDelegate(this, onProjectPaneFocusChanged);
		uiEventRegistry->OnProjectPaneFocusChanged += ProjectPaneFocusChangedHandler;
		URLClickedHandler = new URLClickedDelegate(this, onUrlLinkClicked);
		uiEventRegistry->OnURLClicked += URLClickedHandler;
		ShowDialogHandler = new ShowDialogDelegate(this, onShowDialog);
		uiEventRegistry->OnShowDialog += ShowDialogHandler;
		ShowCommonDialogHandler = new ShowCommonDialogDelegate(this, onShowCommonDialog);
		uiEventRegistry->OnShowCommonDialog += ShowCommonDialogHandler;

		ShowFileCorruptionDetailsPaneHandler = new ShowFileCorruptionDetailsPaneDelegate(this, onShowFileCorruptionDetailsPane);
		uiEventRegistry->OnShowFileCorruptionDetailsPane += ShowFileCorruptionDetailsPaneHandler;
		ShowNetworkCorruptionDetailsPaneHandler = new ShowNetworkCorruptionDetailsPaneDelegate(this, onShowNetworkCorruptionDetailsPane);
		uiEventRegistry->OnShowNetworkCorruptionDetailsPane += ShowNetworkCorruptionDetailsPaneHandler;

		wizardToLaunchAppFrom = NULL;
		launchProgressDlg = new LaunchProgressDialog();

		holoScriptRegistry = HoloScriptRegistry::getInstance();
		String *loadingDatabase;

		try
		{
			// initialize the function database
			loadingDatabase = "Native Function Database";
			String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
			InterceptedFunctionDB *db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
			if (!db->IsLoaded)
				db->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);

			loadingDatabase = ".NET Function Database";
			InterceptedFunctionDB *dotNetDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
			if (!dotNetDB->IsLoaded)
				dotNetDB->LoadDatabase(functionFolder, DOT_NET_FUNC_DB_FILE_NAME);

			// initialize the error codes database
			loadingDatabase = "Error Code Database";
			ErrorCodeDB *errorDb = ErrorCodeDB::GetInstance();
			if (!errorDb->IsLoaded)
				errorDb->LoadDatabase(functionFolder);

			// initialize the exception database
			loadingDatabase = "Exceptions Database";
			ExceptionDB *exceptionDb = ExceptionDB::GetInstance();
			if (!exceptionDb->IsLoaded)
				exceptionDb->LoadDatabase(functionFolder);

			//initialize the faults database
			loadingDatabase = "Faults Database";
			FaultFunctionDB *faultDB = FaultFunctionDB::GetInstance();
			if (!faultDB->IsLoaded)
				faultDB->LoadDatabase(functionFolder);
		}
		catch (Exception *e)
		{
			if (notificationDelegate)
				notificationDelegate->Invoke();

			String *msg = String::Concat("There is an error in the ", loadingDatabase, ":\n");
			msg = String::Concat(msg, e->get_Message(), "\n\nHolodeck cannot continue with this error, and will exit now.");
			UserNotification::ErrorNotify(msg);
			System::Diagnostics::Process * hd = System::Diagnostics::Process::GetCurrentProcess();
			hd->Kill();
		}

		InitializeUI();

		startPageUrl = String::Concat("file://", GuiSupport::RegistryHelper::getInstance()->GetInstallationPath(), "/");
		// show the getting started page
		if ((RegistrationMgr::GetInstance()->IsFeatureDisabledTrial()) || (RegistrationMgr::GetInstance()->IsTimeTrial()))
			startPageUrl = String::Concat(startPageUrl, NAG_SCREEN_HELP_FILE);
		else
			startPageUrl = String::Concat(startPageUrl, GETTING_STARTED_HELP_FILE);

		DisplayStartPage();
		startPage = nonDockablePaneManager->TabPages->get_Item(nonDockablePaneManager->TabPages->Count - 1);

		UpdateApplicationMenuItems();
	}

	//*************************************************************************
	// Method:		~MainFrame
	// Description: Destructor for the MainFrame class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	MainFrame::~MainFrame()
	{
	}

	//*************************************************************************
	// Method:		OnLoad
	// Description: called when the UI is loaded
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::OnLoad(EventArgs* e)
	{
		this->Activate();

		// call the base implementation
		Form::OnLoad(e);
	}

	//*************************************************************************
	// Method:		MakeMenuBar
	// Description: Initializes UI components for the menu bar
	//
	// Parameters:
	//	None
	//
	// Return Value: the MenuControl object representing the menu
	//*************************************************************************
	MenuControl *MainFrame::MakeMenuBar()
	{
		MenuCommand *fileNewProjectMenuItem = new MenuCommand("&New Project...", new EventHandler(this, fileNewProjectMenuItemClicked));
		fileNewProjectMenuItem->Shortcut = Shortcut::CtrlN;
		MenuCommand *fileNewTestApplicationMenuItem = new MenuCommand("New Test &Application...", new EventHandler(this, fileNewTestApplicationMenuItemClicked));
		MenuCommand *fileMenuSeparatorMenuItem1 = new MenuCommand("-");
		MenuCommand *fileOpenProjectMenuItem = new MenuCommand("&Open Project...", new EventHandler(this, fileOpenProjectMenuItemClicked));
		fileOpenProjectMenuItem->Shortcut = Shortcut::CtrlO;
		MenuCommand *fileCloseMenuItem = new MenuCommand("&Close", new EventHandler(this, fileCloseMenuItemClicked));
		MenuCommand *fileMenuSeparatorMenuItem2 = new MenuCommand("-");
		MenuCommand *fileExportLogToFileMenuItem = new MenuCommand("&Export Log to File...", new EventHandler(this, fileExportLogToFileMenuItemClicked));
		MenuCommand *fileExportResourcesToFileMenuItem = new MenuCommand("Export &Resources to File...", new EventHandler(this, fileExportResourcesToFileMenuItemClicked));
		MenuCommand *fileSaveProjectMenuItem = new MenuCommand("&Save Project", new EventHandler(this, fileSaveProjectMenuItemClicked));
		MenuCommand *fileSaveProjectAsMenuItem = new MenuCommand("Save &Project As...", new EventHandler(this, fileSaveProjectAsMenuItemClicked));
		MenuCommand *fileMenuSeparatorMenuItem3 = new MenuCommand("-");
		MenuCommand *filePrintLogMenuItem = new MenuCommand("Print &Log...", new EventHandler(this, filePrintLogMenuItemClicked));
		filePrintLogMenuItem->Shortcut = Shortcut::CtrlP;
		MenuCommand *fileMenuSeparatorMenuItem4 = new MenuCommand("-");
		MenuCommand *fileExitMenuItem = new MenuCommand("E&xit", new EventHandler(this, fileExitMenuItemClicked));

		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
		{
			filePrintLogMenuItem->Enabled = false;
		}

		MenuCommand *fileMenu = new MenuCommand("&File");
		fileMenu->MenuCommands->Add(fileNewProjectMenuItem);
		fileMenu->MenuCommands->Add(fileNewTestApplicationMenuItem);
		fileMenu->MenuCommands->Add(fileMenuSeparatorMenuItem1);
		fileMenu->MenuCommands->Add(fileOpenProjectMenuItem);
		fileMenu->MenuCommands->Add(fileCloseMenuItem);
		fileMenu->MenuCommands->Add(fileMenuSeparatorMenuItem2);
		fileMenu->MenuCommands->Add(fileExportLogToFileMenuItem);
		fileMenu->MenuCommands->Add(fileExportResourcesToFileMenuItem);
		fileMenu->MenuCommands->Add(fileSaveProjectMenuItem);
		fileMenu->MenuCommands->Add(fileSaveProjectAsMenuItem);
		fileMenu->MenuCommands->Add(fileMenuSeparatorMenuItem3);
		fileMenu->MenuCommands->Add(filePrintLogMenuItem);
		fileMenu->MenuCommands->Add(fileMenuSeparatorMenuItem4);
		fileMenu->MenuCommands->Add(fileExitMenuItem);

		applicationCopySelectedItemMenuItem = new MenuCommand("&Copy selected item", new EventHandler(this, applicationCopySelectedItemMenuItemClicked));
		applicationCopySelectedItemMenuItem->Shortcut = Shortcut::CtrlC;
		MenuCommand *applicationMenuSeparatorMenuItem0 = new MenuCommand("-");

		applicationPauseMenuItem = new MenuCommand("&Pause Application", new EventHandler(this, applicationPauseMenuItemClicked));
		applicationStopMenuItem = new MenuCommand("&Stop", new EventHandler(this, applicationStopMenuItemClicked));
		applicationRestartMenuItem = new MenuCommand("R&estart", new EventHandler(this, applicationRestartMenuItemClicked));
		applicationRemoveMenuItem = new MenuCommand("Re&move", new EventHandler(this, applicationRemoveMenuItemClicked));

		MenuCommand *applicationMenuSeparatorMenuItem1 = new MenuCommand("-");
		applicationAttachDebuggerMenuItem = new MenuCommand("Attach &Debugger and Log Exceptions", new EventHandler(this, applicationAttachDebuggerMenuItemClicked));
		applicationLogFirstChanceMenuItem = new MenuCommand("Log First Chance E&xceptions", new EventHandler(this, applicationLogFirstChanceMenuItemClicked));
		applicationLogFirstChanceMenuItem->Enabled = false;
		applicationInheritSettingsMenuItem = new MenuCommand("C&hild Process Inherits Settings", new EventHandler(this, applicationInheritSettingsMenuItemClicked));
		MenuCommand *applicationMenuSeparatorMenuItem2 = new MenuCommand("-");
		MenuCommand *applicationCreateATestMenuItem = new MenuCommand("Create A Scheduled &Test...", new EventHandler(this, applicationCreateATestMenuItemClicked));
		MenuCommand *applicationNewFaultMenuItem = new MenuCommand("Create A New &Fault", new EventHandler(this, applicationNewFaultMenuItemClicked));
		MenuCommand *applicationCreateAResourceTestMenuItem = new MenuCommand("Create A New &Resource Fault...", new EventHandler(this, applicationCreateAResourceTestMenuItemClicked));
		MenuCommand *applicationNewNetworkCorruptionMenuItem = new MenuCommand("Create A New &Network Fuzzing Fault...", new EventHandler(this, applicationNewNetworkCorruptionMenuItemClicked));
		MenuCommand *applicationNewFileCorruptionMenuItem = new MenuCommand("Create A New File Fuzzing Fault", new EventHandler(this, applicationNewFileCorruptionMenuItemClicked));

		MenuCommand *applicationMenu = new MenuCommand ("&Application");
		applicationMenu->MenuCommands->Add(applicationCopySelectedItemMenuItem);
		applicationMenu->MenuCommands->Add(applicationMenuSeparatorMenuItem0);
		applicationMenu->MenuCommands->Add(applicationPauseMenuItem);
		applicationMenu->MenuCommands->Add(applicationStopMenuItem);
		applicationMenu->MenuCommands->Add(applicationRestartMenuItem);
		applicationMenu->MenuCommands->Add(applicationRemoveMenuItem);
		applicationMenu->MenuCommands->Add(applicationMenuSeparatorMenuItem1);

		// Only show the attach debugger menu item if running on Windows XP (OS version 5.1) or higher, as
		// we can't detach the debugger on anything earlier
		OSVERSIONINFO osVersion;
		osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osVersion);
		if ((osVersion.dwMajorVersion > 5) || ((osVersion.dwMajorVersion == 5) && (osVersion.dwMinorVersion > 0)))
		{
			applicationMenu->MenuCommands->Add(applicationAttachDebuggerMenuItem);
			applicationMenu->MenuCommands->Add(applicationLogFirstChanceMenuItem);
		}

		applicationMenu->MenuCommands->Add(applicationInheritSettingsMenuItem);
		applicationMenu->MenuCommands->Add(applicationMenuSeparatorMenuItem2);
		applicationMenu->MenuCommands->Add(applicationCreateATestMenuItem);
		applicationMenu->MenuCommands->Add(applicationCreateAResourceTestMenuItem);
		applicationMenu->MenuCommands->Add(applicationNewFaultMenuItem);
		applicationMenu->MenuCommands->Add(applicationNewNetworkCorruptionMenuItem);
		applicationMenu->MenuCommands->Add(applicationNewFileCorruptionMenuItem);

		sessionCodeCoverageTestGeneratorMenuItem = new MenuCommand(codeCoverageMenuItemText, new EventHandler(this, sessionCodeCoverageTestGeneratorMenuItemClicked));
		sessionStressTestGeneratorMenuItem = new MenuCommand(stressMenuItemText, new EventHandler(this, sessionStressTestGeneratorMenuItemClicked));
		sessionRecordSessionMenuItem = new MenuCommand(recordMenuItemText, new EventHandler(this, sessionRecordSessionMenuItemClicked));
		sessionReplaySessionMenuItem = new MenuCommand(replayMenuItemText, new EventHandler(this, sessionReplaySessionMenuItemClicked));
		MenuCommand *sessionMenuSeparatorMenuItem1 = new MenuCommand("-");
		sessionPerThreadMenuItem = new MenuCommand("Per &Thread", new EventHandler(this, sessionPerThreadMenuItemClicked));
		MenuCommand *sessionMenuSeparatorMenuItem2 = new MenuCommand("-");
		MenuCommand *sessionCreateReportMenuItem = new MenuCommand("Create a &Holodeck Report", new EventHandler(this, sessionCreateReportMenuItemClicked));

		MenuCommand *sessionMenu = new MenuCommand("&Session");
		sessionMenu->MenuCommands->Add(sessionCodeCoverageTestGeneratorMenuItem);
		sessionMenu->MenuCommands->Add(sessionStressTestGeneratorMenuItem);
		sessionMenu->MenuCommands->Add(sessionRecordSessionMenuItem);
		sessionMenu->MenuCommands->Add(sessionReplaySessionMenuItem);
		sessionMenu->MenuCommands->Add(sessionMenuSeparatorMenuItem1);
		sessionMenu->MenuCommands->Add(sessionPerThreadMenuItem);
		//sessionMenu->MenuCommands->Add(sessionMenuSeparatorMenuItem2);
		//sessionMenu->MenuCommands->Add(sessionCreateReportMenuItem);

		MenuCommand *logFindMenuItem = new MenuCommand("&Find...", new EventHandler(this, logFindMenuItemClicked));
		logFindMenuItem->Shortcut = Shortcut::CtrlF;
		MenuCommand *logFindNextMenuItem = new MenuCommand("Find &Next", new EventHandler(this, logFindNextMenuItemClicked));
		logFindNextMenuItem->Shortcut = Shortcut::F3;
		MenuCommand *logMenuSeparatorMenuItem1 = new MenuCommand("-");
		MenuCommand *logFilterMenuItem = new MenuCommand("Functions &Logged...", new EventHandler(this, logFilterMenuItemClicked));
		
		MenuCommand *logMenu = new MenuCommand("&Log");
		logMenu->MenuCommands->Add(logFindMenuItem);
		logMenu->MenuCommands->Add(logFindNextMenuItem);
		logMenu->MenuCommands->Add(logMenuSeparatorMenuItem1);
		logMenu->MenuCommands->Add(logFilterMenuItem);

		MenuCommand *toolsCustomWrapperLibraryMenuItem = new MenuCommand("&Create a Custom Test Project", new EventHandler(this, toolsCustomWrapperLibraryMenuItemClicked));
		MenuCommand *toolsAddHolodeckInterceptsMenuItem = new MenuCommand("&Add Holodeck Intercepts", new EventHandler (this, toolsAddHolodeckInterceptsMenuItemClicked));
		MenuCommand *toolsHOlodeckInterceptsManagerMenuItem = new MenuCommand("Holodeck Intercepts &Manager", new EventHandler (this, toolsHolodeckInterceptsManagerMenuItemClicked));

		MenuCommand *toolsMenu = new MenuCommand("&Tools");
		toolsMenu->MenuCommands->Add(toolsCustomWrapperLibraryMenuItem);
		toolsMenu->MenuCommands->Add(toolsAddHolodeckInterceptsMenuItem);
		toolsMenu->MenuCommands->Add(toolsHOlodeckInterceptsManagerMenuItem);

		viewProjectPaneMenuItem = new MenuCommand("&Project Pane", new EventHandler(this, viewProjectPaneMenuItemClicked));
		viewResourcePaneMenuItem = new MenuCommand("&Resource Pane", new EventHandler(this, viewResourcePaneMenuItemClicked));
		viewLimitsPaneMenuItem = new MenuCommand("&Limits Pane", new EventHandler(this, viewLimitsPaneMenuItemClicked));
		viewFileCorruptDetailsPaneMenuItem = new MenuCommand("&File Fuzzing Details Pane", new EventHandler(this, viewFileCorruptDetailsPaneMenuItemClicked));
		viewNetworkMessageDetailsPaneMenuItem = new MenuCommand("&Network Message Details Pane", new EventHandler(this, viewNetworkMessageDetailsPaneMenuItemClicked));
		viewPropertiesPaneMenuItem = new MenuCommand("Pr&operties Pane", new EventHandler(this, viewPropertiesPaneMenuItemClicked));
		viewHelpPaneMenuItem = new MenuCommand("&Help Pane", new EventHandler(this, viewHelpPaneMenuItemClicked));
		MenuCommand *viewMenuSeparatorItem1 = new MenuCommand("-");
		MenuCommand *viewFieldChooserMenuItem = new MenuCommand("Field &Chooser", new EventHandler(this, viewFieldChooserMenuItemClicked));

		MenuCommand *viewMenu = new MenuCommand("&View");
		viewMenu->MenuCommands->Add(viewProjectPaneMenuItem);
		viewMenu->MenuCommands->Add(viewResourcePaneMenuItem);
		viewMenu->MenuCommands->Add(viewLimitsPaneMenuItem);
		viewMenu->MenuCommands->Add(viewFileCorruptDetailsPaneMenuItem);
		viewMenu->MenuCommands->Add(viewNetworkMessageDetailsPaneMenuItem);
		viewMenu->MenuCommands->Add(viewPropertiesPaneMenuItem);
		viewMenu->MenuCommands->Add(viewHelpPaneMenuItem);
		viewMenu->MenuCommands->Add(viewMenuSeparatorItem1);
		viewMenu->MenuCommands->Add(viewFieldChooserMenuItem);

		MenuCommand *helpHelpTopicsMenuItem = new MenuCommand("Help &Topics...", new EventHandler(this, helpHelpTopicsMenuItemClicked));
		MenuCommand *helpPDFHelpTopicsMenuItem = new MenuCommand("&PDF Help Topics...", new EventHandler(this, helpPDFHelpTopicsMenuItemClicked));
		MenuCommand *helpMenuSeparatorMenuItem1 = new MenuCommand("-");
		MenuCommand *helpHolodeckSiteMenuItem = new MenuCommand("&Holodeck Site...", new EventHandler(this, helpHolodeckSiteMenuItemClicked));
		MenuCommand *helpReportABugMenuItem = new MenuCommand("&Report a Bug...", new EventHandler(this, helpReportABugMenuItemClicked));
		MenuCommand *helpMenuSeparatorMenuItem2 = new MenuCommand("-");
		MenuCommand *helpRegisterMenuItem = new MenuCommand("R&egister Holodeck...", new EventHandler(this, helpRegisterMenuItemClicked));
		MenuCommand *helpMenuSeparatorMenuItem3 = new MenuCommand("-");
		MenuCommand *helpAboutHolodeckMenuItem = new MenuCommand("&About Holodeck...", new EventHandler(this, helpAboutHolodeckMenuItemClicked));

		MenuCommand *helpMenu = new MenuCommand("&Help");
		helpMenu->MenuCommands->Add(helpHelpTopicsMenuItem);
		helpMenu->MenuCommands->Add(helpPDFHelpTopicsMenuItem);
		helpMenu->MenuCommands->Add(helpMenuSeparatorMenuItem1);
		helpMenu->MenuCommands->Add(helpHolodeckSiteMenuItem);
		helpMenu->MenuCommands->Add(helpReportABugMenuItem);
		helpMenu->MenuCommands->Add(helpMenuSeparatorMenuItem2);
		helpMenu->MenuCommands->Add(helpRegisterMenuItem);
		helpMenu->MenuCommands->Add(helpMenuSeparatorMenuItem3);
		helpMenu->MenuCommands->Add(helpAboutHolodeckMenuItem);

		MenuControl *mainMenu = new MenuControl();
		mainMenu->Style = this->visualStyle;
		mainMenu->MultiLine = false;
		mainMenu->Dock = DockStyle::Top;

		mainMenu->MenuCommands->Add(fileMenu);
		mainMenu->MenuCommands->Add(sessionMenu);
		mainMenu->MenuCommands->Add(applicationMenu);
		mainMenu->MenuCommands->Add(logMenu);
		mainMenu->MenuCommands->Add(toolsMenu);
		mainMenu->MenuCommands->Add(viewMenu);
		mainMenu->MenuCommands->Add(helpMenu);

		return mainMenu;
	}

	//*************************************************************************
	// Method:		MakeButtonBar
	// Description: Initializes the UI components for the toolbar
	//
	// Parameters:
	//	None
	//
	// Return Value: the object representing the button bar
	//*************************************************************************
	ToolBar *MainFrame::MakeButtonBar()
	{
		ToolBar *buttonBar = new ToolBar();
		buttonBar->Appearance = ToolBarAppearance::Flat;
		buttonBar->ShowToolTips = true;
		buttonBar->BorderStyle = BorderStyle::None;
		buttonBar->Dock = DockStyle::Top;
		buttonBar->ImageList = new ImageList();
		buttonBar->ButtonClick += new ToolBarButtonClickEventHandler(this, toolBarButtonClicked);

		buttonBar->ImageList = ImageListManager::getInstance()->ButtonBarImageList;

		ToolBarButton *newProjectButton = new ToolBarButton();
		newProjectButton->ToolTipText = "New Project";
		newProjectButton->ImageIndex = NewProjectType;

		ToolBarButton *openProjectButton = new ToolBarButton();
		openProjectButton->ToolTipText = "Open Project";
		openProjectButton->ImageIndex = OpenProjectType;

		ToolBarButton *saveProjectButton = new ToolBarButton();
		saveProjectButton->ToolTipText = "Save Project";
		saveProjectButton->ImageIndex = SaveProjectType;

		ToolBarButton *toolBarSeparator1 = new ToolBarButton();
		toolBarSeparator1->Style = ToolBarButtonStyle::Separator;

		ToolBarButton *newTestApplicationButton = new ToolBarButton();
		newTestApplicationButton->ToolTipText = "New Test Application";
		newTestApplicationButton->ImageIndex = NewTestAppType;

		ToolBarButton *createScheduledTestButton = new ToolBarButton();
		createScheduledTestButton->ToolTipText = "New Scheduled Test";
		createScheduledTestButton->ImageIndex = CreateTestType;

		ToolBarButton *createResourceTestButton = new ToolBarButton();
		createResourceTestButton->ToolTipText = "New Resource Fault";
		createResourceTestButton->ImageIndex = CreateResourceTestType;

		ToolBarButton *newFaultButton = new ToolBarButton();
		newFaultButton->ToolTipText = "New Fault";
		newFaultButton->ImageIndex = NewFaultType;

		ToolBarButton *createFileCorruptionFaultButton = new ToolBarButton();
		createFileCorruptionFaultButton->ToolTipText = "New File Fuzzing Fault";
		createFileCorruptionFaultButton->ImageIndex = FileCorruptionFaultType;

		ToolBarButton *createNetworkCorruptionFaultButton = new ToolBarButton();
		createNetworkCorruptionFaultButton->ToolTipText = "New Network Fuzzing Fault";
		createNetworkCorruptionFaultButton->ImageIndex = NetworkCorruptionFaultType;

		ToolBarButton *toolBarSeparator2 = new ToolBarButton();
		toolBarSeparator2->Style = ToolBarButtonStyle::Separator;

		ToolBarButton *createReportButton = new ToolBarButton();
		createReportButton->ToolTipText = "Create A Report";
		createReportButton->ImageIndex = CreateReportType;

		ToolBarButton *toolBarSeparator3 = new ToolBarButton();
		toolBarSeparator3->Style = ToolBarButtonStyle::Separator;

		projectPaneButton = new ToolBarButton();
		projectPaneButton->ToolTipText = "Show/Hide the Project Pane";
		projectPaneButton->ImageIndex = ProjectPaneType;
		projectPaneButton->Style = ToolBarButtonStyle::ToggleButton;
		projectPaneButton->Pushed = true;

		resourcePaneButton = new ToolBarButton();
		resourcePaneButton->ToolTipText = "Show/Hide the Resource Pane";
		resourcePaneButton->ImageIndex = ResourcePaneType;
		resourcePaneButton->Style = ToolBarButtonStyle::ToggleButton;
		resourcePaneButton->Pushed = true;

		fileCorruptDetailsPaneButton = new ToolBarButton();
		fileCorruptDetailsPaneButton->ToolTipText = "Show/Hide the File Fuzzing Details Pane";
		fileCorruptDetailsPaneButton->ImageIndex = FileCorruptionDetailsPaneType;
		fileCorruptDetailsPaneButton->Style = ToolBarButtonStyle::ToggleButton;
		fileCorruptDetailsPaneButton->Pushed = true;

		networkMessageDetailsPaneButton = new ToolBarButton();
		networkMessageDetailsPaneButton->ToolTipText = "Show/Hide the Network Message Details Pane";
		networkMessageDetailsPaneButton->ImageIndex = NetworkMessageDetailsPaneType;
		networkMessageDetailsPaneButton->Style = ToolBarButtonStyle::ToggleButton;
		networkMessageDetailsPaneButton->Pushed = true;

		limitsPaneButton = new ToolBarButton();
		limitsPaneButton->ToolTipText = "Show/Hide the Limits Pane";
		limitsPaneButton->ImageIndex = LimitsPaneType;
		limitsPaneButton->Style = ToolBarButtonStyle::ToggleButton;
		limitsPaneButton->Pushed = true;

		propertiesPaneButton = new ToolBarButton();
		propertiesPaneButton->ToolTipText = "Show/Hide the Properties Pane";
		propertiesPaneButton->ImageIndex = PropertiesPaneType;
		propertiesPaneButton->Style = ToolBarButtonStyle::ToggleButton;
		propertiesPaneButton->Pushed = true;

		helpPaneButton = new ToolBarButton();
		helpPaneButton->ToolTipText = "Show/Hide the Help Pane";
		helpPaneButton->ImageIndex = DynamicHelpType;
		helpPaneButton->Style = ToolBarButtonStyle::ToggleButton;
		helpPaneButton->Pushed = true;

		buttonBar->Buttons->Add(newProjectButton);
		buttonBar->Buttons->Add(openProjectButton);
		buttonBar->Buttons->Add(saveProjectButton);
		buttonBar->Buttons->Add(newTestApplicationButton);
		buttonBar->Buttons->Add(toolBarSeparator1);
		buttonBar->Buttons->Add(createScheduledTestButton);
		buttonBar->Buttons->Add(createResourceTestButton);
		buttonBar->Buttons->Add(newFaultButton);
		buttonBar->Buttons->Add(createFileCorruptionFaultButton);
		buttonBar->Buttons->Add(createNetworkCorruptionFaultButton);
		buttonBar->Buttons->Add(toolBarSeparator2);
		buttonBar->Buttons->Add(createReportButton);
		buttonBar->Buttons->Add(toolBarSeparator3);
		buttonBar->Buttons->Add(projectPaneButton);
		buttonBar->Buttons->Add(resourcePaneButton);
		buttonBar->Buttons->Add(limitsPaneButton);
		buttonBar->Buttons->Add(fileCorruptDetailsPaneButton);
		buttonBar->Buttons->Add(networkMessageDetailsPaneButton);
		buttonBar->Buttons->Add(propertiesPaneButton);
		buttonBar->Buttons->Add(helpPaneButton);

		return buttonBar;
	}

	//*************************************************************************
	// Method:		InitializeUI
	// Description: Initializes all UI components for the class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::InitializeUI()
	{
		SiUtils::Registry registry(ROOT_KEY);
		if (registry.OpenKey (INSTALLED_ROOT_KEY, ROOT_PATH, KEY_ALL_ACCESS))
		{
			if (!registry.ValueExists (PANE_SETTINGS_VALUE_NAME))
				resetPaneSettings();
		}
		registry.Close();

		String *keyString;

		this->visualStyle = VisualStyle::IDE;
		this->dockManager = new DockingManager(this, this->visualStyle);
		ImageListManager *imageMgr = ImageListManager::getInstance();

		recordMenuItemText = new String("&Record Session");
		replayMenuItemText = new String("Re&play Session");
		codeCoverageMenuItemText = new String("&Code Coverage Test Generator...");
		stressMenuItemText = new String("&Stress Test Generator...");
		MenuControl *mainMenu = MakeMenuBar();
		buttonBar = MakeButtonBar();

		nonDockablePaneManager = new NonDockablePaneManager();

		// Reduce the amount of flicker that occurs when windows are redocked within
        // the container. As this prevents unsightly backcolors being drawn in the
        // WM_ERASEBACKGROUND that seems to occur.
		SetStyle(ControlStyles::DoubleBuffer, true);

		// order matters here.  The layout manager puts them in backwards so if you add mainMenu
		// first, then buttonBar the menu will be below the button bar.
		this->Controls->Add(nonDockablePaneManager);
		this->Controls->Add(buttonBar);
		this->Controls->Add(mainMenu);

		// add the docking panes
		projectPaneData = new ProjectPaneDataArray();
		keyString = String::Concat(projectPaneData->GetType()->Name, "0");
		keyString = String::Concat(keyString, "0");
		if (!currentProject->ContainsDataArrayKey(keyString))
			currentProject->AddDataArray(keyString, projectPaneData);

		projectPane = dockManager->Contents->Add(new ProjectPane(projectPaneData), "Project", NULL, NULL);
		projectPane->ImageList = imageMgr->ButtonBarImageList;
		projectPane->ImageIndex = ButtonBarTypes::ProjectPaneType;
		WindowContent *projectPaneWindowContent = dynamic_cast<WindowContent *>(dockManager->AddContentWithState(projectPane, State::DockRight));
		
		propertiesPane = dockManager->Contents->Add(new PropertiesPane(), "Properties", NULL, NULL);
		propertiesPane->ImageList = imageMgr->ButtonBarImageList;
		propertiesPane->ImageIndex = ButtonBarTypes::PropertiesPaneType;

		dynamicHelpPane = new HelpPane();
		HelpPaneLinkClickedHandler = new HelpPaneLinkClickedDelegate(this, onHelpPaneLinkClicked);
		dynamicHelpPane->OnHelpPaneLinkClicked += HelpPaneLinkClickedHandler;
		helpPane = dockManager->Contents->Add(dynamicHelpPane, "Dynamic Help", NULL, NULL);
		helpPane->ImageList = imageMgr->ButtonBarImageList;
		helpPane->ImageIndex = ButtonBarTypes::DynamicHelpType;
		
		WindowContent *propertiesAndHelpWindow = dynamic_cast<WindowContent *>(dockManager->AddContentWithState(helpPane, State::DockRight));
		dockManager->AddContentToWindowContent(propertiesPane, propertiesAndHelpWindow);
		dockManager->AddContentToZone(projectPane, propertiesAndHelpWindow->ParentZone, 0);

		limPane = new LimitPane();
		limitPane = dockManager->Contents->Add(limPane, "Limits", NULL, NULL);
		limitPane->ImageList = imageMgr->ButtonBarImageList;
		limitPane->ImageIndex = ButtonBarTypes::LimitsPaneType;

		resPane = new ResourcePane();
		resourcePane = dockManager->Contents->Add(resPane, "Resources", NULL, NULL);
		resourcePane->ImageList = imageMgr->ButtonBarImageList;
		resourcePane->ImageIndex = ButtonBarTypes::ResourcePaneType;

		fcDetailsPane = new FileCorruptionDetailsPane();
		fileCorruptDetailsPane = dockManager->Contents->Add(fcDetailsPane, "File Fuzzing Details", NULL, NULL);
		fileCorruptDetailsPane->ImageList = imageMgr->ButtonBarImageList;
		fileCorruptDetailsPane->ImageIndex = ButtonBarTypes::FileCorruptionDetailsPaneType;

		nmDetailsPane = new NetworkMessageDetailsPane();
		networkMessageDetailsPane = dockManager->Contents->Add(nmDetailsPane, "Network Message Details", NULL, NULL);
		networkMessageDetailsPane->ImageList = imageMgr->ButtonBarImageList;
		networkMessageDetailsPane->ImageIndex = ButtonBarTypes::NetworkMessageDetailsPaneType;

		WindowContentTabbed *resourcesAndLimitWindow = dynamic_cast <WindowContentTabbed*> (dockManager->AddContentWithState(limitPane, State::DockBottom));
		dockManager->AddContentToWindowContent(resourcePane, resourcesAndLimitWindow);
		dockManager->AddContentToWindowContent(fileCorruptDetailsPane, resourcesAndLimitWindow);
		dockManager->AddContentToWindowContent(networkMessageDetailsPane, resourcesAndLimitWindow);
		dockManager->InnerControl = nonDockablePaneManager;
		dockManager->OuterControl = buttonBar;

		dockManager->TabControlCreated += new DockingManager::TabControlCreatedHandler(this, onResourcesAndLimitsTabControlCreated);

		// set the delegates for open and closing the dockable panes
		DockingManager::ContentHandler *visibilityHandler = new DockingManager::ContentHandler(this, onPaneVisibilityChanged);
		dockManager->ContentHidden += visibilityHandler;
		dockManager->ContentShown += visibilityHandler;

		// ensure all panes have same threading option set
		if (uiEventRegistry->OnPerThreadChanged)
		{
			currentProject->IsInPerThreadMode = sessionPerThreadMenuItem->Checked;
			uiEventRegistry->OnPerThreadChanged->Invoke(currentProject->IsInPerThreadMode);
		}

		// show the panes
		dockManager->ShowContent(projectPane);
		dockManager->ShowContent(propertiesPane);
		dockManager->ShowContent(resourcePane);
		dockManager->ShowContent(limitPane);
		dockManager->ShowContent(helpPane);
		dockManager->ShowContent(fileCorruptDetailsPane);
		dockManager->ShowContent(networkMessageDetailsPane);

		this->HelpButton = true;

		UpdateHolodeckWindowTitle("");

		this->ClientSize = *(__nogc new System::Drawing::Size(1000, 625));
		this->Closing += new CancelEventHandler(this, onWindowClosing);

		loadDockablePaneConfigs();

		if (helpPane->Visible)
		{
			dynamicHelpPane->DisplayUrls = true;
		}
		else
		{
			dynamicHelpPane->DisplayUrls = false;
		}

		this->refreshMenuItemAndButtonBarState();

		this->Icon = imageMgr->AppIcon;
	}

	//****************************************************************************
	// START OF MENU ITEM CLICK EVENT HANDLERS
	// For all functions below:
	// Description: Handles a menu item click event
	//
	// Parameters:
	//	sender - the object that sent the event
	//	args - the event arguments representing the click event
	//
	// Return Value: None
	//*************************************************************************
	//*************************************************************************
	// Method:		fileNewProjectMenuItemClicked
	//*************************************************************************
	void MainFrame::fileNewProjectMenuItemClicked(Object *sender, EventArgs *args)
	{
		int result;

		if (IsProjectActive)
		{
			//don't pop the dlg if in silent mode
			if (!silentMode)
			{
				result = MessageBox(NULL, "Would you like to save your project?", "Holodeck", MB_YESNOCANCEL | MB_TOPMOST | MB_TASKMODAL);

				//If want to save, then save the project
				if (result == IDYES)
				{
					fileSaveProjectMenuItemClicked(sender, args);
				}
			}

			//Now close the previous project and start a new one
			if (result == IDNO || result == IDYES || silentMode)
			{
				CloseProject();
				runNewProjectWizard(NULL);
			}
		}
		else
		{
			runNewProjectWizard(NULL);
		}

	}
	//*************************************************************************
	// Method:		fileNewTestApplicationMenuItemClicked
	//*************************************************************************
	void MainFrame::fileNewTestApplicationMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
			DisplayTrialDisabledNotice(NEW_TEST_APP_HELP_FILE);
		else
		{
			if (IsProjectActive)
				runNewTestApplicationWizard();
			else
				UserNotification::InfoNotify("Please open a project first");
		}
	}
	//*************************************************************************
	// Method:		fileOpenProjectMenuItemClicked
	//*************************************************************************
	void MainFrame::fileOpenProjectMenuItemClicked(Object *sender, EventArgs *args)
	{
		int result;

		if (IsProjectActive)
		{
			//don't pop the dlg if in silent mode
			if (!silentMode)
			{
				result = MessageBox(NULL, "Would you like to save your project?", "Holodeck", MB_YESNOCANCEL | MB_TOPMOST | MB_TASKMODAL);

				//If want to save, then save the project
				if (result == IDYES)
				{
					fileSaveProjectMenuItemClicked(sender, args);				
				}
			}

			//Now close the previous project and start a new one
			if (result == IDNO || result == IDYES || silentMode)
			{
				CloseProject();
				this->OpenProjectDlg();
			}
		}
		else
			this->OpenProjectDlg();
	}
	//*************************************************************************
	// Method:		fileCloseMenuItemClicked
	//*************************************************************************
	void MainFrame::fileCloseMenuItemClicked(Object *sender, EventArgs *args)
	{
		int result;

		if (IsProjectActive)
		{
			//don't pop message if in silent mode
			if (!silentMode)
			{
				result = MessageBox(NULL, "Would you like to save your project?", "Holodeck", MB_YESNOCANCEL | MB_TOPMOST | MB_TASKMODAL);

				//If want to save, then save the project
				if (result == IDYES)
				{
					fileSaveProjectMenuItemClicked(sender, args);
				}
			}

			if (result == IDYES || result == IDNO || silentMode)
			{
				CloseProject();
			}
		}

	}
	//*************************************************************************
	// Method:		fileExportLogToFileMenuItemClicked
	//*************************************************************************
	void MainFrame::fileExportLogToFileMenuItemClicked(Object *sender, EventArgs * args)
	{
		if (IsProjectActive)
		{
			Crownwood::Magic::Controls::TabPage *tabPage = nonDockablePaneManager->SelectedTab;
			Object *pane = NULL;
			if (tabPage)
				pane = tabPage->Control;

			if (!tabPage || !pane)
			{
				UserNotification::ErrorNotify("Please select the API or Network log pane which you wish to export");
				return;
			}

			if (pane->GetType()->ToString()->Equals (S"HolodeckGui.LogPane"))
			{
				DisplayableDataNode *dataNode = GetActiveNode(true);
				
				if (!dataNode)
					return;

				dataNode->Type = UIData::ActiveLogPane;
				onSaveLogPane(dataNode);
			}
			else if (pane->GetType()->ToString()->Equals (S"HolodeckGui.NetworkLogPane"))
			{
				DisplayableDataNode *dataNode = GetActiveNode(true);

				if (!dataNode)
					return;

				dataNode->Type = UIData::ActiveNetworkLogPane;
				onSaveNetworkLogPane(dataNode);
			}
			else
			{
				UserNotification::ErrorNotify("Please select the API or Network log pane which you wish to export");
				return;
			}
		}
		else
			UserNotification::ErrorNotify("Please open a project first");
	}
	//*************************************************************************
	// Method:		fileExportResourcesToFileMenuItemClicked
	//*************************************************************************
	void MainFrame::fileExportResourcesToFileMenuItemClicked (Object *sender, EventArgs *args)
	{
		if (IsProjectActive)
		{
			if (dataEventRegistry->OnSaveResourcePane)
				dataEventRegistry->OnSaveResourcePane->Invoke();
		}
		else
			UserNotification::ErrorNotify("Please open a project first");
	}
	//*************************************************************************
	// Method:		fileSaveProjectMenuItemClicked
	//*************************************************************************
	void MainFrame::fileSaveProjectMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (IsProjectActive)
		{
			RegistryHelper * regHelper = RegistryHelper::getInstance();
			//If the project was started from the commandline without
			//an HDP name then currentProject->FullPath will be NULL
			//and we should pass control to the Save As handler
			if (currentProject->FullPath == NULL)
			{
				this->fileSaveProjectAsMenuItemClicked(sender, args);
				return;
			}
			else
			{
				ProjectFileWriter *writer = new ProjectFileWriter();
				if (!(writer->SaveProject(currentProject, currentProject->FullPath)))
					UserNotification::ErrorNotify("There was an error, project could not be saved");
				else
					regHelper->SetLastProjectSavePath (System::IO::Path::GetDirectoryName (currentProject->FullPath));
			}
		}
		else 
			UserNotification::ErrorNotify("Please open a project first");
	}
	//*************************************************************************
	// Method:		fileSaveProjectAsMenuItemClicked
	//*************************************************************************
	void MainFrame::fileSaveProjectAsMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (IsProjectActive)
		{
			System::Windows::Forms::SaveFileDialog *saveDialog = new System::Windows::Forms::SaveFileDialog();
			RegistryHelper * regHelper = RegistryHelper::getInstance();
		
			saveDialog->AddExtension = true;
			saveDialog->Filter = "Holodeck projects(*.hdp)|*.hdp";
			saveDialog->DefaultExt = "*.hdp";
			saveDialog->CheckPathExists = true;
			saveDialog->InitialDirectory = currentProject->Folder;

			String * oldFolder = currentProject->Folder;
			String * oldName = currentProject->Name;
			String * oldFullPath = currentProject->FullPath;
			if (saveDialog->ShowDialog (this) == DialogResult::OK)
			{
				//If saving to the current project file, then call normal Save
				if (String::Compare (saveDialog->FileName, oldFullPath, true) == 0)
				{
					this->fileSaveProjectMenuItemClicked (sender, args);
					return;
				}
				else
				{
					//delete old files if they exist
					try
					{
						String * useName = saveDialog->FileName->Substring (0, saveDialog->FileName->Length - 4);

						if (Directory::Exists (String::Concat (useName, "_CorruptedFiles")))
							Directory::Delete (String::Concat (useName, "_CorruptedFiles"), true);

						if (Directory::Exists (String::Concat (useName, "_Logs")))
							Directory::Delete (String::Concat (useName, "_Logs"), true);

						if (Directory::Exists (String::Concat (useName, "_Exceptions")))
							Directory::Delete (String::Concat (useName, "_Exceptions"), true);
					}
					catch (...)
					{
						UserNotification::ErrorNotify("Could not overwrite the existing project files or directories, please make sure they are not being used.");
						return;
					}
				}

				regHelper->SetLastProjectSavePath (System::IO::Path::GetDirectoryName (saveDialog->FileName));
				currentProject->Folder = System::IO::Path::GetDirectoryName(saveDialog->FileName);
				currentProject->Name = System::IO::Path::GetFileName(saveDialog->FileName);
				int index = currentProject->Name->IndexOf(".hdp");
				if (index > 0)
					currentProject->Title = currentProject->Name->Substring(0, index);
				else
					currentProject->Title = currentProject->Name;
				currentProject->FullPath = String::Concat(currentProject->Folder, "\\", currentProject->Name);

				//transfer report recorded session, and log folders to new project folder
				ProjectFileWriter *writer = new ProjectFileWriter();
				writer->TransferReportsAndRecordedSessions(currentProject);
				writer->TransferSavedLogs(currentProject);
				writer->TransferSavedCorruptedFiles(currentProject);
				writer->TransferSavedExceptions(currentProject);
				if (!(writer->SaveProject(currentProject, currentProject->FullPath)))
				{
					//revert to old project
					currentProject->Folder = oldFolder;
					currentProject->Name = oldName;
					currentProject->FullPath = oldFullPath;

					//Display error dialog
					UserNotification::ErrorNotify("There was an error saving the project. Please make sure you have write access to the specified location");
				}
				else
					this->UpdateHolodeckWindowTitle (currentProject->Title);
			}
		}
	}
	
	//*************************************************************************
	// Method:		filePrintLogMenuItemClicked
	//*************************************************************************
	void MainFrame::filePrintLogMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (nonDockablePaneManager->SelectedTab != NULL)
		{
			Object * printjob = nonDockablePaneManager->SelectedTab->Control;

			if (printjob->GetType()->ToString()->Equals (new String("HolodeckGui.LogPane"))||
				(printjob->GetType()->ToString()->Equals (new String("HolodeckGui.NetworkLogPane"))))
			{
				Printer * holoPrinter = new Printer();
				holoPrinter->PrintJobs->Add(printjob);
				holoPrinter->ShowPreview();
			}
			else
				UserNotification::InfoNotify("The selected tab-window is not a Log Window, please select a Log Window");
		}
	}

	//*************************************************************************
	// Method:		fileExitMenuItemClicked
	//*************************************************************************
	void MainFrame::fileExitMenuItemClicked(Object *sender, EventArgs *args)
	{
		this->Close();		
	}

	//*************************************************************************
	// Method:		applicationCopySelectedItemMenuItemClicked
	//*************************************************************************
	void MainFrame::applicationCopySelectedItemMenuItemClicked(Object *sender, EventArgs *args)
	{
		try
		{
			DataObject * myClipDataObject = new DataObject();

			//first check the dockable panes for focus
			for (int i=0; i < dockManager->Contents->Count; i++)
			{
				Control * cntrl = dockManager->Contents->Item[i]->Control;
				if (cntrl && cntrl->ContainsFocus)
				{
					//resource pane
					if (cntrl->GetType()->ToString()->Equals (S"HolodeckGui.ResourcePane"))
					{
						String * copyText = S"";
						ResourcePane * resourcePane = dynamic_cast <ResourcePane *> (cntrl);
						if (!resourcePane)
							return;

						ResourcePaneDataNode * rpDataNode = resourcePane->GetSelectedNode();
						if (rpDataNode)
							copyText = rpDataNode->ClipboardText;
						else
							copyText = resourcePane->GetSelectedNodeText();

						if (copyText)
						{
							myClipDataObject->SetData (System::Windows::Forms::DataFormats::Text, copyText);
							System::Windows::Forms::Clipboard::SetDataObject (myClipDataObject, true);
						}
						return;
					}
					//all other panes
					else
					{
						String * txtString = getFocusedControlText(cntrl->Controls);
						if (txtString)
						{
							myClipDataObject->SetData (System::Windows::Forms::DataFormats::Text, txtString);
							System::Windows::Forms::Clipboard::SetDataObject (myClipDataObject, true);
							return;
						}
					}
				}
			}

			//then check the non-dockable panes
			if (nonDockablePaneManager->SelectedTab != NULL)
			{
				String * copyText = NULL;
				Object * pane = nonDockablePaneManager->SelectedTab->Control;

				//logpane
				if (pane->GetType()->ToString()->Equals (S"HolodeckGui.LogPane"))
				{
					LogPane * logPane = dynamic_cast <LogPane*> (pane);
					if (!logPane)
						return;

					if (logPane->SelectedTreeListViewIndex == -1)
						return;

					LogPaneDataNode * lpDataNode = logPane->DataArray->GetSortedDataNodeAt (logPane->SelectedTreeListViewIndex);
					if (!lpDataNode || !lpDataNode->LogEntry)
						return;

					copyText = lpDataNode->LogEntry->ClipboardText;
				}
				//network logpane
				else if (pane->GetType()->ToString()->Equals (S"HolodeckGui.NetworkLogPane"))
				{
					NetworkLogPane * nlogPane = dynamic_cast <NetworkLogPane*> (pane);
					if (!nlogPane)
						return;

					if (nlogPane->SelectedListViewIndex == -1)
						return;

					NetworkLogPaneDataNode * nlDataNode = nlogPane->DataArray->GetSortedDataNodeAt (nlogPane->SelectedListViewIndex);
					if (!nlDataNode || !nlDataNode->LogEntry)
						return;
					
					copyText = nlDataNode->LogEntry->ClipboardText;
				}
				//resource test pane
				else if (pane->GetType()->ToString()->Equals (S"HolodeckGui.ResourceTestPane"))
				{
					ResourceTestPane * rtPane = dynamic_cast <ResourceTestPane*> (pane);
					if (!rtPane)
						return;
					
					if (rtPane->PaneListView->SelectedIndices->Count < 1)
						return;

					ResourceTestDataNode * rtDataNode = dynamic_cast <ResourceTestDataNode *> (rtPane->DataArray->GetDataNodeByID (dynamic_cast <String *> (rtPane->PaneListView->SelectedItems->Item[0]->Tag)));
					if (!rtDataNode)
						return;

					copyText = rtDataNode->ClipboardText;
				}
				//network corruption pane
				else if (pane->GetType()->ToString()->Equals (S"HolodeckGui.NetworkCorruptionPane"))
				{
					NetworkCorruptionPane * ncPane = dynamic_cast <NetworkCorruptionPane*> (pane);
					if (!ncPane)
						return;
					
					if (ncPane->PaneListView->SelectedIndices->Count < 1)
						return;

					NetworkCorruptionDataNode * ncDataNode = dynamic_cast <NetworkCorruptionDataNode *> (ncPane->DataArray->GetDataNodeByID (dynamic_cast <String *> (ncPane->PaneListView->SelectedItems->Item[0]->Tag)));
					if (!ncDataNode)
						return;

					copyText = ncDataNode->ClipboardText;
				}
				//file corruption pane
				else if (pane->GetType()->ToString()->Equals (S"HolodeckGui.FileCorruptionPane"))
				{
					FileCorruptionPane * fcPane = dynamic_cast <FileCorruptionPane*> (pane);
					if (!fcPane)
						return;
					
					if (fcPane->PaneListView->SelectedIndices->Count < 1)
						return;

					FileCorruptionDataNode * fcDataNode = dynamic_cast <FileCorruptionDataNode *> (fcPane->DataArray->GetDataNodeByID (dynamic_cast <String *> (fcPane->PaneListView->SelectedItems->Item[0]->Tag)));
					if (!fcDataNode)
						return;

					copyText = fcDataNode->ClipboardText;
				}
				//scheduled test pane
				else if (pane->GetType()->ToString()->Equals (S"HolodeckGui.ScheduledTestPane"))
				{
					ScheduledTestPane * stPane = dynamic_cast <ScheduledTestPane*> (pane);
					if (!stPane)
						return;
					
					if (stPane->PaneListView->SelectedIndices->Count < 1)
						return;

					ScheduledTestDataNode * stDataNode = dynamic_cast <ScheduledTestDataNode * > (stPane->DataArray->GetDataNodeByID (dynamic_cast <String *> (stPane->PaneListView->SelectedItems->Item[0]->Tag)));
					if (!stDataNode)
						return;

					copyText = stDataNode->ClipboardText;
				}
				// exception pane
				else if (pane->GetType()->ToString()->Equals(S"HolodeckGui.ExceptionPane"))
				{
					ExceptionPane * exPane = dynamic_cast <ExceptionPane*> (pane);
					if (!exPane)
						return;
					
					if (exPane->PaneListView->SelectedIndices->Count < 1)
						return;

					ExceptionDataNode * exDataNode = dynamic_cast <ExceptionDataNode * > (exPane->DataArray->GetDataNodeByID (dynamic_cast <String *> (exPane->PaneListView->SelectedItems->Item[0]->Tag)));
					if (!exDataNode)
						return;

					copyText = exDataNode->ClipboardText;
				}

				//copy the retrieved text to the clipboard
				if (copyText != NULL)
				{
					myClipDataObject->SetData (DataFormats::Text, copyText);
					Clipboard::SetDataObject (myClipDataObject, true);
				}
			}
		}
		catch (Exception * e)
		{
			UserNotification::ErrorNotify(String::Concat (S"Could not copy the item to the clipboard\r\n\r\nReason:", e->Message, "\r\n\r\nYou may want to try copying again."));
			return;
		}
	}

	//*************************************************************************
	// Method:		applicationPauseMenuItemClicked
	//*************************************************************************
	void MainFrame::applicationPauseMenuItemClicked(Object *sender, EventArgs *args)
	{
		HoloScriptApplication *holoScriptApplication;
		DisplayableDataNode *dataNode = GetActiveNode(true);

		if (dataNode)
		{
			holoScriptApplication = holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID);
			if (!holoScriptApplication)
				return;

			if (!applicationPauseMenuItem->Checked)
				holoScriptApplication->PauseApplication();
			else
				holoScriptApplication->ResumeApplication();
		}

	}

	//*************************************************************************
	// Method:		applicationRestartMenuItemClicked
	//*************************************************************************
	void MainFrame::applicationRestartMenuItemClicked(Object *sender, EventArgs *args)
	{
		HoloScriptApplication *holoScriptApplication;
		DisplayableDataNode *dataNode = GetActiveNode(true);

		if (dataNode)
		{
			holoScriptApplication = holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID);
			
			if (!holoScriptApplication)
				holoScriptApplication = holoScriptRegistry->getHoloScriptAppForTerminatedProcess(dataNode->processID);

			if (holoScriptApplication)
			{
				//Set the app to be restarted
				this->hsAppToRestart = holoScriptApplication;

				//Launch the restart thread
				System::Threading::ThreadStart *threadStart = new System::Threading::ThreadStart(this, ApplicationRestartThreadFunction);
				System::Threading::Thread *thread = new System::Threading::Thread(threadStart);
				thread->Start();

				//Show wait dialog
				launchProgressDlg->UserText = "Holodeck is restarting the specified application.";
				uiEventRegistry->OnShowDialog(launchProgressDlg);
			}
		}

	}

	//*************************************************************************
	// Method:		applicationRemoveMenuItemClicked
	//*************************************************************************
	void MainFrame::applicationRemoveMenuItemClicked(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = GetActiveNode(true);
		if (dataNode)
		{
			//set the pid of the aut which should be removed
			unsigned int pid_to_remove= dataNode->processID;

			//check if #AUTs is more than 1
			int TerminatedWillSaveCount = 0;
			int RunningWillSaveCount = 0;
			for (int hsIndex = 0; hsIndex < holoScriptRegistry->TerminatedCount; hsIndex++)
			{
				if (holoScriptRegistry->getTerminatedHoloScriptAppByIndex (hsIndex)->ShouldRestartWithProject)
					TerminatedWillSaveCount++;
			}

			for (int hsIndex = 0; hsIndex < holoScriptRegistry->Count; hsIndex++)
			{
				//don't count services.exe as a removable process
				if (holoScriptRegistry->getHoloScriptAppByIndex(hsIndex) != holoScriptRegistry->ServicesExeApplication)
					RunningWillSaveCount++;
			}

			if (RunningWillSaveCount + holoScriptRegistry->TerminatedCount == 1)
			{
				UserNotification::InfoNotify ("You cannot remove this application since you must have at least one application under test in your project");
				return;
			}
			if ( (holoScriptRegistry->Count + TerminatedWillSaveCount == 1) && (holoScriptRegistry->getHoloScriptAppForProcess(pid_to_remove)) )
			{
				UserNotification::InfoNotify ("You cannot remove this application as it is the last remaining application that can be saved with this project");
				return;
			}

			//remove the dataarrays and the tabpages associated with this process
			this->ProcessRemoved(pid_to_remove, true);

			//stop and running test generation
			StressTestGenerationStop();
			CodeCoverageTestGenerationStop();

			//Invoke remove of process from different panes etc
			uiEventRegistry->OnProcessRemoved->Invoke (pid_to_remove);
		}
	}

	//*************************************************************************
	// Method:		applicationStopMenuItemClicked
	//*************************************************************************
	void MainFrame::applicationStopMenuItemClicked(Object *sender, EventArgs *args)
	{
		HoloScriptApplication *holoScriptApplication;
		DisplayableDataNode *dataNode = GetActiveNode(true);

		if (dataNode)
		{
			unsigned int processID = dataNode->processID;
			holoScriptApplication = holoScriptRegistry->getHoloScriptAppForProcess(processID);
			if (holoScriptApplication)
			{
				CodeCoverageTestGenerationStop();
				StressTestGenerationStop();
				holoScriptApplication->TerminateOnClose = true;
				holoScriptRegistry->TerminateApplication(processID);
			}
		}
	}
	//*************************************************************************
	// Method:		applicationAttachDebuggerMenuItemClicked
	//*************************************************************************
	void MainFrame::applicationAttachDebuggerMenuItemClicked(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = GetActiveNode(true);
		if (dataNode)
		{
			unsigned int processID = dataNode->processID;
			HoloScriptApplication *holoScriptApplication = holoScriptRegistry->getHoloScriptAppForProcess(processID);
			if (holoScriptApplication)
			{
				applicationAttachDebuggerMenuItem->Checked = !applicationAttachDebuggerMenuItem->Checked;
				holoScriptApplication->AttachDebugger = applicationAttachDebuggerMenuItem->Checked;

				UpdateApplicationMenuItems();
			}
		}
	}
	//*************************************************************************
	// Method:		applicationLogFirstChanceMenuItemClicked
	//*************************************************************************
	void MainFrame::applicationLogFirstChanceMenuItemClicked(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = GetActiveNode(true);
		if (dataNode)
		{
			unsigned int processID = dataNode->processID;
			HoloScriptApplication *holoScriptApplication = holoScriptRegistry->getHoloScriptAppForProcess(processID);
			if (holoScriptApplication)
			{
				applicationLogFirstChanceMenuItem->Checked = !applicationLogFirstChanceMenuItem->Checked;
				holoScriptApplication->LogFirstChance = applicationLogFirstChanceMenuItem->Checked;
			}
		}
	}
	//*************************************************************************
	// Method:		applicationInheritSettingsMenuItemClicked
	//*************************************************************************
	void MainFrame::applicationInheritSettingsMenuItemClicked(Object *sender, EventArgs *args)
	{
		HoloScriptApplication *holoScriptApplication;
		DisplayableDataNode *dataNode = GetActiveNode(true);

		if (dataNode)
		{
			applicationInheritSettingsMenuItem->Checked = !applicationInheritSettingsMenuItem->Checked;

			holoScriptApplication = holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID);
			if (holoScriptApplication)
				holoScriptApplication->InheritSettings = applicationInheritSettingsMenuItem->Checked;
		}
	}

	//*************************************************************************
	// Method:		applicationCreateATestMenuItemClicked
	//*************************************************************************
	void MainFrame::applicationCreateATestMenuItemClicked(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = GetActiveNode(true);
		if (!dataNode)
			return;

		dataNode->Type = UIData::Test;
		uiEventRegistry->OnStartCreateTestWizard->Invoke(dataNode);
	}

	//*************************************************************************
	// Method:		applicationCreateAResourceTestMenuItemClicked
	//*************************************************************************
	void MainFrame::applicationCreateAResourceTestMenuItemClicked(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = GetActiveNode(true);
		if (!dataNode)
			return;

		ResourceTestDataNode *resourceTestDataNode = new ResourceTestDataNode("", "", FaultType::CustomFault, dataNode->processID, 0);
		uiEventRegistry->OnStartCreateResourceTestWizard->Invoke(resourceTestDataNode);
	}

	//*************************************************************************
	// Method:		applicationNewFaultMenuItemClicked
	//*************************************************************************
	void MainFrame::applicationNewFaultMenuItemClicked(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = GetActiveNode(true);
		if (!dataNode)
			return;

		if (!holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID))
		{
			UserNotification::InfoNotify("You can only create faults for running processes.");
			return;
		}

		dataNode->Name = "Faults";
		dataNode->Type = UIData::Faults;
		DisplayTabPage(dataNode, true);
	}
	//*************************************************************************
	// Method:		applicationNewNetworkCorruptionMenuItemClicked
	//*************************************************************************
	void MainFrame::applicationNewNetworkCorruptionMenuItemClicked(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = GetActiveNode(true);
		if (!dataNode)
			return;

		dataNode->Name = "Network Fuzzing Faults";
		dataNode->Type = UIData::NetworkCorruptionFaultType;
		dataNode->threadID = 0; // Network corruption isn't ever per-thread

		uiEventRegistry->OnStartNetworkCorruptionWizard->Invoke(dataNode);
	}
	//*************************************************************************
	// Method:		applicationNewFileCorruptionMenuItemClicked
	//*************************************************************************
	void MainFrame::applicationNewFileCorruptionMenuItemClicked(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = GetActiveNode(true);
		if (!dataNode)
			return;

		dataNode->Name = "File Fuzzing Faults";
		dataNode->Type = UIData::FileCorruptionFaultType;
		dataNode->threadID = 0; // File corruption isn't ever per-thread

		uiEventRegistry->OnStartFileCorruptionWizard->Invoke(dataNode);
	}

	//*************************************************************************
	// Method:		sessionCodeCoverageTestGeneratorMenuItemClicked
	//*************************************************************************
	void MainFrame::sessionCodeCoverageTestGeneratorMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
			DisplayTrialDisabledNotice(CODE_COVERAGE_HELP_FILE);
		else
		{
			MenuCommand *menuCommand = dynamic_cast <MenuCommand*> (sender);
			if (IsProjectActive)
			{
				if (menuCommand->Text->CompareTo(codeCoverageMenuItemText) == 0)
				{
					uiEventRegistry->OnShowDialog(codeCoverageTestGenerationDlg);
					if (codeCoverageTestGenerationDlg->DialogResult == DialogResult::OK)
					{
						//don't pop the dlg if in silent mode
						if (!silentMode)
						{
							int result = MessageBox(NULL, "Would you like to save your project before starting test generation?", "Holodeck", MB_YESNO | MB_TOPMOST | MB_TASKMODAL);

							//If want to save, then save the project
							if (result == IDYES)
							{
								fileSaveProjectAsMenuItemClicked(sender, args);
							}
						}
						CodeCoverageTestGenerationStart(codeCoverageTestGenerationDlg->Intensity, true,
							codeCoverageTestGenerationDlg->Timeout * 1000);
					}
				}
				else
				{
					CodeCoverageTestGenerationStop();
				}
			}
			else
			{
				UserNotification::InfoNotify("Please open a project first");
				return;
			}
		}
	}
	//*************************************************************************
	// Method:		sessionStressTestGeneratorMenuItemClicked
	//*************************************************************************
	void MainFrame::sessionStressTestGeneratorMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
			DisplayTrialDisabledNotice(STRESS_TEST_HELP_FILE);
		else
		{
			MenuCommand *menuCommand = dynamic_cast <MenuCommand*> (sender);
			if (!holoScriptRegistry->AnyAppsRunning)
			{
				UserNotification::InfoNotify("Cannot run stress test generation as there are no running test applications");
				return;
			}

			if (IsProjectActive)
			{
				if (menuCommand->Text->CompareTo(stressMenuItemText) == 0)
				{
					uiEventRegistry->OnShowDialog(stressTestGenerationDlg);
					if (stressTestGenerationDlg->DialogResult == DialogResult::OK)
					{
						StressTestGenerationStart(stressTestGenerationDlg->GenerationType, stressTestGenerationDlg->Intensity);
					}
				}
				else
				{
					StressTestGenerationStop();
				}
			}
			else
			{
				UserNotification::InfoNotify("Please open a project first");
				return;
			}
		}
	}
	//*************************************************************************
	// Method:		sessionReplaySessionMenuItemClicked
	//*************************************************************************
	void MainFrame::sessionReplaySessionMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
			DisplayTrialDisabledNotice(REPLAY_SESSION_HELP_FILE);
		else
		{
			MenuCommand *menuCommand = dynamic_cast <MenuCommand*> (sender);
			DisplayableDataNode *dataNode = new DisplayableDataNode("", UIData::RecordedSession, 0, 0);

			if (IsProjectActive)
			{
				if (menuCommand->Text->CompareTo(replayMenuItemText) == 0)
				{
					RecordedSessionReplayStart(NULL);
				}
				else
				{
					RecordedSessionReplayStop(dataNode);
				}
			}
			else
			{
				UserNotification::InfoNotify("Please open a project first");
				return;
			}
		}
	}
	//*************************************************************************
	// Method:		sessionRecordSessionMenuItemClicked
	//*************************************************************************
	void MainFrame::sessionRecordSessionMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
			DisplayTrialDisabledNotice(RECORD_SESSION_HELP_FILE);
		else
		{
			MenuCommand *menuCommand = dynamic_cast <MenuCommand*> (sender);
			DisplayableDataNode *dataNode = new DisplayableDataNode("", UIData::RecordedSession, 0, 0);

			if (IsProjectActive)
			{
				if (menuCommand->Text->CompareTo(recordMenuItemText) == 0)
				{
					RecordedSessionStart(dataNode);
				}
				else
				{
					RecordedSessionStop(dataNode);
				}
			}
			else
			{
				UserNotification::InfoNotify("Please open a project first");
				return;
			}
		}
	}
	//*************************************************************************
	// Method:		sessionCreateReportMenuItemClicked
	//*************************************************************************
	void MainFrame::sessionCreateReportMenuItemClicked(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode;
		String *name = "Holodeck Report";
		String *keyString;

		if (IsProjectActive)
		{
			dataNode = new DisplayableDataNode(name, UIData::Report, 0, 0);
			dataNode->ID = name;
			onDeletePane(dataNode);
		
			ReportPaneDataArray *reportDataArray = new ReportPaneDataArray(holoScriptRegistry);
			if (!reportDataArray->CreateReport(String::Concat(currentProject->Folder, REPORT_FOLDER_DEFAULT_VALUE), currentProject->DataArrayList))
				return;

			keyString = "Default Report";
			if (currentProject->ContainsDataArrayKey(keyString))
			{
				ReportPaneDataArray *oldDataArray = dynamic_cast <ReportPaneDataArray*> (currentProject->GetDataArrayByKey(keyString));
				if (oldDataArray)
				{
					currentProject->RemoveDataArray(oldDataArray);
					delete oldDataArray;
				}
			}
			currentProject->AddDataArray(keyString, reportDataArray);

			dataNode->DataArray = reportDataArray;
			dataEventRegistry->OnReportCreated->Invoke(dataNode);
			DisplayTabPage(dataNode, true);
		}
		else
		{
			UserNotification::InfoNotify("Please open a project first");
			return;
		}
	}
	//*************************************************************************
	// Method:		sessionPerThreadMenuItemClicked
	//*************************************************************************
	void MainFrame::sessionPerThreadMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (!IsProjectActive)
		{
			UserNotification::InfoNotify("Please open a project first");
			return;
		}

		sessionPerThreadMenuItem->Checked = !sessionPerThreadMenuItem->Checked;
		currentProject->IsInPerThreadMode = sessionPerThreadMenuItem->Checked;

		// call the OnPerThreadChanged handler
		if (uiEventRegistry->OnPerThreadChanged)
			uiEventRegistry->OnPerThreadChanged->Invoke(currentProject->IsInPerThreadMode);
	}
	//*************************************************************************
	// Method:		logFindMenuItemClicked
	//*************************************************************************
	void MainFrame::logFindMenuItemClicked(Object *sender, EventArgs *args)
	{
		Crownwood::Magic::Controls::TabPage *selectedTab = nonDockablePaneManager->SelectedTab;
		HoloScriptApplication *holoScriptApp = NULL;

		if (!selectedTab || !selectedTab->Control)
			return;

		String *controlType = selectedTab->Control->GetType()->ToString();
		bool findInApiLog = controlType->Equals(new String("HolodeckGui.LogPane"));
		bool findInNetworkLog = controlType->Equals(new String("HolodeckGui.NetworkLogPane"));
		if ((selectedTab == NULL) || (!findInApiLog && !findInNetworkLog))
		{
			UserNotification::InfoNotify("Please select a Log pane to search");
			return;
		}

		if (!findNextDialog)
			findNextDialog = new FindNextDialog();

		if (!netLogFindNextDialog)
			netLogFindNextDialog = new NetworkLogFindNextDialog();

		if (findInApiLog)
		{
			netLogFindNextDialog->Hide();

			// check to make sure it is an appropriate type
			LogPane *pane = dynamic_cast<LogPane *>(selectedTab->Control);
			
			findNextDialog->CurrentLogPane = pane;
			findNextDialog->Show();
		}
		else if (findInNetworkLog)
		{
			findNextDialog->Hide();

			NetworkLogPane *pane = dynamic_cast<NetworkLogPane *>(selectedTab->Control);
			netLogFindNextDialog->CurrentLogPane = pane;
			netLogFindNextDialog->CurrentLogDetailsPane = dynamic_cast<NetworkMessageDetailsPane *>(networkMessageDetailsPane->Control);

			netLogFindNextDialog->Show();
		}
	}
	//*************************************************************************
	// Method:		logFindNextMenuItemClicked
	//*************************************************************************
	void MainFrame::logFindNextMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (!findNextDialog || !netLogFindNextDialog)
		{
			logFindMenuItemClicked(sender, args);
			return;
		}
		
		Crownwood::Magic::Controls::TabPage *selectedTab = nonDockablePaneManager->SelectedTab;
		HoloScriptApplication *holoScriptApp = NULL;

		if (!selectedTab || !selectedTab->Control)
			return;

		String *controlType = selectedTab->Control->GetType()->ToString();
		bool findInApiLog = controlType->Equals(new String("HolodeckGui.LogPane"));
		bool findInNetworkLog = controlType->Equals(new String("HolodeckGui.NetworkLogPane"));
		if ((selectedTab == NULL) || (!findInApiLog && !findInNetworkLog))
		{
			UserNotification::InfoNotify("Please select a Log pane to search");
			return;
		}

		if (findInApiLog)
		{
			netLogFindNextDialog->Hide();

			// check to make sure it is an appropriate type
			LogPane *pane = dynamic_cast<LogPane *>(selectedTab->Control);
			
			findNextDialog->CurrentLogPane = pane;
			findNextDialog->SearchNext();
		}
		else if (findInNetworkLog)
		{
			findNextDialog->Hide();

			NetworkLogPane *pane = dynamic_cast<NetworkLogPane *>(selectedTab->Control);
			netLogFindNextDialog->CurrentLogPane = pane;
			netLogFindNextDialog->CurrentLogDetailsPane = dynamic_cast<NetworkMessageDetailsPane *>(networkMessageDetailsPane->Control);

			netLogFindNextDialog->SearchNext();
		}
	}

	//*************************************************************************
	// Method:		logFilterMenuItemClicked
	//*************************************************************************
	void MainFrame::logFilterMenuItemClicked(Object *sender, EventArgs *args)
	{
		bool terminatedApp = false;

		DisplayableDataNode *dataNode = GetActiveNode(true);
		if (!dataNode)
			return;

		//Try and get the running HoloScritpApp for this process
		HoloScriptApplication *holoScriptApp = holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID);
		//If this process is no longer running, then get the terminated HoloScriptApp for the process
		if (!holoScriptApp)
		{
			holoScriptApp = holoScriptRegistry->getHoloScriptAppForTerminatedProcess (dataNode->processID);
			terminatedApp = true;
			if (!holoScriptApp)
				return;
		}

		FunctionsFilterForm *dialog = new FunctionsFilterForm(NULL, NULL);
		dialog->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
		dialog->Text = "Log Filter";
		dialog->InstructionsText = "Select the functions to log.  Note: This will only apply to new log entries, not existing ones.";
		dialog->ShowFilterEnableCheckBox = false;
		bool appPaused;
		
		dialog->FunctionsList = holoScriptApp->FilteredFunctionList;
		appPaused = holoScriptApp->ApplicationPaused;
		if (!appPaused && !terminatedApp)
			holoScriptApp->PauseApplication();

		uiEventRegistry->OnShowDialog(dialog);

		if (dialog->DialogResult == DialogResult::OK)
		{
			holoScriptApp->FilteredFunctionList = dialog->FunctionsList;
		}

		if (!appPaused && !terminatedApp)
			holoScriptApp->ResumeApplication();
	}
	//*************************************************************************
	// Method:		toolsCustomWrapperLibraryMenuItemClicked
	//*************************************************************************
	void MainFrame::toolsCustomWrapperLibraryMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
			DisplayTrialDisabledNotice(LIBRARY_EXPORTER_HELP_FILE);
		else
		{
			CustomTestProjectGenerator::CustomTestProjectWizard *ctpw = new CustomTestProjectGenerator::CustomTestProjectWizard();
			uiEventRegistry->OnShowDialog(ctpw);
		}
	}
	//*************************************************************************
	// Method:		toolsAddHolodeckInterceptsMenuItemClicked
	//*************************************************************************
	void MainFrame::toolsAddHolodeckInterceptsMenuItemClicked(Object *sender, EventArgs *args)
	{
		MainFrame::launchNewIntercepts();
	}
	//*************************************************************************
	// Method:		toolsHolodeckInterceptsManagerMenuItemClicked
	//*************************************************************************
	void MainFrame::toolsHolodeckInterceptsManagerMenuItemClicked(Object *sender, EventArgs *args)
	{
		FaultyUserLibrariesDialog * ulDialog = new FaultyUserLibrariesDialog();
		ulDialog->PerformCheckAndPopulateList();
		uiEventRegistry->OnShowDialog(ulDialog);
	}
	//*************************************************************************
	// Method:		viewFieldChooserMenuItemClicked
	//*************************************************************************
	void MainFrame::viewFieldChooserMenuItemClicked(Object *sender, EventArgs *args)
	{
		Pane *selectedPane = NULL;
		FieldChooser *fieldChooser = NULL;
		Crownwood::Magic::Controls::TabPage *selectedTab = nonDockablePaneManager->SelectedTab;
		HoloScriptApplication *holoScriptApp = NULL;
		bool resume = false;
	
		if (selectedTab == NULL)
		{
			UserNotification::InfoNotify("Please select a Log or Scheduled Tests Pane");
			return;
		}

		this->Cursor = Cursors::WaitCursor;

		// check to make sure it is an appropriate type
		SelectableFieldPane *pane = dynamic_cast<SelectableFieldPane *>(selectedTab->Control);
		if (pane)
		{
			holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(pane->ProcessID);
			if (holoScriptApp && !holoScriptApp->ApplicationPaused)
			{
				holoScriptApp->PauseApplication();
				resume = true;
			}

			ListView *paneListView = pane->PaneListView;
			TreeListView *paneTreeListView = pane->PaneTreeListView;
			ContainerListView *paneContainerListView = pane->PaneContainerListView;
			if (paneListView)
				fieldChooser = new FieldChooser(paneListView, pane->AllColumns);
			else if (paneTreeListView)
				fieldChooser = new FieldChooser(paneTreeListView, pane->AllColumns);
			else if (paneContainerListView)
				fieldChooser = new FieldChooser(paneContainerListView, pane->AllColumns);
		}

		if (fieldChooser == NULL)
		{
			UserNotification::InfoNotify("Please select a pane with selectable columns");

			this->Cursor = Cursors::Default;
			return;
		}

		int result = uiEventRegistry->OnShowDialog(fieldChooser);
		if (result == DialogResult::OK)
		{
			if (pane)
			{
				pane->VisibleColumns = fieldChooser->SelectedColumns;
				pane->SaveSettings(pane->PaneName);
			}
		}

		if (holoScriptApp && resume)
			holoScriptApp->ResumeApplication();

		this->Cursor = Cursors::Default;
	}

	//*************************************************************************
	// Method:		viewProjectPaneMenuItemClicked
	//*************************************************************************
	void MainFrame::viewProjectPaneMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (projectPane->Visible)
			dockManager->HideContent(projectPane);
		else
			dockManager->ShowContent(projectPane);

		this->refreshMenuItemAndButtonBarState();
	}
	//*************************************************************************
	// Method:		viewResourcePaneMenuItemClicked
	//*************************************************************************
	void MainFrame::viewResourcePaneMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (resourcePane->Visible)
			dockManager->HideContent(resourcePane);
		else
			dockManager->ShowContent(resourcePane);

		this->refreshMenuItemAndButtonBarState();
	}
	//*************************************************************************
	// Method:		viewLimitsPaneMenuItemClicked
	//*************************************************************************
	void MainFrame::viewLimitsPaneMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (limitPane->Visible)
			dockManager->HideContent(limitPane);
		else
			dockManager->ShowContent(limitPane);

		this->refreshMenuItemAndButtonBarState();
	}
	//*************************************************************************
	// Method:		viewFileCorruptDetailsPaneMenuItemClicked
	//*************************************************************************
	void MainFrame::viewFileCorruptDetailsPaneMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (fileCorruptDetailsPane->Visible)
			dockManager->HideContent(fileCorruptDetailsPane);
		else
			dockManager->ShowContent(fileCorruptDetailsPane);

		this->refreshMenuItemAndButtonBarState();
	}
	//*************************************************************************
	// Method:		viewNetworkMessageDetailsPaneMenuItemClicked
	//*************************************************************************
	void MainFrame::viewNetworkMessageDetailsPaneMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (networkMessageDetailsPane->Visible)
			dockManager->HideContent(networkMessageDetailsPane);
		else
			dockManager->ShowContent(networkMessageDetailsPane);

		this->refreshMenuItemAndButtonBarState();
	}
	//*************************************************************************
	// Method:		viewPropertiesPaneMenuItemClicked
	//*************************************************************************
	void MainFrame::viewPropertiesPaneMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (propertiesPane->Visible)
			dockManager->HideContent(propertiesPane);
		else
			dockManager->ShowContent(propertiesPane);

		this->refreshMenuItemAndButtonBarState();
	}
	//*************************************************************************
	// Method:		viewHelpPaneMenuItemClicked
	//*************************************************************************
	void MainFrame::viewHelpPaneMenuItemClicked(Object *sender, EventArgs *args)
	{
		if (helpPane->Visible)
		{
			dynamicHelpPane->DisplayUrls = false;
			dockManager->HideContent(helpPane);
		}
		else
		{
			dynamicHelpPane->DisplayUrls = true;
			dockManager->ShowContent(helpPane);
		}

		this->refreshMenuItemAndButtonBarState();
	}
	//*************************************************************************
	// Method:		helpHelpTopicsMenuItemClicked
	//*************************************************************************
	void MainFrame::helpHelpTopicsMenuItemClicked(Object *sender, EventArgs *args)
	{
		String *url = String::Concat("file://", GuiSupport::RegistryHelper::getInstance()->GetInstallationPath(), "/");
		url = String::Concat(url, TABLE_OF_CONTENTS_HELP_FILE);
		this->nonDockablePaneManager->displayHelpPane(url);
	}
	//*************************************************************************
	// Method:		helpPDFHelpTopicsMenuItemClicked
	//*************************************************************************
	void MainFrame::helpPDFHelpTopicsMenuItemClicked(Object *sender, EventArgs *args)
	{
		String *url = String::Concat("file://", GuiSupport::RegistryHelper::getInstance()->GetInstallationPath(), "/");
		url = String::Concat(url, PDF_HELP_FILE);
		url = String::Concat("external*", url);
		this->nonDockablePaneManager->displayHelpPane(url);
	}
	//*************************************************************************
	// Method:		helpHolodeckSiteMenuItemClicked
	//*************************************************************************
	void MainFrame::helpHolodeckSiteMenuItemClicked(Object *sender, EventArgs *args)
	{
		this->nonDockablePaneManager->displayHelpPane("http://www.securityinnovation.com/holodeck");
	}
	//*************************************************************************
	// Method:		helpReportABugMenuItemClicked
	//*************************************************************************
	void MainFrame::helpReportABugMenuItemClicked(Object *sender, EventArgs *args)
	{
		this->nonDockablePaneManager->displayHelpPane("http://www.securityinnovation.com/holodeck/bugs");
	}
	//*************************************************************************
	// Method:		helpRegisterMenuItemClicked
	//*************************************************************************
	void MainFrame::helpRegisterMenuItemClicked(Object *sender, EventArgs *args)
	{
		RegistrationDialog *dialog = new RegistrationDialog();
		uiEventRegistry->OnShowDialog(dialog);
	}
	//*************************************************************************
	// Method:		helpAboutHolodeckMenuItemClicked
	//*************************************************************************
	void MainFrame::helpAboutHolodeckMenuItemClicked(Object *sender, EventArgs *args)
	{
		AboutBox *aboutBox = new AboutBox();
		uiEventRegistry->OnShowDialog(aboutBox);
	}
	//*************************************************************************
	// END OF MENU ITEM CLICK EVENT HANDLERS
	//*************************************************************************

	//*************************************************************************
	// Method:		OnProcessPaused event handler
	//*************************************************************************
	void MainFrame::onProcessPaused(unsigned int processID)
	{
		HoloScriptApplication * holoScriptApplication = holoScriptRegistry->getHoloScriptAppForProcess(processID);
		if (holoScriptApplication)
			holoScriptApplication->SetAppPauseState(true);
	}

	//**************************************************************************
	// Method:		CloseProject
	// Description: closes the current project
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::CloseProject()
	{
		Type *type;

		dataEventRegistry->OnCloseProject->Invoke();
		uiEventRegistry->OnCloseProjectUI->Invoke();

		RecordedSessionStop(NULL);
		RecordedSessionReplayStop(NULL);
		StressTestGenerationStop();
		CodeCoverageTestGenerationStop();

		IsProjectActive = false;

		while (currentProject->DisplayableDataArrayCount > 0)
		{
			DisplayableDataArray *dataArray = currentProject->GetDisplayableDataArray(0);
			type = dataArray->GetType();
			currentProject->RemoveDataArrayAt(0);
			dataArray->Clear();
			if (type->Name->CompareTo("ProjectPaneDataArray") != 0)
				dataArray->CleanUp();
		}
		
		CloseAllNonDockablePanes();

		holoScriptRegistry->terminateAppsAndRemoveAll();
		this->UpdateHolodeckWindowTitle ("");
	}

	//*************************************************************************
	// Method:		OpenProjectDlg
	// Description: Creates file open dialog and calls openProject
	//
	// Parameters:
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool MainFrame::OpenProjectDlg()
	{
		HoloScriptApplication *hsApp;
		RegistryHelper *regHelper = RegistryHelper::getInstance();
		System::Windows::Forms::OpenFileDialog * openFileDlg = new System::Windows::Forms::OpenFileDialog();
		openFileDlg->AddExtension = true;
		openFileDlg->AddExtension = true;
		openFileDlg->Filter = "Holodeck projects(*.hdp)|*.hdp";
		openFileDlg->DefaultExt = "*.hdp";
		openFileDlg->CheckPathExists = true;
		openFileDlg->CheckFileExists = true;
		openFileDlg->InitialDirectory = regHelper->GetLastProjectSavePath();

		if (openFileDlg->ShowDialog(this) == DialogResult::OK)
		{
			currentProject->Folder = System::IO::Path::GetDirectoryName(openFileDlg->FileName);
			currentProject->FullPath = openFileDlg->FileName;
			currentProject->Name = System::IO::Path::GetFileName(openFileDlg->FileName);
			int index = currentProject->Name->IndexOf(".hdp");
			if (index > 0)
				currentProject->Title = currentProject->Name->Substring(0, index);
			else
				currentProject->Title = currentProject->Name;

			ProjectFileReader *reader = new ProjectFileReader();
			reader->OnLaunchSavedApp += new LaunchSavedAppDelegate(this, LaunchSavedApp);
			if (reader->OpenProject(currentProject->FullPath, currentProject))
			{
				IsProjectActive = true;
				sessionPerThreadMenuItem->Checked = currentProject->IsInPerThreadMode;
				this->UpdateHolodeckWindowTitle (currentProject->Title);
				if (holoScriptRegistry->Count > 0)
					hsApp = holoScriptRegistry->getHoloScriptAppByIndex(0);
				
				if (hsApp)
				{
					UInt32 pID;
					hsApp->GetApplicationProcessID(pID);
					uiEventRegistry->OnFocusChanged(new DisplayableDataNode("Faults", UIData::Faults, pID, 0));
				}
				return true;
			}
			else
				return false;
		}
		return true;
	}

	//**************************************************************************
	// Method:		LaunchSavedApp
	// Description: launches the saved AUT with all the saved settings	
	//
	// Parameters:
	//		procInfo - contains process info, including log info, scheduled tests
	//		procFaults - contains faults to be injected
	//		procLimits - contains limits to be applied
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::LaunchSavedApp(SortedList *procInfo, SortedList *procFaults, SortedList *procLimits)
	{
		DisplayableDataNode *dataNode;
		HoloScriptApplication *hsApp;
		unsigned int newProcID, newThreadID;
		
		String *keyString;
		keyString = "AutPath";
		String *AutPath = procInfo->get_Item(keyString)->ToString();
		keyString = "AutCmdLine";
		String *AutCmdLine = procInfo->get_Item(keyString)->ToString();
		keyString = "AutInjectMode";
		String *AutInjectMode = procInfo->get_Item(keyString)->ToString();
		keyString = "AutStartPaused";
		bool AutStartPaused = Convert::ToBoolean(procInfo->get_Item(keyString));
		keyString = "AutProcessChaining";
		bool AutProcessChaining = Convert::ToBoolean(procInfo->get_Item(keyString));
		keyString = "AutAttachDebugger";
		bool AutAttachDebugger = Convert::ToBoolean(procInfo->get_Item(keyString));
		keyString = "AutLogFirstChance";
		bool AutLogFirstChance = Convert::ToBoolean(procInfo->get_Item(keyString));
		keyString = "AutInheritSettings";
		bool AutInheritSettings = Convert::ToBoolean(procInfo->get_Item(keyString));
		keyString = "FilteredFuncs";
		ArrayList * FilteredFuncs = dynamic_cast <ArrayList *> (procInfo->get_Item(keyString));		
		keyString = "SchTests";
		ArrayList * SchTests = dynamic_cast <ArrayList *> (procInfo->get_Item(keyString));
		keyString = "ResourceTests";
		ArrayList * ResourceTests = dynamic_cast <ArrayList *> (procInfo->get_Item(keyString));
		keyString = "NetworkCorruptionFaults";
		ArrayList * NetworkCorruptionFaults = dynamic_cast <ArrayList *> (procInfo->get_Item(keyString));
		keyString = "FileCorruptionFaults";
		ArrayList * FileCorruptionFaults = dynamic_cast <ArrayList *> (procInfo->get_Item(keyString));

		keyString = "DiskFaults";
		ArrayList * DiskFaults = dynamic_cast <ArrayList *> (procFaults->get_Item(keyString));
		keyString = "MemoryFaults";
		ArrayList * MemoryFaults = dynamic_cast <ArrayList *> (procFaults->get_Item(keyString));
		keyString = "NetworkFaults";
		ArrayList * NetworkFaults = dynamic_cast <ArrayList *> (procFaults->get_Item(keyString));
		keyString = "RegistryFaults";
		ArrayList * RegistryFaults = dynamic_cast <ArrayList *> (procFaults->get_Item(keyString));
		keyString = "ProcessFaults";
		ArrayList * ProcessFaults = dynamic_cast <ArrayList *> (procFaults->get_Item(keyString));
		keyString = "CustomFaults";
		ArrayList * CustomFaults = dynamic_cast <ArrayList *> (procFaults->get_Item(keyString));
		
		keyString = "diskLimitActive";
		bool diskLimitActive = Convert::ToBoolean(procLimits->get_Item(keyString));
		keyString = "diskLimitValue";
		Int64 diskLimitValue = Convert::ToInt64(procLimits->get_Item(keyString));
		keyString = "diskLimitUnit";
		String * diskLimitUnit = procLimits->get_Item(keyString)->ToString();

		keyString = "memoryLimitActive";
		bool memoryLimitActive = Convert::ToBoolean(procLimits->get_Item(keyString));
		keyString = "memoryLimitValue";
		Int64 memoryLimitValue = Convert::ToInt64(procLimits->get_Item(keyString));
		keyString = "memoryLimitUnit";
		String * memoryLimitUnit = procLimits->get_Item(keyString)->ToString();

		keyString = "downLimitEnabled";
		bool downLimitEnabled = Convert::ToBoolean(procLimits->get_Item(keyString));
		keyString = "downloadBandwidth";
		UInt64 downloadBandwidth = Convert::ToUInt64(procLimits->get_Item(keyString));
		keyString = "upLimitEnabled";
		bool upLimitEnabled = Convert::ToBoolean(procLimits->get_Item(keyString));
		keyString = "uploadBandwidth";
		UInt64 uploadBandwidth = Convert::ToUInt64(procLimits->get_Item(keyString));

		Object *args[] = __gc new Object*[1];

		NewProjectWizard *wizard = new NewProjectWizard(true);
		wizard->set_ApplicationPath(AutPath);
		//get application name from path
		int index = AutPath->LastIndexOf("\\");
		index++;
		String *appName = AutPath->Substring(index, AutPath->Length-index);
		wizard->set_ApplicationName(appName);
		wizard->set_CommandLineParams(AutCmdLine);
		wizard->set_PauseOnStart(true);
		wizard->set_FilteredFunctionList(FilteredFuncs);
		wizard->set_ProcessChaining(AutProcessChaining);
		wizard->set_InheritSettings(AutInheritSettings);
		wizard->set_AttachDebugger(AutAttachDebugger);
		wizard->set_LogFirstChance(AutLogFirstChance);

		if (AutInjectMode->CompareTo("LAUNCH") == 0)
		{
			wizard->LaunchApp = true;
		}
		else if (AutInjectMode->CompareTo("ATTACH") == 0)
		{
			AttachApplicationForm * attachToApp = new AttachApplicationForm(AutPath);
			uiEventRegistry->OnShowDialog(attachToApp);
			if (attachToApp->get_LaunchNewAut())
			{
				wizard->LaunchApp = true;
			}
			else
			{
                int procID = attachToApp->get_ProcessID();
				wizard->ProcessID = procID;
				wizard->AttachToApp = true;
			}
		}
		else if (AutInjectMode->CompareTo("LAUNCHSERVICE") == 0)
		{
			String *path = ServiceHelper::GetServiceExecutablePath(AutPath);
			if (path)
				wizard->ApplicationPath = path;
			wizard->StartAService = true;
			wizard->ServiceToStart = AutPath;
		}
		else if (AutInjectMode->CompareTo("ATTACHSERVICE") == 0)
		{
			String *path = ServiceHelper::GetServiceExecutablePath(AutPath);
			if (path)
				wizard->ApplicationPath = path;
			wizard->AttachToAService = true;
			wizard->ServiceToAttachTo = AutPath;
		}
		

		NewTestApplicationWorker(wizard);
		newProcID = wizard->get_ProcessID();
		hsApp = holoScriptRegistry->getHoloScriptAppForProcess(newProcID);
		if (hsApp)
			hsApp->GetApplicationThreadID(newThreadID);
		else
		{
			UserNotification::ErrorNotify("The application under test has terminated before Holodeck could load the project.  Please close the project and try again.");
			return;
		}

		
		//set the limits
		if (diskLimitActive)
		{
			LimitPaneDataNode *dataNode = new LimitPaneDataNode("Disk Limit", UIData::DiskLimit, newProcID, 0);
			dataNode->LimitValue = diskLimitValue;
			dataNode->UpdateDelegates();
			args[0] = dataNode;
			dataNode->CreateDelegate->DynamicInvoke(args);
		}
		if (memoryLimitActive)
		{
			LimitPaneDataNode *dataNode = new LimitPaneDataNode("Memory Limit", UIData::MemoryLimit, newProcID, 0);
			dataNode->LimitValue = memoryLimitValue;
			dataNode->UpdateDelegates();
			args[0] = dataNode;
			dataNode->CreateDelegate->DynamicInvoke(args);
		}

		if (downLimitEnabled)
		{
			LimitPaneDataNode *downLimitNode = new LimitPaneDataNode("Network Download Limit", UIData::NetworkDownLimit, newProcID, 0);
			downLimitNode->LimitValue = downloadBandwidth;
			downLimitNode->UpdateDelegates();
			args[0] = downLimitNode;
			downLimitNode->CreateDelegate->DynamicInvoke(args);
		}
		
		if (upLimitEnabled)
		{
			LimitPaneDataNode *upLimitNode = new LimitPaneDataNode("Network Upload Limit", UIData::NetworkUpLimit, newProcID, 0);
			upLimitNode->LimitValue = uploadBandwidth;
			upLimitNode->UpdateDelegates();
			args[0] = upLimitNode;
			upLimitNode->CreateDelegate->DynamicInvoke(args);
		}
		
				
		//Add Faults
		if (DiskFaults != NULL)
		{
			IEnumerator * enumerator = DiskFaults->GetEnumerator();
			while(enumerator->MoveNext())
			{
				dataNode = new FaultPaneDataNode((enumerator->Current)->ToString(), UIData::DiskFault, newProcID, 0);
				dataNode->UpdateDelegates();
				args[0] = dataNode;
				dataNode->CreateDelegate->DynamicInvoke(args);
			}
		}
		if (MemoryFaults != NULL)
		{
			IEnumerator * enumerator = MemoryFaults->GetEnumerator();
			while(enumerator->MoveNext())
			{
				dataNode = new FaultPaneDataNode((enumerator->Current)->ToString(), UIData::MemoryFault, newProcID, 0);
				dataNode->UpdateDelegates();
				args[0] = dataNode;
				dataNode->CreateDelegate->DynamicInvoke(args);
			}
		}
		if (NetworkFaults != NULL)
		{
			IEnumerator * enumerator = NetworkFaults->GetEnumerator();
			while(enumerator->MoveNext())
			{
				dataNode = new FaultPaneDataNode((enumerator->Current)->ToString(), UIData::NetworkFault, newProcID, 0);
				dataNode->UpdateDelegates();
				args[0] = dataNode;
				dataNode->CreateDelegate->DynamicInvoke(args);
			}
		}
		if (RegistryFaults != NULL)
		{
			IEnumerator * enumerator = RegistryFaults->GetEnumerator();
			while(enumerator->MoveNext())
			{
				dataNode = new FaultPaneDataNode((enumerator->Current)->ToString(), UIData::RegistryFault, newProcID, 0);
				dataNode->UpdateDelegates();
				args[0] = dataNode;
				dataNode->CreateDelegate->DynamicInvoke(args);
			}
		}
		if (ProcessFaults != NULL)
		{
			IEnumerator * enumerator = ProcessFaults->GetEnumerator();
			while(enumerator->MoveNext())
			{
				dataNode = new FaultPaneDataNode((enumerator->Current)->ToString(), UIData::ProcessFault, newProcID, 0);
				dataNode->UpdateDelegates();
				args[0] = dataNode;
				dataNode->CreateDelegate->DynamicInvoke(args);
			}
		}
		if (CustomFaults != NULL)
		{
			IEnumerator * enumerator = CustomFaults->GetEnumerator();
			while(enumerator->MoveNext())
			{
				dataNode = new FaultPaneDataNode((enumerator->Current)->ToString(), UIData::CustomFault, newProcID, 0);
				dataNode->UpdateDelegates();
				args[0] = dataNode;
				dataNode->CreateDelegate->DynamicInvoke(args);
			}
		}

		//Add scheduled tests
		if (SchTests != NULL)
		{
			dataNode = new DisplayableDataNode("", UIData::Test, newProcID, 0);
			IEnumerator *enumerator = SchTests->GetEnumerator();
			while (enumerator->MoveNext())
			{
				ScheduledTestDataNode *testNode  = dynamic_cast <ScheduledTestDataNode *> (enumerator->Current);
				testNode->processID = newProcID;
				testNode->UpdateDelegates();
				args[0] = testNode;
				testNode->CreateDelegate->DynamicInvoke(args);
			}
		}

		//Add resource faults
		if (ResourceTests != NULL)
		{
			IEnumerator *enumerator = ResourceTests->GetEnumerator();
			while (enumerator->MoveNext())
			{
				ResourceTestDataNode *resourceTestNode  = dynamic_cast <ResourceTestDataNode *> (enumerator->Current);
				resourceTestNode->processID = newProcID;
				resourceTestNode->UpdateDelegates();
				args[0] = resourceTestNode;
				resourceTestNode->CreateDelegate->DynamicInvoke(args);
			}
		}

		//Add the network corruption faults
		if (NetworkCorruptionFaults != NULL)
		{
			dataNode = new DisplayableDataNode("", UIData::NetworkCorruptionFault, newProcID, 0);
			IEnumerator *enumerator = NetworkCorruptionFaults->GetEnumerator();
			while (enumerator->MoveNext())
			{
				NetworkCorruptionDataNode *networkCorruptionNode  = dynamic_cast <NetworkCorruptionDataNode*> (enumerator->Current);
				networkCorruptionNode->processID = newProcID;
				networkCorruptionNode->UpdateDelegates();
				args[0] = networkCorruptionNode;
				networkCorruptionNode->CreateDelegate->DynamicInvoke(args);
			}
		}

		//Add the file corruption faults
		if (FileCorruptionFaults != NULL)
		{
			dataNode = new DisplayableDataNode("", UIData::FileCorruptionFault, newProcID, 0);
			IEnumerator *enumerator = FileCorruptionFaults->GetEnumerator();
			while (enumerator->MoveNext())
			{
				FileCorruptionDataNode *fileCorruptionNode  = dynamic_cast <FileCorruptionDataNode*> (enumerator->Current);
				fileCorruptionNode->processID = newProcID;
				fileCorruptionNode->UpdateDelegates();
				args[0] = fileCorruptionNode;
				fileCorruptionNode->CreateDelegate->DynamicInvoke(args);
			}
		}


		//added so the value persists in hsApp object
		hsApp->set_PauseOnStart(AutStartPaused);
		
		if (!AutStartPaused)
			hsApp->ResumeApplication();

	}

	//*************************************************************************
	// Method:		CloseAllNonDockablePanes
	// Description: closes all non dockable panes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::CloseAllNonDockablePanes()
	{
		Crownwood::Magic::Collections::TabPageCollection *pages = nonDockablePaneManager->TabPages;
		ArrayList *listToRemove = new ArrayList();
		IEnumerator *enumerator = pages->GetEnumerator();
		while (enumerator->MoveNext())
		{
			Crownwood::Magic::Controls::TabPage *page = dynamic_cast<Crownwood::Magic::Controls::TabPage *>(enumerator->Current);
			if (!page)
				continue;

			if (page != startPage)
				listToRemove->Add(page);
		}
		
		// actually remove the pages
		enumerator = listToRemove->GetEnumerator();
		while (enumerator->MoveNext())
		{
			Crownwood::Magic::Controls::TabPage *page = dynamic_cast<Crownwood::Magic::Controls::TabPage *>(enumerator->Current);
			if (!page)
				continue;
			
			pages->Remove(page);
		}
	}

	//*************************************************************************
	// Method:		toolBarButtonClicked
	// Description: Handles the click event on a tool bar button
	//
	// Parameters:
	//	sender - the object that sent the event
	//	args - the event arguments representing the click event
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::toolBarButtonClicked(Object *sender, ToolBarButtonClickEventArgs *args)
	{
		// use the image index since they are in the same order anyway
		// this saves us from having to worry about where the separators are in the button bar
		switch (args->Button->ImageIndex)
		{
		case ButtonBarTypes::NewProjectType:
			fileNewProjectMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::OpenProjectType:
			fileOpenProjectMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::SaveProjectType:
			fileSaveProjectMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::NewTestAppType:
			fileNewTestApplicationMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::CreateTestType:
			applicationCreateATestMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::CreateResourceTestType:
			applicationCreateAResourceTestMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::NewFaultType:
			applicationNewFaultMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::CreateReportType:
			sessionCreateReportMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::ProjectPaneType:
			viewProjectPaneMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::ResourcePaneType:
			viewResourcePaneMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::LimitsPaneType:
			viewLimitsPaneMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::FileCorruptionDetailsPaneType:
			viewFileCorruptDetailsPaneMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::NetworkMessageDetailsPaneType:
			viewNetworkMessageDetailsPaneMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::PropertiesPaneType:
			viewPropertiesPaneMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::DynamicHelpType:
			viewHelpPaneMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::FileCorruptionFaultType:
			applicationNewFileCorruptionMenuItemClicked(sender, args);
			break;
		case ButtonBarTypes::NetworkCorruptionFaultType:
			applicationNewNetworkCorruptionMenuItemClicked(sender, args);
			break;
		default:			// should never get here
			break;
		}
	}
	//*************************************************************************
	// Method:		refreshMenuItemAndButtonBarState
	// Description: Sets the state of the menu items and toolbar buttons that can
	//	have states
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::refreshMenuItemAndButtonBarState()
	{
		viewProjectPaneMenuItem->Checked = projectPane->Visible;
		projectPaneButton->Pushed = projectPane->Visible;

		viewResourcePaneMenuItem->Checked = resourcePane->Visible;
		resourcePaneButton->Pushed = resourcePane->Visible;

		viewPropertiesPaneMenuItem->Checked = propertiesPane->Visible;
		propertiesPaneButton->Pushed = propertiesPane->Visible;

		viewHelpPaneMenuItem->Checked = helpPane->Visible;
		helpPaneButton->Pushed = helpPane->Visible;

		viewLimitsPaneMenuItem->Checked = limitPane->Visible;
		limitsPaneButton->Pushed = limitPane->Visible;

		viewFileCorruptDetailsPaneMenuItem->Checked = fileCorruptDetailsPane->Visible;
		fileCorruptDetailsPaneButton->Pushed = fileCorruptDetailsPane->Visible;

		viewNetworkMessageDetailsPaneMenuItem->Checked = networkMessageDetailsPane->Visible;
		networkMessageDetailsPaneButton->Pushed = networkMessageDetailsPane->Visible;
	}


	//*************************************************************************
	// Method:		onResourcesAndLimitsTabControlCreated
	// Description: Called when a new tabcontrol is created
	//
	// Parameters:
	//	sender - originator of the event
	//	e - the event arguments representing the event
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onResourcesAndLimitsTabControlCreated(Crownwood::Magic::Controls::TabControl *tc)
	{
		//hook up selection event
		tc->SelectionChanged += new EventHandler(this, onResourcesAndLimitWindowSelectionChanged);
	}

	//*************************************************************************
	// Method:		onResourcesAndLimitWindowSelectionChanged
	// Description: Called when selection has changed in resource and limits tabcontrol
	//
	// Parameters:
	//	sender - originator of the event
	//	e - the event arguments representing the event
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onResourcesAndLimitWindowSelectionChanged(Object *sender, EventArgs *e)
	{
		DisplayableDataNode *dataNode;
		Crownwood::Magic::Controls::TabControl *tabControl = dynamic_cast <Crownwood::Magic::Controls::TabControl*> (sender);
		if (!sender)
			return;

		if (!resPane || !tabControl || !tabControl->SelectedTab)
			return;

		if (tabControl->SelectedTab->Control == resPane)
		{
			ResourcePaneDataArray *resourcePaneDataArray = resPane->CurrentDataArray;
			if (!resourcePaneDataArray)
				return;

			dataNode = new DisplayableDataNode("Resources", NodeType::Resources, resPane->ProcessID, 0);
			dataNode->DataArray = resourcePaneDataArray;
			uiEventRegistry->OnFocusChanged(dataNode);
		}
		else if (tabControl->SelectedTab->Control == limPane)
		{
			LimitPaneDataArray *limitPaneDataArray = limPane->DataArray;
			if (!limitPaneDataArray)
				return;

			dataNode = new DisplayableDataNode("Limits", NodeType::Limits, limPane->ProcessID, 0);
			dataNode->DataArray = limitPaneDataArray;
			uiEventRegistry->OnFocusChanged(dataNode);
		}
		else if (tabControl->SelectedTab->Control == fcDetailsPane)
		{
			SavedFileCorruptionDataNode *fileCorruptionDataNode = fcDetailsPane->CurrentDataNode;
			if (!fileCorruptionDataNode)
				return;

			uiEventRegistry->OnFocusChanged(fileCorruptionDataNode);
		}
		else if (tabControl->SelectedTab->Control == nmDetailsPane)
		{
			DisplayableDataNode *nlDataNode = nmDetailsPane->CurrentDataNode;
			if (!nlDataNode)
				return;

			uiEventRegistry->OnFocusChanged(nlDataNode);
		}
	}

	//*************************************************************************
	// Method:		onPaneVisibilityChanged
	// Description: Handles a docking pane's close/open event.  Updates the menu bar
	//	and button bar.
	//
	// Parameters:
	//	c - the content object that was changed
	//	args - the event arguments representing the event
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onPaneVisibilityChanged(Content *c, EventArgs *args)
	{
		this->refreshMenuItemAndButtonBarState();
	}

	//*************************************************************************
	// Method:		onWindowClosing
	// Description: Handles the main frame's window closing event
	//
	// Parameters:
	//	sender - the object that sent the event
	//	args - the event arguments representing the cancel event
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onWindowClosing(Object *sender, CancelEventArgs *args)
	{
		int result;

		if (displayTabPagesThread && displayTabPagesThread->IsAlive)
		{
			isRunning = false;
			displayTabPagesThread->Join(WAIT_FOR_THREAD_JOIN_TIMEOUT);
		}

		if (IsProjectActive)
		{
			//don't pop the dlg if in silent mode
			if (!silentMode)
			{
				result = MessageBox((HWND) this->Handle.ToInt32(), "Would you like to save your project?", "Holodeck", MB_YESNOCANCEL | MB_TOPMOST | MB_TASKMODAL);

				if (result == IDYES)
				{
					fileSaveProjectMenuItemClicked(sender, args);
				}
			}

			if (result == IDYES || result == IDNO || silentMode)
			{
				saveDockablePaneConfigs(NULL);
				this->holoScriptRegistry->terminateAppsAndRemoveAll();
				this->Dispose();
			}
			else
				args->Cancel = true;
		}
		else
		{
			saveDockablePaneConfigs(NULL);
			this->holoScriptRegistry->terminateAppsAndRemoveAll();
			this->Dispose();
		}
	}

	//*************************************************************************
	// Method:		saveDockablePaneConfigs
	// Description: Handles saving the positions and properties of the dockable panes
	//	to the registry
	//
	// Parameters:
	//	byteData - pane settings data from a file, if this param is NULL, pane
	//			settings data will be dynamically retrieved from the dock manager
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::saveDockablePaneConfigs(Byte byteData[])
	{
		Byte buffer[];
		unsigned char *unmanagedBuffer;

		try
		{
			if (byteData == NULL)
				buffer = dockManager->SaveConfigToArray();
			else
				buffer = byteData;

			unmanagedBuffer = new unsigned char[buffer->Length];
			if (unmanagedBuffer == NULL)
				return;

			for (int i = 0; i < buffer->Length; i++)
				unmanagedBuffer[i] = buffer[i];
		}
		catch(...)
		{
			//don't write key if there was a problem getting the config info
			delete[] unmanagedBuffer;
			return;
		}

		SiUtils::Registry registry(ROOT_KEY);
		SiString path = ROOT_PATH;
		if (!registry.KeyExists(path))
			registry.CreateKey(ROOT_KEY, path);

		registry.OpenKey(ROOT_KEY, ROOT_PATH);

		registry.Write(PANE_SETTINGS_VALUE_NAME, unmanagedBuffer, buffer->Length);

		registry.Close();
		delete[] unmanagedBuffer;
	}

	//*************************************************************************
	// Method:		loadDockablePaneConfigs
	// Description: Handles loading the positions and properties of the dockable panes
	//	from the registry
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::loadDockablePaneConfigs()
	{
		SiUtils::Registry registry(ROOT_KEY);
		SiString path = ROOT_PATH;
		if (!registry.KeyExists(path))
			return;

		registry.OpenKey(ROOT_KEY, ROOT_PATH);
		if (!registry.ValueExists(PANE_SETTINGS_VALUE_NAME))
		{
			registry.Close();
			return;
		}

		int size, type;
		registry.GetValueSizeAndType(PANE_SETTINGS_VALUE_NAME, size, type);

		if (size <= 0)
		{
			registry.Close();
			return;
		}

		unsigned char *unmanagedBuffer = new unsigned char[size];
		if (unmanagedBuffer == NULL)
		{
			registry.Close();
			return;
		}

		unsigned long readSize;
		readSize = size;
		registry.Read(PANE_SETTINGS_VALUE_NAME, unmanagedBuffer, readSize);
		Byte buffer[] = new Byte[readSize];

		for (unsigned long i = 0; i < readSize; i++)
			buffer[i] = unmanagedBuffer[i];

		dockManager->LoadConfigFromArray(buffer);

		registry.Close();
		delete[] unmanagedBuffer;
	}

	//**************************************************************************
	// Method:		resetPaneSettings
	// Description: Resets the PaneSettings registry key with data from a file
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::resetPaneSettings()
	{
		String *dataFile = GuiSupport::RegistryHelper::getInstance()->GetInstallationPath();
		dataFile = String::Concat (dataFile, S"\\", PANE_SETTINGS_DATA_FILE_NAME);
		
		if (File::Exists (dataFile))
		{
			StreamReader * reader = new StreamReader(dataFile);
			String * dataString = "";
			unsigned int maxBufferSize = 50000;
			Byte buffer[] = new Byte[maxBufferSize];
			
			unsigned int counter = 0;
			while (reader->Peek() > -1)
			{
				dataString = reader->ReadLine();
				for (int i=0; i < dataString->Length; i+= 2)
				{
					buffer[counter] = Byte::Parse (dataString->Substring(i,2), System::Globalization::NumberStyles::HexNumber);
					counter++;

					//if the panesettings file was corrupted or has too much data
					//then return doing nothing
					if (counter >= maxBufferSize)
						return;
				}
			}
			
			Byte sizedBuffer[] = new Byte[counter + 1];
			for (unsigned int i=0; i < counter; i++)
				sizedBuffer[i] = buffer[i];

			reader->Close();

			this->saveDockablePaneConfigs (sizedBuffer);
		}
	}

	//**************************************************************************
	// Method:		runNewProjectWizard
	// Description: Runs the new project wizard and creates the resulting
	//	data structures
	//
	// Parameters:
	//	defaultProjectWizard - contains default project options, if NULL
	//				then NewWizard dialog is displayed and data is got
	//				from the user.
	//
	// Return Value: true if the new project was successfully launched, false otherwise
	//*************************************************************************
	bool MainFrame::runNewProjectWizard(NewProjectWizard * defaultProjectWizard)
	{
		NewProjectWizard *newProjectWizard = new NewProjectWizard(true);
		
		if (defaultProjectWizard == NULL)
			uiEventRegistry->OnShowDialog(newProjectWizard);
		else
		{
			//set default options
			newProjectWizard = defaultProjectWizard;
		}

		if (newProjectWizard->DialogResult == DialogResult::OK || defaultProjectWizard != NULL)
		{
			Cursor::Current = Cursors::WaitCursor;

			CloseAllNonDockablePanes();
			
			if (defaultProjectWizard == NULL)
			{
				currentProject->Name = newProjectWizard->ProjectName;
				int index = currentProject->Name->IndexOf(".hdp");
				if (index > 0)
					currentProject->Title = currentProject->Name->Substring(0, index);
				else
					currentProject->Title = currentProject->Name;

				currentProject->FullPath = newProjectWizard->ProjectPath;
				currentProject->Folder = newProjectWizard->ProjectFolder;
			}
			else
			{
				currentProject->Name = "Untitled";
				currentProject->Title = NULL;
				currentProject->FullPath = NULL;
				currentProject->Folder = NULL;
			}

			// Create saved corrupted files array
			SavedFileCorruptionDataArray* savedFilesArray = new SavedFileCorruptionDataArray(0);
			currentProject->AddDataArray(S"Saved Fuzzed Files", savedFilesArray);

			// Create saved exceptions array
			ExceptionDataArray* savedExceptionsArray = new ExceptionDataArray(0);
			currentProject->AddDataArray(S"Saved Exceptions", savedExceptionsArray);

			IsProjectActive = true;
			this->UpdateHolodeckWindowTitle (currentProject->Title);
			bool retVal = NewTestApplicationWorker(newProjectWizard);
			
			//save the project on disk if creating a named project
			if (defaultProjectWizard == NULL)
				fileSaveProjectMenuItemClicked(NULL, NULL);

			Cursor::Current = Cursors::Default;

			return retVal;
		}

		return false;
	}

	//**************************************************************************
	// Method:		NewTestApplicationWorker
	// Description: worker function that performs tasks common to new application and new project
	//
	// Parameters:
	//	wizard - filled in wizard object
	//
	// Return Value: true if the new test app was successfully launched, false otherwise
	//*************************************************************************
	bool MainFrame::NewTestApplicationWorker(NewProjectWizard *wizard)
	{
		unsigned int pID;
		HoloScriptApplication *newHoloScriptApp;

		try
		{
			newHoloScriptApp = new HoloScriptApplication();
			newHoloScriptApp->ProjectPath = currentProject->FullPath;
			newHoloScriptApp->OnInitializeAppForProcessChaining = new InitializeAppForProcessChainingDelegate(this, onInitializeAppForProcessChaining);
			newHoloScriptApp->OnProcessChainingAttachComplete = new ProcessChainingAttachCompleteDelegate(this, onProcessChainingAttachComplete);
			newHoloScriptApp->OnThreadCreated += new ThreadCreatedDelegate(this, onThreadCreated);
			newHoloScriptApp->OnCantCreateLogFile += new CantCreateLogFileDelegate(this, onCantCreateLogFile);
			newHoloScriptApp->OnHeatProcessTerminated += new HeatProcessTerminatedDelegate(this, onHeatProcessTerminated);
			newHoloScriptApp->OnHeatProcessPaused += new HeatProcessPausedDelegate(this, onHeatProcessPaused);
			newHoloScriptApp->OnHeatProcessResumed += new HeatProcessResumedDelegate(this, onHeatProcessResumed);

			wizardToLaunchAppFrom = wizard;
			hsAppToLaunch = newHoloScriptApp;
			System::Threading::ThreadStart *threadStart = new System::Threading::ThreadStart(this, NewTestApplicationThreadFunction);
			System::Threading::Thread *thread = new System::Threading::Thread(threadStart);

			if (wizardToLaunchAppFrom->LaunchApp)
				launchProgressDlg->UserText = "Holodeck is launching the specified application.";
			else if (wizardToLaunchAppFrom->AttachToApp)
				launchProgressDlg->UserText = "Holodeck is attaching to the specified application.";
			else if (wizardToLaunchAppFrom->StartAService)
				launchProgressDlg->UserText = "Holodeck is launching the specified service.";
			else if (wizardToLaunchAppFrom->AttachToAService)
				launchProgressDlg->UserText = "Holodeck is attaching to the specified service.";

			thread->Start();
			uiEventRegistry->OnShowDialog(launchProgressDlg);

			newHoloScriptApp->ProcessChaining = wizardToLaunchAppFrom->ProcessChaining;
			newHoloScriptApp->InheritSettings = wizardToLaunchAppFrom->InheritSettings;

			CreateDataArrays(newHoloScriptApp);

			newHoloScriptApp->GetApplicationProcessID(pID);
			wizardToLaunchAppFrom->ProcessID = pID;
			newHoloScriptApp->GetApplicationProcessID(pID);

			if (!wizardToLaunchAppFrom->PauseOnStart)
			{
				newHoloScriptApp->PauseOnStart = false;
				newHoloScriptApp->ResumeApplication();
			}
			else
			{
				onHeatProcessPaused(pID);
			}

			wizardToLaunchAppFrom = NULL;
			hsAppToLaunch = NULL;

			return newHoloScriptApp->IsRunning;
		}
		catch(Exception *e)
		{
			String *message = String::Concat("Could not complete this operation.  Reason: ", e->ToString());
			UserNotification::ErrorNotify(message);
			return false;
		}
	}

	//**************************************************************************
	// Method:		NewTestApplicationThreadFunction
	// Description: the thread function to do the work to start a new test application
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::NewTestApplicationThreadFunction()
	{
		try
		{
			hsAppToLaunch->AttachDebugger = wizardToLaunchAppFrom->AttachDebugger;
			hsAppToLaunch->LogFirstChance = wizardToLaunchAppFrom->LogFirstChance;

			if (wizardToLaunchAppFrom->LaunchApp)
			{
				if (!hsAppToLaunch->LaunchApplication(wizardToLaunchAppFrom->ApplicationPath, wizardToLaunchAppFrom->CommandLineParams, true, wizardToLaunchAppFrom->FilteredFunctionList))
				{
					launchProgressDlg->CloseDialog();
					UserNotification::ErrorNotify("Holodeck could not launch the application");
					return;
				}

				GuiSupport::RegistryHelper::getInstance()->AddRecentItem(wizardToLaunchAppFrom->ApplicationPath);
			}
			else if (wizardToLaunchAppFrom->AttachToApp)
			{
				if (!hsAppToLaunch->AttachApplication(wizardToLaunchAppFrom->ProcessID, true, wizardToLaunchAppFrom->FilteredFunctionList, false))
				{
					launchProgressDlg->CloseDialog();
					UserNotification::ErrorNotify("Holodeck could not attach to the application");
					return;
				}
			}
			else if (wizardToLaunchAppFrom->StartAService)
			{
				if (!hsAppToLaunch->LaunchService(wizardToLaunchAppFrom->ServiceToStart, true, wizardToLaunchAppFrom->FilteredFunctionList))
				{
					launchProgressDlg->CloseDialog();
					UserNotification::ErrorNotify("Holodeck could not launch the service");
					return;
				}
			}
			else if (wizardToLaunchAppFrom->AttachToAService)
			{
				if (!hsAppToLaunch->AttachService(wizardToLaunchAppFrom->ServiceToAttachTo, true, wizardToLaunchAppFrom->FilteredFunctionList, false))
				{
					launchProgressDlg->CloseDialog();
					UserNotification::ErrorNotify("Holodeck could not attach to the service");
					return;
				}
			}

			launchProgressDlg->CloseDialog();
		}
		catch(InvalidOperationException *e)
		{
			launchProgressDlg->CloseDialog();

			String *message;
			if (e->Message->ToLower()->StartsWith("process performance counter"))
			{
				message = "There was a problem with the Windows internal performance counters, so Holodeck could not attach to this application or service.";
			}
			else
			{
				message = String::Concat("Holodeck could not attach to the application or service.  Reason: ", e->Message);
				message = String::Concat(message, "\n\n", e->StackTrace);
			}
			UserNotification::ErrorNotify(message);
		}
		catch(Exception *e)
		{
			launchProgressDlg->CloseDialog();

			String *message = String::Concat("Holodeck could not attach to the application or service.  Reason: ", e->Message);
			message = String::Concat(message, "\n\n", e->StackTrace);
			UserNotification::ErrorNotify(message);
			return;
		}
	}

	//**********************************************************************************
	// Method:		ApplicationRestartThreadFunction
	// Description: Worker for thread which restarts an AUT
	//
	// Parameters:
	//  None, but MainFrame::hsAppToRestart should be set to the AUT which should be
	//	restarted
	//
	// Return Value: None
	//**********************************************************************************
	void MainFrame::ApplicationRestartThreadFunction()
	{
		if (this->hsAppToRestart == NULL)
		{
			launchProgressDlg->CloseDialog();
			return;
		}

		try
		{
			unsigned int restartProcessID;
			this->hsAppToRestart->GetApplicationProcessID (restartProcessID);

			FaultPaneDataArray * faultArray;
			LimitPaneDataArray * limitArray;
			ScheduledTestDataArray * testArray;
			ResourceTestDataArray *resourceTestArray;
			NetworkCorruptionDataArray *networkCorruptionArray;
			FileCorruptionDataArray *fileCorruptionArray;

			//stop and running test generation
			StressTestGenerationStop();
			CodeCoverageTestGenerationStop();

			//GET THE DATA ARRAYS WE WANT TO SAVE AND REMOVE THEM FROM THE CURRENT PROJECT
			//FaultPaneDataArray
			DisplayableDataNode *dNode = new DisplayableDataNode("", UIData::Faults, restartProcessID, 0);
			faultArray = dynamic_cast <FaultPaneDataArray *>(currentProject->GetDataArray(dNode));		//FaultPaneDataArray
			
			//LimitPaneDataArray
			dNode = new DisplayableDataNode("", UIData::Limits, restartProcessID, 0);
			limitArray = dynamic_cast <LimitPaneDataArray *>(currentProject->GetDataArray(dNode));
			
			//ScheduledTestDataArray 
			dNode = new DisplayableDataNode("", UIData::Tests, restartProcessID, 0);
			testArray = dynamic_cast <ScheduledTestDataArray *>(currentProject->GetDataArray(dNode));

			//ResourceTestDataArray
			dNode = new DisplayableDataNode("", UIData::ResourceFaults, restartProcessID, 0);
			resourceTestArray = dynamic_cast <ResourceTestDataArray *>(currentProject->GetDataArray(dNode));

			//NetworkCorruptionDataArray
			dNode = new DisplayableDataNode("", UIData::NetworkCorruptionFault, restartProcessID, 0);
			networkCorruptionArray = dynamic_cast <NetworkCorruptionDataArray *>(currentProject->GetDataArray(dNode));

			//FileCorruptionDataArray
			dNode = new DisplayableDataNode("", UIData::FileCorruptionFault, restartProcessID, 0);
			fileCorruptionArray = dynamic_cast <FileCorruptionDataArray *>(currentProject->GetDataArray(dNode));

			//Save other data from this application object
			String *applicationPath = NULL;
			String *injectMode = NULL;
			bool isService = this->hsAppToRestart->IsService;

			if (isService)
			{
				applicationPath = this->hsAppToRestart->ServiceName;
				injectMode = this->hsAppToRestart->ApplicationAttachedTo ? "ATTACHSERVICE" : "LAUNCHSERVICE";
			}
			else
			{
				applicationPath = this->hsAppToRestart->ApplicationPath;
				injectMode = this->hsAppToRestart->ApplicationAttachedTo ? "ATTACH" : "LAUNCH";
			}
			String * commandLineParams = this->hsAppToRestart->CommandLineParams;
			ArrayList * filteredFunctionList = this->hsAppToRestart->FilteredFunctionList;
			bool processChaining = this->hsAppToRestart->ProcessChaining;
			bool inheritSettings = this->hsAppToRestart->InheritSettings;
			bool savedPauseOnStart = this->hsAppToRestart->PauseOnStart;
			bool attachDebugger = this->hsAppToRestart->AttachDebugger;
			bool logFirstChance = this->hsAppToRestart->LogFirstChance;
			
			//Kill the process and mark is as not project restartable
			unsigned int pid;
			this->hsAppToRestart->ShouldRestartWithProject = false;
			this->hsAppToRestart->GetApplicationProcessID(pid);

			if (isService)
			{
				// check to see if the user is trying to restart a service.  If they are
				// and the app to restart is not running, but the service is, tell the user
				// and return
				if (!hsAppToRestart->IsRunning)
				{
					ServiceController *service = new ServiceController(applicationPath);
					if (service && ((service->Status != ServiceControllerStatus::Stopped) && 
						(service->Status != ServiceControllerStatus::StopPending)))
					{
						UserNotification::InfoNotify("The service is running as another Holodeck process.  Please select that process and choose to restart it there instead.");

						//Hide the wait dialog
						launchProgressDlg->CloseDialog();
						return;
					}
				}
			}
			

			holoScriptRegistry->TerminateApplication(pid);

			// sometimes the service still isn't stopped.  kill it now.  have to wait or
			// we get an exception because the service would not be ready to start
			if (isService)
			{
				ServiceController *service = new ServiceController(applicationPath);
				if (service && (service->Status != ServiceControllerStatus::Stopped))
				{
					try
					{
						service->Stop();
					}
					catch(...)
					{
					}

					service->WaitForStatus(ServiceControllerStatus::Stopped, TimeSpan::FromMilliseconds(WAIT_FOR_SERVICE_TIMEOUT));
				}
			}
		
			//Set the saved data in the new application object
			this->hsAppToRestart = new HoloScriptApplication();
			this->hsAppToRestart->ProjectPath = this->currentProject->FullPath;
			this->hsAppToRestart->OnInitializeAppForProcessChaining = new InitializeAppForProcessChainingDelegate(this, onInitializeAppForProcessChaining);
			this->hsAppToRestart->OnProcessChainingAttachComplete = new ProcessChainingAttachCompleteDelegate(this, onProcessChainingAttachComplete);
			this->hsAppToRestart->OnThreadCreated += new ThreadCreatedDelegate(this, onThreadCreated);
			this->hsAppToRestart->OnHeatProcessTerminated += new HeatProcessTerminatedDelegate(this, onHeatProcessTerminated);
			this->hsAppToRestart->OnHeatProcessPaused += new HeatProcessPausedDelegate(this, onHeatProcessPaused);
			this->hsAppToRestart->OnHeatProcessResumed += new HeatProcessResumedDelegate(this, onHeatProcessResumed);
			if (isService)
				this->hsAppToRestart->LaunchService(applicationPath, true, filteredFunctionList);
			else
				this->hsAppToRestart->LaunchApplication(applicationPath, commandLineParams, true, filteredFunctionList);
			this->hsAppToRestart->ProcessChaining = processChaining;
			this->hsAppToRestart->InheritSettings = inheritSettings;
			this->hsAppToRestart->PauseOnStart = savedPauseOnStart;
			this->hsAppToRestart->AttachDebugger = attachDebugger;
			this->hsAppToRestart->LogFirstChance = logFirstChance;

			this->hsAppToRestart->GetApplicationProcessID (pid);

			//Create data arrays for this new application object
			CreateDataArrays(this->hsAppToRestart);

			Object *tObj[] = __gc new Object*[1];
			//Add saved scheduled tests
			for (int i=0; i < testArray->Count; i++)
			{
				ScheduledTestDataNode *testNode  = dynamic_cast <ScheduledTestDataNode *> (testArray->GetDataNodeByIndex(i));
				testNode->processID = pid;
				testNode->UpdateDelegates();
				tObj[0] = testNode;
				testNode->CreateDelegate->DynamicInvoke(tObj);
			}

			//Add the faults
			for (int faultCount = 0; faultCount < faultArray->get_Count(); faultCount++)
			{
				FaultPaneDataNode * faultDataNode = dynamic_cast <FaultPaneDataNode*> (faultArray->GetDataNodeByIndex(faultCount));
				faultDataNode->processID = pid;
				faultDataNode->UpdateDelegates();
				tObj[0] = faultDataNode;
				faultDataNode->CreateDelegate->DynamicInvoke(tObj);
			}

			//Add the limits
			for (int limitCount = 0; limitCount < limitArray->get_Count(); limitCount++)
			{
				LimitPaneDataNode * limitDataNode = dynamic_cast <LimitPaneDataNode*> (limitArray->GetDataNodeByIndex(limitCount));
				limitDataNode->processID = pid;
				limitDataNode->UpdateDelegates();
				tObj[0] = limitDataNode;
				limitDataNode->CreateDelegate->DynamicInvoke(tObj);
			}

			//Add the resource tests
			for (int resourceTests=0; resourceTests < resourceTestArray->Count; resourceTests++)
			{
				ResourceTestDataNode * resourceTestNode  = dynamic_cast <ResourceTestDataNode *> (resourceTestArray->GetDataNodeByIndex(resourceTests));
				resourceTestNode->processID = pid;
				resourceTestNode->UpdateDelegates();
				tObj[0] = resourceTestNode;
				resourceTestNode->CreateDelegate->DynamicInvoke(tObj);
			}
				
			//Add network corruption data
			for (int ncFault=0; ncFault < networkCorruptionArray->Count; ncFault++)
			{
				NetworkCorruptionDataNode *networkCorruptionNode  = dynamic_cast <NetworkCorruptionDataNode*> (networkCorruptionArray->GetDataNodeByIndex(ncFault));
				networkCorruptionNode->processID = pid;
				networkCorruptionNode->UpdateDelegates();
				tObj[0]= networkCorruptionNode;
				networkCorruptionNode->CreateDelegate->DynamicInvoke(tObj);
			}

			//Add the file corruption data
			for (int fcFault=0; fcFault < fileCorruptionArray->Count; fcFault++)
			{
				FileCorruptionDataNode *fileCorruptionNode  = dynamic_cast <FileCorruptionDataNode*> (fileCorruptionArray->GetDataNodeByIndex(fcFault));
				fileCorruptionNode->processID = pid;
				fileCorruptionNode->UpdateDelegates();
				tObj[0] = fileCorruptionNode;
				fileCorruptionNode->CreateDelegate->DynamicInvoke(tObj);
			}

			//Finally, resume the application if the user so requested
			if (! this->hsAppToRestart->PauseOnStart)
				this->hsAppToRestart->ResumeApplication();
		}
		catch(Exception *e)
		{
			String *message = String::Concat("Could not restart this application or service: ", e->Message);
			UserNotification::InfoNotify(message);
		}

		//Hide the wait dialog
		launchProgressDlg->CloseDialog();
	}

	//**************************************************************************
	// Method:		runNewTestApplicationWizard
	// Description: Runs the new test application wizard and creates the resulting
	//	data structures
	//
	// Parameters:
	//	None
	//
	// Return Value: true if the new test app was successfully launched, false otherwise
	//*************************************************************************
	bool MainFrame::runNewTestApplicationWizard()
	{
		NewProjectWizard *newProjectWizard = new NewProjectWizard(false);
		uiEventRegistry->OnShowDialog(newProjectWizard);

		if (newProjectWizard->DialogResult == DialogResult::OK)
		{
			Cursor::Current = Cursors::WaitCursor;
			bool retVal = NewTestApplicationWorker(newProjectWizard);
			Cursor::Current = Cursors::Default;
			return retVal;
		}

		return false;
	}

	//**************************************************************************
	// Method:		onCreateReport
	// Description: event handler that's called when a report should be created
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onCreateReport(DisplayableDataNode *dataNode)
	{
		sessionCreateReportMenuItemClicked(NULL, NULL);
	}

	//**************************************************************************
	// Method:		onStartModifyTestWizard
	// Description: event handler that's called when a test wizard should be run
	//
	// Parameters:
	//	dataNode - represents the test to modify
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onStartModifyTestWizard(DisplayableDataNode *dataNode)
	{
		ScheduledTestDataArray *testDataArray;
		ScheduledTestDataNode *testDataNode;
		HoloScriptRegistry *holoScriptRegistry = HoloScriptRegistry::getInstance();
		HoloScriptApplication *holoScriptApp = holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID);
		bool appPaused = false;
		ScheduledTestDataNode *nodeToModify = NULL;

		if (holoScriptApp)
			appPaused = holoScriptApp->ApplicationPaused;

		this->Cursor = Cursors::WaitCursor;
		if (!appPaused && holoScriptApp)
			holoScriptApp->PauseApplication();

		testDataArray = dynamic_cast<ScheduledTestDataArray *>(currentProject->GetDataArray(dataNode));
		if (!testDataArray)
		{
			this->Cursor = Cursors::Default;
			return;
		}
	
		nodeToModify = dynamic_cast <ScheduledTestDataNode*> (testDataArray->GetDataNodeByID(dataNode->ID));
		
		if (!nodeToModify)
		{
			this->Cursor = Cursors::Default;
			return;
		}

		
		NewTestWizard *wizard = new NewTestWizard(nodeToModify->GetCloneObject(), true);

		if (!wizard)
		{
			this->Cursor = Cursors::Default;
			return;
		}

		uiEventRegistry->OnShowDialog(wizard);
		if (wizard->DialogResult == DialogResult::OK)
		{
			InterceptedFunctionDB *db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
			InterceptedFunction *function = db->GetFunctionByName(wizard->Function);

			if (!function)
			{
				// Try the .NET database
				db = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
				function = db->GetFunctionByName(wizard->Function);
			}

			testDataNode = new ScheduledTestDataNode(function->Name, dataNode->processID, dataNode->threadID);
			testDataNode->Function = function->Name;
			testDataNode->Category = function->Category;
			testDataNode->ReturnValue = wizard->ReturnValue;
			testDataNode->SpecifyErrorCode = wizard->SpecifyErrorCode;
			testDataNode->ErrorCode = wizard->ErrorCode;
			testDataNode->MatchParameters = wizard->ParametersToMatch;
			testDataNode->ChangeParameters = wizard->ParametersToChange;
			testDataNode->TestOwner = "User";
			testDataNode->FiringOption = wizard->FiringOption;
			testDataNode->PauseAppOnTestFire = wizard->PauseAppOnTestFire;
			testDataNode->FiringDetails = wizard->FiringData;

			Object *args[] = __gc new Object*[3];
			args[0] = nodeToModify;
			args[1] = testDataNode;
			args[2] = Convert::ToString("All");
			testDataNode->ModifyDelegate->DynamicInvoke(args);

			DisplayTabPage(testDataNode, true);
		}

		if (!appPaused)
			holoScriptRegistry->ResumeApplication(dataNode->processID);

		this->Cursor = Cursors::Default;
	}
	
	//**************************************************************************
	// Method:		onStartCreateTestWizard
	// Description: event handler that's called when a test wizard should be run
	//
	// Parameters:
	//	dataNode - the node representing the options to create with.  If the name
	//		field of this param is non-null, it will be considered the function
	//		to start off the wizard with.
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onStartCreateTestWizard(DisplayableDataNode *dataNode)
	{
		ScheduledTestDataArray *testDataArray;
		ScheduledTestDataNode *testDataNode;
		HoloScriptRegistry *holoScriptRegistry = HoloScriptRegistry::getInstance();
		
		HoloScriptApplication *holoScriptApp = holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID);
		if (!holoScriptApp)
		{
			holoScriptApp = holoScriptRegistry->getHoloScriptAppForTerminatedProcess(dataNode->processID);
			if (!holoScriptApp)
			{
				UserNotification::InfoNotify("You can only create tests for processes started in this session.");
				return;
			}
		}

		bool appPaused = holoScriptApp->ApplicationPaused;
		LogItem *logItem;

		this->Cursor = Cursors::WaitCursor;
		if (!appPaused)
			holoScriptApp->PauseApplication();
		
		testDataNode = new ScheduledTestDataNode("", dataNode->processID, dataNode->threadID);

		testDataArray = dynamic_cast<ScheduledTestDataArray *>(currentProject->GetDataArray(testDataNode));
		if (!testDataArray)
		{
			this->Cursor = Cursors::Default;
			return;
		}

		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
		{
			if (testDataArray->HasCreatedOneTest)
			{
				this->Cursor = Cursors::Default;
				if (!appPaused)
					holoScriptApp->ResumeApplication();
				DisplayTrialTestLimitExceededNotice();
				return;
			}
		}

		//if from a log pane then fill in the datanode before passing to the wizard
		if (dataNode->Type == ActiveLogPane)
		{
			logItem = dynamic_cast <LogItem*> (dataNode->Element);
			if (logItem)
			{
				testDataNode->Category = logItem->Category;
				testDataNode->Function = logItem->Function;
				testDataNode->ReturnValue = logItem->ReturnValue;
				testDataNode->ErrorCode = logItem->ErrorCode;
				testDataNode->MatchParameters = testDataArray->ConvertToParameterArray(logItem->Parameters, testDataNode->Function, true);
			}
		}
		
		NewTestWizard *wizard = new NewTestWizard(testDataNode, false);
		if (!wizard)
			return;

		uiEventRegistry->OnShowDialog(wizard);
		if (wizard->DialogResult == DialogResult::OK)
		{
			InterceptedFunctionDB *db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
			InterceptedFunction *function;
			if (wizard->IsHiddenFunction)
				function = db->GetFunctionByName(String::Concat(wizard->Function, "A"));
			else
				function = db->GetFunctionByName(wizard->Function);

			if (!function)
			{
				// Try the .NET database
				db = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
				function = db->GetFunctionByName(wizard->Function);
			}

			testDataNode->Name = function->Name;
			testDataNode->Function = function->Name;
			testDataNode->Category = function->Category;
			testDataNode->ReturnValue = wizard->ReturnValue;
			testDataNode->SpecifyErrorCode = wizard->SpecifyErrorCode;
			testDataNode->ErrorCode = wizard->ErrorCode;
			testDataNode->MatchParameters = wizard->ParametersToMatch;
			testDataNode->ChangeParameters = wizard->ParametersToChange;
			testDataNode->TestOwner = "User";
			testDataNode->FiringOption = wizard->FiringOption;
			testDataNode->PauseAppOnTestFire = wizard->PauseAppOnTestFire;
			testDataNode->FiringDetails = wizard->FiringData;

			Object *args[] = __gc new Object*[1];
			args[0] = testDataNode;
			testDataNode->CreateDelegate->DynamicInvoke(args);

			if (wizard->IsHiddenFunction)
			{
				//replace with W and make another test
				String *functionName = function->Name;
				functionName = functionName->Remove(functionName->Length - 1, 1);
				functionName = String::Concat(functionName, "W");
				testDataNode = new ScheduledTestDataNode(functionName, dataNode->processID, dataNode->threadID);

				testDataNode->Function = functionName;
				testDataNode->Category = function->Category;
				testDataNode->ReturnValue = wizard->ReturnValue;
				testDataNode->SpecifyErrorCode = wizard->SpecifyErrorCode;
				testDataNode->ErrorCode = wizard->ErrorCode;
				testDataNode->MatchParameters = wizard->ParametersToMatch;
				testDataNode->ChangeParameters = wizard->ParametersToChange;
				testDataNode->TestOwner = "User";
				testDataNode->FiringOption = wizard->FiringOption;
				testDataNode->PauseAppOnTestFire = wizard->PauseAppOnTestFire;
				testDataNode->FiringDetails = wizard->FiringData;

				args[0] = testDataNode;
				testDataNode->CreateDelegate->DynamicInvoke(args);
			}

			DisplayTabPage(testDataNode, true);
		}

		if (!appPaused)
			holoScriptRegistry->ResumeApplication(dataNode->processID);

		this->Cursor = Cursors::Default;
	}

	//**************************************************************************
	// Method:		onStartNetworkCorruptionWizard
	// Description: event handler that's called when a network corruption
	//				wizard should be run
	//
	// Parameters:
	//	dataNode - the node representing the options to create with.
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onStartNetworkCorruptionWizard(DisplayableDataNode *dataNode)
	{
		this->Cursor = Cursors::WaitCursor;

		DisplayableDataArray *dataArray = currentProject->GetDataArray(dataNode);
		NetworkCorruptionDataArray *ncDataArray = dynamic_cast<NetworkCorruptionDataArray*>(dataArray);

		if (!ncDataArray)
		{
			UserNotification::InfoNotify("You can only create network fuzzing faults for processes started in this session.");
			return;
		}

		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
		{
			if (ncDataArray->HasCreatedOneNCFault)
			{
				this->Cursor = Cursors::Default;
				DisplayTrialTestLimitExceededNotice();
				return;
			}
		}

		NetworkCorruptionWizard *netCorruptWizard = new NetworkCorruptionWizard(NULL, false);
		uiEventRegistry->OnShowDialog(netCorruptWizard);

		if (netCorruptWizard->DialogResult == DialogResult::OK)
		{
			NetworkCorruptionDataNode *newNode = new NetworkCorruptionDataNode("Network Fuzzing", ncDataArray->ProcessID);
			newNode->SentData = netCorruptWizard->SentData;
			newNode->ReceivedData = netCorruptWizard->ReceivedData;
			newNode->PortList = netCorruptWizard->PortList;
			newNode->PortString = netCorruptWizard->PortString;
			newNode->CorruptType = netCorruptWizard->CorruptType;
			newNode->SearchFor = netCorruptWizard->SearchFor;
			newNode->SearchRegularExpression = netCorruptWizard->SearchRegularExpression;
			newNode->ReplaceWith = netCorruptWizard->ReplaceWith;
			newNode->ReplacementString = netCorruptWizard->ReplacementString;
			newNode->UISettings = netCorruptWizard->Settings;
			ncDataArray->BeginCreateNode(newNode);
			DisplayTabPage(dataNode, true);
		}

		this->Cursor = Cursors::Default;
	}

	//**************************************************************************
	// Method:		onStartModifyNetworkCorruptionWizard
	// Description: event handler that's called when a network corruption
	//				wizard should be run
	//
	// Parameters:
	//	dataNode - the node representing the options to modify.
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onStartModifyNetworkCorruptionWizard(DisplayableDataNode *dataNode)
	{
		this->Cursor = Cursors::WaitCursor;

		DisplayableDataArray *dataArray = currentProject->GetDataArray(dataNode);
		NetworkCorruptionDataArray *ncDataArray = dynamic_cast<NetworkCorruptionDataArray*>(dataArray);

		if (!ncDataArray)
		{
			this->Cursor = Cursors::Default;
			return;
		}
	
		NetworkCorruptionDataNode* nodeToModify = dynamic_cast <NetworkCorruptionDataNode*>
			(ncDataArray->GetDataNodeByID(dataNode->ID));
		if (!nodeToModify)
		{
			this->Cursor = Cursors::Default;
			return;
		}

		NetworkCorruptionWizard *netCorruptWizard = new NetworkCorruptionWizard(nodeToModify, true);
		uiEventRegistry->OnShowDialog(netCorruptWizard);

		if (netCorruptWizard->DialogResult == DialogResult::OK)
		{
			NetworkCorruptionDataNode *newNode = new NetworkCorruptionDataNode("Network Fuzzing", ncDataArray->ProcessID);
			newNode->SentData = netCorruptWizard->SentData;
			newNode->ReceivedData = netCorruptWizard->ReceivedData;
			newNode->PortList = netCorruptWizard->PortList;
			newNode->PortString = netCorruptWizard->PortString;
			newNode->CorruptType = netCorruptWizard->CorruptType;
			newNode->SearchFor = netCorruptWizard->SearchFor;
			newNode->SearchRegularExpression = netCorruptWizard->SearchRegularExpression;
			newNode->ReplaceWith = netCorruptWizard->ReplaceWith;
			newNode->ReplacementString = netCorruptWizard->ReplacementString;
			newNode->UISettings = netCorruptWizard->Settings;

			Object *args[] = __gc new Object*[3];
			args[0] = nodeToModify;
			args[1] = newNode;
			args[2] = Convert::ToString("All");
			newNode->ModifyDelegate->DynamicInvoke(args);

			DisplayTabPage(dataNode, true);
		}

		this->Cursor = Cursors::Default;
	}

	//**************************************************************************
	// Method:		onStartFileCorruptionWizard
	// Description: event handler that's called when a file corruption
	//				wizard should be run
	//
	// Parameters:
	//	dataNode - the node representing the options to create with.
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onStartFileCorruptionWizard(DisplayableDataNode *dataNode)
	{
		this->Cursor = Cursors::WaitCursor;

		DisplayableDataArray *dataArray = currentProject->GetDataArray(dataNode);
		FileCorruptionDataArray *ncDataArray = dynamic_cast<FileCorruptionDataArray*>(dataArray);

		if (!ncDataArray)
		{
			UserNotification::InfoNotify("You can only create file fuzzing faults for processes started in this session.");
			return;
		}

		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
		{
			if (ncDataArray->HasCreatedOneNCFault)
			{
				this->Cursor = Cursors::Default;
				DisplayTrialTestLimitExceededNotice();
				return;
			}
		}

		FileCorruptionWizard *fileCorruptWizard = new FileCorruptionWizard(dynamic_cast <FileCorruptionDataNode*> (dataNode), false);
		uiEventRegistry->OnShowDialog(fileCorruptWizard);

		if (fileCorruptWizard->DialogResult == DialogResult::OK)
		{
			FileCorruptionDataNode *newNode = new FileCorruptionDataNode("File Fuzzing", ncDataArray->ProcessID);
			newNode->OriginalFile = fileCorruptWizard->OriginalFile;
			newNode->CorruptType = fileCorruptWizard->CorruptType;
			newNode->SearchFor = fileCorruptWizard->SearchFor;
			newNode->SearchRegularExpression = fileCorruptWizard->SearchRegularExpression;
			newNode->ReplaceWith = fileCorruptWizard->ReplaceWith;
			newNode->ReplacementString = fileCorruptWizard->ReplacementString;
			newNode->Regenerate = fileCorruptWizard->Regenerate;
			newNode->UISettings = fileCorruptWizard->Settings;
			ncDataArray->BeginCreateNode(newNode);
			DisplayTabPage(dataNode, true);
		}

		this->Cursor = Cursors::Default;
	}

	//**************************************************************************
	// Method:		onStartModifyFileCorruptionWizard
	// Description: event handler that's called when a file corruption
	//				wizard should be run
	//
	// Parameters:
	//	dataNode - the node representing the options to modify.
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onStartModifyFileCorruptionWizard(DisplayableDataNode *dataNode)
	{
		this->Cursor = Cursors::WaitCursor;

		DisplayableDataArray *dataArray = currentProject->GetDataArray(dataNode);
		FileCorruptionDataArray *ncDataArray = dynamic_cast<FileCorruptionDataArray*>(dataArray);

		if (!ncDataArray)
		{
			this->Cursor = Cursors::Default;
			return;
		}
	
		FileCorruptionDataNode* nodeToModify = dynamic_cast <FileCorruptionDataNode*>
			(ncDataArray->GetDataNodeByID(dataNode->ID));
		if (!nodeToModify)
		{
			this->Cursor = Cursors::Default;
			return;
		}

		FileCorruptionWizard *fileCorruptWizard = new FileCorruptionWizard(nodeToModify, true);
		uiEventRegistry->OnShowDialog(fileCorruptWizard);

		if (fileCorruptWizard->DialogResult == DialogResult::OK)
		{
			FileCorruptionDataNode *newNode = new FileCorruptionDataNode("File Fuzzing", ncDataArray->ProcessID);
			newNode->OriginalFile = fileCorruptWizard->OriginalFile;
			newNode->CorruptType = fileCorruptWizard->CorruptType;
			newNode->SearchFor = fileCorruptWizard->SearchFor;
			newNode->SearchRegularExpression = fileCorruptWizard->SearchRegularExpression;
			newNode->ReplaceWith = fileCorruptWizard->ReplaceWith;
			newNode->ReplacementString = fileCorruptWizard->ReplacementString;
			newNode->Regenerate = fileCorruptWizard->Regenerate;
			newNode->UISettings = fileCorruptWizard->Settings;

			Object *args[] = __gc new Object*[3];
			args[0] = nodeToModify;
			args[1] = newNode;
			args[2] = Convert::ToString("All");
			newNode->ModifyDelegate->DynamicInvoke(args);

			DisplayTabPage(dataNode, true);
		}

		this->Cursor = Cursors::Default;
	}

	//**************************************************************************
	// Method:		onStartCreateResourceTestWizard
	// Description: event handler that's called when a resource test wizard should be run
	//
	// Parameters:
	//	dataNode - the node representing the resource to start the wizard with
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onStartCreateResourceTestWizard(ResourceTestDataNode *dataNode)
	{
		if (!dataNode)
			return;
 
		ResourceTestDataArray *resourceTestDataArray;
		ResourcePaneDataArray *resourcePaneDataArray;
		HoloScriptRegistry *holoScriptRegistry = HoloScriptRegistry::getInstance();
		
		HoloScriptApplication *holoScriptApp = holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID);
		if (!holoScriptApp)
		{
			UserNotification::InfoNotify("You can only create tests for processes started in this session.");
			return;
		}

		bool appPaused = holoScriptApp->ApplicationPaused;

		this->Cursor = Cursors::WaitCursor;
		if (!appPaused)
			holoScriptApp->PauseApplication();
		
		resourceTestDataArray = dynamic_cast<ResourceTestDataArray*>(currentProject->GetDataArray(dataNode));
		DisplayableDataNode *resourceDataNode = new DisplayableDataNode("", UIData::Resources, dataNode->processID, 0);
		resourcePaneDataArray = dynamic_cast<ResourcePaneDataArray*>(currentProject->GetDataArray(resourceDataNode));
		if (!resourceTestDataArray || !resourcePaneDataArray)
		{
			this->Cursor = Cursors::Default;
			return;
		}

		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
		{
			if (resourceTestDataArray->HasCreatedOneTest)
			{
				this->Cursor = Cursors::Default;
				if (!appPaused)
					holoScriptApp->ResumeApplication();
				DisplayTrialTestLimitExceededNotice();
				return;
			}
		}
		
		NewResourceTestWizard *wizard = new NewResourceTestWizard(resourcePaneDataArray->ResourcePaths, dataNode, false);
		if (!wizard)
			return;

		uiEventRegistry->OnShowDialog(wizard);
		if (wizard->DialogResult == DialogResult::OK)
		{
			for (int i=0; i<wizard->ResourcePaths->Count; i++)
			{
				ResourceTestDataNode *resourceTestDataNode = new ResourceTestDataNode(dynamic_cast <String*> (wizard->ResourcePaths->Item[i]), wizard->FaultName, wizard->FaultType, dataNode->processID, 0);

				Object *args[] = __gc new Object*[1];
				args[0] = resourceTestDataNode;
				resourceTestDataNode->CreateDelegate->DynamicInvoke(args);
			}

			DisplayTabPage(dataNode, true);
		}

		if (!appPaused)
			holoScriptRegistry->ResumeApplication(dataNode->processID);

		this->Cursor = Cursors::Default;
	}

	//**************************************************************************
	// Method:		onStartModifyResourceTestWizard
	// Description: event handler that's called when a resource test wizard should be run
	//
	// Parameters:
	//	dataNode - represents the test to modify
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onStartModifyResourceTestWizard(ResourceTestDataNode *dataNode)
	{
		ResourceTestDataArray *resourceTestDataArray;
		ResourcePaneDataArray *resourcePaneDataArray;
		HoloScriptRegistry *holoScriptRegistry = HoloScriptRegistry::getInstance();
		HoloScriptApplication *holoScriptApp = holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID);
		bool appPaused = false;
		ResourceTestDataNode *nodeToModify = NULL;

		if (holoScriptApp)
			appPaused = holoScriptApp->ApplicationPaused;

		this->Cursor = Cursors::WaitCursor;
		if (!appPaused && holoScriptApp)
			holoScriptApp->PauseApplication();

		resourceTestDataArray = dynamic_cast<ResourceTestDataArray *>(currentProject->GetDataArray(dataNode));
		DisplayableDataNode *resourceDataNode = new DisplayableDataNode("", UIData::Resources, dataNode->processID, 0);
		resourcePaneDataArray = dynamic_cast<ResourcePaneDataArray*>(currentProject->GetDataArray(resourceDataNode));
		if (!resourceTestDataArray || !resourcePaneDataArray)
		{
			this->Cursor = Cursors::Default;
			return;
		}
	
		nodeToModify = dynamic_cast <ResourceTestDataNode*> (resourceTestDataArray->GetDataNodeByID(dataNode->ID));
		if (!nodeToModify)
		{
			this->Cursor = Cursors::Default;
			return;
		}

		NewResourceTestWizard *wizard = new NewResourceTestWizard(resourcePaneDataArray->ResourcePaths, nodeToModify, true);
		if (!wizard)
		{
			this->Cursor = Cursors::Default;
			return;
		}

		uiEventRegistry->OnShowDialog(wizard);
		if (wizard->DialogResult == DialogResult::OK)
		{
			ResourceTestDataNode *resourceTestNode = new ResourceTestDataNode(dynamic_cast <String*> (wizard->ResourcePaths->Item[0]), wizard->FaultName, wizard->FaultType, dataNode->processID, 0);

			Object *args[] = __gc new Object*[3];
			args[0] = nodeToModify;
			args[1] = resourceTestNode;
			args[2] = Convert::ToString("All");
			resourceTestNode->ModifyDelegate->DynamicInvoke(args);

			DisplayTabPage(dataNode, true);
		}

		if (!appPaused)
			holoScriptRegistry->ResumeApplication(dataNode->processID);

		this->Cursor = Cursors::Default;
	}

	//**************************************************************************
	// Method:		onDeleteRecordedSession
	// Description: event handler that's called when a recorded session should be deleted
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onDeleteRecordedSession(DisplayableDataNode *dataNode)
	{
		RecordedSessionDataArray *recordedDataArray = dynamic_cast <RecordedSessionDataArray*>(currentProject->GetDataArray(dataNode));
		if (recordedDataArray)
		{
			recordedDataArray->DeleteRecordedSession();
			currentProject->RemoveDataArray(recordedDataArray);
			delete recordedDataArray;

			Object *args[] = __gc new Object*[1];
			args[0] = dataNode;
			dataNode->DeletedDelegate->DynamicInvoke(args);	
		}
	}

	//**************************************************************************
	// Method:		onUrlLinkClicked
	// Description: called when the user clicks on a link and wants to go to a
	//	new location
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onUrlLinkClicked(String *url)
	{
		nonDockablePaneManager->displayHelpPane(url);
	}

	//**************************************************************************
	// Method:		onShowDialog
	// Description: called when a modal dialog needs to be displayed
	//
	// Parameters:
	//	formToDisplay - the form to display
	//
	// Return Value: None
	//*************************************************************************
	System::Windows::Forms::DialogResult MainFrame::onShowDialog(Form *formToDisplay)
	{
		System::Windows::Forms::DialogResult result = DialogResult::OK;
		if (formToDisplay)
		{
			if (findNextDialog)
				findNextDialog->TopMost = false;

			if (netLogFindNextDialog)
				netLogFindNextDialog->TopMost = false;

			MethodInfo * customShow = formToDisplay->GetType()->GetMethod ("ShowDialogCustom", static_cast <BindingFlags> (BindingFlags::Public|BindingFlags::Instance));
			if (customShow)
			{
				customShow->Invoke (formToDisplay, NULL);
				result = DialogResult::OK;
			}
			else
				result = formToDisplay->ShowDialog();

			if (netLogFindNextDialog)
				netLogFindNextDialog->TopMost = true;

			if (findNextDialog)
				findNextDialog->TopMost = true;
		}
		return result;
	}

	//**************************************************************************
	// Method:		onShowCommonDialog
	// Description: called when a common dialog needs to be displayed
	//
	// Parameters:
	//	formToDisplay - the form to display
	//
	// Return Value: None
	//*************************************************************************
	System::Windows::Forms::DialogResult MainFrame::onShowCommonDialog(CommonDialog *formToDisplay)
	{
		System::Windows::Forms::DialogResult result = DialogResult::OK;
		if (formToDisplay)
		{
			if (findNextDialog)
				findNextDialog->TopMost = false;
				
			if (netLogFindNextDialog)
				netLogFindNextDialog->TopMost = false;

				result = formToDisplay->ShowDialog();

			if (netLogFindNextDialog)
				netLogFindNextDialog->TopMost = true;
	
			if (findNextDialog)			
				findNextDialog->TopMost = true;
		}
		return result;
	}

	//**************************************************************************
	// Method:		onCantCreateLogFile
	// Description: called when the aut doesn't have permissions to create the
	//	log file
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onCantCreateLogFile()
	{
		String *url = String::Concat("file://", GuiSupport::RegistryHelper::getInstance()->GetInstallationPath(), "/");
		url = String::Concat(url, CANT_CREATE_LOG_HELP_FILE);
		HtmlDialog *dialog = new HtmlDialog(url);
		uiEventRegistry->OnShowDialog(dialog);
	}

	//**********************************************************************************
	// Method:		onCopyToClipboard
	// Description: called by ui elements to invoke the copy to clipboard routine
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//**********************************************************************************
	void MainFrame::onCopyToClipboard()
	{
		this->applicationCopySelectedItemMenuItemClicked(NULL, NULL);
	}

	//**************************************************************************
	// Method:		RecordedSessionStart
	// Description: event handler that's called when a recorded session should started
	//	always create a new session.  Not possible to stop recording and restart
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::RecordedSessionStart(DisplayableDataNode *dataNode)
	{
		RecordedSessionDataArray *recordedSessionData;
		String *sessionName;
		int recordedSessionCount = 0;

		if (!holoScriptRegistry->AnyAppsRunning)
		{
			UserNotification::InfoNotify("Cannot run start a recorded session as there are no running test applications");
			return;
		}

		recordedSessionData = new RecordedSessionDataArray(holoScriptRegistry);
		currentProject->CurrentRecordingSession = recordedSessionData;

		do
		{
			sessionName = "Recorded Session #";
			recordedSessionCount++;
			sessionName = String::Concat(sessionName, Convert::ToString(recordedSessionCount));
		} while (currentProject->ContainsDataArrayKey(sessionName));
			
		currentProject->AddDataArray(sessionName, recordedSessionData);

		recordedSessionData->StartRecording(String::Concat(currentProject->Folder, RECORDED_SESSION_FOLDER), sessionName);

		dataNode->Name = sessionName;
		dataNode->DataArray = recordedSessionData;
		Object *args[] = __gc new Object*[1];
		args[0] = dataNode;
		dataNode->CreatedDelegate->DynamicInvoke(args);
	}

	//**************************************************************************
	// Method:		RecordedSessionStop
	// Description: event handler that's called when a recorded session should stopped
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::RecordedSessionStop(DisplayableDataNode *dataNode)
	{
		if (currentProject->CurrentRecordingSession)
			currentProject->CurrentRecordingSession->StopRecording();
	}

	//**************************************************************************
	// Method:		RecordedSessionReplayStart
	// Description: event handler that's called when a recorded session should be replayed
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::RecordedSessionReplayStart(DisplayableDataNode *dataNode)
	{
		if (dataNode)
			currentProject->CurrentRecordingSession = dynamic_cast <RecordedSessionDataArray*>(currentProject->GetDataArray(dataNode));

		if (!currentProject->CurrentRecordingSession)
		{
			UserNotification::InfoNotify("Please select a recorded session node in the project pane");
			return;
		}

		CodeCoverageTestGenerationStop();
		StressTestGenerationStop();

		//don't pop the dlg if in silent mode
		if (!silentMode)
		{
			int result = MessageBox(NULL, "Would you like to save your project before starting the replay?", "Holodeck", MB_YESNO | MB_TOPMOST | MB_TASKMODAL);

			//If want to save, then save the project
			if (result == IDYES)
			{
				fileSaveProjectAsMenuItemClicked(NULL, NULL);
			}
		}

		currentProject->CurrentRecordingSession->StartReplay();
	}

	//**************************************************************************
	// Method:		RecordedSessionReplayStop
	// Description: event handler that's called when a recorded session replay should stop
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::RecordedSessionReplayStop(DisplayableDataNode *dataNode)
	{
		if (currentProject->CurrentRecordingSession)
			currentProject->CurrentRecordingSession->StopReplay();
	}

	//**************************************************************************
	// Method:		RecordedSessionStarted
	// Description: event handler that's called when a recorded session has started
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::RecordedSessionStarted(DisplayableDataNode *dataNode)
	{
		sessionRecordSessionMenuItem->Text = "&Stop Recording";
		sessionReplaySessionMenuItem->Enabled = false;
	}

	//**************************************************************************
	// Method:		RecordedSessionStopped
	// Description: event handler that's called when a recorded session has stopped
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::RecordedSessionStopped(DisplayableDataNode *dataNode)
	{
		sessionRecordSessionMenuItem->Text = "&Record Session";
		sessionReplaySessionMenuItem->Enabled = true;
	}

	//**************************************************************************
	// Method:		RecordedSessionReplayStarted
	// Description: event handler that's called when a recorded session is replayed
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::RecordedSessionReplayStarted(DisplayableDataNode *dataNode)
	{
		sessionReplaySessionMenuItem->Text = "&Stop Replay";
		sessionRecordSessionMenuItem->Enabled = false;
	}

	//**************************************************************************
	// Method:		RecordedSessionReplayStopped
	// Description: event handler that's called when a recorded session replay has stopped
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::RecordedSessionReplayStopped(DisplayableDataNode *dataNode)
	{
		sessionReplaySessionMenuItem->Text = "Re&play Session";
		sessionRecordSessionMenuItem->Enabled = true;
	}

	//**************************************************************************
	// Method:		CCTestGenStopped
	// Description: event handler that's called when code coverage test generation has stopped
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::CCTestGenStopped()
	{
		sessionCodeCoverageTestGeneratorMenuItem->Text = "&Code Coverage Test Generator...";
		sessionStressTestGeneratorMenuItem->Enabled = true;
		codeCoverageRunning = false;
	}

	//**************************************************************************
	// Method:		StressTestGenStopped
	// Description: event handler that's called when stress test generation has stopped
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::StressTestGenStopped()
	{
		sessionStressTestGeneratorMenuItem->Text = "&Stress Test Generator...";
		sessionCodeCoverageTestGeneratorMenuItem->Enabled = true;
	}


	//**************************************************************************
	// Method:		onSaveReport
	// Description: event handler that's called when a report should be saved
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onSaveReport(DisplayableDataNode *dataNode)
	{
		SaveFileDialog *saveDialog = new SaveFileDialog();
		ReportPaneDataArray *reportPaneData = dynamic_cast <ReportPaneDataArray*>(currentProject->GetDataArray(dataNode));
		DisplayableDataNode *modifiedDataNode;
		String *propertyName = "Name";
	
		if (reportPaneData)
		{
			saveDialog->AddExtension = true;
			saveDialog->Filter = "Holodeck Reports (*.htm)|*.htm";
			saveDialog->DefaultExt = "*.htm";
			saveDialog->Title = "Save Holdeck Report";
			saveDialog->CheckPathExists = true;
			saveDialog->FileName = reportPaneData->ReportPath;

			if (saveDialog->ShowDialog(this) == DialogResult::OK)
			{
				reportPaneData->set_ReportPath(saveDialog->FileName);
				modifiedDataNode = new DisplayableDataNode(dataNode);
				modifiedDataNode->Name = reportPaneData->ReportName;
				
				Object *args[] = __gc new Object*[3];
				args[0] = dataNode;
				args[1] = modifiedDataNode;
				args[2] = propertyName;
				dataNode->ModifiedDelegate->DynamicInvoke(args);
			}
		}
		else
		{
			UserNotification::ErrorNotify("Can't find report data to save");
		}

	}

	//**************************************************************************
	// Method:		onSaveLogPane
	// Description: event handler that's called when a log pane should be saved
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onSaveLogPane(DisplayableDataNode *dataNode)
	{
		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
			DisplayTrialDisabledNotice(EXPORT_LOG_HELP_FILE);
		else
		{
			if (dataNode)
			{
				LogPaneDataArray *dataArray = dynamic_cast <LogPaneDataArray*>(currentProject->GetDataArray(dataNode));
				int procID = dataNode->processID;
				HoloScriptRegistry * hsReg = HoloScriptRegistry::getInstance();
				HoloScriptApplication * hsApp = hsReg->getHoloScriptAppForProcess(procID);

				if (!hsApp)
					hsApp = hsReg->getHoloScriptAppForTerminatedProcess(procID);

				if (!dataArray)
				{
					UserNotification::InfoNotify("Please select a log pane or a project pane log node");
					return;
				}

				// check to see if this is a saved log
				bool isSavedLogFile = false;
				if (!hsApp)
				{
					hsApp = new HoloScriptApplication();
					if (!hsApp || !dataArray || !dataArray->SavedLog)
						return;

					isSavedLogFile = true;
				}
				System::Windows::Forms::SaveFileDialog *exportLogDlg = new System::Windows::Forms::SaveFileDialog();
				exportLogDlg->Title = S"Exporting API Log ...";
				exportLogDlg->AddExtension = true;
				exportLogDlg->Filter = "Excel Comma Separated Value File (*.csv)|*.csv";
				exportLogDlg->DefaultExt = "*.csv";
				exportLogDlg->CheckPathExists = true;
				exportLogDlg->InitialDirectory = currentProject->Folder;

				if (exportLogDlg->ShowDialog(this) == DialogResult::OK)
				{
					if (dataArray && isSavedLogFile)
						dataArray->ExportSavedLogItemsToFile(exportLogDlg->FileName);
					else if (dataArray)
						dataArray->ExportDataToFile(exportLogDlg->FileName);
					else
						hsApp->ExportToDiffFile(exportLogDlg->FileName, NULL);
				}
			}
		}
	}

	//**************************************************************************
	// Method:		onSaveNetworkLogPane
	// Description: event handler that's called when a network log pane
	//				should be saved
	//
	// Parameters:
	//	networkLogPane - the logpane which should be exported
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onSaveNetworkLogPane(DisplayableDataNode *dataNode)
	{
		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
			DisplayTrialDisabledNotice(EXPORT_LOG_HELP_FILE);
		else
		{
			if (dataNode)
			{
				NetworkLogPaneDataArray *dataArray = dynamic_cast <NetworkLogPaneDataArray*>(currentProject->GetDataArray(dataNode));

				System::Windows::Forms::SaveFileDialog *exportLogDlg = new System::Windows::Forms::SaveFileDialog();
				exportLogDlg->Title = S"Exporting Network Log ...";
				exportLogDlg->AddExtension = true;
				exportLogDlg->Filter = "Excel Comma Separated Value File (*.csv)|*.csv";
				exportLogDlg->DefaultExt = "*.csv";
				exportLogDlg->CheckPathExists = true;
				exportLogDlg->InitialDirectory = currentProject->Folder;

				if (exportLogDlg->ShowDialog(this) == DialogResult::OK)
					dataArray->ExportToFile(exportLogDlg->FileName);
			}
		}
	}

	//**************************************************************************
	// Method:		onSaveResourcePane
	// Description: event handler that's called when a resource pane should be saved
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onSaveResourcePane()
	{
		ResourcePaneDataArray *dataArray = resPane->CurrentDataArray;
		if (!dataArray)
		{
			UserNotification::ErrorNotify("There are currently no resources to export");
			return;
		}
		else
		{
			System::Windows::Forms::SaveFileDialog *exportResourcesDlg = new System::Windows::Forms::SaveFileDialog();
			exportResourcesDlg->Title = S"Exporting Resources ...";
			exportResourcesDlg->AddExtension = true;
			exportResourcesDlg->Filter = "Excel Comma Separated Value File (*.csv)|*.csv";
			exportResourcesDlg->DefaultExt = "*.csv";
			exportResourcesDlg->CheckPathExists = true;
			exportResourcesDlg->InitialDirectory = this->currentProject->Folder;

			if (exportResourcesDlg->ShowDialog(this) == DialogResult::OK)
				dataArray->ExportToFile(exportResourcesDlg->FileName);
		}
	}

	//**************************************************************************
	// Method:		onDeletePane
	// Description: event handler that's called when a pane should be deleted
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onDeletePane(DisplayableDataNode *dataNode)
	{
		Crownwood::Magic::Controls::TabPage *tabPage;
		ReportPaneDataArray *reportDataArray;
		LogPaneDataArray *logDataArray;
		NetworkLogPaneDataArray *netlogDataArray;
		bool isOkayToDeleteFile = false;
		DisplayableDataNode * tmpDataNode;

		switch (dataNode->Type)
		{
		case Report:
			tabPage = GetTabPage(dataNode);
			if (tabPage)
				nonDockablePaneManager->TabPages->Remove(tabPage);

			reportDataArray = dynamic_cast <ReportPaneDataArray*>(currentProject->GetDataArray(dataNode));
			if (!reportDataArray)
				return;
			
			reportDataArray->DeleteReport();
			currentProject->RemoveDataArray(reportDataArray);
			reportDataArray->CleanUp();
			break;
		case SavedLogPane:
			tabPage = GetTabPage(dataNode);
			if (tabPage)
				nonDockablePaneManager->TabPages->Remove(tabPage);

			logDataArray = dynamic_cast <LogPaneDataArray*>(currentProject->GetDataArray(dataNode));
			if (!logDataArray)
				return;
			
			tmpDataNode = new DisplayableDataNode(dataNode);
			tmpDataNode->Type = UIData::SavedNetworkLogPane;
			netlogDataArray = dynamic_cast <NetworkLogPaneDataArray*>(currentProject->GetDataArray(tmpDataNode));

			//remove the file from disk only if the saved netowork log node has been removed as well
			if (!netlogDataArray || !netlogDataArray->SavedLog)
			{
				try
				{
					if (!logDataArray->DeleteSavedLog())
						UserNotification::InfoNotify(String::Concat (S"Could not remove the saved log file from disk. Please remove the file manually\r\n\r\nFilename: ", logDataArray->SavedLogFilePath));
				}
				catch (Exception *)
				{
					UserNotification::InfoNotify(String::Concat (S"Could not remove the saved log file from disk. Please remove the file manually\r\n\r\nFilename: ", logDataArray->SavedLogFilePath));
				}
			}

			logDataArray->SavedLog = false;
			currentProject->RemoveDataArray(logDataArray);
			break;
		case SavedNetworkLogPane:
			tabPage = GetTabPage(dataNode);
			if (tabPage)
				nonDockablePaneManager->TabPages->Remove(tabPage);

			netlogDataArray = dynamic_cast <NetworkLogPaneDataArray*>(currentProject->GetDataArray(dataNode));
			if (!netlogDataArray)
				return;
			
			tmpDataNode = new DisplayableDataNode(dataNode);
			tmpDataNode->Type = UIData::SavedLogPane;
			logDataArray = dynamic_cast <LogPaneDataArray*>(currentProject->GetDataArray(tmpDataNode));

			//remove the file from disk only if the saved netowork log node has been removed as well
			if (!logDataArray || !logDataArray->SavedLog)
			{
				try
				{
					if (!netlogDataArray->DeleteSavedLog())
						UserNotification::InfoNotify(String::Concat (S"Could not remove the saved log file from disk. Please remove the file manually\r\n\r\nFilename: ", logDataArray->SavedLogFilePath));
				}
				catch (Exception *)
				{
					UserNotification::InfoNotify(String::Concat (S"Could not remove the saved log file from disk. Please remove the file manually\r\n\r\nFilename: ", logDataArray->SavedLogFilePath));
				}
			}
			netlogDataArray->SavedLog = false;
			currentProject->RemoveDataArray(netlogDataArray);
			break;
		}
		
		if (!dataNode->DeletedDelegate)
			return;
		else
		{
			Object *args[] = __gc new Object*[1];
			args[0] = dataNode;
			dataNode->DeletedDelegate->DynamicInvoke(args);
		}
	}

	//**************************************************************************
	// Method:		onDoubleClick
	// Description: event handler that's called when a project pane node is doubleclicked
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onDoubleClick(DisplayableDataNode *dataNode)
	{
		DisplayTabPage(dataNode, true);
	}

	//**************************************************************************
	// Method:		onHelpPaneLinkClicked
	// Description: event handler that's called when a link in the dynamic
	//	help pane is clicked
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onHelpPaneLinkClicked(Object *sender, String *url)
	{
		DisplayableDataNode *newNode = new DisplayableDataNode(url, UIData::HelpDetails, -1, -1);
		DisplayTabPage(newNode, true);
	}

	//**************************************************************************
	// Method:		GetTabPage
	// Description: Given a title, procID and threadID returns the desired tab page
	//
	// Parameters:
	//	dataNode - node that represents the page we want to get
	//
	// Return Value: tab page if found null, if not
	//*************************************************************************
	Crownwood::Magic::Controls::TabPage* MainFrame::GetTabPage(DisplayableDataNode *dataNode)
	{
		Crownwood::Magic::Controls::TabPage *tabPage;
		String *typeName, *tabName;

		HoloScriptApplication *holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(dataNode->processID);
		if (!holoScriptApp)
			holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForTerminatedProcess(dataNode->processID);
		
		switch (dataNode->Type)
		{
		case ActiveLogPane:
		case SavedLogPane:
			typeName = " Log";
			break;
		default:
			break;
		}

		if (holoScriptApp && (dataNode->Type == Tests) || (dataNode->Type == Test))
			tabName = String::Concat(holoScriptApp->ApplicationName, " Tests");
		else
			tabName = String::Concat(dataNode->Name, typeName);

		if (dataNode->processID != 0)
		{
			if (holoScriptApp && holoScriptApp->IsService)
			{
				tabName = String::Concat(tabName, " (");
				tabName = String::Concat(tabName, holoScriptApp->ServiceName);
				tabName = String::Concat(tabName, ") ");
			}

			char *procIDString = new char[12];
			char *threadIDString = new char[12];
			procIDString = itoa(dataNode->processID, procIDString, 10);
			threadIDString = itoa(dataNode->threadID , threadIDString, 10);
			tabName = String::Concat(tabName, " - ");
			tabName = String::Concat(tabName, procIDString);
			tabName = String::Concat(tabName, "(");
			if (Int32::Parse (threadIDString) != 0)
				tabName = String::Concat(tabName, threadIDString);
			else
				tabName = String::Concat(tabName, "All Threads");
			tabName = String::Concat(tabName, ")");
			delete procIDString;
		}
		Crownwood::Magic::Collections::TabPageCollection *pages = nonDockablePaneManager->TabPages;
		tabPage = pages->get_Item(tabName);

		return tabPage;
	}
	
	//**************************************************************************
	// Method:		ConstructTabTitle
	// Description: Given a title, procID and threadID returns the desired tab page
	//
	// Parameters:
	//	dataNode - node representing the tab page we are creating a title for
	//
	// Return Value: title of the new tab page.
	//*************************************************************************
	String* MainFrame::ConstructTabTitle(DisplayableDataNode *dataNode)
	{
		String *tabName;
		String *typeName;

		HoloScriptApplication *holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(dataNode->processID);
		if (!holoScriptApp)
			holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForTerminatedProcess(dataNode->processID);

		switch (dataNode->Type)
		{
		case ActiveLogPane:
		case SavedLogPane:
			typeName = " Log";
			break;
		default:
			break;
		}

		if (holoScriptApp && ((dataNode->Type == Tests) || (dataNode->Type == Test)) )
		{
			tabName = String::Concat(holoScriptApp->ApplicationName, " Tests");
		}
		else
			tabName = String::Concat(dataNode->Name, typeName);
		
		if (dataNode->processID != 0)
		{
			if (holoScriptApp && holoScriptApp->IsService)
			{
				tabName = String::Concat(tabName, " (");
				tabName = String::Concat(tabName, holoScriptApp->ServiceName);
				tabName = String::Concat(tabName, ") ");
			}

			char *procIDString = new char[12];
			char *threadIDString = new char[12];
			procIDString = itoa(dataNode->processID, procIDString, 10);
			threadIDString = itoa(dataNode->threadID , threadIDString, 10);
			tabName = String::Concat(tabName, " - ");
			tabName = String::Concat(tabName, procIDString);
			tabName = String::Concat(tabName, "(");
			if (Int32::Parse (threadIDString) != 0)
				tabName = String::Concat(tabName, threadIDString);
			else
				tabName = String::Concat(tabName, "All Threads");
			tabName = String::Concat(tabName, ")");
			delete[] procIDString;
			delete[] threadIDString;
		}

		return tabName;
	}	
	
	//**************************************************************************
	// Method:		DisplayTabPage
	// Description: Select or display a given tab page
	//
	// Parameters:
	//	dataNode - node associated with the page to display
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::DisplayTabPage(DisplayableDataNode *dataNode, bool tabSelected)
	{
		DisplayableDataNode *tabNode = new DisplayableDataNode(dataNode);
		HoloScriptApplication *holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(dataNode->processID);
		if (!holoScriptApp)
			holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForTerminatedProcess(dataNode->processID);

		switch (tabNode->Type)
		{
		case UIData::ActiveLogPane:
		case UIData::SavedLogPane:
			{
				Crownwood::Magic::Controls::TabPage *tabPage = GetTabPage(tabNode);
				if (tabPage)
					tabPage->Selected = true;
				else
				{
					LogPaneDataArray *logDataArray = dynamic_cast <LogPaneDataArray*>(currentProject->GetDataArray(tabNode));
					
					if (logDataArray)
					{
						logDataArray->OpenSavedLog();

						LogPane *logPane = new LogPane(logDataArray);
						Crownwood::Magic::Controls::TabPage *newPage;

						if (holoScriptApp && holoScriptApp->IsPaused)
						{
							//make sure paused isn't the default icon
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), logPane, tabNode->Type);
							newPage->ImageIndex = PAUSED_PROCESS_INDEX;
						}
						else if (holoScriptApp && holoScriptApp->IsRunning)
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), logPane, tabNode->Type);
						else
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), logPane, TERMINATED_PROCESS_INDEX);				

						newPage->Tag = logDataArray;
						nonDockablePaneManager->addPage(newPage);
						newPage->Selected = tabSelected;
						logDataArray->Refresh();
					}
				}
				break;
			}
		case UIData::ActiveNetworkLogPane:
		case UIData::SavedNetworkLogPane:
			{
				Crownwood::Magic::Controls::TabPage *tabPage = GetTabPage(tabNode);
				if (tabPage)
					tabPage->Selected = true;
				else
				{
					NetworkLogPaneDataArray *logDataArray = dynamic_cast <NetworkLogPaneDataArray*>(currentProject->GetDataArray(tabNode));
					if (logDataArray)
					{
						LogPaneDataArray *tmp_logDataArray = dynamic_cast <LogPaneDataArray*>(currentProject->GetDataArray(new DisplayableDataNode (S"", UIData::NodeType::SavedLogPane, tabNode->processID, tabNode->threadID)));
						if (tmp_logDataArray)
							tmp_logDataArray->OpenSavedLog();

						NetworkLogPane *logPane = new NetworkLogPane(logDataArray);
						Crownwood::Magic::Controls::TabPage *newPage;
					
						if (holoScriptApp && holoScriptApp->IsPaused)
						{
							//make sure paused isn't the default icon
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), logPane, tabNode->Type);
							newPage->ImageIndex = PAUSED_PROCESS_INDEX;
						}
						else if (holoScriptApp && holoScriptApp->IsRunning)
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), logPane, tabNode->Type);
						else 
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), logPane, TERMINATED_PROCESS_INDEX);							

						newPage->Tag = logDataArray;
						nonDockablePaneManager->addPage(newPage);
						newPage->Selected = tabSelected;
						logDataArray->Refresh();
					}
				}
				break;
			}
		case UIData::Faults:
		case UIData::MemoryFaultType:
		case UIData::DiskFaultType:
		case UIData::NetworkFaultType:
		case UIData::RegistryFaultType:
		case UIData::ProcessFaultType:
		case UIData::CustomFaultType:
		case UIData::MemoryFault:
		case UIData::DiskFault:
		case UIData::NetworkFault:
		case UIData::RegistryFault:
		case UIData::ProcessFault:
		case UIData::CustomFault:
			{
				//fault pane is always named 'Faults'
				tabNode->Name = "Faults";
				Crownwood::Magic::Controls::TabPage *tabPage = GetTabPage(tabNode);
				if (tabPage)
					tabPage->Selected = true;
				else
				{
					FaultPaneDataArray *faultDataArray = dynamic_cast <FaultPaneDataArray*>(currentProject->GetDataArray(tabNode));
					if (faultDataArray)
					{
						FaultPane *faultPane = new FaultPane(faultDataArray, tabNode->threadID);
						Crownwood::Magic::Controls::TabPage *newPage;
						if (holoScriptApp && holoScriptApp->IsPaused)
						{
							//make sure paused isn't the default icon
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), faultPane, UIData::Faults);
							newPage->ImageIndex = PAUSED_PROCESS_INDEX;
						}
						else if (holoScriptApp && holoScriptApp->IsRunning)
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), faultPane, UIData::Faults);
						else
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), faultPane, TERMINATED_PROCESS_INDEX);

						newPage->Tag = faultDataArray;
						nonDockablePaneManager->TabPages->Add(newPage);
						newPage->Selected = tabSelected;
						faultDataArray->Refresh();
					}
				}
				break;
			}
		case UIData::NetworkCorruptionFault:
		case UIData::NetworkCorruptionFaultType:
			{
				tabNode->Name = "Network Fuzzing Faults";
				Crownwood::Magic::Controls::TabPage *tabPage = GetTabPage(tabNode);
				if (tabPage)
					tabPage->Selected = true;
				else
				{
					NetworkCorruptionDataArray *ncDataArray = dynamic_cast <NetworkCorruptionDataArray*>(currentProject->GetDataArray(tabNode));
					if (ncDataArray)
					{
						NetworkCorruptionPane *ncPane = new NetworkCorruptionPane(ncDataArray);
						Crownwood::Magic::Controls::TabPage *newPage;
						if (holoScriptApp && holoScriptApp->IsPaused)
						{
							//make sure paused isn't the default icon
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), ncPane, UIData::NetworkCorruptionFaultType);
							newPage->ImageIndex = PAUSED_PROCESS_INDEX;
						}
						else if (holoScriptApp && holoScriptApp->IsRunning)
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), ncPane, UIData::NetworkCorruptionFaultType);
						else 
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), ncPane, TERMINATED_PROCESS_INDEX);

						newPage->Tag = ncDataArray;
						nonDockablePaneManager->TabPages->Add(newPage);
						newPage->Selected = true;
						ncDataArray->Refresh();
					}
				}
				break;
			}
		case UIData::FileCorruptionFault:
		case UIData::FileCorruptionFaultType:
			{
				tabNode->Name = "File Fuzzing Faults";
				Crownwood::Magic::Controls::TabPage *tabPage = GetTabPage(tabNode);
				if (tabPage)
					tabPage->Selected = true;
				else
				{
					FileCorruptionDataArray *fcDataArray = dynamic_cast <FileCorruptionDataArray*>(currentProject->GetDataArray(tabNode));
					if (fcDataArray)
					{
						FileCorruptionPane *fcPane = new FileCorruptionPane(fcDataArray);
						Crownwood::Magic::Controls::TabPage *newPage;
						if (holoScriptApp && holoScriptApp->IsPaused)
						{
							//make sure paused isn't the default icon
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), fcPane, UIData::FileCorruptionFaultType);
							newPage->ImageIndex = PAUSED_PROCESS_INDEX;
						}
						else if (holoScriptApp && holoScriptApp->IsRunning)
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), fcPane, UIData::FileCorruptionFaultType);
						else
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), fcPane, TERMINATED_PROCESS_INDEX);

						newPage->Tag = fcPane;
						nonDockablePaneManager->TabPages->Add(newPage);
						newPage->Selected = true;
						fcDataArray->Refresh();
					}
				}
				break;
			}
		case UIData::SavedCorruptedFile:
		case UIData::SavedCorruptedFiles:
			{
				tabNode->Name = "Saved Fuzzed Files";
				Crownwood::Magic::Controls::TabPage *tabPage = GetTabPage(tabNode);
				if (tabPage)
					tabPage->Selected = true;
				else
				{
					SavedFileCorruptionDataArray *sfcDataArray = dynamic_cast <SavedFileCorruptionDataArray*>(currentProject->GetDataArray(tabNode));
					if (sfcDataArray)
					{
						SavedFileCorruptionPane *sfcPane = new SavedFileCorruptionPane(sfcDataArray);
						Crownwood::Magic::Controls::TabPage *newPage;
						if (holoScriptApp && holoScriptApp->IsPaused)
						{
							//make sure paused isn't the default icon
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), sfcPane, UIData::SavedCorruptedFiles);
							newPage->ImageIndex = PAUSED_PROCESS_INDEX;
						}
						else if (holoScriptApp && holoScriptApp->IsRunning)
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), sfcPane, UIData::SavedCorruptedFiles);
						else 
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), sfcPane, TERMINATED_PROCESS_INDEX);

						newPage->Tag = sfcDataArray;
						nonDockablePaneManager->TabPages->Add(newPage);
						newPage->Selected = true;
						sfcDataArray->Refresh();
					}
				}
				break;
			}
		case UIData::CorruptedFile:
		case UIData::CorruptedFiles:
			{
				tabNode->Name = "Fuzzed Files";
				Crownwood::Magic::Controls::TabPage *tabPage = GetTabPage(tabNode);
				if (tabPage)
					tabPage->Selected = true;
				else
				{
					SavedFileCorruptionDataArray *sfcDataArray = dynamic_cast <SavedFileCorruptionDataArray*>(currentProject->GetDataArray(tabNode));
					if (sfcDataArray)
					{
						SavedFileCorruptionPane *sfcPane = new SavedFileCorruptionPane(sfcDataArray);
						Crownwood::Magic::Controls::TabPage *newPage;
						if (holoScriptApp && holoScriptApp->IsPaused)
						{
							//make sure paused isn't the default icon
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), sfcPane, UIData::CorruptedFiles);
							newPage->ImageIndex = PAUSED_PROCESS_INDEX;
						}
						else if (holoScriptApp && holoScriptApp->IsRunning)
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), sfcPane, UIData::CorruptedFiles);
						else 
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), sfcPane, TERMINATED_PROCESS_INDEX);

						newPage->Tag = sfcDataArray;
						nonDockablePaneManager->TabPages->Add(newPage);
						newPage->Selected = true;
						sfcDataArray->Refresh();
					}
				}
				break;
			}
		case UIData::Tests:
		case UIData::Test:
			{
				Crownwood::Magic::Controls::TabPage *tabPage = GetTabPage(tabNode);
				if (tabPage)
					tabPage->Selected = true;
				else
				{
					ScheduledTestDataArray *testDataArray = dynamic_cast <ScheduledTestDataArray*>(currentProject->GetDataArray(tabNode));
					if (testDataArray)
					{
						ScheduledTestPane *testPane = new ScheduledTestPane(testDataArray, tabNode->threadID);
						Crownwood::Magic::Controls::TabPage *newPage;
						if (holoScriptApp && holoScriptApp->IsPaused)
						{
							//make sure paused isn't the default icon
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), testPane, UIData::Tests);
							newPage->ImageIndex = PAUSED_PROCESS_INDEX;
						}
						else if (holoScriptApp && holoScriptApp->IsRunning)
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), testPane, UIData::Tests);
						else
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), testPane, TERMINATED_PROCESS_INDEX);

						newPage->Tag = testDataArray;
						nonDockablePaneManager->TabPages->Add(newPage);
						newPage->Selected = tabSelected;
						testDataArray->Refresh();
					}
				}
				break;
			}
		case UIData::ResourceFaults:
		case UIData::ResourceFault:
			{
				tabNode->Name = "Resource Faults";
				Crownwood::Magic::Controls::TabPage *tabPage = GetTabPage(tabNode);
				if (tabPage)
					tabPage->Selected = true;
				else
				{
					ResourceTestDataArray *rtDataArray = dynamic_cast <ResourceTestDataArray*>(currentProject->GetDataArray(tabNode));
					if (rtDataArray)
					{
						ResourceTestPane *rtPane = new ResourceTestPane(rtDataArray);
						Crownwood::Magic::Controls::TabPage *newPage;
						if (holoScriptApp && holoScriptApp->IsPaused)
						{
							//make sure paused isn't the default icon
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), rtPane, UIData::ResourceFaults);
							newPage->ImageIndex = PAUSED_PROCESS_INDEX;
						}
						else if (holoScriptApp && holoScriptApp->IsRunning)
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), rtPane, UIData::ResourceFaults);
						else 
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), rtPane, TERMINATED_PROCESS_INDEX);

						newPage->Tag = rtDataArray;
						nonDockablePaneManager->TabPages->Add(newPage);
						newPage->Selected = true;
						rtDataArray->Refresh();
					}
				}
				break;
			}
		case UIData::ExceptionPane:
		case UIData::ExceptionInfo:
			{
				if (tabNode->processID == 0)
					tabNode->Name = "Saved Exceptions";
				else
					tabNode->Name = "Exceptions";
				Crownwood::Magic::Controls::TabPage *tabPage = GetTabPage(tabNode);
				if (tabPage)
					tabPage->Selected = true;
				else
				{
					ExceptionDataArray *exceptionDataArray = dynamic_cast <ExceptionDataArray*>(currentProject->GetDataArray(tabNode));
					if (exceptionDataArray)
					{
						ExceptionPane *exceptionPane = new ExceptionPane(exceptionDataArray, tabNode->threadID);
						Crownwood::Magic::Controls::TabPage *newPage;
						if (holoScriptApp && holoScriptApp->IsPaused)
						{
							//make sure paused isn't the default icon
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), exceptionPane, tabNode->Type);
							newPage->ImageIndex = PAUSED_PROCESS_INDEX;
						}
						else if (holoScriptApp && holoScriptApp->IsRunning)
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), exceptionPane, tabNode->Type);
						else 
							newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), exceptionPane, TERMINATED_PROCESS_INDEX);

						newPage->Tag = exceptionDataArray;
						nonDockablePaneManager->TabPages->Add(newPage);
						newPage->Selected = tabSelected;
						exceptionDataArray->Refresh();
					}
				}
				break;
			}
		case UIData::Limits:
		case UIData::MemoryLimit:
		case UIData::DiskLimit:
		case UIData::NetworkUpLimit:
		case UIData::NetworkDownLimit:
			limitPane->BringToFront();
			break;
		case UIData::Files:
		case UIData::File:
		case UIData::Registry:
		case UIData::RegistryEntry:
			resourcePane->BringToFront();
			break;
		case UIData::HelpDetails:
			nonDockablePaneManager->displayHelpPane(tabNode->Name);
			break;
		case UIData::Report:
			{
				Crownwood::Magic::Controls::TabPage *tabPage = GetTabPage(tabNode);
				if (tabPage)
					tabPage->Selected = true;
				else
				{
					ReportPaneDataArray *reportDataArray = dynamic_cast <ReportPaneDataArray*>(currentProject->GetDataArray(tabNode));
					if (reportDataArray)
					{
						ReportPane *reportPane = new ReportPane(reportDataArray);
						Crownwood::Magic::Controls::TabPage *newPage;

						if (!reportPane->showContents(reportDataArray->ReportPath))
							return;

						newPage = new Crownwood::Magic::Controls::TabPage(ConstructTabTitle(tabNode), reportPane, tabNode->Type);
						newPage->Tag = reportDataArray;
						nonDockablePaneManager->TabPages->Add(newPage);
						newPage->Selected = true;

					}
				}
				break;
			}
		}
	}

	//**************************************************************************
	// Method:		ModifyDataArray
	// Description: Updates the array and tab page title if necessary when a node changes in the ui
	//
	// Parameters:
	//	dataNode - original data
	//	modifiedDataNode - modified data
	//	propertyName - name of property that was modified
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::ModifyDataArray(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName)
	{
		if (propertyName->CompareTo("Name") == 0)
		{
			switch (dataNode->Type)
			{
			case Report:
				{
					Crownwood::Magic::Controls::TabPage *tabPage = GetTabPage(dataNode);
					if (tabPage)
						tabPage->Title = ConstructTabTitle(modifiedDataNode);

					ReportPaneDataArray *reportDataArray = dynamic_cast <ReportPaneDataArray*>(currentProject->GetDataArray(modifiedDataNode));
					if (reportDataArray)
					{
						//re-add report to dataarray with new name
						currentProject->RemoveDataArray(reportDataArray);
						currentProject->AddDataArray(modifiedDataNode->Name, reportDataArray);
					}
					break;
				}
			case RecordedSession:
				{
					RecordedSessionDataArray *recordedDataArray = dynamic_cast <RecordedSessionDataArray*>(currentProject->GetDataArray(dataNode));
					if (recordedDataArray)
						recordedDataArray->Name = modifiedDataNode->Name;
					break;
				}
			}
		}
	}

	//**************************************************************************
	// Method:		onRestartProject
	// Description: uses control::begininvoke to allow other threads to call restart project
	//
	// Parameters:
	//	procID - processID of the app to restart
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::onRestartProject(bool pauseOnStart)
	{
		Object *args[] = new Object*[1];

		args[0] = __box(pauseOnStart);
		Control::BeginInvoke(xRestartProjectHandler, args);
	}

	//**************************************************************************
	// Method:		RestartProject
	// Description: restarts the app associated with a process id.
	//
	// Parameters:
	//	pauseOnStart - true if we are to pause on start
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::RestartProject(bool pauseOnStart)
	{
		DisplayableDataNode *dataNode;
		DisplayableDataArray *dataArray;
		RecordedSessionDataArray *recordedSessionDataArray;
		HoloScriptApplication *holoScriptApp;
		String *applicationPath, *commandLineParams;
		ArrayList *filteredFunctionList;
		ArrayList *tempHoloScriptArray = new ArrayList();
		ArrayList *resumeHoloScriptArray = new ArrayList();
		Type *type;
		unsigned int procID;

		uiEventRegistry->OnCloseProjectUI->Invoke();
		projectPaneData->Clear();

		CloseAllNonDockablePanes();

		ArrayList * keyList = new ArrayList();
		ArrayList * exceptionsArray = new ArrayList();
		SortedList *SavedLogs = new SortedList();

		//save unique keys and data of exceptions already saved
		DisplayableDataArray * ddArray = currentProject->GetDataArrayByKey (S"Saved Exceptions");
		if (ddArray)
		{
			ExceptionDataArray * eDataArray  = dynamic_cast <ExceptionDataArray *> (ddArray);
			if (eDataArray)
			{
				for (int i=0; i < eDataArray->Count; i++)
				{
					ExceptionDataNode * eDataNode = dynamic_cast <ExceptionDataNode *> (eDataArray->GetDataNodeByIndex(i));
					if (eDataNode)
					{
						exceptionsArray->Add (eDataNode);
						String * key = String::Concat (eDataNode->ExeName, eDataNode->Exception);
						keyList->Add (key);
					}
				}
			}
		}

		for (int i = 0; i < currentProject->DisplayableDataArrayCount; i++)
		{
			dataArray = currentProject->GetDisplayableDataArray(i);
			type = dataArray->GetType();
			if (type->Name->CompareTo("RecordedSessionDataArray") != 0)
			{
				//backup exceptions before erasing them
				if (type->Name->CompareTo("ExceptionDataArray") == 0)
				{
					for (int i=0; i < dataArray->Count; i++)
					{
						ExceptionDataNode * eDataNode = dynamic_cast <ExceptionDataNode *> (dataArray->GetDataNodeByIndex(i));
						if (eDataNode)
						{
							ExceptionDataNode * save_eDataNode = new ExceptionDataNode(eDataNode->MiniDumpFile, 0, 0);
							save_eDataNode->TimeStamp = eDataNode->TimeStamp;
							save_eDataNode->ExeName = eDataNode->ExeName;
							save_eDataNode->Exception = eDataNode->Exception;
							save_eDataNode->MiniDumpFile = eDataNode->MiniDumpFile;
							save_eDataNode->ID = eDataNode->MiniDumpFile;
							
							//use a key to make sure we arn't adding dupes
							String * key = String::Concat (save_eDataNode->ExeName, save_eDataNode->Exception);
							if (! keyList->Contains (key))
							{
								keyList->Add (key);
								exceptionsArray->Add (save_eDataNode);
							}
						}

					}
				}
				//backup logs and save info before erasing
				else if (type->Name->CompareTo("LogPaneDataArray") == 0)
				{
					LogPaneDataArray * lpDataArray = dynamic_cast <LogPaneDataArray *> (dataArray);
					if (lpDataArray)
					{
						if (!lpDataArray->SavedLog)
						{
							String *logsFolder = String::Concat(currentProject->Folder, "\\", currentProject->Title, "_Logs");
							if (!Directory::Exists(logsFolder))
								Directory::CreateDirectory(logsFolder);

							HoloScriptApplication * hsApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess (lpDataArray->ProcessID);
							if (!hsApp)
								hsApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForTerminatedProcess(lpDataArray->ProcessID);
							if (hsApp)
							{
								String *logFilePath = hsApp->GetLogFileName();
								int index = logFilePath->LastIndexOf("\\");
								index++;
								String *logFileName = logFilePath->Substring(index, logFilePath->Length-index);
								String *savedLogFilePath = String::Concat(logsFolder, "\\", logFileName);

								DateTime currentTime = DateTime::Now;
								String * saveTime = String::Concat(currentTime.ToShortDateString(), " ", currentTime.ToLongTimeString());
								saveTime = String::Concat(hsApp->ApplicationName, "&", saveTime);

								String *logKey = ProjectFileReader::CreateSavedLogKey(savedLogFilePath, saveTime);
								if (!SavedLogs->ContainsKey (logKey))
								{
									try
									{
										File::Copy(logFilePath, savedLogFilePath, true);
									}
									catch(...)
									{
									}

									SavedLogs->Add (logKey, savedLogFilePath);
								}
							}
						}
						else
						{
							String *logKey = ProjectFileReader::CreateSavedLogKey(lpDataArray->SavedLogFilePath, lpDataArray->ExeAndLogSavedTime);
							if (!SavedLogs->ContainsKey (logKey))
								SavedLogs->Add (logKey, lpDataArray->SavedLogFilePath);
						}
					}
				}

				//remove the dataarray and perform cleanup
				currentProject->RemoveDataArray(dataArray);
				dataArray->Clear();
				if (type->Name->CompareTo("ProjectPaneDataArray") != 0)
					dataArray->CleanUp();

				i--;
			}
		}

		for (int i=0; i < holoScriptRegistry->Count; i++)
		{
            holoScriptApp = holoScriptRegistry->getHoloScriptAppByIndex(i);
			tempHoloScriptArray->Add(holoScriptApp);
		}
	
		for (int i=0; i < holoScriptRegistry->TerminatedCount; i++)
		{
            holoScriptApp = holoScriptRegistry->getTerminatedHoloScriptAppByIndex(i);
			tempHoloScriptArray->Add(holoScriptApp);
		}

		ArrayList* exceptionsArrayArray = new ArrayList();

		for (int i=0; i<tempHoloScriptArray->Count; i++)
		{
			holoScriptApp = dynamic_cast <HoloScriptApplication*> (tempHoloScriptArray->get_Item(i));
			if (holoScriptApp && !holoScriptApp->IsChainedProcess && holoScriptApp->ShouldRestartWithProject)
			{
				applicationPath = holoScriptApp->ApplicationPath;
				commandLineParams = holoScriptApp->CommandLineParams;
				filteredFunctionList = holoScriptApp->FilteredFunctionList;
				bool processChaining = holoScriptApp->ProcessChaining;
				bool inheritSettings = holoScriptApp->InheritSettings;
				bool savedPauseOnStart = holoScriptApp->PauseOnStart;
				bool attachDebugger = holoScriptApp->AttachDebugger;
				bool logFirstChance = holoScriptApp->LogFirstChance;
				holoScriptApp->GetApplicationProcessID(procID);
				holoScriptRegistry->StartTerminateApplication(procID);
				holoScriptApp = new HoloScriptApplication();
				holoScriptApp->ProjectPath = currentProject->FullPath;
				holoScriptApp->OnInitializeAppForProcessChaining = new InitializeAppForProcessChainingDelegate(this, onInitializeAppForProcessChaining);
				holoScriptApp->OnProcessChainingAttachComplete = new ProcessChainingAttachCompleteDelegate(this, onProcessChainingAttachComplete);
				holoScriptApp->OnThreadCreated += new ThreadCreatedDelegate(this, onThreadCreated);
				holoScriptApp->OnHeatProcessTerminated += new HeatProcessTerminatedDelegate(this, onHeatProcessTerminated);
				holoScriptApp->OnHeatProcessPaused += new HeatProcessPausedDelegate(this, onHeatProcessPaused);
				holoScriptApp->OnHeatProcessResumed += new HeatProcessResumedDelegate(this, onHeatProcessResumed);
				holoScriptApp->LaunchApplication(applicationPath, commandLineParams, true, filteredFunctionList);
				holoScriptApp->ProcessChaining = processChaining;
				holoScriptApp->InheritSettings = inheritSettings;
				holoScriptApp->PauseOnStart = savedPauseOnStart;
				holoScriptApp->AttachDebugger = attachDebugger;
				holoScriptApp->LogFirstChance = logFirstChance;
				CreateDataArrays(holoScriptApp);

				if (!pauseOnStart)
					resumeHoloScriptArray->Add(holoScriptApp);
				holoScriptRegistry->EndTerminateApplication(procID);
			}
		}

		//add old recorded session nodes
		for (int i = 0; i < currentProject->DisplayableDataArrayCount; i++)
		{
			dataArray = currentProject->GetDisplayableDataArray(i);
			type = dataArray->GetType();
			if (type->Name->CompareTo("RecordedSessionDataArray") == 0)
			{
				recordedSessionDataArray = dynamic_cast <RecordedSessionDataArray*> (dataArray);
				dataNode = new DisplayableDataNode(recordedSessionDataArray->Name, UIData::RecordedSession, 0, 0);
				dataNode->DataArray = recordedSessionDataArray;
				Object *args[] = __gc new Object*[1];
				args[0] = dataNode;
				dataNode->CreatedDelegate->DynamicInvoke(args);
			}
		}

		//create dataarray for saved exceptions and add saved exceptions
		if (!currentProject->ContainsDataArrayKey (S"Saved Exceptions"))
		{
			ExceptionDataArray* exceptionsdataArray = new ExceptionDataArray(0);
			currentProject->AddDataArray(S"Saved Exceptions", exceptionsdataArray);
		}
		for (int i=0; i < exceptionsArray->Count; i++)
		{
			ExceptionDataNode * eDataNode = dynamic_cast <ExceptionDataNode *> (exceptionsArray->Item[i]);
			if (eDataNode)
			{
				Object *tObj[] = __gc new Object*[1];
				eDataNode->UpdateDelegates();
				tObj[0] = eDataNode;
				eDataNode->CreateDelegate->DynamicInvoke(tObj);
			}
		}

		//add saved log nodes
		for (int savedLogFileCount=0; savedLogFileCount < SavedLogs->Count; savedLogFileCount++)
		{
			String *logFilePath = SavedLogs->GetByIndex(savedLogFileCount)->ToString();
			String *logKey = SavedLogs->GetKey(savedLogFileCount)->ToString()->ToLower();
 			if (File::Exists(logFilePath))
			{
				//parse PID from key
				int index = logKey->LastIndexOf("&");
				unsigned int oldPID = UInt32::Parse(logKey->Substring(index+1, logKey->Length-index-1));
				//remove pid from key string
				String *ExeAndLogFileTime = logKey->Substring(0, index);
				//parse exe name and file time from key
				index = ExeAndLogFileTime->LastIndexOf(".exe&");
				index = index + 4;
				String *ExeName = ExeAndLogFileTime->Substring(0, index);
				String *logFileTime = ExeAndLogFileTime->Substring(++index, ExeAndLogFileTime->Length-index);
				
				LogPaneDataArray * logDataArray = new LogPaneDataArray(ExeName, logFilePath, oldPID, ExeAndLogFileTime);
				//make old log node name from app name and  save time
				String *nodeName = String::Concat(ExeName, " (", logFileTime, ")"); 
				nodeName = String::Concat(" #", (savedLogFileCount+1).ToString(), ". ", nodeName);
				DisplayableDataNode *dataNode = new DisplayableDataNode(nodeName, UIData::SavedLogPane, oldPID, 0);
				dataNode->ID = String::Concat(oldPID.ToString(), dataNode->threadID.ToString());
				String *keyString = String::Concat(logDataArray->GetType()->Name, Convert::ToString(oldPID));
				keyString = String::Concat(keyString, Convert::ToString(savedLogFileCount));
				if (!currentProject->ContainsDataArrayKey(keyString))
					currentProject->AddDataArray(keyString, logDataArray);
				dataNode->DataArray = logDataArray;

				NetworkLogPaneDataArray *netLogDataArray = new NetworkLogPaneDataArray(oldPID, 0, logFilePath, logDataArray->HoloScriptApplicationInstance);
				nodeName = String::Concat(nodeName, " Network Log");
				DisplayableDataNode *netDataNode = new DisplayableDataNode(nodeName, UIData::SavedNetworkLogPane, oldPID, 0);
				netDataNode->ID = String::Concat("nl", oldPID.ToString(), netDataNode->threadID.ToString());
				keyString = String::Concat(netLogDataArray->GetType()->Name, Convert::ToString(oldPID));
				keyString = String::Concat(keyString, Convert::ToString(savedLogFileCount));
				if (!currentProject->ContainsDataArrayKey(keyString))
					currentProject->AddDataArray(keyString, netLogDataArray);
				netDataNode->DataArray = netLogDataArray;

				dataEventRegistry->OnLogPaneCreated->Invoke(dataNode);
				dataEventRegistry->OnNetworkLogPaneCreated->Invoke(netDataNode);
			}
		}

		//remove old terminated processes
		holoScriptRegistry->RemoveTerminatedProcesses();

		if (dataEventRegistry->OnProjectRestarted)
			dataEventRegistry->OnProjectRestarted->Invoke();
		
		//resume any processes that need to be resumed
		for (int i=0; i < resumeHoloScriptArray->Count; i++)
		{
			HoloScriptApplication *hsApp = dynamic_cast <HoloScriptApplication*> (resumeHoloScriptArray->Item[i]);
			if (hsApp)
				hsApp->ResumeApplication();
		}
	}

	//**************************************************************************
	// Method:		onSaveProject
	// Description: called when another piece of ui wants to save the project
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::onSaveProject()
	{
		fileSaveProjectMenuItemClicked(NULL, NULL);
	}

	//**************************************************************************
	// Method:		StressTestGenerationStart
	// Description: starts stress test generation
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::StressTestGenerationStart(GenerationType generationType, Intensity intensity)
	{
		if (!testGenerationData)
			testGenerationData = new TestGenerationDataArray(holoScriptRegistry, currentProject->DataArrayList);
		testGenerationData->StressTestGenerationStart(generationType, intensity);
		sessionStressTestGeneratorMenuItem->Text = "&Stop Stress Test Generation";
		sessionCodeCoverageTestGeneratorMenuItem->Enabled = false;
	}

	//**************************************************************************
	// Method:		CodeCoverageTestGenerationStart
	// Description: starts code coverage test generation
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::CodeCoverageTestGenerationStart(Intensity intensity, bool restartProject, int timeout)
	{
		if (!testGenerationData)
			testGenerationData = new TestGenerationDataArray(holoScriptRegistry, currentProject->DataArrayList);
		testGenerationData->CodeCoverageTestGenerationStart(intensity, restartProject, timeout);
		codeCoverageRunning = true;
		sessionCodeCoverageTestGeneratorMenuItem->Text = "&Stop Code Coverage Test Generation";
		sessionStressTestGeneratorMenuItem->Enabled = false;
	}

	//**************************************************************************
	// Method:		StressTestGenerationStop
	// Description: stops code coverage test generation
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::StressTestGenerationStop()
	{
		if (testGenerationData)
			testGenerationData->StressTestGenerationStop();
	}

	//**************************************************************************
	// Method:		CodeCoverageTestGenerationStop
	// Description: stops code coverage test generation
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::CodeCoverageTestGenerationStop()
	{
		if (testGenerationData)
			testGenerationData->CodeCoverageTestGenerationStop();	
	}

	//**************************************************************************
	// Method:		IsServiceInstalled
	// Description: returns whether or not a service is installed on the system
	//
	// Parameters:
	//	serviceName - the name of the service to check
	//
	// Return Value: true if it is installed, false otherwise
	//*************************************************************************
	bool MainFrame::IsServiceInstalled(String *serviceName)
	{
		String *lowerServiceName = serviceName->ToLower();
		
		// see if the service is on the machine
		ServiceController *installedServices[] = ServiceController::GetServices();
		for (int i = 0; i < installedServices->Count; i++)
		{
			if (installedServices[i]->ServiceName->ToLower()->Equals(lowerServiceName) || 
				installedServices[i]->DisplayName->ToLower()->Equals(lowerServiceName))
			{
				return true;
			}
		}

		return false;
	}

	//**************************************************************************
	// Method:		ProcessCommandLine
	// Description: processes the command line arguments
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::ProcessCommandLine()
	{
		int index;
		HoloScriptApplication *hsApp;
		bool isProjectOptionSpecified = false;
		bool wasSomeAppLaunchedSuccessfully = false;

		if (commandLineArgs)
		{
			// check for silent mode before anything else
			if (!RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
			{
				for (int i=0; i<commandLineArgs->Count; i++)
				{
					String *arg = dynamic_cast <String*> (commandLineArgs->get_Item(i));

					if (arg->CompareTo("/silent") == 0)
					{
						silentMode = true;
						UserNotification::SilentMode = true;
						WindowState = FormWindowState::Minimized;
					}
				}
			}

			// check for tool launch
			for (int i=0; i<commandLineArgs->Count; i++)
			{
				String *arg = dynamic_cast <String*> (commandLineArgs->get_Item(i));

				if (arg->CompareTo("/addnewintercepts") == 0)
				{
					toolsAddHolodeckInterceptsMenuItemClicked(NULL, NULL);
					return;
				}
				else if (arg->CompareTo("/customtestproject") == 0)
				{
					toolsCustomWrapperLibraryMenuItemClicked(NULL, NULL);
					return;
				}
			}

			String *projPath = dynamic_cast <String *> (commandLineArgs->get_Item(0));
			
			if (projPath->EndsWith(".hdp") && File::Exists(projPath))
			{
				currentProject->Folder = System::IO::Path::GetDirectoryName(projPath);
				currentProject->FullPath = projPath;
				currentProject->Name = System::IO::Path::GetFileName(projPath);

				ProjectFileReader *reader = new ProjectFileReader();
				reader->OnLaunchSavedApp += new LaunchSavedAppDelegate(this, LaunchSavedApp);
				if (reader->OpenProject(projPath, currentProject))
				{
					currentProject->FullPath = projPath;
					index = currentProject->FullPath->LastIndexOf("\\");
					if (index != -1)
					{
						currentProject->Folder = currentProject->FullPath->Substring(0, index);
						currentProject->Name = currentProject->FullPath->Substring(index+1);
						currentProject->Title = currentProject->FullPath->Substring(index+1, currentProject->FullPath->Length-index-5);
					}
					sessionPerThreadMenuItem->Checked = currentProject->IsInPerThreadMode;
					IsProjectActive = true;
					this->UpdateHolodeckWindowTitle (currentProject->Title);
					isProjectOptionSpecified = true;
					if (holoScriptRegistry->Count > 0)
						hsApp = holoScriptRegistry->getHoloScriptAppByIndex(0);
					
					if (hsApp)
					{
						UInt32 pID;
						hsApp->GetApplicationProcessID(pID);
						uiEventRegistry->OnFocusChanged(new DisplayableDataNode("Faults", UIData::Faults, pID, 0));
						wasSomeAppLaunchedSuccessfully = hsApp->IsRunning;
					}
				}
				else
				{
					UserNotification::ErrorNotify(String::Concat ("Could not load the project ", projPath));
					wasSomeAppLaunchedSuccessfully = false;
					return;
				}
			}
			else if (projPath->StartsWith ("/launch:"))
			{
				if (projPath->IndexOf (".exe") > 0)
				{
					String * exePath = projPath->Replace ("/launch:", "")->Trim();
					String * cmdLine = exePath->Replace (exePath->Substring (0, exePath->IndexOf (".exe") + 4), "");
					
					NewProjectWizard *defaultProjectWizard = new NewProjectWizard(true);
					defaultProjectWizard->LaunchApp = true;
					defaultProjectWizard->ApplicationPath = exePath;
					defaultProjectWizard->ApplicationName = exePath->Substring(exePath->LastIndexOf ("\\") + 1);
					defaultProjectWizard->PauseOnStart = false;
					defaultProjectWizard->ProcessChaining = true;
					defaultProjectWizard->InheritSettings = true;
					defaultProjectWizard->AttachDebugger = true;
					defaultProjectWizard->LogFirstChance = false;
					defaultProjectWizard->CommandLineParams = cmdLine;
					defaultProjectWizard->ReassessFunctions();
					wasSomeAppLaunchedSuccessfully = this->runNewProjectWizard (defaultProjectWizard);
					isProjectOptionSpecified = true;
				}
			}
			else if (projPath->StartsWith ("/launchservice:"))
			{
				// shouldn't see anything with an extension here
				if (projPath->IndexOf (".") > 0)
				{
					UserNotification::InfoNotify("/launchservice option needs a service name, not an executable path");
				}
				else
				{
					String *serviceName = projPath->Replace ("/launchservice:", "")->Trim();

					if (!IsServiceInstalled(serviceName))
					{
						UserNotification::InfoNotify(String::Concat("\"", serviceName, "\" is not a registered service on this machine.  Please use a valid service name."));
					}
					else
					{
						ServiceController *service = new ServiceController(serviceName);
						if (service)
						{
							String *path = String::Format("Win32_Service.Name='{0}'", service->ServiceName);
							ManagementObject *wmiService = new ManagementObject(path);
							if (wmiService)
							{
								wmiService->Get();

								Object *startMode = wmiService->get_Item(S"Startmode");

								// don't allow disabled services
								if (startMode && (startMode->ToString()->ToLower()->CompareTo("disabled") == 0))
								{
									UserNotification::InfoNotify(String::Concat("\"", serviceName, "\" is disabled and can not be launched.  Please choose a different service."));
								}
								else
								{
									NewProjectWizard *defaultProjectWizard = new NewProjectWizard(true);
									defaultProjectWizard->StartAService = true;
									defaultProjectWizard->ServiceToStart = serviceName;
									defaultProjectWizard->PauseOnStart = false;
									defaultProjectWizard->ProcessChaining = true;
									defaultProjectWizard->InheritSettings = true;
									defaultProjectWizard->AttachDebugger = true;
									defaultProjectWizard->LogFirstChance = false;
									defaultProjectWizard->ReassessFunctions();

									wasSomeAppLaunchedSuccessfully = this->runNewProjectWizard (defaultProjectWizard);
									isProjectOptionSpecified = true;
								}
							}
						}
					}
				}
			}
			else if (projPath->StartsWith ("/attach:"))
			{
				String * pidVal = projPath->Replace ("/attach:", "")->Trim();
				NewProjectWizard *defaultProjectWizard = new NewProjectWizard(true);
				defaultProjectWizard->ProcessID = UInt32::Parse (pidVal);
				defaultProjectWizard->ApplicationPath = "";
				defaultProjectWizard->ApplicationName = "";
				defaultProjectWizard->AttachToApp = true;
				defaultProjectWizard->PauseOnStart = false;
				defaultProjectWizard->ProcessChaining = true;
				defaultProjectWizard->InheritSettings = true;
				defaultProjectWizard->AttachDebugger = true;
				defaultProjectWizard->LogFirstChance = false;
				defaultProjectWizard->CommandLineParams = "";
				defaultProjectWizard->ReassessFunctions();
				wasSomeAppLaunchedSuccessfully = this->runNewProjectWizard (defaultProjectWizard);
				isProjectOptionSpecified = true;
			}
			else if (projPath->StartsWith ("/attachservice:"))
			{
				// shouldn't see anything with an extension here
				if (projPath->IndexOf (".") > 0)
				{
					UserNotification::InfoNotify("/attachservice option needs a service name, not an executable path");
				}
				else
				{
					String *serviceName = projPath->Replace ("/attachservice:", "")->Trim();

					if (!IsServiceInstalled(serviceName))
					{
						UserNotification::InfoNotify(String::Concat("\"", serviceName, "\" is not a registered service on this machine.  Please use a valid service name."));
					}
					else
					{
						ServiceController *service = new ServiceController(serviceName);
						if (service->Status != ServiceControllerStatus::Running)
						{
							UserNotification::InfoNotify(String::Concat("\"", serviceName, "\" is installed but is not running so Holodeck can not attach to it."));
						}
						else
						{
							NewProjectWizard *defaultProjectWizard = new NewProjectWizard(true);
							defaultProjectWizard->AttachToAService = true;
							defaultProjectWizard->ServiceToAttachTo = serviceName;
							defaultProjectWizard->PauseOnStart = false;
							defaultProjectWizard->ProcessChaining = true;
							defaultProjectWizard->InheritSettings = true;
							defaultProjectWizard->AttachDebugger = true;
							defaultProjectWizard->LogFirstChance = false;
							defaultProjectWizard->CommandLineParams = "";
							defaultProjectWizard->ReassessFunctions();
							wasSomeAppLaunchedSuccessfully = this->runNewProjectWizard (defaultProjectWizard);
							isProjectOptionSpecified = true;
						}
					}
				}
			}
		}
		else
			return;

		if (!RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
		{
			String *arg;

			for (int i=0; i<commandLineArgs->Count; i++)
			{
				arg = dynamic_cast <String*> (commandLineArgs->get_Item(i));

				if (wasSomeAppLaunchedSuccessfully && arg->StartsWith("/randomstress"))
				{
					if (!isProjectOptionSpecified)
					{
						UserNotification::ErrorNotify("You need to specify a project in order to run Random Stress tests");
						return;
					}
					arg = arg->Substring(arg->IndexOf(':')+1);
					if (arg->CompareTo("low") == 0)
					{
						StressTestGenerationStart(RandomStressTest, Low);
					}
					else if (arg->CompareTo("medium") == 0)
					{
						StressTestGenerationStart(RandomStressTest, Medium);
					}
					else if (arg->CompareTo("high") == 0)
					{
						StressTestGenerationStart(RandomStressTest, High);
					}
				}
				else if (wasSomeAppLaunchedSuccessfully && arg->StartsWith("/intelligentstress"))
				{
					if (!isProjectOptionSpecified)
					{
						UserNotification::ErrorNotify("You need to specify a project in order to run Intelligent Stress tests");
						return;
					}

					arg = arg->Substring(arg->IndexOf(':')+1);
					if (arg->CompareTo("low") == 0)
					{
						StressTestGenerationStart(IntelligentStressTest, Low);
					}
					else if (arg->CompareTo("medium") == 0)
					{
						StressTestGenerationStart(IntelligentStressTest, Medium);
					}
					else if (arg->CompareTo("high") == 0)
					{
						StressTestGenerationStart(IntelligentStressTest, High);
					}
				}
				else if (wasSomeAppLaunchedSuccessfully && arg->StartsWith("/codecoverage"))
				{
					if (!isProjectOptionSpecified)
					{
						UserNotification::ErrorNotify("You need to specify a project in order to run Code Coverage tests");
						return;
					}

					arg = arg->Substring(arg->IndexOf(':')+1);

					int timeout = 30;
					if (arg->IndexOf(',') != -1)
					{
						// Parameter after comma is timeout in seconds
						try
						{
							timeout = Int32::Parse(arg->Substring(arg->IndexOf(',') + 1));
						}
						catch (Exception*)
						{
							// Bad timeout, use default
						}

						if (timeout <= 0)
							timeout = 30; // Bad timeout, use default

						arg = arg->Substring(0, arg->IndexOf(','));
					}

					if (arg->CompareTo("low") == 0)
					{
						CodeCoverageTestGenerationStart(Low, false, timeout * 1000);
					}
					else if (arg->CompareTo("medium") == 0)
					{
						CodeCoverageTestGenerationStart(Medium, false, timeout * 1000);
					}
					else if (arg->CompareTo("high") == 0)
					{
						CodeCoverageTestGenerationStart(High, false, timeout * 1000);
					}
				}
				else if (wasSomeAppLaunchedSuccessfully && arg->CompareTo("/record") == 0)
				{		
					if (!isProjectOptionSpecified)
					{
						UserNotification::ErrorNotify("You need to specify a project in order to start recording a session");
						return;
					}

					DisplayableDataNode *dataNode = new DisplayableDataNode("", UIData::RecordedSession, 0, 0);
					RecordedSessionStart(dataNode);
				}
			}
		}
	}

	//**************************************************************************
	// Method:		GetActiveNode
	// Description: Gets pID and tID for active ui and returns in a datanode
	//
	// Parameters:
	//	displayWarning - whether a msg box should be displayed if an active node is not found
	//
	// Return Value: datanode with active pid and tid
	//*************************************************************************
	DisplayableDataNode *MainFrame::GetActiveNode(bool displayWarning)
	{
		Control *control = this->ActiveControl;
		ProjectPane *projectPane;
		LimitPane *limitPane;
		ResourcePane *resourcePane;
		LogPane *logPane;
		ExceptionPane * exceptionPane;
		ScheduledTestPane *testPane;
		FileCorruptionPane * fileCorruptionPane;
		SavedFileCorruptionPane * savedFileCorruptionPane;
		NetworkCorruptionPane *networkPane;
		NetworkLogPane * networkLogPane;
		FaultPane *faultPane;
		ResourceTestPane *resourceTestPane;
		DisplayableDataNode *dataNode;

		try
		{
			Type *type = control->GetType();
			if ( (type->Name->CompareTo("LogTreeListView") == 0) || (type->Name->CompareTo("NetworkLogListView") == 0) ||
				(type->Name->CompareTo("ScheduledTestListView") == 0) || (type->Name->CompareTo("CheckBoxListView") == 0))
			{
				//navigate up to the pane
				control = control->Parent;
				type = control->GetType();
			}
			else if (type->Name->CompareTo("CheckBox") == 0)
			{
				//navigate up to the fault pane
				control = control->Parent->Parent->Parent;
				type = control->GetType();
			}
			else if (type->Name->CompareTo("RadioButton") == 0)
			{
				// navigate up to the fault pane
				control = control->Parent->Parent->Parent;
				type = control->GetType();
			}
			else if (type->Name->CompareTo("ListView") == 0)
			{
				//navigate up to the fault pane
				control = control->Parent;
				type = control->GetType();
			}

			if (type->Name->CompareTo("LogPane") == 0)
			{
				logPane = dynamic_cast <LogPane*> (control);
				dataNode = new DisplayableDataNode("", UIData::Process, logPane->ProcessID, logPane->ThreadID);
			}
			else if (type->Name->CompareTo("ExceptionPane") == 0)
			{
				exceptionPane = dynamic_cast <ExceptionPane*> (control);
				dataNode = new DisplayableDataNode("", UIData::Process, exceptionPane->ProcessID, exceptionPane->ThreadID);
			}
			else if (type->Name->CompareTo("FileCorruptionPane") == 0)
			{
				fileCorruptionPane = dynamic_cast <FileCorruptionPane*> (control);
				dataNode = new DisplayableDataNode("", UIData::Process, fileCorruptionPane->ProcessID, 0);
			}
			else if (type->Name->CompareTo("SavedFileCorruptionPane") == 0)
			{
				savedFileCorruptionPane = dynamic_cast <SavedFileCorruptionPane*> (control);
				dataNode = new DisplayableDataNode("", UIData::Process, savedFileCorruptionPane->ProcessID, 0);
			}
			else if (type->Name->CompareTo("ScheduledTestPane") == 0)
			{
				testPane = dynamic_cast <ScheduledTestPane*> (control);
				dataNode = new DisplayableDataNode("", UIData::Process, testPane->ProcessID, testPane->ThreadID);
			}
			else if (type->Name->CompareTo("NetworkCorruptionPane") == 0)
			{
				networkPane = dynamic_cast <NetworkCorruptionPane*> (control);
				dataNode = new DisplayableDataNode("", UIData::Process, networkPane->ProcessID, 0);
			}
			else if (type->Name->CompareTo("FaultPane") == 0)
			{
				faultPane = dynamic_cast <FaultPane*> (control);
				dataNode = new DisplayableDataNode("", UIData::Process, faultPane->ProcessID, faultPane->ThreadID);
			}
			else if (type->Name->CompareTo("ResourceTestPane") == 0)
			{
				resourceTestPane = dynamic_cast <ResourceTestPane*> (control);
				dataNode = new DisplayableDataNode("", UIData::Process, resourceTestPane->ProcessID, 0);
			}
			else if (type->Name->CompareTo("NetworkLogPane") == 0)
			{
				networkLogPane = dynamic_cast <NetworkLogPane*> (control);
				dataNode = new DisplayableDataNode("", UIData::Process, networkLogPane->ProcessID, networkLogPane->ThreadID);
			}
			else if (type->Name->CompareTo("WindowContentTabbed") == 0)
			{
				WindowContentTabbed *windowContent = dynamic_cast <WindowContentTabbed*> (control);
				Control *tabbedControl = windowContent->CurrentContent->Control;
				Type *tabbedType = tabbedControl->GetType();

				if (tabbedType->Name->CompareTo("ProjectPane") == 0)
				{
					projectPane = dynamic_cast <ProjectPane*> (tabbedControl);
					
					TreeNode *treeNode = projectPane->SelectedNode;
					DisplayableDataNode *node = dynamic_cast <DisplayableDataNode*> (treeNode->Tag);
					if (node->processID != 0)
						dataNode = new DisplayableDataNode("", UIData::Process, node->processID, node->threadID);
					else if (displayWarning)
					{
						// Node was a global node without a process ID, can't use it.
						UserNotification::InfoNotify("Please select a pane or a project pane node associated with the application you want to test");
					}
				}
				else if (tabbedType->Name->CompareTo("LimitPane") == 0)
				{
					limitPane = dynamic_cast <LimitPane*> (tabbedControl);
					dataNode = new DisplayableDataNode("", UIData::Process, limitPane->ProcessID, 0);	
				}
				else if (tabbedType->Name->CompareTo("ResourcePane") == 0)
				{
					resourcePane = dynamic_cast <ResourcePane*> (tabbedControl);
					dataNode = new DisplayableDataNode("", UIData::Process, resourcePane->ProcessID, 0);
				}
				else if (displayWarning)
				{
					if (!IsProjectActive)
						UserNotification::InfoNotify("Please open a project first");
					else
						UserNotification::InfoNotify("Please select a pane or a project pane node associated with the application you want to test");
				}

			}
			else if (displayWarning)
			{
				if (!IsProjectActive)
					UserNotification::InfoNotify("Please open a project first");
				else
					UserNotification::InfoNotify("Please select a pane or a project pane node associated with the application you want to test");
			}
				
		}
		catch(...)
		{
			if (displayWarning)
			{
				if (!IsProjectActive)
					UserNotification::InfoNotify("Please open a project first");
				else
					UserNotification::InfoNotify("Please select a pane or a project pane node associated with the application you want to test");
			}
		}
        
		return dataNode;
	}

	//**************************************************************************
	// Method:		onThreadCreated
	// Description: called when a new thread has been created
	//
	// Parameters:
	//	pid - the id of the process the thread was created in
	//  tid - the id of the new thread
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::onThreadCreated(unsigned int pid, unsigned int tid)
	{
		threadCreatedMutex->WaitOne();

		try
		{
			HoloScriptApplication *holoScriptApp = holoScriptRegistry->getHoloScriptAppForProcess(pid);
			String *keyString;
			DisplayableDataNode *dataNode;

			if (!holoScriptApp || !IsProjectActive)
			{
				threadCreatedMutex->ReleaseMutex();
				return;
			}

			keyString = String::Concat(pid.ToString(), "_", tid.ToString());
			if (!createdThreadsTable->Contains(keyString))
				createdThreadsTable->Add(keyString, keyString);

			keyString = String::Concat("LogPaneDataArray", Convert::ToString(pid));
			keyString = String::Concat(keyString, Convert::ToString(tid));
			
			//if thread is unique then create a new logdata array
			//special case for logdata and netlogdata, all the other dataarrays are per-process, logdata and netlogdata are per-thread
			if (!currentProject->ContainsDataArrayKey(keyString))
			{
				LogPaneDataArray *logDataArray = new LogPaneDataArray(holoScriptApp->ApplicationName, pid, tid);
				currentProject->AddDataArray(keyString, logDataArray);
				dataNode = new DisplayableDataNode(holoScriptApp->ApplicationName, UIData::ActiveLogPane, pid, tid);
				dataNode->ID = String::Concat(pid.ToString(), tid.ToString());
				dataNode->DataArray = logDataArray;

				if (dataEventRegistry->OnLogPaneCreated)
					dataEventRegistry->OnLogPaneCreated->Invoke(dataNode);
			}

			keyString = String::Concat("NetworkLogPaneDataArray", Convert::ToString(pid));
			keyString = String::Concat(keyString, Convert::ToString(tid));

			// if thread is unique then create a new netlogdata array
			if (!currentProject->ContainsDataArrayKey(keyString))
			{
				NetworkLogPaneDataArray *logDataArray = new NetworkLogPaneDataArray(pid, tid);
				currentProject->AddDataArray(keyString, logDataArray);
				dataNode = new DisplayableDataNode(holoScriptApp->ApplicationName, UIData::ActiveNetworkLogPane, pid, tid);
				dataNode->ID = String::Concat(pid.ToString(), tid.ToString());
				dataNode->DataArray = logDataArray;

				if (dataEventRegistry->OnNetworkLogPaneCreated)
					dataEventRegistry->OnNetworkLogPaneCreated->Invoke(dataNode);
			}

			threadCreatedMutex->ReleaseMutex();
		}
		catch(...)
		{
			threadCreatedMutex->ReleaseMutex();
		}
	}

	//**************************************************************************
	// Method:		onInitializeAppForProcessChaining
	// Description: called to initialize the holoscriptapp object before it is
	//	actually attached to the new process on process chaining
	//
	// Parameters:
	//	parentHoloScriptApp - holoscript representing the parent process
	//	newHoloScriptApp - holoscript representing the new process
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onInitializeAppForProcessChaining(HoloScriptApplication *parentHoloScriptApp, HoloScriptApplication *newHoloScriptApp)
	{
		newHoloScriptApp->ProjectPath = currentProject->FullPath;
		newHoloScriptApp->OnInitializeAppForProcessChaining = new InitializeAppForProcessChainingDelegate(this, onInitializeAppForProcessChaining);
		newHoloScriptApp->OnProcessChainingAttachComplete = new ProcessChainingAttachCompleteDelegate(this, onProcessChainingAttachComplete);
		newHoloScriptApp->OnThreadCreated += new ThreadCreatedDelegate(this, onThreadCreated);
		newHoloScriptApp->OnCantCreateLogFile += new CantCreateLogFileDelegate(this, onCantCreateLogFile);
		newHoloScriptApp->OnHeatProcessTerminated += new HeatProcessTerminatedDelegate(this, onHeatProcessTerminated);
		newHoloScriptApp->OnHeatProcessPaused += new HeatProcessPausedDelegate(this, onHeatProcessPaused);
		newHoloScriptApp->OnHeatProcessResumed += new HeatProcessResumedDelegate(this, onHeatProcessResumed);
		newHoloScriptApp->IsChainedProcess = true;

		if (parentHoloScriptApp->InheritSettings)
		{
			newHoloScriptApp->FilteredFunctionList = parentHoloScriptApp->FilteredFunctionList;
			newHoloScriptApp->AttachDebugger = parentHoloScriptApp->AttachDebugger;
			newHoloScriptApp->LogFirstChance = parentHoloScriptApp->LogFirstChance;
		}
		else
		{
			// Disable memory, time, and .NET functions by default (we don't know if it's a .NET app yet, so
			// assume native at first)
			ArrayList* disabledFunctions = new ArrayList();

			String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
			InterceptedFunctionDB* interceptedFunctionDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
			if (!interceptedFunctionDB->IsLoaded)
				interceptedFunctionDB->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);

			System::Array* memoryFunctions = interceptedFunctionDB->GetFunctionCategoryArray("MEMORY");
			System::Array* timeFunctions = interceptedFunctionDB->GetFunctionCategoryArray("TIME");
			System::Array* dangerousFunctions = interceptedFunctionDB->GetFunctionCategoryArray("DANGEROUS");
			System::Array* miscellaneousFunction = interceptedFunctionDB->GetFunctionCategoryArray("MISCELLANEOUS");

			IEnumerator* enumerator = memoryFunctions->GetEnumerator();
			while (enumerator->MoveNext())
			{
				InterceptedFunction* func = dynamic_cast<InterceptedFunction*>(enumerator->Current);
				if (!disabledFunctions->Contains(func->Name))
					disabledFunctions->Add(func->Name);
			}

			enumerator = timeFunctions->GetEnumerator();
			while (enumerator->MoveNext())
			{
				InterceptedFunction* func = dynamic_cast<InterceptedFunction*>(enumerator->Current);
				if (!disabledFunctions->Contains(func->Name))
					disabledFunctions->Add(func->Name);
			}

			enumerator = dangerousFunctions->GetEnumerator();
			while (enumerator->MoveNext())
			{
				InterceptedFunction* func = dynamic_cast<InterceptedFunction*>(enumerator->Current);
				if (!disabledFunctions->Contains(func->Name))
					disabledFunctions->Add(func->Name);
			}

			enumerator = miscellaneousFunction->GetEnumerator();
			while (enumerator->MoveNext())
			{
				InterceptedFunction* func = dynamic_cast<InterceptedFunction*>(enumerator->Current);
				if (!disabledFunctions->Contains(func->Name))
					disabledFunctions->Add(func->Name);
			}

			//Disable the critical section funtions as well
			disabledFunctions->Add (S"LeaveCriticalSection");

			// Disable .NET functions
			interceptedFunctionDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
			if (!interceptedFunctionDB->IsLoaded)
				interceptedFunctionDB->LoadDatabase(functionFolder, DOT_NET_FUNC_DB_FILE_NAME);

			enumerator = interceptedFunctionDB->Functions->GetEnumerator();
			while (enumerator->MoveNext())
			{
				InterceptedFunction* func = dynamic_cast<InterceptedFunction*>(enumerator->Current);
				if (!disabledFunctions->Contains(func->Name))
					disabledFunctions->Add(func->Name);
			}

			newHoloScriptApp->FilteredFunctionList = disabledFunctions;
			newHoloScriptApp->AttachDebugger = true;
			newHoloScriptApp->LogFirstChance = false;
		}
	}

	//**************************************************************************
	// Method:		onProcessChainingAttachComplete
	// Description: called after the holoscriptapp object is attached to the
	//	new child process but before it is resumed
	//
	// Parameters:
	//	parentHoloScriptApp - holoscript representing the parent process
	//	newHoloScriptApp - holoscript representing the new process
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onProcessChainingAttachComplete(HoloScriptApplication *parentHoloScriptApp, HoloScriptApplication *newHoloScriptApp)
	{
		//Set the TerminateOnClose flag to true
		newHoloScriptApp->TerminateOnClose = true;

		UInt32 procID, threadID;
		newHoloScriptApp->GetApplicationProcessID(procID);
		newHoloScriptApp->GetApplicationThreadID(threadID);

		System::Diagnostics::Process *processList[];
		System::Diagnostics::Process *currentProcess;
		processList = System::Diagnostics::Process::GetProcesses();
		
		String *fullPath = S"";

		for (int i=0; i<processList->Length; i++)
		{
			String *procName;
			currentProcess = dynamic_cast <System::Diagnostics::Process*> (processList->GetValue(i));
			if (currentProcess->Id == procID)
			{
				try
				{
					procName = currentProcess->MainModule->FileName;
					fullPath = procName;
					int slashPosition = procName->LastIndexOf('\\');
					if (slashPosition != -1)
						procName = procName->Substring(slashPosition + 1);
				}
				catch(...)
				{
					procName = currentProcess->ProcessName;
				}

				newHoloScriptApp->ApplicationName = procName;
				break;
			}
		}

		if ((fullPath->CompareTo(S"") != 0) && (!parentHoloScriptApp->InheritSettings))
		{
			// See if the app is a .NET app.  If it is, and we're not inheriting settings, change the
			// default logging
			const wchar_t __pin* wszValue = PtrToStringChars(fullPath);
			if (CLRIsModuleManaged(wszValue))
			{
				// .NET app, disable all native functions by default
				ArrayList* disabledFunctions = new ArrayList();

				String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
				InterceptedFunctionDB* interceptedFunctionDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
				if (!interceptedFunctionDB->IsLoaded)
					interceptedFunctionDB->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);

				InterceptedFunctionDB* dotNetFunctionDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
				if (!dotNetFunctionDB->IsLoaded)
					dotNetFunctionDB->LoadDatabase(functionFolder, DOT_NET_FUNC_DB_FILE_NAME);

				System::Array *miscFunctions = dotNetFunctionDB->GetFunctionCategoryArray("MISCELLANEOUS");
				System::Array *timeFunctions = dotNetFunctionDB->GetFunctionCategoryArray("TIME");

				IEnumerator* enumerator = miscFunctions->GetEnumerator();
				while (enumerator->MoveNext())
				{
					InterceptedFunction* func = dynamic_cast<InterceptedFunction*>(enumerator->Current);
					if (!disabledFunctions->Contains(func->Name))
						disabledFunctions->Add(func->Name);
				}

				enumerator = timeFunctions->GetEnumerator();
				while (enumerator->MoveNext())
				{
					InterceptedFunction* func = dynamic_cast<InterceptedFunction*>(enumerator->Current);
					if (!disabledFunctions->Contains(func->Name))
						disabledFunctions->Add(func->Name);
				}

				enumerator = interceptedFunctionDB->Functions->GetEnumerator();
				while (enumerator->MoveNext())
				{
					InterceptedFunction* func = dynamic_cast<InterceptedFunction*>(enumerator->Current);
					if (!disabledFunctions->Contains(func->Name))
						disabledFunctions->Add(func->Name);
				}

				disabledFunctions->Add(S"System.IO.BinaryReader.Read()");
				disabledFunctions->Add(S"System.IO.BinaryReader.PeekChar()");

				newHoloScriptApp->FilteredFunctionList = disabledFunctions;
			}
		}

		CreateDataArrays(newHoloScriptApp);
	
		if (parentHoloScriptApp->InheritSettings)
		{
			InheritSettingsFromParentProcess(parentHoloScriptApp, newHoloScriptApp);
		}

		if (newHoloScriptApp->PauseOnStart)
		{
			onHeatProcessPaused(procID);
		}
	}

	//**************************************************************************
	// Method:		InheritSettingsFromParentProcess
	// Description: set all the parent settings to the child process
	//
	// Parameters:
	//  parentHoloScriptApp - The parent
	//	newHoloScriptApp - The child that needs to inherit settings
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::InheritSettingsFromParentProcess(HoloScriptApplication *parentHoloScriptApp, HoloScriptApplication *newHoloScriptApp)
	{
		DisplayableDataNode *dataNode;
		UInt32 parentProcessID, newProcessID;
		Type *type;
		DisplayableDataArray *dataArray;
		Object *args[] = __gc new Object*[1];

		newHoloScriptApp->FilteredFunctionList = parentHoloScriptApp->FilteredFunctionList;
		newHoloScriptApp->ProcessChaining = parentHoloScriptApp->ProcessChaining;
		newHoloScriptApp->InheritSettings = parentHoloScriptApp->InheritSettings;
		newHoloScriptApp->PauseOnStart = parentHoloScriptApp->PauseOnStart;
		newHoloScriptApp->AttachDebugger = parentHoloScriptApp->AttachDebugger;
		newHoloScriptApp->LogFirstChance = parentHoloScriptApp->LogFirstChance;

		for (int i = 0; i < currentProject->DisplayableDataArrayCount; i++)
		{
			dataArray = currentProject->GetDisplayableDataArray(i);
			type = dataArray->GetType();
			parentHoloScriptApp->GetApplicationProcessID(parentProcessID);
			newHoloScriptApp->GetApplicationProcessID(newProcessID);

			if ( (type->Name->CompareTo("FaultPaneDataArray") == 0) && (dataArray->ProcessID == parentProcessID) )
			{
				for (int j = 0; j < dataArray->Count; j++)
				{
					dataNode = new FaultPaneDataNode(dynamic_cast <FaultPaneDataNode*> (dataArray->GetDataNodeByIndex(j)));
					dataNode->processID = newProcessID;
					dataNode->UpdateDelegates();
					args[0] = dataNode;
					dataNode->CreateDelegate->DynamicInvoke(args);
				}
			}
			else if ( (type->Name->CompareTo("FileCorruptionDataArray") == 0) && (dataArray->ProcessID == parentProcessID) )
			{
				for (int j = 0; j < dataArray->Count; j++)
				{
					dataNode = new FileCorruptionDataNode(dynamic_cast <FileCorruptionDataNode*> (dataArray->GetDataNodeByIndex(j)));
					dataNode->processID = newProcessID;
					dataNode->UpdateDelegates();
					args[0] = dataNode;
					dataNode->CreateDelegate->DynamicInvoke(args);
				}
			}
			else if ( (type->Name->CompareTo("LimitPaneDataArray") == 0) && (dataArray->ProcessID == parentProcessID) )
			{
				for (int j = 0; j < dataArray->Count; j++)
				{
					dataNode = new LimitPaneDataNode(dynamic_cast <LimitPaneDataNode*> (dataArray->GetDataNodeByIndex(j)));
					dataNode->processID = newProcessID;
					dataNode->UpdateDelegates();
					args[0] = dataNode;
					dataNode->CreateDelegate->DynamicInvoke(args);
				}
			}
			else if ( (type->Name->CompareTo("NetworkCorruptionDataArray") == 0) && (dataArray->ProcessID == parentProcessID) )
			{
				for (int j = 0; j < dataArray->Count; j++)
				{
					dataNode = new NetworkCorruptionDataNode(dynamic_cast <NetworkCorruptionDataNode*> (dataArray->GetDataNodeByIndex(j)));
					dataNode->processID = newProcessID;
					dataNode->UpdateDelegates();
					args[0] = dataNode;
					dataNode->CreateDelegate->DynamicInvoke(args);
				}
			}
			else if ( (type->Name->CompareTo("ResourceTestDataArray") == 0) && (dataArray->ProcessID == parentProcessID) )
			{
				for (int j = 0; j < dataArray->Count; j++)
				{
					dataNode = new ResourceTestDataNode(dynamic_cast <ResourceTestDataNode*> (dataArray->GetDataNodeByIndex(j)));
					dataNode->processID = newProcessID;
					dataNode->UpdateDelegates();
					args[0] = dataNode;
					dataNode->CreateDelegate->DynamicInvoke(args);
				}
			}
			else if ( (type->Name->CompareTo("ScheduledTestDataArray") == 0) && (dataArray->ProcessID == parentProcessID) )
			{
				for (int j = 0; j < dataArray->Count; j++)
				{
					dataNode = new ScheduledTestDataNode(dynamic_cast <ScheduledTestDataNode*> (dataArray->GetDataNodeByIndex(j)));
					dataNode->processID = newProcessID;
					dataNode->UpdateDelegates();
					args[0] = dataNode;
					dataNode->CreateDelegate->DynamicInvoke(args);
				}
			}
		}
	}

	//**************************************************************************
	// Method:		DisplayTabPagesThreadFunction
	// Description: Thread function that displays the tab pages in the queue
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::DisplayTabPagesThreadFunction()
	{
		while (isRunning)
		{
			System::Threading::Monitor::Enter(pendingTabPagesToDisplay->SyncRoot);
			try
			{
				if (isRunning)
				{
					while (pendingTabPagesToDisplay->Count > 0)
					{
						NewProcessTabPage *page = dynamic_cast<NewProcessTabPage *>(pendingTabPagesToDisplay->Peek());
						// display the fault pane and log pane for the process
						Object *args[] = new Object* [2];
						args[0] = page->node;
						args[1] = __box(page->makeActive);
						IAsyncResult *result = Control::BeginInvoke(displayNewProcessHandler, args);
						result->AsyncWaitHandle->WaitOne(250, false);

						// peeked above so we don't lose the page if an exception is thrown.  Now remove it
						pendingTabPagesToDisplay->Dequeue();
					}
				}
				Thread::Sleep(500);
			}
			catch(Exception *)
			{
			}
			__finally
			{
				System::Threading::Monitor::Exit(pendingTabPagesToDisplay->SyncRoot);
			}
		}
	}

	//**************************************************************************
	// Method:		DisplayNewProcess
	// Description: Adds a new process's pages for displaying to the queue
	//
	// Parameters:
	//	logDataNode - the node representing the process' log pane
	//	faultDataNode - the node representing the process' fault pane
	//	makeTabActive - true to make the new tab pages the fore-most in the UI,
	//		false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::DisplayNewProcess(DisplayableDataNode *logDataNode, DisplayableDataNode *faultDataNode, Boolean makeTabActive)
	{
		System::Threading::Monitor::Enter(pendingTabPagesToDisplay->SyncRoot);
		try
		{
			NewProcessTabPage *tabPage = new NewProcessTabPage();
			tabPage->node = faultDataNode;
			tabPage->makeActive = makeTabActive;
			
			NewProcessTabPage *tabPage2 = new NewProcessTabPage();
			tabPage2->node = logDataNode;
			tabPage2->makeActive = false;

			pendingTabPagesToDisplay->Enqueue(tabPage2);
			pendingTabPagesToDisplay->Enqueue(tabPage);
		}
		catch(Exception *)
		{
		}
		__finally
		{
			System::Threading::Monitor::Exit(pendingTabPagesToDisplay->SyncRoot);
		}
	}

	//**************************************************************************
	// Method:		onDisplayNewProcess
	// Description: Displays the tab pages associated with a process
	//
	// Parameters:
	//	dataNode - the node to display
	//	makeTabActive - true to make the new tab pages the fore-most in the UI,
	//		false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void MainFrame::onDisplayNewProcess(DisplayableDataNode *dataNode, Boolean makeTabActive)
	{
		DisplayTabPage(dataNode, makeTabActive);
	}

	//**************************************************************************
	// Method:		CreateDataArrays
	// Description: Given a newly created holoscript app creates the data arrays and projectpane nodes
	//
	// Parameters:
	//	holoScriptApp - pointer to holoscript app to create arrays for
	//
	// Return Value: datanode with active pid and tid
	//*************************************************************************
	void MainFrame::CreateDataArrays(HoloScriptApplication *holoScriptApp)
	{
		DisplayableDataNode *dataNode;
		UInt32 procID, threadID;
		String *keyString;

		holoScriptApp->GetApplicationProcessID(procID);
		holoScriptApp->GetApplicationThreadID(threadID);
		holoScriptRegistry->addHoloScriptAppForProcess(procID, holoScriptApp);

		// don't make data arrays for services.exe on service launch
		if (holoScriptApp == HoloScriptRegistry::getInstance()->ServicesExeApplication)
			return;

		// create data arrays
		LogPaneDataArray *logDataArray = new LogPaneDataArray(holoScriptApp->ApplicationName, procID, 0);
		logDataArray->LoggedNewThreadID = new LoggedNewThreadIDDelegate(this, onLoggedNewThreadID);
		keyString = String::Concat(logDataArray->GetType()->Name, Convert::ToString(procID));
		keyString = String::Concat(keyString, "0");
		if (!currentProject->ContainsDataArrayKey(keyString))
			currentProject->AddDataArray(keyString, logDataArray);

		NetworkLogPaneDataArray *networkLogDataArray = new NetworkLogPaneDataArray(procID, 0);
		keyString = String::Concat(networkLogDataArray->GetType()->Name, Convert::ToString(procID));
		keyString = String::Concat(keyString, "0");
		if (!currentProject->ContainsDataArrayKey(keyString))
			currentProject->AddDataArray(keyString, networkLogDataArray);

		FaultPaneDataArray *faultDataArray = new FaultPaneDataArray(procID);
		keyString = String::Concat(faultDataArray->GetType()->Name, Convert::ToString(procID));
		keyString = String::Concat(keyString, "0");
		if (!currentProject->ContainsDataArrayKey(keyString))
			currentProject->AddDataArray(keyString, faultDataArray);

		NetworkCorruptionDataArray *ncDataArray = new NetworkCorruptionDataArray(procID);
		keyString = String::Concat(ncDataArray->GetType()->Name, Convert::ToString(procID));
		keyString = String::Concat(keyString, "0");
		if (!currentProject->ContainsDataArrayKey(keyString))
			currentProject->AddDataArray(keyString, ncDataArray);

		FileCorruptionDataArray *fcDataArray = new FileCorruptionDataArray(procID);
		keyString = String::Concat(fcDataArray->GetType()->Name, Convert::ToString(procID));
		keyString = String::Concat(keyString, "0");
		if (!currentProject->ContainsDataArrayKey(keyString))
			currentProject->AddDataArray(keyString, fcDataArray);

		SavedFileCorruptionDataArray *sfcDataArray = new SavedFileCorruptionDataArray(procID);
		keyString = String::Concat(sfcDataArray->GetType()->Name, Convert::ToString(procID));
		keyString = String::Concat(keyString, "0");
		if (!currentProject->ContainsDataArrayKey(keyString))
			currentProject->AddDataArray(keyString, sfcDataArray);

		ResourcePaneDataArray *resourceDataArray = new ResourcePaneDataArray(procID);
		keyString = String::Concat(resourceDataArray->GetType()->Name, Convert::ToString(procID));
		keyString = String::Concat(keyString, "0");
		if (!currentProject->ContainsDataArrayKey(keyString))
			currentProject->AddDataArray(keyString, resourceDataArray);
	
		LimitPaneDataArray *limitDataArray = new LimitPaneDataArray(procID);
		keyString = String::Concat(limitDataArray->GetType()->Name, Convert::ToString(procID));
		keyString = String::Concat(keyString, "0");
		if (!currentProject->ContainsDataArrayKey(keyString))
			currentProject->AddDataArray(keyString, limitDataArray);

		ScheduledTestDataArray *testDataArray = new ScheduledTestDataArray(procID);
		keyString = String::Concat(testDataArray->GetType()->Name, Convert::ToString(procID));
		keyString = String::Concat(keyString, "0");
		if (!currentProject->ContainsDataArrayKey(keyString))
			currentProject->AddDataArray(keyString, testDataArray);

		ResourceTestDataArray *resourceTestDataArray = new ResourceTestDataArray(procID);
		keyString = String::Concat(resourceTestDataArray->GetType()->Name, Convert::ToString(procID));
		keyString = String::Concat(keyString, "0");
		if (!currentProject->ContainsDataArrayKey(keyString))
			currentProject->AddDataArray(keyString, resourceTestDataArray);

		ExceptionDataArray *exceptionDataArray = new ExceptionDataArray(procID);
		keyString = String::Concat(exceptionDataArray->GetType()->Name, Convert::ToString(procID));
		keyString = String::Concat(keyString, "0");
		if (!currentProject->ContainsDataArrayKey(keyString))
			currentProject->AddDataArray(keyString, exceptionDataArray);

		ResourcePane *resPane = dynamic_cast <ResourcePane*> (resourcePane->Control);
		resPane->AddDataArray(resourceDataArray);
	
		LimitPane *limPane = dynamic_cast <LimitPane*> (limitPane->Control);
		limPane->AddDataArray(limitDataArray);
		
		//Add a project pane node for the process
		dataNode = new DisplayableDataNode(holoScriptApp->ApplicationName, UIData::ActiveLogPane, procID, 0);
		dataNode->ID = String::Concat(procID.ToString(), dataNode->threadID.ToString());
		dataNode->DataArray = logDataArray;
		dataEventRegistry->OnLogPaneCreated->Invoke(dataNode);

		DisplayableDataNode *faultPaneDataNode = new DisplayableDataNode("Faults", UIData::DiskFault, procID, 0);
		faultPaneDataNode->DataArray = faultDataArray;

		DisplayableDataNode *logPaneDataNode = new DisplayableDataNode(holoScriptApp->ApplicationName, UIData::ActiveLogPane, procID, 0);
		DisplayNewProcess(logPaneDataNode, faultPaneDataNode, !holoScriptApp->IsChainedProcess);

		//Add a node for the first thread
		logDataArray = new LogPaneDataArray(holoScriptApp->ApplicationName, procID, threadID);
		keyString = String::Concat(logDataArray->GetType()->Name, Convert::ToString(procID));
		keyString = String::Concat(keyString, Convert::ToString(threadID));
		if (!currentProject->ContainsDataArrayKey(keyString))
		{
			currentProject->AddDataArray(keyString, logDataArray);
			dataNode = new DisplayableDataNode(holoScriptApp->ApplicationName, UIData::ActiveLogPane, procID, threadID);
			dataNode->ID = String::Concat(procID.ToString(), dataNode->threadID.ToString());
			dataNode->DataArray = logDataArray;
			dataEventRegistry->OnLogPaneCreated->Invoke(dataNode);
		}

		//Add a node for network logs for the first thread
		networkLogDataArray = new NetworkLogPaneDataArray(procID, threadID);
		keyString = String::Concat(networkLogDataArray->GetType()->Name, Convert::ToString(procID));
		keyString = String::Concat(keyString, Convert::ToString(threadID));
		if (!currentProject->ContainsDataArrayKey(keyString))
		{
			currentProject->AddDataArray(keyString, networkLogDataArray);
			dataNode = new DisplayableDataNode(holoScriptApp->ApplicationName, UIData::ActiveNetworkLogPane, procID, threadID);
			dataNode->ID = String::Concat(procID.ToString(), dataNode->threadID.ToString());
			dataNode->DataArray = networkLogDataArray;
			dataEventRegistry->OnNetworkLogPaneCreated->Invoke(dataNode);
		}
	}

	//**************************************************************************
	// Method:		UpdateApplicationMenuItems
	// Description: updates the application menu items to match the app state
	//
	// Parameters:
	//	isPaused - true if the app is paused
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::UpdateApplicationMenuItems()
	{
		HoloScriptApplication *holoApp = NULL;
		DisplayableDataNode *dataNode = GetActiveNode(false);

		if (dataNode)
		{
			holoApp = holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID);
			if (!holoApp)
				holoApp = holoScriptRegistry->getHoloScriptAppForTerminatedProcess(dataNode->processID);
		}

		//RESTART is enabled when the AUT was launched and not attached to
		if (holoApp && !holoApp->ApplicationAttachedTo)
			applicationRestartMenuItem->Enabled = true;
		else
			applicationRestartMenuItem->Enabled = false;

		//REMOVE is enabled for all apps
		if (holoApp)
			applicationRemoveMenuItem->Enabled = true;
		else
			applicationRemoveMenuItem->Enabled = false;

		//Take care of Pause\Resume menuitems
		if (holoApp && holoApp->ApplicationPaused && holoApp->IsRunning)
		{
			applicationPauseMenuItem->Enabled = true;
			applicationPauseMenuItem->Checked = true;
		}
		else if (holoApp && !holoApp->ApplicationPaused && holoApp->IsRunning)
		{
			applicationPauseMenuItem->Enabled = true;
			applicationPauseMenuItem->Checked = false;
		}
		else
		{
			applicationPauseMenuItem->Enabled = false;
			applicationPauseMenuItem->Checked = false;
		}

		// Take care of AttachDebugger
		if (holoApp && holoApp->IsRunning)
		{
			applicationAttachDebuggerMenuItem->Checked = holoApp->AttachDebugger;
			applicationAttachDebuggerMenuItem->Enabled = true;
		}
		else
		{
			applicationAttachDebuggerMenuItem->Checked = false;
			applicationAttachDebuggerMenuItem->Enabled = false;
		}

		// Take care of LogFirstChance
		if (holoApp && holoApp->IsRunning && holoApp->AttachDebugger)
		{
			applicationLogFirstChanceMenuItem->Checked = holoApp->LogFirstChance;
			applicationLogFirstChanceMenuItem->Enabled = holoApp->AttachDebugger;
		}
		else
		{
			applicationLogFirstChanceMenuItem->Checked = false;
			applicationLogFirstChanceMenuItem->Enabled = false;
		}

		//Take care of InheritSettings menu item
		if (holoApp)
		{
			applicationInheritSettingsMenuItem->Enabled = true;
			applicationInheritSettingsMenuItem->Checked = holoApp->InheritSettings;
		}
		else
		{
			applicationInheritSettingsMenuItem->Enabled = false;
			applicationInheritSettingsMenuItem->Checked = false;
		}

		//Take care of Stop menuitem
		if (holoApp && holoApp->IsRunning)
			applicationStopMenuItem->Enabled = true;
		else
			applicationStopMenuItem->Enabled = false;
	}

	//**************************************************************************
	// Method:		onFocusChanged
	// Description: called when ui focus changes
	//
	// Parameters:
	//	dataNode - node that represents ui that has focus
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::onFocusChanged(DisplayableDataNode *dataNode)
	{
		UpdateApplicationMenuItems();

		if (!dataNode)
			return;

		HoloScriptApplication *holoScriptApp = holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID);

		if (!holoScriptApp)
			return;

		if (findNextDialog && netLogFindNextDialog)
		{
			if (((dataNode->Type == UIData::NodeType::ActiveLogPane) || (dataNode->Type == UIData::NodeType::SavedLogPane)) && netLogFindNextDialog->Visible)
			{
				logFindMenuItemClicked(NULL, EventArgs::Empty);
			}
			else if (((dataNode->Type == UIData::NodeType::ActiveNetworkLogPane) || (dataNode->Type == UIData::NodeType::SavedNetworkLogPane)) && findNextDialog->Visible)
			{
				logFindMenuItemClicked(NULL, EventArgs::Empty);
			}
			else if ((dataNode->Type == UIData::NodeType::ActiveLogPane) || (dataNode->Type == UIData::NodeType::SavedLogPane))
			{
				Crownwood::Magic::Controls::TabPage *tabPage = GetTabPage(dataNode);
				if (tabPage)
					findNextDialog->CurrentLogPane = dynamic_cast<LogPane *>(tabPage->Control);
			}
			else if ((dataNode->Type == UIData::NodeType::ActiveNetworkLogPane) || (dataNode->Type == UIData::NodeType::SavedNetworkLogPane))
			{
				Crownwood::Magic::Controls::TabPage *tabPage = GetTabPage(dataNode);
				if (tabPage)
					netLogFindNextDialog->CurrentLogPane = dynamic_cast<NetworkLogPane *>(tabPage->Control);
			}
		}
	}

	//**************************************************************************
	// Method:		onProjectPaneFocusChanged
	// Description: called when the focus is changed in the project pane
	//
	// Parameters:
	//	dataNode - node that represents the node that has focus
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::onProjectPaneFocusChanged(DisplayableDataNode *dataNode)
	{
		ReportPane *reportPane = NULL;
		DisplayableDataNode * newdataNode = NULL;
		ScheduledTestDataNode * sctDataNode = NULL;
		NetworkCorruptionDataNode * ncDataNode = NULL;
		
		Crownwood::Magic::Controls::TabPage *tabPage = NULL;

		bool callFocusDelegate = true;
		switch (dataNode->Type)
		{
		case NodeType::Report:
		case NodeType::Reports:
			tabPage = GetTabPage (dataNode);
			if (tabPage)
			{
				reportPane = dynamic_cast <ReportPane*> (tabPage->Control);
				if (reportPane)
				{
					newdataNode = new DisplayableDataNode("Holodeck Report", UIData::Report, 0, 0);
					newdataNode->DataArray = reportPane->DataArray;
				}
			}
			break;
		case NodeType::Resources:
			newdataNode = new DisplayableDataNode(dataNode);
			newdataNode->DataArray = currentProject->GetDataArray (dataNode);
			break;

		case NodeType::Test:
			sctDataNode = dynamic_cast <ScheduledTestDataNode *>((currentProject->GetDataArray(dataNode))->GetDataNodeByID (dataNode->ID));
			uiEventRegistry->OnFocusChanged(sctDataNode);
			callFocusDelegate = false;
			break;

		case NodeType::NetworkCorruptionFault:
			ncDataNode = dynamic_cast <NetworkCorruptionDataNode *>((currentProject->GetDataArray(dataNode))->GetDataNodeByID (dataNode->ID));
			uiEventRegistry->OnFocusChanged(ncDataNode);
			callFocusDelegate = false;
			break;

		default:
			break;
		}

		if (callFocusDelegate)
		{
			if (newdataNode)
				uiEventRegistry->OnFocusChanged(newdataNode);
			else
				uiEventRegistry->OnFocusChanged(dataNode);
		};
	}

	//**************************************************************************
	// Method:		DisplayTrialDisabledNotice
	// Description: displays a notice that some functionality is disabled due to
	//	using a trial version, and the displays the help page specified
	//
	// Parameters:
	//	helpUrl - the help page to display
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::DisplayTrialDisabledNotice(String *helpUrl)
	{
		UserNotification::InfoNotify("This feature is not available in the free trial version of Holodeck.\n\nHowever, if you purchase Holodeck this and all features of Holodeck will be available to you.\nThe help page for this feature will now be displayed.");
		String *url = String::Concat("file://", GuiSupport::RegistryHelper::getInstance()->GetInstallationPath(), "/", helpUrl);
		onHelpPaneLinkClicked(this, url);
	}

	//**************************************************************************
	// Method:		DisplayTrialTestLimitExceededNotice
	// Description: displays a notice that some functionality is disabled due to
	//	limitations placed on a trial version for the number of tests or nc faults
	//
	// Parameters:
	//	None
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::DisplayTrialTestLimitExceededNotice()
	{
		UserNotification::InfoNotify("Only one Test or Network Fuzzing Fault may be created in the free trial version of Holodeck.\n\nHowever, if you purchase Holodeck this and all features of Holodeck will be available to you.");
	}

	//**************************************************************************
	// Method:		ProcessRemoved
	// Description: Removes a process from the project
	//
	// Parameters:
	//	processIDToRemove - the pid of the process to remove
	//	removeTabPages	- if tab pages associated with the processID should be removed
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::ProcessRemoved(unsigned int processIDToRemove, bool removeTabPages)
	{
		if (removeTabPages)
		{
			//find the tabpages for the specified process
			ArrayList * tabPagesToRemove = new ArrayList();
			for (int i=0; i < nonDockablePaneManager->TabPages->Count; i++)
			{
				Crownwood::Magic::Controls::TabPage * tPage = dynamic_cast <Crownwood::Magic::Controls::TabPage *> (nonDockablePaneManager->TabPages->Item[i]);
				int charIndex = tPage->Title->LastIndexOf ("-");
				if (charIndex == -1 || charIndex > tPage->Title->Length)
					continue;
				String * tString = tPage->Title->Substring (charIndex + 1);
				charIndex = tString->LastIndexOf ("(");
				if (charIndex == -1 || charIndex > tString->Length)
					continue;
				tString = tString->Substring (0, charIndex);
				if (UInt32::Parse (tString) == processIDToRemove)
					tabPagesToRemove->Add (tPage);
			}
			
			//remove the tabpages for the specified process
			for (int rtp=0; rtp < tabPagesToRemove->Count; rtp++)
				nonDockablePaneManager->TabPages->Remove (dynamic_cast <Crownwood::Magic::Controls::TabPage *> (tabPagesToRemove->Item[rtp]));
		}
		//get the holoscript application
		HoloScriptApplication *holoScriptApplication = holoScriptRegistry->getHoloScriptAppForProcess(processIDToRemove);

		//terminate / detatch the application/service if it has not been terminated yet
		if (holoScriptApplication)
			holoScriptApplication->TerminateApplication();

		//find the data arrays for the specified process
		ArrayList * arraysToRemove = this->GetDataArraysForProcess (processIDToRemove, true);

		//if (holoScriptRegistry->Count == 0)
		this->uiEventRegistry->OnFocusChanged (NULL);

		//remove the data arrays for the specified process
		for (int rem=0; rem < arraysToRemove->Count; rem++)
			currentProject->RemoveDataArray (dynamic_cast <DisplayableDataArray *> (arraysToRemove->Item[rem]));

		//remove the holoscript app from the holoscript registry
		holoScriptRegistry->removeHoloScriptAppForTerminatedProcess(processIDToRemove);
	}

	//**************************************************************************
	// Method:		GetDataArraysForProcess
	// Description: Returns the data arrays for a specified process
	//
	// Parameters:
	//	processID - the pid of the process for which data arrays are required
	//	includeLogs - true, if the return value should contain log arrays we well
	//
	// Return Value: An arraylist of data-arrays which belong to the sepcified process
	//*************************************************************************
	ArrayList * MainFrame::GetDataArraysForProcess(unsigned int processID, bool includeLogs)
	{
		FaultPaneDataArray * tFaultPaneDataArray;
		FileCorruptionDataArray * tFileCorruptionDataArray;
		LogPaneDataArray * tLogPaneDataArray;
		NetworkCorruptionDataArray * tNetworkCorruptionDataArray;
		RecordedSessionDataArray * tRecordedSessionDataArray;
		ReportPaneDataArray * tReportPaneDataArray;
		ResourcePaneDataArray * tResourcePaneDataArray;
		LimitPaneDataArray * tLimitPaneDataArray;
		ResourceTestDataArray *tResourceTestDataArray;
		SavedFileCorruptionDataArray *tSavedFileCorruptionDataArray;
		ScheduledTestDataArray *tScheduledTestDataArray;
		ExceptionDataArray *tExceptionDataArray;

		DisplayableDataArray * dataArray;
		ArrayList * array = new ArrayList();
		Type * type;
		//find the data arrays for the specified process
		for (int i = 0; i < currentProject->DisplayableDataArrayCount; i++)
		{
			dataArray = currentProject->GetDisplayableDataArray(i);
			type = dataArray->GetType();

			if (type->Name->CompareTo ("FaultPaneDataArray") == 0)
			{
				tFaultPaneDataArray = dynamic_cast <FaultPaneDataArray *> (dataArray);
				if (tFaultPaneDataArray && tFaultPaneDataArray->ProcessID == processID)
					array->Add (dataArray);
			}
			else if (type->Name->CompareTo ("FileCorruptionDataArray") == 0)
			{
				tFileCorruptionDataArray = dynamic_cast <FileCorruptionDataArray *> (dataArray);
				if (tFileCorruptionDataArray && tFileCorruptionDataArray->ProcessID == processID)
					array->Add (dataArray);
			}
			else if (type->Name->CompareTo ("LogPaneDataArray") == 0)
			{
				tLogPaneDataArray = dynamic_cast <LogPaneDataArray *> (dataArray);
				if (tLogPaneDataArray && tLogPaneDataArray->ProcessID == processID && includeLogs)
					array->Add (dataArray);
			}
			else if (type->Name->CompareTo ("NetworkCorruptionDataArray") == 0)
			{
				tNetworkCorruptionDataArray = dynamic_cast <NetworkCorruptionDataArray *> (dataArray);
				if (tNetworkCorruptionDataArray && tNetworkCorruptionDataArray->ProcessID == processID)
					array->Add (dataArray);
			}
			else if (type->Name->CompareTo ("RecordedSessionDataArray") == 0)
			{
				tRecordedSessionDataArray = dynamic_cast <RecordedSessionDataArray *> (dataArray);
				if (tRecordedSessionDataArray && tRecordedSessionDataArray->ProcessID == processID)
					array->Add (dataArray);
			}
			else if (type->Name->CompareTo ("ReportPaneDataArray") == 0)
			{
				tReportPaneDataArray = dynamic_cast <ReportPaneDataArray *> (dataArray);
				if (tReportPaneDataArray && tReportPaneDataArray->ProcessID == processID)
					array->Add (dataArray);
			}
			else if (type->Name->CompareTo ("ResourcePaneDataArray") == 0)
			{
				tResourcePaneDataArray = dynamic_cast <ResourcePaneDataArray *> (dataArray);
				if (tResourcePaneDataArray && tResourcePaneDataArray->ProcessID == processID)
					array->Add (dataArray);
			}
			else if (type->Name->CompareTo ("LimitPaneDataArray") == 0)
			{
				tLimitPaneDataArray = dynamic_cast <LimitPaneDataArray *> (dataArray);
				if (tLimitPaneDataArray && tLimitPaneDataArray->ProcessID == processID)
					array->Add (dataArray);
			}
			else if (type->Name->CompareTo ("ResourceTestDataArray") == 0)
			{
				tResourceTestDataArray = dynamic_cast <ResourceTestDataArray *> (dataArray);
				if (tResourceTestDataArray && tResourceTestDataArray->ProcessID == processID)
					array->Add (dataArray);
			}
			else if (type->Name->CompareTo ("SavedFileCorruptionDataArray") == 0)
			{
				tSavedFileCorruptionDataArray = dynamic_cast <SavedFileCorruptionDataArray *> (dataArray);
				if (tSavedFileCorruptionDataArray && tSavedFileCorruptionDataArray->ProcessID == processID)
					array->Add (dataArray);
			}
			else if (type->Name->CompareTo ("ScheduledTestDataArray") == 0)
			{
				tScheduledTestDataArray = dynamic_cast <ScheduledTestDataArray *> (dataArray);
				if (tScheduledTestDataArray && tScheduledTestDataArray->ProcessID == processID)
					array->Add (dataArray);
			}
			else if (type->Name->CompareTo ("ExceptionDataArray") == 0)
			{
				tExceptionDataArray = dynamic_cast <ExceptionDataArray *> (dataArray);
				if (tExceptionDataArray && tExceptionDataArray->ProcessID == processID)
					array->Add (dataArray);
			}
		}

		return array;
	}

	//**************************************************************************
	// Method:		onPauseProcess
	// Description: called to pause a process
	//
	// Parameters:
	//  processID - process to be paused
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::onPauseProcess(unsigned int processID)
	{
		applicationPauseMenuItemClicked(NULL, NULL);
	}

	//**************************************************************************
	// Method:		onResumeProcess
	// Description: called to resume a process
	//
	// Parameters:
	//  processID - process to be resumed
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::onResumeProcess(unsigned int processID)
	{
		applicationPauseMenuItemClicked(NULL, NULL);
	}

	//**************************************************************************
	// Method:		onTerminateProcess
	// Description: called to terminate a process
	//
	// Parameters:
	//  processID - process to be terminated
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::onTerminateProcess(unsigned int processID)
	{
		applicationStopMenuItemClicked(NULL, NULL);
	}

	//**************************************************************************
	// Method:		onRestartProcess
	// Description: called to restart a process
	//
	// Parameters:
	//  processID - process to be restarted
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::onRestartProcess(unsigned int processID)
	{
		applicationRestartMenuItemClicked(NULL, NULL);
	}

	//**************************************************************************
	// Method:		onRemoveProcess
	// Description: called to remove a process
	//
	// Parameters:
	//  processID - process to be removed
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::onRemoveProcess(unsigned int processID)
	{
		applicationRemoveMenuItemClicked(NULL, NULL);
	}

	//**************************************************************************
	// Method:		onHeatProcessTerminated
	// Description: called when a process has been terminated
	//
	// Parameters:
	//  processID - process that has been terminated
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::onHeatProcessTerminated(unsigned int processID)
	{
		//make sure holoscriptregistry is updated, unless we are running code coverage, then we should wait
		if (!codeCoverageRunning)
			holoScriptRegistry->TerminateApplication(processID);

		if (uiEventRegistry->OnProcessTerminated)
			uiEventRegistry->OnProcessTerminated->Invoke(processID);

		//modify icon for all tabpages that match this pid
		Crownwood::Magic::Controls::TabPage *tabPage;
		Crownwood::Magic::Collections::TabPageCollection *pages = nonDockablePaneManager->TabPages;
		IEnumerator *pageEnumerator = pages->GetEnumerator();
		while (pageEnumerator->MoveNext())
		{
			tabPage = dynamic_cast <Crownwood::Magic::Controls::TabPage*> (pageEnumerator->Current);
			if ( (!tabPage) || (tabPage->Title->IndexOf(processID.ToString()) == -1) )
				continue;
			tabPage->ImageIndex = TERMINATED_PROCESS_INDEX;
		}

		UpdateApplicationMenuItems();
	}

	//**************************************************************************
	// Method:		onHeatProcessPaused
	// Description: called when a process has been paused
	//
	// Parameters:
	//  processID - process that has been paused
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::onHeatProcessPaused(unsigned int processID)
	{
		if (uiEventRegistry->OnProcessPaused)
			uiEventRegistry->OnProcessPaused->Invoke(processID);

		//modify icon for all tabpages that match this pid
		Crownwood::Magic::Controls::TabPage *tabPage;
		Crownwood::Magic::Collections::TabPageCollection *pages = nonDockablePaneManager->TabPages;
		IEnumerator *pageEnumerator = pages->GetEnumerator();
		while (pageEnumerator->MoveNext())
		{
			tabPage = dynamic_cast <Crownwood::Magic::Controls::TabPage*> (pageEnumerator->Current);
			if ( (!tabPage) || (tabPage->Title->IndexOf(processID.ToString()) == -1) )
				continue;
			tabPage->ImageIndex = PAUSED_PROCESS_INDEX;
		}

		UpdateApplicationMenuItems();
	}

	//**************************************************************************
	// Method:		onHeatProcessResumed
	// Description: called when a process has been resumed
	//
	// Parameters:
	//  processID - process that has been resumed
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::onHeatProcessResumed(unsigned int processID)
	{
		if (uiEventRegistry->OnProcessResumed)
			uiEventRegistry->OnProcessResumed->Invoke(processID);

		//modify icon for all tabpages that match this pid
		Crownwood::Magic::Controls::TabPage *tabPage;
		Crownwood::Magic::Collections::TabPageCollection *pages = nonDockablePaneManager->TabPages;
		IEnumerator *pageEnumerator = pages->GetEnumerator();
		while (pageEnumerator->MoveNext())
		{
			tabPage = dynamic_cast <Crownwood::Magic::Controls::TabPage*> (pageEnumerator->Current);
			if ( (!tabPage) || (tabPage->Title->IndexOf(processID.ToString()) == -1) )
				continue;
			tabPage->ImageIndex = tabPage->DefaultImageIndex;
		}

		UpdateApplicationMenuItems();
	}

	//**************************************************************************
	// Method:		DisplayStartPage
	// Description: displays the start html page in the non dockable area
	//
	// Parameters:
	//  None
	//
	// Return Value: none
	//*************************************************************************
	void MainFrame::DisplayStartPage()
	{
		this->nonDockablePaneManager->displayHelpPane(startPageUrl);
	}

	//**************************************************************************
	// Method:		getFocusedControlText
	// Description: Gets the text (or selected text) from the control which
	//				is selected and has focus within the supplied collection
	//				of controls
	//
	// Parameters:
	//  controls -	a collection of controls which are to be searched for a
	//				control which has focus
	//
	// Return Value: Text to be used for copying to the clipboard, NULL if no
	//				 text is found
	//*************************************************************************
	String* MainFrame::getFocusedControlText(Control::ControlCollection * controls)
	{
		bool isFound = false;
		String * returnValue = NULL;
		for (int i=0; i < controls->Count; i++)
		{
			Control * c = controls->Item[i];
			if (c->Focused)
			{
				returnValue = this->getSelectedTextFromControl (c);
				isFound = true;
			}
			else if (c->Controls->Count > 0)
			{
				String * tmpReturnValue = getFocusedControlText (c->Controls);
				if (tmpReturnValue != NULL)
				{
					returnValue = tmpReturnValue;
					isFound = true;
				}
			}
		}
		if (isFound)
			return returnValue;
		else
			return NULL;
	};

	//**********************************************************************************
	// Method:		getSelectedTextFromControl
	// Description: Checks whether the supplied control supports the SelectedText
	//				property, if it does, then this method returns the selected text
	//				otherwise it returns the complete text within the supplied control
	//
	// Parameters:
	//  c -			the control from which text is required
	//
	// Return Value: the text within the control
	//**********************************************************************************
	String* MainFrame::getSelectedTextFromControl(Control * c)
	{
		PropertyInfo * pSelectedText = c->GetType()->GetProperty ("SelectedText");

		if (pSelectedText)
		{
			Object * selObject = pSelectedText->GetValue (c, NULL);
			if (selObject)
			{
				String * selection = dynamic_cast <String *> (selObject);
				if (selection)
					return selection;
			}
		}
		
		return c->Text;
	}

	//**********************************************************************************
	// Method:		UpdateHolodeckWindowTitle
	// Description: Appends the specified projectTitle to the Holodeck window caption
	//
	// Parameters:
	//  projectTitle - the name of the project
	//
	// Return Value: None
	//**********************************************************************************
	void MainFrame::UpdateHolodeckWindowTitle(String * projectTitle)
	{
		String * title = "";
		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
			title = HOLODECK_TRIAL_TITLE_TEXT;
		else
			title = HOLODECK_FULL_TITLE_TEXT;

		if (projectTitle && projectTitle->Trim()->Length > 0)
			title = String::Concat (projectTitle, S" - ", title);
		else if (projectTitle == NULL)
			title = String::Concat (S"Untitled - ", title);

		this->Text = title;
	}

	//**********************************************************************************
	// Method:		CheckForFaultyUserLibraries
	// Description: Checks for user generated intercepts which may be missing files
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//**********************************************************************************
	void MainFrame::CheckForFaultyUserLibraries()
	{
		if (!this->silentMode)
		{
			//Check for faulty user replacement libraries
			FaultyUserLibrariesDialog * ulDialog = new FaultyUserLibrariesDialog();
			ulDialog->PerformCheckAndPopulateList();
			if (ulDialog->ContainsFaultyModules)
				uiEventRegistry->OnShowDialog(ulDialog);
		}
	}

	//**********************************************************************************
	// Method:		onLoggedNewThreadID
	// Description: called when a process's main log data array gets notice of a new thread
	//	being logged
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//**********************************************************************************
	void MainFrame::onLoggedNewThreadID(unsigned int processID, unsigned int threadID)
	{
		String *keyString = String::Concat(processID.ToString(), "_", threadID.ToString());
		if (!createdThreadsTable->Contains(keyString))
		{
			onThreadCreated(processID, threadID);
		}
	}

	//**********************************************************************************
	// Method:		onShowFileCorruptionDetailsPane
	// Description: called when the file corruption details pane should be shown
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//**********************************************************************************
	void MainFrame::onShowFileCorruptionDetailsPane()
	{
		dockManager->ShowContent(fileCorruptDetailsPane);
		fileCorruptDetailsPane->BringToFront();
	}

	//**********************************************************************************
	// Method:		onShowNetworkCorruptionDetailsPane
	// Description: called when the network corruption details pane should be shown
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//**********************************************************************************
	void MainFrame::onShowNetworkCorruptionDetailsPane()
	{
		dockManager->ShowContent(networkMessageDetailsPane);
		networkMessageDetailsPane->BringToFront();
	}

	void MainFrame::launchNewIntercepts()
	{
		// Check if we are even to try running this feature
		// BUGBUG: replace LIBRARY_EXPORTER_HELP_FILE with help file for New Intercepts help file
		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
		{
			DisplayTrialDisabledNotice(LIBRARY_EXPORTER_HELP_FILE);
			return;
		}

		// Check if .NET 2.0 "is" available
		RegistryKey *nkey = Microsoft::Win32::Registry::LocalMachine->OpenSubKey("Software\\Microsoft\\.NETFramework\\Policy\\v2.0");
		if (!nkey)
		{
			AddNewIntercepts::AddCustomInterceptWizard *p = new AddNewIntercepts::AddCustomInterceptWizard();
			uiEventRegistry->OnShowDialog(p);
		}
		else
		{
			Microsoft::Win32::RegistryKey *key = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(ROOT_PATH);
			if (!key) return;
			String *HolodeckInstallPath			= dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));

			System::Diagnostics::Process *ne	= new System::Diagnostics::Process();
			String *sNewInterceptExePath		= (String*) HolodeckInstallPath->Clone();
			sNewInterceptExePath				= String::Concat(HolodeckInstallPath, L"\\NewIntercepts\\NewIntercepts.exe");

			ne->Start(sNewInterceptExePath);
		}
	}
}