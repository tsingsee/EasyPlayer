#include "StdAfx.h"
#include "BuildDialog.h"
#include "XmlManager.h"

#include "SkinButton.h"
#include "SkinComboBox.h"
#include "SkinDialog.h"
#include "SkinEdit.h"
#include "SkinListBox.h"
#include "SkinSliderCtrl.h"
#include "SkinProgressCtrl.h"
#include "SkinListCtrl.h"
#include "SkinTreeCtrl.h"
#include "SkinTabCtrl.h"
#include "SkinRichEdit.h"
#include "SkinUIMenu.h"
#include "FlashControl.h"
#include "SkinToolBar.h"

CBuildDialog::CBuildDialog(void)
{
	m_hOwnWnd = NULL;
	m_ptWindowPos.SetPoint(0,0);
	m_szWindowSize.SetSize(0,0);
	m_szRcSize.SetSize(0,0);
	m_szMinSize.SetSize(0,0);
	m_szMaxSize.SetSize(0,0);
	m_bExtrude = false;
	m_bClip = false;
	m_bDragMoving = false;
	m_colBK = 0;
	m_bDefaultSkin = false;
	m_pXml = new CXmlManager;
}

CBuildDialog::~CBuildDialog(void)
{
	SafeDelete(m_pXml);

	for (int i=0;i<(int)m_ControlArray.size();i++)
	{
		SafeDelete(m_ControlArray[i]);
	}

	m_ControlArray.clear();

	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < (int)m_StringArray.size(); i++)
	{
		tagString * pStatic = m_StringArray[i];

		SafeDelete(pStatic->pPositionData);
		SafeDelete(pStatic);
	}

	m_StringArray.clear();

	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < (int)m_ImageArray.size(); i++)
	{
		tagImage * pImageData = m_ImageArray[i];

		SafeDelete(pImageData->pPositionData);
		UIRenderEngine->RemoveImage(pImageData->pImage);
		SafeDelete(pImageData);
	}

	m_ImageArray.clear();
	m_DialogArray.clear();
}

bool CBuildDialog::ParseWindow( LPCTSTR lpszFileName )
{
	LPCTSTR lpszFilePath = lpszFileName;

	CFileFind fd;
	if( fd.FindFile(lpszFileName) == FALSE )
	{
		TCHAR szWorkDirectory[MAX_PATH]={0};
		UIRenderEngine->GetWorkDirectory(szWorkDirectory,MAX_PATH);
		StrCat(szWorkDirectory,TEXT("\\"));
		StrCat(szWorkDirectory,lpszFileName);
		
		if( fd.FindFile(szWorkDirectory) == FALSE )
		{
			return false;
		}

		lpszFilePath = szWorkDirectory;
	}

	if (!lpszFilePath)
	{
		return false;
	}
	if( !m_pXml->LoadFromFile(lpszFilePath) )
		return false;

	CXmlNode root = m_pXml->GetRoot();
	if( !root.IsValid() ) return false;

	LPCTSTR pstrClass = root.GetName();;
	int nAttributes = 0;
	LPCTSTR pstrName = NULL;
	LPCTSTR pstrValue = NULL;
	LPTSTR pstr = NULL;

	if( _tcscmp(pstrClass, _T("Window")) == 0 ) 
	{
		int nAttributes = root.GetAttributeCount();
		for( int i = 0; i < nAttributes; i++ ) 
		{
			pstrName = root.GetAttributeName(i);
			pstrValue = root.GetAttributeValue(i);
			// 增加窗口初始化位置 [2/25/2016 Dingshuai]
			if (_tcscmp(pstrName, _T("pos")) == 0)
			{
				LPTSTR pstr = NULL;
				m_ptWindowPos.x = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				m_ptWindowPos.y = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 

			}
			else if( _tcscmp(pstrName, _T("size")) == 0 )
			{
				LPTSTR pstr = NULL;
				m_szWindowSize.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				m_szWindowSize.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
			} 
			else if( _tcscmp(pstrName, _T("roundcorner")) == 0 ) 
			{
				LPTSTR pstr = NULL;
				m_szRcSize.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				m_szRcSize.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
			} 
			else if( _tcscmp(pstrName, _T("mininfo")) == 0 )
			{
				LPTSTR pstr = NULL;
				m_szMinSize.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				m_szMinSize.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
			}
			else if( _tcscmp(pstrName, _T("maxinfo")) == 0 ) 
			{
				LPTSTR pstr = NULL;
				m_szMaxSize.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				m_szMaxSize.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
			}
			else if( _tcscmp(pstrName, _T("Resizing")) == 0 ) 
			{
				if (  _tcscmp(pstrValue, _T("true")) == 0 )
				{
					m_bExtrude = true;
				}
			}
			else if( _tcscmp(pstrName, _T("clip")) == 0 ) 
			{
				if (  _tcscmp(pstrValue, _T("true")) == 0 )
				{
					m_bClip = true;
				}
			}
			else if( _tcscmp(pstrName, _T("DragMoving")) == 0 ) 
			{
				if (  _tcscmp(pstrValue, _T("true")) == 0 )
				{
					m_bDragMoving = true;
				}
			}			
			else if( _tcscmp(pstrName, _T("bkcolor")) == 0 ) 
			{
				if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
				m_colBK = _tcstoul(pstrValue, &pstr, 16);
			}
			else if( _tcscmp(pstrName, _T("default")) == 0 ) 
			{
				m_bDefaultSkin = _tcscmp(pstrValue, _T("true")) == 0;
			}
		}
	}

	return ParseGraphics();
}

