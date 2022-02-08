#include <iostream>
#include "template1.h"
using namespace std;

class CRM {
	protected:
		int n; //The number rows of the original matrix
		int m; //The number of columns of the original matrix
		int nonZeros; //The number of non-zero elements in the original matrix
		int* values; //Array that contains all non-zero values of the matrix, assuming they are all integers
		int valueIterator = 0;
		int* rowPos; //Array that for each row of the original matrix contains the position in the values matrix in which the first non-zero element of this row is stored.
		int rowIterator = 0;
		int* colPos; //Array that contains the column number for each one of the non-zero values in the original matrix.
		int colIterator = 0;
					 //There may be others you may need
		int rowCur = 0;
		int rowPrev = -1;
	public:
		CRM ( ); //default or empty constructor
		CRM (int rows, int cols, int numNonZeros);
		int getNumRows ( );
		int getNumNonZeros();
		void addValue (int value); //add a new value in the values array
		void addRow (int row); //add a row number in the rowPos array
		void addColumn (int col);//add a column number in the colPos array
		void display (); //print the contents of the three arrays. Each array must be on a different line (separated by a 
                        //new line character) and you have exactly a single space between each value printed.
		int mostInfluentialUser(); //find the most influential user
		int mostActiveUser(); //find the most active user
		int* influentiality(); // array of each users influentiality (unranked)
		int* activity(); // array of each users activity (unranked)
		int* influentialUsers (); //compute vector of users ranked by how much influential they are
		int* activeUsers (); //compute vector of users ranked by how much active they are
		~CRM(); //destructor
		//You may define and use any other method you need		
};
//some of the methods – Katia Papakonstantinopoulou
CRM::CRM ( ) {
	n = 0;
	m = 0;
	values = NULL;
	rowPos = NULL;
	colPos = NULL;
}

CRM::CRM (int rows, int cols, int numNonZeros) {
	n = rows;
	m = cols;
	nonZeros = numNonZeros;
	values = new int [nonZeros];
	rowPos = new int [n];
	colPos = new int [nonZeros];
}


int CRM::getNumRows()
{
	return n;
}

int CRM::getNumNonZeros()
{
	return nonZeros;
}

void CRM::addValue(int value)
{
	values[valueIterator] = value;
	valueIterator++;
}

void CRM::addRow(int row)
{
	rowCur = row;

	// if we encounter a user with no retweets, put -1 in rowPos 
	// condense w [rowIterator++]
	while (rowCur > rowPrev + 1) {
		rowPos[rowIterator] = -1;
		rowIterator++;
		rowPrev++;
	}

	// if the current row is equal to previous row plus one, place the value found in the values array in the rowPos array
	if (rowCur == (rowPrev + 1)) {
		rowPos[rowIterator] = valueIterator - 1;
		rowIterator++ ;
	}

	// if you've gotten to the end of the values array, and there are a bunch of users with no retweets at the end,
	//add in -1 to the end of the rowPos array until the row pos array reaches its end
	if (valueIterator == nonZeros) {

		while (rowIterator < n) {
			rowPos[rowIterator] = -1;
			rowIterator++;
		}
	}
	rowPrev = rowCur;
	
}

void CRM::addColumn(int col)
{
	colPos[colIterator] = col;
	colIterator++;
}

void CRM::display()
{
	cout << "values: ";
	for (int i = 0; i < nonZeros; i++) {
		cout << values[i] << " ";
	}
	cout << "\n rowPos: ";
	for (int i = 0; i < n; i++) {
		cout << rowPos[i] << " ";
	}
	cout << "\n colPos: ";
	for (int i = 0; i < nonZeros; i++) {
		cout << colPos[i] << " ";
	}
}

int CRM::mostInfluentialUser(){
	int* copy = influentiality();
	int max = 0;
	int maxIndex = 0;
	
	for (int i = 0; i < n; i++) {
		if (copy[i] > max) {
			max = copy[i];
			maxIndex = i;
		}
	}
	return maxIndex;
}

int CRM::mostActiveUser()
{
	int* copy = activity();
	int max = 0;
	int maxIndex = 0;

	for (int i = 0; i < n; i++) {
		if (copy[i] > max) {
			max = copy[i];
			maxIndex = i;
		}
	}
	return maxIndex;
}

