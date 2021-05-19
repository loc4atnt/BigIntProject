#include <stdio.h>
#include <string.h>
#include <time.h>
#include "BigIntIO/BigIntIO.h"
#include "BigInt/BigInt.h"

#include <time.h>

#define RUNNING_CMD_BUFFER_LEN 500

void runInputCommands(FILE *inF, const char *outPath) {
	remove(outPath);

	FILE* ouF;
	char buff[RUNNING_CMD_BUFFER_LEN];
	while (fgets(buff, RUNNING_CMD_BUFFER_LEN, inF)!=NULL) {
		buff[strcspn(buff, "\n")] = '\0';

		fopen_s(&ouF, outPath, "a");
		// printf("=> %s\n", buff);

		if (!runInputCommand(ouF, buff)) {
			printf("x | Lenh \"%s\" khong hop le!\n", buff);
			fprintf(ouF, "x | Lenh khong hop le!\n");
		}
		else {
			printf("+ | Lenh \"%s\" thuc hien xong!\n", buff);
		}

		fclose(ouF);
	}
}

void main(int argCount, char **args) {

	// -9485571363021587 * 24689531702694
	BigInt a = decStrToBigInt("-9485571363021587");
	BigInt b = decStrToBigInt("24689531702694");
	BigInt c = a * b;
	char* str = to_base10(&a);
	printf("%s\n", str);
	free(str);
	return;

	//resetDebugTime();
	//BigInt a = decStrToBigInt("235463381314039523573896854894651656220931");
	//debugTime("a");
	//resetDebugTime();
	//char* str = to_base10(&a);
	//debugTime("str");
	//printf("%s", str);
	//free(str);
	//return;

	if (argCount < 3) {
		printf("Sai cu phap!\n");
		printf("Cu phap lenh: 20120020_20120561.exe <input_path> <output_path>\n");
		return;
	}

	FILE* inpFile = NULL;

	fopen_s(&inpFile, args[1], "r");

	if ((!inpFile)) {
		printf("Duong dan File khong ton tai!\n");
		return;
	}

	printf("----- Mo file thanh cong -----\n");

	printf("\nDang ky lenh input.....\n");
	registerInputCommnands();
	printf("----- Dang ky lenh input thanh cong -----\n");

	printf("\nChay lenh tu file %s.....\n", args[1]);
	runInputCommands(inpFile, args[2]);
	printf("----- Chay lenh thanh cong -----\n");

	printf("\nDong file, giai phong bo nho.....\n");
	fclose(inpFile);
	freeCommandList();
	printf("----- Chuong trinh da ket thuc, xem ket qua tai %s -----\n", args[2]);
}