// Copyright 2021 Aradoaie Ioana-Maria
#include <stdio.h>
#include <stdlib.h>
#define MODULO 10007

// a function for dynamic allocation of a matrix
int **alloc_matrix(int rows, int columns)
{
	int **matrix = malloc(rows * sizeof(int *));
	// checking if the memory has been allocated successfully
	if (!matrix)
		return NULL;
	for (int i = 0; i < rows; i++) {
		matrix[i] = malloc(columns * sizeof(int));
		if (!matrix[i]) {
			return NULL;
			free(matrix[i]);
			}
	}
	return matrix;
}

// function for dynamic allocation of three vectors that store
// different data: one containing the number of rows for each matrix,
// one for the number of columns for each matrix
// and one containing the given matrices
void allocate(int ****mat_array, int **rows, int **columns)
{
	*rows = malloc(sizeof(int));
	*columns = malloc(sizeof(int));
	*mat_array = malloc(sizeof(int **));
}

// function for reallocating the memory of the three previously allocated
// vectors; the "number" variable represents the number of matrices
void reallocate(int ****mat_array, int **rows, int **columns, int number)
{
	*rows = realloc(*rows, (number + 1) * sizeof(int));
	*columns = realloc(*columns, (number + 1) * sizeof(int));
	*mat_array = realloc(*mat_array, (number + 1) * sizeof(int **));
}

// function used for reading a matrix
void read_matrix(int rows, int columns, int **matrix)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			scanf("%d", &matrix[i][j]);
}

// function used for reading a matrix
void free_matrix(int rows, int **matrix)
{
	for (int i = 0; i < rows; i++)
		free(matrix[i]);
	free(matrix);
}

// function used for displaying a matrix
void print_matrix(int rows, int columns, int **matrix)
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
}

// function used for resizing a matrix according to given dimensions
void resize_matrix(int index, int ****mat_array, int **rows,
				   int **columns, int number)
{
	scanf("%d", &index);
	// checking if the given index exists
	if (index > number || index < 0) {
		printf("No matrix with the given index\n");
		int a, b;
		scanf("%d", &a);
		// the execution of the printf command is prevented
		// indefinitely if the given index does not exist
		for (int i = 0; i < a; i++)
			scanf("%d", &b);
		scanf("%d", &a);
		for (int i = 0; i < a; i++)
			scanf("%d", &b);
	} else {
		int no_r, no_c;
		scanf("%d", &no_r);
		// the new dimensions are read and the memory will be allocated
		// dynamically for two vectors that will contain the values ​​that
		// we have to place on the rows/columns of the new matrix
		int *new_rows = malloc(no_r * sizeof(int));
		for (int i = 0; i < no_r; i++)
			scanf("%d", &new_rows[i]);
		scanf("%d", &no_c);
		int *new_columns = malloc(no_c * sizeof(int));
		for (int i = 0; i < no_c; i++)
			scanf("%d", &new_columns[i]);
		int **mat = malloc(no_r * sizeof(int *));
		// checking to see if the memory was succesfully allocated
		if (!mat)
			exit(-1);
		else
			for (int i = 0; i < no_r; i++) {
				mat[i] = malloc(no_c * sizeof(int));
				if (!mat[i])
					free(mat[i]);
			}
		// the new matrix is ​​built according to the given rules
		for (int i = 0; i < no_r; i++)
			for (int j = 0; j < no_c; j++)
				mat[i][j] = (*mat_array)[index][new_rows[i]][new_columns[j]];
		free_matrix((*rows)[index], (*mat_array)[index]);
		(*mat_array)[index] = mat;
		// the values ​​in the vectors are updated
		// according to the dimensions of the new matrix
		(*rows)[index] = no_r;
		(*columns)[index] = no_c;
		// the allocated memory for rows and columns vectors is freed
		free(new_rows);
		free(new_columns);
	}
}

