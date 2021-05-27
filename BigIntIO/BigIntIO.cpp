#include "BigIntIO.h"
#include "../BigInt/BigInt.h"
#include <string.h>
#include <malloc.h>

bool runCalCmd(CmdParam cmdParam, FILE* file) {
	char *resStr;
	BigInt res;
	BigInt n1, n2;
	if (cmdParam.params[0][0] == '2') {
		n1 = binStrToBigInt(cmdParam.params[1]);
		n2 = binStrToBigInt(cmdParam.params[3]);
	}
	else {
		n1 = decStrToBigInt(cmdParam.params[1]);
		n2 = decStrToBigInt(cmdParam.params[3]);
	}

	switch (cmdParam.params[2][0]) {
	case '+':
		res = n1 + n2;
		break;
	case '-':
		res = n1 - n2;
		break;
	case '*':
		res = n1 * n2;
		break;
	case '/':
		res = n1 / n2;
		break;
	case '%':
		res = n1 % n2;
		break;
	}

	resStr = (cmdParam.params[0][0] == '2') ? bigIntToBinStr(&res) : bigIntToDecStr(&res);
	fprintf(file, "%s\n", resStr);
	free(resStr);
	return true;
}

bool runConvertBigIntBaseCmd(CmdParam cmdParam, FILE* file) {
	BigInt n;
	char* resStr;
	if (cmdParam.params[0][0] == '2') {
		n = binStrToBigInt(cmdParam.params[2]);
		resStr = bigIntToDecStr(&n);
	}
	else {
		n = decStrToBigInt(cmdParam.params[2]);
		resStr = bigIntToBinStr(&n);
	}
	fprintf(file, "%s\n", resStr);
	free(resStr);
	return true;
}

bool runBitmathCmd(CmdParam cmdParam, FILE* file) {
	char* resStr;
	BigInt res;
	BigInt n1, n2;
	if (cmdParam.params[0][0] == '2') {
		n1 = binStrToBigInt(cmdParam.params[1]);
		n2 = binStrToBigInt(cmdParam.params[3]);
	}
	else {
		n1 = decStrToBigInt(cmdParam.params[1]);
		n2 = decStrToBigInt(cmdParam.params[3]);
	}

	switch (cmdParam.params[2][0]) {
	case '&':
		res = n1 & n2;
		break;
	case '|':
		res = n1 | n2;
		break;
	case '^':
		res = n1 ^ n2;
		break;
	}

	resStr = (cmdParam.params[0][0] == '2') ? bigIntToBinStr(&res) : bigIntToDecStr(&res);
	fprintf(file, "%s\n", resStr);
	free(resStr);
	return true;
}

bool runNotCmd(CmdParam cmdParam, FILE* file) {
	BigInt n;
	char* resStr;
	if (cmdParam.params[0][0] == '2') {
		n = ~binStrToBigInt(cmdParam.params[2]);
		resStr = bigIntToBinStr(&n);
	}
	else {
		n = ~decStrToBigInt(cmdParam.params[2]);
		resStr = bigIntToDecStr(&n);
	}
	fprintf(file, "%s\n", resStr);
	free(resStr);
	return true;
}

bool runBitMoveCmd(CmdParam cmdParam, FILE* file) {
	char* resStr;
	BigInt res;
	BigInt n1 = (cmdParam.params[0][0] == '2') ? binStrToBigInt(cmdParam.params[1]) : decStrToBigInt(cmdParam.params[1]);
	int32_t movingSteps = getValue(decStrToBigInt(cmdParam.params[3]));

	switch (cmdParam.params[2][0]) {
	case '>':
		res = n1 >> movingSteps;
		break;
	case '<':
		res = n1 << movingSteps;
		break;
	}

	resStr = (cmdParam.params[0][0] == '2') ? bigIntToBinStr(&res) : bigIntToDecStr(&res);
	fprintf(file, "%s\n", resStr);
	free(resStr);
	return true;
}

