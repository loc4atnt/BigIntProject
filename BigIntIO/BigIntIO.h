#ifndef _BigIntIO_H_
#define _BigIntIO_H_

#include <stdio.h>
#include <stdint.h>

struct CmdParam {
	char** params;
	uint8_t paramCount;
};

typedef bool (*CommandFunction)(CmdParam cmdParam, FILE* outFile);

struct Command {
	CommandFunction cmdFunc;
	CmdParam param;
};

static Command* cmdList = NULL;
static uint8_t cmdListSize = 0;

void registerInputCommnands();
void registerInputCommnand(const char *cmdStruct, CommandFunction cmdFunc);
void freeCommandParam(CmdParam param);
void freeCommandList();
void addCommandToList(Command cmd);
bool runInputCommand(FILE *outFile, const char *cmdLine);
CmdParam convertCmdLineToParam(const char* line);
bool isParamMatchParam(CmdParam p1, CmdParam p2);

#endif