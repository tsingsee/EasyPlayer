#include "StdAfx.h"
#include "ImageEx.h"
#include <math.h>
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CImageEx::CImageEx()
{
	::SetRect(&m_rcNinePart,0,0,0,0);
	m_cbAlpha = 255;

	return;
}

//析构函数
CImageEx::~CImageEx()
{
	//销毁图片
	DestroyImage(); 

	return;
}

//销毁图片
bool CImageEx::DestroyImage()
{
	Destroy();

	return true;
}

//加载图片
bool CImageEx::LoadImage(LPCTSTR pszFileName)
{
	HRESULT hr = CImage::Load(pszFileName);
	if (hr == S_OK)
	{
		return SetAlphaBit();
	}
	else
	{
		return FALSE;
	}
}

//加载图片
bool CImageEx::LoadImage(HINSTANCE hInstance, LPCTSTR pszResourceName,LPCTSTR pszResourceType/*=TEXT("IMAGE")*/)
{
	//查找资源
	HRSRC hResource=FindResource(hInstance,pszResourceName,pszResourceType);
	if (hResource==NULL) return false;

	//读取资源
	DWORD dwImageSize=SizeofResource(hInstance,hResource);
	LPVOID pImageBuffer=LoadResource(hInstance,hResource);

	//创建数据
	IStream * pIStream=NULL;
	if (CreateStreamOnHGlobal(NULL,TRUE,&pIStream)!=S_OK)
	{
		ASSERT(FALSE);
		return false;
	}

	//写入数据
	pIStream->Write(pImageBuffer,dwImageSize,NULL);

	HRESULT hr = CImage::Load(pIStream);

 	//释放资源
 	SafeRelease(pIStream);

	if (hr == S_OK)
	{
		return SetAlphaBit();
	}
	else
	{
		return FALSE;
	}
}

//绘画图像
BOOL CImageEx::DrawImage(CDC * pDC, INT nXPos, INT nYPos)
{
	pDC->SetStretchBltMode(HALFTONE);

	if ( m_ImageClone.IsNull() )
		return CImage::Draw(pDC->GetSafeHdc(),nXPos,nYPos,GetWidth(),GetHeight());
	else
	{
		if ( m_cbAlpha != 255 )
			m_ImageClone.AlphaBlend(pDC->GetSafeHdc(),nXPos,nYPos,GetWidth(),GetHeight(),0,0,GetWidth(),GetHeight(),m_cbAlpha);
		else m_ImageClone.Draw(pDC->GetSafeHdc(),nXPos,nYPos,GetWidth(),GetHeight());
	}

	return CImage::Draw(pDC->GetSafeHdc(),nXPos,nYPos);
}

//绘画图像
BOOL CImageEx::DrawImage( CDC * pDC, INT nXPos, INT nYPos, INT nDestWidth, INT nDestHeight )
{
	pDC->SetStretchBltMode(HALFTONE);
	if( nDestWidth == 0 || nDestHeight == 0 ) return FALSE;

	if ( m_ImageClone.IsNull() )
		return CImage::Draw(pDC->GetSafeHdc(),nXPos,nYPos,nDestWidth,nDestHeight);
	else
	{
		if ( m_cbAlpha != 255 )
			m_ImageClone.AlphaBlend(pDC->GetSafeHdc(),nXPos,nYPos,nDestWidth,nDestHeight,0,0,GetWidth(),GetHeight(),m_cbAlpha);
		else m_ImageClone.Draw(pDC->GetSafeHdc(),nXPos,nYPos,nDestWidth,nDestHeight);
	}

	return FALSE;
}

