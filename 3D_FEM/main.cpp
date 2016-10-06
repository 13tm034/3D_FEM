#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"Header.h"


int main(void){

	//printf("                     ##                                                                      ##                      \n");
	//printf("  ## #               ##                                                 ###  ###             ##                      \n");
	//printf(" #  ##                       #                                           #    #                                      \n");
	//printf("#    #                       #                                           #    #                                      \n");
	//printf("#                            #                                           #    #                                      \n");
	//printf("#          ####    ####    ######     ####    ## ##     ####             #    #  ## ###    ####    ##   ##           \n");
	//printf(" ##       #    #      #      #       #    #  #  #  #   #    #            #    #   ##   #      #    #     #           \n");
	//printf("   ##          #      #      #            #  #  #  #        #            #    #   #    #      #    #     #           \n");
	//printf("     #     #####      #      #        #####  #  #  #    #####            #    #   #    #      #     #   #            \n");
	//printf("#     #   #    #      #      #       #    #  #  #  #   #    #            #    #   #    #      #     #   #            \n");
	//printf("#     #  #     #      #      #      #     #  #  #  #  #     #            #    #   #    #      #     #   #            \n");
	//printf("#     #  #     #      #      #   #  #     #  #  #  #  #     #            #    #   #    #      #      # #     #       \n");
	//printf("##   #   #    ##      #      #   #  #    ##  #  #  #  #    ##            #    #   #    #      #      # #    ###      \n");
	//printf("# ###     #### ## ########    ###    #### ## ## ## ##  #### ##            ####   ###  ### ########    #     ###      \n");

	printf("    _/      _/                      _/        _/                            _/                          _/      _/                  _/       \n");
	printf("   _/_/  _/_/    _/_/_/    _/_/_/  _/_/_/        _/_/_/      _/_/        _/_/_/_/    _/_/      _/_/    _/      _/          _/_/_/  _/_/_/    \n");
	printf("  _/  _/  _/  _/    _/  _/        _/    _/  _/  _/    _/  _/_/_/_/        _/      _/    _/  _/    _/  _/      _/        _/    _/  _/    _/   \n");
	printf(" _/      _/  _/    _/  _/        _/    _/  _/  _/    _/  _/              _/      _/    _/  _/    _/  _/      _/        _/    _/  _/    _/    \n");
	printf("_/      _/    _/_/_/    _/_/_/  _/    _/  _/  _/    _/    _/_/_/          _/_/    _/_/      _/_/    _/      _/_/_/_/    _/_/_/  _/_/_/	     \n");

	printf("ooooooooooo o       oooooooo8 ooooooooooo      ooooooooooo ooooooooooo oooo     oooo       oooooooo8 ooooo  oooo oooooooo8 ooooooooooo ooooooooooo oooo     oooo \n");
	printf(" 888    88 888     888        88  888  88       888    88   888    88   8888o   888       888          888  88  888        88  888  88  888    88   8888o   888  \n");
	printf(" 888ooo8  8  88     888oooooo     888           888ooo8     888ooo8     88 888o8 88        888oooooo     888     888oooooo     888      888ooo8     88 888o8 88  \n");
	printf(" 888     8oooo88           888    888           888         888    oo   88  888  88               888    888            888    888      888    oo   88  888  88  \n");
	printf("o888o  o88o  o888o o88oooo888    o888o         o888o       o888ooo8888 o88o  8  o88o      o88oooo888    o888o   o88oooo888    o888o    o888ooo8888 o88o  8  o88o \n");

	/*load number*/
	DISPLAY("LOAD NUMBER");
	int N, *pN, E, *pE, M, *pM;
	N = 0;
	E = 0;
	M = 0;
	pN = &N;		//節点数
	pE = &E;		//要素数
	pM = &M;		//材料種数
	
	load_number(pN, pE, pM);
	printf("NODE NUMBER:\tN=%d\n", N);
	printf("ELEMENTAL NUMBER:\tE=%d\n", E);
	printf("MATERIAL NUMBER:\tM=%d\n", M);


	/*declaret*/
	DISPLAY("DACLARE");
	declare_check();
	node *no;		//節点情報
	element *el;	//要素情報
	material *m;	//材料種
	no = (node *)malloc(N*sizeof(node));
	el = (element *)malloc(E*sizeof(element));
	m = (material *)malloc(M*sizeof(material));
	declare_check1(E, N, M);
	printf("no[%d]\n", N);
	printf("el[%d]\n", E);
	printf("m[%d]\n", M);
	double **K;
	K = (double **)malloc(sizeof(double *)*(N + N + N));
	for (int i = 0; i < N + N + N; i++){
		K[i] = (double *)malloc(sizeof(double)*(N + N + N));
	}
	double *S;
	S = (double *)malloc(sizeof(double)*(N + N + N));
	declare_check2(E, N, M);
	printf("%d*%d STIFFNES MATRIX\n", N+N+N,N+N+N);
	printf("%d STRAIN VECTOR\n", N + N + N);
	
	/*initialization*/
	initial(K, S, no, el, m, N, E, M);
	
	/*load_node_element*/
	DISPLAY("LOAD");
	info_N(no,N);
	int *NASTRAN_no;
	NASTRAN_no = (int *)malloc(sizeof(int)*(no[N-1].num));
	for (int i = 0; i < no[N - 1].num; i++){
		NASTRAN_no[i] = 0;
	}

	for (int i = 0; i < N; i++){
		NASTRAN_no[no[i].num] = i;
	}
	
	//for (int i = 0; i < no[N - 1].num; i++){
	//	printf("%d,%d\n", NASTRAN_no[i],i);
	//}
	//NASATRAN node　から元のノードが参照できるようにする

	info_E(el,E);
	

	for (int i = 0; i < E; i++){
		for (int j = 0; j < 8; j++){
			/*printf("(%d,%d)", el[i].node[j], NASTRAN_no[el[i].node[j]]);*/
			el[i].node[j] = NASTRAN_no[el[i].node[j]];
		}
		/*printf("\n");*/
	}
	//要素節点番号をNASTEANnodeから本来のノードに変換

	int element[8] = {};

	for (int i = 0; i < E; i++){
		for (int j = 0; j < 8; j++){
			element[j] = el[i].node[j];
		}
		//el[i].node[0] = element[4];
		//el[i].node[1] = element[7];
		//el[i].node[2] = element[6];
		//el[i].node[3] = element[5];
		//el[i].node[4] = element[0];
		//el[i].node[5] = element[3];
		//el[i].node[6] = element[2];
		//el[i].node[7] = element[1];

		el[i].node[0] = element[3];
		el[i].node[1] = element[7];
		el[i].node[2] = element[4];
		el[i].node[3] = element[0];
		el[i].node[4] = element[2];
		el[i].node[5] = element[6];
		el[i].node[6] = element[5];
		el[i].node[7] = element[1];


		//el[i].node[0] = element[0];
		//el[i].node[1] = element[1];
		//el[i].node[2] = element[2];
		//el[i].node[3] = element[3];
		//el[i].node[4] = element[4];
		//el[i].node[5] = element[5];
		//el[i].node[6] = element[6];
		//el[i].node[7] = element[7];
		
	}


	info_M(m);
	printf("N=%d,E=%d,M=%d\n", N, E, M);
	for (int i = 0; i < N; i++){
		printf("node=%d, x=%10f,%10f,%10f, xf=%10f,%10f,%10f, xrc=%d,%d,%d\n"
			, i, no[i].x[0], no[i].x[1], no[i].x[2], no[i].xf[0], no[i].xf[1], no[i].xf[2]
			, no[i].xrc[0], no[i].xrc[1], no[i].xrc[2]);
	}

	for (int i = 0; i < E; i++){
		printf("element:%d material: %d (",i,el[i].m);

		for (int j = 0; j < 8; j++){

			printf("%d,", el[i].node[j]);

		}
		printf(")\n");

	}
	for (int i = 0; i < N; i++){
		S[3 * i] = no[i].xf[0];
		S[3 * i + 1] = no[i].xf[1];
		S[3 * i + 2] = no[i].xf[2];
	}


	printf("e=%lf,v=%lf\n",m[0].e,m[0].v);
	original_output(no, N);
	system("PAUSE");

	/*Kmatrix start*/
	DISPLAY("STIFFNESS MATRIX");
	//Kmatrix_log();
	Kmatrix(no, el, m, K, E, N, M);
	
	DISPLAY("SET BOUNDARY CONDITIONS")
	setBC(no, S, K, E, N, M);
	
	//Kmatrix_BC(K, N);
	//S_BC(S, N);

	DISPLAY("SOLVING MATRIX");
	clock_t start, end;
	start = clock();
	solve_matrix(K, S, N);
	end = clock();
	printf("処理時間　%d\n", end - start);
	for (int i = 0; i < N; i++){
		no[i].xd[0] = S[i + i + i];
		no[i].xd[1] = S[i + i + i + 1];
		no[i].xd[2] = S[i + i + i + 2];
	
	}

	difference_output(S, N);
	coordinate_output(no, N);
	element_node_output(el, E);
	strain_stress_calc(no, el, m, E, N, M);
	result_output(el, no, S, E, N);
	

	system("PAUSE");
}


