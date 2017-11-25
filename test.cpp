#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include "matrix.h"
#include <string>

using namespace std;

matrix m;

class Node
{
	public:
	Node *child;
	virtual int type() = 0;
};

class CharNode : public Node
{
	public:
	char value;
	int type()
	{
		return 0;
	}
	CharNode(char value)
	{
		this->value = value;
	}
};

class FloatNode : public Node
{
	public:
	float value;
	int type()
	{
		return 1;
	}
	FloatNode(float value)
	{
		this->value = value;
	}
	
};

class MatrixNode : public Node
{
	public:
	matrix* value;
	int type()
	{
		return 2;
	}
	MatrixNode(matrix *value)
	{
		this->value = value;
	}
	~MatrixNode()
	{
		delete value;
	}
};

int get_operator_order(char cur)
{
	if(cur == '+' || cur == '-')return 1;
	if(cur == '*' || cur == '/')return 2;
	if(cur == '%')return 3;
	if(cur == '^')return 4;
	if(cur == '$')return 5;
	if(cur == '(')return 0;
	if(cur == ')')return 0;
};

bool is_higher_order_than(char cur, CharNode* node)
{
	if(node->value == '(' && cur == ')')return true;
	if(node->value == '%' && cur == '%')return true;
	if(get_operator_order(cur) > get_operator_order(node->value))
	{
		return true;
	}
	return false;
};

template <class T>
class Stack
{
	public:
	T *top;
	Stack()
	{
		this->top = NULL;
	}
	void add(T *node)
	{
		node->child = this->top;
		this->top = node;
	}
	T* pop()
	{
		T * temp = this->top;
		this->top = this->top->child;
		return temp;
	}
	void print()
	{
		T *current = this->top;
		while(current != NULL)
		{
			switch(current->type())
			{
				case 2:
					//((MatrixNode*)(current))->value->print_matrix();
					break;
				case 1:
					cout<<((FloatNode*)(current))->value<<endl;
					break;
				case 0:
					cout<<((CharNode*)(current))->value<<endl;
					break;
			}
			if(current->type() == 2)
			{
				cout<<((FloatNode*)(current))->value<<endl;
			}else {
				cout<<((CharNode*)(current))->value<<endl;
			}
			current = current->child;
		}
	}
};

char* infix_to_reverse_polish(char *infix)
{
	int countAll = 0, countVirtual = 0;
	for (int i = 0; infix[i] != '\0'; ++i)
	{
		if(infix[i] != ' ')
		{
			countAll++;
			if(infix[i] == '.' || infix[i] == '*' || infix[i] == '/' || infix[i] == '^' || infix[i] == '+' || infix[i] == '-' || infix[i] == '(' || infix[i] == ')' || infix[i] == ']' || infix[i] == '[')
			{
				countVirtual++;
			}
		}
	}
	char *temp = new char[countAll + countVirtual + 1];
	Stack<Node>* temp_stack = new Stack<Node>();
	int j = 0;
	bool flag = true, flagNegative = false;
	for (int i = 0; infix[i] != '\0'; ++i)
	{
		if(infix[i] != ' ')
		{
			if(infix[i] == '[')
			{
				while(infix[i] != ']')
				{
					if(infix[i] != ' ')
					{
						temp[j] = infix[i];
						j++;
					}
					i++;
				}
				flagNegative = true;
			}
			if(infix[i] == '.' || infix[i] == '*' || infix[i] == '/' || infix[i] == '^' || infix[i] == '+' || infix[i] == '-' || infix[i] == '(' || infix[i] == ')' || infix[i] == '%')
			{
				flag = false;
				if(infix[i] == '-' && !flagNegative)infix[i]= '%';
				if(temp_stack->top != NULL)
				{
					//flagNegative = false;
					if((infix[i] == '(' || is_higher_order_than(infix[i], (CharNode*)temp_stack->top))&& infix[i] != ')'){
						CharNode *x = new CharNode(infix[i]);
						flagNegative = false;
						if(infix[i] == '.')
						{
							while(infix[i] != '/')
							{
								i++;
							}
							x->value = '$';
							i++;
							flagNegative = true;
						}
						temp_stack->add(x);
					}else{
						if(infix[i] == ')')flagNegative = true;
						CharNode *x = (CharNode*)temp_stack->pop();
						if(x->value != '(')
						{
							temp[j] = x->value;
							j++;
							i--;
						}
						delete x;
					}
				}else{
					CharNode *x = new CharNode(infix[i]);
					flagNegative = false;
					if(infix[i] == '.')
					{
						while(infix[i] != '/')
						{
							i++;
						}
						x->value = '$';
						i++;
						flagNegative = true;
					}
					temp_stack->add(x);
				}
			}else{
				if(!flag)
				{
					temp[j] = ';';
					j++;
				}
				temp[j] = infix[i];
				j++;
				flag = true;
				flagNegative = true;
			}
		}
	}
	while(temp_stack->top != NULL){
		CharNode *x = (CharNode*)temp_stack->pop();
		temp[j] = x->value;
		j++;
	}

	temp[j] = '\0';

	return temp;
}

