#include "BigIntIO.h"
#include "../Config.h"
#include <string.h>
#include <malloc.h>

bool runCalCmdWithBinaryMode(CmdParam cmdParam, FILE* outFile) {
	printf("Thuc hien phep toan nek :)\n");
	return true;
}

void registerInputCommnands() {
	registerInputCommnand("2 <> + <>", runCalCmdWithBinaryMode);
	registerInputCommnand("2 <> - <>", runCalCmdWithBinaryMode);
	registerInputCommnand("2 <> * <>", runCalCmdWithBinaryMode);
	registerInputCommnand("2 <> / <>", runCalCmdWithBinaryMode);
}

void registerInputCommnand(const char* cmdStruct, CommandFunction cmdFunc) {
	Command cmd;
	cmd.cmdFunc = cmdFunc;
	cmd.param = convertCmdLineToParam(cmdStruct);

#ifdef DEBUG
	printf("\nNew command with %d param\n", cmd.param.paramCount);
	for (int i = 0; i < cmd.param.paramCount; i++) {
		printf("%s", cmd.param.params[i]);
		printf((i < cmd.param.paramCount - 1) ? ", " : "\n");
	}
#endif

	// add to list
	addCommandToList(cmd);
}

CmdParam convertCmdLineToParam(const char* line) {
	CmdParam param;
	param.params = NULL;
	param.paramCount = 0;

	uint8_t cmdStructBuffLen = (strlen(line) + 1);
	char* cmdStruct_Clone = (char*)malloc(cmdStructBuffLen);
	strcpy_s(cmdStruct_Clone, cmdStructBuffLen, line);

	char* context = NULL;
	char* token = NULL;

	token = strtok_s(cmdStruct_Clone, " ", &context);

	while (token != NULL) {
		param.paramCount++;
		param.params = (char**)realloc(param.params, sizeof(char*) * param.paramCount);
		param.params[param.paramCount - 1] = token;
		token = strtok_s(NULL, " ", &context);
	}

	return param;
}

void addCommandToList(Command cmd) {
	cmdListSize++;
	cmdList = (Command*)realloc(cmdList, sizeof(Command) * cmdListSize);
	cmdList[cmdListSize - 1] = cmd;
}

void freeCommandParam(CmdParam param) {
	free(param.params[0]);
	free(param.params);
}

void freeCommandList() {
	for (int i = 0; i < cmdListSize; i++) {
		freeCommandParam(cmdList[i].param);
	}
	free(cmdList);
}

bool runInputCommand(FILE* outFile, const char* cmdLine) {
	bool runningState = false;
	CmdParam cmdParam = convertCmdLineToParam(cmdLine);
	for (int i = 0; i < cmdListSize; i++) {
		if (isParamMatchParam(cmdParam, cmdList[i].param)) {
			runningState = (cmdList[i].cmdFunc == NULL) ? true : cmdList[i].cmdFunc(cmdParam, outFile);
		}
	}
	freeCommandParam(cmdParam);
	return runningState;
}

bool isParamMatchParam(CmdParam runningCmdParam, CmdParam templateCmdParam) {
	if (runningCmdParam.paramCount != templateCmdParam.paramCount) return false;

	for (int i = 0; i < runningCmdParam.paramCount; i++) {
		if ((templateCmdParam.params[i][0] == '<') && (templateCmdParam.params[i][1] == '>')) continue;
		if (strcmp(runningCmdParam.params[i], templateCmdParam.params[i]) != 0) return false;
	}

	return true;
}