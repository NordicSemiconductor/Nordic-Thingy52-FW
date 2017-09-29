//-----------------------------------------------------------------------------------
//    The confidential and proprietary information contained in this file may 
//    only be used by a person authorised under and to the extent permitted
//    by a subsisting licensing agreement from ARM Limited or its affiliates 
//    or between you and a party authorised by ARM
//
//           (C) COPYRIGHT [2016] ARM Limited or its affiliates
//               ALL RIGHT RESERVED
//
//    This entire notice must be reproduced on all copies of this file
//    and copies of this files may only be made by a person if such person is
//    permitted to do so under the terms of a subsisting license agreement
//    from ARM Limited or its affiliates or between you and a party authorised by ARM
//-----------------------------------------------------------------------------------

#ifndef CRYS_RSA_TYPES_H
#define CRYS_RSA_TYPES_H

#include "crys_hash.h"
#include "crys_pka_defs_hw.h"
#include "ssi_pal_types.h"
#include "ssi_pal_compiler.h"

#ifdef DX_SOFT_KEYGEN
#include "ccsw_crys_rsa_shared_types.h"
#endif


#ifdef __cplusplus
extern "C"
{
#endif
/*!
@file
@brief This file contains all of the enums and definitions that are used for the CRYS RSA APIs.
*/

/************************ Defines ******************************/

/* Adjust the context size to the HAH context size in TEE */
#define CRYS_PKA_RSA_HASH_CTX_SIZE_IN_WORDS CRYS_HASH_USER_CTX_SIZE_IN_WORDS

/* maximal allowed key size in words */
#define CRYS_RSA_MAX_VALID_KEY_SIZE_VALUE_IN_BYTES    (CRYS_RSA_MAX_VALID_KEY_SIZE_VALUE_IN_BITS / SASI_BITS_IN_BYTE)

#define CRYS_RSA_MIN_VALID_KEY_SIZE_VALUE_IN_BITS              512
#define CRYS_RSA_VALID_KEY_SIZE_MULTIPLE_VALUE_IN_BITS         256


#define CRYS_RSA_MAX_KEY_GENERATION_SIZE_BITS   CRYS_RSA_MAX_KEY_GENERATION_HW_SIZE_BITS

/* FIPS 184-4 definitions for allowed RSA and FFC DH key sizes */
#define CRYS_RSA_FIPS_KEY_SIZE_1024_BITS   1024
#define CRYS_RSA_FIPS_KEY_SIZE_2048_BITS   2048
#define CRYS_RSA_FIPS_KEY_SIZE_3072_BITS   3072
#define CRYS_RSA_FIPS_MODULUS_SIZE_BITS    CRYS_RSA_FIPS_KEY_SIZE_2048_BITS

#define CRYS_DH_FIPS_KEY_SIZE_1024_BITS    1024
#define CRYS_DH_FIPS_KEY_SIZE_2048_BITS    2048


/* If the salt length is not available in verify than the user can use this define and the algorithm will */
/* calculate the salt length alone*/
/* Security Note: it is recommended not to use this flag and to support the Salt length on each verify*/
#define CRYS_RSA_VERIFY_SALT_LENGTH_UNKNOWN                     0xFFFF

/* The minimum exponents values */
#define CRYS_RSA_MIN_PUB_EXP_VALUE  3
#define CRYS_RSA_MIN_PRIV_EXP_VALUE 1

/* The maximum buffer size for the 'H' value */

#define CRYS_RSA_TMP_BUFF_SIZE (CRYS_RSA_OAEP_ENCODE_MAX_MASKDB_SIZE + CRYS_RSA_OAEP_ENCODE_MAX_SEEDMASK_SIZE + CRYS_PKA_RSA_HASH_CTX_SIZE_IN_WORDS*sizeof(uint32_t) + sizeof(CRYS_HASH_Result_t))

#define CRYS_PKCS1_HashFunc_t CRYS_HASH_OperationMode_t


#define CRYS_RSA_OAEP_MAX_HLEN							CRYS_HASH_SHA512_DIGEST_SIZE_IN_BYTES

/* MGF1 declarations */
#define CRYS_RSA_MGF_2_POWER_32              			65535 /*!< \internal 0xFFFF This is the 2^32 of the 2^32*hLen boundary check */
#define CRYS_RSA_SIZE_OF_T_STRING_BYTES	    			(CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS*sizeof(uint32_t))

/***********************************************************
 *
 * RSA PKCS#1 v2.1 DEFINES
 *
 ***********************************************************/
#define CRYS_RSA_OAEP_ENCODE_MAX_SEEDMASK_SIZE 			CRYS_RSA_OAEP_MAX_HLEN
#define CRYS_RSA_PSS_SALT_LENGTH 				CRYS_RSA_OAEP_MAX_HLEN
#define CRYS_RSA_PSS_PAD1_LEN					8

#define CRYS_RSA_OAEP_ENCODE_MAX_MASKDB_SIZE  			(CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS*sizeof(uint32_t)) /*!< \internal For OAEP Encode; the max size is emLen */
#define CRYS_RSA_OAEP_DECODE_MAX_DBMASK_SIZE  			(CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS*sizeof(uint32_t)) /*!< \internal For OAEP Decode; the max size is emLen */

/************************ Enums ********************************/

/*! Defines the enum for the HASH operation mode. */
typedef enum
{
	CRYS_RSA_HASH_MD5_mode  = 0,	/*!< MD5 mode for PKCS1 v1.5 only. */
	CRYS_RSA_HASH_SHA1_mode = 1,	/*!< HASH SHA1. */
	CRYS_RSA_HASH_SHA224_mode = 2,  /*!< HASH SHA224. */
	CRYS_RSA_HASH_SHA256_mode = 3,  /*!< HASH SHA256. */
	CRYS_RSA_HASH_SHA384_mode = 4,  /*!< HASH SHA384. */
	CRYS_RSA_HASH_SHA512_mode = 5,	/*!< HASH SHA512. */
	CRYS_RSA_After_MD5_mode = 6,		/*!< For PKCS1 v1.5 only when the data is already hashed with MD5. */
	CRYS_RSA_After_SHA1_mode = 7,	/*!< To be used when the data is already hashed with SHA1. */
	CRYS_RSA_After_SHA224_mode = 8,	/*!< To be used when the data is already hashed with SHA224. */
	CRYS_RSA_After_SHA256_mode = 9,	/*!< To be used when the data is already hashed with SHA256. */
	CRYS_RSA_After_SHA384_mode = 10,	/*!< To be used when the data is already hashed with SHA384. */
	CRYS_RSA_After_SHA512_mode = 11,	/*!< To be used when the data is already hashed with SHA512. */
	CRYS_RSA_After_HASH_NOT_KNOWN_mode = 12,    /*!< \internal used only for PKCS#1 Ver 1.5 - possible to perform verify operation without hash mode input,
						the hash mode is derived from the signature.*/
	CRYS_RSA_HASH_NO_HASH_mode = 13,	/*!< Used for PKCS1 v1.5 Encrypt and Decrypt.*/
	CRYS_RSA_HASH_NumOfModes,

	CRYS_RSA_HASH_OpModeLast  = 0x7FFFFFFF,

}CRYS_RSA_HASH_OpMode_t;


/*! Defines the enum of the RSA decryption mode. */
typedef enum
{
	CRYS_RSA_NoCrt = 10,
	CRYS_RSA_Crt   = 11,

	CRYS_RSADecryptionNumOfOptions,

	CRYS_RSA_DecryptionModeLast= 0x7FFFFFFF,

}CRYS_RSA_DecryptionMode_t;

/* the Key source enum */
typedef enum
{
	CRYS_RSA_ExternalKey = 1,
	CRYS_RSA_InternalKey = 2,

	CRYS_RSA_KeySourceLast= 0x7FFFFFFF,

}CRYS_RSA_KeySource_t;

/*! MGF values. */
typedef enum
{
	CRYS_PKCS1_MGF1 = 0,
	CRYS_PKCS1_NO_MGF = 1,
	CRYS_RSA_NumOfMGFFunctions,

	CRYS_PKCS1_MGFLast= 0x7FFFFFFF,

}CRYS_PKCS1_MGF_t;

/*! Defines the enum of the various PKCS1 versions. */
typedef enum
{
	CRYS_PKCS1_VER15 = 0,
	CRYS_PKCS1_VER21 = 1,

	CRYS_RSA_NumOf_PKCS1_versions,

	CRYS_PKCS1_versionLast= 0x7FFFFFFF,

}CRYS_PKCS1_version;


/* enum defining primality testing mode in Rabin-Miller
   and Lucas-Lehmer tests */
typedef enum
{
        /* P and Q primes */
	CRYS_RSA_PRIME_TEST_MODE     = 0,
        /* auxiliary primes */
//        CRYS_RSA_AUX_PRIME_TEST_MODE = 1,
        /* FFC (DH, DSA) primes */
	CRYS_DH_PRIME_TEST_MODE      = 1,

	CRYS_RSA_DH_PRIME_TEST_OFF_MODE

}CRYS_RSA_DH_PrimeTestMode_t;

/************************ Public and private key database Structs ******************************/

/* .................. The public key definitions ...................... */
/* --------------------------------------------------------------------- */

/* The public key data structure */
typedef struct
{
	/* The RSA modulus buffer and its size in bits */
	uint32_t n[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t nSizeInBits;

	/* The RSA public exponent buffer and its size in bits */
	uint32_t e[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t eSizeInBits;

	/* #include the specific fields that are used by the low level */
	uint32_t crysRSAIntBuff[CRYS_PKA_BARRETT_MOD_TAG_BUFF_SIZE_IN_WORDS];

}CRYSRSAPubKey_t;

/*! The public key's user structure prototype. */
typedef struct CRYS_RSAUserPubKey_t
{
	uint32_t valid_tag;
	uint32_t  PublicKeyDbBuff[ sizeof(CRYSRSAPubKey_t)/sizeof(uint32_t) + 1 ];


}CRYS_RSAUserPubKey_t;

/* .................. The private key definitions ...................... */
/* --------------------------------------------------------------------- */

/* The private key on non-CRT mode data structure */
typedef struct
{
	/* The RSA private exponent buffer and its size in bits */
	uint32_t d[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t dSizeInBits;

	/* The RSA public exponent buffer and its size in bits */
	uint32_t e[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t eSizeInBits;

}CRYSRSAPrivNonCRTKey_t;

/* The private key on CRT mode data structure */
#ifndef CRYS_NO_RSA_SMALL_CRT_BUFFERS_SUPPORT
/* use small CRT buffers */
typedef struct
{
	/* The first factor buffer and size in bits */
	uint32_t P[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS/2];
	uint32_t PSizeInBits;

	/* The second factor buffer and its size in bits */
	uint32_t Q[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS/2];
	uint32_t QSizeInBits;

	/* The first CRT exponent buffer and its size in bits */
	uint32_t dP[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS/2];
	uint32_t dPSizeInBits;

	/* The second CRT exponent buffer and its size in bits */
	uint32_t dQ[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS/2];
	uint32_t dQSizeInBits;

	/* The first CRT coefficient buffer and its size in bits */
	uint32_t qInv[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS/2];
	uint32_t qInvSizeInBits;

}CRYSRSAPrivCRTKey_t;

/* size of CRYSRSAPrivCRTKey_t structure in words (used for temp buffers allocation) */
#define CRYS_RSA_SIZE_IN_WORDS_OF_CRYSRSAPrivCRTKey_t  (CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS*7/2 + 5)

#else /* use large CRT buffers */
typedef struct
{
	/* The first factor buffer and size in bits */
	uint32_t P[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t PSizeInBits;

	/* The second factor buffer and its size in bits */
	uint32_t Q[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t QSizeInBits;

	/* The first CRT exponent buffer and its size in bits */
	uint32_t dP[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t dPSizeInBits;

	/* The second CRT exponent buffer and its size in bits */
	uint32_t dQ[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t dQSizeInBits;

	/* The first CRT coefficient buffer and its size in bits */
	uint32_t qInv[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t qInvSizeInBits;

}CRYSRSAPrivCRTKey_t;

/* size of CRYSRSAPrivCRTKey_t structure in words (used for temp buffers allocation) */
#define CRYS_RSA_SIZE_IN_WORDS_OF_CRYSRSAPrivCRTKey_t  (CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS*5 + 5)

#endif

/* The private key data structure */
typedef struct
{
	/* The RSA modulus buffer and its size in bits */
	uint32_t n[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t nSizeInBits;

	/* The decryption operation mode */
	CRYS_RSA_DecryptionMode_t OperationMode;

	/* the source ( Import or Keygen ) */
	CRYS_RSA_KeySource_t KeySource;


	/* The union between the CRT and non-CRT data structures */
	union
	{
		CRYSRSAPrivNonCRTKey_t NonCrt;
		CRYSRSAPrivCRTKey_t    Crt;
	}PriveKeyDb;

	/* #include specific fields that are used by the low level */
	uint32_t crysRSAPrivKeyIntBuff[CRYS_PKA_PRIV_KEY_BUFF_SIZE_IN_WORDS];

}CRYSRSAPrivKey_t;

/*! The private key's user structure prototype. */
typedef struct CRYS_RSAUserPrivKey_t
{
   uint32_t valid_tag;
   uint32_t  PrivateKeyDbBuff[ sizeof(CRYSRSAPrivKey_t)/sizeof(uint32_t) + 1 ] ;

}CRYS_RSAUserPrivKey_t;

/*! Temporary buffers for RSA usage. */
typedef struct CRYS_RSAPrimeData_t
{
   /* The aligned input and output data buffers */
	uint32_t DataIn[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t DataOut[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];

	uint8_t  InternalBuff[CRYS_RSA_TMP_BUFF_SIZE] SASI_PAL_COMPILER_ALIGN (4);

}CRYS_RSAPrimeData_t;

/* the KG data type */
typedef union CRYS_RSAKGData_t
{
	struct
	{
		/* The aligned input and output data buffers */
		uint32_t p[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS / 2];
		uint32_t q[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS / 2];
		union {
			/* #include specific fields that are used by the low level */
			uint32_t crysRSAKGDataIntBuff[CRYS_PKA_KGDATA_BUFF_SIZE_IN_WORDS];
			#ifdef DX_SOFT_KEYGEN
			/* # added for compatibility with size of KGData SW type */
			uint32_t TempbuffExp[PKI_KEY_GEN_TEMP_BUFF_SIZE_WORDS];
			#endif
		}kg_buf;
	}KGData;

	union {
	        struct {
			CRYS_RSAPrimeData_t   	PrimData;
		}primExt;
		#ifdef DX_SOFT_KEYGEN
		/* # added for compatibility with size of SW CRYSRSAPrivKey_t type */
	        SW_Shared_CRYS_RSAPrimeData_t SW_Shared_PrimData;
		#endif
	}prim;
}CRYS_RSAKGData_t;

 /*************
 *	RSA contexts
 **************/
/************************ CRYS RSA struct for Private Key ******************************/



typedef struct
{

	/* A union for the Key Object - there is no need for the Private
	key and the Public key to be in the memory at the same time */
	CRYS_RSAUserPrivKey_t PrivUserKey;

	/* RSA PKCS#1 Version 1.5/2.1 */
	uint8_t   PKCS1_Version;

	/* MGF 2 use for the PKCS1 Ver 2.1 Sign/Verify operation */
	uint8_t MGF_2use;

	/*The Salt random intended length for PKCS#1 PSS Ver 2.1*/
	uint16_t SaltLen;

	/*Struct for the Exp evaluation*/
	CRYS_RSAPrimeData_t  PrimeData;

	/* User Context of the Hash Context - Hash functions get as input a CRYS_HASHUserContext_t */
	//CRYS_HASHUserContext_t  HashUserContext;
	uint32_t CRYSPKAHashCtxBuff[CRYS_PKA_RSA_HASH_CTX_SIZE_IN_WORDS];
	CRYS_HASH_Result_t        HASH_Result;
	uint16_t                  HASH_Result_Size;     /*in words*/
	CRYS_RSA_HASH_OpMode_t    RsaHashOperationMode; /*RSA HASH enum. */
	CRYS_HASH_OperationMode_t HashOperationMode;    /*CRYS HASH enum. */
	uint16_t                  HashBlockSize;        /*in words*/
	bool doHash;

	/* Used for sensitive data manipulation in the context space, which is safer and which saves stack space */
	uint32_t EBD[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t EBDSizeInBits;

	/* Used for sensitive data manipulation in the context space, which is safer and which saves stack space */
	uint8_t  T_Buf[CRYS_RSA_SIZE_OF_T_STRING_BYTES];
	uint16_t T_BufSize;

	/*Buffer for the use of the Ber encoder in the case of PKCS#1 Ver 1.5 - in Private context only*/
	uint32_t  BER[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint16_t  BERSize;

	/* This Buffer is added for encrypting the context. Note: This block must be
	at the end of the context. */
	uint8_t DummyBufAESBlockSize[16];

}RSAPrivContext_t;

/* Temporary buffers for the RSA usage */
typedef struct CRYS_RSAPrivUserContext_t
{
	uint32_t valid_tag;
	uint32_t AES_iv; /* For the use of the AES CBC mode of Encryption and Decryption of the context in CCM */
	uint8_t  context_buff[ sizeof(RSAPrivContext_t) + sizeof(uint32_t)] SASI_PAL_COMPILER_ALIGN (4);  /* must be aligned to 4 */

}CRYS_RSAPrivUserContext_t;


/************************ CRYS RSA struct for Public Key ******************************/


typedef struct
{

	/* A union for the Key Object - there is no need for the Private
	key and the Public key to be in the memory in the same time */
	CRYS_RSAUserPubKey_t PubUserKey;

	/* public key size in bytes */
	uint32_t nSizeInBytes;

	/* RSA PKCS#1 Version 1.5/2.1 */
	uint8_t   PKCS1_Version;

	/* MGF 2 use for the PKCS1 Ver 2.1 Sign/Verify operation */
	uint8_t MGF_2use;

	/*The Salt random intended length for PKCS#1 PSS Ver 2.1*/
	uint16_t SaltLen;

	/*Struct for the Exp evaluation*/
	CRYS_RSAPrimeData_t  PrimeData;

	/* User Context of the Hash Context - Hash functions get as input a CRYS_HASHUserContext_t */
	uint32_t CRYSPKAHashCtxBuff[CRYS_PKA_RSA_HASH_CTX_SIZE_IN_WORDS];

	CRYS_HASH_Result_t        HASH_Result;
	uint16_t                  HASH_Result_Size; /* denotes the length, in words, of the hash function output */
	CRYS_RSA_HASH_OpMode_t    RsaHashOperationMode; /*RSA HASH enum. */
	CRYS_HASH_OperationMode_t HashOperationMode;    /*CRYS HASH enum. */
	uint16_t                  HashBlockSize; /*in words*/
	bool doHash;

	/* Used for sensitive data manipulation in the context space, which is safer and which saves stack space */
	uint32_t EBD[CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t EBDSizeInBits;

	/* Used for sensitive data manipulation in the context space, which is safer and which saves stack space */
	uint8_t T_Buf[CRYS_RSA_SIZE_OF_T_STRING_BYTES];
	uint16_t T_BufSize;

	/* This Buffer is added for encrypting the context ( encrypted part's size must be 0 modulo 16).
	* Note: This block must be at the end of the context.
	*/
	uint8_t DummyBufAESBlockSize[16];

}RSAPubContext_t;

/*! Temporary buffers for the RSA usage. */
typedef struct CRYS_RSAPubUserContext_t
{
   uint32_t valid_tag;
   uint32_t AES_iv; /* For the use of the AES CBC mode of Encryption and Decryption of the context in CCM */
   uint32_t  context_buff[ sizeof(RSAPubContext_t)/sizeof(uint32_t) + 1] ;

}CRYS_RSAPubUserContext_t;



/*! Required for internal FIPS verification for RSA key generation. */
typedef struct CRYS_RSAKGFipsContext_t{
	CRYS_RSAPrimeData_t 	primData;
	uint8_t			decBuff[((CRYS_RSA_MIN_VALID_KEY_SIZE_VALUE_IN_BITS/SASI_BITS_IN_BYTE) - 2*(CRYS_HASH_SHA1_DIGEST_SIZE_IN_BYTES) -2)];
	uint8_t			encBuff[CRYS_RSA_FIPS_MODULUS_SIZE_BITS/SASI_BITS_IN_BYTE];
}CRYS_RSAKGFipsContext_t;

/*! Required for internal FIPS verification for RSA KAT.      *
*  The RSA KAT tests defined for scheme 2.1 with modulus key size of 2048.      */
typedef struct CRYS_RSAFipsKatContext_t{
	union {
		CRYS_RSAUserPubKey_t 	userPubKey; 	// used for RsaEnc and RsaVerify
		CRYS_RSAUserPrivKey_t   userPrivKey;  // used for RsaDec and RsaSign
	}userKey;
	union {
		CRYS_RSAPrivUserContext_t userPrivContext;  // used for RsaSign
		CRYS_RSAPubUserContext_t userPubContext;  // used for RsaVerify
		CRYS_RSAPrimeData_t 	primData;  // used for RsaEnc and RsaDec
	}userContext;
	union {
		struct {   // used for RsaEnc and RsaDec
			uint8_t		encBuff[CRYS_RSA_FIPS_MODULUS_SIZE_BITS/SASI_BITS_IN_BYTE];
			uint8_t		decBuff[((CRYS_RSA_FIPS_MODULUS_SIZE_BITS/SASI_BITS_IN_BYTE) - 2*(CRYS_HASH_SHA1_DIGEST_SIZE_IN_BYTES) -2)];
		}userOaepData;
		uint8_t			signBuff[CRYS_RSA_FIPS_MODULUS_SIZE_BITS/SASI_BITS_IN_BYTE]; // used for RsaSign and RsaVerify
	}userData;
}CRYS_RSAFipsKatContext_t;


#ifdef __cplusplus
}
#endif

#endif
