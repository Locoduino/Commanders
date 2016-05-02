//-------------------------------------------------------------------
#ifndef __Chain_H__
#define __Chain_H__
//-------------------------------------------------------------------

template<class T> class CHAINLISTITEM
{
public:
	T Obj;					// Element carried by the link.
	CHAINLISTITEM *pNext;

	inline CHAINLISTITEM<T>() { this->pNext = NULL; }
};

template<class T> class CHAINLIST
{
public:							
	CHAINLISTITEM<T> *pFirst;		// Address of next element in chain, or NULL for none.
	CHAINLISTITEM<T> *pCurrentItem;	// Address of the cutrent item.
	
	inline CHAINLIST() { this->pFirst = NULL; this->pCurrentItem = NULL; }
	void AddItem(const T &t);
	void NextCurrent();
};

#define CHAIN_ENUMERATE(T, list, func) CHAINLISTITEM<T> *pCurr = list.pFirst; while (pCurr != NULL) { func(pCurr->Obj); pCurr = pCurr->pNext; }

// This function appends element into chain.
template<class T> void CHAINLIST<T>::AddItem(const T &t)
{
	CHAINLISTITEM<T> *pCurr = this->pFirst;

	if (pCurr == NULL)
	{
		this->pFirst = new CHAINLISTITEM<T>();
		this->pCurrentItem = this->pFirst;
		pCurr = this->pFirst;
	}
	else
		while (pCurr != NULL)
		{
			if (pCurr->pNext == NULL)
			{
				pCurr->pNext = new CHAINLISTITEM<T>();
				pCurr = pCurr->pNext;
				break;
			}
		}

	pCurr->pNext = NULL;
	pCurr->Obj = t;
}

// This function move the current item to the next in the chain.
template<class T> void CHAINLIST<T>::NextCurrent()
{
	if (this->pFirst == NULL)
		return;

	this->pCurrentItem = this->pCurrentItem->pNext;

	if (this->pCurrentItem == NULL)
		this->pCurrentItem = this->pFirst;

	return;
}
#endif