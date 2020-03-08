#include <iostream>
#include <algorithm>
#include <queue>
#include <math.h>
#include <string>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;

int N;

struct Node
{
	Node(double prob, Node *l, Node *r)
	{
		probability = prob;
		left = l;
		right = r;
	}

	double probability;
	Node *left;
	Node *right;
};

Node *getNode(double prob)
{
	Node *node = new Node(prob, nullptr, nullptr);
	return node;
}

auto compare = [](Node *l, Node *r)
{
	return l->probability > r->probability;
};

priority_queue<Node*, vector<Node*>, decltype(compare)> priorityQueue(compare);
double probabilities[1000];

double calculateEntropy()
{
	double sum = 0;
	for (int i = 0; i < N; i++)
	{
		sum += probabilities[i] * log2(1 / probabilities[i]);
	}
	return sum;
}

double calculateEfficency(double Lsr, double Entropy)
{
	return (Entropy / Lsr)*100.;
}

void calculateLsr(Node *root,double &Lsr,int level) // Racunamo Lsr pomocu InOrder obilaska stabla
{
	if (root != nullptr)
	{
		calculateLsr(root->left,Lsr,level+1);

		if (root->left == nullptr && root->right == nullptr) // Ako je list onda ga racunamo za srednju duzinu
		{
			Lsr += level * root->probability;
			//cout << " + " << level << "*" << root->probability << " ";

		}
		calculateLsr(root->right,Lsr,level+1);
	}
}

void deleteTree(Node *root)
{
	if (root != nullptr)
	{
		deleteTree(root->left);
		deleteTree(root->right);
	}
	delete root;
}

template<class _Tnumber, class _Titerator >
bool next_variation
(
	_Titerator const& _First
	, _Titerator const& _Last
	, _Tnumber const& _Upper
	, _Tnumber const& _Start = 0
	, _Tnumber const& _Step = 1
)
{
	_Titerator _Next = _First;
	while (_Next != _Last)
	{
		*_Next += _Step;
		if (*_Next < _Upper)
		{
			return true;
		}
		(*_Next) = _Start;
		++_Next;
	}
	return false;
}

void printLine()
{
	cout << "-------------------------------------------\n";
}

int main()
{	
	ofstream f;
	f.open("HaffmanLog.txt");

	int prosirenje;
	
	cout << "Unesite broj simbola koje zelite da kodujete:\n";
	cin >> N;

	f << N << " simbola.\n";
	f << "Verovatnoce:\n";
	double sum_check = 0;
	while (true)
	{
		cout << "Unesite verovatnoce pojavljivanja tih simbola(zbir verovatnoca mora biti 1):\n";
		for (int i = 0; i < N; i++)
		{
			cin >> probabilities[i];
			f << probabilities[i] << " ";
			sum_check += probabilities[i];
		}
		f << "\n";
		if (sum_check == 1)
		{
			break;
		}
		else
		{
			cout << "Niste uneli validne verovatnoce! Zbir verovatnoca mora biti 1!\n";
		}
	}

	cout << "Unesite prosirenje do kog zelite da se stabla formiraju:\n";
	cin >> prosirenje;

	double firstEntropy = calculateEntropy();

	//vector<int> options{ 0, 1};
	//combination_with_repetiton(2, 2, options);


	for (int i = 0; i < prosirenje; i++)
	{

		clock_t time_beg = clock();
		vector<int> arr;
		arr.clear();
		for (int k = 0; k <= i; k++)
		{
			arr.push_back(0);
		}
		do {

			double probability = 1;
			for (auto u : arr)
			{
				probability *= probabilities[u];
				//cout << u << " ";
			}

			priorityQueue.push(getNode(probability));

			//cout << "\n";
			//cout << arr << "\n";
		} while (next_variation(arr.begin(), arr.end(), N, 0));

		Node *root = nullptr;

		while (!priorityQueue.empty())
		{
			Node *n1 = priorityQueue.top();
			priorityQueue.pop();
		

			if (priorityQueue.empty())
			{
				root = n1;
				break;
			}

			Node *n2 = priorityQueue.top();
			priorityQueue.pop();

			double prob = n1->probability + n2->probability;
			Node *n3 = new Node(prob, n2, n1);
			priorityQueue.push(n3);
		}
		double Entropy = double(i+1) * firstEntropy;
		double Lsr = 0;
		calculateLsr(root, Lsr, 0);
		double Efficency = calculateEfficency(Lsr,Entropy);

		deleteTree(root);

		clock_t time_end = clock();

		

		
		printLine();
	
		cout << "Prosirenje broj " << i + 1 << ":\n";
		cout << "Entropija: " << Entropy << " | " << "Srednja duzina kodne reci: " << Lsr << " | " << "Efikasnost :" << Efficency << "%\n";
		

		cout << "Vreme da se napravi hafmenovo stablo:\n";
		cout << "HH:MM:SS\n";
		double seconds = double(time_end - time_beg) / CLOCKS_PER_SEC;
		int minutes = seconds / 60;
		int hours = minutes / 60;
		minutes = minutes % 60;
		int secInt = seconds;
		secInt = secInt % 60;

		if(seconds < 60)
			cout << setw(2) << setfill('0') << hours << ":" << setw(2) << minutes << ":" << setw(2) << seconds << "\n";
		else
			cout << setw(2) << setfill('0') << hours << ":" << setw(2) << minutes << ":" << setw(2) << secInt << "\n";
	

		f << "Prosirenje broj " << i + 1 << ":\n";
		f << "Entropija: " << Entropy << " | " << "Srednja duzina kodne reci: " << Lsr << " | " << "Efikasnost :" << Efficency << "%\n";


		f << "Vreme da se napravi hafmenovo stablo:\n";
		f << "HH:MM:SS\n";

		if (seconds < 60)
			f << setw(2) << setfill('0') << hours << ":" << setw(2) << minutes << ":" << setw(2) << seconds << "\n";
		else
			f << setw(2) << setfill('0') << hours << ":" << setw(2) << minutes << ":" << setw(2) << secInt << "\n";


		f << "-----------------------------------\n";

	}

	f.close();

	return 0;
}

/*
0.65
0.05
0.08
0.15
0.04
0.03
*/