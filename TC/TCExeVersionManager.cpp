#include "StdAfx.h"
#include "TCExeVersionManager.h"
#include "TCRunTimeManager.h"
#include "exeVersion/VersionInfo.h"

bool CTCExeVersionManager::m_isChangeExeIcon=false;
CString CTCExeVersionManager::m_productName=_T("TC");
CString CTCExeVersionManager::m_verDesInfo=_T("TC");
CString CTCExeVersionManager::m_fileInfo=_T("1.0.0.0");
CString CTCExeVersionManager::m_productInfo=_T("1.0.0.0");
CString CTCExeVersionManager::m_internalName=_T("TC");	//默认内部名称TC
CString CTCExeVersionManager::m_companyName=_T("简单软件");
CString CTCExeVersionManager::m_leagalInfo=_T("(C)<简单软件>保留所有版权");


CTCExeVersionManager::CTCExeVersionManager(void)
{

}

typedef struct tagHEADER
{
    WORD idReserved;
    WORD idType;
    WORD idCount;
}HEADER, *LPHEADER;
typedef struct tagICONDIRENTRY
{
    BYTE bWidth;
    BYTE bHeight;
    BYTE bColorCount;
    BYTE bReserved;
    WORD wPlanes;
    WORD wBitCount;
    DWORD dwBytesInRes;
    DWORD dwImageOffset;
}ICONDIRENTRY, *LPICONDIRENTRY;

#pragma pack( push )
#pragma pack( 2 )
typedef struct tagGRPICONDIRENTRY
{
    BYTE bWidth;
    BYTE bHeight;
    BYTE bColorCount;
    BYTE bReserved;
    WORD wPlanes;
    WORD wBitCount;
    DWORD dwBytesInRes;
    WORD nID;
}GRPICONDIRENTRY, *LPGRPICONDIRENTRY;;

typedef struct tagGRPICONDIR
{
    WORD idReserved;
    WORD idType;
    WORD idCount;
    GRPICONDIRENTRY idEntries[1];
}GRPICONDIR, *LPGRPICONDIR;

void CTCExeVersionManager::changedExeIcon(LPCTSTR lpExeName, LPCTSTR lpIconFile)
{
	LPICONDIRENTRY pIconDirEntry(NULL);      
    LPGRPICONDIR pGrpIconDir(NULL);      
    HEADER header;      
    LPBYTE pIconBytes(NULL);      
    HANDLE hIconFile(NULL);      
    DWORD dwRet(0), nSize(0), nGSize(0), dwReserved(0);      
    HANDLE hUpdate(NULL);      
    BOOL ret(FALSE);      
    WORD i(0);      

    //打开图标文件      
    hIconFile = CreateFile(lpIconFile, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);      
    if (hIconFile == INVALID_HANDLE_VALUE)      
    {      
        return;      
    }      
    //读取文件头部信息      
    ret=ReadFile(hIconFile, &header, sizeof(HEADER), &dwReserved, NULL);      
    if (!ret)      
    {      
        CloseHandle(hIconFile);      
        return;      
    }      
    //建立每一个图标的目录信息存放区域      
    pIconDirEntry = (LPICONDIRENTRY)new BYTE[header.idCount*sizeof(ICONDIRENTRY)];      
    if (pIconDirEntry==NULL)
    {      
        CloseHandle(hIconFile);      
        return;      
    }      
    //从Icon文件中读取每一个图标的目录信息      
    ret = ReadFile(hIconFile, pIconDirEntry, header.idCount*sizeof(ICONDIRENTRY), &dwReserved, NULL);      
    if (!ret)      
    {      
        delete[] pIconDirEntry;      
        CloseHandle(hIconFile);      
        return;      
    }      
    //建立EXE文件中RT_GROUP_ICON所需的数据结构存放区域      
    nGSize=sizeof(GRPICONDIR)+header.idCount*sizeof(ICONDIRENTRY);      
    pGrpIconDir=(LPGRPICONDIR)new BYTE[nGSize];      
    //填充信息，这里相当于一个转换的过程      
    pGrpIconDir->idReserved=header.idReserved;      
    pGrpIconDir->idType=header.idType;      
    pGrpIconDir->idCount=header.idCount;      
    //复制信息并设置每一个图标对应的ID。ID为位置索引号      
    for(i=0;i<header.idCount;i++)      
    {      
        pGrpIconDir->idEntries[i].bWidth=pIconDirEntry[i].bWidth;      
        pGrpIconDir->idEntries[i].bHeight=pIconDirEntry[i].bHeight;      
        pGrpIconDir->idEntries[i].bColorCount=pIconDirEntry[i].bColorCount;      
        pGrpIconDir->idEntries[i].bReserved=pIconDirEntry[i].bReserved;      
        pGrpIconDir->idEntries[i].wPlanes=pIconDirEntry[i].wPlanes;      
        pGrpIconDir->idEntries[i].wBitCount=pIconDirEntry[i].wBitCount;      
        pGrpIconDir->idEntries[i].dwBytesInRes=pIconDirEntry[i].dwBytesInRes;      
        pGrpIconDir->idEntries[i].nID=i;      
    }      
    //开始更新EXE中的图标资源，ID定为最小０，如果原来存在０ID的图标信息则被替换为新的。      
    hUpdate = BeginUpdateResourceW(lpExeName, false);      
    if (hUpdate!=NULL)      
    {      
        //首先更新RT_GROUP_ICON信息      
        ret = UpdateResource(hUpdate, RT_GROUP_ICON, MAKEINTRESOURCE(0), MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT), (LPVOID)pGrpIconDir, nGSize);      
        if (!ret)      
        {      
            delete[] pIconDirEntry;      
            delete[] pGrpIconDir;      
            CloseHandle(hIconFile);      
            return;      
        }      
        //接着的是每一个Icon的信息存放      
        for(i=0;i<header.idCount;i++)      
        {      
            //Icon的字节数      
            nSize = pIconDirEntry[i].dwBytesInRes;      
            //偏移文件的指针到当前图标的开始处      
            dwRet=SetFilePointer(hIconFile, pIconDirEntry[i].dwImageOffset, NULL, FILE_BEGIN);      
            if (dwRet==INVALID_SET_FILE_POINTER)      
            {      
                break;      
            }      
            //准备pIconBytes来存放文件里的Byte信息用于更新到EXE中。      
            delete[] pIconBytes;      
            pIconBytes = new BYTE[nSize];      
            ret = ReadFile(hIconFile, (LPVOID)pIconBytes, nSize, &dwReserved, NULL);      
            if(!ret)      
            {      
                break;      
            }      
            //更新每一个ID对应的RT_ICON信息      
            ret = UpdateResourceW(hUpdate, RT_ICON, MAKEINTRESOURCE(pGrpIconDir->idEntries[i].nID), MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT), (LPVOID)pIconBytes, nSize);      
            if(!ret)      
            {      
                break;      
            }      
        }      
        //结束EXE资源的更新操作      
        if (pIconBytes!=NULL)      
        {      
            delete[] pIconBytes;      
        }      
        EndUpdateResourceW(hUpdate, false);      
    }      
    //清理资源并关闭Icon文件，到此更新操作结束！      
    delete[] pGrpIconDir;      
    delete[] pIconDirEntry;      
    CloseHandle(hIconFile); 

	m_isChangeExeIcon=true;
}

