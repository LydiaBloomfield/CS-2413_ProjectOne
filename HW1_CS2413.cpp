#include <iostream>
using namespace std;

class CRM {
protected:
	int n; //The number rows of the original matrix
	int m; //The number of columns of the original matrix
	int nonZeros; //The number of non-zero elements in the original matrix
	int* values; //Array that contains all non-zero values of the matrix, assuming they are all integers
	int valueIterator = 0; // counter used in addValues()
	int* rowPos; //Array that for each row of the original matrix contains the position in the values matrix in which the first non-zero element of this row is stored.
	int rowIterator = 0; // Counter used in add(Rows)
	int* colPos; //Array that contains the column number for each one of the non-zero values in the original matrix.
	int colIterator = 0; // Counter used in addCol()
	int rowCur = 0; // Used in addRow()
	int rowPrev = -1; // Used in addRow()
public:
	CRM(); //default or empty constructor
	CRM(int rows, int cols, int numNonZeros); // constructor
	int getNumRows(); //returns number of rows in original matrix
	void addValue(int value); //add a new value in the values array
	void addRow(int row); //add a row number in the rowPos array
	void addColumn(int col);//add a column number in the colPos array
	void display(); //print the contents of the three arrays. Each array must be on a different line (separated by a 
					//new line character) and you have exactly a single space between each value printed.
	int mostInfluentialUser(); //find the most influential user
	int mostActiveUser(); //find the most active user
	int* influentiality(); // array of each users influentiality (unranked)
	int* activity(); // array of each users activity (unranked)
	int* influentialUsers(); //compute vector of users ranked by how influential they are
	int* activeUsers(); //compute vector of users ranked by how active they are
	~CRM(); //destructor	
};

//some of the methods – Katia Papakonstantinopoulou
CRM::CRM() {
	n = 0;
	m = 0;
	values = NULL;
	rowPos = NULL;
	colPos = NULL;
}

CRM::CRM(int rows, int cols, int numNonZeros) {
	n = rows;
	m = cols;
	nonZeros = numNonZeros;
	values = new int[nonZeros];
	rowPos = new int[n];
	colPos = new int[nonZeros];
}

int CRM::getNumRows() {
	return n;
}

void CRM::addValue(int value){
	// add value to value array
	values[valueIterator++] = value;
}

void CRM::addRow(int row){
	rowCur = row;

	// if we encounter a user who has not retweeted anyone, put -1 in rowPos 
	while (rowCur > rowPrev + 1) {
		rowPos[rowIterator++] = -1;
		rowPrev++;
	}

	// if the current row is equal to previous row plus one, place the index of the values array in the rowPos array
	if (rowCur == (rowPrev + 1)) {
		rowPos[rowIterator++] = valueIterator - 1;
	}

	// if you reach the end of the values array, and there is still space in the rowPos array,
	// add in -1 to the array until the row pos array reaches its end
	if (valueIterator == nonZeros) {
		while (rowIterator < n) {
			rowPos[rowIterator++] = -1;
		}
	}
	rowPrev = rowCur;
}

void CRM::addColumn(int col){
	// add column position
	colPos[colIterator++] = col;
}

void CRM::display(){
	// display arrays
	cout << "values: ";
	for (int i = 0; i < nonZeros; i++) {
		cout << values[i] << " ";
	}
	cout << "\nrowPos: ";
	for (int i = 0; i < n; i++) {
		cout << rowPos[i] << " ";
	}
	cout << "\ncolPos: ";
	for (int i = 0; i < nonZeros; i++) {
		cout << colPos[i] << " ";
	}
}

int CRM::mostInfluentialUser(){
	// return the first position of the mostInfluentialUsers array
	return influentialUsers()[0];
}

int CRM::mostActiveUser(){
	// return the first position of the mostActiveUsers array
	return activeUsers()[0];
}

// helper method to calculate influentiality of each user
int* CRM::influentiality(){
	int* outputVector = new int[n];
	for (int i = 0; i < n; i++) outputVector[i] = 0;

	// iterate throught the output vector with index i
	for (int i = 0; i < n; i++) {
		int total = 0;
		
		// iterate through the colPos array with index j
		for (int j = 0; j < nonZeros; j++) {
			// if the value in the colPos array is the same as the id of the current user, add it to the user's total
			if (colPos[j] == i) {
				total += values[j];
			}
		}
		// set the current user's total
		outputVector[i] = total;
	}
	return outputVector;
}

