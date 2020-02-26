//
//  main.c
//  rpn
//
//  Created by William McCarthy on 192//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXOP   100    /* max size of operand/operator */
#define NUMBER '0'     /* signal that a number was found */
#define MAXVAL  100

size_t sp = 0;   // aka unsigned long -- printf using %zu
double val[MAXVAL];   // stack of values

char buf[BUFSIZ];
double variable[26];
size_t bufp = 0;

int getch_(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }
void ungetch_(int c) {
  if (bufp >= BUFSIZ) { fprintf(stderr, "ungetch: too many characters\n");  return; }
  buf[bufp++] = c;
}

int getop(char* s) {
  int i, c;
  while ((s[0] = c = getch_()) == ' ' || c == '\t') { }  // skip whitespace
  s[1] = '\0';
  
  if (!isdigit(c) && c != '.') { return c; }  // if not a digit, return

  i = 0;
  if (isdigit(c)) {  // get digits before '.'
    while (isdigit(s[++i] = c = getch_())) { }
  }
  if (c == '.') {    // get digits after decimal (if any)
    while (isdigit(s[++i] = c = getch_())) { }
  }
  s[i] = '\0';
  if (c != EOF) { ungetch_(c); }
  return NUMBER;      // return type is NUMBER, number stored in s
}

double pop(void) {
  if (sp == 0) { fprintf(stderr, "stack underflow\n");  return 0.0; }
  return val[--sp];
}

void push(double f) {
  if (sp == MAXVAL) { fprintf(stderr, "stack overflow -- can't push %g\n", f);  return; }
  val[sp++] = f;
}
void swap(void) {
        double t1 = pop();
        double t2 = pop();
        push(t1);
        push(t2);
}


void duplicate(){
	double op = pop();
	push(op);
	push(op);
}

void rpn(void) {
  int type;
  double op2;
  char s[BUFSIZ];
  
  while ((type = getop(s)) != EOF) {
    switch(type) {
      case '\n':    printf("\t%.8g\n", pop());  break;
      case NUMBER:  push(atof(s));              break;
      case '+':     push(pop() + pop());        break;
      case '*':     push(pop() * pop());        break;
      case '-':     push(-(pop() - pop()));     break;
	  //exercise 4-3
	  case '%':
		if((op2 = pop()) == 0.0) { fprintf(stderr, "divide by zero\n"); break;}
		else{
			push((int) pop % (int) op2);
			break;
		}
      case '/':
        if ((op2 = pop()) == 0.0) { fprintf(stderr, "divide by zero\n");  break; }
        push(pop() / op2);
        break;
	//exercise 4-5
	 case 's':
        push(sin(pop()));
        break;
     case 'e':
        op2 = exp(pop());
        push(op2);
		break;
     case 'p':
            op2 = pop();
            op2 = pow(op2, pop());
            push(op2);
            break;
	//exercise 4-4		
	case 'S':
			swap(s);
			break;
	case 'd':
			duplicate(s);
			break;
	case 'P':
			push(val[sp - 1]);
			break;
	case 'c':
			sp = 0;
			break;
	//exercise 4-6		
	case 'A':  variable[0]  = pop();							break;
	case 'B':  variable[1]  = pop();							break;
	case 'C':  variable[2]  = pop();							break;
	case 'D':  variable[3]  = pop();							break;
	case 'E':  variable[4]  = pop();							break;
	case 'F':  variable[5]  = pop();							break;
	case 'G':  variable[6]  = pop();							break;
	case 'H':  variable[7]  = pop();							break;
	case 'I':  variable[8]  = pop();							break;
	case 'K':  variable[9]  = pop();							break;
	case 'L':  variable[10] = pop();							break;
	case 'M':  variable[11] = pop();							break;
	case 'N':  variable[12] = pop();							break;
	case 'O':  variable[13] = pop();							break;
	case 'P':  variable[14] = pop();							break;
	case 'Q':  variable[15] = pop();							break;
	case 'R':  variable[16] = pop();							break;
	case 'S':  variable[17] = pop();							break;
	case 'T':  variable[18] = pop();							break;
	case 'U':  variable[19] = pop();							break;
	case 'V':  variable[20] = pop();							break;
	case 'W':  variable[21] = pop();							break;
	case 'X':  variable[22] = pop();							break;
	case 'Y':  variable[23] = pop();							break;
	case 'Z':  variable[24] = pop();							break;
    default:      fprintf(stderr, "unknown command %s\n", s);	break;
    }
  }
}

int main(int argc, const char * argv[]) {
  rpn();

  return 0;
}
