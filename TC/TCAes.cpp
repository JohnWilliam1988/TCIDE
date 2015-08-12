#include "stdafx.h"
#include "TCAes.h"
#include <windows.h>
Aes::~Aes()
{
	
}
Aes::Aes()
{
	
}
////////////////////////////////////////////////////////////////////////////////////////////////
//构造函数
Aes::Aes(int keysize,unsigned char* keyBytes)
{
	memset(key,0,keysize);
	SetNbNkNr(keysize);                         //设置密钥块数，轮数 
	int testlen=strlen((char*)keyBytes);
	memcpy(key,keyBytes,testlen);				//字符串拷贝函数，把keyBytes的keysize个字符复制到key中
	KeyExpansion();								//密钥扩展，必须提前做的初始化
}
////////////////////////////////////////////////////////////////////////////////////////////////
void Aes::SetNbNkNr(int keySize)
{
//只用 32位加密
	Nb=4;
	Nk=8;    
	Nr=14;  

// 	if(keySize==Bits128)
// 	{
// 		Nk=4;    //4*4字节，128位密钥，10轮加密
// 		Nr=10;
// 	}
// 	else if(keySize==Bits192)
// 	{
// 		Nk=6;    //6*4字节，192位密钥，12轮加密
// 		Nr=12;
// 	}
// 	else if(keySize==Bits256)
// 	{
// 		Nk=8;    //8*4字节，256位密钥，14轮加密
// 		Nr=14;
// 	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
void Aes::KeyExpansion()
{

	memset(w,0,16*15);
	for(int row=0;row<Nk;row++)       //拷贝seed 密钥
	{
		w[4*row+0] =  key[4*row];
		w[4*row+1] =  key[4*row+1];
		w[4*row+2] =  key[4*row+2];
		w[4*row+3] =  key[4*row+3];
	}

	unsigned char temps[4]={0};
	unsigned char *temp = temps;
	for(int row=Nk;row<4*(Nr+1);row++)
	{
		temp[0]=w[4*row-4];     //当前列的前一列  
		temp[1]=w[4*row-3];
		temp[2]=w[4*row-2];
		temp[3]=w[4*row-1];
		if(row%Nk==0)           //逢nk时，对当前列的前一列作特殊处理
		{
			RotWord(temp,temp);
			SubWord(temp,temp);   //先移位，再代换，最后和轮常量异或
			
			temp[0] = (BYTE)( (int)temp[0] ^ (int) AesRcon[4*(row/Nk)+0] );   
			temp[1] = (BYTE)( (int)temp[1] ^ (int) AesRcon[4*(row/Nk)+1] );
			temp[2] = (BYTE)( (int)temp[2] ^ (int) AesRcon[4*(row/Nk)+2] );
			temp[3] = (BYTE)( (int)temp[3] ^ (int) AesRcon[4*(row/Nk)+3] );
        }
		else if ( Nk > 6 && (row % Nk == 4) )  //这个还没有搞清楚
        {
			SubWord(temp,temp);
        }     
        // w[row] = w[row-Nk] xor temp
        w[4*row+0] = (BYTE) ( (int) w[4*(row-Nk)+0] ^ (int)temp[0] );
		w[4*row+1] = (BYTE) ( (int) w[4*(row-Nk)+1] ^ (int)temp[1] );
		w[4*row+2] = (BYTE) ( (int) w[4*(row-Nk)+2] ^ (int)temp[2] );
		w[4*row+3] = (BYTE) ( (int) w[4*(row-Nk)+3] ^ (int)temp[3] );
	}  // for loop
	
}
////////////////////////////////////////////////////////////////////////////////////////////////
//密钥移位函数
void Aes::RotWord(unsigned char* word, unsigned char* temp)
{
	//byte* temp = new byte[4];
	temp[0] = word[1];
	temp[1] = word[2];
	temp[2] = word[3];
	temp[3] = word[0];
//	return temp;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//密钥字代换函数
void Aes::SubWord(unsigned char* word, unsigned char* temp)
{
	//byte* temp = new byte[4];
	for(int j=0;j<4;j++)
	{
		temp[j] = AesSbox[16*(word[j] >> 4)+(word[j] & 0x0f)];  //实际上也可以写成AesSbox[[j]];因为两者相等
	}
//	return temp;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//Aes加密函数
void Aes::Cipher(unsigned char* input, unsigned char* output)
{
	memset(&State[0][0],0,16);
	for(int i=0;i<4*Nb;i++)                        //这里是先写列后写行的，即输入是一列一列的进来的
	{
		State[i%4][i/4]=input[i];					//换成先写行后写列也是可以的，只要在输出时也是这样就可以了
	}
	AddRoundKey(0);									//轮密钥加
	
	for (int round = 1; round <= (Nr - 1); round++)  // main round loop
	{
        SubBytes();									//字节代换
        ShiftRows();								//行移位
        MixColumns();								//列混淆
        AddRoundKey(round);							//轮密钥加
	}  // main round loop
	
	SubBytes();										//字节代换
	ShiftRows();									//行移位
	AddRoundKey(Nr);								//轮密钥加
	
	// output = state
	for (int i = 0; i < (4 * Nb); i++)
	{
        output[i] =  State[i % 4][ i / 4];
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////
//Aes解密函数
void Aes::InvCipher(unsigned char* input,unsigned char* output)
{
	memset(&State[0][0],0,16);
	for (int i = 0; i < (4 * Nb); i++)
	{
		State[i % 4][ i / 4] = input[i];
	}
	
	AddRoundKey(Nr);
	
	for (int round = Nr-1; round >= 1; round--)  // main round loop
	{
        InvShiftRows();
        InvSubBytes();
        AddRoundKey(round);
        InvMixColumns();
	}  // end main round loop for InvCipher
	
	InvShiftRows();
	InvSubBytes();
	AddRoundKey(0);
	
	// output = state
	for (int i = 0; i < (4 * Nb); i++)
	{
        output[i] =  State[i % 4][ i / 4];
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
//轮密钥加
void Aes::AddRoundKey(int round)
{
	int i,j;  //i行 j列           //因为密钥w是一列一列排列的，即 k0 k4 k8 k12
	for(j=0;j<4;j++)			  //							  k1 k5 k9 k13
	{							  //							  k2 k6 k10k14
		for(i=0;i<4;i++)		  //							  k3 k7 k11k15
		{						  // 所以i行j列的下标是4*((round*4)+j)+i即16*round+4*j+i
			State[i][j]=(unsigned char)((int)State[i][j]^(int)w[4*((round*4)+j)+i]);  
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
//字节代换函数
void Aes::SubBytes()                              //Page 103
{
	int i,j;
	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			State[i][j]=AesSbox[State[i][j]];
			//因为 16*(State[i][j]>>4)+State[i][j]&0x0f=State[i][j]


		}
	}
}
void Aes::InvSubBytes()
{
	int i,j;
	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			State[i][j]=AesiSbox[State[i][j]]; //因为 16*(State[i][j]>>4)+State[i][j]&0x0f=State[i][j]
		}
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////
void Aes::ShiftRows()
{
	unsigned char temp[4*4];                                        //Page105
	int i,j;
	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			temp[4*i+j]=State[i][j];
		}
	}
	for(i=1;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(i==1)State[i][j]=temp[4*i+(j+1)%4];					//第一行左移1位
			else if(i==2)State[i][j]=temp[4*i+(j+2)%4];				//第二行左移2位
			else if(i==3)State[i][j]=temp[4*i+(j+3)%4];				//第三行左移3位
		}
	}
}
void Aes::InvShiftRows()
{
	unsigned char temp[4*4];
	int i,j;
	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			temp[4*i+j]=State[i][j];
		}
	}
	for(i=1;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			//if(i==1)State[i][j]=temp[4*i+(j-1)%4];    在此犯了一个错误 -1%4=-1 而不是3，所以采用了下面再加一个4的做法
			if(i==1)State[i][j]=temp[4*i+(j+3)%4];			//第一行右移1位 j-1+4=j+3
			else if(i==2)State[i][j]=temp[4*i+(j+2)%4];		//第二行右移2位 j-2+4=j+2
			else if(i==3)State[i][j]=temp[4*i+(j+1)%4];		//第三行右移3位 j-3+4=j+2
		}
	}
	
}
////////////////////////////////////////////////////////////////////////////////////////////////
void Aes::MixColumns()
{
	unsigned char temp[4*4];
	int i,j;
	for(j=0;j<4;j++)                                    //2 3 1 1  列混淆矩阵  Page107
	{													//1 2 3 1
		for(i=0;i<4;i++)								//1 1 2 3
		{												//3 1 1 2
			temp[4*i+j]=State[i][j];
		}
	}
	for(j=0;j<4;j++)
	{
		State[0][j] = (unsigned char) ( (int)gfmultby02(temp[0+j]) ^ (int)gfmultby03(temp[4*1+j]) ^
			(int)gfmultby01(temp[4*2+j]) ^ (int)gfmultby01(temp[4*3+j]) );
		State[1][j] = (unsigned char) ( (int)gfmultby01(temp[0+j]) ^ (int)gfmultby02(temp[4*1+j]) ^
			(int)gfmultby03(temp[4*2+j]) ^ (int)gfmultby01(temp[4*3+j]) );
		State[2][j] = (unsigned char) ( (int)gfmultby01(temp[0+j]) ^ (int)gfmultby01(temp[4*1+j]) ^
			(int)gfmultby02(temp[4*2+j]) ^ (int)gfmultby03(temp[4*3+j]) );
		State[3][j] = (unsigned char) ( (int)gfmultby03(temp[0+j]) ^ (int)gfmultby01(temp[4*1+j]) ^
			(int)gfmultby01(temp[4*2+j]) ^ (int)gfmultby02(temp[4*3+j]) );
	}
	
}
void Aes::InvMixColumns()
{
	unsigned char temp[4*4];
	int i,j;
	for (i = 0; i < 4; i++)  // copy State into temp[]
	{
        for (j = 0; j < 4; j++)                         //0e 0b 0d 09   逆变换矩阵 Page108
        {												//09 0e 0b 0d
			temp[4*i+j] =  State[i][j];					//0d 09 0e 0b
        }												//0b 0d 09 0e
	}
	
	for (j = 0; j < 4; j++)
	{
		State[0][j] = (unsigned char) ( (int)gfmultby0e(temp[j]) ^ (int)gfmultby0b(temp[4+j]) ^
			(int)gfmultby0d(temp[4*2+j]) ^ (int)gfmultby09(temp[4*3+j]) );
		State[1][j] = (unsigned char) ( (int)gfmultby09(temp[j]) ^ (int)gfmultby0e(temp[4+j]) ^
			(int)gfmultby0b(temp[4*2+j]) ^ (int)gfmultby0d(temp[4*3+j]) );
		State[2][j] = (unsigned char) ( (int)gfmultby0d(temp[j]) ^ (int)gfmultby09(temp[4+j]) ^
			(int)gfmultby0e(temp[4*2+j]) ^ (int)gfmultby0b(temp[4*3+j]) );
		State[3][j] = (unsigned char) ( (int)gfmultby0b(temp[j]) ^ (int)gfmultby0d(temp[4+j]) ^
			(int)gfmultby09(temp[4*2+j]) ^ (int)gfmultby0e(temp[4*3+j]) );
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char Aes::gfmultby01(unsigned char b)
{
	return b;
}
unsigned char Aes::gfmultby02(unsigned char b)
{
	if (b < 0x80)
        return (unsigned char)(int)(b <<1);
	else
        return (unsigned char)( (int)(b << 1) ^ (int)(0x1b) );
}

unsigned char Aes::gfmultby03(unsigned char b)
{
	return (unsigned char) ( (int)gfmultby02(b) ^ (int)b );
}

unsigned char Aes::gfmultby09(unsigned char b)
{
	return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^ (int)b );
}

unsigned char Aes::gfmultby0b(unsigned char b)
{
	return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
		(int)gfmultby02(b) ^ (int)b );
}

unsigned char Aes::gfmultby0d(unsigned char b)
{
	return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
		(int)gfmultby02(gfmultby02(b)) ^ (int)(b) );
}

unsigned char Aes::gfmultby0e(unsigned char b)
{
	return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
		(int)gfmultby02(gfmultby02(b)) ^(int)gfmultby02(b) );
}
////////////////////////////////////////////////////////////////////////////////////////////////


