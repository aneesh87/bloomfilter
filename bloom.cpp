#include <string.h>
#include <stdio.h>
#include <bitset>
#include <iostream>
#include <cstdlib>

#define ARR_LENGTH 8
#define BLOOMSETSIZE 2000000

typedef struct bf_t{

  std::bitset<BLOOMSETSIZE> bloomFilterSet;

} bf_t;

unsigned int primes[ARR_LENGTH] = { 1540655027, 1840695761, 1740683437, 1840701563, 
                                    1640680681, 1940700029, 2100003041, 2040668849
                                  };

/*
	Hash Function used 
   Reference MurmurHash2 -- Austin Appleby
   https://github.com/aappleby/smhasher/blob/master/src/MurmurHash2.cpp
*/

int MurmurHash2(const void * key, int len, unsigned int seed) {
   // Note: m,r (or referred as a,b) taken strictly less than set of prime numbers

   // 'm' and 'r' are mixing constants generated offline.
   // They're not really 'magic', they just happen to work well.

   const int m = 1540483477;
   const int r = 24;
   // Initialize the hash to a 'random' value
   int h = seed ^ len;
   // Mix 4 bytes at a time into the hash
   const unsigned char * data = (const unsigned char *)key;
   while(len >= 4) {
		int k = *(int *)data;
		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h *= m; 
		h ^= k;
		data += 4;
		len -= 4;
	}
	// Handle the last few bytes of the input array
	switch(len) {
      case 3: h ^= data[2] << 16;
      case 2: h ^= data[1] << 8;
      case 1: h ^= data[0];
              h *= m;
	};
	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.
	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;
	return h%BLOOMSETSIZE;
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
     int pos = MurmurHash2(s, strlen(s), primes[j]);
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
      int pos = MurmurHash2(q, strlen(q), primes[j]);
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