Node* reverse_polish_to_float(char *reverse_polish)
{
	Stack<Node>* temp_stack = new Stack<Node>();
	for (int i = 0; reverse_polish[i] != '\0'; ++i)
	{
		if(reverse_polish[i] == ';');
		else if(reverse_polish[i] == '[')
		{
			int count = 0;
			for (int k = i+1; reverse_polish[k] != ']'; ++k)
			{
				if(reverse_polish[k] != ' ')
				{
					count++;
				}
			}
			char *temp_string = new char[count];
			int j = 0;
			i++;
			while(reverse_polish[i] != ']')
			{
				if(reverse_polish[i] != ' ')
				{
					temp_string[j] = reverse_polish[i];
					j++;
				}
				i++;
			}
			Node *tempNode = new FloatNode(atof(temp_string));
			/*temp value*/
			temp_stack->add(tempNode);
			delete temp_string;
		}
		else if(reverse_polish[i] == '$')
		{
			Node *left = temp_stack->pop();
			Node *tempNode = new FloatNode(((FloatNode*)(left))->value * -1);
			/*temp value*/
			temp_stack->add(tempNode);
			delete left;
		}
		else if(reverse_polish[i] == '%')
		{
			Node *left = temp_stack->pop();
			Node *tempNode;
			if(left->type() == 1)
			{
				tempNode = new FloatNode(((FloatNode*)(left))->value * -1);
			}else{
				matrix *tempMatrix = new matrix;
				*tempMatrix = ( *(((MatrixNode*)left)->value) * -1);
				tempNode = new MatrixNode(tempMatrix);
			}
			/*temp value*/
			temp_stack->add(tempNode);
			delete left;
		}
		else if(reverse_polish[i] == '*' || reverse_polish[i] == '/' || reverse_polish[i] == '^' || reverse_polish[i] == '+' || reverse_polish[i] == '-')
		{
			Node *right = temp_stack->pop();
			Node *left = temp_stack->pop();
			Node *tempNode = NULL;
			switch(reverse_polish[i]){
				case '+':
					if(left->type() == 1)
					{
						if(right->type() == 1)
						{
							tempNode = new FloatNode(( (((FloatNode*)left)->value) + (((FloatNode*)right)->value) ));
						}else{
							matrix *tempMatrix = new matrix;
							*tempMatrix = ( (((FloatNode*)left)->value) + *(((MatrixNode*)right)->value) );
							tempNode = new MatrixNode(tempMatrix);
						}
					}else{
						if(right->type() == 1)
						{
							matrix *tempMatrix = new matrix;
							*tempMatrix = ( *(((MatrixNode*)left)->value) + (((FloatNode*)right)->value) );
							tempNode = new MatrixNode(tempMatrix);
						}else{
							matrix *tempMatrix = new matrix;
							*tempMatrix = ( *(((MatrixNode*)left)->value) + *(((MatrixNode*)right)->value) );
							tempNode = new MatrixNode(tempMatrix);
						}
					}
					/*temp values*/
					break;
				case '-':
					if(left->type() == 1)
					{
						if(right->type() == 1)
						{
							tempNode = new FloatNode(( (((FloatNode*)left)->value) - (((FloatNode*)right)->value) ));
						}else{
							matrix *tempMatrix = new matrix;
							*tempMatrix = ( (((FloatNode*)left)->value) - *(((MatrixNode*)right)->value) );
							tempNode = new MatrixNode(tempMatrix);
						}
					}else{
						if(right->type() == 1)
						{
							matrix *tempMatrix = new matrix;
							*tempMatrix = ( *(((MatrixNode*)left)->value) - (((FloatNode*)right)->value) );
							tempNode = new MatrixNode(tempMatrix);
						}else{
							matrix *tempMatrix = new matrix;
							*tempMatrix = ( *(((MatrixNode*)left)->value) - *(((MatrixNode*)right)->value) );
							tempNode = new MatrixNode(tempMatrix);
						}
					}
					/*temp values*/
					break;
				case '*':
					if(left->type() == 1)
					{
						if(right->type() == 1)
						{
							tempNode = new FloatNode(( (((FloatNode*)left)->value) * (((FloatNode*)right)->value) ));
						}else{
							matrix *tempMatrix = new matrix;
							*tempMatrix = ( (((FloatNode*)left)->value) * *(((MatrixNode*)right)->value) );
							tempNode = new MatrixNode(tempMatrix);
						}
					}else{
						if(right->type() == 1)
						{
							matrix *tempMatrix = new matrix;
							*tempMatrix = ( *(((MatrixNode*)left)->value) * (((FloatNode*)right)->value) );
							tempNode = new MatrixNode(tempMatrix);
						}else{
							matrix *tempMatrix = new matrix;
							*tempMatrix = ( *(((MatrixNode*)left)->value) * *(((MatrixNode*)right)->value) );
							tempNode = new MatrixNode(tempMatrix);
						}
					}
					/*temp values*/
					break;
				case '/':
					if(left->type() == 1)
					{
						if(right->type() == 1)
						{
							tempNode = new FloatNode(( (((FloatNode*)left)->value) / (((FloatNode*)right)->value) ));
						}else{
							matrix *tempMatrix = new matrix;
							*tempMatrix = ( (((FloatNode*)left)->value) / *(((MatrixNode*)right)->value) );
							tempNode = new MatrixNode(tempMatrix);
						}
					}else{
						if(right->type() == 1)
						{
							matrix *tempMatrix = new matrix;
							*tempMatrix = ( *(((MatrixNode*)left)->value) / (((FloatNode*)right)->value) );
							tempNode = new MatrixNode(tempMatrix);
						}else{
							matrix *tempMatrix = new matrix;
							*tempMatrix = ( *(((MatrixNode*)left)->value) / *(((MatrixNode*)right)->value) );
							tempNode = new MatrixNode(tempMatrix);
						}
					}
					/*temp values*/
					break;
				// case '^':
				// 	if(left->type() == 1)
				// 	{
				// 		if(right->type() == 1)
				// 		{
				// 			tempNode = new FloatNode(pow((((FloatNode*)left)->value),(((FloatNode*)right)->value) ));
				// 		}else{
				// 			matrix *tempMatrix = new matrix;
				// 			*tempMatrix = ( (((FloatNode*)left)->value) ^ *(((MatrixNode*)right)->value) );
				// 			tempNode = new MatrixNode(tempMatrix);
				// 		}
				// 	}else{
				// 		if(right->type() == 1)
				// 		{
				// 			matrix *tempMatrix = new matrix;
				// 			*tempMatrix = ( *(((MatrixNode*)left)->value) ^ (((FloatNode*)right)->value) );
				// 			tempNode = new MatrixNode(tempMatrix);
				// 		}else{
				// 			matrix *tempMatrix = new matrix;
				// 			*tempMatrix = ( *(((MatrixNode*)left)->value) ^ *(((MatrixNode*)right)->value) );
				// 			tempNode = new MatrixNode(tempMatrix);
				// 		}
				// 	}
				// 	/*temp values*/
				// 	break;
			}
			temp_stack->add(tempNode);
			delete right;
			delete left;
		}else{
			int temp = i;
			while(true)
			{
				i++;
				if(reverse_polish[i] != '\0')
				{
					if(reverse_polish[i] == ';' || reverse_polish[i] == '*' || reverse_polish[i] == '/' || reverse_polish[i] == '^' || reverse_polish[i] == '+' || reverse_polish[i] == '-' || reverse_polish[i] == '(' || reverse_polish[i] == ')' || reverse_polish[i] == '$' || reverse_polish[i] == '[' ||
						reverse_polish[i] == '%')
					{
						i--;
						break;	
					}
				}else{
					i--;
					break;
				}
			}
			int j = 0;
			bool is_Matrix = false;
			char * word = new char[i-temp+1];
			for (int k = temp; k <= i; ++k)
			{
				if (reverse_polish[k] != ' ' && reverse_polish[k] != '\0')
				{
					if(48 <= ((int)reverse_polish[k]) && ((int)reverse_polish[k]) <= 57);
					else{
						is_Matrix = true;
					}
					word[j] = reverse_polish[k];
					j++;
				}
			}
			word[j] = '\0';
			//cout<<"size:"<<i-temp+1<<" "<<word<<endl;
			Node *tempNode;
			if(is_Matrix)
			{
				matrix*  tempMatrix= new matrix;
				string testString = "A = [1.4 2.2 3.2 1;4.4 5.4 6.4 2;3.3 4.2 2 3;1 2 3 4];";
				tempMatrix->fill_matrix(testString);
				tempNode = new MatrixNode(tempMatrix);
			}else{
				tempNode = new FloatNode(atof(word));
			}
			temp_stack->add(tempNode);
		}
	}
	return temp_stack->pop();
}

