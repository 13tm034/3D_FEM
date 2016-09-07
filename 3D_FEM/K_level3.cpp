#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"Header.h"

void Bmatrix(node *no, element *el, double B[][24], int i,double *r,double J[][3]){


	/*�v�f�ړ_������W���o��*/
	int n[8] = {};					//�S�̐ߓ_�ԍ�
	


	for (int j = 0; j < 8; j++){
		n[j] = el[i].node[j];
	}


	double nd[3][8] = {};			//��v�f�̊e�ߓ_�̍��W�l

	for (int j = 0; j < 8; j++){
		nd[0][j] = no[n[j]].x[0];
		nd[1][j] = no[n[j]].x[1];
		nd[2][j] = no[n[j]].x[2];
		//printf("node %d = (%lf,%lf,%lf)\n", j, nd[0][j], nd[1][j], nd[2][j]);
	}

	

	/*jacob�}�g���N�X�̍쐬*/
	double dndr[3][8] = {};		//dn/dr
	double kariJ[3][3] = {};
	jacob(J, dndr, nd, r);

	/*printf("J\n");
	for (int j = 0; j < 3; j++){
		for (int k = 0; k < 3; k++){
			printf("%lf,", J[i][j]);
		}
		printf("\n");
	}*/


	for (int j = 0; j < 3; j++){
		for (int k = 0; k < 3; k++){
			kariJ[j][k] = J[j][k];
		}
	}



	/*jacob�}�g���N�X�̋t�s��*/
	double invJ[3][3] = {};		//jacob�̋t�s��
	inv_jacob(invJ, J);
	for (int j = 0; j < 3; j++){
		for (int k = 0; k < 3; k++){
			J[j][k] = kariJ[j][k];
		}
	}

	//printf("invJ\n");
	//for (int j=0; j < 3; j++){
	//	for (int k = 0; k < 3; k++){
	//		printf("%lf,", invJ[i][j]);
	//	}
	//	printf("\n");
	//}

	//printf("kariJ\n");
	//for (int j = 0; j < 3; j++){
	//	for (int k = 0; k < 3; k++){
	//		printf("%lf,", kariJ[i][j]);
	//	}
	//	printf("\n");
	//}



	/*dndx�̌v�Z*/
	double dndx[3][8] = {};		//dn/dx
	DnDx(dndx, invJ,dndr);
	

	/*B�}�g���N�X�֓W�J*/
	for (int j = 0; j < 8; j++){
		B[0][j + j + j] = dndx[0][j];
		B[1][j + j + j + 1] = dndx[1][j];
		B[2][j + j + j + 2] = dndx[2][j];
		
		B[3][j + j + j] = dndx[1][j];
		B[3][j + j + j + 1] = dndx[0][j];
		
		B[5][j + j + j] = dndx[2][j];
		B[5][j + j + j + 2] = dndx[0][j];
		
		B[4][j + j + j + 1] = dndx[2][j];
		B[4][j + j + j + 2] = dndx[1][j];
	}


}

void Dmatrix(double D[][6], material *m, int i, element *el){	//20160609debug

	double e = m[el[i].m].e;		// �|�A�\����
	double v = m[el[i].m].v;		//�c�e���W��

	double a = e*(1 - v) / ((1 + v)*(1 - 2 * v));

	for (int j = 0; j < 3; j++){			//�Ίp����
		for (int k = 0; k < 3; k++){
			D[j][k] = 1;
		if (j == k){
			D[j + 3][k + 3] = (1 - 2 * v) / (2 * (1 - v));//if
		}
		}
	}

	D[0][1] = v / (1 - v);					//�Ίp�����ȊO
	D[0][2] = v / (1 - v);
	D[1][2] = v / (1 - v);
	D[1][0] = v / (1 - v);
	D[2][0] = v / (1 - v);
	D[2][1] = v / (1 - v);


	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 6; j++){
			D[i][j] = a*D[i][j];
		}
	}

}


void BDBmatrix(double B[][24], double D[][6], double Ke[][24]){

	double Bt[24][6] = {};					//B�̓]�n
	rollingB(Bt, B);



	double BD[24][6] = {};					//B��D�̐�
	productBtD(Bt, D, BD);


	productBDB(BD, B, Ke);					//BD��B�̐�


}

void set_eRC(double **K, node *no, double *S, double *temp, int i, int j,int N){

	int count = 1;
	if (no[i].xrc[j] == 1){									//�ψʍS���������Ă����

		printf("Restraint Node:\t%d\n", i);
		for (int k = 0; k < N + N + N; k++){
			temp[k] = K[k][i + i + i + j] * no[i].xd[j];	//temp�ɐߓ_�ψʂƍ����s���3i+j�s�̈���ς������̂���
		}
		temp[i + i + i + j] = 0;							//�ψʍS�������������ߓ_�̍S��������0����
		S[i + i + i + j] = no[i].xd[j];						//�ψʍS���̂��������ߓ_�̕ψʕ����̕ψʂ���͏��ɂ���B


		for (int k = 0; k < N + N + N; k++){
			S[k] = S[k] - temp[k];							//�����̉��̓x�N�g������temp������
			K[k][i + i + i + j] = 0;						//3j+i�s�ڂ̈��A3j+i��ڂ̈�s��0����
			K[i + i + i + j][k] = 0;
		}

		K[i + i + i + j][i + i + i + j] = 1;				//3j+i�s3j+i��ڂ�1����
		count++;
	}

}

void det_jacob(double J[][3], double *detJ){


	*detJ = J[0][0] * (J[1][1] * J[2][2] - J[1][2] * J[2][1])
		- J[0][1] * (J[1][0] * J[2][2] - J[1][2] * J[2][0])
		+ J[0][2] * (J[1][0] * J[2][1] - J[1][1] * J[2][0]);
	
	/*printf("detJ=%lf", *detJ);

	if (*detJ <= 0){
		system("PAUSE");


	}*/

}

void product_detJ_weight(double pKe[][24], double detJ, double *w){

	for (int i = 0; i < 24; i++){
		for (int j = 0; j < 24; j++){

			pKe[i][j] = pKe[i][j] * detJ*w[0] * w[1] * w[2];

		}
	}


}

void addKe(double pKe[][24], double Ke[][24]){

	for (int i = 0; i < 24; i++){
		for (int j = 0; j < 24; j++){
			Ke[i][j] = Ke[i][j] + pKe[i][j];
		}
	}


}