using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.ServiceProcess;
using System.Configuration.Install;
using System.Collections.Specialized;

namespace TTScheduler
{
    [RunInstallerAttribute(true)]
    public class TTServiceInstaller: Installer
    {
        private ServiceInstaller serviceInstaller;
        private ServiceProcessInstaller processInstaller;

        public TTServiceInstaller ( )
        {            
            // Delete the TTScheduler event log if it already exists...
            if ( EventLog.Exists( "TTScheduler" ) )
                EventLog.Delete( "TTScheduler" );

            serviceInstaller = new ServiceInstaller( );
            processInstaller = new ServiceProcessInstaller( );            

            // Use a valid user account to run the service...
            processInstaller.Account = ServiceAccount.User;

            // Set the service to start automatically...
            serviceInstaller.StartType = ServiceStartMode.Automatic;
            serviceInstaller.ServiceName = "TTScheduler";
            serviceInstaller.DisplayName = "TestTracker Scheduler";

            Installers.Add( serviceInstaller );
            Installers.Add( processInstaller );
        }
        
    }


	public class TTScheduler : System.ServiceProcess.ServiceBase
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
        private RunQueue runQueue = new RunQueue( );

		public TTScheduler()
		{
			// This call is required by the Windows.Forms Component Designer.
			InitializeComponent();

			// TODO: Add any initialization after the InitComponent call
		}

		// The main entry point for the process
		static void Main()
		{
			System.ServiceProcess.ServiceBase[] ServicesToRun;
	
			// More than one user Service may run within the same process. To add
			// another service to this process, change the following line to
			// create a second service object. For example,
			//
			//   ServicesToRun = new System.ServiceProcess.ServiceBase[] {new Service1(), new MySecondUserService()};
			//
			ServicesToRun = new System.ServiceProcess.ServiceBase[] { new TTScheduler() };

			System.ServiceProcess.ServiceBase.Run(ServicesToRun);
		}

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			components = new System.ComponentModel.Container();
			this.ServiceName = "TTScheduler";            
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		/// <summary>
		/// Set things in motion so your service can do its work.
		/// </summary>
		protected override void OnStart(string[] args)
		{
			runQueue.StartScheduler( );
		}
 
		/// <summary>
		/// Stop this service.
		/// </summary>
		protected override void OnStop()
		{
            // Disconnect the connection to the database...
			runQueue.dbConnection.Disconnect( );
		}
	}
}
