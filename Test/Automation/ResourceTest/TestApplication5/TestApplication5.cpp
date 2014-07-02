// This is the main project file for VC++ application project 
// generated using an Application Wizard::

#using <mscorlib.dll>

using namespace System;

int main()
{
	try {
		/*<Resource>*/Microsoft::Win32::RegistryKey *rKey0 = Microsoft::Win32::RegistryKey::OpenRemoteBaseKey (Microsoft::Win32::RegistryHive::ClassesRoot, String::Empty);
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::RegistryKey *rKey0 = Microsoft::Win32::RegistryKey::OpenRemoteBaseKey (Microsoft::Win32::RegistryHive::CurrentConfig, String::Empty);
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::RegistryKey *rKey0 = Microsoft::Win32::RegistryKey::OpenRemoteBaseKey (Microsoft::Win32::RegistryHive::DynData, String::Empty);
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::RegistryKey *rKey0 = Microsoft::Win32::RegistryKey::OpenRemoteBaseKey (Microsoft::Win32::RegistryHive::LocalMachine, String::Empty);
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::RegistryKey *rKey0 = Microsoft::Win32::RegistryKey::OpenRemoteBaseKey (Microsoft::Win32::RegistryHive::PerformanceData, String::Empty);
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::RegistryKey *rKey0 = Microsoft::Win32::RegistryKey::OpenRemoteBaseKey (Microsoft::Win32::RegistryHive::Users, String::Empty);
	} catch (Exception *) {
	}

	try {
		/*<Resource>*/Microsoft::Win32::Registry::ClassesRoot->GetSubKeyNames ();
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::ClassesRoot->CreateSubKey ("TestKey1");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::ClassesRoot->DeleteSubKey ("TestKey1");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::ClassesRoot->CreateSubKey ("TestKey2");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::ClassesRoot->DeleteSubKeyTree ("TestKey2");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::ClassesRoot->SetValue ("newValue", S"newData");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::ClassesRoot->GetValue ("newValue");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::ClassesRoot->DeleteValue ("newValue");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::ClassesRoot->Flush ();
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::ClassesRoot->Close ();
	} catch (Exception *) {
	}

	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentConfig->GetSubKeyNames ();
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentConfig->CreateSubKey ("TestKey1");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentConfig->DeleteSubKey ("TestKey1");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentConfig->CreateSubKey ("TestKey2");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentConfig->DeleteSubKeyTree ("TestKey2");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentConfig->SetValue ("newValue", S"newData");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentConfig->GetValue ("newValue");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentConfig->DeleteValue ("newValue");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentConfig->Flush ();
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentConfig->Close ();
	} catch (Exception *) {
	}

	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentUser->GetSubKeyNames ();
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentUser->CreateSubKey ("TestKey1");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentUser->DeleteSubKey ("TestKey1");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentUser->CreateSubKey ("TestKey2");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentUser->DeleteSubKeyTree ("TestKey2");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentUser->SetValue ("newValue", S"newData");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentUser->GetValue ("newValue");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentUser->DeleteValue ("newValue");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentUser->Flush ();
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::CurrentUser->Close ();
	} catch (Exception *) {
	}

	try {
		/*<Resource>*/Microsoft::Win32::Registry::DynData->GetSubKeyNames ();
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::DynData->CreateSubKey ("TestKey1");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::DynData->DeleteSubKey ("TestKey1");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::DynData->CreateSubKey ("TestKey2");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::DynData->DeleteSubKeyTree ("TestKey2");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::DynData->SetValue ("newValue", S"newData");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::DynData->GetValue ("newValue");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::DynData->DeleteValue ("newValue");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::DynData->Flush ();
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::DynData->Close ();
	} catch (Exception *) {
	}

	try {
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::LocalMachine->GetSubKeyNames ();
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::LocalMachine->CreateSubKey ("TestKey1");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::LocalMachine->DeleteSubKey ("TestKey1");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::LocalMachine->CreateSubKey ("TestKey2");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::LocalMachine->DeleteSubKeyTree ("TestKey2");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::LocalMachine->SetValue ("newValue", S"newData");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::LocalMachine->GetValue ("newValue");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::LocalMachine->DeleteValue ("newValue");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::LocalMachine->Flush ();
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::LocalMachine->Close ();
	} catch (Exception *) {
	}

	try {
		/*<Resource>*/Microsoft::Win32::Registry::PerformanceData->GetSubKeyNames ();
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::PerformanceData->CreateSubKey ("TestKey1");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::PerformanceData->DeleteSubKey ("TestKey1");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::PerformanceData->CreateSubKey ("TestKey2");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::PerformanceData->DeleteSubKeyTree ("TestKey2");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::PerformanceData->SetValue ("newValue", S"newData");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::PerformanceData->GetValue ("newValue");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::PerformanceData->DeleteValue ("newValue");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::PerformanceData->Flush ();
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::PerformanceData->Close ();
	} catch (Exception *) {
	}

	try {
		/*<Resource>*/Microsoft::Win32::Registry::Users->GetSubKeyNames ();
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::Users->CreateSubKey ("TestKey1");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::Users->DeleteSubKey ("TestKey1");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::Users->CreateSubKey ("TestKey2");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::Users->DeleteSubKeyTree ("TestKey2");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::Users->SetValue ("newValue", S"newData");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::Users->GetValue ("newValue");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::Users->DeleteValue ("newValue");
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::Users->Flush ();
	} catch (Exception *) {
	}
	try {
		/*<Resource>*/Microsoft::Win32::Registry::Users->Close ();
	} catch (Exception *) {
	}

	try {
		/*<Resource>*/Microsoft::Win32::RegistryKey *rKey1 = Microsoft::Win32::RegistryKey::OpenRemoteBaseKey (Microsoft::Win32::RegistryHive::LocalMachine, String::Empty);
		try {
			/*<Resource>*/rKey1 = rKey1->OpenSubKey ("SOFTWARE");
			/*<Resource>*/rKey1 = rKey1->OpenSubKey ("HolodeckEE", true);
			try {
				/*<Resource>*/Microsoft::Win32::RegistryKey *rKey2 = rKey1->CreateSubKey ("ResourcePaneTestKey1");
				try {
					/*<Resource>*/rKey2->CreateSubKey ("newKey");
				} catch (Exception *) {
				}
				try {
					/*<Resource>*/rKey2->DeleteSubKeyTree ("newKey");
				} catch (Exception *) {
				}
				try {
					/*<Resource>*/rKey2->SetValue ("newValue", S"newData");
				} catch (Exception *) {
				}
				try {
					/*<Resource>*/rKey2->GetValue ("newValue");
				} catch (Exception *) {
				}
				try {
					/*<Resource>*/rKey2->DeleteValue ("newValue");
				} catch (Exception *) {
				}
				try {
					/*<Resource>*/rKey2->Flush ();
				} catch (Exception *) {
				}
				try {
					/*<Resource>*/rKey2->GetValueNames ();
				} catch (Exception *) {
				}
				try {
					/*<Resource>*/rKey2->OpenSubKey ("NonExistent");
				} catch (Exception *) {
				}
				try {
					/*<Resource>*/rKey2->Close ();
				} catch (Exception *) {
				}
			} catch (Exception *) {
			}
			/*<Resource>*/rKey1->DeleteSubKey ("ResourcePaneTestKey1");
			/*<Resource>*/rKey1->Close ();
		} catch (Exception *) {
		}
	} catch (Exception *) {
	}

	return 0;
}