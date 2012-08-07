#include "arduiparam.hpp"
#include <stdlib.h>

#ifdef ARDUI_DEBUG_X86
#include <iostream>
#endif

ArduiParam::ArduiParam(void)
{
	pHead = NULL;
	nbItem = 0;
	ClearParam();
}

void ArduiParam::ClearParam(void)
{
	iCurrentStr = 0;
	*CurrentStr = '\0';
	ArduiParamState = ARDUI_PARAM_STR_PUT_CMD;
	*StrValue = '\0';
	setHead(pHead);
	pItemForPrint = NULL;
}

char *ArduiParam::GetFirstItemCmd(void)
{
	pItemForPrint = pHead;
	return pItemForPrint->strCmd;
}

char *ArduiParam::GetNextItemCmd(void)
{
	if (pItemForPrint && pItemForPrint->pNext)
	{
		ArduiParamItem *ptemp = pItemForPrint;
		pItemForPrint = ptemp->pNext;
		return ptemp->strCmd;
	}
	pItemForPrint = NULL;
	return NULL;
}


int ArduiParam::AddAndShortCommand (ArduiParamItem *pAddItem)
{
	if (!pHead)
	{
		pHead = pAddItem;
		pItem = pAddItem;
	}
	else
	{
		ArduiParamItem *pTemp;
		ArduiParamItem *pLast;
		int cmp;

		for (pTemp = pHead ; pTemp ; pTemp = pTemp->pNext)
		{
			cmp = strcmp(pTemp->strCmd,pAddItem->strCmd);
#ifdef ARDUI_DEBUG_X86
			cout << "cmp :"<< cmp <<
					"(" << pTemp->strCmd << ")" <<
					"(" << pAddItem->strCmd << ")" << endl;
#endif
			if ( cmp == 0)
			{
#ifdef ARDUI_DEBUG_X86
				cout << "same command :"<< pAddItem->strCmd << endl;
#endif
				return nbItem;
			}

			if (cmp > 0 )
			{
#ifdef ARDUI_DEBUG_X86
				cout << "insert command :"<< pAddItem->strCmd <<
						"before : "<< pTemp->strCmd <<endl;
#endif
				pTemp->Insert(pAddItem);
				if (pHead == pTemp)
					setHead(pAddItem);

				break;
			}
#ifdef ARDUI_DEBUG_X86
			else
				cout << "next command :"<< pAddItem->strCmd << endl;
#endif

			pLast = pTemp;
		}

		if (!pTemp)
		{
#ifdef ARDUI_DEBUG_X86
			cout << "add command :"<< pAddItem->strCmd << endl;
#endif
			pLast->Add(pAddItem);
		}
	}


	nbItem++;
	return nbItem;
}

void ArduiParam::setHead (ArduiParamItem *pNewHead)
{
	pHead = pNewHead;
	pItem = pHead;
}

int ArduiParam::AddCommand (ArduiParamItem *pAddItem)
{
	static ArduiParamItem *pNextItem = NULL;
	if (!pHead)
		setHead( pAddItem);
	else
		pNextItem->Add(pAddItem);

	pNextItem = pAddItem;
	nbItem++;
	return nbItem;
}

#ifdef ARDUI_DEBUG_X86
void ArduiParam::debug (void)
{
	int i;
	ArduiParamItem *pCmd = pHead;
	for (i=0 ; i<nbItem && pCmd ; i++)
	{
		cout<<"cmd:"<<i<<" str:"<<pCmd->strCmd<<endl;
		pCmd = pCmd->pNext;
	}
}
#endif

