//-------------------------------------------------------------------
#ifndef __CMDRSChain_H__
#define __CMDRSChain_H__
//-------------------------------------------------------------------

template<class T> class CMDRSCHAINEDLISTITEM
{
public:
	T *Obj;					// Element carried by the link.
	CMDRSCHAINEDLISTITEM *pNext;

	inline CMDRSCHAINEDLISTITEM<T>() { this->pNext = NULL; }
};

template<class T> class CMDRSCHAINEDLIST
{
public:							
	CMDRSCHAINEDLISTITEM<T> *pFirst;		// Address of next element in chain, or NULL for none.
	CMDRSCHAINEDLISTITEM<T> *pCurrentItem;	// Address of the cutrent item.
	
	inline CMDRSCHAINEDLIST() { this->pFirst = NULL; this->pCurrentItem = NULL; }
	void AddItem(T *t);
	void NextCurrent();
};

#define CHAIN_ENUMERATE(T, list, func) CMDRSCHAINEDLISTITEM<T> *pCurr = list.pFirst; while (pCurr != NULL) { func(pCurr->Obj); pCurr = pCurr->pNext; }

// This function appends element into chain.
template<class T> void CMDRSCHAINEDLIST<T>::AddItem(T *t)
{
	CMDRSCHAINEDLISTITEM<T> *pCurr = this->pFirst;

	if (pCurr == NULL)
	{
		this->pFirst = new CMDRSCHAINEDLISTITEM<T>();
		this->pCurrentItem = this->pFirst;
		pCurr = this->pFirst;
	}
	else
	{
		while (pCurr->pNext != NULL)
			pCurr = pCurr->pNext;

		pCurr->pNext = new CMDRSCHAINEDLISTITEM<T>();
		pCurr = pCurr->pNext;
	}

	pCurr->pNext = NULL;
	pCurr->Obj = t;
}

// This function move the current item to the next in the chain.
template<class T> void CMDRSCHAINEDLIST<T>::NextCurrent()
{
	if (this->pFirst == NULL)
		return;

	this->pCurrentItem = this->pCurrentItem->pNext;

	if (this->pCurrentItem == NULL)
		this->pCurrentItem = this->pFirst;

	return;
}
#endif