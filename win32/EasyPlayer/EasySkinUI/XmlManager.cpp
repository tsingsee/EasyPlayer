
#include "StdAfx.h"
#include "XmlManager.h"
#ifndef TRACE
#define TRACE
#endif

#pragma warning(disable: 4244)
///////////////////////////////////////////////////////////////////////////////////////
//
//
//

CXmlNode::CXmlNode() : m_pOwner(NULL)
{
}

CXmlNode::CXmlNode(CXmlManager* pOwner, int iPos) : m_pOwner(pOwner), m_iPos(iPos), m_nAttributes(0)
{
}

CXmlNode CXmlNode::GetSibling()
{
    if( m_pOwner == NULL ) return CXmlNode();
    ULONG iPos = m_pOwner->m_pElements[m_iPos].iNext;
    if( iPos == 0 ) return CXmlNode();
    return CXmlNode(m_pOwner, iPos);
}

bool CXmlNode::HasSiblings() const
{
    if( m_pOwner == NULL ) return false;
    ULONG iPos = m_pOwner->m_pElements[m_iPos].iNext;
    return iPos > 0;
}

CXmlNode CXmlNode::GetChild()
{
    if( m_pOwner == NULL ) return CXmlNode();
    ULONG iPos = m_pOwner->m_pElements[m_iPos].iChild;
    if( iPos == 0 ) return CXmlNode();
    return CXmlNode(m_pOwner, iPos);
}

CXmlNode CXmlNode::GetChild(LPCTSTR pstrName)
{
    if( m_pOwner == NULL ) return CXmlNode();
    ULONG iPos = m_pOwner->m_pElements[m_iPos].iChild;
    while( iPos != 0 ) 
	{
        if( _tcscmp(m_pOwner->m_pstrXML + m_pOwner->m_pElements[iPos].iStart, pstrName) == 0 )
		{
            return CXmlNode(m_pOwner, iPos);
        }
        iPos = m_pOwner->m_pElements[iPos].iNext;
    }
    return CXmlNode();
}

bool CXmlNode::HasChildren() const
{
    if( m_pOwner == NULL ) return false;
    return m_pOwner->m_pElements[m_iPos].iChild != 0;
}

CXmlNode CXmlNode::GetParent()
{
    if( m_pOwner == NULL ) return CXmlNode();
    ULONG iPos = m_pOwner->m_pElements[m_iPos].iParent;
    if( iPos == 0 ) return CXmlNode();
    return CXmlNode(m_pOwner, iPos);
}

bool CXmlNode::IsValid() const
{
    return m_pOwner != NULL;
}

LPCTSTR CXmlNode::GetName() const
{
    if( m_pOwner == NULL ) return NULL;
    return m_pOwner->m_pstrXML + m_pOwner->m_pElements[m_iPos].iStart;
}

LPCTSTR CXmlNode::GetValue() const
{
    if( m_pOwner == NULL ) return NULL;
    return m_pOwner->m_pstrXML + m_pOwner->m_pElements[m_iPos].iData;
}

LPCTSTR CXmlNode::GetAttributeName(int iIndex)
{
    if( m_pOwner == NULL ) return NULL;
    if( m_nAttributes == 0 ) _MapAttributes();
    if( iIndex < 0 || iIndex >= m_nAttributes ) return _T("");
    return m_pOwner->m_pstrXML + m_aAttributes[iIndex].iName;
}

LPCTSTR CXmlNode::GetAttributeValue(int iIndex)
{
    if( m_pOwner == NULL ) return NULL;
    if( m_nAttributes == 0 ) _MapAttributes();
    if( iIndex < 0 || iIndex >= m_nAttributes ) return _T("");
    return m_pOwner->m_pstrXML + m_aAttributes[iIndex].iValue;
}

LPCTSTR CXmlNode::GetAttributeValue(LPCTSTR pstrName)
{
    if( m_pOwner == NULL ) return NULL;
    if( m_nAttributes == 0 ) _MapAttributes();
    for( int i = 0; i < m_nAttributes; i++ ) 
	{
        if( _tcscmp(m_pOwner->m_pstrXML + m_aAttributes[i].iName, pstrName) == 0 ) return m_pOwner->m_pstrXML + m_aAttributes[i].iValue;
    }
    return _T("");
}

