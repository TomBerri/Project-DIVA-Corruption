#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "../include/DSC_rand.h"

int compare_opcodes_len12(char a[12], char b[12]) {
	/*
	 * This is a custom strcmp method for 12 character arrays
	 * This is used instead of strcmp because of problems were caused by input strings using NULL values.
	 */
	for (int z=0; z<12; z++) {
		if (a[z] != b[z]) {
			return -1;
		}
	}
	return 0;
}

void getRandomTarget(FILE *DSC, FILE *randDSC , int DSC_size) {
	/*
	 * The "big boy" method which randomises the inputed DSC and outputs a new randomised DSC.
	 * This method searches for any standard target (face buttons, doubles and star) with a random standard target.
	 * It also searches for rush and hold targets and randomises them for any rush or hold target.
	 * Technical Zones and Chance Times are unaffected.
	 */
	char dsc_read[4]; // Used for reading in 4 characters from the DSC
	char target_opcode[4] = {6, 0, 0, 0}; // Used to see if dsc_read indicates a target as the next read
	char start_opcode[8] = {'ÿ', 'ÿ', 'ÿ', 'ÿ', 1, 0, 0, 0}; // Used for the start and end hold time of the end hold target
	char rush_and_hold_opcode[12] = {1, 0, 0, 0, 'ÿ', 'ÿ', 'ÿ', 'ÿ', 1, 0, 0, 0}; // Used to indicate the end of the hold info.
	int i, j, k; // Different iterators for the DSC
	
	for (i=0; i<DSC_size; i+=4) { // For each 4 characters from the file, find out if it is equal to a target opcode
		int rand_target;
		int rush_hold_exiter;
		fread(&dsc_read, sizeof(char), 4, DSC);
	
		if (strcmp(dsc_read, target_opcode) == 0) { // If the read in is a target opcode, write it, then read in the next 4
			fwrite(&dsc_read, sizeof(char), 4, randDSC);
			fread(&dsc_read, sizeof(char), 4, DSC);
	
			if (dsc_read[0] < 8 || (dsc_read[0] == 12)) { // If the target is a face button, double or star, randomise, write to randDSC, add 4 to i
				do {
					rand_target = rand() % 13;
				} while (rand_target > 7 && rand_target < 12);
				dsc_read[0] = rand_target;
				fwrite(&dsc_read, sizeof(char), 4, randDSC);
				i+=4;
				
			} else if ((dsc_read[0] > 8 && dsc_read[0] < 12) || (dsc_read[0] > 24 && dsc_read[0] < 29)) { // If the target is a hold or rush, randomise, write to randDSC, add 4 to i
				char random_target[4] = {0, 0, 0, 0};
				char opcode_finder[12];
				char dsc_write[4]; // This is now used to read from DSC whilst in this condition. DSC read is used to store the target read in earlier.
								
				rand_target = rand() % 8;
				
				if (rand_target < 4) {
					rand_target += 8;
				} else {
					rand_target += 21;
				}
				
				random_target[0] = rand_target;
				fwrite(&random_target, sizeof(char), 4, randDSC);
				i+=4;
				//printf("rand target: %i\n", rand_target);
				
				if ((dsc_read[0] > 7 && dsc_read[0] < 12) && (rand_target > 7 && rand_target < 12)) { // If the target read in is a hold and the randomised target is a hold, just change the hold start and end targets
					for (j=0; j<DSC_size; j+=4) {
					
						fread(&dsc_write, sizeof(char), 4 , DSC);
					
						if (strcmp(dsc_write, dsc_read) == 0) {
							fwrite(&random_target, sizeof(char), 4, randDSC);
							j+=4;
							break;
						}
					
						fwrite(&dsc_write, sizeof(char), 4, randDSC);
					
					}
				
					i = i + j;
				
				} else if ((dsc_read[0] > 7 && dsc_read[0] < 12) && (rand_target > 24 && rand_target < 29)) { // If the target read in is a hold and the randomised target is a rush...
					for (j=0; j<DSC_size; j+=4) { // This for loop essentially removes the end hold target from the DSC and replaces the start hold target for a rush target
						
						fread(&opcode_finder, sizeof(char), 12 , DSC);
						
						if (compare_opcodes_len12(opcode_finder, rush_and_hold_opcode) == 0) {
							
							j+=12;
							fwrite(&opcode_finder, sizeof(char), 12, randDSC);
							
							for(k=0; k<DSC_size; k+=4) {
								fread(&opcode_finder, sizeof(char), 12 , DSC);
								
								if (compare_opcodes_len12(opcode_finder, rush_and_hold_opcode) == 0) {
									
									k+=12;
									rush_hold_exiter = 1;
									break;
								}
								
								fseek(DSC, -8, SEEK_CUR);
							}
							
							if (rush_hold_exiter == 1) {
								break;
							}
						}
						
						dsc_write[0] = opcode_finder[0];
						dsc_write[1] = opcode_finder[1];
						dsc_write[2] = opcode_finder[2];
						dsc_write[3] = opcode_finder[3];
						fwrite(&dsc_write, sizeof(char), 4, randDSC); 
						
						fseek(DSC, -8, SEEK_CUR);
					}
					
					i = i + j + k;
					
				} else if ((dsc_read[0] > 24 && dsc_read[0] < 29) && (rand_target > 7 && rand_target < 12)) { // If the target read in is a rush and the randomised target is a hold...
					FILE *temp_hold_end;
					temp_hold_end = fopen("temp_hold.bin", "wb");
					fwrite(&target_opcode, sizeof(char), 4, temp_hold_end);
					fwrite(&random_target, sizeof(char), 4, temp_hold_end);
					fwrite(&start_opcode, sizeof(char), 8, temp_hold_end);
					fread(&dsc_write, sizeof(char), 4 , DSC);
					fwrite(&dsc_write, sizeof(char), 4, randDSC);
					fread(&dsc_write, sizeof(char), 4 , DSC);
					fwrite(&dsc_write, sizeof(char), 4, randDSC);
					i+=8;
					
					/*
					*	As you can see, this part of the randomisation is very messy and is still very buggy and experimental. This can sometimes cause the DSC to crash on load and also cause
					*	a end hold target to never appear making you MISS the rest of the chart. If you experience either of these two things, it is known!
					*	
					*	Essentially this for loop changes the rush to a start hold target and adds in an end hold target using the start hold information. I'm still trying to figure out a
					*	better way to detect where (or rather when) the hold end should be inserted. As before, this is still very buggy but work the majority of the time. 
					*/
					
					for (j=0; j<DSC_size; j+=4) {
						
						fread(&opcode_finder, sizeof(char), 12 , DSC);
						
						if (compare_opcodes_len12(opcode_finder, rush_and_hold_opcode) == 0) {
							
							j+=12;
							fwrite(&opcode_finder, sizeof(char), 12, randDSC);
							fwrite(&opcode_finder, sizeof(char), 12, temp_hold_end);
							fclose(temp_hold_end);
							
							for(k=0; k<DSC_size; k+=4) {
								fread(&dsc_write, sizeof(char), 4 , DSC);
								
								if (dsc_write[0] == 1) {
									fseek(DSC, +4, SEEK_CUR);
									fread(&dsc_write, sizeof(char), 4 , DSC);
									
									if (dsc_write[0] == 1) {
										fseek(DSC, -12, SEEK_CUR);
										fread(&dsc_write, sizeof(char), 4 , DSC);
										fwrite(&dsc_write, sizeof(char), 4, randDSC);
										fread(&dsc_write, sizeof(char), 4 , DSC);
										fwrite(&dsc_write, sizeof(char), 4, randDSC);
										fread(&dsc_write, sizeof(char), 4 , DSC);
										
										k+=12;
										
										temp_hold_end = fopen("temp_hold.bin", "rb");
										fseek(temp_hold_end, 0L, SEEK_END);
										int temp_size = ftell(temp_hold_end);
										rewind(temp_hold_end);
										
										for (int q=0; q<temp_size; q+=4) {
											fread(&dsc_write, sizeof(char), 4 , temp_hold_end);
											fwrite(&dsc_write, sizeof(char), 4, randDSC);
										}
										
										fclose(temp_hold_end);
										remove("temp_hold.bin");
										
										rush_hold_exiter = 1;
										break;
									}
									
									fseek(DSC, -12, SEEK_CUR);
									fread(&dsc_write, sizeof(char), 4 , DSC);
								}
							
								fwrite(&dsc_write, sizeof(char), 4, randDSC);
							}
						
							if (rush_hold_exiter == 1) {
								break;
							}
						}
						
						dsc_write[0] = opcode_finder[0];
						dsc_write[1] = opcode_finder[1];
						dsc_write[2] = opcode_finder[2];
						dsc_write[3] = opcode_finder[3];
						fwrite(&dsc_write, sizeof(char), 4, randDSC);
						fwrite(&dsc_write, sizeof(char), 4, temp_hold_end); 
						
						fseek(DSC, -8, SEEK_CUR);	
					}
					
					i = i + j + k;
				}
				
			} else {
				fwrite(&dsc_read, sizeof(char), 4, randDSC);
			}
			
			
		} else {
			fwrite(&dsc_read, sizeof(char), 4, randDSC);
		}
	}
}

