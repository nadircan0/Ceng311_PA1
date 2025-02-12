#include "set.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * This function initializes a set without an element and returns it.
 */
Set initSet() {
	//TODO: Fill this function.
	Set newSet = (Set)malloc(sizeof(Set_t));
	if(newSet == NULL){
		return  NULL;
	}


	newSet->elements = NULL; // whithout an element
	newSet->cardinality = 0;

	return newSet;
	}

/**
 * This function creates an integer-typed element and returns it. 
 * It allocates memory for both the element and the integer.
 */
Element createIntegerElement(int data) {
	//TODO: Fill this function.
	Element newElement = (Element)malloc(sizeof(Element_t));


	newElement -> type = INTEGER;

	newElement -> data = malloc(sizeof(data));


	*((int*)newElement->data) = data;
	
	return newElement;
}

/**
 * This function creates an float-typed element and returns it. 
 * It allocates memory for both the element and the float.
 */
Element createFloatElement(float data) {
	//TODO: Fill this function.
	Element newElement = (Element)malloc(sizeof(Element_t));


	newElement -> type = FLOAT;

	newElement -> data = malloc(sizeof(data));


	*((float*)newElement->data) = data;
	return newElement;
}

/**
 * This function creates an string-typed element and returns it. 
 * It allocates memory for both the element and the string.
 * This function uses the `strlen` and the `strcpy` functions.
 */
Element createStringElement(char* data) {
	//TODO: Fill this function.
	Element newElement = (Element)malloc(sizeof(Element_t));


	newElement -> type = STRING;

	newElement -> data = malloc(strlen(data)+1);


	strcpy(((char*)newElement->data), data);
	return newElement;
	
}

/**
 * This function creates an matrix-point-typed element and returns it.
 * It allocates memory for both the element and the matrix point.
 */
Element createMatrixPointElement(int x, int y, int data) {
	//TODO: Fill this function.
	// I choose x is a row y is a column
	Element newElement = (Element)malloc(sizeof(Element_t));
	if(newElement == NULL){
		return NULL;
	}


	newElement -> type = MATRIX_POINT;

	// firstly we should create a matrix for assign to (newElement->data)

	int* oneDmatrix = (int*)malloc( 3 * sizeof(int));

	if(oneDmatrix == NULL){
		free(newElement);
		return NULL;
	}
	



	oneDmatrix[0]=x;
	oneDmatrix[1]=y;
	oneDmatrix[2]=data;
	
	newElement -> data = oneDmatrix;
	return(newElement);
	
}

/**
 * This function compares two elements to see if they are identical. 
 * First, it checks if the types are identical. If they are the same, 
 * it checks if the data are the same. However, this function does not 
 * compare addresses because the value from two different addresses 
 * can be the same. It returns one if the elements are the same; 
 * otherwise, it returns zero. This function uses the `strcmp` function.
 */
int isSame(Element e1, Element e2) {
	//TODO: Fill this function.

	if(e1 -> type != e2 -> type){
		return 0;
	}

	//for matrix comparision
	int* e1_data = (int*)e1 -> data;
	int* e2_data = (int*)e2 -> data;
	

	// we should check all data types that given
	switch (e1 -> type) {
		case INTEGER:
			return *((int*)e1 -> data) == *((int*)e2 -> data);
		case FLOAT:
			return *((float*)e1 -> data) == *((float*)e2 -> data);
		case STRING:
			return (strcmp((char*)e1 -> data, (char*)e2 -> data) == 0);	
		case MATRIX_POINT:
			return (e1_data[0]==e2_data[0]) && (e1_data[1]==e2_data[1]) && (e1_data[2]==e2_data[2]); 
		default:
			return 0; // we can detect unknown type
	}

	
}

/**
 * This function checks if the given element is in the set or not. 
 * It does not compare the addresses. It returns one if the given 
 * element is in the set; otherwise, it returns zero.
 */
int in(Set set, Element element) {
	//TODO: Fill this function.

	//first we should check set and element null?


	// we can use isSame function that create before this func, we know if two elements equal it returns 1 else 0
	for(int i = 0; i < set -> cardinality; i++){
		if(isSame(set -> elements[i], element )){
			return 1;
		}
	}
	
	return 0;
}

/**
 * This function inserts an element into the given set. 
 * The same element cannot be twice in the same set.
 * It returns one if the inserting element is successful; 
 * otherwise, it returns zero. This function uses the 
 * `realloc` function.
 */