bool CXmlNode::GetAttributeValue(int iIndex, LPTSTR pstrValue, SIZE_T cchMax)
{
    if( m_pOwner == NULL ) return false;
    if( m_nAttributes == 0 ) _MapAttributes();
    if( iIndex < 0 || iIndex >= m_nAttributes ) return false;
    _tcsncpy_s(pstrValue,-1, m_pOwner->m_pstrXML + m_aAttributes[iIndex].iValue, cchMax);
    return true;
}

bool CXmlNode::GetAttributeValue(LPCTSTR pstrName, LPTSTR pstrValue, SIZE_T cchMax)
{
    if( m_pOwner == NULL ) return false;
    if( m_nAttributes == 0 ) _MapAttributes();
    for( int i = 0; i < m_nAttributes; i++ )
	{
        if( _tcscmp(m_pOwner->m_pstrXML + m_aAttributes[i].iName, pstrName) == 0 )
		{
            _tcsncpy_s(pstrValue,-1, m_pOwner->m_pstrXML + m_aAttributes[i].iValue, cchMax);
            return true;
        }
    }
    return false;
}

int CXmlNode::GetAttributeCount()
{
    if( m_pOwner == NULL ) return 0;
    if( m_nAttributes == 0 ) _MapAttributes();
    return m_nAttributes;
}

bool CXmlNode::HasAttributes()
{
    if( m_pOwner == NULL ) return false;
    if( m_nAttributes == 0 ) _MapAttributes();
    return m_nAttributes > 0;
}

bool CXmlNode::HasAttribute(LPCTSTR pstrName)
{
    if( m_pOwner == NULL ) return false;
    if( m_nAttributes == 0 ) _MapAttributes();
    for( int i = 0; i < m_nAttributes; i++ ) 
	{
        if( _tcscmp(m_pOwner->m_pstrXML + m_aAttributes[i].iName, pstrName) == 0 ) return true;
    }
    return false;
}

void CXmlNode::_MapAttributes()
{
    m_nAttributes = 0;
    LPCTSTR pstr = m_pOwner->m_pstrXML + m_pOwner->m_pElements[m_iPos].iStart;
    LPCTSTR pstrEnd = m_pOwner->m_pstrXML + m_pOwner->m_pElements[m_iPos].iData;
    pstr += _tcslen(pstr) + 1;
    while( pstr < pstrEnd ) 
	{
        m_pOwner->_SkipWhitespace(pstr);
        m_aAttributes[m_nAttributes].iName = pstr - m_pOwner->m_pstrXML;
        pstr += _tcslen(pstr) + 1;
        m_pOwner->_SkipWhitespace(pstr);
        if( *pstr++ != _T('\"') ) return; // if( *pstr != _T('\"') ) { pstr = ::CharNext(pstr); return; }
        
        m_aAttributes[m_nAttributes++].iValue = pstr - m_pOwner->m_pstrXML;
        if( m_nAttributes >= MAX_XML_ATTRIBUTES ) return;
        pstr += _tcslen(pstr) + 1;
    }
}


///////////////////////////////////////////////////////////////////////////////////////
//
//
//

CXmlManager::CXmlManager(LPCTSTR pstrXML)
{
    m_pstrXML = NULL;
    m_pElements = NULL;
    m_nElements = 0;
    m_bPreserveWhitespace = true;
    if( pstrXML != NULL ) Load(pstrXML);
}

CXmlManager::~CXmlManager()
{
    Release();
}

bool CXmlManager::IsValid() const
{
    return m_pElements != NULL;
}

void CXmlManager::SetPreserveWhitespace(bool bPreserve)
{
    m_bPreserveWhitespace = bPreserve;
}

