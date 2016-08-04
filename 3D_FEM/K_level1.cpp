#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"Header.h"

void Kmatrix(node *no, element *el, material *m, double **K, int E, int N, int M){



	for (int i = 0; i < E; i++){			//要素iについて
		double Ke[24][24] = {};
		Kematrix_log(i);
		Kematrix(no, el, m, Ke, i);			//Kマトリクスの作成
		Kematrix_log1(Ke, i);
		addK(el, Ke,K,i);
		printf("Element Stiffness Matrix:\t%d\n", i);
	}


}

void setBC(node *no, double *S, double **K, int E, int N, int M){
	load_stress(no, S,N);

	set_RC(no, S, K, E, N, M);

}



