#include "stdafx.h"

#include "resource.h"

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

#include "toyotadb.h"
#include "ToyotaDBDlg.h"
#include "Twins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_TEXT_SIZE         (512)   // Due Fucking MS Limit !
#define HASH_DIGEST_SIZE      (sizeof(DWORD) * 4)        // Binary digest

/* ******************************************************************** **
** @@ CMP_IndexFrom()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

int __cdecl CMP_IndexFrom(const void* pKey1,const void* pKey2)
{
   DBF_REC_LINK*    p1 = (DBF_REC_LINK*)pKey1;
   DBF_REC_LINK*    p2 = (DBF_REC_LINK*)pKey2;

   if (p1->_dwIndexFrom > p2->_dwIndexFrom)
   {
      return 1;
   }
   else if (p1->_dwIndexFrom < p2->_dwIndexFrom)
   {
      return -1;
   }
   else
   {
      return 0;
   }
}

/* ******************************************************************** **
** @@ CMP_LinkTo()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

int __cdecl CMP_LinkTo(const void* pKey1,const void* pKey2)
{
   DBF_REC_MAP*   p1 = (DBF_REC_MAP*)pKey1;
   DBF_REC_MAP*   p2 = (DBF_REC_MAP*)pKey2;

   if (p1->_dwLinkTo > p2->_dwLinkTo)
   {
      return 1;
   }
   else if (p1->_dwLinkTo < p2->_dwLinkTo)
   {
      return -1;
   }
   else
   {
      return 0;
   }
}

/* ******************************************************************** **
** @@ CTwins::CTwins()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : Constructor
** ******************************************************************** */

CTwins::CTwins(CWnd* pParent /*=NULL*/)
:  CDialog(CTwins::IDD, pParent)
{
   //{{AFX_DATA_INIT(CTwins)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT

   _pImageList = NULL;

   _pParent = NULL;

   _pChecksEN = NULL;
   _pChecksRU = NULL;

   _dwRecCnt = 0;

   _bSelectedLeft  = false;
   _bSelectedRight = false;
   
   _dwSyncPosLeft  = 0;
   _dwSyncPosRight = 0;
}