bool CBuildDialog::ParseGraphics()
{
	CXmlNode root = m_pXml->GetRoot();
	if( !root.IsValid() ) return false;

	LPCTSTR pstrName = NULL;
	LPCTSTR pstrValue = NULL;
	LPCTSTR pstrClass = NULL;
	int nAttributes = 0;
	LPTSTR pstr = NULL;

	for( CXmlNode node = root.GetChild() ; node.IsValid(); node = node.GetSibling() ) 
	{
		pstrClass = node.GetName();

		if( _tcscmp(pstrClass, _T("Font")) == 0 ) 
		{
			nAttributes = node.GetAttributeCount();
			LPCTSTR pFontName = NULL;
			int size = 12;
			bool bold = false;
			bool underline = false;
			bool italic = false;
			bool defaultfont = false;

			for( int i = 0; i < nAttributes; i++ )
			{
				pstrName = node.GetAttributeName(i);
				pstrValue = node.GetAttributeValue(i);
				if( _tcscmp(pstrName, _T("name")) == 0 ) 
				{
					pFontName = pstrValue;
				}
				else if( _tcscmp(pstrName, _T("size")) == 0 ) 
				{
					size = _tcstol(pstrValue, &pstr, 10);
				}
				else if( _tcscmp(pstrName, _T("bold")) == 0 ) 
				{
					bold = (_tcscmp(pstrValue, _T("true")) == 0);
				}
				else if( _tcscmp(pstrName, _T("underline")) == 0 ) 
				{
					underline = (_tcscmp(pstrValue, _T("true")) == 0);
				}
				else if( _tcscmp(pstrName, _T("italic")) == 0 ) 
				{
					italic = (_tcscmp(pstrValue, _T("true")) == 0);
				}
				else if( _tcscmp(pstrName, _T("default")) == 0 ) 
				{
					defaultfont = (_tcscmp(pstrValue, _T("true")) == 0);
				}
			}

			if ( defaultfont ) UIRenderEngine->SetDefaultFont(pFontName,size,bold,underline,italic);
			else UIRenderEngine->AddFont(pFontName,size,bold,underline,italic);
		}
		else if( _tcscmp(pstrClass, _T("String")) == 0 ) 
		{
			nAttributes = node.GetAttributeCount();
			
			LPCTSTR pText = NULL;
			UINT nFontIndex = NULL;
			COLORREF color = 0;
			tagPositionData *pPositionData = new tagPositionData;
			UINT uID = 0;
			UINT uFormat = DT_LEFT | DT_TOP | DT_SINGLELINE;
			bool bVisible = true;

			for( int i = 0; i < nAttributes; i++ )
			{
				pstrName = node.GetAttributeName(i);
				pstrValue = node.GetAttributeValue(i);
				if( _tcscmp(pstrName, _T("text")) == 0 ) 
				{
					pText = pstrValue;
				}
				else if( _tcscmp(pstrName, _T("font")) == 0 ) 
				{
					nFontIndex = _ttoi(pstrValue);
				}
				else if( _tcscmp(pstrName, _T("visible")) == 0 ) 
				{
					bVisible =  _tcscmp(pstrName, _T("false")) == 0;
				}
				else if( _tcscmp(pstrName, _T("format")) == 0 ) 
				{
					uFormat = _ttoi(pstrValue);
				}
				else if( _tcscmp(pstrName, _T("color")) == 0 ) 
				{
					if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
					color = _tcstoul(pstrValue, &pstr, 16);
				}
				else if( _tcscmp(pstrName, _T("pos")) == 0 ) 
				{
					pPositionData->nFixedPostion[0] = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
					pPositionData->nFixedPostion[1] = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
					pPositionData->ptPosition.x = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
					pPositionData->ptPosition.y = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
					pPositionData->szSize.cx = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
					pPositionData->szSize.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);  
				}
				else if( _tcscmp(pstrName, _T("id")) == 0 ) 
				{
					uID = _ttoi(pstrValue);
				}
			}

			AddString(pText, nFontIndex, uFormat,color,bVisible,pPositionData,uID);
		}
		else if( _tcscmp(pstrClass, _T("Image")) == 0 ) 
		{
			nAttributes = node.GetAttributeCount();

			LPCTSTR pPath = NULL;
			BYTE cbAlpha = 255;
			DWORD dwMask = 0;
			int nType = 0;
			int nRotation = 0;
			bool bGray = false;
			bool bVisible = true;

			tagPositionData *pPositionData = new tagPositionData;
			UINT uID = 0;

			for( int i = 0; i < nAttributes; i++ )
			{
				pstrName = node.GetAttributeName(i);
				pstrValue = node.GetAttributeValue(i);
				if( _tcscmp(pstrName, _T("path")) == 0 ) 
				{
					pPath = pstrValue;
				}
				else if( _tcscmp(pstrName, _T("alpha")) == 0 ) 
				{
					cbAlpha = _ttoi(pstrValue);
				}
				else if( _tcscmp(pstrName, _T("gray")) == 0 ) 
				{
					bGray = _tcscmp(pstrValue, _T("true")) == 0;
				}
				else if( _tcscmp(pstrName, _T("mask")) == 0 ) 
				{
					if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
					dwMask = _tcstoul(pstrValue, &pstr, 16);
				}
				else if( _tcscmp(pstrName, _T("pos")) == 0 ) 
				{
					pPositionData->nFixedPostion[0] = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
					pPositionData->nFixedPostion[1] = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
					pPositionData->ptPosition.x = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
					pPositionData->ptPosition.y = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
					pPositionData->szSize.cx = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
					pPositionData->szSize.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);  
				}
				else if( _tcscmp(pstrName, _T("id")) == 0 ) 
				{
					uID = _ttoi(pstrValue);
				}
				else if( _tcscmp(pstrName, _T("type")) == 0 ) 
				{
					nType = _ttoi(pstrValue);
				}
				else if( _tcscmp(pstrName, _T("rotation")) == 0 ) 
				{
					nRotation = _ttoi(pstrValue);
				}
				else if( _tcscmp(pstrName, _T("visible")) == 0 ) 
				{
					bVisible =  _tcscmp(pstrName, _T("false")) == 0;
				}
			}

			AddImage(pPath,cbAlpha,dwMask,nType,nRotation,bGray,bVisible,pPositionData,uID);
		}
	}

	return true;
}

