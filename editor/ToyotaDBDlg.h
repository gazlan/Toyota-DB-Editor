#if !defined(AFX_TOYOTADBDLG_H__28D409ED_490F_4DFF_A203_A7FD39766B70__INCLUDED_)
#define AFX_TOYOTADBDLG_H__28D409ED_490F_4DFF_A203_A7FD39766B70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FILE_10_M_EN                            "M_ENGLISH_10"
#define FILE_DBF_10_M_EN                        ".\\DB\\M_English_10.dbf"
#define FILE_DBV_10_M_EN                        ".\\DB\\M_English_10.dbv"
#define FILE_10_M_RU                            "M_RUSSIAN_10"
#define FILE_DBF_10_M_RU                        ".\\DB\\M_Russian_10.dbf"
#define FILE_10_M_LINK                          "M_LINK_10"
#define FILE_DBF_10_M_LINK                      ".\\DB\\M_Link_10.dbf"
#define FILE_DBV_10_M_LINK                      ".\\DB\\M_Link_10.dbv"
#define FILE_10_M_MAPPER                        "M_MAPPER_10"
#define FILE_DBF_10_M_MAPPER                    ".\\DB\\M_Mapper_10.dbf"

#define FILE_10_U_EN                            "U_ENGLISH_10"
#define FILE_DBF_10_U_EN                        ".\\DB\\U_English_10.dbf"
#define FILE_DBV_10_U_EN                        ".\\DB\\U_English_10.dbv"
#define FILE_10_U_RU                            "U_RUSSIAN_10"
#define FILE_DBF_10_U_RU                        ".\\DB\\U_Russian_10.dbf"
#define FILE_10_U_LINK                          "U_LINK_10"
#define FILE_DBF_10_U_LINK                      ".\\DB\\U_Link_10.dbf"
#define FILE_DBV_10_U_LINK                      ".\\DB\\U_Link_10.dbv"
#define FILE_10_U_MAPPER                        "U_MAPPER_10"
#define FILE_DBF_10_U_MAPPER                    ".\\DB\\U_Mapper_10.dbf"

#define FILE_10_V_EN                            "V_ENGLISH_10"
#define FILE_DBF_10_V_EN                        ".\\DB\\V_English_10.dbf"
#define FILE_DBV_10_V_EN                        ".\\DB\\V_English_10.dbv"
#define FILE_10_V_RU                            "V_RUSSIAN_10"
#define FILE_DBF_10_V_RU                        ".\\DB\\V_Russian_10.dbf"
#define FILE_10_V_LINK                          "V_LINK_10"
#define FILE_DBF_10_V_LINK                      ".\\DB\\V_Link_10.dbf"
#define FILE_DBV_10_V_LINK                      ".\\DB\\V_Link_10.dbv"
#define FILE_10_V_MAPPER                        "V_MAPPER_10"
#define FILE_DBF_10_V_MAPPER                    ".\\DB\\V_Mapper_10.dbf"

#define DIX_ER_TECHNICAL                        "DIX_ER_TECHNICAL"
#define DIX_ER_TECHNICAL_DBF                    ".\\DB\\er_technical.dbf"
#define DIX_ER_TECHNICAL_DBV                    ".\\DB\\er_technical.dbv"

#define DB_QUANTA                         (128)    // PageSize

enum DDB_TABLE
{
   DT_M_ENGLISH_10,
   DT_U_ENGLISH_10,
   DT_V_ENGLISH_10,
   DT_NO_TABLE       // Should be Last Not Existing Entry - For Force Load
};

enum DBV_FLAGS
{
   DBV_FLAG_NONE       = 0,
   DBV_FLAG_TRANLATED  = 0x01,
   DBV_FLAG_VERIFIED   = 0x02,
   DBV_FLAG_MARKED     = 0x04,
   DBV_FLAG_COMMENTED  = 0x08,
   DBV_FLAG_CHANGED    = 0x10,
   DBV_FLAG_DUPLICATED = 0x20
};
 
#pragma pack(push,1)
struct DBF_REC_RU
{
   BYTE        _byErased;
   DWORD       _dwIndexFrom;
   DWORD       _dwLinkFrom;
};
#pragma pack(pop)

#pragma pack(push,1)
struct DBF_REC_LINK
{
   BYTE        _byErased;
   DWORD       _dwRecNum;
   DWORD       _dwIndexFrom;
   DWORD       _dwLinkFrom;
   DWORD       _dwLinkTo;
   DWORD       _dwLinkCnt;
   DWORD       _dwText;
   DWORD       _dwComment;
   BYTE        _byFlags;
};
#pragma pack(pop)

struct DBV_RECORD_EN
{
   DWORD       _dwIndex;
   DWORD       _dwText;
   char*       _pszText;
   DWORD       _dwTextSize;
};