bool CXmlManager::Load(LPCTSTR pstrXML)
{
    Release();
    SIZE_T cchLen = _tcslen(pstrXML) + 1;
    m_pstrXML = static_cast<LPTSTR>(malloc(cchLen * sizeof(TCHAR)));
    ::CopyMemory(m_pstrXML, pstrXML, cchLen * sizeof(TCHAR));
    bool bRes = _Parse();
    if( !bRes ) Release();
    return bRes;
}

bool CXmlManager::LoadFromMem(BYTE* pByte, DWORD dwSize, int encoding)
{
#ifdef _UNICODE
    if (encoding == XMLFILE_ENCODING_UTF8)
    {
        DWORD nWide = ::MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)pByte, dwSize, NULL, 0 );

        m_pstrXML = static_cast<LPTSTR>(malloc((nWide + 1)*sizeof(TCHAR)));
        ::MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)pByte, dwSize, m_pstrXML, nWide );
        m_pstrXML[nWide] = _T('\0');
    }
    else if (encoding == XMLFILE_ENCODING_ASNI)
    {
        DWORD nWide = ::MultiByteToWideChar( CP_ACP, 0, (LPCSTR)pByte, dwSize, NULL, 0 );

        m_pstrXML = static_cast<LPTSTR>(malloc((nWide + 1)*sizeof(TCHAR)));
        ::MultiByteToWideChar( CP_ACP, 0, (LPCSTR)pByte, dwSize, m_pstrXML, nWide );
        m_pstrXML[nWide] = _T('\0');
    }
    else
    {
        if ( dwSize >= 2 && ( ( pByte[0] == 0xFE && pByte[1] == 0xFF ) || ( pByte[0] == 0xFF && pByte[1] == 0xFE ) ) )
        {
            dwSize = dwSize / 2 - 1;

            if ( pByte[0] == 0xFE && pByte[1] == 0xFF )
            {
                pByte += 2;

                for ( DWORD nSwap = 0 ; nSwap < dwSize ; nSwap ++ )
                {
                    register CHAR nTemp = pByte[ ( nSwap << 1 ) + 0 ];
                    pByte[ ( nSwap << 1 ) + 0 ] = pByte[ ( nSwap << 1 ) + 1 ];
                    pByte[ ( nSwap << 1 ) + 1 ] = nTemp;
                }
            }
            else
            {
                pByte += 2;
            }

            m_pstrXML = static_cast<LPTSTR>(malloc((dwSize + 1)*sizeof(TCHAR)));
            ::CopyMemory( m_pstrXML, pByte, dwSize * sizeof(TCHAR) );
            m_pstrXML[dwSize] = _T('\0');

            pByte -= 2;
        }
    }
#else // !_UNICODE
    if (encoding == XMLFILE_ENCODING_UTF8)
    {
        DWORD nWide = ::MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)pByte, dwSize, NULL, 0 );

        LPWSTR w_str = static_cast<LPWSTR>(malloc((nWide + 1)*sizeof(WCHAR)));
        ::MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)pByte, dwSize, w_str, nWide );
        w_str[nWide] = L'\0';

        DWORD wide = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)w_str, nWide, NULL, 0, NULL, NULL);

        m_pstrXML = static_cast<LPTSTR>(malloc((wide + 1)*sizeof(TCHAR)));
        ::WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)w_str, nWide, m_pstrXML, wide, NULL, NULL);
        m_pstrXML[wide] = _T('\0');

        free(w_str);
    }
    else if (encoding == XMLFILE_ENCODING_UNICODE)
    {
        if ( dwSize >= 2 && ( ( pByte[0] == 0xFE && pByte[1] == 0xFF ) || ( pByte[0] == 0xFF && pByte[1] == 0xFE ) ) )
        {
            dwSize = dwSize / 2 - 1;

            if ( pByte[0] == 0xFE && pByte[1] == 0xFF )
            {
                pByte += 2;

                for ( DWORD nSwap = 0 ; nSwap < dwSize ; nSwap ++ )
                {
                    register CHAR nTemp = pByte[ ( nSwap << 1 ) + 0 ];
                    pByte[ ( nSwap << 1 ) + 0 ] = pByte[ ( nSwap << 1 ) + 1 ];
                    pByte[ ( nSwap << 1 ) + 1 ] = nTemp;
                }
            }
            else
            {
                pByte += 2;
            }

            DWORD nWide = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pByte, dwSize, NULL, 0, NULL, NULL);
            m_pstrXML = static_cast<LPTSTR>(malloc((nWide + 1)*sizeof(TCHAR)));
            ::WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)pByte, dwSize, m_pstrXML, nWide, NULL, NULL);
            m_pstrXML[nWide] = _T('\0');

            pByte -= 2;
        }
    }
    else
    {
        m_pstrXML = static_cast<LPTSTR>(malloc((dwSize + 1)*sizeof(TCHAR)));
        ::CopyMemory( m_pstrXML, pByte, dwSize * sizeof(TCHAR) );
        m_pstrXML[dwSize] = _T('\0');
    }
