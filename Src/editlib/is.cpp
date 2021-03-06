///////////////////////////////////////////////////////////////////////////
//  File:       is.cpp
//  Version:    1.2.0.5
//  Created:    29-Dec-1998
//
//  Copyright:  Stcherbatchenko Andrei
//  E-mail:     windfall@gmx.de
//
//  Implementation of the CCrystalEditView class, a part of the Crystal Edit -
//  syntax coloring text editor.
//
//  You are free to use or modify this code to the following restrictions:
//  - Acknowledge me somewhere in your about box, simple "Parts of code by.."
//  will be enough. If you can't (or don't want to), contact me personally.
//  - LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//  16-Aug-99
//      Ferdinand Prantl:
//  +   FEATURE: corrected bug in syntax highlighting C comments
//  +   FEATURE: extended levels 1- 4 of keywords in some languages
//
//  ... it's being edited very rapidly so sorry for non-commented
//        and maybe "ugly" code ...
////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ccrystaltextbuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using CommonKeywords::IsNumeric;

// InstallShield keywords (IS3.0)
static BOOL IsISKeyword(LPCTSTR pszChars, int nLength)
{
	static LPCTSTR const s_apszISKeywordList[] =
	{
		_T("BOOL"),
		_T("BYREF"),
		_T("CHAR"),
		_T("HIWORD"),
		_T("HWND"),
		_T("INT"),
		_T("LIST"),
		_T("LONG"),
		_T("LOWORD"),
		_T("NUMBER"),
		_T("POINTER"),
		_T("QUAD"),
		_T("RGB"),
		_T("SHORT"),
		_T("STRING"),
		_T("begin"),
		_T("call"),
		_T("case"),
		_T("declare"),
		_T("default"),
		_T("downto"),
		_T("else"),
		_T("elseif"),
		_T("end"),
		_T("endif"),
		_T("endswitch"),
		_T("endwhile"),
		_T("external"),
		_T("for"),
		_T("function"),
		_T("goto"),
		_T("if"),
		_T("program"),
		_T("prototype"),
		_T("repeat"),
		_T("return"),
		_T("step"),
		_T("switch"),
		_T("then"),
		_T("to"),
		_T("typedef"),
		_T("until"),
		_T("void"),
		_T("while"),
	};
	return xiskeyword<_tcsncmp>(pszChars, nLength, s_apszISKeywordList);
}

static BOOL IsUser1Keyword(LPCTSTR pszChars, int nLength)
{
	static LPCTSTR const s_apszUser1KeywordList[] =
	{
		_T("AFTER"),
		_T("APPEND"),
		_T("BACK"),
		_T("BEFORE"),
		_T("CANCEL"),
		_T("COMMAND"),
		_T("COMMANDEX"),
		_T("CONTINUE"),
		_T("DISABLE"),
		_T("DLG_ERR"),
		_T("ENABLE"),
		_T("EXCLUSIVE"),
		_T("EXISTS"),
		_T("FAILIFEXISTS"),
		_T("FALSE"),
		_T("FULL"),
		_T("NEXT"),
		_T("NO"),
		_T("NONEXCLUSIVE"),
		_T("NOSET"),
		_T("OFF"),
		_T("ON"),
		_T("PARTIAL"),
		_T("REPLACE"),
		_T("REPLACE_ITEM"),
		_T("RESET"),
		_T("RESTART"),
		_T("SET"),
		_T("TRUE"),
		_T("YES"),
	};
	return xiskeyword<_tcsncmp>(pszChars, nLength, s_apszUser1KeywordList);
}

static BOOL IsUser2Keyword(LPCTSTR pszChars, int nLength)
{
	static LPCTSTR const s_apszUser2KeywordList[] =
	{
		_T("CMDLINE"),
		_T("ERRORFILENAME"),
		_T("INFOFILENAME"),
		_T("ISRES"),
		_T("ISUSER"),
		_T("ISVERSION"),
		_T("SRCDIR"),
		_T("SRCDISK"),
		_T("SUPPORTDIR"),
		_T("TARGETDIR"),
		_T("TARGETDISK"),
		_T("WINDIR"),
		_T("WINDISK"),
		_T("WINSYSDIR"),
		_T("WINSYSDISK"),
	};
	return xiskeyword<_tcsncmp>(pszChars, nLength, s_apszUser2KeywordList);
}