bool runMinMaxPowCmd(CmdParam cmdParam, FILE* file) {
	char* resStr;
	BigInt res;
	BigInt n1, n2;
	if (cmdParam.params[0][0] == '2') {
		n1 = binStrToBigInt(cmdParam.params[2]);
		n2 = binStrToBigInt(cmdParam.params[3]);
	}
	else {
		n1 = decStrToBigInt(cmdParam.params[2]);
		n2 = decStrToBigInt(cmdParam.params[3]);
	}

	if (strcmp(cmdParam.params[1], "min") == 0) {
		res = min(n1, n2);
	}
	else if (strcmp(cmdParam.params[1], "max") == 0) {
		res = max(n1, n2);
	}
	else {
		res = pow(n1, n2);
	}

	resStr = (cmdParam.params[0][0] == '2') ? bigIntToBinStr(&res) : bigIntToDecStr(&res);
	fprintf(file, "%s\n", resStr);
	free(resStr);
	return true;
}

bool runAbsDigitPrimeToBaseCmd(CmdParam cmdParam, FILE* file) {
	char* resStr = NULL;
	BigInt res;
	BigInt n = (cmdParam.params[0][0] == '2') ? binStrToBigInt(cmdParam.params[2]) : decStrToBigInt(cmdParam.params[2]);
	
	if (strcmp(cmdParam.params[1], "digits") == 0) {
		res = assignValue(digits(&n));
		resStr = bigIntToDecStr(&res);
	}
	else if (strcmp(cmdParam.params[1], "abs") == 0) {
		res = abs(n);
		resStr = (cmdParam.params[0][0] == '2') ? bigIntToBinStr(&res) : bigIntToDecStr(&res);
	}
	else if (strcmp(cmdParam.params[1], "is_prime") == 0) {
		resStr = boolToString(is_prime(n));
	}
	else if (strcmp(cmdParam.params[1], "to_base32") == 0) {
		resStr = to_base32(n);
	}
	else if (strcmp(cmdParam.params[1], "to_base58")==0) {
		resStr = to_base58(n);
	}
	else {
		resStr = to_base64(n);
	}

	if (resStr != NULL) {
		fprintf(file, "%s\n", resStr);
		free(resStr);
		return true;
	}
	return false;
}

bool runCompare1CharCmd(CmdParam cmdParam, FILE* file) {
	char* resStr;
	bool res = false;
	BigInt n1, n2;
	if (cmdParam.params[0][0] == '2') {
		n1 = binStrToBigInt(cmdParam.params[1]);
		n2 = binStrToBigInt(cmdParam.params[3]);
	}
	else {
		n1 = decStrToBigInt(cmdParam.params[1]);
		n2 = decStrToBigInt(cmdParam.params[3]);
	}

	switch (cmdParam.params[2][0]) {
	case '>':
		res = (n1 > n2);
		break;
	case '<':
		res = (n1 < n2);
		break;
	}

	resStr = boolToString(res);
	fprintf(file, "%s\n", resStr);
	free(resStr);
	return true;
}

bool runCompare2CharCmd(CmdParam cmdParam, FILE* file) {
	char* resStr;
	bool res = false;
	BigInt n1, n2;
	if (cmdParam.params[0][0] == '2') {
		n1 = binStrToBigInt(cmdParam.params[1]);
		n2 = binStrToBigInt(cmdParam.params[3]);
	}
	else {
		n1 = decStrToBigInt(cmdParam.params[1]);
		n2 = decStrToBigInt(cmdParam.params[3]);
	}

	switch (cmdParam.params[2][0]) {
	case '>':
		res = (n1 >= n2);
		break;
	case '<':
		res = (n1 <= n2);
		break;
	case '=':
		res = (n1 == n2);
		break;
	case '!':
		res = (n1 != n2);
	}

	resStr = boolToString(res);
	fprintf(file, "%s\n", resStr);
	free(resStr);
	return true;
}