// swap function for two pointers using xor
void swap_p(int *a, int *b)
{
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *a ^ *b;
}

// function for constructing the transpose of a given matrix
int **transpose_matrix(int ****mat_array, int **rows, int **columns, int index)
{
	int **transpose;
	// memory is allocated for the transpose by
	// inverting the rows and columns between them
	transpose = alloc_matrix((*columns)[index], (*rows)[index]);
	for (int i = 0; i < (*columns)[index]; i++) {
		for (int j = 0; j < (*rows)[index]; j++)
			// the transpose is created by swapping
			// the rows and columns dimensions
			transpose[i][j] = (*mat_array)[index][j][i];
	}
	free_matrix((*rows)[index], (*mat_array)[index]);
	(*mat_array)[index] = transpose;
	swap_p(&(*rows)[index], &(*columns)[index]);
	return (*mat_array)[index];
}

// function for deleting the matrix located at a given index
void free_matrixes(int ****mat_array, int **rows, int **columns, int index,
				   int *number)
{	scanf("%d", &index);
	// checking if the given index exists
	if (index > *number || index <= -1) {
		printf("No matrix with the given index\n");
	} else {
		// freeing the memory that was allocated for that matrix
		free_matrix((*rows)[index], (*mat_array)[index]);
		// shifting the rest of the matrices to the left
		for (int i = index; i < (*number); i++) {
			(*mat_array)[i] = (*mat_array)[i + 1];
			(*rows)[i] = (*rows)[i + 1];
			(*columns)[i] = (*columns)[i + 1];
		}
		// decreasing the number of matrices stored
		(*number)--;
	}
}

// function for sorting all matrices according to the sum of the elements
void sort_mat(int number, int ****mat_array, int **rows, int **columns)
{
	int *sort = malloc((number + 1) * sizeof(int));
	int **mat;
	// creating a vector that stores the sum of elements for each matrix;
	for (int i = 0; i <= number; i++) {
		sort[i] = 0;
		for (int k = 0; k < (*rows)[i]; k++)
			for (int j = 0; j < (*columns)[i]; j++) {
				sort[i] = (sort[i] + (*mat_array)[i][k][j]) % MODULO;
				// if the value is negative, bringing it to int is required
				if (sort[i] < 0)
					sort[i] = sort[i] + MODULO;
			}
	}
	// the vector is sorted in ascending order
	// using the selection sort method
	for (int i = 0; i < number; i++)
		for (int j = i + 1; j <= number; j++)
			if (sort[i] > sort[j]) {
				swap_p(&sort[i], &sort[j]);
				mat = (*mat_array)[i];
				(*mat_array)[i] = (*mat_array)[j];
				(*mat_array)[j] = mat;
				swap_p(&(*rows)[i], &(*rows)[j]);
				swap_p(&(*columns)[i], &(*columns)[j]);
			}
	free(sort);
}