ARDUI_PARAM_ERROR_e ArduiParam::PutChar(char c)
{
	ArduiParamItem *pTempItem;
	bool bStoreCarac = false;
	CB_ARDUI_COMMAND cb = NULL;
	ARDUI_COMMAND_TYPE_e cmd_cb;
	int Var = 0;
	ARDUI_PARAM_ERROR_e err = ARDUI_PARAM_NO_ERROR;

#ifdef ARDUI_DEBUG_X86
	cout << "putchar debug c:" << c <<
			" state :" << ArduiParamStrSyntax[ArduiParamState]<<
			" icurrent : "<<iCurrentStr <<endl;
#endif

	if (c == ARDUI_PARAM_READ_CARAC)
	{
		if (ArduiParamState == ARDUI_PARAM_STR_PUT_CMD && bCommandFound)
		{
			cb = pItem->cmd;
			cmd_cb = ARDUI_PARAM_READ;
		}
		else
			err= Error(bCommandFound? ARDUI_PARAM_ERROR_CMD_NO_FOURND:ARDUI_PARAM_ERROR_SYNTAX);
	}
	else if (c == ARDUI_PARAM_TERMINATE_CARAC)
	{

#ifdef ARDUI_DEBUG_X86
		cout << "putchar debug ARDUI_PARAM_TERMINATE_CARAC" << bCommandFound <<endl;
#endif

		if (!bCommandFound)
		{
			err= Error(ARDUI_PARAM_ERROR_CMD_NO_FOURND);
		}
		else
		{
			if (ArduiParamState == ARDUI_PARAM_STR_PUT_CMD )
			{
				cb = pItem->cmd;
				cmd_cb = ARDUI_PARAM_ACT ;
			}
			else if (ArduiParamState == ARDUI_PARAM_STR_SET_VALUE)
			{
				Var = atoi(StrValue);
				cb = pItem->cmd;
				cmd_cb = ARDUI_PARAM_WRITE;
			}
			else
				err= Error(ARDUI_PARAM_ERROR_SYNTAX);
		}
	}
	else if (c == ARDUI_PARAM_WRITE_CARAC)
	{
		if (bCommandFound && ArduiParamState == ARDUI_PARAM_STR_PUT_CMD )
		{
			ArduiParamState = ARDUI_PARAM_STR_SET_VALUE;
			iCurrentStr = 0;
		}
	}
	else
	{
		cb = NULL;
		bStoreCarac = true;
	}


	if (bStoreCarac)
	{
		if (ArduiParamState == ARDUI_PARAM_STR_SET_VALUE)
		{
			if (iCurrentStr < MAX_SIZE_VALUE)
			{
				if(c >= '0' && c <= '9')
				{
					StrValue[iCurrentStr] = c;
					iCurrentStr++;
					StrValue[iCurrentStr] = 0;
				}
				else
					err= Error(ARDUI_PARAM_ERROR_SYNTAX);
			}
			else
				err= Error(ARDUI_PARAM_ERROR_SYNTAX);
		}

		else if (ArduiParamState == ARDUI_PARAM_STR_PUT_CMD)
		{
			bCommandFound = false;

			CurrentStr[iCurrentStr] = c;
			CurrentStr[iCurrentStr+1] = 0;

			for ( pTempItem=pItem ; pTempItem ; pTempItem = pTempItem->pNext )
			{
#ifdef ARDUI_DEBUG_X86
				cout << "1er StrCmd : "<< *pTempItem->strCmd << endl;
#endif
				if (pTempItem->strCmd[iCurrentStr] == c)
				{
#ifdef ARDUI_DEBUG_X86
					cout << " look for cmd : " << pTempItem->strCmd
						 << " carac c:"<<pTempItem->strCmd[iCurrentStr]
						 << " c+1 "<< pTempItem->strCmd[iCurrentStr+1]   <<endl;
#endif
					pItem = pTempItem; //select last good
					if (!pItem->strCmd[iCurrentStr+1])
						bCommandFound = true;
					break;
				}
			}
			iCurrentStr++;
			if (!pTempItem)
				err= Error(ARDUI_PARAM_ERROR_CMD_NO_FOURND);
		}
		else
			err= Error(ARDUI_PARAM_ERROR_SYNTAX);
	}

	if (cb)
	{
		if (!cb(cmd_cb,Var))
			err= Error(ARDUI_PARAM_ERROR_SYNTAX);
		else
			ClearParam();
	}

	return err;
}

ARDUI_PARAM_ERROR_e ArduiParam::Error(ARDUI_PARAM_ERROR_e err)
{
	CurrentStr[iCurrentStr+1] = 0;
#ifdef ARDUI_DEBUG_X86
	cout<<strArduiParamError[err]<<" ( " << CurrentStr << " ) "<< endl;
#endif
	ClearParam();

	return err;
}

ArduiParamItem::ArduiParamItem(char *str , CB_ARDUI_COMMAND cb)
{
	strcpy(strCmd,str);
	cmd = cb;
	pNext = NULL;
	pPrev = NULL;
}

void ArduiParamItem::Add(ArduiParamItem *pAdd)
{
	if(pNext)
		pNext->pPrev = pAdd;

	pAdd->pNext = pNext;
	pNext = pAdd;
	pAdd->pPrev = this;
}

void ArduiParamItem::Insert(ArduiParamItem *pAdd)
{
	if (pPrev)
		pPrev->pNext = pAdd;

	pAdd->pPrev = pPrev;
	pAdd->pNext = this;
	pPrev = pAdd;
}

