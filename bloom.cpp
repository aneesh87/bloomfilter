#include <string.h>
#include <stdio.h>
#include <bitset>
#include <iostream>
#include <cstdlib>

#define ARR_LENGTH 8
#define BLOOMSETSIZE 2000000

#define MAX_PRIME 2100003041

typedef struct bf_t{

  std::bitset<BLOOMSETSIZE> bloomFilterSet;
  unsigned int alist[8];  // List of a values
  unsigned int b;         // b values

} bf_t;

unsigned int primes[ARR_LENGTH] = { 1540655027, 1840695761, 1740683437, 1840701563, 
                                    1640680681, 1940700029, 2100003041, 2040668849
                                  };



unsigned int customhash (void * key, int len, unsigned int seed, unsigned int * alist, unsigned int b) {

   unsigned int sum = b;
   unsigned int i = 0;
   char * hkey = (char*) key;
   while( *hkey != '\0')
   { 
      sum += (alist[i%8]* ((unsigned int) *hkey))%seed;
      hkey += 1;
      i++;
   }
   return (sum%BLOOMSETSIZE);
}

/*
 * Function: static_list
 * Function to assign static values to a list and b
 * This is to get a fixed error rate.
 * Code to randomly generate is available (see below)
 *
 * 
 * Return: None
 */

void static_list(bf_t * B) {

   B[0].alist[0] = 16807;
   B[0].alist[1] = 282475249;
   B[0].alist[2] = 1622650073;
   B[0].alist[3] = 984943658;
   B[0].alist[4] = 1144108930;
   B[0].alist[5] = 470211272;
   B[0].alist[6] = 101027544;
   B[0].alist[7] = 1457850878;
   B[0].b        = 1458777923;

   B[1].alist[0] = 2007237709;
   B[1].alist[1] = 823564440;
   B[1].alist[2] = 1115438165;
   B[1].alist[3] = 1784484492;
   B[1].alist[4] = 74243042;
   B[1].alist[5] = 114807987;
   B[1].alist[6] = 1137522503;
   B[1].alist[7] = 1441282327;
   B[1].b = 16531729;

   B[2].alist[0] = 823378840;
   B[2].alist[1] = 143542612;
   B[2].alist[2] = 896544303;
   B[2].alist[3] = 1474833169;
   B[2].alist[4] = 1264817709;
   B[2].alist[5] = 1998097157;
   B[2].alist[6] = 1817129560;
   B[2].alist[7] = 1131570933;
   B[2].b = 197493099;

   B[3].alist[0] = 1404280278;
   B[3].alist[1] = 893351816;
   B[3].alist[2] = 1505795335;
   B[3].alist[3] = 1954899097;
   B[3].alist[4] = 1636807826;
   B[3].alist[5] = 563613512;
   B[3].alist[6] = 101929267;
   B[3].alist[7] = 1580723810;
   B[3].b = 704877633;

   B[4].alist[0] = 1358580979;
   B[4].alist[1] = 1624379149;
   B[4].alist[2] = 28233538;
   B[4].alist[3] = 784558821;
   B[4].alist[4] = 530511967;
   B[4].alist[5] = 10007631;
   B[4].alist[6] = 1551901393;
   B[4].alist[7] = 1617819336;
   B[4].b = 1399125485;

   B[5].alist[0] = 156091745;
   B[5].alist[1] = 1356425228;
   B[5].alist[2] = 1899894091;
   B[5].alist[3] = 585640194;
   B[5].alist[4] = 937186357;
   B[5].alist[5] = 1646035001;
   B[5].alist[6] = 1025921153;
   B[5].alist[7] = 510616708;
   B[5].b = 590357944;

   B[6].alist[0] = 771515668;
   B[6].alist[1] = 357571490;
   B[6].alist[2] = 1044788124;
   B[6].alist[3] = 1927702196;
   B[6].alist[4] = 1952509530;
   B[6].alist[5] = 130060903;
   B[6].alist[6] = 1942727722;
   B[6].alist[7] = 1083454666;
   B[6].b = 1108728549;

   B[7].alist[0] = 685118024;
   B[7].alist[1] = 18794760;
   B[7].alist[2] = 1060806853;
   B[7].alist[3] = 571540977;
   B[7].alist[4] = 194847408;
   B[7].alist[5] = 2035308228;
   B[7].alist[6] = 158374933;
   B[7].alist[7] = 1075260298;
   B[7].b = 824938981;

}

/*
 * Function: reset_bloom
 *
 * Function to reset all bits in bloom filter to 0
 *
 * Return Value: None
 */

void reset_bloom(bf_t * t) {
     for(int j=0; j<ARR_LENGTH; j++)
        t[j].bloomFilterSet.reset();
}
/* 
 * Function: create_bf()
 *
 * Function to create a new Bloom filter 
 * 
 * Return Value: Reference to newly created bloom filter
 */

