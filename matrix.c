#include "matrix.h"
#include "set.h"
#include <stdlib.h>

/**
 * This function initializes and returns a dense matrix 
 * as a dynamic two	-dimensional array.
 */
DenseMatrix initDenseMatrix(int columnLength, int rowLength) {
	//TODO: Fill this function.

	DenseMatrix denseMatrix = (DenseMatrix)malloc(sizeof(DenseMatrix_t));



	denseMatrix -> columnLength = columnLength;
	denseMatrix -> rowLength = rowLength;
	denseMatrix -> matrix = (int**)malloc(rowLength * sizeof(int*));
	


	// we will allocate for each matrix rows and colums
	for(int i = 0; i < denseMatrix->rowLength; i++){
		denseMatrix -> matrix[i] = (int*)malloc(columnLength * sizeof(int));		
	}

	return denseMatrix;

}

/**
 * This function deallocates all the rows and 
 * columns of the given dense matrix.
 */
void freeDenseMatrix(DenseMatrix denseMatrix) {
	//TODO: Fill this function.
	for(int r = 0; r<denseMatrix->rowLength ; r++){
			free(denseMatrix -> matrix[r]);
	}

	free(denseMatrix -> matrix);
	free(denseMatrix);

}

/**
 * This function creates a dense matrix and fills 
 * the elements in the given parse matrix as a set.
 */
DenseMatrix parseMatrixToDenseMatrix(Set parseMatrix, int columnLength, int rowLength) {
	//TODO: Fill this function.
	DenseMatrix denseMatrix = initDenseMatrix( columnLength,  rowLength);
	if(denseMatrix == NULL){
		return NULL;
	}
	
	
	for(int i = 0; i < rowLength; i++){
		for (int j=0; j < columnLength; j++) {
		
		denseMatrix -> matrix[i][j] = 0;

		}
	}



	for (int i= 0; i< parseMatrix -> cardinality ; i++) {
		Element tempElement = parseMatrix -> elements[i];
		
		if(tempElement == NULL || tempElement -> data == NULL){
			continue;
		}


		int* tempElementData = (int*)tempElement -> data;
		
		int row = tempElementData[0];
		int column = tempElementData[1];
		int value = tempElementData[2];

		
		// we need to check "if parse matrix's element's x or y values greather than columnlength or rowlength"	
		if(row <= rowLength && column <= columnLength){
			
		denseMatrix -> matrix[row][column] =  value;
		
		}



	}

	return denseMatrix;


}

/**
 * This function creates and returns a parse matrix 
 * as a set depending on the given dense matrix's elements.
 */
Set denseMatrixToParseMatrix(DenseMatrix denseMatrix) {
	//TODO: Fill this function.



	Set parseMatrix = initSet();


	for(int i = 0; i < denseMatrix -> rowLength; i++){
		for (int j=0; j < denseMatrix -> columnLength; j++) {
			int value = denseMatrix -> matrix[i][j];

			if(value != 0){
				Element tempElement = createMatrixPointElement(i, j, value);
				if(tempElement != 0){
					insertElement(parseMatrix, tempElement);
				}
			}
		
		}
		
	}


	return parseMatrix;

}

/**
 * This matrix creates a new dense matrix, and the 
 * matrix is the addition of the given two matrices.
 */
DenseMatrix addDenseMatrices(DenseMatrix dm1, DenseMatrix dm2) {
	//TODO: Fill this function.


	// first we will check these matrces's size are equal
	if(!(dm1 -> columnLength == dm2 -> columnLength || dm1 -> rowLength == dm2 -> rowLength)){
		return NULL; // mission failed! because these are not equal
	}

	int rowLength = dm1 -> rowLength;
	int columnLength = dm1 -> columnLength;

	DenseMatrix newDenseMatrix = initDenseMatrix(columnLength, rowLength);

	for(int r = 0; r<rowLength; r++){
		for (int c = 0; c<columnLength; c++) {
			int dm1Value = dm1 -> matrix[r][c];
			int dm2Value = dm2 -> matrix[r][c];

			int newValue = dm2Value + dm1Value;

			newDenseMatrix -> matrix[r][c] = newValue;
		}
	}

	return newDenseMatrix;

}
/**
 * This matrix creates a new sparse matrix as a set, 
 * and the matrix is the addition of the given 
 * two sparse matrices.
 */
Set addSparseMatrices(Set sm1, Set sm2, int columnLength, int rowLength) {
	//TODO: Fill this function.
	

	
	// we need to add two parse matrices to new parsematrix but it can be hard we have no prepared function
	// because of that we can transform dens matrix to our parse matrices and than we can transform parse matrix

	DenseMatrix firstDenseMatrix = parseMatrixToDenseMatrix(sm1, columnLength, rowLength);
	DenseMatrix secondDenseMatrix = parseMatrixToDenseMatrix(sm2, columnLength, rowLength);


	DenseMatrix sumOfDenseMatrices = addDenseMatrices(firstDenseMatrix, secondDenseMatrix);

	

	Set newSparseMatrix = denseMatrixToParseMatrix(sumOfDenseMatrices);	

	freeDenseMatrix(firstDenseMatrix);
	freeDenseMatrix(secondDenseMatrix);
	freeDenseMatrix(sumOfDenseMatrices);


	return newSparseMatrix;


}
