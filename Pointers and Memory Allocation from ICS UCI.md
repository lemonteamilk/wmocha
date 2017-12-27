# Pointers and Memory Allocation 

[Original article link](https://www.ics.uci.edu/~dan/class/165/notes/memory.html)

When declaring a variable, the type given is the type of any expression which looks like the declaration. Thus, if we have the declarations

`int a, *b, c[], *(*d[])();`

then, in the code, the expressions `a, *b, c[] and *(*d[])()` would all evaluate to an integer.

Encountering the declaration, you might have a hard time figuring out that d is an array of pointers to functions which return integer pointers, but you do know what type it will evaluate to when used in the context given. 

Thus you know that the statement `a = *(*d[5])(x, y)` will place an integer in a, even if you are not sure what happened. 

You could similarly match types by stripping off matching levels of indirections:  `b = (*d[5])(x, y)` would store an integer pointer in b rather than the value of the integer.

Although the expression given in the declaration is generally the correct way to use the variable, the relation between pointers and arrays allows for other uses.

Since an array name is just a pointer, we can actually use the expressions `b[]` and `*c` as well.  (Using the alternate notation is often confusing but occasionally more clear.)

For example, the sixth element of an array, declared by either of the two methods mentioned above, can be accessed in either of the two following methods:

`b[5]`   or   `*(b+5)`

(Recall that the monadic "\*" operator merely takes the value at the right and performs one level of indirection on it.)

The second method adds `5*(size of the array element type)` to the address of array, resulting in a pointer to the sixth element, and then the "\*" causes an indirection on that address, resulting in the value stored there.

The subscripted notation is merely shorthand for this.

For a lab, your testing programs might know that the input is restricted to 1000 digits, but your multiplier should not know this.  Thus, you will need to allocate one-dimensional arrays of unpredictable size within these functions.  To do this, use the system library function `malloc` which will give you a specified number of contiguous bytes of memory.

You must first, in your declarations, tell the compiler the type of the return value of `malloc` with the declaration (along with your variable declarations):

`char *malloc();`

Now, let's say you want an array of 10 integers.  Let A be an integer pointer (declared `int *A`).  To get the array, use the command:

`A = (int *) malloc( 10 * sizeof(int) );`

The `sizeof()` function is expanded by the compiler to be the number of bytes in one element of the type given as the argument.

Thus, if there are 4 bytes in an integer, `malloc` will return 40 bytes (beginning on a double-word boundary to guarantee proper alignment of multiple-byte objects -- you don't need to worry about this).

The `(int *)` preceeding the function call is called a cast. This changes the type of the return of `malloc` (which is a character pointer) into an integer pointer so that it may be stored in `A`.

Often casts only affect the compiler's internal representation of what a type is but occasionally they generate code to physically modify data, so you should almost always use them when mixing things of differing types.

Always make sure, after calling `malloc`, that you actually got the space you requested.  `malloc` will return a null pointer if it could not get you the space.

A null pointer can be represented in your code as a zero, or as `NULL` if you include the standard I/O file `<stdio.h>`.

Finally, the function free is used to return space to the operating system which was allocated by `malloc`. 

You should give, as a parameter, to free exactly the pointer given to you by `malloc`.  You cannot give back part of an allocation.

Thus there should be exactly as many calls to free as to `malloc`, and they should involve exactly the same blocks of memory.

# Multi-dimensional arrays

C uses two implementations of arrays, depending on the declaration. They are the same for one dimension, but different for more dimensions.

For example, if an array is declared as

`int array[10][20][30]; `

than there are exactly 6000 ints of storage allocated, and a reference of the form `array[i][j][k]` will be translated to

`*( array + i*20*30 + j*30 + k ) `

which calculates the correct offset from the pointer "array", and then does an indirection on it.

To pass an array of this type to a procedure, you must declare the parameter as

`proc( arg ) int arg[][20][30]; `

(You may declare the value of the first dimension, but the compiler doesn't care since it is not needed.)

You will probably not be using this kind of array, since you don't have constant bounds on the sizes of any of your arrays.

The second type of array is a pointer-vector structure, where each dimension is represented by a vector of pointers of objects of the next dimension, except the last dimension, which consists of arrays of data.
This sounds like a mess, but it really isn't:

If a three-dimensional array is declared as

`int ***array;`

(and we will assume for the moment that it has been allocated space for a $10\times20\times30$ array), then there is an array of 10 pointers to pointers to ints, 10 arrays of 20 pointers to ints, and 6000 ints.

The 200 elements of the 10 arrays each point to a block of 30 ints, and the 10 elements of the one array each point to one of the 10 arrays.  The array variable points to the head of the array with 10 elements.

In short, `array` points to a pointer to a pointer to an integer, `*array` points to a pointer to an integer, `**array` points to an integer, and `***array` is an integer.

In this case, an access of the form `array[i][j][k]` results in an access of the form

`*( *( *(array+i) + j ) + k ) `

Which means:  *Take a pointer to the main array, add* `i` *to offset to the pointer to the correct second dimension array and indirect to it.*

Now we have a pointer to one of the arrays of 20 pointers, and we add `j` to get the offset to the next dimension, and we do an indirection on that.

We now have a pointer to an array of 30 integers, so we add `k` to get a pointer to the desired integer, do an indirection, and we have the integer.

Passing arrays of this type is simple, you declare the parameter the same way, as `int ***arrayname`.

Now the fun begins:  **how to allocate memory for a pointer-vector array.** 

We get memory with the function

`char *malloc( nbytes ); `

`malloc` returns a character pointer to a contiguous block of *nbytes* bytes, or a `NULL` pointer (`NULL` is defined in the library package `<stdio.h>`) if it cannot get the space.

As before, we will assume that the variable is defined as

`int ***array; `

and we want the dimensions to be $10\times20\times30$ (all of the stuff below could be done for an arbitrary `i`,`j`,`k`, which is closer to what you need).

First, we need an array of 10 `int **`'s, so we use the following:

`array = (int ***) malloc( 10 * sizeof(int **) );`

The `sizeof` function returns an integer telling how many bytes are needed by something of type `int **`, and we need 10 of them.

The `(int ***)` is a cast which changes the pointer type from `char *` to `int ***`, to keep the types correct.

Don't forget that, after this call to `malloc`, you should check to see `if array==NULL`.

Note:  `malloc` is asked for 10 `int **`'s but its return is a pointer to them, so the result is an `int ***`.

Now that we have the 10 pointers, we can get the next level of pointers:
```C
for ( i = 0 ; i < 10 ; ++i ) {
	   array[i] = (int ** ) malloc ( 20 * sizeof ( int * ));
}
```
And finally, we can fill in each of these pointers with an array of 30 integers:
```C
for ( i = 0 ; i < 10 ; ++i ) {
	    for ( j = 0 ; j < 20 ; ++j ) {
		array[i][j] = (int * ) malloc( 30 * sizeof (int) );
	    }
	}
```
Again, remember that each call to `malloc` must check the result.

Also note that we could have put the two steps above together, filling each set of 20 pointers as we get them.

It is much more efficient to combine all similar allocations and divide up the memory after getting it.  (It is also much easier to make mistakes.)

When you've convinced yourself the the following works, you will understand C pointers fairly well.

```C
array = (int *** ) malloc( 10 * sizeof(int ** ) );
	array[0] = (int ** ) malloc( 10 * 20 * sizeof( int * ) );
	array[0][0] = (int * ) malloc( 10 * 20 * 30 * sizeof(int) );
	for ( j = 1  ;  j < 20  ;  ++j ) {
	    array[0][j] = array[0][j-1] + 30;
	}
	for ( i = 1  ;  i < 10  ;  ++i ) {
	    array[i] = array[i-1] + 20;
	    array[i][0] = array[i-1][20-1] + 30;
	    for ( j = 1  ;  j < 20  ;  ++j ) {
		array[i][j] = array[i][j-1] + 30;
	    }
	}
```
This is the method you should use, but you will probably only need two dimensional arrays, which are far easier once you understand this example.

Space is returned to the system with the command

`free( pointer ); `

For returning the space to the system, you always return exactly what you were given, i.e., the exact pointer that `malloc` gave you.

You cannot return a portion of an allocation.

Thus there should be a perfect one-to-one correspondence between calls to `malloc` and calls to `free`.


*Last modified: Mar 25, 2016*
