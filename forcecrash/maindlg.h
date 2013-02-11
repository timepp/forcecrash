#pragma once

#include "resource.h"
#include "processinfo.h"

class CMainDlg: public CDialogImpl<CMainDlg>, public CDialogResize<CMainDlg>
{
public:
	enum {IDD = IDD_MAINDLG};

	BEGIN_MSG_MAP(CMainDlg)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnOK)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		COMMAND_HANDLER(IDC_REFRESH, BN_CLICKED, OnBnClickedRefresh)
		NOTIFY_HANDLER(IDC_LIST, NM_DBLCLK, OnNMDblclkList)
		COMMAND_HANDLER(IDC_BUTTON_ELEVATE, BN_CLICKED, OnBnClickedButtonElevate)
		CHAIN_MSG_MAP(CDialogResize<CMainDlg>)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(CMainDlg)
		DLGRESIZE_CONTROL(IDC_LIST, DLSZ_SIZE_X|DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_REFRESH, DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BUTTON_ELEVATE, DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_TIP, DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

private:
	void ConfigureList();
	void RefreshList();
	void AdjustList();
	CSortListViewCtrl m_list;
private:
	LRESULT OnOK(int code, int id, HWND wnd, BOOL& handled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnBnClickedRefresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
	LRESULT OnNMDblclkList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonElevate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
