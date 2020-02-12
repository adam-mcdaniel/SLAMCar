// PX2GeneralAlloctor.cpp

#include "PX2GeneralAlloctor.hpp"
#include "PX2ScopedCS.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
char *GeneralAlloctor::msStartFree = 0;
char *GeneralAlloctor::msEndFree = 0;
size_t GeneralAlloctor::msHeapSize = 0;
Mutex GeneralAlloctor::msMutex;
GeneralAlloctor::AllocObj* volatile GeneralAlloctor::msFreeList[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//----------------------------------------------------------------------------
void *GeneralAlloctor::Allocate (size_t numBytes)
{
	void *ret = 0;

	if (0 == numBytes)
		return 0;

	if (numBytes > (size_t)MaxAllowAlloctedBytes) 
	{
		// һ������
		ret = malloc(numBytes);
	}
	else 
	{
		AllocObj* volatile * myFreeList = msFreeList + FreeListIndex(numBytes);

		ScopedCS cs(&msMutex);

		AllocObj *retTemp = *myFreeList;

		if (0 == retTemp)
		{
			// �����Ӧ�Ľڵ���û�п��õ��ڴ�, ȥ��������ϵͳ���ڴ�
			ret = ReFill(RoundUp(numBytes));
		}
		else
		{
			// ��Ӧ�ڵ����д����ȡһ�����ظ��ͻ��������޸�ָ��ָ����һ��������
			// ���
			*myFreeList = retTemp->ObjLink;
			ret = retTemp;
		}
	}

	return ret;
}
//----------------------------------------------------------------------------
void GeneralAlloctor::Deallocate (void *buffer, size_t numBytes)
{
	if (0==buffer || 0==numBytes)
		return;

	if (numBytes > (size_t)MaxAllowAlloctedBytes)
	{
		free(buffer);
	}
	else 
	{
		AllocObj* volatile *myFreeList = msFreeList + FreeListIndex(numBytes);

		AllocObj *temp = (AllocObj*)buffer;

		ScopedCS cs(&msMutex);

		temp->ObjLink = *myFreeList;

		*myFreeList = temp;
	}
}
//----------------------------------------------------------------------------
void *GeneralAlloctor::Reallocate (void *buffer, size_t oldSize, size_t newSize)
{
	void* result;
	size_t copySize;

	if (oldSize>(size_t)MaxAllowAlloctedBytes && newSize>(size_t)MaxAllowAlloctedBytes)
	{
		return realloc(buffer, newSize);
	}

	if (RoundUp(oldSize) == RoundUp(newSize))
		return(buffer);

	result = Allocate(newSize);
	copySize = newSize > oldSize? oldSize : newSize;
	memcpy(result, buffer, copySize);
	Deallocate(buffer, oldSize);

	return(result);
}
//----------------------------------------------------------------------------
void* GeneralAlloctor::ReFill (size_t numBytes)
{
	// ���Է���20��
	int numObjs = 20;
	char *chunk = ChunkAlloc(numBytes, numObjs);

	AllocObj* volatile* myFreeList = 0;
	AllocObj* result = 0;
	AllocObj* curObj = 0;
	AllocObj* nextObj = 0;
	int i = 0;

	if (1 == numObjs)
		return chunk; // ֻ�õ�һ����û�ж�����ڴ���������ˣ�ֱ�Ӹ�������

	// �Ѷ������������������

	myFreeList = msFreeList + FreeListIndex(numBytes);

	result = (AllocObj*)chunk;

	*myFreeList = nextObj = (AllocObj*)(chunk + numBytes);
	for (i = 1; ; i++)
	{
		curObj = nextObj;
		nextObj = (AllocObj*)((char*)nextObj + numBytes);
		if (numObjs - 1 == i) // the last one
		{
			curObj->ObjLink = 0;
			break;
		} 
		else
		{
			curObj->ObjLink = nextObj;
		}
	}

	return(result);
}
//----------------------------------------------------------------------------
/*
* ChunkAlloc�Ǳ�ReFill���õģ�Ҳ����˵���ڶ�Ӧ�Ŀ���������û�д��ʱ�����ã�
* SGI������߻����ȿ��ǴӺ����������ڴ档
* static char* msStartFree; static char* msEndFree;��������־�����Ŀ�ʼ��
* �����ġ��ڳ����ѿ�ʼ������ʼ��Ϊ0��Ҳ����һ��ʼ��������ʲôҲû�С��������
* �ѿ�ʼ���û�����һ��32�ֽڵ��ڴ棬�쿴��Ӧ�Ŀ�����������û�д������ͼʹ��
* ReFill����ȡ�ڴ棬���ǵ�����Ը������20*32�ֽڣ���32�ֽڷ��أ�ReFill����
* ChunkAlloc������640�ֽڣ���������������ʲôҲû�����¡�ֻ���Լ�������ϵͳ
* Ҫ�ˣ�����ÿ��Ҫ����Ҫһ���ָ������������ϵͳҪ 2�������󣬾���
* 2* 20 * 32 = 2* 640�ֽڡ�����ܳɣ����ǰ�640�ֽ��е�32�ֽڷ��ظ������ˣ�����
* ��640-32�ֽ������Ӧ�Ŀ����������õ�640�ֽ�������ʹ�á����ˣ������û�����
* ����һ��64�ֽڵ��ڴ棬�쿴��Ӧ������������û�д��������refill������ָ����
* ��20*64=1280�ֽڣ���������ֻ��640�ֽڰ����ܸ����ٸ����ٰɣ�640�ֽ�ȫ������
* �൱ȥ640/64 = 10��Ҫ����ڴ�顣��1�����ظ��ͻ�����ʣ�µ�9��(640-64)�ֽ�����
* ��Ӧ�Ŀ�������
*/
char* GeneralAlloctor::ChunkAlloc (size_t numBytesPerObj, int &numObjs)
{
	char *result = 0;
	size_t totalNumBytes = numBytesPerObj * numObjs;
	size_t bytesLeft = msEndFree - msStartFree;

	if (bytesLeft >= totalNumBytes)
	{
		result = msStartFree;
		msStartFree += totalNumBytes;
		return result;
	} 
	else if (bytesLeft >= numBytesPerObj) 
	{
		numObjs = (int)(bytesLeft/numBytesPerObj);
		totalNumBytes = numBytesPerObj * numObjs;
		result = msStartFree;
		msStartFree += totalNumBytes;
		return(result);
	}
	else 
	{
		size_t bytesToGet = 2*totalNumBytes + RoundUp(msHeapSize>>4);

		if (bytesLeft > 0) 
		{
			// ����ʣ����ڴ�,�ȰѺ�������ʣ�µ��ڴ�,���ӵ���Ӧ�Ŀ�����������ȥ

			AllocObj* volatile* myFreeList = msFreeList + FreeListIndex(bytesLeft);

			((AllocObj*)msStartFree)->ObjLink = *myFreeList;

			*myFreeList = (AllocObj*)msStartFree;
		}

		msStartFree = (char*)malloc(bytesToGet);

		if (0 == msStartFree)
		{   
			// ���������ϵͳû���ڴ��ˡ�
			// ��취�Ӹ�����������������һ���ڴ������һ�����ڵ��㹻���ڴ棬��
			// ��ChunkAlloc����һ�β�����
			size_t i;
			AllocObj* volatile* myFreeList;
			AllocObj *p = 0;

			for (i=numBytesPerObj; i<=(size_t)MaxAllowAlloctedBytes; i+=(size_t)Align) 
			{				
				myFreeList = msFreeList + FreeListIndex(i);
				p = *myFreeList;
				if (0 != p) 
				{
					// �ø���Ŀ��������������п������飬ж��һ�����

					*myFreeList = p->ObjLink;
					msStartFree = (char*)p;
					msEndFree = msStartFree + i;

					 // ��ʱ�ĺ���һ���ܹ���Ӧ����һ������������ڴ�
					return ChunkAlloc(numBytesPerObj, numObjs);
				}
			}

			 // ���������е���������ɽ��ˮ��
			msEndFree = 0;
			msStartFree = (char*)malloc(bytesToGet);
		}

		// �Ѿ��ɹ��Ĵ�ϵͳ��Ҫ������Ҫ���ڴ�,�����뵽���ڴ沦������ʹ��,����
		msHeapSize += bytesToGet;
		msEndFree = msStartFree + bytesToGet;
		return ChunkAlloc(numBytesPerObj, numObjs);
	}
}
//----------------------------------------------------------------------------