bf_t * create_bf() {
   bf_t * BloomFilter = new bf_t[ARR_LENGTH];
   reset_bloom(BloomFilter); 

/*

// Random generation code
// Uncomment this code to generate 'a' list values and 'b' values randomly 
//


   for(int j=0; j<8; j++) {
       BloomFilter[j].alist[0] = rand()%MAX_PRIME;
       BloomFilter[j].alist[1] = rand()%MAX_PRIME;
       BloomFilter[j].alist[2] = rand()%MAX_PRIME;
       BloomFilter[j].alist[3] = rand()%MAX_PRIME;
       BloomFilter[j].alist[4] = rand()%MAX_PRIME;
       BloomFilter[j].alist[5] = rand()%MAX_PRIME;
       BloomFilter[j].alist[6] = rand()%MAX_PRIME;
       BloomFilter[j].alist[7] = rand()%MAX_PRIME;
       BloomFilter[j].b = rand()%MAX_PRIME;
   }
   
*/


 // static list for a fixed error rate

   static_list(BloomFilter);

   return BloomFilter;	
}

/* 
 * Function: insert_bf
 *
 * Function to insert the input string into the given Bloom filter 
 *
 * Return Value: None
 */
void insert_bf(bf_t *b, char *s) {
	for(int j=0; j<ARR_LENGTH; j++)
   { 
     unsigned int pos = customhash(s, strlen(s), primes[j], b[j].alist, b[j].b);
     //std::cout<<pos;
	  b[j].bloomFilterSet.set(pos);
	}
}

/* 
 * Function: is_element
 * Function to test whether the given string is present in bloom filter.
 * 
 * Return Value: If in any of 8 array bloom element sets, the bit for string 
 * is not set it returns 0, otherwise it will return 1
 */  
int is_element(bf_t *b, char *q) {
	for(int j=0; j<ARR_LENGTH; j++) 
   {  
      unsigned int pos = customhash(q, strlen(q), primes[j], b[j].alist, b[j].b);
	   if(b[j].bloomFilterSet.test(pos)==0) return 0;
	}
	return 1;
}

void sample_string_A(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'a';
   s[4] = 'b';
   s[5] = (char)(1 + ((i*(i-3)) %217));
   s[6] = (char)(1 + ((17*i+129)%233 ));
   s[7] = '\0';
}
void sample_string_B(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'a';
   s[4] = (char)(1 + ((i*(i-3)) %217));
   s[5] = (char)(1 + ((17*i+129)%233 ));
   s[6] = '\0';
}
void sample_string_C(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = 'a';
   s[3] = (char)(1 + ((i*(i-3)) %217));
   s[4] = (char)(1 + ((17*i+129)%233 ));
   s[5] = '\0';
}
void sample_string_D(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'b';
   s[4] = 'b';
   s[5] = (char)(1 + ((i*(i-3)) %217));
   s[6] = (char)(1 + ((17*i+129)%233 ));
   s[7] = '\0';
}
void sample_string_E(char *s, long i)
{  s[0] = (char)(1 + (i%254));
   s[1] = (char)(1 + ((i/254)%254));
   s[2] = (char)(1 + (((i/254)/254)%254));
   s[3] = 'a';
   s[4] = (char)(2 + ((i*(i-3)) %217));
   s[5] = (char)(1 + ((17*i+129)%233 ));
   s[6] = '\0';
}



int main()
{  long i,j; 
   bf_t * bloom;
   bloom = create_bf();
   printf("Created Filter\n");
   for( i= 0; i< 1450000; i++ )
   {  char s[8];
      sample_string_A(s,i);
      insert_bf( bloom, s );
   }
   for( i= 0; i< 500000; i++ )
   {  char s[7];
      sample_string_B(s,i);
      insert_bf( bloom, s );
   }
   for( i= 0; i< 50000; i++ )
   {  char s[6];
      sample_string_C(s,i);
      insert_bf( bloom, s );
   }
   printf("inserted 2,000,000 strings of length 8,7,6.\n");
   
   for( i= 0; i< 1450000; i++ )
   {  char s[8];
      sample_string_A(s,i);
      if( is_element( bloom, s ) != 1 )
   {  printf("found negative error (1)\n"); exit(0); }
   }
   for( i= 0; i< 500000; i++ )
   {  char s[7];
      sample_string_B(s,i);
      if( is_element( bloom, s ) != 1 )
   {  printf("found negative error (2)\n"); exit(0); }
   }
   for( i= 0; i< 50000; i++ )
   {  char s[6];
      sample_string_C(s,i);
      if( is_element( bloom, s ) != 1 )
   {  printf("found negative error (3)\n"); exit(0); }
   }
   j = 0;
   for( i= 0; i< 500000; i++ )
   {  char s[8];
      sample_string_D(s,i);
      if( is_element( bloom, s ) != 0 )
   j+=1;
   }
   for( i= 0; i< 500000; i++ )
   {  char s[7];
      sample_string_E(s,i);
      if( is_element( bloom, s ) != 0 )
   j+=1;
   }
   printf("Found %d positive errors out of 1,000,000 tests.\n",j);
   printf("Positive error rate %f\%.\n", (float)j/10000.0);

} 
