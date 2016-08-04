#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"Header.h"


void difference_output(double *S,int N){
	FILE *fp_result;
	errno_t errors;
	char file_name[] = "result.txt";

	if (errors = fopen_s(&fp_result, file_name, "w") != 0){
		printf("\n file open failed \n");
	}

	printf("\nflag\n");

	for (int i = 0; i < N; i++){
		fprintf(fp_result, "%lf,%lf,%lf\n", S[3 * i], S[3 * i + 1], S[3 * i + 2]);
	}

	printf("flag\n");
	fclose(fp_result);
}

void original_output(node *no,int N){

	FILE *fp_original;
	errno_t errors;
	char file_name[] = "original.txt";

	if (errors = fopen_s(&fp_original, file_name, "w") != 0){
		printf("\n file open failed \n");
	}

	for (int i = 0; i < N; i++){
		fprintf(fp_original, "%lf,%lf,%lf\n", no[i].x[0], no[i].x[1], no[i].x[2]);
	}

	fclose(fp_original);
}