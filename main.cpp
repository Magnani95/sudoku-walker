#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <gmp.h>
#include <gmpxx.h>


using namespace std;

//	MACRO
#define DIM 9
//	CONSTEXPR
constexpr uint64_t factorial(uint64_t n)
{
	return n == 0 ? 1  :  n * factorial(n-1);
}
//	GLOBAL VARIABLE

void print_mat(unsigned char [DIM][DIM]);

bool is_valid(unsigned char [DIM][DIM]);
	bool check_line(unsigned char matrix[DIM][DIM], unsigned int i);
	bool check_column(unsigned char matrix[DIM][DIM],unsigned int i);
	bool check_box(unsigned char matrix[DIM][DIM],unsigned int i);
void first_valid_block(unsigned char matrix[DIM][DIM], mpz_t skipped_permutation);


int main()
{
	unsigned char matrix[DIM][DIM];
	for (size_t j = 0; j < 9; j++) {
		for(size_t i=0; i<9; i++){
				matrix[i][j]=j;
			}
	}
	//---	GMP
	mpz_t skipped_permutation;
	mpz_t valid, not_valid, i, loop_end;
	mpz_t update_value;
	mpz_t q, r, fact;
	mpz_t percentage, print_modulus;

	mpz_init(skipped_permutation);
	mpz_set_ui(skipped_permutation,0);

	mpz_init(valid);
	mpz_set_ui(valid,0);

	mpz_init(not_valid);
	mpz_set_ui(not_valid,0);

	mpz_init(i);

	mpz_init(loop_end);
	mpz_ui_pow_ui(loop_end,factorial(DIM),DIM-1);

	mpz_init(update_value);

	mpz_init(q);
	mpz_init(r);

	mpz_init(fact);
	mpz_set_ui(fact,factorial(DIM));

	mpz_init(percentage);
	mpz_init(print_modulus);

	//--- END GMP
	//	LINE CHECKER

	first_valid_block(matrix, skipped_permutation);
	//cout<<"\nFinished first_valid_block()";
	print_mat(matrix);

	mpz_set(i,skipped_permutation);

	//---END LINE CHECKER

	////cout<<"\n--PRE\n"; print_mat(matrix);
	//cout<<endl;
	for (; mpz_cmp(i,loop_end) != 0; mpz_add_ui(i,i,1)) {
		mpz_mod_ui(print_modulus, i, 500000000UL);
		if(mpz_cmp_ui(print_modulus,0) == 0) {
			mpz_tdiv_q(q,i,fact);
			mpz_tdiv_q(percentage,i,loop_end);
			mpz_mul_ui(percentage,percentage,10000);
			cout<<"\rFull-first permutations "<<q<<" - ("<<percentage<<"%%%) "<<i;
			//print_mat(matrix);
		}
		if(is_valid(matrix)){
			cout<<"\n--VALID!\t"<<i;
			mpz_add_ui(valid,valid,1);
		}else{
			mpz_add_ui(not_valid,not_valid,1);
		}
		//	matrix permutation-update
		next_permutation(&matrix[1][0],&matrix[1][DIM]);
		for (size_t r = 2; r < DIM; r++) {
			mpz_set_ui(update_value,factorial(DIM));
			mpz_mul_ui(update_value,update_value,r);
			mpz_tdiv_r(update_value, i, update_value);

			if ( mpz_cmp_ui(update_value, 0) == 0){
				next_permutation(&matrix[r][0],&matrix[r][DIM]);
				////cout<<"\n--Updated row: "<<r;
				////cout<<"\nPOST-UPDATE"<<"\n"; print_mat(matrix);
			}
		}
		////cout<<"\n--------------------------------------";
		//if ( mpz_cmp_ui(i, factorial(12)*5) > 0)
		//if ( mpz_cmp_ui(i, 10) == 0)
		//	break;

	}
	cout<<"\nFINAL"<<"\n"; print_mat(matrix);

	mpz_t valid_per, not_valid_per;

	mpz_init(valid_per);
	mpz_init(not_valid_per);

	mpz_tdiv_q(valid_per, valid, i);
	mpz_tdiv_q(not_valid_per, not_valid, i);
	mpz_mul_ui(valid_per,valid_per, 100);
	mpz_mul_ui(not_valid_per,valid_per, 100);

	cout<<"\nRESULT\n-valid:\t"<<valid<<" ("<<valid_per<<"%)";
	cout<<"\n-not_valid:\t"<<not_valid<<" ("<<not_valid_per<<"%)";
	cout<<"\n-total:\t"<<loop_end;
	cout<<flush<<endl;

	mpz_clear(valid);
	mpz_clear(not_valid);
	mpz_clear(i);
	return 0;
}
//---	MAIN END
//------------------------------------------------------------------------------
void print_mat(unsigned char matrix[DIM][DIM])
{
	//cout<<endl;
	for (size_t l = 0; l < DIM; l++) {
		for (size_t c = 0; c < DIM; c++) {
			//cout<<+matrix[l][c]<<" ";
		}
		//cout<<endl;
	}
}