struct DBV_RECORD_RU
{
   DWORD       _dwIndex;
   DWORD       _dwText;
   char*       _pszText;
   DWORD       _dwTextSize;
   DWORD       _dwCmnt;
   char*       _pszComment;
   DWORD       _dwCommentSize;
   DWORD*      _pArray;
   DWORD       _dwArraySize;
   BYTE        _byFlags;
   bool        _bDirtyFlags;
   bool        _bDirtyTexts;
};

/* ******************************************************************** **
** @@ class CToyotaDBDlg : public CDialog
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

class CToyotaDBDlg : public CDialog
{
   private:

      CSystemTray*         _pTray;
      DBX*                 _pDBX;
      DBX_TABLE*           _pTechnical;
      DDB_TABLE            _Table;
      bool                 _bForceLoad;
      bool                 _bExit;

   public:

      DBX_TABLE*        _pTableEN;
      DBX_TABLE*        _pTableRU;
      DBX_TABLE*        _pTableLink;
      DBX_TABLE*        _pTableMapper;
      CString           _sTableEN;
      CString           _sTableFixEN;
      CString           _sTableVarEN;
      CString           _sTableRU;
      CString           _sTableFixRU;
      CString           _sTableLink;
      CString           _sTableLinkFix;
      CString           _sTableLinkVar;
      CString           _sTableMapper;
      CString           _sTableMapperFix;
      CString           _sOriginal;
      CFont             _FontEN;
      int               _iRecTotal;
      int               _iCurRec;   // Should be int !
      int               _iPageSize;

   // Construction
   public:

   CToyotaDBDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
   //{{AFX_DATA(CToyotaDBDlg)
	enum { IDD = IDD_TOYOTADB };
   CEdit m_Original;
   CListCtrl   m_List;
   CComboBox   m_CMB_Table;
   CString  m_Index;
   BOOL  m_Translated;
   BOOL  m_Verified;
   BOOL  m_Marked;
   CString  _sComment;
   CString  _sGo;
   CString  _sSrc;
   CString  _sDst;
   CString  m_Translation;
	CString	m_Block;
	//}}AFX_DATA

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CToyotaDBDlg)
   public:
   
      virtual BOOL PreTranslateMessage(MSG* pMsg);

   protected:

      virtual void DoDataExchange(CDataExchange* pDX);   // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   HICON m_hIcon;

   // Generated message map functions
   //{{AFX_MSG(CToyotaDBDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnSysCommand(UINT nID,LPARAM lParam);
   afx_msg void OnPaint();
   afx_msg HCURSOR OnQueryDragIcon();
   afx_msg void OnChkTranslated();
   afx_msg void OnChkVerified();
   afx_msg void OnChkMark();
   afx_msg void OnBtnFirst();
   afx_msg void OnBtnUp();
   afx_msg void OnBtnPrev();
   afx_msg void OnBtnNext();
   afx_msg void OnBtnDn();
   afx_msg void OnBtnLast();
   afx_msg void OnBtnGo();
   virtual void OnCancel();
   afx_msg void OnDestroy();
   afx_msg void OnCloseupCmbTable();
   afx_msg void OnBtnUpdate();
   afx_msg void OnBtnSearch();
   afx_msg void OnBtnSomePrev();
   afx_msg void OnBtnSomeNext();
   afx_msg void OnBtnUpdateAll();
   afx_msg void OnClickRedtTranslation(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnChangeEdtOriginal();
   afx_msg void OnClickListDix(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnDblclkListDix(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnRclickListDix(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnGetdispinfoListDix(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnChangeEdtComment();
   afx_msg void OnBtnCompact();
   afx_msg void OnBtnTwins();
   afx_msg LRESULT OnTrayNotification(WPARAM wParam,LPARAM lParam);
   afx_msg void OnClose();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()

   private:

      void  Cleanup();
      void  Load();
      void  CheckModifications();
      bool  SaveFlags();
      bool  SaveComment();
      bool  SaveText();
      bool  SetCommented(bool bSet);
      bool  SetChanged(bool bSet);
      void  Locate();
      int   DB_RetrieveCurRec();
      int   DB_GetMaxRecNum();
      void  LoadDix_Technical();
      DWORD AppendVar(const BYTE* const pText,DWORD dwSize);
      bool  DeleteVar(DWORD dwPos);
      void  SearchTitle();
      void  GetTextColumn(DBX_TABLE* pTable,int iRecNum,const char* const pszTitle,CString& rsText);
      bool  UpdateMemo(const char* const pszTitle,const BYTE* const pBuf,DWORD dwSize);
 
      void  Update
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
      );
      
      bool  UpdateFix
      (
         DBX_TABLE*        pTable,
         DWORD             dwIndex,
         DWORD             dwTextPos,
         DWORD             dwCmntPos,
         DWORD             dwArrayPos,
         BYTE              byFlags
      );
   
   DWORD    FindIndexByLink(DWORD dwLink);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOYOTADBDLG_H__28D409ED_490F_4DFF_A203_A7FD39766B70__INCLUDED_)