// helper method to calculate activity of each user
int* CRM::activity(){
	int* outputVector = new int[n];
	for (int i = 0; i < n; i++) outputVector[i] = 0;
	
	int indexOfValues = 0;

	// iterate through the output vector with index i
	for (int i = 0; i < n; i++) {
		int total = 0;

		// if the next value in the rowPos array is -1
		if (rowPos[i + 1] == -1) {
			// then add all values until the next value that does not equal -1
			while (indexOfValues < rowPos[i + 2]) {
				total += values[indexOfValues];
				indexOfValues++;
			}
			// the activity total of the current user is total, and the activity total of the next user is 0
			outputVector[i + 1] = 0;
			outputVector[i] = total;
			i++;
		}

		// if we are at the end of the array
		else if (i == n - 1) {
			// then add up all the remaining values in the values array
			while (indexOfValues < nonZeros) {
				total += values[indexOfValues];
				indexOfValues++;
			}
			outputVector[i] = total;
		}
		
		else {
			// add up all the values in the values array until the index is equal to the next value in the rowPos array
			while (indexOfValues < rowPos[i + 1]) {
				total += values[indexOfValues];
				indexOfValues++;
			}
			outputVector[i] = total;
		}
	}
	return outputVector;
}

int* CRM::influentialUsers() {
	int* copy = influentiality();
	int* outputVector = new int[n];
	for (int i = 0; i < n; i++) outputVector[i] = i;

	int ciTemp, i, j, temp;

	// selection sort
	// as we sort the array from influentiality(), we simultaneously sort an array of user ids (outputVector)
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
	return outputVector;
}

int* CRM::activeUsers()
{
	int* copy = activity();
	int* outputVector = new int[n];
	for (int i = 0; i < n; i++) outputVector[i] = i;

	int ciTemp, i, j, temp;

	// selection sort
	// as we sort the array from activity(), we simultaneously sort an array of user ids (outputVector)
	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {

			// if total activity of the current user is less than the next, switch their order
			if (copy[i] < copy[j]) {
				temp = copy[i];
				ciTemp = outputVector[i];

				copy[i] = copy[j];
				outputVector[i] = outputVector[j];

				copy[j] = temp;
				outputVector[j] = ciTemp;
			}

			//if the total activity of the current and next users are equal, 
			if (copy[i] == copy[j]) {
				// then the earlier position is given to the user with the lower id
				if (outputVector[j] < outputVector[i]) {
					ciTemp = outputVector[i];
					outputVector[i] = outputVector[j];
					outputVector[j] = ciTemp;
				}
				else if (outputVector[i] > outputVector[j]) {
					ciTemp = outputVector[j];
					outputVector[j] = outputVector[i];
					outputVector[i] = ciTemp;
				}
			}
		}
	}
	return outputVector;
}

CRM::~CRM() {
	if (values != NULL) delete[] values;
	if (rowPos != NULL) delete[] rowPos;
	if (colPos != NULL) delete[] colPos;
	cout << "CRM Object Destroyed!!" << endl;
	n = 0;
	m = 0;
	nonZeros = 0;
}

int main() {

	CRM* A;
	int numRows, numColumns, numNonZeros;
	int row, col, value;

	//read in the first matrix
	cin >> numRows >> numColumns;
	cin >> numNonZeros;

	A = new CRM(numRows, numColumns, numNonZeros);

	for (int i = 0; i < numNonZeros; i++) {
		// read in row of information and add to arrays
		cin >> row >> col >> value;
		(*A).addValue(value);
		(*A).addRow(row);
		(*A).addColumn(col);
	}

	(*A).display();

	cout << endl;

	//Find most influential user
	int mostInf = (*A).mostInfluentialUser();
	cout << "Most influential user: " << mostInf << endl;
	cout << endl;

	//Find most active user
	int mostAct = (*A).mostActiveUser();
	cout << "Most active user: " << mostAct << endl;
	cout << endl;

	// Rank users based on how influential they are
	int* influentialityVector = (*A).influentialUsers();
	cout << "Users ranked by Influentiality: ";
	for (int i = 0; i < (*A).getNumRows(); i++)
		cout << influentialityVector[i] << " ";
	cout << endl << endl;

	//Rank users based on how active they are
	int* activityVector = (*A).activeUsers();
	cout << "Users ranked by Activity: ";
	for (int i = 0; i < (*A).getNumRows(); i++)
		cout << activityVector[i] << " ";
	cout << endl;

	// call the destructors
	delete A;
	delete[] influentialityVector;
	delete[] activityVector;

	return 0;
}