bool CBuildDialog::ParseControl(CWnd* pParentWnd)
{
	CXmlNode root = m_pXml->GetRoot();
	if( !root.IsValid() ) return false;

	LPCTSTR pstrClass = NULL;

	ISkinControl*pControl = NULL;

	for( CXmlNode node = root.GetChild() ; node.IsValid(); node = node.GetSibling() ) 
	{
		pstrClass = node.GetName();

		if( _tcscmp(pstrClass, _T("Button")) == 0 ) pControl = new CSkinButton;
		else if( _tcscmp(pstrClass, _T("ComBox")) == 0 ) pControl = new CSkinComboBox;
		else if( _tcscmp(pstrClass, _T("Edit")) == 0 )	pControl = new CSkinEdit;
		else if( _tcscmp(pstrClass, _T("MulitiEdit")) == 0 )	pControl = new CMultiSkinEdit;
		else if( _tcscmp(pstrClass, _T("ListBox")) == 0 ) pControl = new CSkinListBox;
		else if( _tcscmp(pstrClass, _T("ListCtrl")) == 0 ) pControl = new CSkinListCtrl;
		else if( _tcscmp(pstrClass, _T("Progress")) == 0 ) pControl = new CSkinProgressCtrl;
		else if( _tcscmp(pstrClass, _T("RichEdit")) == 0 ) pControl = new CSkinRichEdit;
		else if( _tcscmp(pstrClass, _T("Slider")) == 0 ) pControl = new CSkinSliderCtrl;
		else if( _tcscmp(pstrClass, _T("TabCtrl")) == 0 ) pControl = new CSkinTabCtrl;
		else if( _tcscmp(pstrClass, _T("ToolBar")) == 0 ) pControl = new CSkinToolBar;
		else if( _tcscmp(pstrClass, _T("TreeCtrl")) == 0 ) pControl = new CSkinTreeCtrl;
		// now we not support this [8/15/2016 SwordTwelve]
//		else if( _tcscmp(pstrClass, _T("DirTreeCtrl")) == 0 ) pControl = new CDirTreeCtrl;

		else if( _tcscmp(pstrClass, _T("Flash")) == 0 ) pControl = new CFlashControl;
		else if( _tcscmp(pstrClass, _T("Static")) == 0 ) pControl = new CSkinStatic;

		//用户自己重绘其他控件
		else if( !CreateUserControl() ) continue;

		//控件创建失败，跳过
		if( !pControl->CreateControl(pParentWnd) ) continue;

		//入栈
		m_ControlArray.push_back(pControl);

		//解析属性
		if( node.HasAttributes() )
		{
			int nAttributes = node.GetAttributeCount();
			for( int i = 0; i < nAttributes; i++ ) 
			{
				pControl->SetAttribute(node.GetAttributeName(i), node.GetAttributeValue(i));
			}
		}

		//解析子节点
		if( node.HasChildren() ) 
		{
			for( CXmlNode child = node.GetChild() ; child.IsValid(); child = child.GetSibling() ) 
			{
				pstrClass = child.GetName();

				if( child.HasAttributes() )
				{
					pControl->ParseItem(&child);
				}
			}
		}
	}

	return true;
}

