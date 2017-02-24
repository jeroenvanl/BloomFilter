/*COMSM1201 Student Jeroen van Lith


Week 10-11: Hashing Extension: B L O O M   F I L T E R S

An interesting antidote to hashing is the use of a Bloom filter,
a probalistic data structure which does not store the actual keys,
and consequently is much more efficient with memory.

It does so by creating a large array of bits, which is loaded with 
(char) '1' if a hash function for a key returns that position. When you 
do this for multiple hash functions (but not too many), then you create 
a vector of positions in which the key is stored. When you then want to 
check whether a key is in the dictionary, you simply check whether all 
of the bits from the hash functions are turned on. Then you know (with 
a small margin for error) that the word is in the dictionary. This is 
useful when you want to check something against a large set of data, 
such as a dictionary.

The trade-off is that you can never be 100% sure whether a word is
in the dictionary, you can only be sure when it is not. In this example,
I set the false-positive rate to be .01, which means I want to be 99% sure
a word is in the dictionary.

The Bloom Filter is inspired by this paper: Kirsch, Adam, and Michael Mitzenmacher.
"Less hashing, same performance: Building a better Bloom filter."
European Symposium on Algorithms. Springer Berlin Heidelberg, 2006.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define FALSEPOSITIVE .01 /*Acceptable rate of false-positives according to the user */
#define NUMALLOWEDARGS 3
#define BUFF 46 /* Pneumonoultramicroscopicsilicovolcanoconiosis[46] is the longest word in big English dictionaries */ 
#define READFILE fscanf(fp,"%s",word)==1
#define MAGICNUM 5381
#define HASHNUM 5
#define strsame(A,B) (strcmp(A, B)==0) /* From the COMSM1201 hand-out. Thanks! */
#define ERROR(PHRASE) {fprintf(stderr,\
"Fatal Error %s occured in %s,line %d\n"\
,PHRASE, __FILE__, __LINE__); exit(2);} /* Also from the COMSM1201 hand-out. Thanks! */
#define TRUE '1'


typedef struct hashtable{
   char *table;
   int size_dictionary;
   int size_table;
   int size_lookups;
   int n_func;
   float false_positive;
}Hashtable;


/* 1. First checks */
void checkCount(int argc);
FILE *openFile(char *filename);

/* 2. Create the hash table */
int getLength(char *file);
void initializeTable(Hashtable *h, char *file);
void bloomSize(Hashtable *h);
void bloomFunc(Hashtable *h);
void readIn(Hashtable *h, char *dic);
void insert(Hashtable *h, char *word);

/*3. Hash functions */
unsigned int hashDjb2(char *str);

/* 4. Calculate results and free memory */
void lookUp(Hashtable *h, char *file);
void freeArray(Hashtable *h);


int main(int argc, char **argv)
{
   Hashtable h;
   checkCount(argc);
   initializeTable(&h,argv[1]);
   readIn(&h,argv[1]);
   lookUp(&h,argv[2]);
   return 0;
}


void initializeTable(Hashtable *h, char *file)
{
   h->size_lookups=0;
   h->false_positive=FALSEPOSITIVE;
   h->size_dictionary=getLength(file);
   bloomSize(h);
   h->table=calloc(h->size_table,sizeof(char));
   bloomFunc(h);
}


void checkCount(int c)
{
   if(c<NUMALLOWEDARGS){
      ERROR("Please type two filenames!\n")
   }
   else if(c>NUMALLOWEDARGS){
      ERROR("Please type only two filenames!\n")
   }
}


void bloomSize(Hashtable *h)
{
   h->size_table=ceil((h->size_dictionary*log(FALSEPOSITIVE))/log(1/(pow(2,log(2)))));
}


void bloomFunc(Hashtable *h)
{
   h->n_func=(h->size_table*log(2))/h->size_dictionary;
}


FILE *openFile(char *filename)
{
   FILE *fp;
   fp=fopen(filename,"r");
   if(fp==NULL){
      ERROR("Failed to open file\n")
   }
   return fp;
}


int getLength(char *file)
{
   int i=0;
   char word[BUFF];
   FILE *fp;
   fp=openFile(file);
   while(READFILE){
      i++;
   }
   fclose(fp);
   return i;
}


void readIn(Hashtable *h, char *dic) 
{
   char word[BUFF];
   FILE *fp;
   fp=openFile(dic);	
   while(READFILE){
      insert(h,word);	
   }
   fclose(fp);
}


void insert(Hashtable *h, char *word)
{
   int i,loc;
   for(i=0;i<h->n_func;i++){
      loc=(hashDjb2(word)*(i+1))%h->size_table;
      if(!h->table[loc]){
         h->table[loc]=TRUE;
      }
   }
}


/* Hash function by Dan Berstein. */
/* Retrieved from http://www.cse.yorku.ca/~oz/hash.html */
unsigned int hashDjb2(char *str) 
{
   unsigned long hash=MAGICNUM;
   while(*str){
      hash+=(hash<<HASHNUM)+*str++; 
   }
   return hash;
}


void lookUp(Hashtable *h, char *file)
{
   int i,l,c=0,loc;
   char word[BUFF];
   FILE *fp;
   fp=openFile(file);
   while(READFILE){
      h->size_lookups++;
      l=0;
      for(i=0;i<h->n_func;i++){
         loc=(hashDjb2(word)*(i+1))%h->size_table;
         if(h->table[loc]==TRUE){
            l++;
         }
      }
      if(l==h->n_func){
         c++;
      }
   }
   printf("%d of the %d words are in the dictionary!",c,h->size_lookups);
   fclose(fp);
}


void freeArray(Hashtable *h)
{
   free(h->table);
}