/* ******************************************************************** **
** @@ CTwins::DoDataExchange()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CTwins::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTwins)
   DDX_Control(pDX, IDC_BTN_DN, m_Dn);
   DDX_Control(pDX, IDC_BTN_UP, m_Up);
   DDX_Control(pDX, IDC_LIST2, List_RU);
   DDX_Control(pDX, IDC_LIST1, List_EN);
   //}}AFX_DATA_MAP
}

/* ******************************************************************** **
** @@ CTwins::MESSAGE_MAP()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

BEGIN_MESSAGE_MAP(CTwins, CDialog)
   //{{AFX_MSG_MAP(CTwins)
   ON_WM_DESTROY()
   ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
   ON_NOTIFY(NM_CLICK, IDC_LIST2, OnClickList2)
   ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
   ON_NOTIFY(NM_RCLICK, IDC_LIST2, OnRclickList2)
   ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
   ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
   ON_NOTIFY(LVN_GETDISPINFO,IDC_LIST1,OnGetdispinfoList1)
   ON_NOTIFY(LVN_GETDISPINFO,IDC_LIST2,OnGetdispinfoList2)
   ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, OnKeydownList1)
   ON_NOTIFY(LVN_KEYDOWN, IDC_LIST2, OnKeydownList2)
   ON_EN_CHANGE(IDC_EDT_FIND_L, OnChangeEdtFindL)
   ON_EN_CHANGE(IDC_EDT_FIND_R, OnChangeEdtFindR)
   ON_BN_CLICKED(IDC_BTN_PREV_L, OnBtnPrevL)
   ON_BN_CLICKED(IDC_BTN_NEXT_L, OnBtnNextL)
   ON_BN_CLICKED(IDC_BTN_PREV_R, OnBtnPrevR)
   ON_BN_CLICKED(IDC_BTN_NEXT_R, OnBtnNextR)
   ON_BN_CLICKED(IDC_BTN_SYNC, OnBtnSync)
   ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
   ON_BN_CLICKED(IDC_BTN_DN, OnBtnDn)
	ON_BN_CLICKED(IDC_BTN_FIRST, OnBtnFirst)
	ON_BN_CLICKED(IDC_BTN_UP2, OnBtnUp2)
	ON_BN_CLICKED(IDC_BTN_PREV2, OnBtnPrev2)
	ON_BN_CLICKED(IDC_BTN_NEXT2, OnBtnNext2)
	ON_BN_CLICKED(IDC_BTN_DN2, OnBtnDn2)
	ON_BN_CLICKED(IDC_BTN_LAST2, OnBtnLast2)
	ON_BN_CLICKED(IDC_BTN_GO2, OnBtnGo2)
	ON_EN_CHANGE(IDC_EDT_GO2, OnChangeEdtGo2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/* ******************************************************************** **
** @@ CTwins::OnInitDialog()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

BOOL CTwins::OnInitDialog() 
{
   CDialog::OnInitDialog();
   
   CenterWindow();
   
   // Virtual List !
   List_EN.InsertColumn( 0,"##",   LVCFMT_LEFT,   70, 0);
   List_EN.InsertColumn( 1,"Index",LVCFMT_RIGHT,  50, 1);
   List_EN.InsertColumn( 2,"GUID", LVCFMT_LEFT,    5, 2);
   List_EN.InsertColumn( 3,"Text", LVCFMT_LEFT, 1000, 3);

   DWORD    dwStyleEN = List_EN.GetExtendedStyle();

   dwStyleEN |= LVS_EX_CHECKBOXES | LVS_OWNERDATA | LVS_EX_FULLROWSELECT | LVS_EX_FLATSB | LVS_EX_GRIDLINES;

   List_EN.SetExtendedStyle(dwStyleEN);

   List_EN.SetItemCount(0);
   List_EN.UpdateWindow();

   // Virtual List !
   List_RU.InsertColumn( 0,"##",   LVCFMT_LEFT,   70, 0);
   List_RU.InsertColumn( 1,"RecNo",LVCFMT_RIGHT,  50, 1);
   List_RU.InsertColumn( 2,"Index",LVCFMT_RIGHT,  50, 2);
   List_RU.InsertColumn( 3,"From", LVCFMT_RIGHT,  50, 3);
   List_RU.InsertColumn( 4,"To",   LVCFMT_RIGHT,  50, 4);
   List_RU.InsertColumn( 5,"Dups", LVCFMT_RIGHT,  20, 5);
   List_RU.InsertColumn( 6,"Text", LVCFMT_LEFT, 1000, 6);

   DWORD    dwStyleRU = List_RU.GetExtendedStyle();

   dwStyleRU |= LVS_EX_CHECKBOXES | LVS_OWNERDATA | LVS_EX_FULLROWSELECT | LVS_EX_FLATSB | LVS_EX_GRIDLINES;

   List_RU.SetExtendedStyle(dwStyleRU);

   _dwRecCnt = 0;

   if (_pParent && _pParent->_pTableLink)
   {
      _dwRecCnt = _pParent->_pTableLink->_FixHeader._dwRecCnt;
   }

   _dwRecCnt = _dwRecCnt  ?  _dwRecCnt  :  1;

   _pChecksEN = new bool[_dwRecCnt];
   _pChecksRU = new bool[_dwRecCnt];

   memset(_pChecksEN,0,_dwRecCnt);
   memset(_pChecksRU,0,_dwRecCnt);

   m_Up.SetBitmap(::LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP_UP)));
   m_Dn.SetBitmap(::LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP_DN)));

   if (!_pImageList)
   {
      _pImageList = new CImageList();

      ASSERT(_pImageList); 

      if (_pImageList)
      {
         CToyotaDBApp*     pApp = (CToyotaDBApp*)AfxGetApp();

         _pImageList->Create(16,16,ILC_COLOR4,0,2);

         CBitmap     BM;
         
         BM.LoadBitmap(IDB_BITMAP_LT);
         
         _pImageList->Add(&BM,RGB(0,0,0));

         List_EN.SetImageList(_pImageList,LVSIL_STATE);
         List_RU.SetImageList(_pImageList,LVSIL_STATE);
      }
   }

   SyncEnabler();

   List_EN.SetItemCount(_dwRecCnt);
   List_EN.UpdateWindow();

   List_RU.SetItemCount(_dwRecCnt);
   List_RU.UpdateWindow();

   List_EN.EnsureVisible(_pParent->_iCurRec - 1,FALSE);
   List_RU.EnsureVisible(_pParent->_iCurRec - 1,FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

/* ******************************************************************** **
** @@ CTwins::OnDestroy()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CTwins::OnDestroy()
{
   CDialog::OnDestroy();
   
   if (_pImageList)
   {
      _pImageList->DeleteImageList();
      delete _pImageList;
      _pImageList = NULL;
   }

   if (_pChecksEN)
   {
      delete[] _pChecksEN; 
      _pChecksEN = NULL;
   }

   if (_pChecksRU)
   {
      delete[] _pChecksRU; 
      _pChecksRU = NULL;
   }
}

/* ******************************************************************** **
** @@ CTwins::OnClickList1()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CTwins::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
   *pResult = 0;

   if (!_pParent)
   {
      // Error !
      ASSERT(0);
      return;
   }

   int      iIndex = List_EN.GetSelectionMark();

   if (iIndex < 0)
   {
      return;
   }

   // Link
   DBX_COLUMN*    pRecNum = _pParent->_pTableLink->GetColumn("RECORD NUM");

   const BYTE* const    pRecordLink = _pParent->_pTableLink->Go(iIndex + 1);

   ASSERT(pRecordLink);

   ASSERT(pRecNum);

   DWORD    dwRecNum = *(DWORD*)pRecNum->Get(pRecordLink);

   const BYTE* const    pRecordEN = _pParent->_pTableEN->Go(dwRecNum);

   if (!_pParent->_pTableEN)
   {
      // Error !
      ASSERT(0);
      return;
   }

   DBX_COLUMN*    pIndexEN = _pParent->_pTableEN->GetColumn("INDEX");

   DWORD    dwIndexFrom = *(DWORD*)pIndexEN->Get(pRecordEN);

   DWORD    dwRecNumRu = FindByIndex(dwIndexFrom);

   int   iTop = dwRecNumRu - 1;

   int   iCnt = List_EN.GetCountPerPage();

   int   iBottom = min((int)dwRecNumRu + iCnt,(int)_dwRecCnt - 1);

   List_RU.SetItemState(iTop,LVIS_SELECTED,LVIS_SELECTED);
   List_RU.SetSelectionMark(iTop);

   // Dirty trick to synchronize views
   List_RU.EnsureVisible(iTop,FALSE);
   List_RU.EnsureVisible(iBottom,FALSE);
   List_RU.EnsureVisible(iTop,FALSE);

   List_RU.SetSelectionMark(iTop);
}

/* ******************************************************************** **
** @@ CTwins::OnClickList2()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CTwins::OnClickList2(NMHDR* pNMHDR,LRESULT* pResult) 
{
   *pResult = 0;

   if (!_pParent)
   {
      // Error !
      ASSERT(0);
      return;
   }

   int      iIndex = List_RU.GetSelectionMark();

   Enabler(iIndex);

   if (iIndex < 0)
   {
      GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
      GetDlgItem(IDC_BTN_DN)->EnableWindow(FALSE);
   }
   else if (!iIndex)
   {
      GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
      GetDlgItem(IDC_BTN_DN)->EnableWindow(TRUE);
   }
   else if (iIndex == (_pParent->_iRecTotal - 1))
   {
      GetDlgItem(IDC_BTN_UP)->EnableWindow(TRUE);
      GetDlgItem(IDC_BTN_DN)->EnableWindow(FALSE);
   }
   else if ((iIndex >= 0) && iIndex < (_pParent->_iRecTotal))   
   {
      GetDlgItem(IDC_BTN_UP)->EnableWindow(TRUE);
      GetDlgItem(IDC_BTN_DN)->EnableWindow(TRUE);
   }
}

/* ******************************************************************** **
** @@ CTwins::OnDblclkList1()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CTwins::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
   *pResult = 0;

   if (!_pParent)
   {
      // Error !
      ASSERT(0);
      return;
   }

   int      iIndex = List_EN.GetSelectionMark();

   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

/* ******************************************************************** **
** @@ CTwins::OnRclickList1()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CTwins::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
   *pResult = 0;

   NM_LISTVIEW*      pNMListView = (NM_LISTVIEW*)pNMHDR;

   LVHITTESTINFO     hitinfo;

   // Copy click point
   hitinfo.pt = pNMListView->ptAction;

   // Make the hit test...
   int item = List_EN.HitTest(&hitinfo);

   if (item == -1)
   {
      // Nothing to do !
      return;
   }

   // We hit one item... did we hit state image (check box)?
   // This test only works if we are in list or report mode.
   if ((hitinfo.flags & LVHT_ONITEMSTATEICON))
   {
      ToggleCheckBox1(item);
   }

   OnClickList1(pNMHDR,pResult);
}

/* ******************************************************************** **
** @@ CTwins::OnGetdispinfoList1()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CTwins::OnGetdispinfoList1(NMHDR* pNMHDR,LRESULT* pResult) 
{
   *pResult = 0;

   if (!_pParent)
   {
      // Error !
      ASSERT(0);
      return;
   }

   NMLVDISPINFO*     pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

   LV_ITEM*    pItem = &(pDispInfo)->item;

   int      iIndex = pItem->iItem;

   // Link
   DBX_COLUMN*    pRecNum = _pParent->_pTableLink->GetColumn("RECORD NUM");

   const BYTE* const    pRecordLink = _pParent->_pTableLink->Go(iIndex + 1);

   ASSERT(pRecordLink);

   ASSERT(pRecNum);

   DWORD    dwRecNum = *(DWORD*)pRecNum->Get(pRecordLink);

   // English
   ASSERT(_pParent->_pTableEN);

   DBX_COLUMN*    pIndexEN = _pParent->_pTableEN->GetColumn("INDEX");
   DBX_COLUMN*    pGUID    = _pParent->_pTableEN->GetColumn("GUID");
   DBX_COLUMN*    pTextEN  = _pParent->_pTableEN->GetColumn("TEXT");

   ASSERT(pIndexEN);
   ASSERT(pTextEN);

   const BYTE* const    pRecordEN = _pParent->_pTableEN->Go(dwRecNum);

   ASSERT(pRecordEN);

   DWORD    dwIndexEN = *(DWORD*)pIndexEN->Get(pRecordEN);
   DWORD    dwTextEN  = *(DWORD*)pTextEN-> Get(pRecordEN);

   ASSERT(dwTextEN);
   ASSERT(!(dwTextEN & 0x07));

   ASSERT(_pParent->_pTableEN->GetMemBufVar());
   
   if (pItem->mask & LVIF_TEXT)
   {
      switch (pItem->iSubItem)
      {
         case 0: // ##
         {
            sprintf(pItem->pszText,"%ld",iIndex + 1);
            break;
         }
         case 1: // Index
         {
            sprintf(pItem->pszText,"%ld",dwIndexEN);
            break;
         }
         case 2: // GUID
         {
            char  pszDigest[MAX_PATH];

            for (int ii = 0, jj = 0; ii < HASH_DIGEST_SIZE; ++ii, jj += 2)
            {
               sprintf(&pszDigest[jj],"%02X",((BYTE*)pGUID-> Get(pRecordEN))[ii]);
            }

            strcpy(pItem->pszText,pszDigest);
            break;
         }
         case 3: // Text
         {
            strncpy(pItem->pszText,(char*)_pParent->_pTableEN->GetMemBufVar() + dwTextEN + sizeof(DWORD),MAX_TEXT_SIZE);
            pItem->pszText[MAX_TEXT_SIZE] = 0;  // Ensure ASCIIZ
            break;
         }
      }

      if( pItem->mask & LVIF_IMAGE)
      {
         // To enable check box, we have to enable state mask...
         pItem->mask      |= LVIF_STATE;
         pItem->stateMask  = LVIS_STATEIMAGEMASK;

         if (_pChecksEN[iIndex])
         {
            // Turn check box on
            pItem->state = INDEXTOSTATEIMAGEMASK(2);
         }
         else
         {
            // Turn check box off
            pItem->state = INDEXTOSTATEIMAGEMASK(1);
         }
      }
   }
}

/* ******************************************************************** **
** @@ CTwins::OnClickList2()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CTwins::Enabler(int iIndex)
{
   if (iIndex < 0)
   {
      GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
      GetDlgItem(IDC_BTN_DN)->EnableWindow(FALSE);
      MessageBeep(MB_ICONSTOP);
   }
   else if (!iIndex)
   {
      GetDlgItem(IDC_BTN_UP)->EnableWindow(FALSE);
      GetDlgItem(IDC_BTN_DN)->EnableWindow(TRUE);
      MessageBeep(MB_ICONSTOP);
   }
   else if (iIndex == (_pParent->_iRecTotal - 1))
   {
      GetDlgItem(IDC_BTN_UP)->EnableWindow(TRUE);
      GetDlgItem(IDC_BTN_DN)->EnableWindow(FALSE);
      MessageBeep(MB_ICONSTOP);
   }
   else if ((iIndex >= 0) && iIndex < (_pParent->_iRecTotal))   
   {
      GetDlgItem(IDC_BTN_UP)->EnableWindow(TRUE);
      GetDlgItem(IDC_BTN_DN)->EnableWindow(TRUE);
   }
}

/* ******************************************************************** **
** @@ CTwins::OnDblclkList2()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CTwins::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
   *pResult = 0;

   if (!_pParent)
   {
      // Error !
      ASSERT(0);
      return;
   }

   int      iIndex = List_RU.GetSelectionMark();

   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

/* ******************************************************************** **
** @@ CTwins::OnRclickList2()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CTwins::OnRclickList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
   *pResult = 0;

   NM_LISTVIEW*      pNMListView = (NM_LISTVIEW*)pNMHDR;

   LVHITTESTINFO     hitinfo;

   // Copy click point
   hitinfo.pt = pNMListView->ptAction;

   // Make the hit test...
   int item = List_RU.HitTest(&hitinfo);

   if (item == -1)
   {
      // Nothing to do !
      return;
   }

   // We hit one item... did we hit state image (check box)?
   // This test only works if we are in list or report mode.
   if ((hitinfo.flags & LVHT_ONITEMSTATEICON))
   {
      ToggleCheckBox2(item);
   }

   OnClickList2(pNMHDR,pResult);
}

/* ******************************************************************** **
** @@ CTwins::OnGetdispinfoList2()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void CTwins::OnGetdispinfoList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
   *pResult = 0;

   if (!_pParent)
   {
      // Error !
      ASSERT(0);
      return;
   }

   NMLVDISPINFO*     pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

   LV_ITEM*    pItem = &(pDispInfo)->item;

   int      iIndex = pItem->iItem;

   // Link
   ASSERT(_pParent->_pTableLink);

   DBX_COLUMN*    pRecNum    = _pParent->_pTableLink->GetColumn("RECORD NUM");
   DBX_COLUMN*    pIndexFrom = _pParent->_pTableLink->GetColumn("INDEX FROM");
   DBX_COLUMN*    pLinkFrom  = _pParent->_pTableLink->GetColumn("LINK FROM");
   DBX_COLUMN*    pLinkTo    = _pParent->_pTableLink->GetColumn("LINK TO");
   DBX_COLUMN*    pLinkCnt   = _pParent->_pTableLink->GetColumn("LINK CNT");
   DBX_COLUMN*    pTextRU    = _pParent->_pTableLink->GetColumn("TEXT");

   ASSERT(pRecNum);
   ASSERT(pIndexFrom);
   ASSERT(pLinkFrom);
   ASSERT(pLinkTo);
   ASSERT(pLinkCnt);
   ASSERT(pTextRU);

   const BYTE* const    pRecordLink = _pParent->_pTableLink->Go(iIndex + 1);

   ASSERT(pRecordLink);

   DWORD    dwRecNum    = *(DWORD*)pRecNum->   Get(pRecordLink);
   DWORD    dwIndexFrom = *(DWORD*)pIndexFrom->Get(pRecordLink);
   DWORD    dwLinkFrom  = *(DWORD*)pLinkFrom-> Get(pRecordLink);
   DWORD    dwLinkTo    = *(DWORD*)pLinkTo->   Get(pRecordLink);
   DWORD    dwLinkCnt   = *(DWORD*)pLinkCnt->  Get(pRecordLink);
   DWORD    dwTextRU    = *(DWORD*)pTextRU->   Get(pRecordLink);

   ASSERT(dwRecNum);
   ASSERT(dwLinkFrom);
   ASSERT(dwLinkTo);

   ASSERT(dwTextRU);
   ASSERT(!(dwTextRU & 0x07));

   CString     sTextRU = _T("");

   GetTextColumn(_pParent->_pTableLink,dwLinkTo,"TEXT",sTextRU);
   
   if (pItem->mask & LVIF_TEXT)
   {
      switch (pItem->iSubItem)
      {
         case 0: // ##
         {
            sprintf(pItem->pszText,"%ld",iIndex + 1);
            break;
         }
         case 1: // RecNo
         {
            sprintf(pItem->pszText,"%ld",dwRecNum);
            break;
         }
         case 2: // Index From
         {
            sprintf(pItem->pszText,"%ld",dwIndexFrom);
            break;
         }
         case 3: // Link From
         {
            sprintf(pItem->pszText,"%ld",dwLinkFrom);
            break;
         }
         case 4: // Link To
         {
            sprintf(pItem->pszText,"%ld",dwLinkTo);
            break;
         }
         case 5: // Link Cnt
         {
            sprintf(pItem->pszText,"%ld",dwLinkCnt);
            break;
         }
         case 6: // Text
         {
            strncpy(pItem->pszText,(LPCTSTR)sTextRU,MAX_TEXT_SIZE);
            pItem->pszText[MAX_TEXT_SIZE] = 0;  // Ensure ASCIIZ
            break;
         }
      }

      if( pItem->mask & LVIF_IMAGE)
      {
         // To enable check box, we have to enable state mask...
         pItem->mask      |= LVIF_STATE;
         pItem->stateMask  = LVIS_STATEIMAGEMASK;

         if (_pChecksRU[iIndex])
         {
            // Turn check box on
            pItem->state = INDEXTOSTATEIMAGEMASK(2);
         }
         else
         {
            // Turn check box off
            pItem->state = INDEXTOSTATEIMAGEMASK(1);
         }
      }
   }
}

/* ******************************************************************** **
** @@ CTwins::SetParent()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */
void CTwins::SetParent(CToyotaDBDlg* pParent)
{
   ASSERT(pParent);

   _pParent = pParent;
}

