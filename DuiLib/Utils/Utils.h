#ifndef __UTILS_H__
#define __UTILS_H__

#pragma once
#include "OAIdl.h"
#include <vector>

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API STRINGorID {
	public:
		STRINGorID (LPCTSTR lpString): m_lpstr (lpString) {}
		STRINGorID (UINT nID): m_lpstr (MAKEINTRESOURCE (nID)) {}
		LPCTSTR m_lpstr;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CDuiPoint: public tagPOINT {
	public:
		CDuiPoint ();
		CDuiPoint (const POINT& src);
		CDuiPoint (int x, int y);
		CDuiPoint (LPARAM lParam);
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CDuiSize: public tagSIZE {
	public:
		CDuiSize ();
		CDuiSize (const SIZE& src);
		CDuiSize (const RECT rc);
		CDuiSize (int cx, int cy);
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CDuiRect: public tagRECT {
	public:
		CDuiRect ();
		CDuiRect (const RECT& src);
		CDuiRect (int iLeft, int iTop, int iRight, int iBottom);

		int GetWidth () const;
		int GetHeight () const;
		void Empty ();
		bool Isnullptr () const;
		void Join (const RECT& rc);
		void ResetOffset ();
		void Normalize ();
		void Offset (int cx, int cy);
		void Inflate (int cx, int cy);
		void Deflate (int cx, int cy);
		void Union (CDuiRect& rc);
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CStdPtrArray {
	public:
		CStdPtrArray (int iPreallocSize = 0);
		CStdPtrArray (const CStdPtrArray& src);
		virtual ~CStdPtrArray ();

		void Empty ();
		void Resize (int iSize);
		bool empty () const;
		int Find (LPVOID iIndex) const;
		bool Add (LPVOID pData);
		bool SetAt (int iIndex, LPVOID pData);
		bool InsertAt (int iIndex, LPVOID pData);
		bool Remove (int iIndex);
		int GetSize () const;
		LPVOID* GetData ();

		LPVOID GetAt (int iIndex) const;
		LPVOID operator[] (int nIndex) const;

	protected:
		LPVOID* m_ppVoid;
		int m_nCount;
		int m_nAllocated;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CStdValArray {
	public:
		CStdValArray (int iElementSize, int iPreallocSize = 0);
		virtual ~CStdValArray ();

		void Empty ();
		bool empty () const;
		bool Add (LPCVOID pData);
		bool Remove (int iIndex);
		int GetSize () const;
		LPVOID GetData ();

		LPVOID GetAt (int iIndex) const;
		LPVOID operator[] (int nIndex) const;

	protected:
		LPBYTE m_pVoid;
		int m_iElementSize;
		int m_nCount;
		int m_nAllocated;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CDuiString : public string_t {
	public:
		CDuiString ();
		CDuiString (const TCHAR ch);
		CDuiString (const string_t& src);
		CDuiString (LPCTSTR lpsz, int nLen = -1);
		inline operator LPCTSTR() const { return c_str (); }

		inline int Compare (LPCTSTR pstr) const { return _tcscmp (c_str (), pstr); }
		inline int CompareNoCase (LPCTSTR pstr) const { return _tcsicmp (c_str (), pstr); }

		inline void MakeUpper () { _tcsupr (&(*this)[0]); }
		inline void MakeLower () { _tcslwr (&(*this)[0]); }

		inline CDuiString Left (size_t nLength) const { return substr (0, nLength); }
		inline CDuiString Mid (size_t iPos, size_t nLength = string_t::npos) const { return substr (iPos, nLength); }
		inline CDuiString Right (size_t nLength) const { return substr (length () - nLength); }

		int Replace (LPCTSTR pstrFrom, LPCTSTR pstrTo);
		int Format (LPCTSTR pstrFormat, ...);
	};

	static std::vector<CDuiString> StrSplit (CDuiString text, CDuiString sp) {
		std::vector<CDuiString> vResults;
		size_t pos = text.find (sp);
		while (pos != string_t::npos) {
			CDuiString t = text.Left (pos);
			vResults.push_back (t);
			text = text.Right (text.length () - pos - sp.length ());
			pos = text.find (sp);
		}
		vResults.push_back (text);
		return vResults;
	}
	/////////////////////////////////////////////////////////////////////////////////////
	//

	struct TITEM {
		CDuiString Key;
		LPVOID Data;
		struct TITEM* pPrev;
		struct TITEM* pNext;
	};

	class UILIB_API CStdStringPtrMap {
	public:
		CStdStringPtrMap (int nSize = 83);
		virtual ~CStdStringPtrMap ();

		void Resize (int nSize = 83);
		LPVOID Find (LPCTSTR key, bool optimize = true) const;
		bool Insert (LPCTSTR key, LPVOID pData);
		LPVOID Set (LPCTSTR key, LPVOID pData);
		bool Remove (LPCTSTR key);
		void RemoveAll ();
		int GetSize () const;
		LPCTSTR GetAt (int iIndex) const;
		LPCTSTR operator[] (int nIndex) const;

	protected:
		TITEM** m_aT;
		int m_nBuckets;
		int m_nCount;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CWaitCursor {
	public:
		CWaitCursor ();
		virtual ~CWaitCursor ();

	protected:
		HCURSOR m_hOrigCursor;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class CDuiVariant: public VARIANT {
	public:
		CDuiVariant () {
			VariantInit (this);
		}
		CDuiVariant (int i) {
			VariantInit (this);
			this->vt = VT_I4;
			this->intVal = i;
		}
		CDuiVariant (float f) {
			VariantInit (this);
			this->vt = VT_R4;
			this->fltVal = f;
		}
		CDuiVariant (LPOLESTR s) {
			VariantInit (this);
			this->vt = VT_BSTR;
			this->bstrVal = s;
		}
		CDuiVariant (IDispatch *disp) {
			VariantInit (this);
			this->vt = VT_DISPATCH;
			this->pdispVal = disp;
		}

		virtual ~CDuiVariant () {
			VariantClear (this);
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////
	//
	static char* w2a (wchar_t* lpszSrc, UINT   CodePage = CP_ACP) {
		if (lpszSrc != nullptr) {
			int  nANSILen = WideCharToMultiByte (CodePage, 0, lpszSrc, -1, nullptr, 0, nullptr, nullptr);
			char* pANSI = new char[nANSILen + 1];
			if (pANSI != nullptr) {
				ZeroMemory (pANSI, nANSILen + 1);
				WideCharToMultiByte (CodePage, 0, lpszSrc, -1, pANSI, nANSILen, nullptr, nullptr);
				return pANSI;
			}
		}
		return nullptr;
	}

	static wchar_t* a2w (char* lpszSrc, UINT   CodePage = CP_ACP) {
		if (lpszSrc != nullptr) {
			int nUnicodeLen = MultiByteToWideChar (CodePage, 0, lpszSrc, -1, nullptr, 0);
			LPWSTR pUnicode = new WCHAR[nUnicodeLen + 1];
			if (pUnicode != nullptr) {
				ZeroMemory ((void*) pUnicode, (nUnicodeLen + 1) * sizeof (WCHAR));
				MultiByteToWideChar (CodePage, 0, lpszSrc, -1, pUnicode, nUnicodeLen);
				return pUnicode;
			}
		}
		return nullptr;
	}

	///////////////////////////////////////////////////////////////////////////////////////
	////
	//struct TImageInfo;
	//class CPaintManagerUI;
	//class UILIB_API CImageString
	//{
	//public:
	//	CImageString();
	//	CImageString(const CImageString&);
	//	const CImageString& operator=(const CImageString&);
	//	virtual ~CImageString();

	//	const CDuiString& GetAttributeString() const;
	//	void SetAttributeString(LPCTSTR pStrImageAttri);
	//	void ModifyAttribute(LPCTSTR pStrModify);
	//	bool LoadImage(CPaintManagerUI* pManager);
	//	bool IsLoadSuccess();

	//	RECT GetDest() const;
	//	void SetDest(const RECT &rcDest);
	//	const TImageInfo* GetImageInfo() const;

	//private:
	//	void Clone(const CImageString&);
	//	void Clear();
	//	void ParseAttribute(LPCTSTR pStrImageAttri);

	//protected:
	//	friend class CRenderEngine;
	//	CDuiString	m_sImageAttribute;

	//	CDuiString	m_sImage;
	//	CDuiString	m_sResType;
	//	TImageInfo	*m_imageInfo;
	//	bool		m_bLoadSuccess;

	//	RECT	m_rcDest;
	//	RECT	m_rcSource;
	//	RECT	m_rcCorner;
	//	BYTE	m_bFade;
	//	DWORD	m_dwMask;
	//	bool	m_bHole;
	//	bool	m_bTiledX;
	//	bool	m_bTiledY;
	//};
}// namespace DuiLib

#endif // __UTILS_H__