void CTCExeVersionManager::TCSetVersionProName(CString name)
{
	CTCRunTimeManager::setTCProjFile(_T("VersionInfo"),_T("ProductName"),m_productName);
	m_productName = name;
}

void CTCExeVersionManager::TCSetVersionDes(CString des)
{
	CTCRunTimeManager::setTCProjFile(_T("VersionInfo"),_T("FileDescription"),m_verDesInfo);
	m_verDesInfo = des;
}

void CTCExeVersionManager::TCSetVersionFile(CString file)
{
	CTCRunTimeManager::setTCProjFile(_T("VersionInfo"),_T("FileVersion"),m_fileInfo);
	m_fileInfo = file;
}

void CTCExeVersionManager::TCSetVersionProduct(CString pro)
{
	CTCRunTimeManager::setTCProjFile(_T("VersionInfo"),_T("ProductVersion"),m_productInfo);
	m_productInfo = pro;
}

void CTCExeVersionManager::TCSetVersionInternal(CString inter)
{
	CTCRunTimeManager::setTCProjFile(_T("VersionInfo"),_T("InternalName"),m_internalName);
	m_internalName = inter;
}

void CTCExeVersionManager::TCSetVersionCompany(CString com)
{
	CTCRunTimeManager::setTCProjFile(_T("VersionInfo"),_T("CompanyName"),m_companyName);
	m_companyName = com;
}

void CTCExeVersionManager::TCSetVersionLeagle(CString leagle)
{
	CTCRunTimeManager::setTCProjFile(_T("VersionInfo"),_T("LegalCopyright"),m_leagalInfo);
	m_leagalInfo = leagle;
}

void CTCExeVersionManager::TCResetVersionInfo()
{
	CTCExeVersionManager::m_productName=_T("TC");
	CTCExeVersionManager::m_verDesInfo=_T("TC");
	CTCExeVersionManager::m_fileInfo=_T("1.0.0.0");
	CTCExeVersionManager::m_productInfo=_T("1.0.0.0");
	CTCExeVersionManager::m_internalName=_T("TC");
	CTCExeVersionManager::m_companyName=_T("简单软件");
	CTCExeVersionManager::m_leagalInfo=_T("(C)<简单软件>保留所有版权");
}