/* ******************************************************************** **
** @@ CTwins::PreTranslateMessage()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

BOOL CTwins::PreTranslateMessage(MSG* pMsg) 
{
   if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
   {  
      // Prevent <ESC> exit !
      return TRUE;
   }
   
   return CDialog::PreTranslateMessage(pMsg);
}

/* ******************************************************************** **
** @@ CTwins::GetTextColumn()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CTwins::GetTextColumn
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
** @@ CTwins::OnKeydownList1()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : Called when a Space Key pressed down
** ******************************************************************** */

void CTwins::OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult)
{
   *pResult = 0;

   LV_KEYDOWN*    pLVKeyDown = (LV_KEYDOWN*)pNMHDR;

   // If user press space, toggle flag on selected item
   if (pLVKeyDown->wVKey == VK_SPACE)
   {
      // Toggle if some item is selected
      if (List_EN.GetSelectionMark() != -1)
      {
         ToggleCheckBox1(List_EN.GetSelectionMark());
      }
   }
}

/* ******************************************************************** **
** @@ CTwins::OnKeydownList2()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : Called when a Space Key pressed down
** ******************************************************************** */

void CTwins::OnKeydownList2(NMHDR* pNMHDR, LRESULT* pResult)
{
   *pResult = 0;

   LV_KEYDOWN*    pLVKeyDown = (LV_KEYDOWN*)pNMHDR;

   // If user press space, toggle flag on selected item
   if (pLVKeyDown->wVKey == VK_SPACE)
   {
      // Toggle if some item is selected
      if (List_RU.GetSelectionMark() != -1)
      {
         ToggleCheckBox2(List_RU.GetSelectionMark());
      }
   }
}

