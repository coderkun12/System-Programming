#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <stack>
using namespace std;
stack<char> mystack;
struct operators
{
    char j;
    struct operators *next;
};
struct operators *head_op = NULL;
void create_operators()
{
    struct operators *n1 = new operators, *n2 = new operators, *n3 = new operators, *n4 = new operators, *n5 = new operators, *n6 = new operators, *n7 = new operators, *n8 = new operators;
    n1->j = '*';
    n1->next = n2;
    head_op = n1;
    n2->j = '+';
    n2->next = n3;
    n3->j = '/';
    n3->next = n4;
    n4->j = '=';
    n4->next = n5;
    n5->j = '>';
    n5->next = n6;
    n6->j = '<';
    n6->next = n7;
    n7->j = '-';
    n7->next = n8;
    n8->j = '&';
    n8->next = NULL;
}

struct keyword
{
    char a[40];
    struct keyword *next;
};
struct keyword *head_key = NULL;
void create_keyword()
{
    struct keyword *k1 = new keyword, *k2 = new keyword, *k3 = new keyword, *k4 = new keyword, *k5 = new keyword, *k6 = new keyword, *k7 = new keyword, *k8 = new keyword;
    head_key = new keyword;
    strcpy(head_key->a, "if");
    head_key->next = k1;
    strcpy(k1->a, "for");
    k1->next = k2;
    strcpy(k2->a, "while");
    k2->next = k3;
    strcpy(k3->a, "int");
    k3->next = k4;
    strcpy(k4->a, "float");
    k4->next = k5;
    strcpy(k5->a, "break");
    k5->next = k6;
    strcpy(k6->a, "do");
    k6->next = k7;
    strcpy(k7->a, "char");
    k7->next = k8;
    strcpy(k8->a, "void");
    k8->next = NULL;
}

struct delimiter
{
    struct delimiter *next;
    char k;
};
struct delimiter *head_del = NULL;

void create_delimiter()
{
    struct delimiter *d1 = new delimiter, *d2 = new delimiter, *d3 = new delimiter, *d4 = new delimiter, *d5 = new delimiter, *d6 = new delimiter;
    head_del = new delimiter;
    head_del->k = ',';
    head_del->next = d1;
    d1->k = ';';
    d1->next = d2;
    d2->k = ':';
    d2->next = d3;
    d3->k = '(';
    d3->next = d4;
    d4->k = ')';
    d4->next = d5;
    d5->k = '{';
    d5->next = d6;
    d6->k = '}';
    d6->next = NULL;
}

struct symbol_table
{
    struct symbol_table *next;
    char g[10];
};
struct symbol_table *head_sym = NULL;

int add_symbol(char a[])
{
    int i = 1;
    struct symbol_table *hh = head_sym;
    while (hh != NULL)
    {
        if (strcmp(hh->g, a) == 0)
        {
            return i;
        }
        i++;
        hh = hh->next;
    }
    i = 1;
    struct symbol_table *ne = new symbol_table;
    strcpy(ne->g, a);
    ne->next = NULL;
    if (head_sym == NULL)
    {
        head_sym = ne;
        return i;
    }
    else
    {
        struct symbol_table *nn = head_sym;
        while (nn->next != NULL)
        {
            nn = nn->next;
            i++;
        }
        nn->next = ne;
        return i;
    }
}

int search_keyword(char momo[])
{
    int k = 1;
    // check if a keyword;
    struct keyword *yy = head_key;
    while (yy != NULL)
    {
        if (strcmp(yy->a, momo) == 0)
        {
            return k;
        }
        k++;
        yy = yy->next;
    }
    return -1;
}

int search_operators(char momo[])
{
    int k = 1;
    char t[10], to;
    strcpy(t, momo);
    to = t[0];
    // check if a operator;
    struct operators *yy = head_op;
    while (yy != NULL)
    {
        if (yy->j == to)
        {
            return k;
        }
        k++;
        yy = yy->next;
    }
    return -1;
}

int search_delimiter(char a[])
{
    int k = 1;
    char t[10], to;
    strcpy(t, a);
    to = t[0];
    // check if a operator;
    struct delimiter *yy = head_del;
    while (yy != NULL)
    {
        if (yy->k == to)
        {
            return k;
        }
        k++;
        yy = yy->next;
    }
    return -1;
}
void tokenization(char xyz[], int o)
{
    int check, flag4, flag1, flag2, flag3, j, i;
    char *uto = strtok(xyz, " ");
    while (uto != NULL)
    {
        if ((strcmp(uto, "{") == 0) || (strcmp(uto, "(") == 0) || (strcmp(uto, "[") == 0))
        {
            mystack.push(uto[0]);
        }
        else if (strcmp(uto, "}") == 0)
        {
            if (mystack.top() == '{')
            {
                mystack.pop();
            }
        }
        else if (strcmp(uto, ")") == 0)
        {
            if (mystack.top() == '(')
            {
                mystack.pop();
            }
        }
        else if (strcmp(uto, "]") == 0)
        {
            if (mystack.top() == '[')
            {
                mystack.pop();
            }
        }
        flag1 = search_keyword(uto);
        flag2 = search_operators(uto);
        flag3 = search_delimiter(uto);
        if (flag1 != -1 && flag2 == -1 && flag3 == -1)
        {
            cout << o << "\t" << uto << "\tKeyword\t\t" << flag1 << endl;
        }
        else if (flag1 == -1 && flag2 != -1 && flag3 == -1)
        {
            cout << o << "\t" << uto << "\tOperator\t" << flag2 << endl;
        }
        else if (flag1 == -1 && flag2 == -1 && flag3 != -1)
        {
            cout << o << "\t" << uto << "\tDelimiter\t" << flag3 << endl;
        }
        else if (flag1 == -1 && flag2 == -1 && flag3 == -1)
        {
            flag4 = 0;
            for (i = 0; i < strlen(uto); i++)
            {
                if (uto[i] >= '0' && uto[i] <= '9')
                {
                    flag4++;
                }
            }
            if (flag4 == strlen(uto))
            {
                cout << o << "\t" << uto << "\tConstant\t" << uto << endl;
            }
            else
            {
                flag4 = add_symbol(uto);
                cout << o << "\t" << uto << "\tIdentifier\t" << flag4 << endl;
            }
        }
        uto = strtok(NULL, " ");
    }
}

int main()
{
    int i = 1;
    create_delimiter();
    create_keyword();
    create_operators();
    char abc[100];
    fstream hh;
    hh.open("C_Code.txt");
    cout << "Line_Num Lexeme Token_Class Token_ID" << endl;
    while (!hh.eof())
    {
        hh.getline(abc, 50);
        tokenization(abc, i);
        i++;
    }
    if (mystack.empty())
    {
        cout << "\nThe program has passed paranthesis check and is correct!";
    }
    else
    {
        cout << "\nThe program didn't pass paranthesis check and is incorrect";
    }
    return 0;
}