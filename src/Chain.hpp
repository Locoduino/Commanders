//-------------------------------------------------------------------
#ifndef __CMDRSChain_H__
#define __CMDRSChain_H__
//-------------------------------------------------------------------

/** this is an item inside a CMDRSCHAINEDLIST.*/
template<class T> class CMDRSCHAINEDLISTITEM
{
public:
	/** Item address.*/ 
	T *Obj;	
	/** Address of the next item.*/
	CMDRSCHAINEDLISTITEM *pNext;

	/** Default constructor.*/
	inline CMDRSCHAINEDLISTITEM<T>() { this->pNext = NULL; }
};

/** This class describes a chained list of items (https://en.wikipedia.org/wiki/Linked_list).
It can be called also a linked list. This is an elegant way to build a list without any big allocation, bypassing
the needs to estimate a minimum size and to increase or decrease this size during execution. The disavantage is a
slower and more complex way to access to an item on the list, and a more complex way to add a new item.

To start, a chained list needs a starting pointer referencing the first item of the list. After that, each item
have a pointer to the next item. If this pointer is NULL, this is the end of the list !

The 'T' type can be of any type, including base types like int or char. But keep in mind that each item must save
a pointer to the next item. If the size of the pointer is greater than the size of the item itself, consider using
a true array...

A notion of 'current item' is used, to let the user of the list to move through the list without doing a loop itself...
*/
template<class T> class CMDRSCHAINEDLIST
{
public:							
	/** Address of next element in chain, or NULL for none. In this case the list is empty.	*/
	CMDRSCHAINEDLISTITEM<T> *pFirst;		
	/** Address of current item in the chain, or NULL for none.	*/
	CMDRSCHAINEDLISTITEM<T> *pCurrentItem;
	
	/** Default constructor.*/
	inline CMDRSCHAINEDLIST() { this->pFirst = NULL; this->pCurrentItem = NULL; }
	/** Add a new item to the list.
	@param inpt Address of the item to add.
	*/
	void AddItem(T *inpt);
	/** Move the current pointer to the next one in the list. If the current was the last one, the current pointer
	will be set to the start of the list, its first item.*/
	void NextCurrent();
};

/** Macro to call a given function on all items in a list.
@param T type of the item
@param list	Chained/linked list to scan.
@param func Function to call. Must be of type
    void function(T *pItem);
*/
#define CMDRSCHAIN_ENUMERATE(T, list, func) CMDRSCHAINEDLISTITEM<T> *pCurr = list.pFirst; while (pCurr != NULL) { func(pCurr->Obj); pCurr = pCurr->pNext; }

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