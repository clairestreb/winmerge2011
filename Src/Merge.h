/////////////////////////////////////////////////////////////////////////////
//    WinMerge:  an interactive diff/merge utility
//    Copyright (C) 1997  Dean P. Grimm
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
/////////////////////////////////////////////////////////////////////////////
/** 
 * @file  Merge.h
 *
 * @brief main header file for the MERGE application
 *
 */
// ID line follows -- this is updated by SVN
// $Id$

#include "resource.h"       // main symbols
#include "scriptable.h"

class CMainFrame;
class MergeCmdLineInfo;

const TCHAR WinMergeWindowClass[] = _T("WinMergeWindowClassW");

/////////////////////////////////////////////////////////////////////////////
// CMergeApp:
// See Merge.cpp for the implementation of this class
//

/**
 * @brief Frame/View/Document types.
 */
enum FRAMETYPE
{
	FRAME_FOLDER, /**< Folder compare frame. */
	FRAME_FILE, /**< File compare frame. */
	FRAME_BINARY, /**< Binary compare frame. */
	FRAME_OTHER, /**< No frame? */
};

enum { IDLE_TIMER = 9754 };

/** 
 * @brief WinMerge application class
 */
extern class CMergeApp
{
public:
	CMergeApp();
	~CMergeApp();

public:
	HINSTANCE m_hInstance;
	CMainFrame *m_pMainWnd;


	static String GetDefaultEditor();
	static String GetDefaultSupplementFolder();

	void ResetOptions();

	BOOL PreTranslateMessage(MSG* pMsg);

	BOOL InitInstance();
	int ExitInstance();
	int DoMessageBox(LPCTSTR lpszPrompt, UINT nType = MB_OK, UINT nIDPrompt = 0);

	static void InitializeSupplements();
	BOOL ParseArgsAndDoOpen(MergeCmdLineInfo& cmdInfo, CMainFrame* pMainFrame);
	// End MergeArgs.cpp

	//@{
	/**
	 * @name Active operations counter.
	 * These functions implement counter for active operations. We need to
	 * track active operations during whose user cannot exit the application.
	 * E.g. copying files in folder compare is such an operation.
	 */
	/**
	 * Increment the active operation counter.
	 */
	void AddOperation() { InterlockedIncrement(&m_nActiveOperations); }
	/**
	 * Decrement the active operation counter.
	 */
	void RemoveOperation()
	{
		ASSERT(m_nActiveOperations > 0);
		InterlockedDecrement( &m_nActiveOperations);
	}
	/**
	 * Get the active operations count.
	 */
	LONG GetActiveOperations() const { return m_nActiveOperations; }
	//@}

private:
	bool m_bNonInteractive;
	LONG m_nActiveOperations; /**< Active operations count. */
} theApp;

#include "EditorFilepathBar.h"

class CMainFrame;

class CDocFrame : public OWindow
{
public:
	CMainFrame *const m_pMDIFrame;
	CEditorFilePathBar m_wndFilePathBar;
	struct HandleSet
	{
		LONG m_cRef;
		UINT m_id;
		HMENU m_hMenuShared;
		HACCEL m_hAccelShared;
	} *const m_pHandleSet;
	void ActivateFrame();
	void DestroyFrame();
	virtual void SavePosition()
	{
	}
	virtual void RecalcLayout();
	virtual BOOL PreTranslateMessage(MSG *)
	{
		return FALSE;
	}
	FRAMETYPE GetFrameType()
	{
		return this ? static_cast<const CDocFrame *>(this)->GetFrameType() : FRAME_OTHER;
	}
protected:
	HWindow *m_pWndLastFocus;
	virtual FRAMETYPE GetFrameType() const = 0;
	CDocFrame(CMainFrame *, HandleSet *, const LONG *FloatScript, const LONG *SplitScript = NULL);
	~CDocFrame();
	void EnableModeless(BOOL bEnable);
	template<UINT id>
	static HandleSet *GetHandleSet()
	{
		static HandleSet handleSet = { 0, id, NULL, NULL };
		return &handleSet;
	}
	typedef CDocFrame CMDIChildWnd;
	virtual LRESULT WindowProc(UINT, WPARAM, LPARAM);
private:
	// Avoid DestroyWindow() in favor of DestroyFrame()
	using OWindow::DestroyWindow;
};

class CSubFrame
	: public OWindow
	, public CFloatState
{
public:
	CSubFrame(CDocFrame *pDocFrame, const LONG *FloatScript, UINT uHitTestCode);
protected:
	UINT const m_uHitTestCode;
	CDocFrame *const m_pDocFrame;
	virtual LRESULT WindowProc(UINT, WPARAM, LPARAM);
};

template<UINT ids_singular, UINT ids_plural>
class FormatAmount : public String
{
public:
	FormatAmount(int n)
	{
		UINT id = n == 1 ? ids_singular : ids_plural;
		append_sprintf(LanguageSelect.LoadString(id).c_str(), n);
	}
};

#include "DiffTextBuffer.h"
#include "DiffWrapper.h"
#include "DiffList.h"
#include "stringdiffs.h"
#include "TempFile.h"
#include "DiffFileInfo.h"
#include "FileFilterHelper.h"
#include "FileTransform.h"
#include "ChildFrm.h"