void CTCExeVersionManager::TCReadVersionInfo(CString profile)
{
	CString csVersionProName,csVersionDes,csVersionFile,csVersionProduct,csVersionInternal,csVersionCompany,csVersionLeagle;
	::GetPrivateProfileStringW(_T("VersionInfo"),_T("ProductName"),_T(""),csVersionProName.GetBuffer(MAX_PATH),MAX_PATH,profile);
	csVersionProName.ReleaseBuffer();
	::GetPrivateProfileStringW(_T("VersionInfo"),_T("FileDescription"),_T(""),csVersionDes.GetBuffer(MAX_PATH),MAX_PATH,profile);
	csVersionDes.ReleaseBuffer();
	::GetPrivateProfileStringW(_T("VersionInfo"),_T("FileVersion"),_T(""),csVersionFile.GetBuffer(MAX_PATH),MAX_PATH,profile);
	csVersionFile.ReleaseBuffer();
	::GetPrivateProfileStringW(_T("VersionInfo"),_T("ProductVersion"),_T(""),csVersionProduct.GetBuffer(MAX_PATH),MAX_PATH,profile);
	csVersionProduct.ReleaseBuffer();
	::GetPrivateProfileStringW(_T("VersionInfo"),_T("InternalName"),_T(""),csVersionInternal.GetBuffer(MAX_PATH),MAX_PATH,profile);
	csVersionInternal.ReleaseBuffer();
	::GetPrivateProfileStringW(_T("VersionInfo"),_T("CompanyName"),_T(""),csVersionCompany.GetBuffer(MAX_PATH),MAX_PATH,profile);
	csVersionCompany.ReleaseBuffer();
	::GetPrivateProfileStringW(_T("VersionInfo"),_T("LegalCopyright"),_T(""),csVersionLeagle.GetBuffer(MAX_PATH),MAX_PATH,profile);
	csVersionLeagle.ReleaseBuffer();
	if(!csVersionProName.IsEmpty())
		m_productName = csVersionProName;
	if(!csVersionDes.IsEmpty())
		m_verDesInfo = csVersionDes;
	if(!csVersionFile.IsEmpty())
		m_fileInfo = csVersionFile;
	if(!csVersionProduct.IsEmpty())
		m_productInfo = csVersionProduct;
	if(!csVersionInternal.IsEmpty())
		m_internalName = csVersionInternal;
	if(!csVersionCompany.IsEmpty())
		m_companyName = csVersionCompany;
	if(!csVersionLeagle.IsEmpty())
		m_leagalInfo = csVersionLeagle;
}

void CTCExeVersionManager::changedExeVerInfo(CString lpExeName)
{
	CVersionInfo vi(lpExeName);
	if(vi.IsValid())
	{		
		vi[L"OriginalFilename"]=CTCRunTimeManager::g_ProjName+L".exe";
		if(!m_productName.IsEmpty())
			vi[L"ProductName"] = m_productName;
		if(!m_verDesInfo.IsEmpty())
			vi[L"FileDescription"] = m_verDesInfo;
		if(!m_fileInfo.IsEmpty())
		{
			CString help = m_fileInfo + _T(".");
			CStringArray version;
			while(!help.IsEmpty())
			{
				int pos = help.Find(_T("."));
				if(pos!=-1)
				{
					CString nume = help.Left(pos);
					version.Add(nume);
					help = help.Right(help.GetLength() - pos -1);
				}
			}
			vi.SetFileVersion(_ttoi(version[0]),_ttoi(version[1]),_ttoi(version[2]),_ttoi(version[3]));
		}    
		if(!m_productInfo.IsEmpty())
		{
			CString help = m_productInfo + _T(".");
			CStringArray versionpro;
			while(!help.IsEmpty())
			{
				int pos = help.Find(_T("."));
				if(pos!=-1)
				{
					CString nume = help.Left(pos);
					versionpro.Add(nume);
					help = help.Right(help.GetLength() - pos -1);
				}
			}
			vi.SetProductVersion(_ttoi(versionpro[0]),_ttoi(versionpro[1]),_ttoi(versionpro[2]),_ttoi(versionpro[3]));    
		}	
		if(!m_internalName.IsEmpty())
			vi[L"InternalName"] = m_internalName;
		if(!m_companyName.IsEmpty())
			vi[L"CompanyName"] = m_companyName;
		if(!m_leagalInfo.IsEmpty())
			vi[L"LegalCopyright"] = m_leagalInfo;
		if(vi.Save())
		{
			//修改成功则将版本信息保存到项目配置文件里面
			CTCRunTimeManager::setTCProjFile(_T("VersionInfo"),_T("ProductName"),m_productName);
			CTCRunTimeManager::setTCProjFile(_T("VersionInfo"),_T("FileDescription"),m_verDesInfo);
			CTCRunTimeManager::setTCProjFile(_T("VersionInfo"),_T("FileVersion"),m_fileInfo);
			CTCRunTimeManager::setTCProjFile(_T("VersionInfo"),_T("ProductVersion"),m_productInfo);
			CTCRunTimeManager::setTCProjFile(_T("VersionInfo"),_T("InternalName"),m_internalName);
			CTCRunTimeManager::setTCProjFile(_T("VersionInfo"),_T("CompanyName"),m_companyName);
			CTCRunTimeManager::setTCProjFile(_T("VersionInfo"),_T("LegalCopyright"),m_leagalInfo);
		}
	}
}







