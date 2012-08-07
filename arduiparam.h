/*
 * arduiparam.h
 *
 *  Created on: 22 mai 2012
 *      Author: jmax
 */

#ifndef ARDUIPARAM_H_
#define ARDUIPARAM_H_

//#define ARDUI_DEBUG_X86

#ifdef ARDUI_DEBUG_X86
#include <string>
#endif

#include <string.h>

#define MAX_ARDUI_CMD 10
#define MAX_SIZE_CMD 5
#define MAX_SIZE_VALUE 10

#define ARDUI_PARAM_READ_CARAC '?'
#define ARDUI_PARAM_WRITE_CARAC '='
#define ARDUI_PARAM_TERMINATE_CARAC '.'



#ifdef ARDUI_DEBUG_X86
using namespace std;
#endif

typedef enum
{
	ARDUI_PARAM_STR_PUT_CMD=0,
	ARDUI_PARAM_STR_PUT_VALUE,
	ARDUI_PARAM_STR_SET_VALUE,
	NB_ARDUI_PARAM_STR_TYPE
}ARDUI_PARAM_ANALYS_SYNTAX;

typedef enum
{
	ARDUI_PARAM_NO_ERROR =0,
	ARDUI_PARAM_ERROR_SYNTAX,
	ARDUI_PARAM_ERROR_CMD_NO_FOURND,
	NB_ARDUI_PARAM_ERROR
}ARDUI_PARAM_ERROR_e;

typedef enum
{
	ARDUI_PARAM_WRITE=0,
	ARDUI_PARAM_READ,
	ARDUI_PARAM_ACT,
	NB_ARDUI_PARAM
}ARDUI_COMMAND_TYPE_e;

#ifdef ARDUI_DEBUG_X86
static string strArduiParamError [NB_ARDUI_PARAM_ERROR] =
{
	"",
	"Syntax error",
	"command no found"
};

static string ArduiParamStrSyntax[NB_ARDUI_PARAM_STR_TYPE] =
{
	"ARDUI_PARAM_STR_PUT_CMD",
	"ARDUI_PARAM_STR_PUT_VALUE",
	"ARDUI_PARAM_STR_SET_VALUE",
};
#endif

typedef bool (*CB_ARDUI_COMMAND) (ARDUI_COMMAND_TYPE_e type , int param);

class ArduiParamItem
{
public :
	ArduiParamItem(char *str , CB_ARDUI_COMMAND cb);
	void Add(ArduiParamItem *pAdd);
	void Insert(ArduiParamItem *pAdd);
	char strCmd[MAX_SIZE_CMD +1 ];
	CB_ARDUI_COMMAND cmd;
	ArduiParamItem *pNext;
	ArduiParamItem *pPrev;
};


class ArduiParam
{
public:
	ArduiParam(void);
	int AddCommand (ArduiParamItem *pItem);
	int AddAndShortCommand (ArduiParamItem *pItem);
#ifdef	ARDUI_DEBUG_X86
	void debug(void);
#endif
	ARDUI_PARAM_ERROR_e PutChar(char c);
	char *GetFirstItemCmd(void);
	char *GetNextItemCmd(void);

private:
	ARDUI_PARAM_ERROR_e Error(ARDUI_PARAM_ERROR_e err);
	void setHead(ArduiParamItem *pItem);

	ArduiParamItem *pHead;
	ArduiParamItem *pItem;

	ArduiParamItem *pItemForPrint;
	int nbItem;


	char CurrentStr[MAX_SIZE_CMD+1];
	int iCurrentStr;
	char StrValue[MAX_SIZE_VALUE+1];
	bool bCommandFound;

	void ClearParam(void);
	ARDUI_PARAM_ANALYS_SYNTAX ArduiParamState;
};

#endif /* ARDUIPARAM_H_ */