void registerInputCommnands() {
	registerInputCommnand("2 <> + <>", runCalCmd);
	registerInputCommnand("2 <> - <>", runCalCmd);
	registerInputCommnand("2 <> * <>", runCalCmd);
	registerInputCommnand("2 <> / <>", runCalCmd);
	registerInputCommnand("2 <> % <>", runCalCmd);
	registerInputCommnand("10 <> + <>", runCalCmd);
	registerInputCommnand("10 <> - <>", runCalCmd);
	registerInputCommnand("10 <> * <>", runCalCmd);
	registerInputCommnand("10 <> / <>", runCalCmd);
	registerInputCommnand("10 <> % <>", runCalCmd);

	registerInputCommnand("10 2 <>", runConvertBigIntBaseCmd);
	registerInputCommnand("2 10 <>", runConvertBigIntBaseCmd);

	// & | ^ ~
	registerInputCommnand("2 <> & <>", runBitmathCmd);
	registerInputCommnand("2 <> | <>", runBitmathCmd);
	registerInputCommnand("2 <> ^ <>", runBitmathCmd);
	registerInputCommnand("2 ~ <>", runNotCmd);
	registerInputCommnand("10 <> & <>", runBitmathCmd);
	registerInputCommnand("10 <> | <>", runBitmathCmd);
	registerInputCommnand("10 <> ^ <>", runBitmathCmd);
	registerInputCommnand("10 ~ <>", runNotCmd);

	// >> <<
	registerInputCommnand("2 <> >> <>", runBitMoveCmd);
	registerInputCommnand("2 <> << <>", runBitMoveCmd);
	registerInputCommnand("10 <> >> <>", runBitMoveCmd);
	registerInputCommnand("10 <> << <>", runBitMoveCmd);

	// min max pow
	registerInputCommnand("2 min <> <>", runMinMaxPowCmd);
	registerInputCommnand("2 max <> <>", runMinMaxPowCmd);
	registerInputCommnand("2 pow <> <>", runMinMaxPowCmd);
	registerInputCommnand("10 min <> <>", runMinMaxPowCmd);
	registerInputCommnand("10 max <> <>", runMinMaxPowCmd);
	registerInputCommnand("10 pow <> <>", runMinMaxPowCmd);

	// abs digits to_base32 to_base58 to_base64 is_prime
	registerInputCommnand("2 digits <>", runAbsDigitPrimeToBaseCmd);
	registerInputCommnand("2 to_base32 <>", runAbsDigitPrimeToBaseCmd);
	registerInputCommnand("2 to_base58 <>", runAbsDigitPrimeToBaseCmd);
	registerInputCommnand("2 to_base64 <>", runAbsDigitPrimeToBaseCmd);
	registerInputCommnand("2 is_prime <>", runAbsDigitPrimeToBaseCmd);
	registerInputCommnand("10 abs <>", runAbsDigitPrimeToBaseCmd);
	registerInputCommnand("10 digits <>", runAbsDigitPrimeToBaseCmd);
	registerInputCommnand("10 to_base32 <>", runAbsDigitPrimeToBaseCmd);
	registerInputCommnand("10 to_base58 <>", runAbsDigitPrimeToBaseCmd);
	registerInputCommnand("10 to_base64 <>", runAbsDigitPrimeToBaseCmd);
	registerInputCommnand("10 is_prime <>", runAbsDigitPrimeToBaseCmd);

	// > <    >= <= == !=
	registerInputCommnand("2 <> > <>", runCompare1CharCmd);
	registerInputCommnand("2 <> < <>", runCompare1CharCmd);
	registerInputCommnand("10 <> > <>", runCompare1CharCmd);
	registerInputCommnand("10 <> < <>", runCompare1CharCmd);
	registerInputCommnand("2 <> >= <>", runCompare2CharCmd);
	registerInputCommnand("2 <> <= <>", runCompare2CharCmd);
	registerInputCommnand("2 <> == <>", runCompare2CharCmd);
	registerInputCommnand("2 <> != <>", runCompare2CharCmd);
	registerInputCommnand("10 <> >= <>", runCompare2CharCmd);
	registerInputCommnand("10 <> <= <>", runCompare2CharCmd);
	registerInputCommnand("10 <> == <>", runCompare2CharCmd);
	registerInputCommnand("10 <> != <>", runCompare2CharCmd);
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

	uint16_t cmdStructBuffLen = (strlen(line) + 1);
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
			break;
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