void DSC_rand() {
	
	/*
	* This is a method used to cycle through all encrypted (DIVAFILE) DSCs located next to the exe. It then decrypts them all (provided the DIVAFILE_Tool.exe is available) and saves them
	* in a new folder called Decrypted_PDX_DSCs. If no DSCs are loacted next to the exe, it will then cycle through all DSCs located in the Decrypted_PDX_DSCs folder. The new randomised 
	* DSCs are stored in a folder called PDRX Files. The DSCs don't have to be re-encrypted and can just inserted into the data.psarc files and packed into a psarc.
	*/
	
	srand(time(NULL)); 
	
	FILE *DSC, *randDSC;
	const char *str_pv = "pv_"; // All the const char variables are used for creating the DSC filenames and arguments for the DIVAFILE_Tool
	const char *dif[4] = {"_easy.dsc", "_normal.dsc", "_hard.dsc", "_extreme.dsc"};
	const char *decrypt_path = "Decrypted_PDX_DSCs/";
	const char *DIVAFILE_Tool_arg = "DIVAFILE_Tool.exe e ";
	const char *rand_path = "PDRX Files/";
	const char *space = " ";
	int x, y;

	CreateDirectory("Decrypted_PDX_DSCs", NULL); // Creates a directory for the decrypted DSCs if one doesn't already exist

	for (x=701; x<831; x++) { // Cycles through all possible DSCs in X, if it exists next to the exe, then decrypt and save it, else continue to the next filename
		for(y=0; y<4; y++) {
			char *DSC_name = (char *) calloc(18, (sizeof(char) * 18));
			char str_DSC_num[4];

			snprintf(str_DSC_num, 4, "%d", x);
			strcat(DSC_name, str_pv);
			strcat(DSC_name, str_DSC_num);
			strcat(DSC_name, dif[y]);

			DSC = fopen(DSC_name, "rb");
	
			if (DSC == NULL) {
				free(DSC_name);
				continue;
			}

			fclose(DSC);

			char *DIVAFILE_Tool = (char *) calloc(80, (sizeof(char) * 80));
			strcat(DIVAFILE_Tool, DIVAFILE_Tool_arg);
			strcat(DIVAFILE_Tool, DSC_name);
			strcat(DIVAFILE_Tool, space);
			strcat(DIVAFILE_Tool, decrypt_path);
			strcat(DIVAFILE_Tool, DSC_name);

			printf("%s\n", DIVAFILE_Tool);

			system(DIVAFILE_Tool);
			free(DIVAFILE_Tool);
		}
	}

	for (x=701; x<831; x++) { // Cycles through all possible DSCs in X, if it exists in the Decrypted_PDX_DSCs, randomise it and save it in the PDRX Files folder, else continue to the next filename
		for(y=0; y<4; y++) {
			char *DSC_name = (char *) calloc(18, (sizeof(char) * 18));
			char *DSC_path_name = (char *) calloc(40, (sizeof(char) * 40));
			char str_DSC_num[4];

			snprintf(str_DSC_num, 4, "%d", x);
			strcat(DSC_name, str_pv);
			strcat(DSC_name, str_DSC_num);
			strcat(DSC_name, dif[y]);

			strcat(DSC_path_name, decrypt_path);
			strcat(DSC_path_name, DSC_name);

			DSC = fopen(DSC_path_name, "rb");

			if (DSC == NULL) {
				free(DSC_path_name);
				free(DSC_name);
				continue;
			}

			printf("%s\n", DSC_path_name);

			char *randDSC_name = (char *) calloc(30, (sizeof(char) * 30));

			strcat(randDSC_name, rand_path);
			strcat(randDSC_name, DSC_name);

			randDSC = fopen(randDSC_name, "wb");

			fseek(DSC, 0L, SEEK_END);
			int DSC_size = ftell(DSC);
			rewind(DSC);

			getRandomTarget(DSC, randDSC, DSC_size);

			free(DSC_name);
			free(DSC_path_name);
			free(randDSC_name);
			fclose(DSC);
			fclose(randDSC);
		}
	}
	fprintf(stderr, "DSC Randomised!");
}