/* built-in functions
		_T("AddFolderIcon"),
		_T("AddProfString"),
		_T("AddressNumber"),
		_T("AddressString"),
		_T("AddressStruct"),
		_T("AppCommandControls"),
		_T("AskDestPath"),
		_T("AskOptionsCreates"),
		_T("AskPath"),
		_T("AskText"),
		_T("AskYesNoCreates"),
		_T("BatchAddAdds"),
		_T("BatchDeleteEx"),
		_T("BatchFileLoad"),
		_T("BatchFileSave"),
		_T("BatchFind"),
		_T("BatchGetFileNameRetrieves"),
		_T("BatchMoveEx"),
		_T("BatchSetFileNameDetermines"),
		_T("CallDLLFx"),
		_T("ChangeDirectory"),
		_T("CloseFile"),
		_T("CmdGetHwndDlg"),
		_T("CommitSharedFiles"),
		_T("ComponentAddItemAdds"),
		_T("ComponentDialog"),
		_T("ComponentGetItemInfoRetrieves"),
		_T("ComponentGetItemSizeQueries"),
		_T("ComponentIsItemSelected"),
		_T("ComponentListItemsCreates"),
		_T("ComponentSelectItem"),
		_T("ComponentSetItemInfoSets"),
		_T("ComponentTotalSizeCalculates"),
		_T("CompressAdd"),
		_T("CompressDel"),
		_T("CompressEnumEnumerates"),
		_T("CompressGet"),
		_T("CompressInfoRetrieves"),
		_T("ConfigAdd"),
		_T("ConfigDeleteDeletes"),
		_T("ConfigFileLoadLoads"),
		_T("ConfigFileSaveSaves"),
		_T("ConfigFindSearches"),
		_T("ConfigGetFileName"),
		_T("ConfigGetIntRetrieves"),
		_T("ConfigMoveMoves"),
		_T("ConfigSetFileName"),
		_T("ConfigSetIntSets"),
		_T("CopyBytes"),
		_T("CopyFileCopies"),
		_T("CreateDir"),
		_T("CreateFileCreates"),
		_T("CreateProgramFolder"),
		_T("CtrlClear"),
		_T("CtrlDir"),
		_T("CtrlGetCurSel"),
		_T("CtrlGetMLETextRetrieves"),
		_T("CtrlGetMultCurSel"),
		_T("CtrlGetStateRetrieves"),
		_T("CtrlGetSubCommand"),
		_T("CtrlGetText"),
		_T("CtrlPGroups"),
		_T("CtrlSelectTextSelects"),
		_T("CtrlSetCurSel"),
		_T("CtrlSetFont"),
		_T("CtrlSetList"),
		_T("CtrlSetMLETextSets"),
		_T("CtrlSetMultCurSel"),
		_T("CtrlSetStateSets"),
		_T("CtrlSetText"),
		_T("DefineDialogRegisters"),
		_T("DeinstallStartStarts"),
		_T("Delay"),
		_T("DeleteDir"),
		_T("DeleteFileDeletes"),
		_T("DeleteFolderIconRemoves"),
		_T("DeleteProgramFolder"),
		_T("Disable"),
		_T("DoInitiates"),
		_T("DoInstall"),
		_T("EnableEnables"),
		_T("EndDialog"),
		_T("EnterDisk"),
		_T("ExistsDir"),
		_T("ExistsDiskChecks"),
		_T("ExitProgMan"),
		_T("EzBatchAddPathAdds"),
		_T("EzBatchAddStringAdds"),
		_T("EzBatchReplaceReplaces"),
		_T("EzConfigAddDriver"),
		_T("EzConfigAddString"),
		_T("EzConfigGetValueRetrieves"),
		_T("EzConfigSetValueRetrieves"),
		_T("EzDefineDialogRegisters"),
		_T("FileCompare"),
		_T("FileDeleteLineDeletes"),
		_T("FileGrepSearches"),
		_T("FileInsertLineInserts"),
		_T("FileSetBeginDefineStarts"),
		_T("FileSetEndDefineEnds"),
		_T("FileSetEndPerform"),
		_T("FileSetPerformPerforms"),
		_T("FileSetPerformEzExecutes"),
		_T("FileSetResetResets"),
		_T("FileSetRoot"),
		_T("FindAllDirs"),
		_T("FindAllFilesSearches"),
		_T("FindFileSearches"),
		_T("FindWindowRetrieves"),
		_T("GetByte"),
		_T("GetDirRetrieves"),
		_T("GetDisk"),
		_T("GetDiskSpaceReturns"),
		_T("GetEnvVar"),
		_T("GetExtentsRetrieves"),
		_T("GetFileInfo"),
		_T("GetFont"),
		_T("GetGroupNameListRetrieves"),
		_T("GetItemNameList"),
		_T("GetLine"),
		_T("GetMemFreeReturns"),
		_T("GetMode"),
		_T("GetProfIntReturns"),
		_T("GetProfString"),
		_T("GetSystemInfo"),
		_T("GetValidDrivesListReturns"),
		_T("GetWindowHandle"),
		_T("Handler"),
		_T("HIWORDRetrieves"),
		_T("InstallationInfoAllows"),
		_T("IsRetrieves"),
		_T("LaunchApp"),
		_T("LaunchAppAndWaitLaunches"),
		_T("ListAddItem"),
		_T("ListAddString"),
		_T("ListCount"),
		_T("ListCreateCreates"),
		_T("ListCurrentItem"),
		_T("ListCurrentString"),
		_T("ListDeleteItemDeletes"),
		_T("ListDeleteStringDeletes"),
		_T("ListDestroy"),
		_T("ListFindItemAttempts"),
		_T("ListFindStringAttempts"),
		_T("ListGetFirstItemAcquires"),
		_T("ListGetFirstStringRetrieves"),
		_T("ListGetNextItem"),
		_T("ListGetNextString"),
		_T("ListReadFromFileReads"),
		_T("ListSetCurrentItemSets"),
		_T("ListSetCurrentStringSets"),
		_T("ListSetIndexSets"),
		_T("ListWriteToFile"),
		_T("LongPathFromShortPath"),
		_T("LongPathToQuote"),
		_T("LongPathToShortPath"),
		_T("LOWORDRetrieves"),
		_T("MessageBeep"),
		_T("MessageBoxCreates"),
		_T("NumToStrConverts"),
		_T("OpenFileOpens"),
		_T("OpenFileModeSets"),
		_T("ParsePath"),
		_T("PathAdd"),
		_T("PathDeleteDeletes"),
		_T("PathFindFinds"),
		_T("PathGet"),
		_T("PathMoveRearranges"),
		_T("PathSet"),
		_T("PlaceBitmap"),
		_T("PlaceWindow"),
		_T("QueryProgGroupQueries"),
		_T("QueryProgItem"),
		_T("QueryShellMgr"),
		_T("ReadBytes"),
		_T("RebootDialogCreates"),
		_T("RegDBCreateKeyExAllows"),
		_T("RegDBDeleteKeyDeletes"),
		_T("RegDBGetAppInfo"),
		_T("RegDBGetItemGets"),
		_T("RegDBGetKeyValueExRetrieves"),
		_T("RegDBKeyExist"),
		_T("RegDBQueryKey"),
		_T("RegDBSetAppInfo"),
		_T("RegDBSetDefaultRoot"),
		_T("RegDBSetItemSets"),
		_T("RegDBSetKeyValueExSets"),
		_T("ReleaseDialog"),
		_T("ReloadProgGroup"),
		_T("RenameFileRenames"),
		_T("ReplaceFolderIcon"),
		_T("ReplaceProfString"),
		_T("SdAskDestPath"),
		_T("SdAskOptionsCreates"),
		_T("SdAskOptionsListCreates"),
		_T("SdBitmapDisplays"),
		_T("SdComponentDialog"),
		_T("SdComponentDialog2Displays"),
		_T("SdComponentDialogAdvDisplays"),
		_T("SdComponentMult"),
		_T("SdConfirmNewDir"),
		_T("SdConfirmRegistration"),
		_T("SdDisplayTopics"),
		_T("SdFinishDisplays"),
		_T("SdFinishRebootDisplays"),
		_T("SdLicense"),
		_T("SdOptionsButtonsDisplays"),
		_T("SdProductName"),
		_T("SdRegisterUserDisplays"),
		_T("SdRegisterUserExDisplays"),
		_T("SdSelectFolderCreates"),
		_T("SdSetupType"),
		_T("SdShowAnyDialog"),
		_T("SdShowDlgEdit1Displays"),
		_T("SdShowDlgEdit2Displays"),
		_T("SdShowDlgEdit3Displays"),
		_T("SdShowFileModsDisplays"),
		_T("SdShowInfoListDisplays"),
		_T("SdShowMsg"),
		_T("SdStartCopy"),
		_T("SdWelcome"),
		_T("SeekBytes"),
		_T("SelectDir"),
		_T("SelectFolderCreates"),
		_T("SendMessage"),
		_T("SetByte"),
		_T("SetColorSets"),
		_T("SetDialogTitleChanges"),
		_T("SetErrorMsg"),
		_T("SetErrorTitle"),
		_T("SetFileInfo"),
		_T("SetFont"),
		_T("SetStatusWindow"),
		_T("SetTitleDisplays"),
		_T("SetupType"),
		_T("ShowGroup"),
		_T("ShowProgramFolder"),
		_T("SizeWindowSpecifies"),
		_T("Split"),
		_T("Sprintf"),
		_T("SprintfBoxAllows"),
		_T("StatusGetSize"),
		_T("StatusUpdateUpdates"),
		_T("StrCompareCompares"),
		_T("StrFind"),
		_T("StrGetTokensRetrieves"),
		_T("StrLength"),
		_T("StrRemoveLastSlashRemoves"),
		_T("StrSubExtracts"),
		_T("StrToLowerConverts"),
		_T("StrToNumConverts"),
		_T("StrToUpperConverts"),
		_T("StructGet"),
		_T("StructGetPPoints"),
		_T("StructPut"),
		_T("StructPutPPlaces"),
		_T("SystemTerminates"),
		_T("UnUseDLLUnloads"),
		_T("UseDLLOpens"),
		_T("VarRestoreRestores"),
		_T("VarSave"),
		_T("VerCompareCompares"),
		_T("VerFindFileVersionRetrieves"),
		_T("VerGetFileVersion"),
		_T("VerSearchAndUpdateFileSearches"),
		_T("VerUpdateFile"),
		_T("WaitOnDialogDisplays"),
		_T("Welcome"),
		_T("WriteBytesWrites"),
		_T("WriteLine"),
		_T("WriteProfString"),
		_T("XCopyFile"),
*/

