#include <iostream>
#include <ctime>
#include <algorithm>

using namespace std;

//declare constant 
const int gene = 15; //chromosome representation
const int budget = 50000; //budget for the relief (must not exceed)
const int damageSum = 80; //total damage sum
const float highRiskSum = 10.9833; //total high risk sum
const int reliefCostSum = 130000; //total relief sum
const int areaCount = 40; //total area count
const int population = 30; //population size 
const double crossoverProb = 0.8; //crossover probability

//damage severity for each area
const int damageSeverity[areaCount] = { 1,1,1,3,2,2,3,3,3,3,2,2,2,2,2,2,3,2,1,2,2,2,1,1,1,2,3,3,1,2,1,1,3,1,3,2,3,3,1,2};

//high risk ratio for each area
const float highRiskRatio[areaCount] = { 0.25,0.5,0.5,0.2,0.333333333,0.25,0.333333333,0.333333333,0.166666667,
	0.25,0.5,0.2,0.2,0.25,0.2,0.25,0.25,0.333333333,0.25,0.25,0.333333333,0.2,0.2,0.2,0.2,0.166666667,0.2,
	0.25,0.25,0.2,0.333333333,0.333333333,0.25,0.2,0.2,0.5,0.333333333,0.25,0.333333333,0.25 }; 

//relief cost for each area
const int reliefCost[areaCount] = { 4560,4360,4860,3770,2000,2280,2600,3200,3190,2030,2720,3310,2980,1670,4100,
	3780,3200,3730,3520,3600,2840,2870,2310,2420,3610,3350,2780,3830,2590,2880,3130,3470,3050,2500,4140,3750,
	2980,4480,3950,3610}; 


//declare required variables
int chromosome[population][gene]; //chromosome data structure
float fitness[population]; //fitness data structure
int parent[2][gene];//parent's data structure
int children[2][gene];//child's data structure

void initializePopulation() {

	cout << "\x1B[93mInitialization of Population\033[0m\n\n"; //DEBUG
	
	//for each chromosome (c) in the population
	for (int c = 0; c < population; c++) {

		bool used[41] = { false }; //initialize used array to false to keep track of used areas

		//for each gene (g) in the chromosome
		for (int g = 0; g < gene; g++) {
			int area; //initialize area variable

			do {
				area = rand() % 40 + 1; //generate random area number between 1 and 40
			} while (used[area]); //check if the area is already used

			chromosome[c][g] = area; //assign the area to the gene
			used[area] = true; //mark the area as used
		}
	}
	
	//print the chromosome - DEBUG
	//for each chromosome (c) in the population
	for (int c = 0; c < population; c++) {
		cout << "\tChromosome " << c + 1 << ": ";
		//for each gene (g) in the chromosome
		for (int g = 0; g < gene; g++) {
			cout << chromosome[c][g] << " ";
		}
		cout << endl;
	}
}

void evaluateChromosome() {
	
	cout << "\n\x1B[93mEvaluation of Chromosome\033[0m\n\n"; //DEBUG

	//declare constant for weightage
	const float damageWeight = 0.3;
	const float highRiskWeight = 0.6;
	const float reliefCostWeight = 0.1;

	//declare required variables
	int damageTotal;
	float highRiskTotal, reliefCostTotal;

	//for each chromosome (c) in the population
	for (int c = 0; c < population; c++) {

		//initialize total values
		damageTotal = 0;
		highRiskTotal = 0;
		reliefCostTotal = 0;

		//for each gene (g) in the chromosome
		for (int g = 0; g < gene; g++) {

			int areaIndex = chromosome[c][g] - 1; // Convert area number to array index

			// Accumulate damage severity, high risk, and relief cost
			damageTotal += damageSeverity[areaIndex];
			highRiskTotal += highRiskRatio[areaIndex];
			reliefCostTotal += reliefCost[areaIndex];
		}

		//calculate fitness value	
		if (reliefCostTotal >= budget) { //if relief cost exceeds budget, fitness value will be halved

			fitness[c] = ((damageWeight * damageTotal / damageSum) + (highRiskWeight * highRiskTotal / highRiskSum) + (reliefCostWeight * reliefCostTotal / reliefCostSum)) * 0.5;
		}
		else {
			fitness[c] = (damageWeight * damageTotal / damageSum) + (highRiskWeight * highRiskTotal / highRiskSum) + (reliefCostWeight * reliefCostTotal / reliefCostSum);
		}

		//print the fitness value - DEBUG
		cout << "\tChromosome " << c + 1 << " - fitness: " << fitness[c] << endl;
	}
}

