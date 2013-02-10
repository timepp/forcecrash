#include "stdafx.h"
#include "maindlg.h"

#include <list>

CMainDlg g_dlg;
CAppModule _Module;

int Run(LPWSTR /*lpstrCmdLine*/ = NULL, int /*nCmdShow*/ = SW_SHOWDEFAULT)
{
	g_dlg.DoModal();
	return 0;
}

int APIENTRY wWinMain(HINSTANCE hInstance,
					  HINSTANCE /*hPrevInstance*/,
					  LPWSTR    /*lpCmdLine*/,
					  int       /*nCmdShow*/)
{
	_Module.Init(NULL, hInstance);
	Run();
	_Module.Term();
	return 0;
}
