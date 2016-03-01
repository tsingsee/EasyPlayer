#ifndef __UIMARKUP_H__
#define __UIMARKUP_H__

#include "SkinUI.h"

#pragma once


enum
{
    XMLFILE_ENCODING_UTF8 = 1,
    XMLFILE_ENCODING_UNICODE = 2,
    XMLFILE_ENCODING_ASNI = 3,
};

class CXmlManager;
class CXmlNode;


class StringOrID
{
public:
	StringOrID(LPCTSTR lpString) : m_lpstr(lpString)
	{ }
	StringOrID(UINT nID) : m_lpstr(MAKEINTRESOURCE(nID))
	{ }
	LPCTSTR m_lpstr;
};


class SKINUI_CLASS CXmlManager
{
    friend CXmlNode;
public:
    CXmlManager(LPCTSTR pstrXML = NULL);
    ~CXmlManager();

    bool Load(LPCTSTR pstrXML);
    bool LoadFromMem(BYTE* pByte, DWORD dwSize, int encoding = XMLFILE_ENCODING_UTF8);
    bool LoadFromFile(LPCTSTR pstrFilename, int encoding = XMLFILE_ENCODING_UTF8);
    void Release();
    bool IsValid() const;

    void SetPreserveWhitespace(bool bPreserve = true);
    void GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const;
    void GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const;

    CXmlNode GetRoot();

private:
    typedef struct tagXMLELEMENT
    {
        ULONG iStart;
        ULONG iChild;
        ULONG iNext;
        ULONG iParent;
        ULONG iData;
    } XMLELEMENT;

    LPTSTR m_pstrXML;
    XMLELEMENT* m_pElements;
    ULONG m_nElements;
    ULONG m_nReservedElements;
    TCHAR m_szErrorMsg[100];
    TCHAR m_szErrorXML[50];
    bool m_bPreserveWhitespace;

private:
    bool _Parse();
    bool _Parse(LPTSTR& pstrText, ULONG iParent);
    XMLELEMENT* _ReserveElement();
    inline void _SkipWhitespace(LPTSTR& pstr) const;
    inline void _SkipWhitespace(LPCTSTR& pstr) const;
    inline void _SkipIdentifier(LPTSTR& pstr) const;
    inline void _SkipIdentifier(LPCTSTR& pstr) const;
    bool _ParseData(LPTSTR& pstrText, LPTSTR& pstrData, char cEnd);
    void _ParseMetaChar(LPTSTR& pstrText, LPTSTR& pstrDest);
    bool _ParseAttributes(LPTSTR& pstrText);
    bool _Failed(LPCTSTR pstrError, LPCTSTR pstrLocation = NULL);
};


class SKINUI_CLASS CXmlNode
{
    friend CXmlManager;
private:
    CXmlNode();
    CXmlNode(CXmlManager* pOwner, int iPos);

public:
    bool IsValid() const;

    CXmlNode GetParent();
    CXmlNode GetSibling();
    CXmlNode GetChild();
    CXmlNode GetChild(LPCTSTR pstrName);

    bool HasSiblings() const;
    bool HasChildren() const;
    LPCTSTR GetName() const;
    LPCTSTR GetValue() const;

    bool HasAttributes();
    bool HasAttribute(LPCTSTR pstrName);
    int GetAttributeCount();
    LPCTSTR GetAttributeName(int iIndex);
    LPCTSTR GetAttributeValue(int iIndex);
    LPCTSTR GetAttributeValue(LPCTSTR pstrName);
    bool GetAttributeValue(int iIndex, LPTSTR pstrValue, SIZE_T cchMax);
    bool GetAttributeValue(LPCTSTR pstrName, LPTSTR pstrValue, SIZE_T cchMax);

private:
    void _MapAttributes();

    enum { MAX_XML_ATTRIBUTES = 64 };

    typedef struct
    {
        ULONG iName;
        ULONG iValue;
    } XMLATTRIBUTE;

    int m_iPos;
    int m_nAttributes;
    XMLATTRIBUTE m_aAttributes[MAX_XML_ATTRIBUTES];
    CXmlManager* m_pOwner;
};


#endif // __UIMARKUP_H__
