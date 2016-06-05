//-------------------------------------------------------------------
#ifndef __Chain_H__
#define __Chain_H__
//-------------------------------------------------------------------

template<class T> class CHAINEDLISTITEM
{
public:
	T Obj;					// Element carried by the link.
	CHAINEDLISTITEM *pNext;

	inline CHAINEDLISTITEM<T>() { this->pNext = NULL; }
};

template<class T> class CHAINEDLIST
{
public:							
	CHAINEDLISTITEM<T> *pFirst;		// Address of next element in chain, or NULL for none.
	CHAINEDLISTITEM<T> *pCurrentItem;	// Address of the cutrent item.
	
	inline CHAINEDLIST() { this->pFirst = NULL; this->pCurrentItem = NULL; }
	void AddItem(const T &t);
	void NextCurrent();
};

#define CHAIN_ENUMERATE(T, list, func) CHAINEDLISTITEM<T> *pCurr = list.pFirst; while (pCurr != NULL) { func(pCurr->Obj); pCurr = pCurr->pNext; }

// This function appends element into chain.
template<class T> void CHAINEDLIST<T>::AddItem(const T &t)
{
	CHAINEDLISTITEM<T> *pCurr = this->pFirst;

	if (pCurr == NULL)
	{
		this->pFirst = new CHAINEDLISTITEM<T>();
		this->pCurrentItem = this->pFirst;
		pCurr = this->pFirst;
	}
	else
	{
		while (pCurr->pNext != NULL)
			pCurr = pCurr->pNext;

		pCurr->pNext = new CHAINEDLISTITEM<T>();
		pCurr = pCurr->pNext;
	}

	pCurr->pNext = NULL;
	pCurr->Obj = t;
}

// This function move the current item to the next in the chain.
template<class T> void CHAINEDLIST<T>::NextCurrent()
{
	if (this->pFirst == NULL)
		return;

	this->pCurrentItem = this->pCurrentItem->pNext;

	if (this->pCurrentItem == NULL)
		this->pCurrentItem = this->pFirst;

	return;
}
#endif