//绘画图像
BOOL CImageEx::DrawImage( CDC * pDC, RECT &rc )
{
	pDC->SetStretchBltMode(HALFTONE);
	if( rc.right-rc.left == 0 || rc.bottom-rc.top == 0 ) return FALSE;

	if ( m_ImageClone.IsNull() )
		return CImage::Draw(pDC->GetSafeHdc(),rc);
	else 
	{
		if ( m_cbAlpha != 255 )
			m_ImageClone.AlphaBlend(pDC->GetSafeHdc(),rc,CRect(0,0,m_ImageClone.GetWidth(),m_ImageClone.GetHeight()),m_cbAlpha);
		else
		{
			if ( m_cbAlpha != 255 )
				m_ImageClone.AlphaBlend(pDC->GetSafeHdc(),rc.left,rc.top,(rc.right-rc.left),(rc.bottom-rc.top),0,0,GetWidth(),GetHeight(),m_cbAlpha);
			else m_ImageClone.Draw(pDC->GetSafeHdc(),rc);
		}
	}

	return FALSE;
}

//绘画图像
BOOL CImageEx::DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT nYSrc)
{
	pDC->SetStretchBltMode(HALFTONE);
	if( nDestWidth == 0 || nDestHeight == 0 ) return FALSE;

	if ( m_ImageClone.IsNull() )
		return CImage::Draw(pDC->GetSafeHdc(),nXDest,nYDest,nDestWidth,nDestHeight,nXScr,nYSrc,GetWidth(),GetHeight());
	else 
	{
		if ( m_cbAlpha != 255 )
			m_ImageClone.AlphaBlend(pDC->GetSafeHdc(),nXDest,nYDest,nDestWidth,nDestHeight,nXScr,nYSrc,GetWidth(),GetHeight(),m_cbAlpha);
		else m_ImageClone.Draw(pDC->GetSafeHdc(),nXDest,nYDest,nDestWidth,nDestHeight,nXScr,nYSrc,GetWidth(),GetHeight());
	}

	return FALSE;
}

//绘画图像
BOOL CImageEx::DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT nYSrc, INT nSrcWidth, INT nSrcHeight)
{
	pDC->SetStretchBltMode(HALFTONE);
	if( nSrcWidth == 0 || nSrcHeight == 0 ) return FALSE;

	if ( m_ImageClone.IsNull() )
		return CImage::Draw(pDC->GetSafeHdc(),nXDest,nYDest,nDestWidth,nDestHeight,nXScr,nYSrc,nSrcWidth,nSrcHeight);
	else 
	{
		if ( m_cbAlpha != 255 )
			m_ImageClone.AlphaBlend(pDC->GetSafeHdc(),nXDest,nYDest,nDestWidth,nDestHeight,nXScr,nYSrc,nDestWidth,nDestHeight,m_cbAlpha);
		else m_ImageClone.Draw(pDC->GetSafeHdc(),nXDest,nYDest,nDestWidth,nDestHeight,nXScr,nYSrc,nSrcWidth,nSrcHeight);
	}

	return FALSE;
}