#define DEFINE_BLOCK pBuf.DefineBlock

#define COOKIE_COMMENT          0x0001
#define COOKIE_PREPROCESSOR     0x0002
#define COOKIE_EXT_COMMENT      0x0004
#define COOKIE_STRING           0x0008
#define COOKIE_CHAR             0x0010

void TextDefinition::ParseLineIS(TextBlock::Cookie &cookie, LPCTSTR const pszChars, int const nLength, int I, TextBlock::Array &pBuf) const
{
	DWORD &dwCookie = cookie.m_dwCookie;

	if (nLength == 0)
	{
		dwCookie &= COOKIE_EXT_COMMENT;
		return;
	}

	BOOL bFirstChar = (dwCookie & ~COOKIE_EXT_COMMENT) == 0;
	BOOL bRedefineBlock = TRUE;
	BOOL bWasCommentStart = FALSE;
	BOOL bDecIndex = FALSE;
	int nIdentBegin = -1;
	do
	{
		int const nPrevI = I++;
		if (bRedefineBlock)
		{
			int const nPos = bDecIndex ? nPrevI : I;
			bRedefineBlock = FALSE;
			bDecIndex = FALSE;
			if (dwCookie & (COOKIE_COMMENT | COOKIE_EXT_COMMENT))
			{
				DEFINE_BLOCK(nPos, COLORINDEX_COMMENT);
			}
			else if (dwCookie & (COOKIE_CHAR | COOKIE_STRING))
			{
				DEFINE_BLOCK(nPos, COLORINDEX_STRING);
			}
			else if (dwCookie & COOKIE_PREPROCESSOR)
			{
				DEFINE_BLOCK(nPos, COLORINDEX_PREPROCESSOR);
			}
			else if (xisalnum(pszChars[nPos]) || pszChars[nPos] == '.' && nPos > 0 && (!xisalpha(pszChars[nPos - 1]) && !xisalpha(pszChars[nPos + 1])))
			{
				DEFINE_BLOCK(nPos, COLORINDEX_NORMALTEXT);
			}
			else
			{
				DEFINE_BLOCK(nPos, COLORINDEX_OPERATOR);
				bRedefineBlock = TRUE;
				bDecIndex = TRUE;
			}
		}
		// Can be bigger than length if there is binary data
		// See bug #1474782 Crash when comparing SQL with with binary data
		if (I < nLength)
		{
			if (dwCookie & COOKIE_COMMENT)
			{
				DEFINE_BLOCK(I, COLORINDEX_COMMENT);
				dwCookie |= COOKIE_COMMENT;
				break;
			}

			//  String constant "...."
			if (dwCookie & COOKIE_STRING)
			{
				if (pszChars[I] == '"')
				{
					dwCookie &= ~COOKIE_STRING;
					bRedefineBlock = TRUE;
					int nPrevI = I;
					while (nPrevI && pszChars[--nPrevI] == '\\')
					{
						dwCookie ^= COOKIE_STRING;
						bRedefineBlock ^= TRUE;
					}
				}
				continue;
			}

			//  Char constant '..'
			if (dwCookie & COOKIE_CHAR)
			{
				if (pszChars[I] == '\'')
				{
					dwCookie &= ~COOKIE_CHAR;
					bRedefineBlock = TRUE;
					int nPrevI = I;
					while (nPrevI && pszChars[--nPrevI] == '\\')
					{
						dwCookie ^= COOKIE_CHAR;
						bRedefineBlock ^= TRUE;
					}
				}
				continue;
			}

			//  Extended comment /*....*/
			if (dwCookie & COOKIE_EXT_COMMENT)
			{
				// if (I > 0 && pszChars[I] == '/' && pszChars[nPrevI] == '*')
				if ((I > 1 && pszChars[I] == '/' && pszChars[nPrevI] == '*' /*&& pszChars[nPrevI - 1] != '/'*/ && !bWasCommentStart) || (I == 1 && pszChars[I] == '/' && pszChars[nPrevI] == '*'))
				{
					dwCookie &= ~COOKIE_EXT_COMMENT;
					bRedefineBlock = TRUE;
				}
				bWasCommentStart = FALSE;
				continue;
			}

			if (I > 0 && pszChars[I] == '/' && pszChars[nPrevI] == '/')
			{
				DEFINE_BLOCK(nPrevI, COLORINDEX_COMMENT);
				dwCookie |= COOKIE_COMMENT;
				break;
			}

			//  Preprocessor directive #....
			if (dwCookie & COOKIE_PREPROCESSOR)
			{
				if (I > 0 && pszChars[I] == '*' && pszChars[nPrevI] == '/')
				{
					DEFINE_BLOCK(nPrevI, COLORINDEX_COMMENT);
					dwCookie |= COOKIE_EXT_COMMENT;
				}
				continue;
			}

			//  Normal text
			if (pszChars[I] == '"')
			{
				DEFINE_BLOCK(I, COLORINDEX_STRING);
				dwCookie |= COOKIE_STRING;
				continue;
			}
			if (pszChars[I] == '\'')
			{
				// if (I + 1 < nLength && pszChars[I + 1] == '\'' || I + 2 < nLength && pszChars[I + 1] != '\\' && pszChars[I + 2] == '\'' || I + 3 < nLength && pszChars[I + 1] == '\\' && pszChars[I + 3] == '\'')
				if (!I || !xisalnum(pszChars[nPrevI]))
				{
					DEFINE_BLOCK(I, COLORINDEX_STRING);
					dwCookie |= COOKIE_CHAR;
					continue;
				}
			}
			if (I > 0 && pszChars[I] == '*' && pszChars[nPrevI] == '/')
			{
				DEFINE_BLOCK(nPrevI, COLORINDEX_COMMENT);
				dwCookie |= COOKIE_EXT_COMMENT;
				bWasCommentStart = TRUE;
				continue;
			}

			bWasCommentStart = FALSE;

			if (bFirstChar)
			{
				if (pszChars[I] == '#')
				{
					DEFINE_BLOCK(I, COLORINDEX_PREPROCESSOR);
					dwCookie |= COOKIE_PREPROCESSOR;
					continue;
				}
				bFirstChar = xisspace(pszChars[I]);
			}

			if (pBuf == NULL)
				continue; // No need to extract keywords, so skip rest of loop

			if (xisalnum(pszChars[I]) || pszChars[I] == '.' && I > 0 && (!xisalpha(pszChars[nPrevI]) && !xisalpha(pszChars[I + 1])))
			{
				if (nIdentBegin == -1)
					nIdentBegin = I;
				continue;
			}
		}
		if (nIdentBegin >= 0)
		{
			if (IsISKeyword(pszChars + nIdentBegin, I - nIdentBegin))
			{
				DEFINE_BLOCK(nIdentBegin, COLORINDEX_KEYWORD);
			}
			else if (IsUser1Keyword(pszChars + nIdentBegin, I - nIdentBegin))
			{
				DEFINE_BLOCK(nIdentBegin, COLORINDEX_USER1);
			}
			else if (IsUser2Keyword(pszChars + nIdentBegin, I - nIdentBegin))
			{
				DEFINE_BLOCK(nIdentBegin, COLORINDEX_USER2);
			}
			else if (IsNumeric(pszChars + nIdentBegin, I - nIdentBegin))
			{
				DEFINE_BLOCK(nIdentBegin, COLORINDEX_NUMBER);
			}
			else
			{
				for (int j = I; j < nLength; j++)
				{
					if (!xisspace(pszChars[j]))
					{
						if (pszChars[j] == '(')
						{
							DEFINE_BLOCK(nIdentBegin, COLORINDEX_FUNCNAME);
						}
						break;
					}
				}
			}
			bRedefineBlock = TRUE;
			bDecIndex = TRUE;
			nIdentBegin = -1;
		}
	} while (I < nLength);

	if (pszChars[nLength - 1] != '\\')
		dwCookie &= COOKIE_EXT_COMMENT;
}

