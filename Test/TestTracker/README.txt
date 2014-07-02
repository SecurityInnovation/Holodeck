To get the test tracker working, you must first  :

	1. Install IIS on Windows 2000-2003 Server.
	2. Install the ASP.NET Web Components for IIS.
	3. Create a new virtual root with proper ASP.NET execution rights called "testtracker."
	4. Set the default web page of the site to be "testtracker.aspx."
	4. Copy the "testtracker" folder contents to the location of the virtual root.
	5. Copy the webctrl_client to a new virtual root of it's own (required for treeview to work).
	6. Install the scheduling service using the .NET Framework "installutil" tool on the service executable.
	7. Ensure that the service is installed to use LocalSystem account and "interact with desktop."
	8. Restore the supplied SQL database to your SQL Server.
	9. Start the SQL Server.
	10. Delete any tests you do not want to immediately run (since the scheduler will attempt to run
            those since they haven't been run in a long time).
	11. Start the scheduler service.
	12. Navigate to the website http://computername/testtracker/testtracker.aspx.

* To perform all these tasks, you should compile the components on your host machine in order to match with your
current .NET version.
