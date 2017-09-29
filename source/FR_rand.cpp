#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/FR_rand.h"

int compare_opcodes_len4(char a[4], char b[4]) {
	/*
	 * This is a custom strcmp method for 4 character arrays
	 * This is used instead of strcmp because of problems were caused by input strings using NULL values.
	 */
	for (int z=0; z<4; z++) {
		if (a[z] != b[z]) {
			return -1;
		}
	}
	return 0;
}

void randomiseArrays(int size, vocaloid *character) {
	/*
	* Randomises and initialises the vocaloid structures for each part of the module 
	*/
	character->body = (int *) malloc(sizeof(int) * size);
	character->hand = (int *) malloc(sizeof(int) * size);
	character->hair = (int *) malloc(sizeof(int) * size);
	character->excl = (int *) malloc(sizeof(int) * size);
	character->bodycount = size-1;
	character->handcount = size-1;
	character->haircount = size-1;
	character->exclcount = size-1;
	
	for (int i = 0; i < size; i++) {
		character->body[i] = i;
		character->hand[i] = i;
		character->hair[i] = i;
		character->excl[i] = i;
	}
	
	int rand_mod;
	int swap;
	
	for (int i = size - 1; i > 0; i--) {
		//Body Shuffle
		rand_mod = rand() % i;
		swap = character->body[i];
		character->body[i] = character->body[rand_mod];
		character->body[rand_mod] = swap;
		
		//Hand Shuffle
		rand_mod = rand() % i;
		swap = character->hand[i];
		character->hand[i] = character->hand[rand_mod];
		character->hand[rand_mod] = swap;
		
		//Hair Shuffle
		rand_mod = rand() % i;
		swap = character->hair[i];
		character->hair[i] = character->hair[rand_mod];
		character->hair[rand_mod] = swap;
		
		//Exclusive Accessory Shuffle
		rand_mod = rand() % i;
		swap = character->excl[i];
		character->excl[i] = character->excl[rand_mod];
		character->excl[rand_mod] = swap;
	}
}

char *makeModuleString (int rand_vcl, int part_num) {
	/*
	* Makes the char string for the inputted vocaloid and module part number
	*/
	const char *itm = "itm";
	const char *vcl[6] = {"mik", "rin", "len", "luk", "mei", "kai"};
	int num_zeros;
	char part_num_str[3];
	char *module = (char *) calloc(9, (sizeof(char) * 9));
	
	if (part_num < 10) { // Set the number of zeros needed for the module string
		num_zeros = 2;
	} else if (part_num < 100) {
		num_zeros = 1;
	} else {
		num_zeros = 0;
	}
						
	snprintf(part_num_str, 3, "%d", part_num); // Convert mik mod num to string
	strcat(module, vcl[rand_vcl]); // Make the "module" string for checking
	strcat(module, itm);
						
	for (int q=0; q<num_zeros; q++) {
		strcat(module, "0");
	}
						
	strcat(module, part_num_str);
	
	return module;
}

char *checkInPDcodes (char * string) {
	/*
	* Checks to see whether the string exists within the PDcodes.txt. If it exists then return the line from PDcodes where it exists, else return "nothing".
	*/
	FILE *PDcodes;
	PDcodes = fopen("PDcodes.txt", "rb");
	char PDcodes_line[22];
	char *return_PDcodes_line = (char *) calloc(22, (sizeof(char) * 22));
	
	while (strstr(PDcodes_line, "hakitm604") == NULL) {
		fread(&PDcodes_line, sizeof(char), 22, PDcodes);
			
		if (strstr(PDcodes_line, string) != NULL) { // If the code is in PDcodes.txt (i.e it corresponds to a module)
			fclose(PDcodes);
			strcpy(return_PDcodes_line, PDcodes_line);
			return return_PDcodes_line;
		}
	}
	
	fclose(PDcodes);
	strcpy(return_PDcodes_line, "nothing");
	return return_PDcodes_line;
}

