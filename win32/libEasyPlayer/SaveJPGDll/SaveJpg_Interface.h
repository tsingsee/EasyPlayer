
class CSaveJpgDll_Interface  
{
public:
	virtual BOOL WINAPI SaveBufferToJpg(BYTE *pData,int nSrcWidth,int nSrcHeight,LPSTR strPath,int nDesWidth,int nDesHeight)=0;
};
typedef	CSaveJpgDll_Interface*	LPSaveJpg;

LPSaveJpg	APIENTRY Create_SaveJpgDll();
void APIENTRY Release_SaveJpgDll(LPSaveJpg lpSaveJpg);