#pragma once

class CTCExeVersionManager
{
public:
	//�ļ��汾��Ϣ
	static CString m_productName;
	static CString m_verDesInfo;
	static CString m_fileInfo;
	static CString m_productInfo;
	static CString m_internalName;	//�ڲ�����
	static CString m_companyName;
	static CString m_leagalInfo;
	//�ļ��汾��Ϣ

	//�޸�ͼ��
	static void changedExeIcon(LPCTSTR lpExeName, LPCTSTR lpIconFile);

	static void TCReadVersionInfo(CString profile);

	static void changedExeVerInfo(CString lpExeName);
	static void TCSetVersionProName(CString name);
	static void TCSetVersionDes(CString des);
	static void TCSetVersionFile(CString file);
	static void TCSetVersionProduct(CString pro);
	static void TCSetVersionInternal(CString inter);
	static void TCSetVersionCompany(CString com);
	static void TCSetVersionLeagle(CString leagle);
	static void TCResetVersionInfo();



private:
	static bool m_isChangeExeIcon;
	CTCExeVersionManager(void);
};