bool CImageEx::Draw( CDC * pDC, INT x, INT y, INT cx, INT cy,INT nLeft,INT nTop,INT nRight,INT nBottom )
{
	int cxImage = GetWidth();
	int cyImage = GetHeight();

	// 左上
	{
		RECT rcDest = {x, y, x+nLeft, y+nTop};
		RECT rcSrc = {0, 0, nLeft, nTop};
		if (!::IsRectEmpty(&rcDest) && !::IsRectEmpty(&rcSrc))
			DrawImage(pDC,rcDest.left, rcDest.top, rcDest.right-rcDest.left, rcDest.bottom-rcDest.top, 
			rcSrc.left, rcSrc.top,rcSrc.right-rcSrc.left, rcSrc.bottom-rcSrc.top);
	}

	// 左边
	{
		RECT rcDest = {x, y+nTop, x+nLeft, (y+nTop)+(cy-nTop-nBottom)};
		RECT rcSrc = {0, nTop, nLeft, nTop+(cyImage-nTop-nBottom)};
		if (!::IsRectEmpty(&rcDest) && !::IsRectEmpty(&rcSrc))
			DrawImage(pDC,rcDest.left, rcDest.top, rcDest.right-rcDest.left, rcDest.bottom-rcDest.top, 
			rcSrc.left, rcSrc.top,rcSrc.right-rcSrc.left, rcSrc.bottom-rcSrc.top);
	}

	// 上边
	{
		RECT rcDest = {x+nLeft, y,x+(cx-nRight), y+nTop};
		RECT rcSrc = {nLeft, 0, (cxImage-nLeft-nRight), nTop};
 		if (!::IsRectEmpty(&rcDest) && !::IsRectEmpty(&rcSrc))
 			DrawImage(pDC,rcDest.left, rcDest.top, rcDest.right-rcDest.left, rcDest.bottom-rcDest.top, 
 			rcSrc.left, rcSrc.top,rcSrc.right-rcSrc.left, rcSrc.bottom-rcSrc.top);
	}

	// 右上
	{
		RECT rcDest = {x+(cx-nRight), y, (x+(cx-nRight))+nRight, y+nTop};
		RECT rcSrc = {cxImage-nRight, 0, (cxImage-nRight)+nRight, nTop};
  		if (!::IsRectEmpty(&rcDest) && !::IsRectEmpty(&rcSrc))
  			DrawImage(pDC,rcDest.left, rcDest.top, rcDest.right-rcDest.left, rcDest.bottom-rcDest.top, 
  			rcSrc.left, rcSrc.top,rcSrc.right-rcSrc.left, rcSrc.bottom-rcSrc.top);
	}

	// 右边
	{
		RECT rcDest = {x+(cx-nRight), y+nTop, (x+(cx-nRight))+nRight, (y+nTop)+(cy-nTop-nBottom)};
		RECT rcSrc = {cxImage-nRight, nTop, (cxImage-nRight)+nRight, nTop+(cyImage-nTop-nBottom)};
  		if (!::IsRectEmpty(&rcDest) && !::IsRectEmpty(&rcSrc))
  			DrawImage(pDC,rcDest.left, rcDest.top, rcDest.right-rcDest.left, rcDest.bottom-rcDest.top, 
  			rcSrc.left, rcSrc.top,rcSrc.right-rcSrc.left, rcSrc.bottom-rcSrc.top);
	}

	// 下边
	{
		RECT rcDest = {x+nLeft, y+(cy-nBottom), (x+nLeft)+(cx-nLeft-nRight), (y+(cy-nBottom))+nBottom};
		RECT rcSrc = {nLeft, cyImage-nBottom, nLeft+(cxImage-nLeft-nRight), (cyImage-nBottom)+nBottom};
 		if (!::IsRectEmpty(&rcDest) && !::IsRectEmpty(&rcSrc))
 			DrawImage(pDC,rcDest.left, rcDest.top, rcDest.right-rcDest.left, rcDest.bottom-rcDest.top, 
 			rcSrc.left, rcSrc.top,rcSrc.right-rcSrc.left, rcSrc.bottom-rcSrc.top);
	}

	// 右下
	{
		RECT rcDest = {x+(cx-nRight), y+(cy-nBottom), (x+(cx-nRight))+nRight, (y+(cy-nBottom))+nBottom};
		RECT rcSrc = {cxImage-nRight, cyImage-nBottom, (cxImage-nRight)+nRight, (cyImage-nBottom)+nBottom};
 		if (!::IsRectEmpty(&rcDest) && !::IsRectEmpty(&rcSrc))
 			DrawImage(pDC,rcDest.left, rcDest.top, rcDest.right-rcDest.left, rcDest.bottom-rcDest.top, 
 			rcSrc.left, rcSrc.top,rcSrc.right-rcSrc.left, rcSrc.bottom-rcSrc.top);
	}

	// 左下
	{
		RECT rcDest = {x, y+(cy-nBottom), x+nLeft, (y+(cy-nBottom))+nBottom};
		RECT rcSrc = {0, cyImage-nBottom, nLeft, (cyImage-nBottom)+nBottom};
		if (!::IsRectEmpty(&rcDest) && !::IsRectEmpty(&rcSrc))
			DrawImage(pDC,rcDest.left, rcDest.top, rcDest.right-rcDest.left, rcDest.bottom-rcDest.top, 
			rcSrc.left, rcSrc.top,rcSrc.right-rcSrc.left, rcSrc.bottom-rcSrc.top);
	}

	// 中间
	{
		RECT rcDest = {x+nLeft, y+nTop, x+(cx-nRight), y+(cy-nBottom)};
		RECT rcSrc = {nLeft, nTop, cxImage-nRight, cyImage-nBottom};

   		if (!::IsRectEmpty(&rcDest) && !::IsRectEmpty(&rcSrc))
   			DrawImage(pDC,rcDest.left, rcDest.top, rcDest.right-rcDest.left, rcDest.bottom-rcDest.top, 
   			rcSrc.left, rcSrc.top,rcSrc.right-rcSrc.left, rcSrc.bottom-rcSrc.top);
	}

	return true;
}

