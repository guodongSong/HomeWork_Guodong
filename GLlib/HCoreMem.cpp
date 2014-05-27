#include <stdlib.h>

static unsigned int g_MemoryAllocationAlignment = 16;
static unsigned int g_MemoryAllocationMask = ~(16-1);

void HCoreAllocateMemoryAlignment(int alignment)
{
	// power 2
	g_MemoryAllocationAlignment = alignment;
	g_MemoryAllocationMask = ~(alignment-1);
}

// 分配一块内存空间, 并确保返回的内存位址是g_MemoryAllocationAlignment的倍数
void *HCoreAllocateAlignedMemory(size_t size)
{
	if ( size==0 ) return NULL;
	//unsigned char *block = new unsigned char[size + g_MemoryAllocationAlignment];
	unsigned char *block = (unsigned char *)malloc(size + g_MemoryAllocationAlignment);
	if ( block )
	{
		unsigned int *aligned = (unsigned int*)((unsigned int)(block + g_MemoryAllocationAlignment + 4) & g_MemoryAllocationMask);
		aligned[-1] = (unsigned int) block;
		return aligned;
	}
	else
	{
		return NULL;
	}
}

// 释放通过HCoreAllocateAlignedMemory分配的内存空间
void HCoreReleaseAlignedMemory(void *memory)
{
	if ( memory==NULL ) return;

	unsigned int *pMemory = (unsigned int *)memory;
	delete (unsigned int *)pMemory[-1];
}

// 分配一块内存空间, 并确保返回的内存位址是16 bytes的倍数
void *HCoreAllocate16BytesAlignedMemory(size_t size)
{
	HCoreAllocateMemoryAlignment(16);
	return HCoreAllocateAlignedMemory(size);
}

// 释放通过HCoreAllocate16BytesAlignedMemory分配的内存空间
void HCoreRelease16BytesAlignedMemory(void *memory)
{
	HCoreReleaseAlignedMemory(memory);
}

void *operator new[](size_t size)
{
	return HCoreAllocateAlignedMemory(size);
}

void operator delete[](void *p)
{
	HCoreReleaseAlignedMemory(p);
}