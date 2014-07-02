Copyright (c) 1998-9
Dr John Maddock

Permission to use, copy, modify, distribute and sell this software
and its documentation for any purpose is hereby granted without fee,
provided that the above copyright notice appear in all copies and
that both that copyright notice and this permission notice appear
in supporting documentation.  Dr John Maddock makes no representations
about the suitability of this software for any purpose.  
It is provided "as is" without express or implied warranty.

************************************************************************

Full instructions for use of this library are contained inside
regex.htm.


Installation and Configuration Options
When you extract the library from its zip file, you must preserve 
its internal directory structure (for example by using the -d option 
when extracting). If you didn't do that when extracting, then you'd 
better stop reading this, delete the files you just extracted, and 
try again! 

Currently the library will automatically detect and configure itself 
for Borland, Microsoft and gcc compilers only. The library will also 
detect the HP, SGI, Rogue Wave, or Microsoft STL implementations. If 
the STL type is detected, then the library will attempt to extract 
suitable compiler configuration options from the STL used. Otherwise 
the library will assume that the compiler is fully compliant with the 
C++ standard: unless various options are defined to depreciate features 
not implemented by your compiler. These options are documented in 
<boost/re_detail/regex_options.hpp>, if you want to add permanent 
configuration options add them to <boost/re_detail/regex_options.hpp> 
which is provided for this purpose - this will allow you to keep your 
configuration options between library versions by retaining 
<boost/re_detail/regex_options.hpp>. 

The library will encase all code inside namespace JM unless the macro 
JM_NO_NAMESPACES is defined, JM is a macro that defaults to "jm", but 
can be defined on the command line to be whatever you want. You can 
also use the macro JM_STD to refer to the namespace enclosing your 
standard template library.

Unlike some other template libraries, this library consists of a mixture 
of template code (in the headers) and static code and data (in cpp files). 
Consequently it is necessary to build the library's support code into a 
library or archive file before you can use it, instructions for specific 
platforms are as follows:

Borland C++ Builder: 

Open up a console window and change to the <regex++>\libs\regex\lib directory. 
Select the appropriate makefile (bcb4.mak for C++ Builder 4, bcb5.mak for C++ 
Builder 5, and bcc55.mak for the 5.5 command line tools, use bcb4.mak for 
older versions of C++ Builder). 
Invoke the makefile (pass the full path to your version of make if you have 
more than one version installed, the makefile relies on the path to make to 
obtain your C++ Builder installation directory and tools) for example: 
make -fbcb5.mak
The build process will build a variety of .lib and .dll files (the exact 
number depends upon the version of Borland's tools you are using) the .lib 
and dll files will be in a sub-directory called bcb4 or bcb5 depending upon 
the makefile used. To install the libraries into your development system use:

make -fbcb5.mak install

library files will be copied to <BCROOT>/lib and the dll's to <BCROOT>/bin, 
where <BCROOT> corresponds to the install path of your Borland C++ tools. 

You may also remove temporary files created during the build process (excluding 
lib and dll files) by using:

make -fbcb5.mak clean

Finally when you use regex++ it is only necessary for you to add the <regex++>/include 
directory to your list of include directories for that project. It is not necessary 
for you to manually add a .lib file to the project; the headers will automatically 
select the correct .lib file for your build mode and tell the linker to include it. 
There is one caveat however: the library can not tell the difference between VCL 
and non-VCL enabled builds when building a GUI application from the command line, 
if you build from the command line with the 5.5 command line tools then you must 
define the pre-processor symbol _NO_VCL in order to ensure that the correct link 
libraries are selected: the C++ Builder IDE normally sets this automatically. 
Hint, users of the 5.5 command line tools may want to add a -D_NO_VCL to bcc32.cfg 
in order to set this option permanently. 
  
  

Microsoft Visual C++ 6 

Open up a command prompt, which has the necessary MSVC environment variables 
defined (for example by using the batch file Vcvars32.bat installed by the 
Visual Studio installation), and change to the <regex++>\libs\regex\lib directory. 

Select the correct makefile - vc6.mak for "vanilla" Visual C++ 6 or vc6-stlport.mak 
if you are using STLPort 4.

Invoke the makefile like this:

nmake -fvc6.mak

You will now have a collection of lib and dll files in a "vc6" subdirectory, to 
install these into your development system use:

nmake -fvc6.mak install

The lib files will be copied to your <VC6>\lib directory and the dll files to 
<VC6>\bin, where <VC6> is the root of your Visual C++ 6 installation.

You can delete all the temporary files created during the build (excluding lib 
and dll files) using:

nmake -fvc6.mak clean 

Finally when you use regex++ it is only necessary for you to add the <regex++>/include 
directory to your list of include directories for that project. It is not necessary 
for you to manually add a .lib file to the project; the headers will automatically 
select the correct .lib file for your build mode and tell the linker to include it. 

Note: if you have replaced the C++ standard library that comes with VC6, then when 
you build the library you must ensure that the environment variables "INCLUDE" and 
"LIB" have been updated to reflect the include and library paths for the new library - 
see vcvars32.bat (part of your Visual Studio installation) for more details. 

If you are building with the full STLPort v4, then use the vc6-stlport.mak file 
provided (The full STLPort libraries appear not to support single-thread static builds). 
  
  

GCC(2.95) 

There is a conservative makefile for the g++ compiler. From the command prompt change 
to the <regex++>/lib directory and type: 

make -fgcc.mak 

At the end of the build process you should have a gcc sub-directory containing 
release and debug versions of the library (libregex++.a and libregex++debug.a). 
When you build projects that use regex++, you will need to add the <regex++>/include 
directory to your list of include paths and add <regex++>/lib/regex++ to your list 
of library files. 

Otherwise: run configure, this will set up the headers and generate makefiles, 
from the command prompt change to the <regex++>/ directory and type: 

configure
make
Other make options include: 

make jgrep: builds the jgrep demo. 

make test: builds and runs the regression tests. 

make timer: builds the timer demo program. 

Note: gcc2.95.x on Win32 is only supported as cygwin and not mingw32 (sorry but 
compiler related bugs prevent this). 

Other compilers: 

Run configure, this will set up the headers and generate makefiles: from the 
command prompt change to the <regex++>/ directory and type: 

configure
make
Other make options include: 

make jgrep: builds the jgrep demo. 

make test: builds and runs the regression tests. 

make timer: builds the timer demo program. 

Troubleshooting: 

If make fails after running configure, you may need to manually disable some 
options: configure uses simple tests to determine what features your compiler 
supports, it does not stress the compiler's internals to any degree as the 
actual regex++ code can do. Other compiler features may be implemented (and 
therefore detected by configure) but known to be buggy, again in this case 
it may be necessary to disable the feature in order to compile regex++ to 
stable code. The output file from configure is <regex++>/include/jm/jm_cfg.h, 
this file lists all the macros that can be defined to configure regex++ along 
with a description to illustrate their usage, experiment changing options in 
jm_cfg.h one at a time until you achieve the effect you require. If you mail 
me questions about configure output, be sure to include both jm_cfg.h and 
config.log with your message. 


