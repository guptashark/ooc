

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
 *
 *
 *  
 */