#endif // _UNICODE

    bool bRes = _Parse();
    if( !bRes ) Release();
    return bRes;
}

bool CXmlManager::LoadFromFile(LPCTSTR pstrFilename, int encoding)
{
	ASSERT(pstrFilename);
    Release();
    HANDLE hFile = ::CreateFile(pstrFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if( hFile == INVALID_HANDLE_VALUE ) return _Failed(_T("打开文件失败"));
    DWORD dwSize = ::GetFileSize(hFile, NULL);
    if( dwSize == 0 ) return _Failed(_T("File is empty"));
    if ( dwSize > 4096*1024 ) return _Failed(_T("File too large"));

    DWORD dwRead = 0;
    BYTE* pByte = new BYTE[ dwSize ];
    ::ReadFile( hFile, pByte, dwSize, &dwRead, NULL );
    ::CloseHandle( hFile );

    if( dwRead != dwSize ) 
	{
        delete[] pByte;
        Release();
        return _Failed(_T("Could not read file"));
    }

    bool ret = LoadFromMem(pByte, dwSize, encoding);
    delete[] pByte;
    return ret;
}

void CXmlManager::Release()
{
    if( m_pstrXML != NULL ) free(m_pstrXML);
    if( m_pElements != NULL ) free(m_pElements);
    m_pstrXML = NULL;
    m_pElements = NULL;
    m_nElements;
}

void CXmlManager::GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const
{
    _tcsncpy_s(pstrMessage, -1,m_szErrorMsg, cchMax);
}

void CXmlManager::GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const
{
    _tcsncpy_s(pstrSource, -1,m_szErrorXML, cchMax);
}

CXmlNode CXmlManager::GetRoot()
{
    if( m_nElements == 0 ) return CXmlNode();
    return CXmlNode(this, 1);
}

bool CXmlManager::_Parse()
{
    _ReserveElement(); // Reserve index 0 for errors
    ::ZeroMemory(m_szErrorMsg, sizeof(m_szErrorMsg));
    ::ZeroMemory(m_szErrorXML, sizeof(m_szErrorXML));
    LPTSTR pstrXML = m_pstrXML;
    return _Parse(pstrXML, 0);
}

bool CXmlManager::_Parse(LPTSTR& pstrText, ULONG iParent)
{
    _SkipWhitespace(pstrText);
    ULONG iPrevious = 0;
    for( ; ; ) 
    {
        if( *pstrText == _T('\0') && iParent <= 1 ) return true;
        _SkipWhitespace(pstrText);
        if( *pstrText != _T('<') ) return _Failed(_T("Expected start tag"), pstrText);
        if( pstrText[1] == _T('/') ) return true;
        *pstrText++ = _T('\0');
        _SkipWhitespace(pstrText);
        // Skip comment or processing directive
        if( *pstrText == _T('!') || *pstrText == _T('?') )
		{
            TCHAR ch = *pstrText;
            if( *pstrText == _T('!') ) ch = _T('-');
            while( *pstrText != _T('\0') && !(*pstrText == ch && *(pstrText + 1) == _T('>')) ) pstrText = ::CharNext(pstrText);
            if( *pstrText != _T('\0') ) pstrText += 2;
            _SkipWhitespace(pstrText);
            continue;
        }
        _SkipWhitespace(pstrText);
        // Fill out element structure
        XMLELEMENT* pEl = _ReserveElement();
        ULONG iPos = pEl - m_pElements;
        pEl->iStart = pstrText - m_pstrXML;
        pEl->iParent = iParent;
        pEl->iNext = pEl->iChild = 0;
        if( iPrevious != 0 ) m_pElements[iPrevious].iNext = iPos;
        else if( iParent > 0 ) m_pElements[iParent].iChild = iPos;
        iPrevious = iPos;
        // Parse name
        LPCTSTR pstrName = pstrText;
        _SkipIdentifier(pstrText);
        LPTSTR pstrNameEnd = pstrText;
        if( *pstrText == _T('\0') ) return _Failed(_T("Error parsing element name"), pstrText);
        // Parse attributes
        if( !_ParseAttributes(pstrText) ) return false;
        _SkipWhitespace(pstrText);
        if( pstrText[0] == _T('/') && pstrText[1] == _T('>') )
        {
            pEl->iData = pstrText - m_pstrXML;
            *pstrText = _T('\0');
            pstrText += 2;
        }
        else
        {
            if( *pstrText != _T('>') ) return _Failed(_T("Expected start-tag closing"), pstrText);
            // Parse node data
            pEl->iData = ++pstrText - m_pstrXML;
            LPTSTR pstrDest = pstrText;
            if( !_ParseData(pstrText, pstrDest, _T('<')) ) return false;
            // Determine type of next element
            if( *pstrText == _T('\0') && iParent <= 1 ) return true;
            if( *pstrText != _T('<') ) return _Failed(_T("Expected end-tag start"), pstrText);
            if( pstrText[0] == _T('<') && pstrText[1] != _T('/') ) 
            {
                if( !_Parse(pstrText, iPos) ) return false;
            }
            if( pstrText[0] == _T('<') && pstrText[1] == _T('/') ) 
            {
                *pstrDest = _T('\0');
                *pstrText = _T('\0');
                pstrText += 2;
                _SkipWhitespace(pstrText);
                SIZE_T cchName = pstrNameEnd - pstrName;
                if( _tcsncmp(pstrText, pstrName, cchName) != 0 ) return _Failed(_T("Unmatched closing tag"), pstrText);
                pstrText += cchName;
                _SkipWhitespace(pstrText);
                if( *pstrText++ != _T('>') ) return _Failed(_T("Unmatched closing tag"), pstrText);
            }
        }
        *pstrNameEnd = _T('\0');
        _SkipWhitespace(pstrText);
    }
}

CXmlManager::XMLELEMENT* CXmlManager::_ReserveElement()
{
    if( m_nElements == 0 ) m_nReservedElements = 0;
    if( m_nElements >= m_nReservedElements ) {
        m_nReservedElements += (m_nReservedElements / 2) + 500;
        m_pElements = static_cast<XMLELEMENT*>(realloc(m_pElements, m_nReservedElements * sizeof(XMLELEMENT)));
    }
    return &m_pElements[m_nElements++];
}

void CXmlManager::_SkipWhitespace(LPCTSTR& pstr) const
{
    while( *pstr > _T('\0') && *pstr <= _T(' ') ) pstr = ::CharNext(pstr);
}

void CXmlManager::_SkipWhitespace(LPTSTR& pstr) const
{
    while( *pstr > _T('\0') && *pstr <= _T(' ') ) pstr = ::CharNext(pstr);
}

void CXmlManager::_SkipIdentifier(LPCTSTR& pstr) const
{
    // 属性只能用英文，所以这样处理没有问题
    while( *pstr != _T('\0') && (*pstr == _T('_') || *pstr == _T(':') || _istalnum(*pstr)) ) pstr = ::CharNext(pstr);
}

void CXmlManager::_SkipIdentifier(LPTSTR& pstr) const
{
    // 属性只能用英文，所以这样处理没有问题
    while( *pstr != _T('\0') && (*pstr == _T('_') || *pstr == _T(':') || _istalnum(*pstr)) ) pstr = ::CharNext(pstr);
}

bool CXmlManager::_ParseAttributes(LPTSTR& pstrText)
{   
    if( *pstrText == _T('>') ) return true;
    *pstrText++ = _T('\0');
    _SkipWhitespace(pstrText);
    while( *pstrText != _T('\0') && *pstrText != _T('>') && *pstrText != _T('/') ) 
	{
        _SkipIdentifier(pstrText);
        LPTSTR pstrIdentifierEnd = pstrText;
        _SkipWhitespace(pstrText);
        if( *pstrText != _T('=') ) return _Failed(_T("Error while parsing attributes"), pstrText);
        *pstrText++ = _T(' ');
        *pstrIdentifierEnd = _T('\0');
        _SkipWhitespace(pstrText);
        if( *pstrText++ != _T('\"') ) return _Failed(_T("Expected attribute value"), pstrText);
        LPTSTR pstrDest = pstrText;
        if( !_ParseData(pstrText, pstrDest, _T('\"')) ) return false;
        if( *pstrText == _T('\0') ) return _Failed(_T("Error while parsing attribute string"), pstrText);
        *pstrDest = _T('\0');
        *pstrText++ = _T('\0');
        _SkipWhitespace(pstrText);
    }
    return true;
}

bool CXmlManager::_ParseData(LPTSTR& pstrText, LPTSTR& pstrDest, char cEnd)
{
    while( *pstrText != _T('\0') && *pstrText != cEnd ) 
	{
        while (*pstrText == _T('&'))
        {
            _ParseMetaChar(++pstrText, pstrDest);
        }

        if( *pstrText == _T(' ') ) 
		{
            *pstrDest++ = *pstrText++;
            if( !m_bPreserveWhitespace ) _SkipWhitespace(pstrText);
        }
        else 
		{
            LPTSTR pstrTemp = ::CharNext(pstrText);
            while( pstrText < pstrTemp) 
			{
                *pstrDest++ = *pstrText++;
            }
        }
    }
    // Make sure that MapAttributes() works correctly when it parses
    // over a value that has been transformed.
    LPTSTR pstrFill = pstrDest + 1;
    while( pstrFill < pstrText ) *pstrFill++ = _T(' ');
    return true;
}

void CXmlManager::_ParseMetaChar(LPTSTR& pstrText, LPTSTR& pstrDest)
{
    if( pstrText[0] == _T('a') && pstrText[1] == _T('m') && pstrText[2] == _T('p') && pstrText[3] == _T(';') ) 
	{
        *pstrDest++ = _T('&');
        pstrText += 4;
    }
    else if( pstrText[0] == _T('l') && pstrText[1] == _T('t') && pstrText[2] == _T(';') )
	{
        *pstrDest++ = _T('<');
        pstrText += 3;
    }
    else if( pstrText[0] == _T('g') && pstrText[1] == _T('t') && pstrText[2] == _T(';') ) 
	{
        *pstrDest++ = _T('>');
        pstrText += 3;
    }
    else if( pstrText[0] == _T('q') && pstrText[1] == _T('u') && pstrText[2] == _T('o') && pstrText[3] == _T('t') && pstrText[4] == _T(';') ) 
	{
        *pstrDest++ = _T('\"');
        pstrText += 5;
    }
    else if( pstrText[0] == _T('a') && pstrText[1] == _T('p') && pstrText[2] == _T('o') && pstrText[3] == _T('s') && pstrText[4] == _T(';') )
	{
        *pstrDest++ = _T('\'');
        pstrText += 5;
    }
    else 
	{
        *pstrDest++ = _T('&');
    }
}

bool CXmlManager::_Failed(LPCTSTR pstrError, LPCTSTR pstrLocation)
{
    // Register last error
    //TRACE(_T("XML Error: %s"), pstrError);
   // TRACE(pstrLocation);
    _tcsncpy_s(m_szErrorMsg, pstrError, (sizeof(m_szErrorMsg) / sizeof(m_szErrorMsg[0])) - 1);
    _tcsncpy_s(m_szErrorXML, pstrLocation != NULL ? pstrLocation : _T(""), _countof(m_szErrorXML) - 1);
    return false; // Always return 'false'
}

