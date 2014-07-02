#include <windows.h>

int main()
{
	int nTimes = 20;

	for (int iCount = 0; iCount < nTimes; iCount++)
	{
		CreateFile("TEST", NULL, NULL, NULL, NULL, NULL, NULL);
	}

	MessageBox(0,0,0,0);
	return 0;
}