void parentSelection() { //Tournament Selection
	
	cout << "\n\x1B[93mParent Selection\033[0m\n\n"; //DEBUG

	//declare required variables
	int parentIndex[2]; //parent index array
	int player1, player2; //player variables

	//make sure that the two parents are different
	do {
		//for each parent (p) in the parent index array
		for (int p = 0; p < 2; p++) {

			//pick random chromosomes as player 1
			player1 = rand() % population;

			//make sure player 2 is different from player 1
			do {
				//pick random chromosomes as player 2
				player2 = rand() % population;

			} while (player2 == player1);

			//print the selected players with their fitness - DEBUG
			cout << "\n\tParent number " << p + 1 << endl;
			cout << "\tPlayer 1 : " << player1 + 1 << endl;
			cout << "\tFitness : " << fitness[player1] << endl;
			cout << "\tPlayer 2 : " << player2 + 1 << endl;
			cout << "\tFitness : " << fitness[player2] << endl;

			//compare the fitness of the two players
			if (fitness[player1] > fitness[player2]) {
				parentIndex[p] = player1; //assign player 1 as parent
			}
			else {
				parentIndex[p] = player2; //assign player 2 as parent
			}
		}

	} while(parentIndex[0] == parentIndex[1]); 

	//print the winner of the tournament - DEBUG
	cout << "\x1B[97m\n\tWinner of the tournament " << parentIndex[0] + 1 << " and " << parentIndex[1] + 1 << "\033[0m\n" << endl;

	//assign the selected parents to the parent data structure
	//for each parent (p) in the parent data structure
	for (int p = 0; p < 2; p++) {
		//for each gene (g) in the chromosome
		for (int g = 0; g < gene; g++) {
			parent[p][g] = chromosome[parentIndex[p]][g];
		}
	}

	//print the selected parents - DEBUG
	cout << "\x1B[97m\tSelected Parents\033[0m\n";
	//for each parent (p) in the parent data structure
	for (int p = 0; p < 2; p++) {
		cout << "\tParent " << p + 1 << ": ";
		//for each gene (g) in the chromosome
		for (int g = 0; g < gene; g++) {
			cout << parent[p][g] << " ";
		}
		cout << endl;
	}
}

void crossover() { //order crossover

	cout << "\n\x1B[93mCrossover\033[0m\n\n"; //DEBUG

	//copy both parents chromosomes to the childs' chromosomes
	//for each parent (p) in the parent data structure
	for (int p = 0; p < 2; p++) {
		//for each gene (g) in the chromosome
		for (int g = 0; g < gene; g++) {
			children[p][g] = parent[p][g];
		}
	}

	//generate a random number between 0 and 1 to determine if crossover will occur
	double random = (rand() % 11) / 10.0;

	//if the random number is less than the crossover probability, crossover will occur
	if (random < crossoverProb) {

		//declare required variables
		int point1, point2;

		do {
			//generate two random points for crossover
			point1 = rand() % gene;
			point2 = rand() % gene;
		} while (point1 >= point2); //make sure point1 is less than point2


		//declare temporary variables
		int temp1[gene]{}, temp2[gene]{};
		int index = 0;

		//copy the genes between the two points to temporary arrays
		for (int g = point1; g <= point2; g++) {
			temp1[index] = parent[0][g];
			temp2[index] = parent[1][g];
			index++;
		}

		//swap the genes between the two points in the children chromosomes
		for (int g = point1; g <= point2; g++) {
			children[0][g] = temp2[g - point1];
			children[1][g] = temp1[g - point1];
		}
		//check for duplicates in the children chromosomes
		//for each parent (p) in the children data structure
		for (int p = 0; p < 2; p++) {
			//for each gene (g) in the chromosome
			for (int g = 0; g < gene; g++) {
				//if the gene is a duplicate, replace it with the corresponding gene from the parent
				for (int i = 0; i < gene; i++) {
					if (children[p][g] == children[p][i] && g != i) {
						children[p][g] = parent[p][i];
					}
				}
			}
		}


		//print the crossover points - DEBUG
		cout << "\tCrossover Points: " << point1 << " and " << point2 << endl;
		cout << "\tProbability of Crossover: " << random << endl;

		//print the children - DEBUG
		cout << "\x1B[97m\n\tChildren\033[0m\n";
		//for each parent (p) in the children data structure
		for (int p = 0; p < 2; p++) {
			cout << "\tChild " << p + 1 << ": ";
			//for each gene (g) in the chromosome
			for (int g = 0; g < gene; g++) {
				cout << children[p][g] << " ";
			}
			cout << endl;
		}
	}
	else {
		//print that crossover did not occur - DEBUG
		cout << "\tProbability of Crossover: " << random << endl;
		cout << "\tCrossover did not occur" << endl;
	}
}

int main() {
	srand(time(NULL));
	initializePopulation();
	evaluateChromosome();
	parentSelection();
	crossover();
	//mutation();
	//survivorSelection();
	return 0;
}
