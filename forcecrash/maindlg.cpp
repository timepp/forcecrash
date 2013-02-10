#include "stdafx.h"
#include "maindlg.h"
#include "processinfo.h"
#include "crash.h"

static CStringW RCString(UINT id)
{
	WCHAR buf[1024];
	::LoadStringW(GetModuleHandle(NULL), id, buf, _countof(buf));
	return buf;
}

LRESULT CMainDlg::OnOK(int /*code*/, int /*id*/, HWND /*wnd*/, BOOL& /*handled*/)
{
	EndDialog(IDOK);
	return 0;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	DlgResize_Init(true, true);

	HICON hIcon = (HICON)::LoadImageW(
		GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_ICON1), IMAGE_ICON, 
		::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), 0
		);
	SetIcon(hIcon, TRUE);
	hIcon = (HICON)::LoadImageW(
		GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_ICON1), IMAGE_ICON, 
		::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0
		);
	SetIcon(hIcon, FALSE);

	ConfigureList();
	CenterWindow();
	RefreshList();
	AdjustList();
	return TRUE;
}

void CMainDlg::ConfigureList()
{
	m_list.SubclassWindow(GetDlgItem(IDC_LIST));
	m_list.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_list.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER|LVS_EX_INFOTIP);

	m_list.AddColumn(RCString(IDS_PROCESSNAME), 0);
	m_list.AddColumn(RCString(IDS_PROCESSID), 1);
	m_list.AddColumn(RCString(IDS_PARENTPROCESSID), 2);
	m_list.AddColumn(RCString(IDS_COMMANDLINE), 3);
	
	m_list.SetColumnSortType(1, LVCOLSORT_LONG);
	m_list.SetColumnSortType(2, LVCOLSORT_LONG);

	m_list.SetColumnWidth(0, 150);
	m_list.SetColumnWidth(1, 50);
	m_list.SetColumnWidth(2, 80);
	m_list.SetColumnWidth(3, 800);
}

void CMainDlg::RefreshList()
{
	// ¼Ç×¡×´Ì¬
	CStringW oldpid;
	int index = m_list.GetSelectedIndex();
	if (index >= 0)
	{
		m_list.GetItemText(index, 1, oldpid);
	}

	int sortcol = m_list.GetSortColumn();
	bool desc = m_list.IsSortDescending();
	int scrollpos = m_list.GetTopIndex();

	m_list.DeleteAllItems();

	process_list_t pl;
	pinfo::get_process_list(pl);
	
	index = 0;
	int selected_index = -1;
	for (process_list_t::const_iterator it = pl.begin(); it != pl.end(); ++it)
	{
		const process_info& pi = *it;
		if (pi.command_line.empty()) continue;
		m_list.AddItem(index, 0, pi.name.c_str());
		m_list.SetItemText(index, 1, tp::cz(L"%u", pi.pid));
		m_list.SetItemText(index, 2, tp::cz(L"%u", pi.ppid));
		m_list.SetItemText(index, 3, pi.command_line.c_str());
		index++;
	}

	if (sortcol >= 0) m_list.SortItems(sortcol, desc);
	CStringW pid;
	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		m_list.GetItemText(i, 1, pid);
		if (pid == oldpid)
		{
			m_list.SelectItem(i);
			break;
		}
	}
}

LRESULT CMainDlg::OnBnClickedRefresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	RefreshList();
	return 0;
}

LRESULT CMainDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CDialogResize<CMainDlg>::OnSize(uMsg, wParam, lParam, bHandled);
	AdjustList();
	return 0;
}

void CMainDlg::AdjustList()
{
	RECT rc;
	m_list.GetClientRect(&rc);
	m_list.SetColumnWidth(3, rc.right - rc.left - 255);
}

LRESULT CMainDlg::OnNMDblclkList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	int index = m_list.GetSelectedIndex();
	if (index >= 0)
	{
		CStringW name;
		CStringW pidstr;
		CStringW cmdline;
		m_list.GetItemText(index, 0, name);
		m_list.GetItemText(index, 1, pidstr);
		m_list.GetItemText(index, 3, cmdline);
		CStringW confirm_text = RCString(IDS_CRASH_CONFIRM);
		confirm_text.Replace(L"%1", name);
		confirm_text.Replace(L"%2", cmdline);
		if (MessageBox(confirm_text, RCString(IDS_CONFIRM), MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
		{
			DWORD pid = (DWORD)_wtoi(pidstr);
			HRESULT hr = crash::CrashProcess(pid);
			if (FAILED(hr))
			{
				WCHAR buffer[1024];
				::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, 0, buffer, _countof(buffer), NULL);
				MessageBox(buffer, L"´íÎó", MB_ICONWARNING|MB_OK);
			}
		}
	}
	
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonElevate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	WCHAR path[MAX_PATH];
	::GetModuleFileNameW(NULL, path, _countof(path));
	HINSTANCE inst = ::ShellExecuteW(NULL, L"runas", path, NULL, NULL, SW_SHOW);
	if ((UINT)inst > 32)
	{
		EndDialog(IDOK);
	}
	return 0;
}