bool CImageEx::Draw( CDC * pDC, const RECT& rectDest, const RECT& rectSrc )
{
	return Draw(pDC,rectDest.left, rectDest.top, rectDest.right-rectDest.left, rectDest.bottom-rectDest.top, 
		rectSrc.left, rectSrc.top,rectSrc.right, rectSrc.bottom);
}

bool CImageEx::Draw( CDC * pDC, const RECT& rectDest )
{
	return Draw(pDC,rectDest,m_rcNinePart);
}

bool CImageEx::DrawFrame( CDC * pDC, const RECT& rectDest )
{
	INT x = rectDest.left;
	INT y = rectDest.top;
	
	INT cx = rectDest.right-rectDest.left;
	INT cy = rectDest.bottom - rectDest.top;

	INT nLeft = m_rcNinePart.left;
	INT nTop = m_rcNinePart.top;
	INT nRight = m_rcNinePart.right;
	INT nBottom = m_rcNinePart.bottom;
 
	// 左边
	{
		RECT rcDest = {x, y, x+nLeft, (y+nTop)+(cy)};
		RECT rcSrc = {0, nTop, nLeft, nTop+(GetHeight()-nTop-nBottom)};
		if (!::IsRectEmpty(&rcDest) && !::IsRectEmpty(&rcSrc))
			DrawImage(pDC,rcDest.left, rcDest.top, rcDest.right-rcDest.left, rcDest.bottom-rcDest.top, 
			rcSrc.left, rcSrc.top,rcSrc.right-rcSrc.left, rcSrc.bottom-rcSrc.top);
	}

	// 上边
	{
		RECT rcDest = {x, y,x+(cx), y+nTop};
		RECT rcSrc = {nLeft, 0, (GetWidth()-nLeft-nRight), nTop};
		if (!::IsRectEmpty(&rcDest) && !::IsRectEmpty(&rcSrc))
			DrawImage(pDC,rcDest.left, rcDest.top, rcDest.right-rcDest.left, rcDest.bottom-rcDest.top, 
			rcSrc.left, rcSrc.top,rcSrc.right-rcSrc.left, rcSrc.bottom-rcSrc.top);
	}

	// 右边
	{
		RECT rcDest = {x+(cx-nRight), y+nTop, (x+(cx-nRight))+nRight, (y+nTop)+(cy-nTop-nBottom)};
		RECT rcSrc = {GetWidth()-nRight, nTop, (GetWidth()-nRight)+nRight, nTop+(GetHeight()-nTop-nBottom)};
		if (!::IsRectEmpty(&rcDest) && !::IsRectEmpty(&rcSrc))
			DrawImage(pDC,rcDest.left, rcDest.top, rcDest.right-rcDest.left, rcDest.bottom-rcDest.top, 
			rcSrc.left, rcSrc.top,rcSrc.right-rcSrc.left, rcSrc.bottom-rcSrc.top);
	}

	// 下边
	{
		RECT rcDest = {x, y+(cy-nBottom), (x)+(cx), (y+(cy-nBottom))+nBottom};
		RECT rcSrc = {nLeft, GetHeight()-nBottom, nLeft+(GetWidth()-nLeft-nRight), (GetHeight()-nBottom)+nBottom};
		if (!::IsRectEmpty(&rcDest) && !::IsRectEmpty(&rcSrc))
			DrawImage(pDC,rcDest.left, rcDest.top, rcDest.right-rcDest.left, rcDest.bottom-rcDest.top, 
			rcSrc.left, rcSrc.top,rcSrc.right-rcSrc.left, rcSrc.bottom-rcSrc.top);
	}
// 	DrawImage(pDC,rectDest.left,rectDest.top,nWidth,m_rcNinePart.top,0,0,GetWidth(),m_rcNinePart.top);
// 	DrawImage(pDC,rectDest.right-m_rcNinePart.right,rectDest.top-m_rcNinePart.top,m_rcNinePart.right,nHeight,GetWidth()-m_rcNinePart.right,0,m_rcNinePart.right,GetHeight());
// 	DrawImage(pDC,rectDest.left,rectDest.bottom-m_rcNinePart.bottom,nWidth,m_rcNinePart.bottom,0,GetHeight()-m_rcNinePart.bottom,GetWidth(),m_rcNinePart.bottom);
// 	DrawImage(pDC,rectDest.left,rectDest.top-m_rcNinePart.top,m_rcNinePart.left,nHeight,0,0,m_rcNinePart.left,GetHeight());

	return true;
}

