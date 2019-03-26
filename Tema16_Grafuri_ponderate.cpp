// Tema16_Grafuri_ponderate.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <limits>

using namespace std;


class Graf {

	int **matrix;
	int **weight; // matricea ponderilor 
	int **nodes;  // matricea legaturilor 
	int **nodList; // lista de noduri 
	int *con;
	int nrCon;
	
protected:
	int size;
	int bonds;
public:
	Graf() {
		size = 0;
		matrix = NULL;
	};

	Graf(int max) {

		size = max;
		matrix = new int*[max + 1];
		weight = new int*[max + 1];
		nodes = new int*[max + 1];
		nodList = new int*[max + 1];
		con = new int[size + 1];


		for (int i = 1; i <= max; i++) {
			matrix[i] = new int[max + 1];
			weight[i] = new int[max + 1];
			nodes[i] = new int[max + 1];
			nodList[i] = new int[max + 1];
		}

		for (int i = 1; i <= max; i++)
			for (int j = 1; j <= max; j++) {
				matrix[i][j] = INT_MAX;
				weight[i][j] = INT_MAX;
				nodes[i][j] = 0;
				nodList[i][j] = 0;
			}
	}

	/*~Graf() {
		for (int i = 1; i <= size; i++)
			delete[] matrix[i];
	}*/

	void conectList();
	void royFloyd();
	void setNodes();
	void copyWeight();
	void setWeight();
	void setList();
	void Prim(int nod);

	friend istream &operator >> (istream & input, Graf& g1);
	friend ostream &operator << (ostream & output, Graf& g1);
    friend Graf operator *(const Graf &, const Graf &);
	friend Graf operator ==(const Graf &, const Graf &);

	
};

istream& operator >> (istream & input, Graf& g1) {
	int i, j,m;
	input >> g1.size;
	input >> g1.bonds;
	for (int k = 1; k <= g1.bonds; k++) {
		input >> i;
		input >> j;
		input >> m;
		g1.matrix[i][j] = m;
		g1.matrix[j][i] = m;
	}
	return input;
}

ostream& operator << (ostream & output, Graf& g1) {
	for (int i = 1; i <= g1.size; i++) {
		for (int j = 1; j <= g1.size; j++)
			output << g1.matrix[i][j] << ' ';
		output << endl;
	}
	return output;
}

//Graf operator *(const Graf & g1, const Graf & g2) {
//
//	auto g3 = new Graf(10);
//	if (g1.size == g2.size) {
//		g3->size = g1.size;
//		for (int i = 1; i <= g1.size; i++)
//			for (int j = 1; j <= g1.size; j++) {
//				if (g1.matrix[i][j] < g2.matrix[i][j])
//					g3->matrix[i][j] = g1.matrix[i][j];
//				else
//					g3->matrix[i][j] = g2.matrix[i][j];
//			}
//		return *g3;
//
//	}
//	cout <<" Grafurile alese nu au acelas numar de noduri " << endl;
//
//}
Graf  operator == (const Graf & g1, const Graf & g2) {
	if (g1.size == g2.size) {

		int ok = 1;
		for (int i = 1; i <= g1.size && ok == 1; i++)
			for (int j = 1; j <= g1.size && ok == 1; j++)
				if (g1.matrix[i][j] != g2.matrix[i][j]) ok = 0;
		if (ok) {
			cout << "DA" << endl;
			return true;
		}
		else {
			cout << "NU" << endl;
			return false;
		}
	}
	else {
		cout << "NU" << endl;
		return false;
	}
}

void Graf::royFloyd() {
	for (int k = 1; k <= size; k++)
		for (int i = 1; i <= size; i++)
			for (int j = 1; j <= size; j++)
				if (i != j && j != k && i != k && matrix[i][k] != INT_MAX && matrix[k][j] != INT_MAX )
					if (matrix[i][j] > matrix[i][k] + matrix[k][j]) {
						weight[i][j] = matrix[i][k] + matrix[k][j];
						nodes[i][j] = k;
						nodes[j][i] = k;
					}
}
void Graf::copyWeight() {
	for (int i = 1; i <= size; i++)
		for (int j = 1; j <= size; j++)
			weight[i][j] = matrix[i][j];
}
void Graf::setWeight() {
	cout << endl;
	cout << "Matricea ponderilor este"<<endl;
	for (int i = 1; i <= size; i++) {
		for (int j = 1; j <= size; j++)
			cout << weight[i][j] << ' ';
		cout << endl;
	}
}
void Graf::setNodes() {
	cout << endl;
	cout << "Matricea nodurilor este:"<<endl;
	for (int i = 1; i <= size; i++) {
		for (int j = 1; j <= size; j++)
			cout << nodes[i][j] << ' ';
		cout << endl;
	}
}
void Graf::conectList() {

	int nod = 1, nr = 1, k = 1;
	int *vis, *tail;
	int first = 0;
	int last = 0;
	int sum = 0;

	vis = new int[size + 1];
	tail = new int[size + 1];

	// initializarea vectorului de vizitati
	for (int i = 1; i <= size; i++)
		vis[i] = 0;

	vis[1] = nr;
	tail[0] = 1;

	while (sum != size) {

		first = last = 0;

		while (first <= last) {
			nod = tail[first++];
			for (int i = 1; i <= size; i++)
				if (matrix[nod][i] != INT_MAX && !vis[i] ) {
					vis[i] = nr;
					tail[++last] = i;
				}

		}

		nodList[vis[1]][0] = last + 1; // prima pozitie va retine numarul de elemente din componenta conexa
		for (int i = 0; i <= last + 1; i++)
			nodList[vis[1]][i + 1] = tail[i];
		sum = sum + last + 1;
		if (sum < size) k++;
	}

	// determinarea conexitarii grafului
	nrCon = k;
	if (k = 1) cout << "Graful dat este conex !" << endl;

}
void Graf::setList( ) {
	for (int i = 1; i <= nrCon; i++) {
		cout << "Componentele conexe din graful dat sunt: " << endl;
		for (int j = 1; j <= nodList[i][0]; j++)
			cout << nodList[i][j] << ' ';
		cout << endl;
	}
	
}

void Graf::Prim(int nod) {

		int *parent, *key, *mstSet;
		parent = new int[size + 1];
		key = new int[size + 1];
		mstSet = new int[size + 1];

		int min, min_index;
		// initializam toate cheile cu valoarea maxima 

		for (int j = 1; j <= size; j++)
			key[j] = matrix[nod][j] , mstSet[j] = 0;

		for (int j = 1; j <= size; j++) {

			min = INT_MAX;

			for (int k = 1; k <= size; k++)
				if (mstSet[k] == 0 && key[k] < min) {
					min = key[k];
					min_index = k;
				}
			mstSet[min_index] = 1;

			for (int k = 1; k < size; k++)
				if (matrix[nod][k] && mstSet[k] == 0 && matrix[nod][k] < key[k]) {
					parent[k] = min_index;
					key[min_index] = matrix[nod][k];
				}

		}

		for (int j = 1; j <= size; j++) {
			cout << parent[j] << ' ' << j << ' ' << matrix[j][parent[j]] << endl;
		}
}

int main(){

	Graf g1(10);
	Graf g2(10);
	Graf g3(10);

	cin >> g1;
	cout << g1 << endl;

	g1.copyWeight();
	g1.royFloyd();
	g1.setWeight();
	g1.setNodes();

	g1.conectList();
	g1.setList();

	/*g1.Prim(1);*/

	cin >> g2;
	cout << g2 << endl;
	
	g3 = g1 * g2;
	cout << g3;
	
	g1 == g2;


	
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