/* ******************************************************************** **
** @@ CTwins::SyncEnabler()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CTwins::SyncEnabler()
{
   BOOL     bSync = (_bSelectedLeft && _bSelectedRight)  ?  TRUE  :  FALSE;

   GetDlgItem(IDC_BTN_SYNC)->EnableWindow(bSync);
}

/* ******************************************************************** **
** @@ CTwins::ToggleCheckBox1()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CTwins::ToggleCheckBox1(int iIndex)
{
   bool     bState = _pChecksEN[iIndex];

   memset(_pChecksEN,0,_dwRecCnt);
   
   _pChecksEN[iIndex] = !bState;

   _bSelectedLeft = _pChecksEN[iIndex];

   _dwSyncPosLeft = _bSelectedLeft  ?  iIndex + 1  :  0;

   SyncEnabler();

   List_EN.LockWindowUpdate();   
   List_EN.RedrawItems(0,_dwRecCnt - 1);
   List_EN.UnlockWindowUpdate();   
}

/* ******************************************************************** **
** @@ CTwins::ToggleCheckBox2()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CTwins::ToggleCheckBox2(int iIndex)
{
   bool     bState = _pChecksRU[iIndex];

   memset(_pChecksRU,0,_dwRecCnt);
   
   _pChecksRU[iIndex] = !bState;

   _bSelectedRight = _pChecksRU[iIndex];

   _dwSyncPosRight = _bSelectedRight  ?  iIndex + 1  :  0;

   SyncEnabler();

   List_RU.LockWindowUpdate();   
   List_RU.RedrawItems(0,_dwRecCnt - 1);
   List_RU.UnlockWindowUpdate();   
}

void CTwins::OnChangeEdtFindL() 
{
   // TODO: If this is a RICHEDIT control, the control will not
   // send this notification unless you override the CDialog::OnInitDialog()
   // function and call CRichEditCtrl().SetEventMask()
   // with the ENM_CHANGE flag ORed into the mask.
   
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

void CTwins::OnChangeEdtFindR() 
{
   // TODO: If this is a RICHEDIT control, the control will not
   // send this notification unless you override the CDialog::OnInitDialog()
   // function and call CRichEditCtrl().SetEventMask()
   // with the ENM_CHANGE flag ORed into the mask.
   
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

void CTwins::OnBtnPrevL() 
{
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

void CTwins::OnBtnNextL() 
{
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

void CTwins::OnBtnPrevR() 
{
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

void CTwins::OnBtnNextR() 
{
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

/* ******************************************************************** **
** @@ CTwins::OnBtnUp()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CTwins::OnBtnSync() 
{
   CString     sMessage = _T("");

   sMessage.Format("Sure?\n\nIf you answer 'YES', right panel will be synchronized with left panel.\n\n"
      "Record [%d] on right panel will be set as record [%d] and vise versa.",_dwSyncPosRight,_dwSyncPosLeft);

   if (MessageBox((LPCTSTR)sMessage,"Last chance!",MB_YESNO | MB_DEFBUTTON2 | MB_ICONSTOP) != IDYES)
   {
      // Nothing to do !
      return;
   }

   // In this order !    
   ExchangeLink(_dwSyncPosLeft,_dwSyncPosRight);
   ExchangeMapper(_dwSyncPosLeft,_dwSyncPosRight,_dwSyncPosLeft - 1,_dwSyncPosRight - 1,MODE_SYNC);

   ToggleCheckBox1(_dwSyncPosLeft - 1);
   ToggleCheckBox2(_dwSyncPosRight - 1);
}

/* ******************************************************************** **
** @@ CTwins::ExchangeMapper()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CTwins::ExchangeMapper(DWORD dwLinkTo,DWORD dwLinkFrom,int iIndexEN,int iIndexRU,SYNC_MODE Mode)
{
   UpdateMapper(dwLinkTo,dwLinkFrom);
   UpdateMapper(dwLinkFrom,dwLinkTo);

   if (iIndexEN != -1)
   {
      switch (Mode)
      {
         case MODE_UP:
         case MODE_DOWN:
         default:
         {
            List_EN.SetItemState(iIndexEN,LVIS_SELECTED,LVIS_SELECTED);
            List_EN.SetSelectionMark(iIndexEN);
            List_EN.EnsureVisible(iIndexEN,FALSE);
            break;
         }
      }

      List_EN.LockWindowUpdate();   
      List_EN.RedrawItems(0,_dwRecCnt - 1);
      List_EN.UnlockWindowUpdate();   
   }

   if (iIndexRU != -1)
   {
      switch (Mode)
      {
         case MODE_UP:
         {
            List_RU.SetItemState(iIndexRU - 1,LVIS_SELECTED,LVIS_SELECTED);
            List_RU.SetSelectionMark(iIndexRU - 1);
            List_RU.EnsureVisible(iIndexRU - 1,FALSE);
            break;
         }
         case MODE_DOWN:
         {
            List_RU.SetItemState(iIndexRU + 1,LVIS_SELECTED,LVIS_SELECTED);
            List_RU.SetSelectionMark(iIndexRU + 1);
            List_RU.EnsureVisible(iIndexRU + 1,FALSE);
            break;
         }
         case MODE_SYNC:
         {
            List_RU.SetItemState(iIndexEN,LVIS_SELECTED,LVIS_SELECTED);
            List_RU.SetSelectionMark(iIndexEN);
            List_RU.EnsureVisible(iIndexEN,FALSE);
            break;
         }
         default:
         {
            List_RU.SetItemState(iIndexRU,LVIS_SELECTED,LVIS_SELECTED);
            List_RU.SetSelectionMark(iIndexRU);
            List_RU.EnsureVisible(iIndexRU,FALSE);
            break;
         }
      }

      List_RU.LockWindowUpdate();   
      List_RU.RedrawItems(0,_dwRecCnt - 1);
      List_RU.UnlockWindowUpdate();   
   }
}

/* ******************************************************************** **
** @@ CTwins::ExchangeLink()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CTwins::ExchangeLink(DWORD dwLinkTo,DWORD dwLinkFrom)
{
   ASSERT(_pParent->_pTableLink);

   DWORD    dwRecSize = _pParent->_pTableLink->_FixHeader._wRecSize;

   DBX_COLUMN*    pLinkTo = _pParent->_pTableLink->GetColumn("LINK TO");

   ASSERT(pLinkTo);

   // From
   const BYTE* const    pCur = _pParent->_pTableLink->Go(dwLinkFrom);

   ASSERT(pCur);

   DWORD    dwLinkToCur = *(DWORD*)pLinkTo->Get(pCur);

   ASSERT(dwLinkToCur);

   // Prev
   const BYTE* const    pPrev = _pParent->_pTableLink->Go(dwLinkTo);

   ASSERT(pPrev);

   DWORD    dwLinkToPrev = *(DWORD*)pLinkTo->Get(pPrev);

   BYTE     _Record[MAX_PATH];

   memset(&_Record,0x20,MAX_PATH);

   // Prev
   memcpy(&_Record,pPrev,dwRecSize);

   ASSERT(pLinkTo);

   pLinkTo->Set(&_Record,&dwLinkToCur);

   _pParent->_pTableLink->UpdateRecord(dwLinkTo,(BYTE*)&_Record);

   // Current
   memcpy(&_Record,pCur,dwRecSize);

   ASSERT(pLinkTo);

   pLinkTo->Set(&_Record,&dwLinkToPrev); 

   _pParent->_pTableLink->UpdateRecord(dwLinkFrom,(BYTE*)&_Record);
}

/* ******************************************************************** **
** @@ CTwins::OnBtnUp()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CTwins::OnBtnUp() 
{
   if (!_pParent)
   {
      // Error !
      ASSERT(0);
      return;
   }

   int      iIndex = List_RU.GetSelectionMark();

   Enabler(iIndex);

   if (iIndex < 1)
   {
      // Nothing to do !
      return;
   }

   ExchangeLink(iIndex,iIndex + 1);
   ExchangeMapper(iIndex + 1,iIndex,-1,iIndex,MODE_UP);
}

/* ******************************************************************** **
** @@ CTwins::OnBtnDn()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CTwins::OnBtnDn() 
{
   if (!_pParent)
   {
      // Error !
      ASSERT(0);
      return;
   }

   int      iIndex = List_RU.GetSelectionMark();

   Enabler(iIndex);

   if (iIndex < 1)
   {
      // Nothing to do !
      return;
   }

   ExchangeLink(iIndex + 2,iIndex + 1);
   ExchangeMapper(iIndex + 2,iIndex + 1,-1,iIndex,MODE_DOWN);
}

/* ******************************************************************** **
** @@ CTwins::FindByIndex()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD CTwins::FindByIndex(DWORD dwIndexFrom)
{
   if (!_pParent || !_pParent->_pTableRU)
   {
      // Error !
      ASSERT(0);
      return 0;
   }

   DBF_REC_LINK     SrchRec;

   memset(&SrchRec,0,sizeof(DBF_REC_LINK));

   SrchRec._dwIndexFrom = dwIndexFrom;

   DWORD    dwStart = _pParent->_pTableLink->_FixHeader._wDataStart;
   DWORD    dwCount = _pParent->_pTableLink->_FixHeader._dwRecCnt;

   BYTE*    pBuf = _pParent->_pTableLink->_pMemBufFix + dwStart;

   // bsearch returns a pointer to an occurrence of key in the array pointed to by base. 
   // If key is not found, the function returns NULL. If the array is not in ascending sort order 
   // or contains duplicate records with identical keys, the result is unpredictable.
   DBF_REC_LINK*     pFound = (DBF_REC_LINK*)bsearch(&SrchRec,pBuf,dwCount,sizeof(DBF_REC_LINK),CMP_IndexFrom);

   if (!pFound)
   {
      return 0;
   }

   return pFound->_dwLinkFrom;
}

/* ******************************************************************** **
** @@ CTwins::UpdateMapper()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void CTwins::UpdateMapper(DWORD dwLinkTo,DWORD dwLinkFrom)
{
   if (!_pParent)
   {
      // Error !
      ASSERT(0);
      return;
   }

   ASSERT(_pParent->_pTableMapper);

   DWORD    dwRecSize = _pParent->_pTableMapper->_FixHeader._wRecSize;

   DBF_REC_MAP    SrchRec;

   memset(&SrchRec,0,sizeof(DBF_REC_MAP));

   SrchRec._dwLinkTo = dwLinkTo;

   DWORD    dwStart = _pParent->_pTableMapper->_FixHeader._wDataStart;
   DWORD    dwCount = _pParent->_pTableMapper->_FixHeader._dwRecCnt;

   BYTE*    pBuf = _pParent->_pTableMapper->_pMemBufFix + dwStart;

   // bsearch returns a pointer to an occurrence of key in the array pointed to by base. 
   // If key is not found, the function returns NULL. If the array is not in ascending sort order 
   // or contains duplicate records with identical keys, the result is unpredictable.
   DBF_REC_MAP*      pFound = (DBF_REC_MAP*)bsearch(&SrchRec,pBuf,dwCount,sizeof(DBF_REC_MAP),CMP_LinkTo);

   if (!pFound)
   {
      // Errot !
      ASSERT(0);
      return;
   }

   DBX_COLUMN*    pLinkFrom = _pParent->_pTableMapper->GetColumn("LINK FROM");

   ASSERT(pLinkFrom);

   // LinkTo always is SELF RecNum !
   const BYTE* const    pRecord = _pParent->_pTableMapper->Go(pFound->_dwLinkTo);

   ASSERT(pRecord);

   // Update Link From
   BYTE     _Record[MAX_PATH];

   memset(&_Record,0x20,MAX_PATH);

   memcpy(&_Record,pRecord,dwRecSize);

   ASSERT(pLinkFrom);

   pLinkFrom->Set(&_Record,&dwLinkFrom);

   _pParent->_pTableMapper->UpdateRecord(pFound->_dwLinkTo,(BYTE*)&_Record);
}

void CTwins::OnBtnFirst() 
{
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

void CTwins::OnBtnUp2() 
{
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

void CTwins::OnBtnPrev2() 
{
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

void CTwins::OnBtnNext2() 
{
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

void CTwins::OnBtnDn2() 
{
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

void CTwins::OnBtnLast2() 
{
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

void CTwins::OnBtnGo2() 
{
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}

void CTwins::OnChangeEdtGo2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
   // FixMe !
   AfxMessageBox("Haven't yet implemented !",MB_ICONSTOP | MB_OK);
}