void CImageEx::SetNinePart( CONST LPRECT lprcNinePart )
{
	if( lprcNinePart == NULL ) return;

	::CopyRect(&m_rcNinePart,lprcNinePart);
}

HBITMAP CImageEx::ImageToBitmap() 
{  
	return (HBITMAP)*this;
}

bool CImageEx::DrawExtrude( CDC*pDC,const RECT& rectDest,bool bLeft,int nPixel )
{
	//创建屏幕
	ASSERT(pDC!=NULL);

	int nWidth = rectDest.right - rectDest.left;
	int nHeight = rectDest.bottom - rectDest.top;

	if ( bLeft )
	{
		DrawImage(pDC,rectDest.left,rectDest.top);

		if(nWidth>GetWidth())
			DrawImage(pDC,GetWidth(),rectDest.top,(nWidth-GetWidth()),GetHeight(),GetWidth()-nPixel,0,nPixel,GetHeight());
	}
	else
	{
		if ( nWidth <=GetWidth() )
		{
			DrawImage(pDC,rectDest.left,rectDest.top,nWidth,GetHeight(),GetWidth()-nWidth,0,nWidth,GetHeight());
		}
		else
		{
			DrawImage(pDC,rectDest.left,rectDest.top,nWidth-GetWidth(),GetHeight(),nPixel,0,nPixel,GetHeight());
			DrawImage(pDC,nWidth-GetWidth(),rectDest.top);
		}
	}

	return true;
}

bool CImageEx::SetGray()
{
	CImage *pImage = &m_ImageClone;
	if ( pImage == NULL && pImage->IsNull() ) 
	{
		pImage = this;
	}

	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();

	BYTE* pArray = (BYTE*)pImage->GetBits();
	int nPitch = pImage->GetPitch();
	int nBitCount = pImage->GetBPP() / 8;

	for (int i = 0; i < nHeight; i++) 
	{
		for (int j = 0; j < nWidth; j++) 
		{
			int grayVal = (BYTE)(((*(pArray + nPitch * i + j * nBitCount) * 306)
				+ (*(pArray + nPitch * i + j * nBitCount + 1) * 601)
				+ (*(pArray + nPitch * i + j * nBitCount + 2) * 117) + 512 ) >> 10);	// 计算灰度值

			*(pArray + nPitch * i + j * nBitCount) = grayVal;							// 赋灰度值
			*(pArray + nPitch * i + j * nBitCount + 1) = grayVal;
			*(pArray + nPitch * i + j * nBitCount + 2) = grayVal;
		}
	}

	return true;
}

