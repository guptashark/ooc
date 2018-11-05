#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "parse.h"
#include "value.h"

/*
 *	evaluation driver
 */

struct Type {
	void * (* new) (va_list ap);
	double (* exec) (const void * tree);
	void (* delete) (void * tree);
};


/* The polymorphic function call */
void * new (const void * type, ...)
{	va_list ap;
	void * result;

	assert(type && ((struct Type *) type) -> new);

	va_start(ap, type);
	result = ((struct Type *) type) -> new(ap);
	* (const struct Type **) result = type;
	va_end(ap);
	return result;
}

static double exec (const void * tree)
{
	assert(tree && * (struct Type **) tree
		&& (* (struct Type **) tree) -> exec);

	return (* (struct Type **) tree) -> exec(tree);
}

void process (const void * tree)
{
	printf("\t%g\n", exec(tree));
}

void delete (void * tree)
{
	assert(tree && * (struct Type **) tree
		&& (* (struct Type **) tree) -> delete);

	(* (struct Type **) tree) -> delete(tree);
}

/*
 *	NUMBER
 */

struct Val {
	const void * type;
	double value;
};

static void * mkVal (va_list ap)
{	struct Val * node = malloc(sizeof(struct Val));

	assert(node);
	node -> value = va_arg(ap, double);
	return node;
}

static double doVal (const void * tree)
{
	return ((struct Val *) tree) -> value;
}
/* There is no freeVal fn because it is 
 * c's free fn. Since Val has no children 
 * as a node, it doesn't need to do anythig special. 
 * Also, the data type is not complex enough to 
 * require any dynamically allocated mem. 
 */



/*
 *	unary operators
 */

struct Un {
	const void * type;
	void * arg;
};

static void * mkUn (va_list ap)
{	struct Un * node = malloc(sizeof(struct Un));

	assert(node);
	/* The argument of the unary op
	 * is already known, and passed in. 
	 */
	node -> arg = va_arg(ap, void *);
	return node;
}

static double doMinus (const void * tree)
{
	return - exec(((struct Un *) tree) -> arg);
}

static void freeUn (void * tree)
{
	/* We have to cast tree to a 
	 * unary operator because it's a 
	 * void ptr. Otherwise we wouldn't 
	 * be able to call delete on it's 
	 * arg.
	 */
	delete(((struct Un *) tree) -> arg);
	free(tree);
}

/*
 *	binary operators
 */

struct Bin {
	const void * type;
	void * left;
	void * right;
};

static void * mkBin (va_list ap)
{	struct Bin * node = malloc(sizeof(struct Bin));

	assert(node);
	/* building the node from the bottom up. 
	 * It's two children must already be 
	 * known... 
	 */
	node -> left = va_arg(ap, void *);
	node -> right = va_arg(ap, void *);
	return node;
}

static double doAdd (const void * tree)
{
	/* The following is somewhat hard to 
	 * follow, we'll write our own 
	 * implementation - we won't do this
	 * for the other fn's becuase it's 
	 * identical. The only point was to
	 * unpack what is happening here
	 */


	/* Fun note: There are three levels of 
	 * brackets. The outermost are for the 
	 * exec fn call, the innermost are 
	 * to set the type that tree is being 
	 * casted to, and then the second level
	 * is to cast tree to (struct Bin *) and 
	 * not tree->left. Struct member access
	 * through a pointer has a higher
	 * operator precedence than type casting. 
	 */ 
	/*
	return exec(((struct Bin *) tree) -> left) +
			exec(((struct Bin *) tree) -> right);
	*/

	struct Bin * t = (struct Bin *)tree;
	double l_val = exec(t -> left);
	double r_val = exec(t -> right);

	return l_val + r_val;
}

static double doSub (const void * tree)
{
	return exec(((struct Bin *) tree) -> left) -
		exec(((struct Bin *) tree) -> right);
}

static double doMult (const void * tree)
{
	return exec(((struct Bin *) tree) -> left) *
		exec(((struct Bin *) tree) -> right);
}

static double doDiv (const void * tree)
{	double left = exec(((struct Bin *) tree) -> left);
	double right = exec(((struct Bin *) tree) -> right);

	if (right == 0.0)
		error("division by zero");
	return left / right;
}


static void freeBin (void * tree)
{
	/* recursive algo to delete a 
	 * binary value
	 *
	 * Why do we have to cast to 
	 * a (struct Bin *) ?? Wouldn't 
	 * delete just call the correct 
	 * delete fn? 
	 *
	 * Ans: tree is a void ptr. In order
	 * to delete it's left and right 
	 * children, we need to know that 
	 * it has them, so we cast it to 
	 * a binary operator to access
	 * those ptrs. 
	 */
	delete(((struct Bin *) tree) -> left);
	delete(((struct Bin *) tree) -> right);
	free(tree);
}

/*
 *	Types
 */

/* these are all binary operators
 * all of them involve mkBin and freeBin. 
 */
static struct Type _Add = { mkBin, doAdd, freeBin };
static struct Type _Sub = { mkBin, doSub, freeBin };
static struct Type _Mult = { mkBin, doMult, freeBin };
static struct Type _Div = { mkBin, doDiv, freeBin };

/* This is a unary operator, negation. 
 * (might be confusing compared to _Sub from 
 * the binary operators)
 */
static struct Type _Minus = { mkUn, doMinus, freeUn };

/* the type that is the leaf of the parse tree - 
 * the values themselves. 
 */
static struct Type _Value = { mkVal, doVal, free };

/* The type names that are pointers to the 
 * types themsleves
 */
const void * Add = & _Add;
const void * Sub = & _Sub;
const void * Mult = & _Mult;
const void * Div = & _Div;
const void * Minus = & _Minus;
const void * Value = & _Value;
