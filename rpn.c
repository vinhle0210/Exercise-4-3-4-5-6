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
#include <string.h>

#define MAXOP   100    /* max size of operand/operator */
#define NUMBER '0'     /* signal that a number was found */
#define VARIABLE '1'
#define MATH '2'
#define STACK '3'
#define MAXVAL  100
#define ENGLISH 26

size_t sp = 0;   // aka unsigned long -- printf using %zu
double val[MAXVAL];   // stack of values

char buf[BUFSIZ];
size_t bufp = 0;
double variable[ENGLISH];

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
 if (isalpha(c)){
    i = 0;
    while (s[++i] = c = getch_()){}
      s[i] = '\0';
      return MATH;
  }
  if( c == '@'){
    i = 0;
    while(isalpha(s[++i] = c = getch_())){}
    s[i] = '\0';
    return STACK;
  }
	if ( c == '=' || c == '?'){
		i = 0;
		while (isupper(s[++i] = c = getch_())){}
    s[i] = '\0';
		return VARIABLE;
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
void var(char* s){
	if(*s == '='){
		variable[*(s+1) - 'A'] = pop();
	}
	else if(*s == '?'){
		push(variable[*(s+1) -'A']);
	}
}
void math(char* s){
  if(strcmp(s, "sin") == 0){
    push(sin(pop()));
  }
  if(strcmp(s, "cos") == 0){
    push(cos(pop()));
  }
  if(strcmp(s, "exp") == 0){
    push(exp(pop()));
  }
  if(strcmp(s, "pow") == 0){
    double op2 = pop();
    push(pow(pop(), op2));
  }
}
void stack(char* s){
  if(strcmp(s, "swap") == 0){
    double op1 = pop();
    double op2 = pop();
    push(op1);
    push(op2);
  }
  if(strcmp(s, "peek")){
    push(val[sp - 1]);
  }
  if(strcmp(s, "clear")){
    sp = 0;
  }
  if(strcmp(s, "duplicate")){
    double op1 = pop();
    push(op1);
    push(op1);
  }
}

void rpn(void) {
  int type;
  double op2;
  char s[BUFSIZ];
  
  while ((type = getop(s)) != EOF) {
    switch(type) {
      case '\n':      printf("\t%.8g\n", pop());  break;
      case NUMBER:    push(atof(s));              break;
	    case VARIABLE:  var(s);				              break;
      case MATH:      math(s);                    break;
      case STACK:     stack(s);                   break;
      case '+':       push(pop() + pop());        break;
      case '*':       push(pop() * pop());        break;
      case '-':       push(-(pop() - pop()));     break;
      case '%':
        op2 = pop(); 
        if(op2 == 0){
          fprintf(stderr, "didvide by zero\n");
          break;
        }
        else{
          push((int) pop() % (int) op2);
          break;
        }      
        push((int) pop() % (int) pop());
      case '/':
        if ((op2 = pop()) == 0.0) { fprintf(stderr, "divide by zero\n");  break; }
        push(pop() / op2);
        break;
      default:      fprintf(stderr, "unknown command %s\n", s);  break;
    }
  }
}

int main(int argc, const char * argv[]) {
  rpn();

  return 0;
}
