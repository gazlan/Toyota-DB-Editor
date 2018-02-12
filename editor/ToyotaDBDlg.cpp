#include "stdafx.h"

#include "..\shared\xlat_tables.h"
#include "..\shared\xlat.h"
#include "..\shared\text.h"
#include "..\shared\file.h"
#include "..\shared\file_walker.h"
#include "..\shared\mmf.h"
#include "..\shared\timestamp.h"
#include "..\shared\vector.h"
#include "..\shared\vector_sorted.h"
#include "..\shared\db_dbx.h"
#include "..\shared\tray.h"

#include "ToyotaDB.h"
#include "ToyotaDBDlg.h"
#include "twins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_ICON_NOTIFY     (WM_APP + 10)

static DWORD               _dwGranulation = 3; // 2 Power: 0, 2, 3, 4

static BYTE                _Dummy[MAX_PATH];

/* ******************************************************************** **
** @@ Error()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void Error()
{
   char*    pBuf = NULL;

   FormatMessage
   (
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,   // Source and Processing Options
      NULL,                                                          // Pointer to Message Source
      GetLastError(),                                                // Requested Message Identifier
      MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),                      // Language Identifier for Requested Message
      (LPTSTR)&pBuf,                                                 // Pointer to Message Buffer
      0,                                                             // Maximum Size of Message Buffer
      NULL                                                           // Address of Array of Message Inserts
   );

   MessageBox(GetActiveWindow(),pBuf,"Error",MB_OK | MB_ICONSTOP);

   LocalFree(pBuf);
}

/* ******************************************************************** **
** @@ CMP_Index()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

inline int CMP_Index(const void** const pKey1,const void** const pKey2)
{
   DBV_RECORD_EN*    p1 = *(DBV_RECORD_EN**)pKey1;
   DBV_RECORD_EN*    p2 = *(DBV_RECORD_EN**)pKey2;

   if (p1->_dwIndex > p2->_dwIndex)
   {
      return 1;
   }
   else if (p1->_dwIndex < p2->_dwIndex)
   {
      return -1;
   }
   else
   {
      return 0;
   }
}

/* ******************************************************************** **
** @@ CMP_Link()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

int __cdecl CMP_Link(const void* pKey1,const void* pKey2)
{
   DBF_REC_RU*    p1 = (DBF_REC_RU*)pKey1;
   DBF_REC_RU*    p2 = (DBF_REC_RU*)pKey2;

   if (p1->_dwLinkFrom > p2->_dwLinkFrom)
   {
      return 1;
   }
   else if (p1->_dwLinkFrom < p2->_dwLinkFrom)
   {
      return -1;
   }
   else
   {
      return 0;
   }
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::LoadDix_Technical()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::LoadDix_Technical()
{
   CWaitCursor       Waiter;

   if (!_pDBX)
   {
      // Error !
      ASSERT(0);
      return;
   }

   _pTechnical = _pDBX->OpenTable
   (
      DIX_ER_TECHNICAL,
      DIX_ER_TECHNICAL_DBF,
      DIX_ER_TECHNICAL_DBV,
      DBX_OM_READ_ONLY,
      DBX_OM_READ_ONLY,
      DBX_FM_FLUSH_NEVER,
      DBX_FM_FLUSH_NEVER
   );

   if (!_pTechnical)
   {
      // Error !
      ASSERT(0);
      return;
   }

   DWORD    dwCnt = _pTechnical->GetRecCnt();

   m_List.SetItemCount(dwCnt);
   m_List.UpdateWindow();
}

/* ******************************************************************** **
** @@ Creator()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */
/*
static void Creator(const DBX_TABLE_INFO& rInfo)
{
   DBX*     pDBF = new DBX;

   if (!pDBF->CreateEmptyTable(&rInfo))
   {
      // Error !
      return;
   }

   DBX_TABLE*     pTable = pDBF->OpenTable((char*)&rInfo._pszName,DBX_OM_READ_WRITE);
   
   if (!pTable)
   {
      // Error !
      ASSERT(0);
      return;
   }

   pDBF->CloseTable(pTable);

   delete pDBF;
   pDBF = NULL;
}
*/
/* ******************************************************************** **
** @@ CToyotaDBDlg::CToyotaDBDlg()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

CToyotaDBDlg::CToyotaDBDlg(CWnd* pParent /*=NULL*/)
:  CDialog(CToyotaDBDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CToyotaDBDlg)
   m_Index = _T("");
   m_Translated = FALSE;
   m_Verified = FALSE;
   m_Marked = FALSE;
   _sComment = _T("");
   _sGo = _T("");
   _sSrc = _T("");
   _sDst = _T("");
   m_Translation = _T("");
   m_Block = _T("");
   //}}AFX_DATA_INIT
   // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
   m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

   _pTray = NULL;
   _pDBX  = NULL;

   _pTechnical = NULL;

   _pTableEN     = NULL;
   _pTableRU     = NULL;
   _pTableLink   = NULL;
   _pTableMapper = NULL;


   _sTableEN    = _T("");
   _sTableFixEN = _T("");
   _sTableVarEN = _T("");

   _sTableRU        = _T("");
   _sTableFixRU     = _T("");
   _sTableLink      = _T("");
   _sTableLinkFix   = _T("");
   _sTableLinkVar   = _T("");
   _sTableMapper    = _T("");
   _sTableMapperFix = _T("");
   _sOriginal       = _T("");

   LOGFONT     LF;

   memset(&LF,0,sizeof(LOGFONT));            // zero out structure

   LF.lfHeight = 18;                         

   strcpy(LF.lfFaceName,"Arial Bold");       // request a face name "Arial"

   VERIFY(_FontEN.CreateFontIndirect(&LF));  // create the font

   _Table = DT_M_ENGLISH_10;

   _iCurRec   = 1;
   _iRecTotal = 1;
   _iPageSize  = 1;

   _bForceLoad = false;

   _bExit = false;
}

void CToyotaDBDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CToyotaDBDlg)
   DDX_Control(pDX, IDC_EDT_ORIGINAL, m_Original);
   DDX_Control(pDX, IDC_LIST_DIX, m_List);
   DDX_Control(pDX, IDC_CMB_TABLE, m_CMB_Table);
   DDX_Text(pDX, IDC_EDT_IDX, m_Index);
   DDV_MaxChars(pDX, m_Index, 255);
   DDX_Check(pDX, IDC_CHK_TRANSLATED, m_Translated);
   DDX_Check(pDX, IDC_CHK_VERIFIED, m_Verified);
   DDX_Check(pDX, IDC_CHK_MARK, m_Marked);
   DDX_Text(pDX, IDC_EDT_COMMENT, _sComment);
   DDV_MaxChars(pDX, _sComment, 255);
   DDX_Text(pDX, IDC_EDT_GO, _sGo);
   DDV_MaxChars(pDX, _sGo, 255);
   DDX_Text(pDX, IDC_REDT_SRC, _sSrc);
   DDV_MaxChars(pDX, _sSrc, 65535);
   DDX_Text(pDX, IDC_REDT_DST, _sDst);
   DDV_MaxChars(pDX, _sDst, 65535);
   DDX_Text(pDX, IDC_REDT_TRANSLATION, m_Translation);
   DDV_MaxChars(pDX, m_Translation, 65535);
   DDX_Text(pDX, IDC_EDT_BLOCK, m_Block);
   DDV_MaxChars(pDX, m_Block, 255);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CToyotaDBDlg, CDialog)
   //{{AFX_MSG_MAP(CToyotaDBDlg)
   ON_WM_SYSCOMMAND()
   ON_WM_PAINT()
   ON_WM_QUERYDRAGICON()
   ON_BN_CLICKED(IDC_CHK_TRANSLATED, OnChkTranslated)
   ON_BN_CLICKED(IDC_CHK_VERIFIED, OnChkVerified)
   ON_BN_CLICKED(IDC_CHK_MARK, OnChkMark)
   ON_BN_CLICKED(IDC_BTN_FIRST, OnBtnFirst)
   ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
   ON_BN_CLICKED(IDC_BTN_PREV, OnBtnPrev)
   ON_BN_CLICKED(IDC_BTN_NEXT, OnBtnNext)
   ON_BN_CLICKED(IDC_BTN_DN, OnBtnDn)
   ON_BN_CLICKED(IDC_BTN_LAST, OnBtnLast)
   ON_BN_CLICKED(IDC_BTN_GO, OnBtnGo)
   ON_WM_DESTROY()
   ON_CBN_CLOSEUP(IDC_CMB_TABLE, OnCloseupCmbTable)
   ON_BN_CLICKED(IDC_BTN_UPDATE, OnBtnUpdate)
   ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
   ON_BN_CLICKED(IDC_BTN_SOME_PREV, OnBtnSomePrev)
   ON_BN_CLICKED(IDC_BTN_SOME_NEXT, OnBtnSomeNext)
   ON_BN_CLICKED(IDC_BTN_UPDATE_ALL, OnBtnUpdateAll)
   ON_NOTIFY(NM_CLICK, IDC_REDT_TRANSLATION, OnClickRedtTranslation)
   ON_EN_CHANGE(IDC_EDT_ORIGINAL, OnChangeEdtOriginal)
   ON_NOTIFY(NM_CLICK, IDC_LIST_DIX, OnClickListDix)
   ON_NOTIFY(NM_DBLCLK, IDC_LIST_DIX, OnDblclkListDix)
   ON_NOTIFY(NM_RCLICK, IDC_LIST_DIX, OnRclickListDix)
   ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DIX, OnGetdispinfoListDix)
   ON_EN_CHANGE(IDC_EDT_COMMENT, OnChangeEdtComment)
   ON_BN_CLICKED(IDC_BTN_COMPACT, OnBtnCompact)
   ON_BN_CLICKED(IDC_BTN_TWINS, OnBtnTwins)
   ON_MESSAGE(WM_ICON_NOTIFY,OnTrayNotification)
   ON_WM_CLOSE()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnInitDialog()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

BOOL CToyotaDBDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   // Set the icon for this dialog.  The framework does this automatically
   //  when the application's main window is not a dialog
   SetIcon(m_hIcon, TRUE);       // Set big icon
   SetIcon(m_hIcon, FALSE);      // Set small icon
   
   CenterWindow();

   CMenu*   pSysMenu = GetSystemMenu(FALSE);

   if (pSysMenu)
   {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING,IDM_EXIT,(LPCTSTR)"E&xit\tX");
   }

   _pTray = new CSystemTray;

   if (_pTray)
   {
      _pTray->Create(this,WM_ICON_NOTIFY,"ToyotaDB Editor",m_hIcon,IDR_POPUP_MENU,FALSE);

      _pTray->MaximiseFromTray(this);
   }

   _pDBX = new DBX;

   // Virtual List !
   m_List.InsertColumn( 0,"English - Russian",LVCFMT_LEFT,200,0);

   DWORD    dwStyle = m_List.GetExtendedStyle();

   dwStyle |= LVS_OWNERDATA | LVS_EX_FULLROWSELECT | LVS_EX_FLATSB | LVS_EX_GRIDLINES;

   m_List.SetExtendedStyle(dwStyle);

   m_List.SetItemCount(0);
   m_List.UpdateWindow();

   LoadDix_Technical();

// ------------------------------------------------------------------------------------
   // TODO: If this is a RICHEDIT control, the control will not
   // send this notification unless you override the CDialog::OnInitDialog()
   // function and call CRichEditCtrl().SetEventMask()
   // with the ENM_CHANGE flag ORed into the mask.