// Helper method
int* CRM::influentiality()
{
	int* outputVector = new int[n];
	for (int i = 0; i < n; i++) outputVector[i] = 0;

	// index of the output vector
	for (int i = 0; i < n; i++) {
		// iterate through the colPos array
		int total = 0;
		for (int j = 0; j < nonZeros; j++) {
			// if the value in the colPos array is the same as the column of the current user, add it to the user's total
			if (colPos[j] == i) {
				total += values[j];
			}
		}
		outputVector[i] = total;
	}
	return outputVector;
}

int* CRM::activity()
{
	int* outputVector = new int[n];
	for (int i = 0; i < n; i++) outputVector[i] = 0;

	int indexOfValues = 0;

	// index of output vector
	for (int i = 0; i < n; i++) {
		int total = 0;

		//Iterate through values array
		do{
			total += values[indexOfValues];
			indexOfValues++;
		} while (indexOfValues < rowPos[i + 1]);
		
		outputVector[i] = total;
	}
	/*for (int k = 0; k < n; k++) {
		cout << outputVector[k] << " ";
	}*/
	return outputVector;
}
;

int* CRM::influentialUsers() {
	int* copy = influentiality();
	int* outputVector = new int[n];
	for (int i = 0; i < n; i++) outputVector[i] = i;

	int ciTemp;
	int i, j, temp;
	
	// selection sort
	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {
			if (copy[i] < copy[j]) {
				temp = copy[i];
				ciTemp = outputVector[i];

				copy[i] = copy[j];
				outputVector[i] = outputVector[j];

				copy[j] = temp;
				outputVector[j] = ciTemp;	
			}
		}
	}

	/*for (int i = 0; i < n; i++) {
		cout << copy[i] << " ";
	}*/

	return outputVector;
}

int* CRM::activeUsers()
{
	int* copy = activity();
	int* outputVector = new int[n];
	for (int i = 0; i < n; i++) outputVector[i] = i;

	int ciTemp;
	int i, j, temp;

	// selection sort
	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {
			if (copy[i] < copy[j]) {
				temp = copy[i];
				ciTemp = outputVector[i];

				copy[i] = copy[j];
				outputVector[i] = outputVector[j];

				copy[j] = temp;
				outputVector[j] = ciTemp;
			}
		}
	}

	/*for (int i = 0; i < n; i++) {
		cout << copy[i] << " ";
	}*/
	return outputVector;
}

CRM::~CRM ( ) {
	if (values != NULL) delete [ ] values;
	if (rowPos != NULL) delete [ ] rowPos;
	if (colPos != NULL) delete [ ] colPos;
	cout << "CRM Object Destroyed!!" << endl;
	n = 0;
	m = 0;
	nonZeros = 0;
}

int main ( ) {

   CRM* A;
   int numRows, numColumns, numNonZeros;
   int row, col, value;

   //read in the first matrix
   cin >> numRows >> numColumns;
   cin >> numNonZeros;

   A = new CRM (numRows, numColumns, numNonZeros);

   // put in numNonZeros as number of iterations of this loop
   for (int i=0; i < (*A).getNumNonZeros(); i++) {
	   // read in row of information
	cin >> row >> col >> value;
	(*A).addValue (value);
	(*A).addRow (row);//needs to be done cleverly in the method
	(*A).addColumn (col);
   }

   (*A).display ( );

//Find most influential user
	int mostInf = (*A).mostInfluentialUser ();
	cout << "Most influential user: " << mostInf << endl;
	cout << endl;

//Find most active user

	(*A).activity();
	cout << endl;

	int mostAct = (*A).mostActiveUser();
	cout << "Most active user: " << mostAct << endl;
	cout << endl;


// Rank users based on how much influential they are
    int* influentialityVector = (*A).influentialUsers ();
	cout << "Users ranked by Influentiality: " << endl;
    for (int i=0; i < (*A).getNumRows ( ); i++) 
		cout << influentialityVector [i] << " ";
    cout << endl << endl;

//Rank users based on how much active they are
//fill-in code
	int* activityVector = (*A).activeUsers();
	cout << "Users ranked by Activity: " << endl;
    for (int i=0; i < (*A).getNumRows ( ); i++) 
		cout << activityVector[i] << " ";
    cout << endl;
//
//// Call the destructors
//     delete A;
//     delete [ ] influentialVector;
//     delete ___; 

    return 0;
}