char *findModulePart(char *line, vocaloid *mik, vocaloid *rin, vocaloid *len, vocaloid *luk, vocaloid *mei, vocaloid *kai) {
	/*
	* This is the main method used for randomising the modules/module parts. A line from the PDcodes is inputted along with all the vocaloid constructs for each character.
	* First it extracts the module part number from the PDcodes line and then uses this to work out what body part to use later on. After that, it randomises the character
	* and then uses the mod num to find out what part it was previously. It then uses the count (for the respective part) from the vocaloid to index the randomised array.
	* It then checks whether the randomised module actually exists within PDcodes. If it does then use it, else decrease the count and try again.
	*/
	int mod_part_num;
	char mod_part_str[3];
	char *rest_of_line;
	char *random_module;
	char *mod_exists = (char *) calloc(22, (sizeof(char) * 22));
	int exists = 0;
	
	mod_part_str[0] = line[6]; // Extract the integer, from the line, to a smaller string
	mod_part_str[1] = line[7];
	mod_part_str[2] = line[8];
	
	mod_part_num = strtol(mod_part_str, &rest_of_line, 10); // Converts the string to an integer
	
	//printf("mod part: %s\n", line);
	//printf("mod num: %i\n", mod_part_num);
	
	while(exists == 0) {
		
		if (mik->bodycount == 0 && rin->bodycount == 0 && len->bodycount == 0 && luk->bodycount == 0 && mei->bodycount == 0 && kai->bodycount == 0 ) {
			randomiseArrays(150, mik);
			randomiseArrays(150, rin);
			randomiseArrays(150, len);
			randomiseArrays(150, luk);
			randomiseArrays(150, mei);
			randomiseArrays(150, kai);
		}
		
		int rand_vcl = rand() % 6; // Randomise which vocaloid
		
		if (rand_vcl == 0) { // If it's Miku
		
			if (mod_part_num < 200) { // Was the module in firstread a body?
				if (mik->bodycount > 0) {
					random_module = makeModuleString(rand_vcl, (mik->body[mik->bodycount])); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, exit the while and return the PDcode line
						exists = 1;
					}
					(mik->bodycount--); // Decrement the count
				} else {
					continue;
				}	
			} else if (mod_part_num > 400 && mod_part_num < 600) { // Was the module in firstread hands?
				if (mik->handcount > 0) {
					random_module = makeModuleString(rand_vcl, (mik->hand[mik->handcount]+400)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(mik->handcount--); // Decrement the count
				} else {
					continue;
				}
			} else if (mod_part_num > 600 && mod_part_num < 800) { // Was the module in firstread hair?
				if (mik->haircount > 0) {
					random_module = makeModuleString(rand_vcl, (mik->hair[mik->haircount]+600)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(mik->haircount--); // Decrement the count
				} else {
					continue;
				}
			} else if (mod_part_num > 800 && mod_part_num < 1000) { // Was the module in firstread exclusive accessory?
				if (mik->exclcount > 0) {
					random_module = makeModuleString(rand_vcl, (mik->excl[mik->exclcount]+800)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(mik->exclcount--);
				} else {
					continue;
				}
			}
			
		} else if (rand_vcl == 1) { //If it's Rin
		
			if (mod_part_num < 200) { // Was the module in firstread a body?
				if (rin->bodycount > 0) {
					random_module = makeModuleString(rand_vcl, (rin->body[rin->bodycount])); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(rin->bodycount--);
				} else {
					continue;
				}	
			} else if (mod_part_num > 400 && mod_part_num < 600) { // Was the module in firstread hands?
				if (rin->handcount > 0) {
					random_module = makeModuleString(rand_vcl, (rin->hand[rin->handcount]+400)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(rin->handcount--);
				} else {
					continue;
				}
			} else if (mod_part_num > 600 && mod_part_num < 800) { // Was the module in firstread hair?
				if (rin->haircount > 0) {
					random_module = makeModuleString(rand_vcl, (rin->hair[rin->haircount]+600)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(rin->haircount--);
				} else {
					continue;
				}
			} else if (mod_part_num > 800 && mod_part_num < 1000) { // Was the module in firstread exclusive accessory?
				if (rin->exclcount > 0) {
					random_module = makeModuleString(rand_vcl, (rin->excl[rin->exclcount]+800)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(rin->exclcount--);
				} else {
					continue;
				}
			}
		
		} else if (rand_vcl == 2) { //If it's Len
		
			if (mod_part_num < 200) { // Was the module in firstread a body?
				if (len->bodycount > 0) {
					random_module = makeModuleString(rand_vcl, (len->body[len->bodycount])); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(len->bodycount--);
				} else {
					continue;
				}	
			} else if (mod_part_num > 400 && mod_part_num < 600) { // Was the module in firstread hands?
				if (len->handcount > 0) {
					random_module = makeModuleString(rand_vcl, (len->hand[len->handcount]+400)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(len->handcount--);
				} else {
					continue;
				}
			} else if (mod_part_num > 600 && mod_part_num < 800) { // Was the module in firstread hair?
				if (len->haircount > 0) {
					random_module = makeModuleString(rand_vcl, (len->hair[len->haircount]+600)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(len->haircount--);
				} else {
					continue;
				}
			} else if (mod_part_num > 800 && mod_part_num < 1000) { // Was the module in firstread exclusive accessory?
				if (len->exclcount > 0) {
					random_module = makeModuleString(rand_vcl, (len->excl[len->exclcount]+800)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(len->exclcount--);
				} else {
					continue;
				}
			}
		
		} else if (rand_vcl == 3) { //If it's Luka
		
			if (mod_part_num < 200) { // Was the module in firstread a body?
				if (luk->bodycount > 0) {
					random_module = makeModuleString(rand_vcl, (luk->body[luk->bodycount])); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(luk->bodycount--);
				} else {
					continue;
				}	
			} else if (mod_part_num > 400 && mod_part_num < 600) { // Was the module in firstread hands?
				if (luk->handcount > 0) {
					random_module = makeModuleString(rand_vcl, (luk->hand[luk->handcount]+400)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(luk->handcount--);
				} else {
					continue;
				}
			} else if (mod_part_num > 600 && mod_part_num < 800) { // Was the module in firstread hair?
				if (luk->haircount > 0) {
					random_module = makeModuleString(rand_vcl, (luk->hair[luk->haircount]+600)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(luk->haircount--);
				} else {
					continue;
				}
			} else if (mod_part_num > 800 && mod_part_num < 1000) { // Was the module in firstread exclusive accessory?
				if (luk->exclcount > 0) {
					random_module = makeModuleString(rand_vcl, (luk->excl[luk->exclcount]+800)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(luk->exclcount--);
				} else {
					continue;
				}
			}
		
		} else if (rand_vcl == 4) { //If it's Meiko
		
			if (mod_part_num < 200) { // Was the module in firstread a body?
				if (mei->bodycount > 0) {
					random_module = makeModuleString(rand_vcl, (mei->body[mei->bodycount])); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(mei->bodycount--);
				} else {
					continue;
				}	
			} else if (mod_part_num > 400 && mod_part_num < 600) { // Was the module in firstread hands?
				if (mei->handcount > 0) {
					random_module = makeModuleString(rand_vcl, (mei->hand[mei->handcount]+400)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(mei->handcount--);
				} else {
					continue;
				}
			} else if (mod_part_num > 600 && mod_part_num < 800) { // Was the module in firstread hair?
				if (mei->haircount > 0) {
					random_module = makeModuleString(rand_vcl, (mei->hair[mei->haircount]+600)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(mei->haircount--);
				} else {
					continue;
				}
			} else if (mod_part_num > 800 && mod_part_num < 1000) { // Was the module in firstread exclusive accessory?
				if (mei->exclcount > 0) {
					random_module = makeModuleString(rand_vcl, (mei->excl[mei->exclcount]+800)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(mei->exclcount--);
				} else {
					continue;
				}
			}
		
		} else { //If it's Kaito
		
			if (mod_part_num < 200) { // Was the module in firstread a body?
				if (kai->bodycount > 0) {
					random_module = makeModuleString(rand_vcl, (kai->body[kai->bodycount])); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(kai->bodycount--);
				} else {
					continue;
				}	
			} else if (mod_part_num > 400 && mod_part_num < 600) { // Was the module in firstread hands?
				if (kai->handcount > 0) {
					random_module = makeModuleString(rand_vcl, (kai->hand[kai->handcount]+400)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(kai->handcount--);
				} else {
					continue;
				}
			} else if (mod_part_num > 600 && mod_part_num < 800) { // Was the module in firstread hair?
				if (kai->haircount > 0) {
					random_module = makeModuleString(rand_vcl, (kai->hair[kai->haircount]+600)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(kai->haircount--);
				} else {
					continue;
				}
			} else if (mod_part_num > 800 && mod_part_num < 1000) { // Was the module in firstread exclusive accessory?
				if (kai->exclcount > 0) {
					random_module = makeModuleString(rand_vcl, (kai->excl[kai->exclcount]+800)); // Create the module string 
					mod_exists = checkInPDcodes(random_module); // Check if the module string exists
					free(random_module);
					if (strstr(mod_exists, "nothing") == NULL) { // If it exists, decrement the count, exit the while and return the PDcode line
						exists = 1;
					}
					(kai->exclcount--);
				} else {
					continue;
				}
			}

		}
	}
	
	//printf("mik body counter: %i\n", mik->bodycount);
	//printf("rin body counter: %i\n", rin->bodycount);
	//printf("len body counter: %i\n", len->bodycount);
	//printf("luk body counter: %i\n", luk->bodycount);
	//printf("mei body counter: %i\n", mei->bodycount);
	//printf("kai body counter: %i\n", kai->bodycount);
	
	return mod_exists;
}

void FR_rand() {
	
	srand(time(NULL));
	
	FILE *fr, *rand_fr, *ids, *PDcodes;
	
	char ch[4];
	int other_code_pointer = 36204;
	
	char mik1[4] = {'Y', '\\', '<', 'ð'};
	char skip1[4] = {'ÿ', 0, 0, 0};
	char skip3[4] = {1, 0, 0, 0};
	char rinitm619[4] = {'¼', '»', 'v', '¯'};
	char end_rand[4] = {20, 'Ž', 23, 'S'};
	char start_other[4] = {'$', 'R', '<', -123};
			
	int fr_size, code_index = 12, i, j, stop_checking;
	
	fr = fopen("firstread.bin", "rb");
	PDcodes = fopen("PDcodes.txt", "rb");
	rand_fr = fopen("PDRX Files/firstread.bin", "wb");
	ids = fopen("temp_ids.bin", "wb");
	
	vocaloid miku;
	vocaloid rin;
	vocaloid len;
	vocaloid luka;
	vocaloid meiko;
	vocaloid kaito;
	randomiseArrays(150, &miku);
	randomiseArrays(150, &rin);
	randomiseArrays(150, &len);
	randomiseArrays(150, &luka);
	randomiseArrays(150, &meiko);
	randomiseArrays(150, &kaito);
	
	if (fr == NULL) {
      perror("Error while opening firstread.bin.\n");
      exit(EXIT_FAILURE);
	}
	
	fseek(fr, 0L, SEEK_END);
	fr_size = ftell(fr);
	rewind(fr);
	
	for (i=0; i<fr_size; i+=4) {
		fread(&ch, sizeof(char), 4, fr);

		if (compare_opcodes_len4(ch, mik1) == 0) {
			fseek(fr, -strlen(ch), SEEK_CUR);
			//fprintf(stderr, "part 1 ok!\n");
			break;
		}
		
		fwrite(&ch, sizeof(char), 4, rand_fr);
	}

	fr_size = fr_size - i;

	for (i=0; i<fr_size; i+=4) {
		fread(&ch, sizeof(char), 4, fr);
		
		if (compare_opcodes_len4(ch, end_rand) == 0) {
			//printf("stop!\n");
			fseek(fr, -strlen(ch), SEEK_CUR);
			break;
		
		} else if (compare_opcodes_len4(ch, skip1) == 0 || strcmp(ch, skip3) == 0) {
			//printf("skipping\n");
			other_code_pointer -= 4;
			fwrite(&ch, sizeof(char), 4, rand_fr);
			continue;
		
		}

		//printf("input ch: %s\n", ch);
		char *PDcodes_checker = checkInPDcodes(ch);
		if (strstr(PDcodes_checker, "nothing") == NULL && stop_checking != 1) {
			
			if (strstr(PDcodes_checker, "meiitm908") != NULL) {
				stop_checking = 1;
			}
			
			char *thing = (char *) calloc(22, (sizeof(char) * 22));
			thing = findModulePart(PDcodes_checker, &miku, &rin, &len, &luka, &meiko, &kaito);
			//printf("outer: %s\n", thing);
				
			ch[0] = thing[code_index];
			ch[1] = thing[code_index+1];
			ch[2] = thing[code_index+2];
			ch[3] = thing[code_index+3];
				
			fwrite(&ch, sizeof(char), 4, rand_fr);
				
			ch[0] = thing[code_index+4];
			ch[1] = thing[code_index+5];
			ch[2] = thing[code_index+6];
			ch[3] = thing[code_index+7];
				
			fwrite(&ch, sizeof(char), 4, ids);
			
				
			if (strstr(PDcodes_checker, "rinitm618") != NULL) {
				//printf("yay!");
				
				fwrite(&skip1, sizeof(char), 4, rand_fr);
				fwrite(&ch, sizeof(char), 4, ids);
				fwrite(&rinitm619, sizeof(char), 4, ids);
				
				ch[0] = thing[code_index];
				ch[1] = thing[code_index+1];
				ch[2] = thing[code_index+2];
				ch[3] = thing[code_index+3];
				
				fwrite(&ch, sizeof(char), 4, rand_fr);
				
				fread(&ch, sizeof(char), 4, fr);
				fread(&ch, sizeof(char), 4, fr);
			}
		
		} else {
			fwrite(&ch, sizeof(char), 4, rand_fr);
			fseek(fr, other_code_pointer, SEEK_CUR);
			fread(&ch, sizeof(char), 4, fr);
			fwrite(&ch, sizeof(char), 4, ids);
			fseek(fr, -other_code_pointer-4, SEEK_CUR);
		}
		
		//printf("pointer to ids: %i\n", other_code_pointer);
		//printf("---\n");
		//printf("i = %i and fr_size = %i\n", i, fr_size);
	}
	
	fr_size = fr_size - i;

	for (i=0; i<fr_size; i+=4) {
		fread(&ch, sizeof(char), 4, fr);
		
		if (strcmp(ch, start_other) == 0) {
			fseek(fr, -strlen(ch), SEEK_CUR);
			//printf("part 2 ok!\n");
			break;
		}
		
		fwrite(&ch, sizeof(char), 4, rand_fr);
	}
	
	fclose(ids);
	
	ids = fopen("temp_ids.bin", "rb");
	fseek(ids, 0L, SEEK_END);
	int ids_size = ftell(ids);
	rewind(ids);
	
	for (j=0; j<ids_size; j+=4) {
		fread(&ch, sizeof(char), 4, fr);
		//printf("fr = %s\n", ch);
		i+=4;
		fread(&ch, sizeof(char), 4, ids);
		//printf("ids = %s\n", ch);
		fwrite(&ch, sizeof(char), 4, rand_fr);	
	}
	
	fr_size = fr_size - i;

	for (i=0; i<fr_size; i+=4) {
		fread(&ch, sizeof(char), 4, fr);
		fwrite(&ch, sizeof(char), 4, rand_fr);
	}
	
	printf("Finished!");
	fclose(ids);
	fclose(fr);
	fclose(PDcodes);
	fclose(rand_fr);
	remove("temp_ids.bin");
}
