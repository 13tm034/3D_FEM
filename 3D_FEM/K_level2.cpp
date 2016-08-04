#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"Header.h"

void addK(element *el, double Ke[][24], double **K, int i){

	/*�v�f�ړ_�ԍ�����S�̐ߓ_�ԍ����o��*/
	int n[8] = {};					//�S�̐ߓ_�ԍ�

	//for (int j = 0; j < 8; j++){
	//	n[j] = el[i].node[j];
	//}


	n[0] = el[i].node[4];
	n[1] = el[i].node[7];
	n[2] = el[i].node[6];
	n[3] = el[i].node[5];
	n[4] = el[i].node[0];
	n[5] = el[i].node[3];
	n[6] = el[i].node[2];
	n[7] = el[i].node[1];
	

	/*�S�̐ߓ_�ԍ��ɏ]���v�f�����s���S�̍����s��ɉ��Z*/
	for (int j = 0; j < 8; j++){
		for (int k = 0; k < 8; k++){
			K[n[j] + n[j] + n[j]][n[k] + n[k] + n[k]] += Ke[j + j + j][k + k + k];
			
			K[n[j] + n[j] + n[j] + 1][n[k] + n[k] + n[k]] += Ke[j + j + j + 1][k + k + k];
			K[n[j] + n[j] + n[j] + 2][n[k] + n[k] + n[k]] += Ke[j + j + j + 2][k + k + k];
			
			K[n[j] + n[j] + n[j]][n[k] + n[k] + n[k] + 1] += Ke[j + j + j][k + k + k + 1];
			K[n[j] + n[j] + n[j]][n[k] + n[k] + n[k] + 2] += Ke[j + j + j][k + k + k + 2];
			
			K[n[j] + n[j] + n[j] + 1][n[k] + n[k] + n[k] + 1] += Ke[j + j + j + 1][k + k + k + 1];
			K[n[j] + n[j] + n[j] + 2][n[k] + n[k] + n[k] + 2] += Ke[j + j + j + 2][k + k + k + 2];
			
			K[n[j] + n[j] + n[j] + 1][n[k] + n[k] + n[k] + 2] += Ke[j + j + j + 1][k + k + k + 2];
			K[n[j] + n[j] + n[j] + 2][n[k] + n[k] + n[k] + 1] += Ke[j + j + j + 2][k + k + k + 1];

		}
	}

	/*printf("\nKmatrix_CHECK\n");
	double check = 0;
	for (int j = 0; j < 24; j++){
		for (int k = 0; k < 24; k++){
			check = K[j][k] - Ke[j][k];
			printf("%lf,", check);
		}
		printf("\n");
	}*/

}


void Kematrix(node *no, element *el, material *m, double Ke[][24],int i){

	//order5 point5
	/*double a = sqrt(1120.0);
	double b = (70 + a) / 126;
	double c = (70 - a) / 126;
	double d = 1 / (15 * (c - b));*/

	//point
	double p[5] = {};
	p[0] = -0.9061798459;
	p[1] = -0.5384693101;
	p[2] = 0;
	p[3] = 0.5384693101;
	p[4] = 0.9061798459;

	//weight
	double weight[5];
	weight[0] = 0.2369268851;
	weight[1] = 0.4786286704;
	weight[2] = 0.5688888889;
	weight[3] = 0.4786286704;
	weight[4] = 0.2369268851;


	for (int j = 0; j < 5; j++){
		for (int k = 0; k < 5; k++){
			for (int l = 0; l < 5; l++){

				double r[3] = {};
				r[0] = p[l];
				r[1] = p[k];
				r[2] = p[j];


				double w[3] = {};
				w[0] = weight[l];
				w[1] = weight[k];
				w[2] = weight[j];

				double pKe[24][24] = {};


				double B[6][24] = {};			//B�}�g���N�X	
				double J[3][3] = {};			//jacob�}�g���N�X
				Bmatrix(no, el, B, i,r,J);



				double detJ = 0;				//jacob��det�̒l
				double *det;
				det = &detJ;
				det_jacob(J, det);
				double D[6][6] = {};			//D�}�g���N�X
				Dmatrix(D, m, i, el);

				BDBmatrix(B, D, pKe);			//B��D�s��̐�

				/*for (int j = 0; j < 24; j++){
					for (int k = 0; k < 24; k++){

						printf("%lf", pKe[j][k]);
					}
					printf("\n");
				}*/


				product_detJ_weight(pKe, detJ, w);		//detJ��weight��pKe�ɂ�����





				addKe(pKe, Ke);							//pKe��Ke�ɑ����Ă���

				}
			}
		}
}


	





void load_stress(node *no, double *S,int N){

	for (int i = 0; i < N; i++){
		S[i + i + i] = no[i].xf[0];
		S[i + i + i + 1] = no[i].xf[1];
		S[i + i + i + 2] = no[i].xf[2];
	}

}

void set_RC(node *no, double *S, double **K, int E, int N, int M){

	double *temp;
	temp = (double *)malloc(sizeof(double)*(N + N + N));
	for (int i = 0; i < N + N + N; i++){
		temp[i] = 0;
	}

	for (int i = 0; i < N; i++){			//�ߓ_i��
		for (int j = 0; j < 3; j++){		//j�������ɂ���
			set_eRC(K, no, S, temp, i, j, N);
			
		}
	}

	//free(temp);
}