/*Samira C. Oliva Madrigal
CmpE 152
Fall 2015
Homework 9

Optimize the following code in C/C++. Do not unroll loops unless asked.
*/
//A.
int a[256], b[256];
for(int i = 0; i < 256; i++)
{
    if(i < 128)
        a[i] = b[i] + 2;
    else
        a[i] = b[i] + 1;
}

//Optimized Version:
//A ternary operator will improve performance; pointerized to 
//eliminate basic induction variables 
// *(a + i) = (i < 128? (*(b + 1) + 2):(*(b + 1) + 1));
// i++;


int a[256], b[256]; 
int *aPtr = a;
int *bPtr = b;
while(aPtr < a + 256)
{
 	 *aPtr  = (aPtr < a + 128? ((*bPtr) + 2):((*bPtr) + 1));
	 aPtr++; bPtr++;
}

another way to iterate body using index i:
// *(a + i) = (i < 128? (*(b + 1) + 2):(*(b + 1) + 1));
// i++;



Performance for A:
real	0m0.698s
user	0m0.636s
sys		0m0.000s

Optimized Version:
real	0m0.004s
user	0m0.000s
sys		0m0.005s



Testing Code Used:



int main(void)
{
  int j;
  int a[256], b[256]; //without need to use a pointer to a
/*
   int i; 
   for(j = 0; j < 1000000; j++)
	for(i = 0; i < 256; i++)
	{
	    if(i < 128)
		a[i] = b[i] + 2;
	    else
		a[i] = b[i] + 1;
	}

*/
   
   int *aPtr = a;
   int *bPtr = b;
   //without need to use a pointer to a
   for(j = 0; j < 1000000; j++)
   {
	while(aPtr < a + 256)
	{
	 	 *aPtr  = (aPtr < a + 128? ((*bPtr) + 2):((*bPtr) + 1));
		 aPtr++; bPtr++;
	}
    }


return 0;
}


COMMANDS:

$ gcc -o hw9 test.c 
$ time ./hw9

//B.
int a[256], b[1280];
for(int i = 0; i < 256; i++)
{
    a[i] = b[5*i];
}

//Optimized:
//Induction variable elimination of 5*i and pointerized version to remove any basic 
//induction variables
 
//address calculation in C this is invalid *(&a + 4*i)
//compiler already accounts for sizeof(int)


int a[256], b[1280];

    int s = 0;
    int *ptrA, *ptrB;
    ptrA = a;
    ptrB = b;

	    while(ptrA < a + 256)
	    {
		
		    *ptrA = *ptrB + s;
		    ptrA++;
		    s += 5;
	    }


Performance for B:
real	0m0.689s
user	0m0.659s
sys		0m0.000s


Optimized Version:
real	0m0.003s
user	0m0.003s
sys	    0m0.000s


Testing Code Used:

int main(void)
{
  int j, i;
  int a[256], b[1280];
/*
  for(j = 0; j < 1000000; j++)
  {
	for(i = 0; i < 256; i++)
	{
	    a[i] = b[5*i];
	}
  }
*/
    
    int s = 0;
    int *ptrA, *ptrB;
    ptrA = a;
    ptrB = b;
    for(j = 0; j < 1000000; j++)
   {
	    while(ptrA < a + 256)
	    {
		
		    *ptrA = *ptrB + s;
		    ptrA++;
		    s += 5;
	    }
    }

	return 0;
}


//C.
int a, b[256];
a = 0;
for(int i = 0; i < 256; i++)
{
    a += 7*b[i] ;
}

// Optimized:
// we do not remove the computation 7 *b[i] because
// it is not of form i*7; the multiplication result directly 
// depends on the value of b[i];
// pointerize and remove induction variable i 
int a, b[256];
a = 0;
int *ptrB = b;
        
while (ptrB < b + 256)
{
    a += a + (*ptrB)*7 ;
    ptrB++;
}


Performance for C:
real	0m0.640s
user	0m0.620s
sys		0m0.004s


Optimized Version:
real	0m0.003s
user	0m0.000s
sys	    0m0.003s


Testing Code Used:

int main(void)
{
  int j;
  
  int a, b[256];
  a = 0;
/*
  int i;
  for(j = 0; j < 1000000; j++)
  {
  
	for(i = 0; i < 256; i++)
	{
   		 a += 7*b[i] ;
	}

  }

*/

   int *ptrB = b;
   for(j = 0; j < 1000000; j++)
  {
 
    while (ptrB < b + 256)
    {
	    a += a + (*ptrB)*7 ;
	    ptrB++;
    }

  }
	return 0;
}


//D.
void swap(int& a, int& b)
{
    int temp;
    temp = a;
    a = b;
    b = temp;
}

void swaparray(int a[], int b[])
{
    for(int i = 0; i < 256; i++)
        swap(a[i], b[i]);
}

//Optimized: remove an induction variable; pointerize
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swaparray(int a[], int b[])
{
    
    int *ptrA = a;
    int *ptrB = b;
       while(ptrA < a + 256)
       {
            swap(*ptrA, *ptrB);
             ptrA++;ptrB++;
       }
}

Performance for D:
real	0m2.922s
user	0m1.276s
sys		0m0.046s


Optimized Version: (user time varies, took lowest of several runs)
real	0m1.198s
user	0m1.176s
sys		0m0.001s

Testing Code Used:

#include <iostream>

using namespace std;

/*
void swap(int& a, int& b)
{
    int temp;
    temp = a;
    a = b;
    b = temp;
}

void swaparray(int a[], int b[])
{
    for(int i = 0; i < 256; i++)
        swap(a[i], b[i]);
}
*/

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swaparray(int a[], int b[])
{
    
    int *ptrA = a;
    int *ptrB = b;
       while(ptrA < a + 256)
       {
            swap(*ptrA, *ptrB);
            ptrA++;ptrB++;
       }

}

int main(void)
{
  int j;
  int a[256], b[256];
  for(j = 0; j < 1000000; j++)
  {
     swaparray(a,b);
  }

	return 0;
}

//E.
int a[256], a2, b[256];
cin >> a2;
for(int i = 0; i < 256; i++)
{
    a[i] = 7*a2*b[i] ;
}


//Optimized:
remove loop invariant 7*a2 and pointerize (eliminate basic induction variable)
    int a[256], b[256], a2;
    int *ptrA, *ptrB;
    ptrA = a;
    ptrB = b;

    cin >> a2;
    int const1 = 7 * a2;
    
        
        while (ptrA < a + 256)
        {
            *ptrA = (*ptrB)*const1;
            ptrA++; ptrB++;
        }

Performance for E:
real	0m0.547s
user	0m0.525s
sys	    0m0.000s

Optimized Version: 
real	0m0.008s
user	0m0.006s
sys  	0m0.000s

Testing Code Used:
#include <iostream>

using namespace std;

int main(void)
{
  int j;
  int a[256], b[256], a2;
  a2 = 2;
/* 
  for(j = 0; j < 1000000; j++)
  {

     for(int i = 0; i < 256; i++)
	{
   	   a[i] = 7*a2*b[i] ;
	}
  }
*/
    int *ptrA, *ptrB;
    ptrA = a;
    ptrB = b;
    int const1 = 7 * a2;
    
    for(j = 0; j < 1000000; j++)  
    {
        while (ptrA < a + 256)
        {
            *ptrA = (*ptrB)*const1;
             ptrA++; ptrB++;
        }

     }


	return 0;
}
