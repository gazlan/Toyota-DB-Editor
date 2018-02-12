#if !defined(AFX_TWINS_H__682A51A9_35BD_4096_9BEC_A56E633EC638__INCLUDED_)
#define AFX_TWINS_H__682A51A9_35BD_4096_9BEC_A56E633EC638__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum SYNC_MODE
{
   MODE_NONE,
   MODE_UP,
   MODE_DOWN,
   MODE_SYNC
};

#pragma pack(push,1)
struct DBF_REC_MAP
{
   BYTE        _byErased;
   DWORD       _dwLinkTo;
   DWORD       _dwLinkFrom;
};
#pragma pack(pop)


class CTwins : public CDialog
{
   private:
      
      bool*             _pChecksEN;
      bool*             _pChecksRU;
      bool              _bSelectedLeft;
      bool              _bSelectedRight;
      DWORD             _dwRecCnt;
      CImageList*       _pImageList;
      DWORD             _dwSyncPosLeft;
      DWORD             _dwSyncPosRight;

      CToyotaDBDlg*     _pParent;
            
   // Construction
   public:

   CTwins(CWnd* pParent = NULL);   // standard constructor

   void SetParent(CToyotaDBDlg* pParent);

// Dialog Data
   //{{AFX_DATA(CTwins)
   enum { IDD = IDD_TWINS };
   CButton  m_Dn;
   CButton  m_Up;
   CListCtrl   List_RU;
   CListCtrl   List_EN;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CTwins)
   public:

      virtual BOOL PreTranslateMessage(MSG* pMsg);

   protected:

      virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CTwins)
   afx_msg void OnDestroy();
   virtual BOOL OnInitDialog();
   afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnClickList2(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnRclickList2(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnGetdispinfoList1(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnGetdispinfoList2(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnKeydownList2(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnChangeEdtFindL();
   afx_msg void OnChangeEdtFindR();
   afx_msg void OnBtnPrevL();
   afx_msg void OnBtnNextL();
   afx_msg void OnBtnPrevR();
   afx_msg void OnBtnNextR();
   afx_msg void OnBtnSync();
   afx_msg void OnBtnUp();
   afx_msg void OnBtnDn();
	afx_msg void OnBtnFirst();
	afx_msg void OnBtnUp2();
	afx_msg void OnBtnPrev2();
	afx_msg void OnBtnNext2();
	afx_msg void OnBtnDn2();
	afx_msg void OnBtnLast2();
	afx_msg void OnBtnGo2();
	afx_msg void OnChangeEdtGo2();
	//}}AFX_MSG
   DECLARE_MESSAGE_MAP()

   private:
      
      void  GetTextColumn(DBX_TABLE* pTable,int iRecNum,const char* const pszTitle,CString& rsText);
      void  ToggleCheckBox1(int iIndex);
      void  ToggleCheckBox2(int iIndex);
      DWORD FindByIndex(DWORD dwIndexFrom);
      void  Enabler(int iIndex);
      void  UpdateMapper  (DWORD dwLinkTo,DWORD dwLinkFrom);
      void  ExchangeMapper(DWORD dwLinkTo,DWORD dwLinkFrom,int iIndexEN = -1,int iIndexRU = -1,SYNC_MODE Mode = MODE_NONE);
      void  ExchangeLink  (DWORD dwLinkTo,DWORD dwLinkFrom);
      void  SyncEnabler();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
