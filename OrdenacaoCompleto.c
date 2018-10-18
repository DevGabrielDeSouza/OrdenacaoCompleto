#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define tammax 1000000

const int key_length = 8;
const int key_position = 290;
const long register_size = 300;

int compare_strings (const void * string_1, const void * string_2) {
	
	const char* s1 = (const char *)(string_1);
	const char* s2 = (const char*)(string_2);
	
	return strncmp(&s1[key_position], &s2[key_position],key_length);	
}

void merge(FILE *output_file, FILE *input_file_1, FILE *input_file_2){	
	
	int read_register_1, read_register_2;
	char buffer1[register_size], buffer2[register_size];
	
	read_register_1 = fread(&buffer1,1,register_size,input_file_1);
	read_register_2 = fread(&buffer2,1,register_size,input_file_2);
	
	while(read_register_1>0 && read_register_2>0){		
		if(strncmp(&buffer1[key_position],&buffer2[key_position],key_length)<0){			
			fwrite(&buffer1,read_register_1,1,output_file);			
			read_register_1 = fread(&buffer1,1,register_size,input_file_1);
		} else{
			fwrite(&buffer2,read_register_2,1,output_file);			
			read_register_2 = fread(&buffer2,1,register_size,input_file_2);
		}		
	}
	
	while(read_register_1>0){		
		fwrite(&buffer1,read_register_1,1,output_file);				
		read_register_1 = fread(&buffer1,1,register_size,input_file_1);		
	}
	
	while(read_register_2>0){		
		fwrite(&buffer2,read_register_2,1,output_file);				
		read_register_2 = fread(&buffer2,1,register_size,input_file_2);		
	}
}

int main(int argc, char *argv[]) {
	FILE *output_file;
	FILE *input_file_1;
	FILE *input_file_2;

	int read_register;
	long file_size;
	long block_amount;
	long register_amount;
		
	output_file = fopen("cep.dat", "r");

	fseek(output_file,0,SEEK_END);

	file_size=ftell(output_file);

	rewind(output_file);
		
	if(file_size%tammax == 0){
		 block_amount= file_size/tammax;
	}else{
		 block_amount = file_size/tammax +1;
	}
	
	int current = 0;
	int end = block_amount-1;
	
	char name[40];
		
	register_amount = tammax/register_size;	
	
	char buffer[register_amount*register_size];	
	
	read_register = fread(buffer, register_size, register_amount, output_file);
	
	while(read_register > 0){		
		qsort(buffer, register_amount, register_size, compare_strings);
		
		sprintf(name,"bloco%d.dat", current);	
		current++;
		
		input_file_1= fopen(name,"w");
		fwrite(buffer, register_size, read_register, input_file_1);
		fclose(input_file_1);
		
		read_register = fread(buffer, register_size, register_amount, output_file);
	}
	
	fclose(output_file);
	
	int read_register_1, read_register_2;
	char buffer1[register_size], buffer2[register_size];
	current=0;
	
	while(current < end){		
		sprintf(name,"bloco%d.dat", current);
		current++;
		input_file_1= fopen(name, "r");
		
		sprintf(name,"bloco%d.dat", current);
		current++;
		input_file_2= fopen(name, "r");
		
		end++;
		sprintf(name, "bloco%d.dat", end);
		output_file= fopen(name,"w");		
		
		merge(output_file, input_file_1, input_file_2);
		
		fclose(output_file);
		fclose(input_file_1);
		fclose(input_file_2);				
	}			
	return 0;
}