TESTCASE
{
	int count = 0;
	BOOL (*pfnIsKeyword)(LPCTSTR, int) = NULL;
	FILE *file = fopen(__FILE__, "r");
	assert(file);
	TCHAR text[1024];
	while (_fgetts(text, _countof(text), file))
	{
		TCHAR c, *p, *q;
		if (pfnIsKeyword && (p = _tcschr(text, '"')) != NULL && (q = _tcschr(++p, '"')) != NULL)
			assert(pfnIsKeyword(p, static_cast<int>(q - p)));
		else if (_stscanf(text, _T(" static BOOL IsISKeyword %c"), &c) == 1 && c == '(')
			pfnIsKeyword = IsISKeyword;
		else if (_stscanf(text, _T(" static BOOL IsUser1Keyword %c"), &c) == 1 && c == '(')
			pfnIsKeyword = IsUser1Keyword;
		else if (_stscanf(text, _T(" static BOOL IsUser2Keyword %c"), &c) == 1 && c == '(')
			pfnIsKeyword = IsUser2Keyword;
		else if (pfnIsKeyword && _stscanf(text, _T(" } %c"), &c) == 1 && (c == ';' ? ++count : 0))
			pfnIsKeyword = NULL;
	}
	fclose(file);
	assert(count == 3);
	return count;
}