bool CImageEx::SetAlphaBit()
{
	ASSERT(IsNull() == false);
	if(IsNull())return FALSE;

	if ( GetBPP() == 32 )//png图像
	{
		LPVOID pBitsSrc = NULL;
		BYTE * psrc = NULL;
		BITMAP stBmpInfo;

		HBITMAP hBmp = (HBITMAP)*this;

		::GetObject(hBmp, sizeof(BITMAP), &stBmpInfo);

		if (32 != stBmpInfo.bmBitsPixel || NULL == stBmpInfo.bmBits)
			return FALSE;

		psrc = (BYTE *) stBmpInfo.bmBits;

		for (int nPosY = 0; nPosY < abs(stBmpInfo.bmHeight); nPosY++)
		{
			for (int nPosX = stBmpInfo.bmWidth; nPosX > 0; nPosX--)
			{
				BYTE alpha  = psrc[3];
				psrc[0] = (BYTE)((psrc[0] * alpha) / 255);
				psrc[1] = (BYTE)((psrc[1] * alpha) / 255);
				psrc[2] = (BYTE)((psrc[2] * alpha) / 255);
				psrc += 4;
			}
		}
	}

	return TRUE;
}

void CImageEx::GetImageParament( CImage *pImg,IMAGEPARAMENT *ppImgParam )
{
	if (pImg->IsNull()) return;  

	ppImgParam->nWidth   = pImg->GetWidth();      //图像宽度  
	ppImgParam->nHeight   = pImg->GetHeight();        //图像高度  
	ppImgParam->nBitCount  = pImg->GetBPP();      //每像素位数  
	ppImgParam->nBytesPerLine   = (pImg->GetWidth()*pImg->GetBPP()+31)/32*4;   //每行字节数  
	ppImgParam->nBytesPerPixel   = pImg->GetBPP()/8;      //每像素字节数  
	if (pImg->GetBPP()<=8)   
		ppImgParam->nNumColors= 1 << pImg->GetBPP();        //调色板单元数  
	else   
		ppImgParam->nNumColors= 0;  
	ppImgParam->nSize  = ppImgParam->nBytesPerLine*ppImgParam->nHeight;        //像素总字节数    
}

int CImageEx::InImage( CImage *pImg,int x,int y )
{
	IMAGEPARAMENT  P;  

	GetImageParament(pImg,&P);  
	if ((x<0)||(y<0)||(x>=P.nWidth)||(y>=P.nHeight))  return 0;  
	else  return 1;  
}

void CImageEx::SetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf )
{
	IMAGEPARAMENT  P;  
	BYTE    *lp;  
	int     i,dw,dh,x1,y1,alpha,delta,Dxb,dwb;  

	GetImageParament(pImg,&P);  
	if (P.nBitCount<8) return;  
	x1=x;  
	y1=y;  
	alpha=delta=0;  
	
	if (x<0) 
	{   
		alpha=-x;    x1=0;  
	}  

	if (y<0) 
	{   
		delta=-y;    y1=0;  
	}  

	if (!InImage(pImg,x1,y1)) return;  
	dw=min(Dx,(int) P.nWidth-x1);      
	dh=min(Dy,(int) P.nHeight-y1);  
	dw -= alpha;  
	dh -= delta;  

	Dxb = Dx*P.nBytesPerPixel;  
	dwb = dw*P.nBytesPerPixel;  
	lp = (BYTE*) pImg->GetPixelAddress(x1,y1);  
	buf += (delta*Dx+alpha)*P.nBytesPerPixel;  
	
	for (i=0;i<dh;i++) 
	{  
		memcpy(lp,buf,dwb);    
		buf += Dxb;   
		lp -= P.nBytesPerLine;  
	}
}

