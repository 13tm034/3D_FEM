//20160516　データ構造の決定
//20160519	Bマトリクス作成→dndxの計算の前まで→dndxの計算→とりあえずBマトリクス完成
//20160520	関数の多階層化→Dマトリクス→BDBの手前まで（BマトリクスをΔで割っていないことに注意）
//20160523	ソースファイルの整理→関数Bmatrixの細分化→BマトリクスをΔで割る。（必要ない？と思われる）→要素剛性行列を全体剛性行列に加算→境界条件の適用
//20160524	境界条件の適用→連立方程式を解く→要素剛性行列に各要素の体積をかける
//20160609	積分方法が分かったのでデバッグ開始、処理層から実行→一部を除いて現在できるところはデバッグ完了→ガウス積分の実装方法を考える。
//20160610	5次のガウス積分を実装→実装完了→デバッグ→ソルバ実装(ガウスジョルンダン)
//20160613	malloc関数の実装→ロード部分を実装
//20160614	全プログラムの実行→jacobの部分に誤りがあり
//20160615  jacobをデバッグ開始
//20160616	jacobのバグを発見→Kematrixにバグがありそう
//20160617	Kematrix周辺デバッグ
//20160620	Bmatrixの最後でスタック、現状は無視→得られた変位はおかしい→境界条件適応後のKは問題なさそう→一応結果は出た→paraviewで図示
//20160622	新たな条件で再計算
//20160624	計算結果に難あり、solveの解がMATLABと同じではない。
//20160629	軸方向引張の結果がおかしい
//20160630	データ構造を横山さんに渡すために構造体部分を確認
//20160711	共益勾配法ソルバーを実装
//20160712	前処理の実装、ファイルからのロード部分を実装
//20160719	ナストランファイル読み込み部分の作成
//20160720  同上
//20160721	節点番号のつけ方にナストランとプログラムで差があるdn/drの部分を変える。
//20160721	節点番号のつけ方をdn/drの部分を元に戻し、入力の時点でかえる
//20160726	節点番号のつけ方の試行
//20160811	デバッグ中ナストラン一要素では発散→横山さんに確認
#include<time.h>
#include<string.h>

#ifndef _HEADER_H_
#define _HEADER_H_

/*macros*/
//#define E 1		//要素数
//#define N E+7	//節点数
//#define M 1		//材料種
#define EPS 0.00000001


#define DISPLAY(string) printf("---------------------------------------------------------%s---------------------------------------------------------\n",string);


/*structure*/
typedef struct nodes{

	double x[3];		//座標
	int xrc[3];			//拘束条件
	double xd[3];		//変位
	double xf[3];		//節点力

}node;

typedef struct elements{

	int node[8];		//要素節点座標
	int m;				//材料番号
}element;

typedef struct materials{
	double v;		//ポアソン比
	double e;		//縦弾性係数
}material;




/*function*/

/*level 1*/
void Kmatrix(node *no, element *el,material *m,double **K,int E,int N,int M);		//全体剛性要素マトリクス
void setBC(node *no, double *S, double **K,int E,int N,int M);						//境界条件適用
/*level 2*/
void Kematrix(node *no, element *el, material *m,double Ke[][24],int i);			//要素剛性マトリクス
void addK(element *el, double Ke[][24], double **K, int i);							//要素剛性行列を全体剛性行列に加算
void load_stress(node *no, double *S,int N);												//応力ロード
void set_RC(node *no, double *S, double **K,int E,int N,int M);						//変位拘束設定
/*level 3*/
void Bmatrix(node *no, element *el, double B[][24],int i,double *r,double J[][3]);	//Bマトリクス
void Dmatrix(double D[][6], material *m,int i,element *el);							//Dマトリクス
void BDBmatrix(double B[][24], double D[][6],double Ke[][24]);						//BDBの計算（実質的Keの計算）
void set_eRC(double **K, node *no, double *S, double *temp,int i,int j,int N);		//境界条件の適用
void det_jacob(double J[][3], double *detJ);										//jacob行列のdet
void product_detJ_weight(double pKe[][24], double detJ, double *w);					//detJとweightをpKeにかける
void addKe(double pKe[][24], double Ke[][24]);										//pKeをKeの要素に足す
/*level 4*/
void rollingB(double Bt[][6], double B[][24]);										//Bの転地
void productBtD(double Bt[][6], double D[][6],double BD[][6]);						//BtDの積
void productBDB(double BD[][6], double B[][24], double Ke[][24]);					//BDBの積
void jacob(double J[][3], double dndr[][8], double nd[][8],double *r);				//jacob行列
void inv_jacob(double invJ[][3], double J[][3]);									//jacob行列の逆行列
void DnDx(double dndx[][8], double invJ[][3],double dndr[][8]);						//DnDxの作成

//solve
void solve_matrix(double **K, double *S, int N);									//マトリクスのソルバ

//input_outputload
void difference_output(double *S, int N);
void input(node *no, element *el, material *m);										//各情報の入力(手入力)
void original_output(node *no, int N);


//load
void load_number(int *N, int *E,int *M);											//要素数、節点数の入力
void info_N(node *no);																//節点情報の読み込み
void info_E(element *el,int E);															//要素情報の読み込み
void info_M(material *m);

//initial

void initial(double **K, double *S, node *no, element *el, material *ml,int N,int E,int M); //動的な変数の初期化


//log
void declare_check(void);
void declare_check1(int E, int N, int M);
void declare_check2(int E, int N, int M);
void Kmatrix_log(void);
void Kematrix_log(int i);
void Kematrix_log1(double Ke[][24], int i);
void Kmatrix_BC(double **K, int N);
void S_BC(double *S, int N);


//timer

void time_display(char *s);


//kari
void inv_K(double **K);


//output
void coordinate_output(node *no, int N);
void element_node_output(element *el, int E);
void result_output(element *el, node *no, double *S, int E, int N);

#endif