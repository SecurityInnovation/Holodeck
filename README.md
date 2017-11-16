Holodeck
========
Holodeck is a [Fault Injection](http://en.wikipedia.org/wiki/Fault_injection) tool for testing Windows binaries and .NET applications. Holodeck utilizes fault-injection techniques to introduce the application to simulated scenarios that arise as the result of "broken" environments, such as out of memory conditions, corrupt files, bad registry data, or mauled network packets. It encapsulates the target application within a simulated, tester-controlled environment that exists between the application and operating system. It is therefore able to introduce and test the application's reaction to various anomalies of the environment by changing the simulation without actually effecting the base operating or file system.

While Holodeck is intercepting all your system and API calls, it is also monitoring and logging them. This gives you the power to examine low-level interactions and network packets - pinpointing and easily recreating bug-generating events with ease. Meanwhile, the integrated debugger creates a "minidump" when errors occur and provides the exact line of code and machine state where the crash occurred. With this information, suitable for use directly in an IDE like Visual Studio, you or your developer will know exactly what was happening at the time an error occurred - such as if your application was trying to access a restricted piece of memory, looking for a file that wasn’t there, or write sensitive/private data to a text file during a crash etc.

[Download Holodeck Installer](https://www.dropbox.com/s/vbb9ahtikaoyrrp/Holodeck%202.8%282_8_0%29.exe) 

Roadmap
--------------

Holodeck currently isn't quite working. It was working last time we released it in 2008, but since then we haven't tried to build it and there are quite a few things that we'd like to change. I'll outline the roadmap that I think will be best for Holodeck's life:

 - Get it to build on VS 2005 with .NET 3.5 on Windows Vista (I believe this was the last configuration that worked)
 - Update it to build on the most recent version of VS and .NET. Still target Vista for HEAT and Holodeck.
 - Remove License key requirements to run and create a new build to host.
 - Update HEAT-NG to work on Windows8
 - Integrate HEAT-NG into Holodeck so it will work on Windows8
 - World domination?


Comprehensive Simulation and Tuning for Fragility & Security Testing
--------------------

Holodeck is a unique test tool that uses fault injection to simulate real-world application and system errors for Windows applications and services. Testers and Developers work in a controlled, repeatable environment to analyze and debug error-handling code and application attack surface – it’s ideal for adept testers doing application fragility and security testing.

Holodeck is the first commercially available fault-simulation tool and was developed by leading researchers in the application quality field.  It is used by organizations like Microsoft, Adobe, Symantec, EMC and McAfee to ensure the products delivered to their customers are of the highest quality. 

Unprecedented Control
----------------------

Faulty error-handling code is a major contributor of reliability, security, and functional defects - and typically goes uncovered by conventional testing methods.   Holodeck's point-and-click environment simulation provides powerful and comprehensive reliability and error handling testing without disrupting the rest of your system. Because you have complete control over all Windows, .Net,  and API calls that your application makes, you can trick your application into thinking that it is out of memory, network bandwidth is maxed out, or that a registry file or key it needs is corrupt. Holodeck can be used as a stand-alone tool or in conjunction with Automated Test Tools or IDE's such as VisualStudio.

Holodeck provides testers and developers with the following benefits:

 - Safe fault injection and environment simulation
 - Comprehensive reporting
 - Application insight via detailed application monitoring
 - API integration with automated testing tools
 - Built-in debugger for fast problem solving
 - Automated scheduled and random test generation
 - more features and benefits

Technical
----------------

Version:  2.8, originally released 04/04/08
 
**Supported Platforms**

 - O/S Environments:  Windows Vista, Windows 2003, Windows XP, Windows 2000 (after Vista is not supported)
 - Programming languages:  all, works on binaries
 - .Net Versions:   1.0 through 3.5 (after .NET 3.5 not supported)

**Minimum Configuration**

 - Windows 2000 SP4
 - 1Ghz CPU
 - 512K RAM
 - 120MB Free Disk Space
