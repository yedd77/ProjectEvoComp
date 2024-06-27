#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>

using namespace std;

//declare constant 
const int gene = 15; //chromosome representation
const int budget = 50000; //budget for the relief (must not exceed)
const int damageSum = 80; //total damage sum
const float highRiskSum = 10.9833; //total high risk sum
const int reliefCostSum = 130000; //total relief sum
const int areaCount = 40; //total area count
const int population = 30; //population size
const double crossoverProb = 0.5; //crossover probability
const double mutationProb = 0.1; //mutation probability
const int maxGeneration = 1000; //number of generations

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
int tempChromosome[population*2][gene]; //temporary chromosome data structure
int countNewChromo;
float fitness[population]; //fitness data structure
int parent[2][gene];//parent's data structure
int children[2][gene];//child's data structure
double avgFitness = 0; //average fitness
double bestFitness = -1.0; //best fitness

//declare constant for weightage
const float damageWeight = 0.3;
const float highRiskWeight = 0.6;
const float reliefCostWeight = 0.1;

//Declare File
ofstream avgFitnessFile; 
ofstream bestFitnessFile;
ofstream bestChromosomeFile;

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
}

void printChromosome() {

	//Display Chromosomes - DEBUG
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

		//print the crossover points - DEBUG
		cout << "\tCrossover Points: " << point1 << " and " << point2 << endl;
		cout << "\tProbability of Crossover: " << random << endl;

		//declare temporary variables
		int temp1[gene]{}, temp2[gene]{};

		//create dynamic array to store all the children genes using vector
		vector<int> queue1, queue2;

		//DEBUG - STEP 2
		//copy the genes between the two points to temporary arrays
		for (int g = point1; g <= point2; g++) {
			temp1[g] = parent[0][g];
			temp2[g] = parent[1][g];
		}

		//DEBUG 
		// print the temporary arrays - DEBUG
		cout << "\x1B[97m\n\tTemporary Arrays\033[0m\n";
		cout << "\tTemporary 1: ";
		for (int i = 0; i < gene; i++) {
			cout << temp1[i] << " ";
		}
		cout << endl;
		cout << "\tTemporary 2: ";
		for (int i = 0; i < gene; i++) {
			cout << temp2[i] << " ";
		}
		cout << endl;

		//store the other values in temp as -1 means empty
		for (int g = 0; g < gene; g++) {
			if (temp1[g] == 0) {
				temp1[g] = -1;
			}
			if (temp2[g] == 0) {
				temp2[g] = -1;
			}
		}
		
		//DEBUG - STEP 3 
		//Copy the remaining genes after point2 to the temporary vectors
		for (int g = point2 + 1; g < gene; g++) {
			queue1.push_back(parent[0][g]);
			queue2.push_back(parent[1][g]);
		}

		//copy the remaining genes before point2 to the temporary vectors
		for (int g = 0; g <= point2; g++) {
			queue1.push_back(parent[0][g]);
			queue2.push_back(parent[1][g]);
		}

		//DEBUG - STEP 4 and 5
		//remove the values on the queue1 from respective temporary array
		for (int i = 0; i < gene; i++) {
			if (temp2[i] != -1) {
				queue1.erase(remove(queue1.begin(), queue1.end(), temp2[i]), queue1.end());
			}
		}
		
		//remove the values on the queue2 from respective temporary array
		for (int i = 0; i < gene; i++) {
			if (temp1[i] != -1) {
				queue2.erase(remove(queue2.begin(), queue2.end(), temp1[i]), queue2.end());
			}
		}

		//swap the two values in the temporary arrays 
		int temp[gene]{};
		for (int g = 0; g < gene; g++) {
			temp[g] = temp1[g];
			temp1[g] = temp2[g];
			temp2[g] = temp[g];
		}

		//DEBUG
		//print the queue - DEBUG
		cout << "\x1B[97m\n\tQueue (Before)\033[0m\n";
		cout << "\tQueue 1: ";
		for (int i = 0; i < queue1.size(); i++) {
			cout << queue1[i] << " ";
		}
		cout << endl;
		cout << "\tQueue 2: ";
		for (int i = 0; i < queue2.size(); i++) {
			cout << queue2[i] << " ";
		}
		cout << endl;

		//DEBUG - STEP 6
		// fill the empty spaces in the temporary arrays with the values in the queue after point 2
		for (int g = point2 + 1; g < gene && !queue1.empty(); g++) {
			temp1[g] = queue1.front();
			queue1.erase(queue1.begin());
			temp2[g] = queue2.front();
			queue2.erase(queue2.begin());
		}

		//DEBUG
		//print the queue - DEBUG
		cout << "\x1B[97m\n\tQueue (After)\033[0m\n";
		cout << "\tQueue 1: ";
		for (int i = 0; i < queue1.size(); i++) {
			cout << queue1[i] << " ";
		}
		cout << endl;
		cout << "\tQueue 2: ";
		for (int i = 0; i < queue2.size(); i++) {
			cout << queue2[i] << " ";
		}
		cout << endl;

		// fill the empty spaces in the temporary arrays with the values in the queue before point 2
		int g = 0;
		while (g <= point2 && !queue1.empty() && !queue2.empty()) {
			for (int i = 0; i < gene; i++) {
				if (temp1[i] == -1) {
					temp1[i] = queue1.front();
					queue1.erase(queue1.begin());
					break;
				}
			}
			for (int i = 0; i < gene; i++) {
				if (temp2[i] == -1) {
					temp2[i] = queue2.front();
					queue2.erase(queue2.begin());
					break;
				}
			}
			g++;
		}

		//clear the queue vectors
		queue1.clear();
		queue2.clear();

		// print the temporary arrays - DEBUG
		cout << "\x1B[97m\n\tTemporary Arrays\033[0m\n";
		cout << "\tTemporary 1: ";
		for (int i = 0; i < gene; i++) {
			cout << temp1[i] << " ";
		}
		cout << endl;
		cout << "\tTemporary 2: ";
		for (int i = 0; i < gene; i++) {
			cout << temp2[i] << " ";
		}
		cout << endl;

		//copy the temporary arrays to the children chromosomes
		for (int g = 0; g < gene; g++) {
			children[0][g] = temp1[g];
			children[1][g] = temp2[g];
		}

		//print the children chromosomes - DEBUG
		cout << "\x1B[97m\n\tChildren Chromosomes\033[0m\n";
		//for each child (c) in the children data structure
		for (int c = 0; c < 2; c++) {
			cout << "\tChild " << c + 1 << ": ";
			//for each gene (g) in the chromosome
			for (int g = 0; g < gene; g++) {
				cout << children[c][g] << " ";
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

void mutation() { //Random Resetting Mutation

	cout << "\n\x1B[93mMutation\033[0m\n\n"; //DEBUG

	//for each child (c) in the children data structure
	for (int c = 0; c < 2; c++) {

		//generate a random number between 0 and 1 to determine if mutation will occur
		double random = (rand() % 11) / 10.0;
		
		//if the random number is less than the mutation probability, mutation will occur
		if (random < mutationProb) {

			
			int selectedGene = (rand() % gene); //select a random gene to mutate
			int mutatedGene; //initialize mutated gene

			do {

				mutatedGene = (rand() % areaCount) + 1; //generate a random area number between 1 and 40

			} while (find(children[c], children[c] + gene, mutatedGene) != children[c] + gene); //check if the area is already in the chromosome

			//print the selected gene and the mutated gene - DEBUG
			cout << "\tMutating Gene " << selectedGene + 1 << " of child " << c + 1 << " from " << children[c][selectedGene] << " to " << mutatedGene << endl; //Debug

			//mutate the selected gene
			children[c][selectedGene] = mutatedGene;
		}
		else {

			//print that mutation did not occur - DEBUG
			cout << "\tMutation did not occur for child " << c + 1 << endl;
		}	
	} 
	
	//print the children chromosomes - DEBUG
	cout << "\x1B[97m\n\tChildren Chromosomes\033[0m\n";

	//for each child (c) in the children data structure
	for (int c = 0; c < 2; c++) {
		cout << "\tChild " << c + 1 << ": ";
		//for each gene (g) in the chromosome
		for (int g = 0; g < gene; g++) {
			cout << children[c][g] << " ";
		}
		cout << endl;
	}

	//copy the children chromosomes to the temporary chromosome data structure
	//for each child (c) in the children data structure
	for (int c = 0; c < 2; c++) {
		//for each gene (g) in the chromosome
		for (int g = 0; g < gene; g++) {
			tempChromosome[countNewChromo][g] = children[c][g];
		}
		countNewChromo++;
	}
}

void copyChromosome() {

	//copy the initial population chromosomes to the temporary chromosome data structure
	//for each chromosome (c) in the population
	for (int c = 0; c < population; c++) {
		//for each gene (g) in the chromosome
		for (int g = 0; g < gene; g++) {
			tempChromosome[countNewChromo][g] = chromosome[c][g];
		}
		countNewChromo++;
	}
}
 
void survivalSelection() { //Rank-based Selection

	cout << "\n\x1B[93mSurvivor Selection\033[0m\n\n"; //DEBUG

	//declare required variables
	int rankIndices[population * 2]; //rank indices array
	float selectionProbabilities[population * 2]; //selection probabilities array
	float tempFitness[population * 2]; //temporary fitness array
	float random; //random number
	int selectedChromosomes[population][gene]; //selected chromosomes array

	//evaluate the fitness of the temporary chromosomes
	//for each chromosome (c) in the temporary chromosome data structure
	for (int c = 0; c < population * 2; c++) {

		//initialize total values
		int damageTotal = 0;
		float highRiskTotal = 0;
		int reliefCostTotal = 0;

		//for each gene (g) in the chromosome
		for (int g = 0; g < gene; g++) {

			int areaIndex = tempChromosome[c][g] - 1; // Convert area number to array index

			// Accumulate damage severity, high risk, and relief cost
			damageTotal += damageSeverity[areaIndex];
			highRiskTotal += highRiskRatio[areaIndex];
			reliefCostTotal += reliefCost[areaIndex];
		}

		//calculate fitness value    
		if (reliefCostTotal >= budget) { //if relief cost exceeds budget, fitness value will be halved
			tempFitness[c] = ((damageWeight * damageTotal / damageSum) + (highRiskWeight * highRiskTotal / highRiskSum) + (reliefCostWeight * reliefCostTotal / reliefCostSum)) * 0.5;
		}
		else {
			tempFitness[c] = (damageWeight * damageTotal / damageSum) + (highRiskWeight * highRiskTotal / highRiskSum) + (reliefCostWeight * reliefCostTotal / reliefCostSum);
		}
	}

	//rank the chromosomes based on their fitness
	iota(rankIndices, rankIndices + population * 2, 0);

	//sort the rank indices array based on the fitness values
	sort(rankIndices, rankIndices + population * 2, [&](int a, int b) { return tempFitness[a] > tempFitness[b]; });

	//print the tempchromosome - DEBUG
	cout << "\x1B[97m\n\tTemporary Chromosome\033[0m\n";
	for (int c = 0; c < population * 2; c++) {
		cout << "\tChromosome " << c + 1 << ": ";
		for (int g = 0; g < gene; g++) {
			cout << tempChromosome[c][g] << " ";
		}
		cout << endl;
	}

	//print the tempfitness - DEBUG
	cout << "\x1B[97m\n\tTemporary Fitness\033[0m\n";
	for (int c = 0; c < population * 2; c++) {
		cout << "\tChromosome " << c + 1 << ": ";
		cout << tempFitness[c] << endl;
	}

	//print the rank indices - DEBUG
	cout << "\x1B[97m\n\tRank Indices\033[0m\n";
	for (int c = 0; c < population * 2; c++) {
		cout << "\tChromosome " << rankIndices[c] + 1 << ": ";
		cout << tempFitness[rankIndices[c]] << endl;
	}

	//calculate the selection probabilities
	float sumOfRanks = (population * 2 * (population * 2 + 1)) / 2.0; //calculate the sum of ranks
	for (int c = 0; c < population * 2; c++) {
		selectionProbabilities[c] = (population * 2 - c) / sumOfRanks;
	}

	//print the selection probabilities - DEBUG
	cout << "\x1B[97m\n\tSelection Probabilities\033[0m\n";
	for (int c = 0; c < population * 2; c++) {
		cout << "\tChromosome " << rankIndices[c] + 1 << ": ";
		cout << selectionProbabilities[c] << endl;
	}

	//select the chromosomes based on the selection probabilities
	for (int i = 0; i < population; i++) {
		int selectedIndex = -1; //initialize selected index
		random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); //generate a random number between 0 and 1
		float cumulativeProbability = 0; //initialize cumulative probability

		for (int j = 0; j < population * 2; j++) {
			cumulativeProbability += selectionProbabilities[j]; //calculate the cumulative probability

			//if the random number is less than the cumulative probability, select the chromosome
			if (random <= cumulativeProbability) {
				selectedIndex = j; //assign the selected index
				break;
			}
		}

		//copy the selected chromosome to the selected chromosomes array
		if (selectedIndex != -1) {
			for (int g = 0; g < gene; g++) {
				selectedChromosomes[i][g] = tempChromosome[rankIndices[selectedIndex]][g];
			}
		}
	}

	//print the selected chromosomes - DEBUG
	cout << "\x1B[97m\n\tSelected Chromosomes\033[0m\n";
	for (int c = 0; c < population; c++) {
		cout << "\tChromosome " << c + 1 << ": ";
		for (int g = 0; g < gene; g++) {
			cout << selectedChromosomes[c][g] << " ";
		}
		cout << endl;
	}

	//copy the selected chromosomes to the chromosome data structure
	for (int c = 0; c < population; c++) {
		for (int g = 0; g < gene; g++) {
			chromosome[c][g] = selectedChromosomes[c][g];
		}
	}
}

void calculateAverageFitness() {

	double totalFitness = 0;

	//For each chromosome in the population
	for (int c = 0; c < population; c++) {
		//Accumulate the fitness into totalFitness
		totalFitness = totalFitness + fitness[c];
	}

	//Divide the totalFitness with population size
	avgFitness = totalFitness / population;

	//Printout the average to the screen - DEBUG
	cout << "\n\tAverage Fitness : " << avgFitness;

	//Print out the average to an output file that keep average fitness
	avgFitnessFile << avgFitness << endl;
}

void recordFitness() {

	//Declare the bestChromosome data structure
	static int bestchromosome[gene];

	//For each chromosome in the population
	for (int c = 0; c < population; c++) {

		// Check if the current chromosome's fitness is better than the best fitness found so far
		if (fitness[c] > bestFitness) {
			// Update the best fitness value
			bestFitness = fitness[c];
			// Copy the current chromosome's genes to the best Chromosome array
			for (int g = 0; g < gene; g++) {
				bestchromosome[g] = chromosome[c][g];
			}
		}
	}
	// Output the best fitness found so far
	cout << "\n\tBest Fitness So Far : " << bestFitness;
	// Write the best fitness to the best fitness file
	bestFitnessFile << bestFitness << endl;
	// Output the genes of the best chromosome
	cout << "\n\tBest Chromosome : ";
	for (int g = 0; g < gene; g++) {
		// Print each gene of the best chromosome
		cout << bestchromosome[g] << " ";

		// Write each gene of the best chromosome to the best chromosome
		bestChromosomeFile << bestchromosome[g] << " ";
	}
	cout << endl;
	bestChromosomeFile << endl;
}

int main() {
	srand(time(NULL)); //seed the random number generator

	//Open files to record details
	avgFitnessFile.open("avgFitnessFile.txt");
	bestChromosomeFile.open("bestchromosome.txt");
	bestFitnessFile.open("bestfitnessFiles.txt");

	initializePopulation();
	for (int gen = 0; gen < maxGeneration; gen++) {

		//Print the generation number - DEBUG
		cout << "\n\x1B[93mGeneration : " << gen + 1 << "\n\033[0m" << endl;
		printChromosome();
		evaluateChromosome();
		calculateAverageFitness();
		recordFitness();
		countNewChromo = 0;

		for (int i = 0; i < population / 2; i++) {

			//Print the iteration number - DEBUG
			cout << "\n\x1B[96mIteration : " << i + 1 << " Generation : " << gen + 1 << "\033[0m";
			parentSelection();
			crossover();
			mutation();
		}
		copyChromosome();
		survivalSelection();
	}

	//Close files after record details
	avgFitnessFile.close();
	bestChromosomeFile.close();
	bestFitnessFile.close();
	
	return 0;
}