int insertElement(Set set, Element element) {
	//TODO: Fill this function.
	 
	//first we check the element is in the set or not
	if(in(set, element) == 1){
		return 0;
	}

	//than we will insert the element to the set, with reallocate address
	// when we use realloc first we should vreate temp Element and then if it's not equal null we can assign original set. we can directly use realloc func for set but maybe error can occur and all original data will be zero.
	Element* temp_Elements = (Element*)realloc(set -> elements, (set -> cardinality +1) * sizeof(Element));
	if(temp_Elements == NULL){
		return 0;
	}

	set -> elements = temp_Elements;
	set -> elements[set -> cardinality ] = element;
	set -> cardinality++;
	return 1; // mission success!


}

/**
 * This function removes the given element in the set. 
 * It returns one if removal is successful; otherwise, 
 * it returns zero. This function uses the `realloc` function.
 */
int removeElement(Set set, Element element) {
	//TODO: Fill this function.

	// first we should check element is in the set, we can use in func
	if(in(set, element) == 0){
		return 0; // failed
	}
	int index = -1;

	for(int i = 0; i < set -> cardinality; i++){
		if(isSame(set -> elements[i], element )){
			index = i;
			break;
		}
	}

	free(set -> elements[index] -> data);
 	free(set -> elements[index]);

	// for avoid holes we should shift elements
	for(int i = index; i < (set -> cardinality -1); i++){
		set -> elements[i] = set -> elements[i+1];
	}

	// last cardinality now Null we will decrease it
	set -> cardinality -- ;

	Element* temp_elements = (Element*)realloc(set->elements, set -> cardinality * (sizeof(Element)));
	if(temp_elements != NULL || set -> cardinality == 0){
		set -> elements = temp_elements;
	}


	return 1;

	}


/**
 * This function creates and returns a new set, which is united of the given sets.
 */
Set unite(Set s1, Set s2) {
	//TODO: Fill this function.

	// first we will create empty set
	Set newSet = initSet();



	for(int i = 0; i < s1 -> cardinality; i++){
		insertElement( newSet, s1 -> elements[i]);
	}	

	for(int i = 0; i < s2 -> cardinality; i++){
		insertElement( newSet, s2 -> elements[i]);
	}	

	return newSet;
}

/**
 * This function creates and returns a new set, which is intersected of the given sets.
 */
Set intersect(Set s1, Set s2) {
	//TODO: Fill this function.
	// first we will create empty set
	Set newSet = initSet();

	if(s1 == NULL && s2 == NULL ){
		return newSet;
	}

	if (s1 -> cardinality < s2 -> cardinality) {
		for(int i = 0; i < s1 -> cardinality; i++){
			if(in(s2 , s1 -> elements[i])){
				insertElement(newSet, s1 -> elements[i] );
			}
		}
	}else {
		for(int i = 0; i < s2 -> cardinality; i++){
			if(in(s1 , s2 -> elements[i])){
				insertElement(newSet, s2 -> elements[i] );
			}
		}
	}

	return newSet;
}

/**

 * This function creates and returns a new set, which is 
 * subtracted from the first given set by the second one.
 */
Set substract(Set s1, Set s2) {
	//TODO: Fill this function.

	Set newSet = initSet();
	

	if(s1 == NULL){
		return newSet;

	}else if (s2 == NULL) {
		for(int i = 0; i < s1 -> cardinality; i++){
			insertElement(newSet, s1 -> elements[i]);
		}
		return newSet;
	}

	for(int i = 0; i < s1 -> cardinality; i++){
		if(!in(s2, s1 -> elements[i])){
			insertElement(newSet, s1 -> elements[i]);
		}
	}

	return newSet;


}

/**
 * This function prints the given element depending on its type.
 */
void printElement(Element element) {
	//TODO: Fill this function.


	int* eMatrixData = (int*)element -> data;

	switch (element -> type) {
		case INTEGER:
			printf("This is an Integer: %d\n", *((int*)element -> data)); 
			break;
		case STRING:
			printf("This is a String: %s\n", (char*)element -> data);
			break;
		case FLOAT:
			printf("This is a Float: %f\n", *((float*)element -> data));
			break;
		case MATRIX_POINT:
			printf("This is a Matrix Point's elements row, column and value: %d, %d, %d \n", eMatrixData[0],eMatrixData[1],eMatrixData[2]);
			break;
		default:
			printf("Null or unknown type \n");	
			break;
	}
	
}

/**
 * This function prints the given set element by element depending on its type.
 */
void printSet(Set set) {
	//TODO: Fill this function.
	for (int i = 0; i < set -> cardinality; i++) {
		printElement(set -> elements[i]);
	}

}




/**
 * This element deallocates all data in the given set and the set itself.
 */
void freeSet(Set set) {
	//TODO: Fill this function.
	if(set != NULL){
		for (int i = 0; i < set ->cardinality; i++) {
		if(set -> elements[i] != NULL){
			free(set -> elements[i] -> data);
			free(set -> elements[i]);
			}
		
		}
	}
	
	free(set->elements);
	free(set);
}
