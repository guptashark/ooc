

/* Recorsive descent parsing: 
 * rules are translated into equivalent C expressions
 *
 * Translate a rule such as: 
 * 	sum : product { +|- product } ... 
 *
 * 	into: 
 *
 * 	void sum(void) {
 * 		product();
 * 		for(;;) {
 * 			switch(token) {
 * 			case '+':
 * 			case '-':
 * 				scan(0), 
 * 				product(); 
 * 				continue;
 * 			}
 * 			return;
 * 		}
 * 	}
 *
 * We create a C function for each grammatical rule - 
 * they call on each other. The only problem is 
 * infinite recursion... (how does this case happen?)
 *
 * token always contains the next input symbol, 
 * since scan returns the token val. Remember that
 * scan(0) persists the bp pointer so that it can 
 * get the next token from the buf. Only when the
 * buffer is exhausted is scan(buf) called to 
 * compute the next stuff. 
 *
 */

/* Processor
 * simple arithmetic is mostly easy: 
 * 	compute as soon as possible, 
 * 	and pass back the results. 
 *
 * Complicated expressions not easy: 
 * 	Need to build a tree that can be 
 * 	processed in traversals. 
 *
 * 	a basic structure can be: 
 * 	struct Node {
 * 		enum tokens token;
 * 		struct Node * left;
 * 		struct Node * right;
 * 	}
 *
 * 	but this is inflexible. Say we 
 * 	have the token "number", and now 
 * 	we need to store a number. then 
 * 	we need a field for it... which 
 * 	doesn't seem like a problem...? 
 *
 * 	Space is wasted in the case of 
 * 	unary operators. (where it's 
 * 	an fn like 'abs', because now
 * 	one of the left and right nodes
 * 	will be null. 
 *
 * 	It's... probably best to copy 
 * 	over the files and then 
 * 	implement them further instead
 * 	of doing this, because the 
 * 	book serves as a complement to 
 * 	the source files that you're 
 * 	supposed to have access to. 
 * 	I'm sure that there will be 
 * 	enough exercises to extend
 * 	an arithmetic expression calc. 
 */