// ------------------------------------------------------------------------------------

   GetDlgItem(IDC_REDT_SRC)->SetFont(&_FontEN,FALSE);
   GetDlgItem(IDC_REDT_DST)->SetFont(&_FontEN,FALSE);

   m_CMB_Table.SetCurSel(_Table);
   
   _bForceLoad = true;  // Initial Loading

   OnCloseupCmbTable();

   return TRUE;  // return TRUE  unless you set the focus to a control
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnPaint()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CToyotaDBDlg::OnPaint() 
{
   if (IsIconic())
   {
      CPaintDC dc(this); // device context for painting

      SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

      // Center icon in client rectangle
      int cxIcon = GetSystemMetrics(SM_CXICON);
      int cyIcon = GetSystemMetrics(SM_CYICON);
      CRect rect;
      GetClientRect(&rect);
      int x = (rect.Width() - cxIcon + 1) / 2;
      int y = (rect.Height() - cyIcon + 1) / 2;

      // Draw the icon
      dc.DrawIcon(x, y, m_hIcon);
   }
   else
   {
      CDialog::OnPaint();
   }
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnQueryDragIcon()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CToyotaDBDlg::OnQueryDragIcon()
{
   return (HCURSOR) m_hIcon;
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnSysCommand()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnSysCommand(UINT nID,LPARAM lParam)
{
   if (nID == SC_CLOSE)
   {
      if (_pTray)
      {
         _pTray->MinimiseToTray(this);
      }
   }
   else if (nID == IDM_EXIT)
   {
      _bExit = true;
      PostMessage(WM_CLOSE);
   }
   else
   {
      CDialog::OnSysCommand(nID,lParam);
   }
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::Load()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::Load()
{
   CWaitCursor       Waiter;

   if (!_pDBX)
   {
      // Error !
      ASSERT(0);
      return;
   }

   if (_pTableEN)
   {
      // Delete if exist
      _pDBX->CloseTable(_pTableEN);
   }

   _pTableEN = _pDBX->OpenTable
   (
      _sTableEN,
      _sTableFixEN,
      _sTableVarEN,
      DBX_OM_READ_ONLY,
      DBX_OM_READ_ONLY,
      DBX_FM_FLUSH_NEVER,
      DBX_FM_FLUSH_NEVER
   );

   if (!_pTableEN)
   {
      // Error !
      ASSERT(0);
      return;
   }

   if (_pTableRU)
   {
      // Delete if exist
      _pDBX->CloseTable(_pTableRU);
   }

   _pTableRU = _pDBX->OpenTable
   (
      _sTableRU,
      _sTableFixRU,
      NULL,
      DBX_OM_IN_PLACE,
      DBX_OM_NONE,
      DBX_FM_FLUSH_ON_CLOSE,
      DBX_FM_FLUSH_NEVER
   );

   if (!_pTableRU)
   {
      // Error !
      ASSERT(0);
      _pDBX->CloseTable(_pTableEN);
      return;
   }

   if (_pTableLink)
   {
      // Delete if exist
      _pDBX->CloseTable(_pTableLink);
   }

   _pTableLink = _pDBX->OpenTable
   (
      _sTableLink,
      _sTableLinkFix,
      _sTableLinkVar,
      DBX_OM_IN_PLACE,
      DBX_OM_READ_WRITE,
      DBX_FM_FLUSH_ON_CLOSE,
      DBX_FM_FLUSH_ON_CLOSE
   );

   if (!_pTableLink)
   {
      // Error !
      ASSERT(0);
      _pDBX->CloseTable(_pTableEN);
      _pDBX->CloseTable(_pTableRU);
      return;
   }

   if (_pTableMapper)
   {
      // Delete if exist
      _pDBX->CloseTable(_pTableMapper);
   }

   _pTableMapper = _pDBX->OpenTable
   (
      _sTableMapper,
      _sTableMapperFix,
      NULL,
      DBX_OM_IN_PLACE,
      DBX_OM_NONE,
      DBX_FM_FLUSH_ON_CLOSE,
      DBX_FM_FLUSH_ON_CLOSE
   );

   if (!_pTableMapper)
   {
      // Error !
      ASSERT(0);
      _pDBX->CloseTable(_pTableEN);
      _pDBX->CloseTable(_pTableRU);
      _pDBX->CloseTable(_pTableLink);
      return;
   }
   
   _iRecTotal = (int)_pTableLink->GetRecCnt();
}

/* ******************************************************************** **
** @@ Cleanup()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::Cleanup()
{
   if (_pTray)
   {
      delete _pTray;
      _pTray = NULL;
   }

   _FontEN.DeleteObject(); 

   if (_pDBX)
   {
      delete _pDBX;
      _pDBX = NULL;
   }
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnChkTranslated()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnChkTranslated() 
{
   SaveFlags();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnChkVerified()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnChkVerified() 
{
   SaveFlags();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnChkMark()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnChkMark() 
{
   SaveFlags();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnBtnFirst()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnBtnFirst() 
{
   CheckModifications();

   _iCurRec = 1;

   Locate();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnBtnUp()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnBtnUp() 
{
   CheckModifications();

   if (_iPageSize)
   {
      _iCurRec -= _iRecTotal / _iPageSize;
   }
   else
   {
      --_iCurRec;
   }

   if (_iCurRec < 1)
   {
      _iCurRec = 1;
      MessageBeep(MB_ICONSTOP);
   }

   Locate();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnBtnPrev()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnBtnPrev() 
{
   CheckModifications();

   --_iCurRec;

   if (_iCurRec < 1)
   {
      _iCurRec = 1;
      MessageBeep(MB_ICONSTOP);
   }

   Locate();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnBtnNext()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnBtnNext() 
{
   CheckModifications();

   ++_iCurRec;

   if (_iCurRec > _iRecTotal)
   {
      _iCurRec = _iRecTotal;
      MessageBeep(MB_ICONSTOP);
   }

   Locate();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnBtnDn()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnBtnDn() 
{
   CheckModifications();

   if (_iPageSize)
   {
      _iCurRec += _iRecTotal / _iPageSize;
   }
   else
   {
      ++_iCurRec;
   }

   if (_iCurRec > _iRecTotal)
   {
      _iCurRec = _iRecTotal;
      MessageBeep(MB_ICONSTOP);
   }

   Locate();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnBtnLast()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnBtnLast() 
{
   CheckModifications();

   _iCurRec = _iRecTotal;

   Locate();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnBtnGo()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnBtnGo() 
{
   UpdateData(TRUE);

   int   iNewRec = atoi((LPCTSTR)_sGo);  

   if (iNewRec == _iCurRec)
   {
      MessageBeep(MB_ICONSTOP);
      return;
   }

   CheckModifications();

   _iCurRec = iNewRec;

   if (_iCurRec < 1)
   {
      _iCurRec = 1;
      MessageBeep(MB_ICONSTOP);
   }

   if (_iCurRec > _iRecTotal)
   {
      _iCurRec = _iRecTotal;
      MessageBeep(MB_ICONSTOP);
   }

   _sGo.Format("%d",_iCurRec);

   Locate();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnCancel()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnCancel() 
{
   CheckModifications();
   
   CDialog::OnCancel();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnDestroy()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnDestroy() 
{
   CDialog::OnDestroy();
   
   CheckModifications();

   if (_pTray)
   {
      _pTray->RemoveIcon();  
   }

   Cleanup();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnCloseupCmbTable()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnCloseupCmbTable() 
{
   DDB_TABLE   KeepTable = _Table;

   if (_bForceLoad)
   {
      KeepTable = DT_NO_TABLE;

      _bForceLoad = false;
   }

   _Table = (DDB_TABLE)m_CMB_Table.GetCurSel();

   if (_Table == KeepTable)
   {
      // Nothing to do !
      return;
   }

   switch (KeepTable)
   {
      case DT_M_ENGLISH_10:
      {
         _sTableEN = FILE_10_M_EN;
         _sTableRU = FILE_10_M_RU;
         break;
      }
      case DT_U_ENGLISH_10:
      {
         _sTableEN = FILE_10_U_EN;
         _sTableRU = FILE_10_U_RU;
         break;
      }
      case DT_V_ENGLISH_10:
      {
         _sTableEN = FILE_10_V_EN;
         _sTableRU = FILE_10_V_RU;
         break;
      }
      default:
      {
         break;
      }
   }

   if (_pDBX && (KeepTable != DT_NO_TABLE))
   {
      _pDBX->CloseTable(_sTableEN);
      _pDBX->CloseTable(_sTableRU);
   }

   switch (_Table)
   {
      case DT_M_ENGLISH_10:
      {
         _sTableEN        = FILE_10_M_EN;
         _sTableFixEN     = FILE_DBF_10_M_EN;
         _sTableVarEN     = FILE_DBV_10_M_EN;
         _sTableRU        = FILE_10_M_RU;
         _sTableFixRU     = FILE_DBF_10_M_RU;
         _sTableLink      = FILE_10_M_LINK;
         _sTableLinkFix   = FILE_DBF_10_M_LINK;
         _sTableLinkVar   = FILE_DBV_10_M_LINK;
         _sTableMapper    = FILE_10_M_MAPPER;
         _sTableMapperFix = FILE_DBF_10_M_MAPPER;
         break;
      }
      case DT_U_ENGLISH_10:
      {
         _sTableEN        = FILE_10_U_EN;
         _sTableFixEN     = FILE_DBF_10_U_EN;
         _sTableVarEN     = FILE_DBV_10_U_EN;
         _sTableRU        = FILE_10_U_RU;
         _sTableFixRU     = FILE_DBF_10_U_RU;
         _sTableLink      = FILE_10_U_LINK;
         _sTableLinkFix   = FILE_DBF_10_U_LINK;
         _sTableLinkVar   = FILE_DBV_10_U_LINK;
         _sTableMapper    = FILE_10_U_MAPPER;
         _sTableMapperFix = FILE_DBF_10_U_MAPPER;
         break;
      }
      case DT_V_ENGLISH_10:
      {
         _sTableEN        = FILE_10_V_EN;
         _sTableFixEN     = FILE_DBF_10_V_EN;
         _sTableVarEN     = FILE_DBV_10_V_EN;
         _sTableRU        = FILE_10_V_RU;
         _sTableFixRU     = FILE_DBF_10_V_RU;
         _sTableLink      = FILE_10_V_LINK;
         _sTableLinkFix   = FILE_DBF_10_V_LINK;
         _sTableLinkVar   = FILE_DBV_10_V_LINK;
         _sTableMapper    = FILE_10_V_MAPPER;
         _sTableMapperFix = FILE_DBF_10_V_MAPPER;
         break;
      }
      default:
      {
         // Error !
         ASSERT(0);
         return;
      }
   }
   
   Load();

   _iCurRec = DB_RetrieveCurRec();

   _iPageSize = (_iRecTotal + 255)/ 256; // 256 blocks max !
   _iPageSize = max(_iPageSize,1);

   if (_iRecTotal)
   {
      m_Index.Format("%d / %d",_iCurRec,_iRecTotal);
   }
   else
   {
      m_Index = _T("< empty >");
   }

   Locate();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnBtnUpdate()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnBtnUpdate() 
{
   SaveComment();
   SaveText();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::CheckModifications()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::CheckModifications()
{
   UpdateData(TRUE);

   CString     sTxt  = _T("");
   CString     sCmnt = _T("");

   GetTextColumn(_pTableLink,_iCurRec,"TEXT",   sTxt);
   GetTextColumn(_pTableLink,_iCurRec,"COMMENT",sCmnt);

   bool  bNewText    = (_sDst     != sTxt);
   bool  bNewComment = (_sComment != sCmnt);
   
   CString     sMessge = _T("");

   if (bNewComment && !bNewText)
   {
      sMessge.Format("Комментарий к записи был изменен.\nСохранить изменения?\n\nПри ответе 'No', все измения будут утеряны.");
   }
   else if (!bNewComment && bNewText)
   {
      sMessge.Format("Перевод текста был изменен.\nСохранить изменения?\n\nПри ответе 'No', все измения будут утеряны.");
   }
   else if (bNewComment && bNewText)
   {
      sMessge.Format("Комментарий к записи и перевод текста были изменены.\nСохранить изменения?\n\nПри ответе 'No', все измения будут утеряны.");
   }
   else
   {
      // No valuable changes !
      return;
   }

   if (MessageBox(sMessge,"Last chance!",MB_ICONSTOP | MB_YESNO) == IDYES)
   {
      SaveComment();
      SaveText();
   }
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::Locate()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::Locate()
{
   // Link
   DBX_COLUMN*    pRecNum    = _pTableLink->GetColumn("RECORD NUM");
   DBX_COLUMN*    pIndexFrom = _pTableLink->GetColumn("INDEX FROM");
   DBX_COLUMN*    pLinkFrom  = _pTableLink->GetColumn("LINK FROM");
   DBX_COLUMN*    pLinkTo    = _pTableLink->GetColumn("LINK TO");
   DBX_COLUMN*    pLinkCnt   = _pTableLink->GetColumn("LINK CNT");
   DBX_COLUMN*    pText      = _pTableLink->GetColumn("TEXT");
   DBX_COLUMN*    pComment   = _pTableLink->GetColumn("COMMENT");
//   DBX_COLUMN*    pTimestamp = _pTableLink->GetColumn("TIMESTAMP");
   DBX_COLUMN*    pBlock     = _pTableLink->GetColumn("BLOCK");
   DBX_COLUMN*    pFlags     = _pTableLink->GetColumn("FLAGS");

   const BYTE* const    pRecordDst = _pTableLink->Go(_iCurRec);

   ASSERT(pRecordDst);

   ASSERT(pRecNum);
   ASSERT(pIndexFrom);
   ASSERT(pLinkFrom);
   ASSERT(pLinkTo);
   ASSERT(pLinkCnt);
   ASSERT(pText);
   ASSERT(pComment);
//   ASSERT(pTimestamp);
   ASSERT(pBlock);
   ASSERT(pFlags);

   DWORD    dwRecNum    = *(DWORD*)pRecNum->   Get(pRecordDst);
   DWORD    dwIndexFrom = *(DWORD*)pIndexFrom->Get(pRecordDst);
   DWORD    dwLinkFrom  = *(DWORD*)pLinkFrom-> Get(pRecordDst);
   DWORD    dwLinkTo    = *(DWORD*)pLinkTo->   Get(pRecordDst);
   DWORD    dwLinkCnt   = *(DWORD*)pLinkCnt->  Get(pRecordDst);
   DWORD    dwText      = *(DWORD*)pText->     Get(pRecordDst);
   DWORD    dwComment   = *(DWORD*)pComment->  Get(pRecordDst);
//   DWORD    dwTimestamp = *(DWORD*)pTimestamp->  Get(pRecordDst);
   DWORD    dwBlock     = *(DWORD*)pBlock->  Get(pRecordDst);

   BYTE     byFlags = *(BYTE*)pFlags->Get(pRecordDst);

   ASSERT(dwLinkFrom);
   ASSERT(dwLinkTo);

   ASSERT(dwText);
   ASSERT(!(dwText & 0x07));

   m_Block.Format("%d",dwBlock + 1);
   m_Index.Format("%d / %d",_iCurRec,_iRecTotal);

   _sGo.Format("%d",_iCurRec);

   _pTableLink->SetCurRec(_iCurRec);

   // Russian
   GetTextColumn(_pTableLink,dwLinkTo,"TEXT",   _sDst);
   GetTextColumn(_pTableLink,dwLinkTo,"COMMENT",_sComment);
   
   // Flags
   m_Translated = (byFlags & DBV_FLAG_TRANLATED)  ?  BST_CHECKED  :  BST_UNCHECKED;
   m_Verified   = (byFlags & DBV_FLAG_VERIFIED)   ?  BST_CHECKED  :  BST_UNCHECKED;
   m_Marked     = (byFlags & DBV_FLAG_MARKED)     ?  BST_CHECKED  :  BST_UNCHECKED;

   // English
   DBX_COLUMN*    pTextSrc = _pTableEN->GetColumn("TEXT");

   ASSERT(pTextSrc);

   const BYTE* const    pRecordSrc = _pTableEN->Go(dwRecNum);

   ASSERT(pRecordSrc);

   DWORD    dwTextSrc = *(DWORD*)pTextSrc-> Get(pRecordSrc);

   ASSERT(dwTextSrc);
   ASSERT(!(dwTextSrc & 0x07));

   ASSERT(_pTableEN->GetMemBufVar());
   
   _sSrc = _pTableEN->GetMemBufVar() + dwTextSrc + sizeof(DWORD);

   UpdateData(FALSE);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::FindIndexByLink()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD CToyotaDBDlg::FindIndexByLink(DWORD dwLink)
{
   if (!_pTableRU)
   {
      // Error !
      return 0;
   }

   DBF_REC_RU     SrchRec;

   memset(&SrchRec,0,sizeof(DBF_REC_RU));

   SrchRec._dwLinkFrom = dwLink;

   DWORD    dwStart = _pTableRU->_FixHeader._wDataStart;
   DWORD    dwCount = _pTableRU->_FixHeader._dwRecCnt;

   BYTE*    pBuf = _pTableRU->_pMemBufFix + dwStart;

   // bsearch returns a pointer to an occurrence of key in the array pointed to by base. 
   // If key is not found, the function returns NULL. If the array is not in ascending sort order 
   // or contains duplicate records with identical keys, the result is unpredictable.
   DBF_REC_RU*    pFound = (DBF_REC_RU*)bsearch(&SrchRec,pBuf,dwCount,sizeof(DBF_REC_RU),CMP_Link);

   if (pFound)
   {
      return pFound->_dwIndexFrom;
   }

/*
   DBX_COLUMN*    pTextSrc = _pTableEN->GetColumn("TEXT");

   ASSERT(pTextSrc);

   const BYTE* const    pRecordSrc = _pTableEN->Go(_iCurRec);

   ASSERT(pRecordSrc);

   DWORD    dwTextSrc = *(DWORD*)pTextSrc-> Get(pRecordSrc);
*/
   // Error !
   ASSERT(0);
   return 0;
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::SaveFlags()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool CToyotaDBDlg::SaveFlags()
{
   CWaitCursor       Waiter;

   if (!_pTableLink)
   {
      // Error !
      ASSERT(0);
      return false;
   }

   const BYTE* const    pRecord = _pTableLink->Go(_iCurRec);

   if (!pRecord)
   {
      // Error !
      ASSERT(0);
      return  false;
   }

   DBX_COLUMN*    pFlags     = _pTableLink->GetColumn("FLAGS");
   DBX_COLUMN*    pTimestamp = _pTableLink->GetColumn("TIMESTAMP");

   ASSERT(pFlags);
   ASSERT(pTimestamp);

   BYTE     byFlags = *(BYTE*)pFlags->Get(pRecord);

   BYTE     byKeepFlags = byFlags;

   UpdateData(TRUE);

   if (m_Translated == TRUE)
   {
      byFlags |= DBV_FLAG_TRANLATED;
   }
   else
   {
      byFlags &= ~DBV_FLAG_TRANLATED;
   } 

   if (m_Verified == TRUE)
   {
      byFlags |= DBV_FLAG_VERIFIED;
   }
   else
   {
      byFlags &= ~DBV_FLAG_VERIFIED;
   }

   if (m_Marked == TRUE)
   {
      byFlags |= DBV_FLAG_MARKED;
   }
   else
   {
      byFlags &= ~DBV_FLAG_MARKED;
   }

   if (byKeepFlags == byFlags)
   {
      // No changes !
      return true;
   }

   DWORD    dwFlags = byFlags;

   BYTE     _Record[MAX_PATH];

   memset(&_Record,0x20,MAX_PATH);
   memcpy(&_Record,pRecord,_pTableLink->_FixHeader._wRecSize);

   FILETIME    TimeNow;

   GetSystemTimeAsFileTime(&TimeNow);

   WORD        wFatDate = 0;
   WORD        wFatTime = 0;

   FileTimeToDosDateTime(&TimeNow,&wFatDate,&wFatTime);

   DWORD    dwTimestamp = (wFatDate << 16) | wFatTime;

   pFlags->    Set(&_Record,&dwFlags);
   pTimestamp->Set(&_Record,&dwTimestamp);

   return _pTableLink->UpdateRecord(_iCurRec,(BYTE*)&_Record);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::SaveComment()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool CToyotaDBDlg::SaveComment()
{
   CWaitCursor       Waiter;

   UpdateData(TRUE);

   CString     sOldComment = _T("");

   GetTextColumn(_pTableLink,_iCurRec,"COMMENT",sOldComment);
   
   if (_sComment == sOldComment)
   {
      // Is NOT changed !
      return true;
   }

   int   iLen = _sComment.IsEmpty()  ?  0  :  _sComment.GetLength() + 1;

   SetCommented(iLen != 0);

   return UpdateMemo("COMMENT",(BYTE*)(LPCTSTR)_sComment,iLen);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::SaveText()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool CToyotaDBDlg::SaveText()
{
   CWaitCursor       Waiter;

   UpdateData(TRUE);

   CString     sOldText = _T("");

   GetTextColumn(_pTableLink,_iCurRec,"TEXT",sOldText);
   
   if (_sDst == sOldText)
   {
      // Is NOT changed !
      return true;
   }

   int   iLen = _sDst.IsEmpty()  ?  0  :  _sDst.GetLength() + 1;

   SetChanged(iLen != 0);

   return UpdateMemo("TEXT",(BYTE*)(LPCTSTR)_sDst,iLen);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::DB_RetrieveCurRec()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

int CToyotaDBDlg::DB_RetrieveCurRec()
{
   if (!_pTableLink)
   {
      // Error !
      ASSERT(0);
      return 1;
   }

   return max(_pTableLink->GetCurRec(),1);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnBtnSearch()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnBtnSearch() 
{
   CheckModifications();

   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnBtnSomePrev()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnBtnSomePrev() 
{
   CheckModifications();

   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnBtnSomeNext()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnBtnSomeNext() 
{
   CheckModifications();

   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnBtnUpdateAll()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnBtnUpdateAll() 
{
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::UpdateFix()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool CToyotaDBDlg::UpdateFix
(
   DBX_TABLE*        pTable,
   DWORD             dwIndex,
   DWORD             dwTextPos,
   DWORD             dwCmntPos,
   DWORD             dwArrayPos,
   BYTE              byFlags
)
{
   const BYTE* const    pRecord = pTable->Go(_iCurRec);

   ASSERT(pRecord);

   ASSERT(!(dwTextPos  & 0x07));
   ASSERT(!(dwCmntPos  & 0x07));
   ASSERT(!(dwArrayPos & 0x07));

   DBX_COLUMN*    pIndex = pTable->GetColumn("INDEX");
   DBX_COLUMN*    pText  = pTable->GetColumn("TEXT");
   DBX_COLUMN*    pCmnt  = pTable->GetColumn("COMMENT");
   DBX_COLUMN*    pArray = pTable->GetColumn("ARRAY");

   DBX_COLUMN*    pFlags = pTable->GetColumn("FLAGS");

   ASSERT(pIndex);
   ASSERT(pText);
   ASSERT(pCmnt);
   ASSERT(pArray);
   ASSERT(pFlags);

   DWORD    dwFlags = byFlags;

   BYTE     _Record[MAX_PATH];

   memset(&_Record,0x20,MAX_PATH);

   pIndex->Set(&_Record,&dwIndex);
   pText-> Set(&_Record,&dwTextPos);
   pCmnt-> Set(&_Record,&dwCmntPos);
   pArray->Set(&_Record,&dwArrayPos);
   pFlags->Set(&_Record,&dwFlags);

   return pTable->UpdateRecord(_iCurRec,(BYTE*)&_Record);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::Update()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::Update
(
   BYTE*                pText,
   DWORD                dwTextSize,
   DBX_TABLE*           pTable,
   DWORD                dwIndex,
   BYTE*                pComment,
   DWORD                dwCommentSize,
   BYTE*                pArray,
   DWORD                dwArraySize,
   DWORD                dwTextOfs,
   DWORD                dwCmntOfs,
   DWORD                dwArrayOfs,
   BYTE                 byFlags,
   bool                 bDirtyTexts
)
{
   ASSERT(pText);
   ASSERT(dwTextSize);
   ASSERT(pTable);
   ASSERT(dwIndex);

   DWORD    dwTextPos  = 0;
   DWORD    dwCmntPos  = 0;
   DWORD    dwArrayPos = 0;

   // Keep OLD value
   dwArrayPos = dwArrayOfs;

   if (bDirtyTexts)
   {
      dwTextPos = AppendVar(pText,   dwTextSize);
      dwCmntPos = AppendVar(pComment,dwCommentSize);
   }
   else
   {
      // Keep OLD values
      dwTextPos = dwTextOfs;
      dwCmntPos = dwCmntOfs;
   }

   UpdateFix(pTable,dwIndex,dwTextPos,dwCmntPos,dwArrayPos,byFlags);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnClickRedtTranslation()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnClickRedtTranslation(NMHDR* pNMHDR, LRESULT* pResult) 
{
   // FixMe !
   // TODO: Add your control notification handler code here
   
   *pResult = 0;
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnChangeEdtOriginal()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnChangeEdtOriginal() 
{
   if (!_pTechnical)
   {
      // Error !
      ASSERT(0);
      return;
   }

   DWORD    dwCnt = _pTechnical->GetRecCnt();

   if (!dwCnt)
   {
      // Error !
      // Is NOT Open?
      return;
   }
   
   GetDlgItemText(IDC_EDT_ORIGINAL,_sOriginal);

   if (_sOriginal.IsEmpty())
   {
      // Nothing to do !
      return;
   }
   
   SearchTitle();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::SearchTitle()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::SearchTitle()
{
   if (!_pTechnical)
   {
      // Error !
      ASSERT(0);
      return;
   }

   char     pszPattern[MAX_PATH + 1];

   strncpy(pszPattern,(LPCTSTR)_sOriginal,MAX_PATH);

   DWORD    dwPatLen = strlen(pszPattern);

   strupr(pszPattern);

   DWORD    dwCnt = _pTechnical->GetRecCnt();

   for (DWORD ii = 0; ii < dwCnt; ++ii)
   {
      DBX_COLUMN*    pSrc = _pTechnical->GetColumn("ENGLISH");

      ASSERT(pSrc);

      const BYTE* const    pRecord = _pTechnical->Go(ii + 1);

      ASSERT(pRecord);

      DWORD    dwSrc = *(DWORD*)pSrc->Get(pRecord);

      ASSERT(dwSrc);
      ASSERT(!(dwSrc & 0x07));

      ASSERT(_pTechnical->GetMemBufVar());
   
      char     pszTest[MAX_PATH + 1];

      strcpy(pszTest,(char*)(_pTechnical->GetMemBufVar() + dwSrc + sizeof(DWORD)));

      if (pszTest && *pszTest)
      {
         strupr(pszTest);

         if (!memcmp(pszTest,pszPattern,dwPatLen))
         {
            m_List.EnsureVisible(ii,FALSE);
            return;
         }
      }
   }

   MessageBeep(MB_ICONEXCLAMATION);
   _sOriginal.SetAt(_sOriginal.GetLength() - 1,0);

   // Dirty trick to set Cursor beyond of the text !
   m_Original.SetSel(0,-1,FALSE);
   m_Original.ReplaceSel((LPCTSTR)_sOriginal,TRUE);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnClickListDix()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnClickListDix(NMHDR* pNMHDR, LRESULT* pResult) 
{
   *pResult = 0;

   ASSERT(_pTechnical);

   if (!_pTechnical || !_pTechnical->GetRecCnt())
   {
      return;
   }

   NM_LISTVIEW*   pNMListView = (NM_LISTVIEW*)pNMHDR;
   
   int   iSelection = pNMListView->iItem;

   DBX_COLUMN*    pSrc = _pTechnical->GetColumn("ENGLISH");
   DBX_COLUMN*    pDst = _pTechnical->GetColumn("RUSSIAN");

   ASSERT(pDst);

   const BYTE* const    pRecord = _pTechnical->Go(iSelection + 1);

   ASSERT(pRecord);

   DWORD    dwDst = *(DWORD*)pDst->Get(pRecord);

   ASSERT(dwDst);
   ASSERT(!(dwDst & 0x07));

   ASSERT(_pTechnical->GetMemBufVar());

   m_Translation = _pTechnical->GetMemBufVar() + dwDst + sizeof(DWORD);

   UpdateData(FALSE);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnDblclkListDix()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnDblclkListDix(NMHDR* pNMHDR, LRESULT* pResult) 
{
   OnClickListDix(pNMHDR,pResult);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnRclickListDix()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnRclickListDix(NMHDR* pNMHDR, LRESULT* pResult) 
{
   OnClickListDix(pNMHDR,pResult);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnGetdispinfoListDix()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnGetdispinfoListDix(NMHDR* pNMHDR,LRESULT* pResult) 
{
   *pResult = 0;

   LV_DISPINFO*      pDispInfo = (LV_DISPINFO*)pNMHDR;

   LV_ITEM*          pItem = &(pDispInfo)->item;

   int      iIndex = pItem->iItem;

   if (pItem->mask & LVIF_TEXT)
   {
      ASSERT(_pTechnical);

      DBX_COLUMN*    pSrc = _pTechnical->GetColumn("ENGLISH");

      ASSERT(pSrc);

      const BYTE* const    pRecord = _pTechnical->Go(iIndex + 1);

      ASSERT(pRecord);

      DWORD    dwSrc = *(DWORD*)pSrc->Get(pRecord);

      ASSERT(dwSrc);
      ASSERT(!(dwSrc & 0x07));

      ASSERT(_pTechnical->GetMemBufVar());
   
      CString  sEN = _pTechnical->GetMemBufVar() + dwSrc + sizeof(DWORD);

      if (!sEN.IsEmpty())
      {
         switch (pItem->iSubItem)
         {
            case 0: 
            {
               wsprintf(pItem->pszText,"%s",(LPCTSTR)sEN);
               break;
            }
         }
      }
   }
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnChangeEdtComment()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnChangeEdtComment() 
{
   // FixMe !
//   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::PreTranslateMessage()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

BOOL CToyotaDBDlg::PreTranslateMessage(MSG* pMsg) 
{
   if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
   {  
      // Prevent <ESC> exit !
      return TRUE;
   }
   
   return CDialog::PreTranslateMessage(pMsg);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnBtnCompact()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnBtnCompact() 
{
   CWaitCursor       Waiter;

   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::AppendVar()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD CToyotaDBDlg::AppendVar(const BYTE* const pText,DWORD dwSize)
{
   ASSERT(_pTableLink);
   ASSERT(_pTableLink->_hVarFile != INVALID_HANDLE_VALUE);

   // Ensure write to END !
   SetFilePointerEOF(_pTableLink->_hVarFile);
                                        
   DWORD    dwFileSize = GetFileSize(_pTableLink->_hVarFile,NULL);

   DWORD    dwBound = 0;

   if (_dwGranulation)
   {
      dwBound = ((dwFileSize + ((1 << _dwGranulation) - 1)) >> _dwGranulation) << _dwGranulation;
   }
   else
   {
      dwBound = dwFileSize;
   }

   DWORD    dwDelta = dwBound - dwFileSize;

   ASSERT(dwDelta < (WORD)(1 << _dwGranulation));

   if (dwDelta)
   {
      WriteBuffer(_pTableLink->_hVarFile,&_Dummy,dwDelta);
   }

   WriteBuffer(_pTableLink->_hVarFile,&dwSize,sizeof(DWORD));
   WriteBuffer(_pTableLink->_hVarFile,(void*)pText,dwSize);

   _pTableLink->_dwDiskSizeVar = GetFileSize(_pTableLink->_hVarFile,NULL);

   return dwBound;
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::DeleteVar()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool CToyotaDBDlg::DeleteVar(DWORD dwPos)
{
   ASSERT(dwPos);
   ASSERT(!(dwPos & 0x07));

   if (_pTableLink->_hVarFile == INVALID_HANDLE_VALUE)
   {
      // Error !
      ASSERT(0);
      return false;
   }

   if (SetFilePointerBOF(_pTableLink->_hVarFile))
   {
      // Error !
      ASSERT(0);
      return false;
   }

   DBV_HEADER     VarHeader;

   memset(&VarHeader,0,sizeof(DBV_HEADER));

   if (!ReadBuffer(_pTableLink->_hVarFile,&VarHeader,sizeof(DBV_HEADER)))
   {
      // Error !
      ASSERT(0);
      return false;
   }

   // Update 'Next' field only
   if (SetFilePointerCUR(_pTableLink->_hVarFile,dwPos + sizeof(DWORD),SEEK_SET) != (dwPos + sizeof(DWORD)))
   {
      // Error !
      ASSERT(0);
      return false;
   }

   // Update Var
   DWORD    dwNext = VarHeader._dwFreeListHead;

   VarHeader._dwFreeListHead = dwPos;

   if (!WriteBuffer(_pTableLink->_hVarFile,&dwNext,sizeof(DWORD)))
   {
      // Error !
      ASSERT(0);
      return false;
   }

   // Update Header
   if (SetFilePointerBOF(_pTableLink->_hVarFile))
   {
      // Error !
      ASSERT(0);
      return false;
   }

   if (!WriteBuffer(_pTableLink->_hVarFile,&VarHeader,sizeof(DBV_HEADER)))
   {
      // Error !
      ASSERT(0);
      return false;
   }

   return true;
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::GetTextColumn()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::GetTextColumn
(
   DBX_TABLE*                 pTable,
   int                        iRecNum,
   const char* const          pszTitle,
   CString&                   rsText
)
{
   const BYTE* const    pRecord = pTable->Go(iRecNum);

   ASSERT(pRecord);

   DBX_COLUMN*    pText = pTable->GetColumn(pszTitle);

   ASSERT(pText);

   DWORD    dwText = *(DWORD*)pText->Get(pRecord);

//   ASSERT(dwText);
   ASSERT(!(dwText & 0x07));

   rsText = _T("");

   if (dwText)
   {
      DWORD    dwMemoText = 0;

      BYTE*    pMemoText = pTable->GetMemo(dwText,dwMemoText);

      ASSERT(pMemoText);
      ASSERT(dwMemoText);

      rsText = pMemoText;

      delete[] pMemoText;
      pMemoText = NULL; 
   }
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::SetCommented()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool CToyotaDBDlg::SetCommented(bool bSet)
{
   CWaitCursor       Waiter;

   if (!_pTableLink)
   {
      // Error !
      ASSERT(0);
      return false;
   }

   const BYTE* const    pRecord = _pTableLink->Go(_iCurRec);

   if (!pRecord)
   {
      // Error !
      ASSERT(0);
      return  false;
   }

   DBX_COLUMN*    pFlags = _pTableLink->GetColumn("FLAGS");

   ASSERT(pFlags);

   BYTE     byFlags = *(BYTE*)pFlags->Get(pRecord);

   DWORD    dwFlags = byFlags;
   
   if (bSet)
   {
      dwFlags |= DBV_FLAG_COMMENTED;
   }
   else
   {
      dwFlags &= ~DBV_FLAG_COMMENTED;
   }

   BYTE     _Record[MAX_PATH];

   memset(&_Record,0x20,MAX_PATH);
   memcpy(&_Record,pRecord,_pTableLink->_FixHeader._wRecSize);

   pFlags->Set(&_Record,&dwFlags);

   return _pTableLink->UpdateRecord(_iCurRec,(BYTE*)&_Record);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::SetChanged()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool CToyotaDBDlg::SetChanged(bool bSet)
{
   CWaitCursor       Waiter;

   if (!_pTableLink)
   {
      // Error !
      ASSERT(0);
      return false;
   }

   const BYTE* const    pRecord = _pTableLink->Go(_iCurRec);

   if (!pRecord)
   {
      // Error !
      ASSERT(0);
      return  false;
   }

   DBX_COLUMN*    pFlags = _pTableLink->GetColumn("FLAGS");

   ASSERT(pFlags);

   BYTE     byFlags = *(BYTE*)pFlags->Get(pRecord);

   DWORD    dwFlags = byFlags;
   
   if (bSet)
   {
      dwFlags |= DBV_FLAG_CHANGED;
   }
   else
   {
      dwFlags &= ~DBV_FLAG_CHANGED;
   }

   BYTE     _Record[MAX_PATH];

   memset(&_Record,0x20,MAX_PATH);
   memcpy(&_Record,pRecord,_pTableLink->_FixHeader._wRecSize);

   pFlags->Set(&_Record,&dwFlags);

   return _pTableLink->UpdateRecord(_iCurRec,(BYTE*)&_Record);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::UpdateMemo()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool CToyotaDBDlg::UpdateMemo
(
   const char* const       pszTitle,
   const BYTE* const       pBuf,
   DWORD                   dwSize
)
{
   DBX_COLUMN*    pMemo      = _pTableLink->GetColumn(pszTitle);
   DBX_COLUMN*    pTimestamp = _pTableLink->GetColumn("TIMESTAMP");

   ASSERT(pMemo);
   ASSERT(pTimestamp);

   const BYTE* const    pRecord = _pTableLink->Go(_iCurRec);

   ASSERT(pRecord);

   DWORD    dwPos = *(DWORD*)pMemo->Get(pRecord);

   if (!_pTableLink || (_pTableLink->_hVarFile == INVALID_HANDLE_VALUE))
   {
      // Error !
      ASSERT(0);
      return false;
   }

   if (dwPos)
   {
      if (!DeleteVar(dwPos))
      {
         // Error !
         ASSERT(0);
         return false;
      }
   }

   dwPos = AppendVar(pBuf,dwSize);

   BYTE     _Record[MAX_PATH];

   memset(&_Record,0x20,MAX_PATH);
   memcpy(&_Record,pRecord,_pTableLink->_FixHeader._wRecSize);

   FILETIME    TimeNow;

   GetSystemTimeAsFileTime(&TimeNow);

   WORD        wFatDate = 0;
   WORD        wFatTime = 0;

   FileTimeToDosDateTime(&TimeNow,&wFatDate,&wFatTime);

   DWORD    dwTimestamp = (wFatDate << 16) | wFatTime;

   pMemo->     Set(&_Record,&dwPos);
   pTimestamp->Set(&_Record,&dwTimestamp);

   return _pTableLink->UpdateRecord(_iCurRec,(BYTE*)&_Record);
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnBtnTwins()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnBtnTwins() 
{
   CTwins      Twins;

   Twins.SetParent(this);

   Twins.DoModal();

   // Reload Tables
   OnCloseupCmbTable();
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnTrayNotification()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

LRESULT CToyotaDBDlg::OnTrayNotification(WPARAM wParam,LPARAM lParam)
{
   //Return quickly if its not for this tray icon
   if (wParam != IDR_POPUP_MENU)
   {
      return 0;
   }

   if (IsWindowVisible())
   {
      return 0;
   }
   
   // Clicking with right button brings up a context menu
   if (LOWORD(lParam) == WM_LBUTTONUP)
   {
      if (_pTray)
      {
         _pTray->MaximiseFromTray(this);
      }

      return 0;
   }
   
   return 0;
}

/* ******************************************************************** **
** @@ CToyotaDBDlg::OnClose()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CToyotaDBDlg::OnClose() 
{
   if (!IsWindowVisible())
   {
      return;
   }

   if (!_bExit)
   {
      if (_pTray)
      {
         _pTray->MinimiseToTray(this);
      }
   }
   else
   {
      CDialog::OnClose();   
   }
}