int main()
{
	string testString = "m = [1.4 2.2 3.2 1;4.4 5.4 6.4 2;3.3 4.2 2 3;1 2 3 4];";
    m.fill_matrix(testString);
 	m.print_matrix();
    cout<<endl<<"operation : "<<"-m+50-m+50-m+50-m+50-m+50-m+50/122131+51+1619/(-m+50-m+50-m+50-m+50-m+50-m+50/122131+51+1619)"<<endl;
   	/*the first print*/
   	matrix r;
   	/*try to uncomment this*/
   	//r = -m+50-m+50-m+50-m+50-m+50-m+50/122131+51+1619/(-m+50-m+50-m+50-m+50-m+50-m+50/122131+51+1619);
   	cout<<endl<<"compiler parsing output : ";
   	r.print_matrix();
   	/*the second print*/
   	char infix[] = "-m+50-m+50-m+50-m+50-m+50-m+50/122131+51+1619/(-m+50-m+50-m+50-m+50-m+50-m+50/122131+51+1619)";
	char *temp = infix_to_reverse_polish(infix);
	cout<<endl<<"myfunction parsing output : ";
	if(reverse_polish_to_float(temp)->type() == 1)
	{
		cout<<endl<<((FloatNode*)reverse_polish_to_float(temp))->value<<endl;
		/*the third print if the output was float*/
	}else{
		/*the second print if the output was matrix*/
		cout<<endl;
		((MatrixNode*)reverse_polish_to_float(temp))->value->print_matrix();
	}
	cout<<endl;
}