void CImageEx::GetAllPalette( CImage *pImg,RGBQUAD *ColorTab )
{
	IMAGEPARAMENT P;  
	GetImageParament(pImg,&P);  
	pImg->GetColorTable(0, P.nNumColors, ColorTab);  
}

void CImageEx::SetAllPalette( CImage *pImg,RGBQUAD *ColorTab )
{
	IMAGEPARAMENT P;  

	GetImageParament(pImg,&P);  
	pImg->SetColorTable(0, P.nNumColors, ColorTab);  
}

int CImageEx::PaletteType( RGBQUAD *ColorTab )
{
	int i,k,m,n,r,g,b;  

	m=n=0;  
	for (i=0; i<256; i++)  
	{  
		r = ColorTab[i].rgbRed;  
		g = ColorTab[i].rgbGreen;  
		b = ColorTab[i].rgbBlue;  
		if ((r != g)||(r != b)) m=0;  
		if ((i>0)&&(r>ColorTab[i-1].rgbRed)) m++;  
		if (r+g+b==0) n++;  
	}  

	k=3;  
	if (m == 255) k=2;      
	else  if (256-n==1) k=0;                       
	else  if (256-n==15) k=1;      

	return(k);  
}

int CImageEx::ImageType()
{
	RGBQUAD ColorTab[256];  
	int     k;  

	if (IsNull()) return(0);  

	switch(GetBPP())  
	{  
	case 1:  k=0;   break;   
	case 4:  k=1;   break;   
	case 8:  k=3;   break;   
	default: k=4;   break;   
	}  

	if (k==3)                    
	{  
		GetColorTable(0,256,ColorTab);  
		k=PaletteType(ColorTab);  
	}  
	return(k);  
}

