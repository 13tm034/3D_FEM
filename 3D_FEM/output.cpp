#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"Header.h"


void difference_output(double *S,int N){
	FILE *fp_result;
	errno_t errors;
	char file_name[] = "displacement.txt";

	if (errors = fopen_s(&fp_result, file_name, "w") != 0){
		printf("\n file open failed \n");
	}

	for (int i = 0; i < N; i++){
		fprintf(fp_result, "%19.5e %19.5e %19.5e\n", S[3 * i], S[3 * i + 1], S[3 * i + 2]);
		printf("differ=%lf\n", sqrt(S[3 * i] * S[3 * i] + S[3 * i + 1] * S[3 * i + 1] + S[3 * i + 2] * S[3 * i + 2]));
	}


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


void coordinate_output(node *no, int N){
	FILE *fp_result;
	errno_t errors;
	char file_name[] = "original_coordinate_value.txt";

	if (errors = fopen_s(&fp_result, file_name, "w") != 0){
		printf("\n file open failed \n");
	}



	for (int i = 0; i < N; i++){
		fprintf(fp_result, "%19.5e %19.5e %19.5e\n", no[i].x[0], no[i].x[1], no[i].x[2]);
	}

	fclose(fp_result);
}


void element_node_output(element *el, int E){
	FILE *fp_result;
	errno_t errors;
	char file_name[] = "node_number.txt";

	if (errors = fopen_s(&fp_result, file_name, "w") != 0){
		printf("\n file open failed \n");
	}


	for (int i = 0; i < E; i++){
		for (int j = 0; j < 8; j++){
			fprintf(fp_result, "%5d ", el[i].node[j]);
		}
		fprintf(fp_result, "\n");
	}

	fclose(fp_result);
}


void result_output(element *el, node *no,  double *S,int E, int N){
	FILE *fp_result;
	errno_t errors;
	char file_name[] = "result.vtk";

	if (errors = fopen_s(&fp_result, file_name, "w") != 0){
		printf("\n file open failed \n");
	}

	fprintf(fp_result, "# vtk DataFile Version 2.0\n");
	fprintf(fp_result, "result\n");
	fprintf(fp_result, "ASCII\n");
	fprintf(fp_result, "DATASET UNSTRUCTURED_GRID\n");
	fprintf(fp_result, "POINTS %d float\n",N);
	
	for (int i = 0; i < N; i++){
		fprintf(fp_result, "%lf %lf %lf\n", no[i].x[0], no[i].x[1], no[i].x[2]);
	}

	fprintf(fp_result, "\n\n");
	fprintf(fp_result, "CELLS %d %d\n", E,E*9);
	
	for (int i = 0; i < E; i++){
		fprintf(fp_result, "8 ");
		for (int j = 0; j < 8; j++){
			fprintf(fp_result, "%d ", el[i].node[j]);
		}
		fprintf(fp_result, "\n");
	}
	fprintf(fp_result, "\n\n");
	fprintf(fp_result, "CELL_TYPES %d\n",E);
	
	for (int i = 0; i < E; i++){
		fprintf(fp_result, "12\n");
	}

	fprintf(fp_result, "\n\n");
	fprintf(fp_result, "POINT_DATA %d\n",N);
	//fprintf(fp_result, "LOOKUP_TABLE default\n");
	fprintf(fp_result, "\VECTORS displacement float\n");
	for (int i = 0; i < N; i++){
		fprintf(fp_result, "%lf %lf %lf\n", S[3 * i] ,S[3 * i + 1], S[3 * i + 2]);
	}
	fprintf(fp_result, "\VECTORS nodal_force float\n");
	for (int i = 0; i < N; i++){
		fprintf(fp_result, "%lf %lf %lf\n", no[i].xf[0], no[i].xf[1], no[i].xf[2]);
	}

	fprintf(fp_result, "\n\n");
	fprintf(fp_result, "CELL_DATA %d\n", E);
	fprintf(fp_result, "SCALARS mises float 1\n");
	fprintf(fp_result, "LOOKUP_TABLE mises\n");
	for (int i = 0; i < E; i++){
		fprintf(fp_result, "%lf\n",el[i].mises);
	}
	fclose(fp_result);
}