#include "Config.h"
#include <stdio.h>
#include <string.h>
#include "BigIntIO/BigIntIO.h"
#include "BigInt/BigInt.h"

#define RUNNING_CMD_BUFFER_LEN 200

void runInputCommands(FILE *inF, FILE *ouF) {
	char buff[RUNNING_CMD_BUFFER_LEN];
	while (fgets(buff, RUNNING_CMD_BUFFER_LEN, inF)!=NULL) {
		buff[strcspn(buff, "\n")] = '\0';

#ifdef DEBUG
		printf("=> %s\n", buff);
#endif

		if (!runInputCommand(ouF, buff)) {
			printf("x | Lenh \"%s\" khong hop le!\n", buff);
		}
		else {
			printf("+ | Lenh \"%s\" thuc hien xong!\n", buff);
		}
	}
}

void main(int argCount, char **args) {
	BigInt a = assignValue(-3);
	BigInt b = assignValue(-2);
	BigInt c = a + b;
	//printf("%d\n", a == b);//decStrToBigInt//bigIntToDecStr
	printf("A: %s\n", bigIntToBinStr(&a));
	printf("B: %s\n", bigIntToBinStr(&b));
	printf("C: %s\n", bigIntToBinStr(&c));
	printf("C: %d - %d - %d\n\n", c.isHasSign, c.byteCount, c.bytes[0]);
	printf("A: %d - %d - %d\n", a.isHasSign, a.byteCount, a.bytes[0]);
	printf("B: %d - %d - %d\n", b.isHasSign, b.byteCount, b.bytes[0]);
	return;
	if (argCount < 3) {
		printf("Sai cu phap!\n");
		printf("Cu phap lenh: 20120020_20120561.exe <input_path> <output_path>\n");
		return;
	}

	FILE* inpFile = NULL;
	FILE* outFile = NULL;

	fopen_s(&inpFile, args[1], "r");
	fopen_s(&outFile, args[2], "w");

	if ((!inpFile) || (!outFile)) {
		printf("Duong dan File khong ton tai!\n");
		return;
	}

	printf("----- Mo file thanh cong -----\n");

	printf("\nDang ky lenh input.....\n");
	registerInputCommnands();
	printf("----- Dang ky lenh input thanh cong -----\n");

	printf("\nChay lenh tu file %s.....\n", args[1]);
	runInputCommands(inpFile, outFile);
	printf("----- Chay lenh thanh cong -----\n");

	printf("\nDong file, giai phong bo nho.....\n");
	fclose(inpFile);
	fclose(outFile);
	freeCommandList();
	printf("----- Chuong trinh da ket thuc, xem ket qua tai %s -----\n", args[2]);
}