void CBuildDialog::DrawString(CDC*pDC)
{
	tagString *pStatic = NULL;
	for (int i=0;i<(int)m_StringArray.size();i++)
	{
		pStatic = m_StringArray[i];
		if ( NULL != pStatic )
		{
			if( !pStatic->bVisible ) continue;

			int nMode = pDC->SetBkMode(TRANSPARENT);

			pDC->SelectObject(UIRenderEngine->GetFont(pStatic->uFontIndex-1));

			pDC->SetTextColor(pStatic->colStatic);

			CRect rcBounder;
			UIRenderEngine->GetRect(m_hOwnWnd,pStatic->pPositionData,rcBounder);
			pDC->DrawText(pStatic->strText, rcBounder, pStatic->uFormat);
			pDC->SetBkMode(nMode);
		}
	}
}


int CBuildDialog::AddString(LPCTSTR lpszText, UINT uFontIndex,UINT uFormat,COLORREF col,bool bVisible,tagPositionData *pPositionData,UINT uID )
{
	if ( GetString(uID) != NULL ) return -1; 

	tagString*pStatic = new tagString;
	if (NULL == pStatic) return -1;

	pStatic->uFontIndex = uFontIndex;
	pStatic->colStatic = col;
	pStatic->pPositionData = pPositionData;
	pStatic->uID = uID;
	pStatic->strText = _CS(lpszText);
	pStatic->uFormat = uFormat;
	pStatic->bVisible = bVisible;
	pStatic->strTextBase=lpszText;
	m_StringArray.push_back(pStatic);

	return m_StringArray.size() - 1;
}