bool is_valid(unsigned char matrix[DIM][DIM])
{
	/*for (size_t i = 0; i < DIM; i++) {
		if(check_line(matrix,i)){
			continue;
		}else{
			lines=false;
			break;
		}
	}*/
	for (size_t i = 0; i < DIM; i++) {
		if(check_column(matrix,i)){
			continue;
		}else{
			return false;
		}
	}
	for (size_t i = 0; i < DIM; i++) {
		if(check_box(matrix,i)){
			continue;
		}else{
			return false;
		}
	}

	return true;
}


bool check_line(unsigned char matrix[DIM][DIM], unsigned int line)
{
	bool digits[9]={0};
	for (size_t i = 0; i < DIM; i++) {
		int value = matrix[line][i];
		if(digits[value] == 0){
			digits[value]=true;
		}else{	//digit is true => double digit
			////cout<<"\n!!impossible\n";
			exit(2);
			return false;
		}
	}
	return true;
}

bool check_column(unsigned char matrix[DIM][DIM], unsigned int column)
{
	bool digits[9]={0};
	for (size_t i = 0; i < DIM; i++) {
		int value = matrix[i][column];
		if(digits[value] == 0){
			digits[value]=true;
		}else{	//digit is true => double digit
			////cout<<"\n!! column: "<<i;
			return false;
		}
	}
	return true;
}
bool check_box(unsigned char matrix[DIM][DIM], unsigned int box)
{
	unsigned char line_offset, column_offset;
	bool digits[DIM]={0};

	switch(box/3){
		case 0:
			line_offset=0;
			break;
		case 1:
			line_offset=3;
			break;
		case 2:
			line_offset=6;
			break;
		default: exit(9);
	}
	switch (box%3) {
		case 0:
			column_offset=0;
			break;
		case 1:
			column_offset=3;
			break;
		case 2:
			column_offset=6;
			break;
		default: exit(9);
	}

	////cout<<"\nBOX:\t"<<box;
	for (size_t i = 0; i < DIM; i++) {
		////cout<<"\n--line idx: "<<i/3+line_offset<<"\n--column idx: "<<i%3+column_offset;
		int value = matrix[i/3+line_offset][i%3+column_offset];
		if(digits[value] == 0){
			digits[value]=true;
		}else{	//digit is true => double digit
			////cout<<"\n!! box: "<<i;
			return false;
		}
	}
	return true;

}

void first_valid_block(unsigned char matrix[DIM][DIM], mpz_t skipped_permutation)
{
	bool valid_line;
	size_t permutation=0;

	mpz_t line_skipped_permutation;
	mpz_init(line_skipped_permutation);
	mpz_set_ui(line_skipped_permutation,0);

	mpz_t fact;
	mpz_init(fact);
	mpz_set_ui(fact,0);

	for (size_t i = 8; 0<i ; --i) {
		//cout<<"\n++Loop:\t"<<i<<flush;
		permutation=0;
		mpz_set_ui(fact,0);
		mpz_set_ui(line_skipped_permutation,0);
		while(next_permutation(&matrix[i][0],&matrix[i][DIM])){
			permutation++;
			valid_line=true;
			for (size_t c = 0; c < DIM; c++) {
				for (size_t l = i+1; l <= DIM; l++) {
					if( matrix[i][c] == matrix[l%DIM][c]){
						valid_line=false;
						break;
					}
				}
				if(!valid_line){
					break;
				}
			}
			if(valid_line){
				break;
			}
		}
		if(!valid_line){
			//cout<<"\nImpossible error\n";
			exit(2);
		}else{
			//cout<<"\n---\nLine ["<<i<<"] is valid ("<<permutation<<")";
			mpz_set_ui(fact,factorial(i));
			mpz_mul_ui(line_skipped_permutation, fact, permutation);
			//cout<<"\nline_skipped_permutation ("<<line_skipped_permutation<<")"<<flush;
			mpz_add(skipped_permutation,skipped_permutation,line_skipped_permutation);
			//cout<<"\nTotal_skipped_permutation ("<<skipped_permutation<<")";
			//cout<<"\n---";

		}
	}
	mpz_clear(fact);
	mpz_clear(line_skipped_permutation);
}
