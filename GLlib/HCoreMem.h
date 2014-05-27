#ifndef _HCoreMEM_H_
#define _HCoreMEM_H_

void *HCoreAllocate16BytesAlignedMemory(size_t size);
void HCoreRelease16BytesAlignedMemory(void *memory);
void *operator new[](size_t size);
void operator delete[](void *p);

#endif // _HCoreMEM_H_