int Aes::char2num(char ch)
{
	if(ch>='0'&&ch<='9')return ch-'0';
	else if(ch>='a'&&ch<='f')return ch-'a'+10;
	return -1;
}

void my_Mid(std::string sourcestr, int beginstr, int ncout, std::string &rstr)
{
	int len = sourcestr.size() ;
	int leng = beginstr + ncout;
	if(leng  <= len )    //所到范围必须小于等于字串总长度
	{
		for(int i=beginstr; i<leng ; i++)
		{
			rstr += sourcestr[i];
		}
	}
	else               //剩余字符传入
	{
		for(int i=beginstr; i<len ; i++)
		{
			rstr += sourcestr[i];
		}
	}
}

void Aes::My_Encrypt(std::string stryuanwen,char *key,std::string &rstr)
{
	unsigned char inBuff[25]={0},ouBuff[25]={0};

	Aes aes(32,(unsigned char*)key);

	int leng = stryuanwen.size();

	std::string stryw;

	char strTmp[32]={0};
	for (int i=0;i<leng;i+=16)
	{
		memset(inBuff,0,25); //这里保证填充值一样,否则每次加密数据将不一样
		memset(ouBuff,0,25);
		stryw.clear();
		my_Mid(stryuanwen,i,16,stryw);
		memcpy(inBuff,stryw.c_str(),16);
		aes.Cipher(inBuff,ouBuff);
		for(int j=0;j<16;j++)
		{
			sprintf(strTmp,"%02x",ouBuff[j]);
			rstr+=strTmp;
			memset(strTmp,0,32);
		}
	}
}

void Aes::My_Decrypt(std::string strciphertext,char *key,std::string &rstr)
{
	unsigned char inBuff[64]={0},ouBuff[32]={0};

	Aes aes(32,(unsigned char*)key);

	int leng=strciphertext.size();
	
	char strTmp[32]={0};
	unsigned char temp[32]={0};

	std::string strct;
	
	for (int i=0;i<leng;i+=32)
	{
		strct.clear();
		memset(inBuff,0,64);
		memset(ouBuff,0,32);
		memset(temp,0,32);

		my_Mid(strciphertext,i,32,strct);
		int len = strct.size();
		memcpy(inBuff,strct.c_str(),len);

		for (int k=0;k<16;k++)
		{
			temp[k]=char2num(inBuff[2*k])*16+char2num(inBuff[2*k+1]);
			// 将字符字面表示的16进制ASCII码值转换成真正的ASCII码值
		}
		aes.InvCipher(temp,ouBuff); 
		sprintf(strTmp,"%s",ouBuff);
		rstr+=strTmp;
		memset(strTmp,0,32);
	}
}