tagPositionData * CBuildDialog::GetStringRect( UINT uID )
{
	tagString*pStatic = NULL;

	static tagPositionData _PositionData={ 0 };

	for (int i=0;i<(int)m_StringArray.size();i++)
	{
		pStatic = m_StringArray[i];
		if ( NULL != pStatic )
		{
			if ( pStatic->uID == uID )
			{
				return pStatic->pPositionData;
			}
		}
	}

	return &_PositionData;
}

tagString * CBuildDialog::GetString( UINT uID )
{
	for (int i=0;i<(int)m_StringArray.size();i++)
	{
		if( m_StringArray[i]->uID == uID )
			return m_StringArray[i];
	}

	return NULL;
}

// 增加配置字串可编辑编辑 [3/4/2016 Dingshuai]
void CBuildDialog::SetString(UINT nID, CString strText)
{
	for (int i=0;i<(int)m_StringArray.size();i++)
	{
		if( m_StringArray[i]->uID == nID )
		{
			m_StringArray[i]->strText = strText;
		}
	}
}

int CBuildDialog::AddImage( LPCTSTR lpszFileName,BYTE dwAlpha,DWORD dwMask,int nType,int nRotation,bool bGray,bool bVisible,tagPositionData *pPositionData,UINT uID )
{
	if ( GetImage(uID) != NULL ) return -1; 

	tagImage*pImageData = new tagImage;
	if (NULL == pImageData) return -1;

	pImageData->pImage = UIRenderEngine->GetImage(lpszFileName);
	pImageData->cbAlpha = dwAlpha;
	pImageData->dwMask = dwMask;
	pImageData->nType = nType;
	pImageData->nRotation = nRotation;
	pImageData->pPositionData = pPositionData;
	pImageData->uID = uID;
	pImageData->bGray = bGray;
	pImageData->bVisible = bVisible;

	pImageData->pImage->SetRotation(nRotation);
	pImageData->pImage->SetAlpha(dwAlpha);
	if(bGray) pImageData->pImage->SetGray();	

	m_ImageArray.push_back(pImageData);

	return m_ImageArray.size() - 1;
}

tagPositionData * CBuildDialog::GetImageRect( UINT uID )
{
	tagImage*pImageData = NULL;

	static tagPositionData _PositionData={ 0 };

	for (int i=0;i<(int)m_ImageArray.size();i++)
	{
		pImageData = m_ImageArray[i];
		if ( NULL != pImageData )
		{
			if ( pImageData->uID == uID )
			{
				return pImageData->pPositionData;
			}
		}
	}

	return &_PositionData;
}

tagImage * CBuildDialog::GetImage( UINT uID )
{
	for (int i=0;i<(int)m_ImageArray.size();i++)
	{
		if( m_ImageArray[i]->uID == uID )
			return m_ImageArray[i];
	}

	return NULL;
}

void CBuildDialog::DrawImage( CDC*pDC )
{
	tagImage *pImageData = NULL;

	for (int i=0;i<(int)m_ImageArray.size();i++)
	{
		pImageData = m_ImageArray[i];

		if ( NULL != pImageData )
		{
			if( !pImageData->bVisible ) continue;

			CRect rcBounder;
			UIRenderEngine->GetRect(m_hOwnWnd,pImageData->pPositionData,rcBounder);
			// 平铺带拉升的(上下左右) [2/23/2016 Dingshuai]
			pImageData->pImage->DrawImage(pDC,rcBounder);
			// 平铺带延展的(左右) [2/23/2016 Dingshuai]
			pImageData->pImage->DrawExtrude(pDC, rcBounder);

		}
	}
}
void CBuildDialog::AddDialog(CBuildDialog *pDialog)
{
	m_DialogArray.push_back(pDialog);
}
void CBuildDialog::DeleteDialog(CBuildDialog *pDialog)
{
	vector<CBuildDialog*>::iterator iter = m_DialogArray.begin();

 	for (;iter != m_DialogArray.end(); ++iter )
 	{
		if(*iter==pDialog)
		{
			m_DialogArray.erase(iter);
			break;
		}

 	}
}