// function for multiplying two matrices
void mat_multi(int ****mat_array, int *number, int **rows,
			   int **columns)
{	int ind1, ind2;
	scanf("%d%d", &ind1, &ind2);
	if (ind1 > (*number) || ind1 <= -1 || ind2 > (*number) || ind2 <= -1) {
		printf("No matrix with the given index\n");
	} else {
		// checking if the multiplication is possible
		if ((*rows)[ind2] == (*columns)[ind1]) {
			// allocating space in vectors for the matrix that
			// will contain the result of the multiplication
			(*number) = (*number) + 1;
			*mat_array = realloc(*mat_array, (*number + 1) * sizeof(int **));
			*rows = realloc(*rows, (*number + 1) * sizeof(int));
			*columns = realloc(*columns, (*number + 1) * sizeof(int));
			int **mat_1 = (*mat_array)[ind1];
			int **mat_2 = (*mat_array)[ind2];
			int **multi = alloc_matrix((*rows)[ind1], (*columns)[ind2]);
			// performing the basic multiplication algorithm by summing the
			// products of the elements that have in common the column size
			// (for the first one) and the row size (for the second)
			for (int i = 0; i < (*rows)[ind1]; i++)
				for (int j = 0; j < (*columns)[ind2]; j++) {
					multi[i][j] = 0;
					for (int k = 0; k < (*rows)[ind2]; k++) {
						multi[i][j] = multi[i][j] + (mat_1[i][k] * mat_2[k][j]);
						multi[i][j] = multi[i][j] % MODULO;
					}
				multi[i][j] = multi[i][j] % MODULO;
				// if the value is negative, bringing it to int is required
				if (multi[i][j] < 0)
					multi[i][j] = multi[i][j] + MODULO;
				}
			// the values ​​in the vectors are initialized according to the
			// dimensions of the matrix resulting from the multiplication
			(*mat_array)[*number] = multi;
			(*rows)[*number] = (*rows)[ind1];
			(*columns)[*number] = (*columns)[ind2];
			} else {
				printf("Cannot perform matrix multiplication\n");
		}
	}
}

