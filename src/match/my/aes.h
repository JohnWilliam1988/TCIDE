/*
* oa protocol layer
* Copyright 2010, xunyou.
* protocol.cpp : Defines the entry point for the DLL application.
*
*/

// headed to implement Advanced Encryption Standard - Rijndael2
#ifndef PROTOCOL_BASE__AES__
#define PROTOCOL_BASE__AES__

#include <match/util/MatchDefs.hpp>

/* USAGE:
	1. Create a AES class (or more as necessary)
	2. Call class method SetParameters
	3. To encrypt, call method StartEncryption with the key, and then 
		call method Encrypt with enough space to store the proper size blocks.
	4. To decrypt, call method StartDecryption with the key, and then 
		call method Decrypt with enough space to store the proper size blocks.

   Alternatively, you can call EncryptBlock and DecryptBlock block to process blocksize 
   (default to 16 bytes) bytes at a time. It is recommended to use the Encrypt function 
   for multiple block encryption since it uses chaining modes that make the overall 
   stream much more secure than the default block based encryption, which by default would 
   be mode ECB.

   EXAMPLE: want to encrypt 37 bytes of data with 192 bit key, which will use 3 16 byte blocks
   AES aes;
   aes.SetParameters(192);
   aes.StartEncryption(key);
   aes.Encrypt(data,output,3); // note data and output must be at least 48 bytes!
*/

// todo - replace all types with u1byte, u4byte, etc

class PARSERS_EXPORT AES {

public:
	// the constructor - makes sure local things are initialized
	// it if fails, throws the string "Tables failed to initialize"
	AES(void);

	// multiple block encryption/decryption modes
	// See http://en.wikipedia.org/wiki/Block_cipher_modes_of_operation
	enum BlockMode {
		ECB = 0, // Electronic CodeBook	  - each block independent, weak
		CBC = 1  // Cipher Block Chaining - most secure
		// todo - CFB = 2, // Cipher FeedBack       - secure
		// todo - OFB = 3, // Output FeedBack		  - secure
		// todo - CTR = 4,  // Counter				  - allows midstream decryption, somewhat secure
		// todo - EAX = 5, - http://www.cs.berkeley.edu/~daw/papers/eprint-short-ae.pdf
		// todo - GCM = 6, - http://www.cryptobarn.com/papers/gcm-spec.pdf
		};

	// block and key size are in bits, legal values are 128, 192, and 256 independently.
	// NOTE: the AES standard only uses a blocksize of 128, so we default to that
	void SetParameters(int keylength, int blocklength = 128);
	
	// call this before any encryption with the key to use
	void StartEncryption(const unsigned char * key);
	// encrypt a single block (default 128 bits, or unsigned char[16]) of data
	void EncryptBlock(const unsigned char * datain, unsigned char * dataout);
	// Call this to encrypt any length data. Note the size is in BLOCKS, so you must
	// have enough space in datain and dataout to accomodate this. Pad your data before
	// calling, preferably using the padding methods listed below.
	// Decryption must use the same mode as the encryption.
	void Encrypt(const unsigned char * datain, unsigned char * dataout, unsigned long numBlocks, BlockMode mode = CBC);

	// call this before any decryption with the key to use
	void StartDecryption(const unsigned char * key);
	// decrypt a single block (default 128 bits, or unsigned char[16]) of data
	void DecryptBlock(const unsigned char * datain, unsigned char * dataout);
	// Call this to decrypt any length data. Note the size is in BLOCKS, so you must
	// have enough space in datain and dataout to accommodate this. Pad your data before
	// calling, preferably using the padding methods listed below. You must know the desired
	// length of the output data, since all the blocks are returned decrypted. 
	// Encryption must use the same mode as the decryption.
	void Decrypt(const unsigned char * datain, unsigned char * dataout, unsigned long numBlocks, BlockMode mode = CBC);

private:

	int Nb,Nk;    // block and key length / 32, should be 4,6,or 8
	int Nr;       // number of rounds

	unsigned char W[4*8*15];   // the expanded key

	// Key expansion code - makes local copy
	void KeyExpansion(const unsigned char * key);

};

//一个字节对齐
#pragma pack(push,2)	
#pragma warning( push )
#pragma warning(disable: 4200) 

//AES 加密帧
struct stAesBuf
{
	unsigned short SrcLen;
	unsigned short BufLen;
	unsigned char  Buffer[0];
};

#define HeadLen_stAesBuf offsetof(stAesBuf, Buffer)
#define allsize_stAesBuf(aesBuf) (aesBuf->BufLen + HeadLen_stAesBuf)


// Restore to default setting
#pragma warning( pop ) 
#pragma pack(pop)

class PARSERS_EXPORT MatchAES : AES {

public:
	// the constructor - makes sure local things are initialized
	MatchAES(void);

	int Encrypt2(const unsigned char* inBuf, int inLen, unsigned char* outBuf, long *poutLen, __in const unsigned char* key);
	int Decrypt2(const unsigned char* inBuf, long inLen, unsigned char* outBuf, long *poutLen, __in const unsigned char* key);

	inline int GetBlockSize() { return BLOCK_SIZE; };

private:
	const int BLOCK_SIZE;
};

#endif //  PROTOCOL_BASE__AES__
// end - AES.h