

#ifndef __LZ4_H__
#define __LZ4_H__

#include <linux/types.h>
#include <linux/string.h>	 



#define LZ4_MEMORY_USAGE 14

#define LZ4_MAX_INPUT_SIZE	0x7E000000 
#define LZ4_COMPRESSBOUND(isize)	(\
	(unsigned int)(isize) > (unsigned int)LZ4_MAX_INPUT_SIZE \
	? 0 \
	: (isize) + ((isize)/255) + 16)

#define LZ4_ACCELERATION_DEFAULT 1
#define LZ4_HASHLOG	 (LZ4_MEMORY_USAGE-2)
#define LZ4_HASHTABLESIZE (1 << LZ4_MEMORY_USAGE)
#define LZ4_HASH_SIZE_U32 (1 << LZ4_HASHLOG)

#define LZ4HC_MIN_CLEVEL			3
#define LZ4HC_DEFAULT_CLEVEL			9
#define LZ4HC_MAX_CLEVEL			16

#define LZ4HC_DICTIONARY_LOGSIZE 16
#define LZ4HC_MAXD (1<<LZ4HC_DICTIONARY_LOGSIZE)
#define LZ4HC_MAXD_MASK (LZ4HC_MAXD - 1)
#define LZ4HC_HASH_LOG (LZ4HC_DICTIONARY_LOGSIZE - 1)
#define LZ4HC_HASHTABLESIZE (1 << LZ4HC_HASH_LOG)
#define LZ4HC_HASH_MASK (LZ4HC_HASHTABLESIZE - 1)


#define LZ4_STREAMSIZE_U64 ((1 << (LZ4_MEMORY_USAGE - 3)) + 4)
#define LZ4_STREAMSIZE	(LZ4_STREAMSIZE_U64 * sizeof(unsigned long long))

#define LZ4_STREAMHCSIZE        262192
#define LZ4_STREAMHCSIZE_SIZET (262192 / sizeof(size_t))

#define LZ4_STREAMDECODESIZE_U64	4
#define LZ4_STREAMDECODESIZE		 (LZ4_STREAMDECODESIZE_U64 * \
	sizeof(unsigned long long))


typedef struct {
	uint32_t hashTable[LZ4_HASH_SIZE_U32];
	uint32_t currentOffset;
	uint32_t initCheck;
	const uint8_t *dictionary;
	uint8_t *bufferStart;
	uint32_t dictSize;
} LZ4_stream_t_internal;
typedef union {
	unsigned long long table[LZ4_STREAMSIZE_U64];
	LZ4_stream_t_internal internal_donotuse;
} LZ4_stream_t;


typedef struct {
	unsigned int	 hashTable[LZ4HC_HASHTABLESIZE];
	unsigned short	 chainTable[LZ4HC_MAXD];
	
	const unsigned char *end;
	
	const unsigned char *base;
	
	const unsigned char *dictBase;
	
	unsigned int	 dictLimit;
	
	unsigned int	 lowLimit;
	
	unsigned int	 nextToUpdate;
	unsigned int	 compressionLevel;
} LZ4HC_CCtx_internal;
typedef union {
	size_t table[LZ4_STREAMHCSIZE_SIZET];
	LZ4HC_CCtx_internal internal_donotuse;
} LZ4_streamHC_t;


typedef struct {
	const uint8_t *externalDict;
	size_t extDictSize;
	const uint8_t *prefixEnd;
	size_t prefixSize;
} LZ4_streamDecode_t_internal;
typedef union {
	unsigned long long table[LZ4_STREAMDECODESIZE_U64];
	LZ4_streamDecode_t_internal internal_donotuse;
} LZ4_streamDecode_t;


#define LZ4_MEM_COMPRESS	LZ4_STREAMSIZE
#define LZ4HC_MEM_COMPRESS	LZ4_STREAMHCSIZE




static inline int LZ4_compressBound(size_t isize)
{
	return LZ4_COMPRESSBOUND(isize);
}


int LZ4_compress_default(const char *source, char *dest, int inputSize,
	int maxOutputSize, void *wrkmem);


int LZ4_compress_fast(const char *source, char *dest, int inputSize,
	int maxOutputSize, int acceleration, void *wrkmem);


int LZ4_compress_destSize(const char *source, char *dest, int *sourceSizePtr,
	int targetDestSize, void *wrkmem);




int LZ4_decompress_fast(const char *source, char *dest, int originalSize);


int LZ4_decompress_safe(const char *source, char *dest, int compressedSize,
	int maxDecompressedSize);


int LZ4_decompress_safe_partial(const char *source, char *dest,
	int compressedSize, int targetOutputSize, int maxDecompressedSize);




int LZ4_compress_HC(const char *src, char *dst, int srcSize, int dstCapacity,
	int compressionLevel, void *wrkmem);


void LZ4_resetStreamHC(LZ4_streamHC_t *streamHCPtr, int compressionLevel);


int	LZ4_loadDictHC(LZ4_streamHC_t *streamHCPtr, const char *dictionary,
	int dictSize);


int LZ4_compress_HC_continue(LZ4_streamHC_t *streamHCPtr, const char *src,
	char *dst, int srcSize, int maxDstSize);


int LZ4_saveDictHC(LZ4_streamHC_t *streamHCPtr, char *safeBuffer,
	int maxDictSize);




void LZ4_resetStream(LZ4_stream_t *LZ4_stream);


int LZ4_loadDict(LZ4_stream_t *streamPtr, const char *dictionary,
	int dictSize);


int LZ4_saveDict(LZ4_stream_t *streamPtr, char *safeBuffer, int dictSize);


int LZ4_compress_fast_continue(LZ4_stream_t *streamPtr, const char *src,
	char *dst, int srcSize, int maxDstSize, int acceleration);


int LZ4_setStreamDecode(LZ4_streamDecode_t *LZ4_streamDecode,
	const char *dictionary, int dictSize);


int LZ4_decompress_safe_continue(LZ4_streamDecode_t *LZ4_streamDecode,
	const char *source, char *dest, int compressedSize,
	int maxDecompressedSize);


int LZ4_decompress_fast_continue(LZ4_streamDecode_t *LZ4_streamDecode,
	const char *source, char *dest, int originalSize);


int LZ4_decompress_safe_usingDict(const char *source, char *dest,
	int compressedSize, int maxDecompressedSize, const char *dictStart,
	int dictSize);


int LZ4_decompress_fast_usingDict(const char *source, char *dest,
	int originalSize, const char *dictStart, int dictSize);

#endif