// function used for performing the operation of addition or
// substract, depending on the character that is being read
// mat_op stands for matrix_operations and i chose this short
// name due to lack of characters on lines / number of lines
int **mat_op(int **a, int **b, int n, char letter)
{
	int **op = alloc_matrix(n, n);
	if (letter == 's') {
		for (int  i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				op[i][j] = (a[i][j] - b[i][j]) % MODULO;
	}
	if (letter == 'a') {
		for (int  i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				op[i][j] = (a[i][j] + b[i][j]) % MODULO;
	}
	return op;
}

// function for freeing the memory allocated for the
// matrices used for the multiplication operations
void free_m(int **m1, int **m2, int **m3, int **m4,
			int **m5, int **m6, int **m7, int dim)
{
	free_matrix(dim, m1);
	free_matrix(dim, m2);
	free_matrix(dim, m3);
	free_matrix(dim, m4);
	free_matrix(dim, m5);
	free_matrix(dim, m6);
	free_matrix(dim, m7);
}

// function for freeing the memory allocated for the matrices
// used for the substraction operations
void free_s(int **s1, int **s2, int **s3, int **s4, int dim)
{
	free_matrix(dim, s1);
	free_matrix(dim, s2);
	free_matrix(dim, s3);
	free_matrix(dim, s4);
}

// function for freeing the memory allocated for the matrices
// used for the sum operations
void free_a(int **a1, int **a2, int **a3, int **a4,
			int **a5, int **a6, int **a7, int **a8,
			int **a9, int **a10, int dim)
{
	free_matrix(dim, a1);
	free_matrix(dim, a2);
	free_matrix(dim, a3);
	free_matrix(dim, a4);
	free_matrix(dim, a5);
	free_matrix(dim, a6);
	free_matrix(dim, a7);
	free_matrix(dim, a8);
	free_matrix(dim, a9);
	free_matrix(dim, a10);
}

// function that frees the 4 small matrices that
// are being assembled into the final matrix
void free_c(int **c1, int **c2, int **c3, int **c4, int dim)
{
	free_matrix(dim, c1);
	free_matrix(dim, c2);
	free_matrix(dim, c3);
	free_matrix(dim, c4);
}

// this function frees the 8 submatrices
// into which the matrix has been divided
void free_q(int ***q1, int ***q2, int dim)
{
	free_matrix(dim, q1[0]);
	free_matrix(dim, q2[0]);
	free_matrix(dim, q1[1]);
	free_matrix(dim, q2[1]);
	free_matrix(dim, q1[2]);
	free_matrix(dim, q2[2]);
	free_matrix(dim, q1[3]);
	free_matrix(dim, q2[3]);
	free(q1); free(q2);
}

// this is the Strassen Algorithm for matrix multiplication implemented
// with a recursive function that follows Divide and Conquer method
int **strassen_multiply(int **mat1, int **mat2, int n)
{
	// if the size of the matrix is equal to 1, then the expected
	// result has been reached; the function will no longer be
	// called and will return the result of the multiplication
	if (n == 1) {
		int **c = alloc_matrix(1, 1); c[0][0] = mat1[0][0] * mat2[0][0];
		return c;
	}
	// if the size is not equal to 1 yet, then memory needs to be
	// allocated for the matrix that will store the result,
	// according to its n sizes
	int **c = alloc_matrix(n, n); int dim = n / 2; int k = 4;
	// the matrix is splitted into 2 matrices that will be
	// treated each as an individual element of the matrix
	int ***q1 = malloc(4 * sizeof(int **)), ***q2 = malloc(4 * sizeof(int **));
	// each matrix will be splitted in 4 smaller
	// matrices; therefore, each q will be a quarter
	for (int i = 0; i < k; i++) {
		q1[i] = alloc_matrix(dim, dim); q2[i] = alloc_matrix(dim, dim);
	}
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++) {
			// the newly created matrices are initialised
			// using the elements of the first matrix
			q1[0][i][j] = mat1[i][j]; q2[0][i][j] = mat2[i][j];
			q1[1][i][j] = mat1[i][j + dim]; q2[1][i][j] = mat2[i][j + dim];
			q1[2][i][j] = mat1[i + dim][j]; q2[2][i][j] = mat2[i + dim][j];
			q1[3][i][j] = mat1[i + dim][j + dim];
			q2[3][i][j] = mat2[i + dim][j + dim];
		}
	// first, the matrices that contain
	// addition operations are initialised
	int **a1 = mat_op(q1[0], q1[3], dim, 'a');
	int **a2 = mat_op(q2[0], q2[3], dim, 'a');
	int **a3 = mat_op(q1[2], q1[3], dim, 'a');
	int **a4 = mat_op(q1[0], q1[1], dim, 'a');
	int **a5 = mat_op(q2[0], q2[1], dim, 'a');
	int **a6 = mat_op(q2[2], q2[3], dim, 'a');
	// then are initialised the ones that
	// contain substraction operations
	int **s1 = mat_op(q2[1], q2[3], dim, 's');
	int **s2 = mat_op(q2[2], q2[0], dim, 's');
	int **s3 = mat_op(q1[2], q1[0], dim, 's');
	int **s4 = mat_op(q1[1], q1[3], dim, 's');
	// these recursive matrices will get smaller
	// and smaller untill they reach size 1 x 1
	int **m1 = strassen_multiply(a1, a2, dim);
	int **m2 = strassen_multiply(a3, q2[0], dim);
	int **m3 = strassen_multiply(q1[0], s1, dim);
	int **m4 = strassen_multiply(q1[3], s2, dim);
	int **m5 = strassen_multiply(a4, q2[3], dim);
	int **m6 = strassen_multiply(s3, a5, dim);
	int **m7 = strassen_multiply(s4, a6, dim);
	free_s(s1, s2, s3, s4, dim); free_q(q1, q2, dim);
	int **a7 = mat_op(m1, m4, dim, 'a'); int **a8 = mat_op(a7, m7, dim, 'a');
	int **a9 = mat_op(m1, m3, dim, 'a'); int **a10 = mat_op(a9, m6, dim, 'a');
	// c1, c2, c3, c4 are the elements
	// of the resulted matrix
	int **c1 = mat_op(a8, m5, dim, 's'); int **c2 = mat_op(m3, m5, dim, 'a');
	int **c3 = mat_op(m2, m4, dim, 'a'); int **c4 = mat_op(a10, m2, dim, 's');
	free_m(m1, m2, m3, m4, m5, m6, m7, dim);
	free_a(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, dim);
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++) {
			// then the final matrix is built
			// by placing each submatrix in it
			c[i][j] = c1[i][j] % MODULO; c[i][j + dim] = c2[i][j] % MODULO;
			// if the value is negative, bringing it to int is required
			if (c[i][j] < 0)
				c[i][j] = c[i][j] + MODULO;
			if (c[i][j + dim] < 0)
				c[i][j + dim] = c[i][j + dim] + MODULO;
			c[i + dim][j] = c3[i][j] % MODULO;
			if (c[i + dim][j] < 0)
				c[i + dim][j] = c[i + dim][j] + MODULO;
			c[i + dim][j + dim] = c4[i][j] % MODULO;
			if (c[i + dim][j + dim] < 0)
				c[i + dim][j + dim] = c[i + dim][j + dim] + MODULO;
		}
	free_c(c1, c2, c3, c4, dim); return c;
}