void CImageEx::RotateCimage( CImage *Imgn, int nAngle )
{
	double alpha = nAngle*3.141592654/180;

	IMAGEPARAMENT P;   
	RGBQUAD ColorTab[256];   
	int i, j, ww, Xd, Yd, Dx, Dy,nSize;   
	double centerx, centery, sintheta, costheta;   
	double X1, Y1, X2, Y2, theta, xx, yy, rr;   
	BYTE **list, *sc;   
	int x1, y1, x2, y2, flag;   
	double p, q, a, b, c, d, t1, t2, t3;   

	if (ImageType() == 2)  flag = 1; //flag为标志位，当取值为1时，表示双线性内插法 ,此时图像类型为灰阶图像  
	else flag = 0; //0表示最近邻点法   
	GetImageParament(this,&P);   

	Dx = P.nWidth;   
	Dy = P.nHeight;   
	nSize = 0;  
	if (Dx < Dy)  
	{  
		nSize = Dy;  
	}  
	else  
	{  
		nSize = Dx;  
	}  
	int nLineBytes = (nSize * P.nBitCount + 31) / 32 * 4;  

	//还有一点要修改，不然当图像高度远大于宽度时会崩溃  
	sc = (BYTE*) malloc(2 * nLineBytes);    // * P.nBytesPerLine); //申请工作单元   
	//  
	list = (BYTE**) malloc(Dy * sizeof(BYTE*)); //对原位图建立二维数组   
	for (i = 0; i < Dy; i++)   
		list[i] = (BYTE*) GetPixelAddress(0, i);   

	centerx = Dx / 2; //计算位图中心位置   
	centery = Dy / 2;   
	rr = sqrt(centerx * centerx + centery *centery); //计算对角线长度   

	theta = atan((double) centery / (double) centerx);   

	X1 = fabs(rr * cos(alpha + theta)) + 0.5;   
	Y1 = fabs(rr * sin(alpha + theta)) + 0.5;   
	X2 = fabs(rr * cos(alpha - theta)) + 0.5;   
	Y2 = fabs(rr * sin(alpha - theta)) + 0.5;   

	if (X2 > X1) X1 = X2; //得外接矩形宽度   
	if (Y2 > Y1) Y1 = Y2; //外接矩形高度   
	ww = (int) (2 * X1);   

	Imgn ->Destroy();   

	//建立结果位图  
	Imgn ->Create(ww, (int) (2 * Y1), P.nBitCount,CImage::createAlphaChannel );  
	if (P.nBitCount == 8)   
	{   
		GetAllPalette(this,ColorTab);   
		//修改一，设置目标调色板  
		SetAllPalette(Imgn, ColorTab); //复制调色板   
	}   

	sintheta = sin(alpha);   
	costheta = cos(alpha);   

	for (j = (int) (centery - Y1), Yd = 0; j <= (centery + Y1); j++, Yd++)   
	{   
		if (P.nBitCount == 8)   
			memset (sc, 0, ww); //256色位图像素行置背景值   
		else memset(sc, 0, ww * P.nBytesPerPixel); //真彩色位图像素行置背景值   

		for (i = (int) (centerx - X1), Xd = 0; i <= centerx + X1; i++, Xd += P.nBytesPerPixel)   
		{   
			xx = centerx + costheta * (i - centerx) + sintheta * (j - centery);   
			yy = centery - sintheta * (i - centerx) + costheta * (j - centery);   
			x1 = (int) xx;   
			x2 = x1 + 1;   
			p = xx - x1;   
			y1 = (int) yy;   
			y2 = y1 + 1;   
			q = yy - y1;   
			if (((x1 < 0)||(x2 >= P.nWidth )||(y1 < 0)||(y2 >= P.nHeight )))   
				continue;   
			if (flag == 0)   
			{   
				if (q > 0.5)  y1 = y2;   
				if (p > 0.5)  x1 = x2;   
				//修改二, sc[Xd]  
				memcpy(&sc[Xd], &list[y1][x1 * P.nBytesPerPixel], P.nBytesPerPixel); //从源位图复制像素数据   
			}   
			else   
			{ // flag等于1，双线性内插法   
				a = (double) list[y1][x1]; //从源位图取数据   
				b = (double) list[y1][x2];   
				c = (double) list[y2][x1];   
				d = (double) list[y2][x2];   
				t1 = (1 - p) * a + p * b; //双线性内插计算   
				t2 = (1 - p) * c + p * d;   
				t3 = (1 - q) * t1 + q * t2;   
				//修改三  
				sc[Xd] = (BYTE) t3;   
			}   
		}   
		SetRectValue(Imgn, 0, Yd, ww, 1, sc);   
	}  

	//png图像
	if ( P.nBitCount == 32 )
	{
		LPVOID pBitsSrc = NULL;
		BYTE * psrc = NULL;
		BITMAP stBmpInfo;

		HBITMAP hBmp = (HBITMAP)m_ImageClone;

		::GetObject(hBmp, sizeof(BITMAP), &stBmpInfo);

		if (32 != stBmpInfo.bmBitsPixel || NULL == stBmpInfo.bmBits)
			return;

		psrc = (BYTE *) stBmpInfo.bmBits;

		for (int nPosY = 0; nPosY < abs(stBmpInfo.bmHeight); nPosY++)
		{
			for (int nPosX = stBmpInfo.bmWidth; nPosX > 0; nPosX--)
			{
				BYTE alpha  = psrc[3];
				psrc[0] = (BYTE)((psrc[0] * alpha) / 255);
				psrc[1] = (BYTE)((psrc[1] * alpha) / 255);
				psrc[2] = (BYTE)((psrc[2] * alpha) / 255);
				psrc += 4;
			}
		}
	}

	free(list); //释放工作单元   
	free(sc);   
}

void CImageEx::SetRotation( int nAngle )
{
	if ( nAngle == 0 ) return;

	m_ImageClone.Destroy();
	RotateCimage(&m_ImageClone,nAngle);
}



//////////////////////////////////////////////////////////////////////////////////