void strassen(int ****mat_array, int **rows, int **columns,
			  int *number, int ind1, int ind2)
{
	if (ind1 > (*number) || ind1 <= -1 || ind2 > (*number) ||
		ind2 <= -1) {
		printf("No matrix with the given index\n");
	} else if ((*rows)[ind2] == (*columns)[ind1]) {
		// memory is allocated for the new matrix that will
		// result after the execution of the Strassen algorithm
		(*number)++;
		reallocate(mat_array, rows, columns, *number);
		(*mat_array)[*number] = strassen_multiply((*mat_array)[ind1],
											   (*mat_array)[ind2],
											   (*rows)[ind2]);
		(*rows)[*number] = (*rows)[ind1];
		(*columns)[*number] = (*columns)[ind1];
	} else {
		printf("Cannot perform matrix multiplication\n");
	}
}

int main(void)
{
	char letter = 'A';
	// the variable which the switch depends on is initialized
	// with a letter that is not found in the given cases
	int *rows, *columns, ***mat_array, m, n, number = -1;
	int index, ind1, ind2;
	allocate(&mat_array, &rows, &columns);
	// the program will treat each case separately, using a
	// switch, depending on the character that is entered
	while (letter != 'Q') {
		scanf("%c", &letter);
		switch (letter) {
		case 'L':
			scanf("%d%d", &m, &n); number++;
			rows[number] = m; columns[number] = n;
			reallocate(&mat_array, &rows, &columns, number + 1);
			mat_array[number] = alloc_matrix(m, n);
			read_matrix(m, n, mat_array[number]);
			break;
		case 'D':
			scanf("%d", &index);
			// checking if the given index exists
			if (index > number || index <= -1)
				printf("No matrix with the given index\n");
			else
				printf("%d %d\n", rows[index], columns[index]);
			break;
		case 'P':
			scanf("%d", &index);
			if (index > number || index <= -1)
				printf("No matrix with the given index\n");
			else
				print_matrix(rows[index], columns[index], mat_array[index]);
			break;
		case 'C':
			resize_matrix(index, &mat_array, &rows, &columns, number);
			break;
		case 'M':
			mat_multi(&mat_array, &number, &rows, &columns);
			break;
		case 'O':
			sort_mat(number, &mat_array, &rows, &columns);
			break;
		case 'T':
			scanf("%d", &index);
			if (index > number || index < -1)
				printf("No matrix with the given index\n");
			else
				mat_array[index] = transpose_matrix(&mat_array, &rows,
													&columns, index);
			break;
		case 'F':
			free_matrixes(&mat_array, &rows, &columns, index, &number);
			break;
		case 'S':
			scanf("%d%d", &ind1, &ind2);
			strassen(&mat_array, &rows, &columns, &number, ind1, ind2);
			break;
		case 'Q':
			// in this case, all allocated resources are released
			for (int i = 0; i <= number; i++)
				free_matrix(rows[i], mat_array[i]);
			free(rows); free(columns); free(mat_array);
			break;
		case '\n':
			break;
		default:
			printf("Unrecognized command\n");
			break;
		}
	}
	return 0;
}
