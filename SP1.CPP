#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <ctype.h>
using namespace std;

// To maintain LC
static int lc = 0;
static int lac_pred = 0;

// Output Table using Linked lsit!
struct op_tab
{
    char label[50], mnemonic[10], op1[6], op2[6];
    struct op_tab *next;
    int lcc;
};

struct op_tab *headop = NULL;

// Creates and adds node to the output table/ linked list
void create_code_tab(char lab[], char op1[], char op2[], char mnemo[])
{
    struct op_tab *newn = new op_tab;
    strcpy(newn->label, lab);
    strcpy(newn->mnemonic, mnemo);
    strcpy(newn->op1, op1);
    newn->lcc = lac_pred;
    if (strcmp(mnemo, "DS") == 0)
    {
        lc = lc + atoi(op1);
    }
    strcpy(newn->op2, op2);
    newn->next = NULL;
    if (headop == NULL)
    {
        newn->lcc = 0;
        headop = newn;
    }
    else
    {
        struct op_tab *cur = headop;
        while (cur->next != NULL)
        {
            cur = cur->next;
        }
        cur->next = newn;
    }
}

// Symbol Pointer using linked list
struct SYMPTR
{
    char symb[40], val[4];
    int lc;
    struct SYMPTR *next;
};

struct SYMPTR *sy_head;

// To create a node whenever we arrive at a symbol
void insert_symb(char ab[])
{
    struct SYMPTR *newn = new SYMPTR;
    strcpy(newn->symb, ab);
    if (sy_head == NULL)
    {
        sy_head = newn;
        sy_head->next = NULL;
    }
    else
    {
        struct SYMPTR *u = sy_head;
        while (u->next != NULL)
        {
            u = u->next;
        }
        u->next = newn;
        newn->next = NULL;
    }
}

// To assign address and value to symbols.
void search_assign(char ab[], char val[])
{
    struct SYMPTR *curr = sy_head;
    while (curr != NULL)
    {
        if (strcmp(ab, curr->symb) == 0)
        {
            break;
        }
        curr = curr->next;
    }
    curr->lc = lc;
    strcpy(curr->val, val);
}

// MOT Table using linkedlist
struct MOTPTR
{
    struct MOTPTR *next;
    char mnemo[10];
    char opc[3];
    int size;
    char type[2];
};

struct MOTPTR *head;

// To match the mneonic. Syntax check
int match(char a[])
{
    int no;
    int k = 0;
    struct MOTPTR *cur = head;
    while (cur != NULL)
    {
        if (strcmp(a, cur->mnemo) == 0)
        {
            k = 1;
            lac_pred = lc;
            lc = lc + cur->size;
            break;
        }
        cur = cur->next;
    }
    return k;
}

// Function which is used to split the lines and is actual brain of the program.
void split(char str[])
{
    char label[50], mnemonic[10], op1[6], op2[6], temp1[10], temp3[10];
    char *oper;
    char *token;
    int i, tr, j, k = 0, flag = 0;
    int l = -1;
    token = strtok(str, " "); // Splits string into 4 parts.
    // While loop to iterate through a line split into tokens.
    while (token != NULL)
    {
        // To check if we are dealing with a label
        if (token[strlen(token) - 1] == ':')
        {
            for (i = 0; i < strlen(token) - 1; i++)
            {
                label[i] = token[i];
            }
            label[i] = '\0';
            insert_symb(label);
            search_assign(label, "");
        }
        // TO check if its a mnemonic
        else if (match(token) == 1)
        {
            // Because AD's dont have these fields
            if (strcmp(token, "START") == 0 || strcmp(token, "END") == 0)
            {
                strcpy(op1, "-");
                strcpy(op2, "-");
                strcpy(label, "-");
            }
            // Because DC and DS doesnt have a label field.
            if (strcmp(token, "DC") == 0 || strcmp(token, "DS") == 0)
            {
                strcpy(label, "-");
            }
            // If a there is no label then make its value as ampth
            if (k == 0)
            {
                strcpy(label, "-");
            }
            strcpy(mnemonic, token);
        }
        else
        {
            i = 0;
            tr = 0;
            // To check if one operand or two
            for (i = 0; i < strlen(token); i++)
            {
                if (token[i] == ',')
                {
                    tr++;
                }
            }
            // If two operands exist
            if (tr != 0)
            {
                oper = strtok(token, ",");
                strcpy(op1, oper);
                oper = strtok(NULL, ",");
                strcpy(op2, oper);
                j = 0;
                // While is used to remove the problem of immediate values being considered as operands.
                while (oper[j] != '\0')
                {
                    if (oper[j] >= '0' && oper[j] <= '9')
                    {
                        flag = 1;
                    }
                    j++;
                }
                // If a symbol then add it to the symbol table
                if ((strcmp(oper, "AREG") != 0 && strcmp(oper, "BREG") != 0 && strcmp(oper, "CREG") != 0 && strcmp(oper, "DREG") != 0) && flag == 0)
                {
                    strcpy(temp1, oper);
                    insert_symb(oper);
                }
            }
            // ELse if there is a single operand
            else
            {
                strcpy(op1, token);
                // TO differentiate between the DS,DC and statements with one operand
                if (k != 0 && l <= -1)
                {
                    strcpy(op2, "-");
                }
                else
                {
                    strcpy(op2, temp1);
                }
            }
            // Below part specially for DS, DC
            if (strcmp(token, "AREG") != 0 && strcmp(token, "BREG") != 0 && strcmp(token, "CREG") != 0 && strcmp(token, "DREG") != 0)
            {
                if (k == 0)
                {
                    strcpy(temp1, token);
                    strcpy(op2, token);
                    l = 3;
                }
                else if (l == 1 && k == 2)
                {
                    search_assign(temp1, token);
                }
            }
        }
        k++;
        l--;
        token = strtok(NULL, " ");
    }
    create_code_tab(label, op1, op2, mnemonic);
}

// Creates hardcoded MOT table.
void create_MOT()
{
    head = (struct MOTPTR *)malloc(sizeof(struct MOTPTR));
    struct MOTPTR *a = new MOTPTR;
    struct MOTPTR *b = new MOTPTR;
    struct MOTPTR *c = new MOTPTR;
    struct MOTPTR *d = new MOTPTR;
    struct MOTPTR *e = new MOTPTR;
    struct MOTPTR *f = new MOTPTR;
    struct MOTPTR *g = new MOTPTR;
    struct MOTPTR *h = new MOTPTR;
    head->next = a;
    strcpy(head->mnemo, "MOV");
    strcpy(head->type, "IS");
    strcpy(head->opc, "01");
    head->size = 2;
    a->next = b;
    strcpy(a->mnemo, "START");
    strcpy(a->type, "AS");
    strcpy(a->opc, "01");
    a->size = 0;
    b->next = c;
    strcpy(b->mnemo, "END");
    strcpy(b->type, "AS");
    strcpy(b->opc, "02");
    b->size = 0;
    c->next = d;
    strcpy(c->mnemo, "ADD");
    strcpy(c->opc, "02");
    strcpy(c->type, "AS");
    c->size = 2;
    d->next = e;
    strcpy(d->mnemo, "SUB");
    strcpy(d->opc, "03");
    strcpy(d->type, "AS");
    d->size = 2;
    e->next = f;
    strcpy(e->mnemo, "DC");
    strcpy(e->opc, "01");
    strcpy(e->type, "DS");
    e->size = 1;
    f->next = g;
    strcpy(f->mnemo, "DS");
    strcpy(f->opc, "02");
    strcpy(f->type, "DS");
    f->size = 0;
    g->next = h;
    strcpy(g->mnemo, "PRINT");
    strcpy(g->opc, "04");
    strcpy(g->type, "IS");
    g->size = 2;
    h->next = NULL;
    strcpy(h->mnemo, "MUL");
    strcpy(h->opc, "05");
    strcpy(h->type, "IS");
    h->size = 2;
}

// Gets the opcode and type for IC of any mnemonic
void print_mnemoIC(char t[])
{
    struct MOTPTR *abc = head;
    while (strcmp(abc->mnemo, t) != 0)
    {
        abc = abc->next;
    }
    cout << "(" << abc->type << ", " << abc->opc << ")";
}

// Prints the operands IC code
void print_operIC(char t[])
{
    int count = 1;
    if (strcmp(t, "AREG") == 0)
    {
        cout << " (R, 1)";
    }
    else if (strcmp(t, "BREG") == 0)
    {
        cout << " (R, 2)";
    }
    else if (strcmp(t, "CREG") == 0)
    {
        cout << " (R, 3)";
    }
    else if (strcmp(t, "DREG") == 0)
    {
        cout << " (R, 4)";
    }
    else if (t[0] >= '0' && t[0] <= '9')
    {
        cout << " (I, " << t << ")";
    }
    else
    {
        struct SYMPTR *cur = sy_head;
        while (strcmp(cur->symb, t) != 0)
        {
            count++;
            cur = cur->next;
        }
        cout << " (S, " << count << ")";
    }
}

// Function to get the Intermediate code.
void print_IC()
{
    struct op_tab *cur = headop;
    while (cur != NULL)
    {
        cout << cur->lcc << " ";
        print_mnemoIC(cur->mnemonic);
        if (strcmp(cur->op1, "-") != 0)
        {
            print_operIC(cur->op1);
        }
        if (strcmp(cur->op2, "-") != 0)
        {
            print_operIC(cur->op2);
        }
        cur = cur->next;
        cout << endl;
    }
}

// Prints Symbol Table.
void print_symtab()
{
    cout << endl;
    cout << "Symbol\t\t"
         << "Location\t"
         << "Value" << endl;
    struct SYMPTR *cur = sy_head;
    while (cur != NULL)
    {
        cout << cur->symb << "\t\t" << cur->lc << "\t\t" << cur->val;
        cur = cur->next;
        cout << endl;
    }
}

// Main fucntion
int main()
{
    char *ggd;
    char ABC[40];
    fstream ff, gg;
    int i, lines;
    lines = 0;
    create_MOT();
    gg.open("ASM1.txt");
    while (!gg.eof())
    {
        gg.getline(ABC, 100);
        lines++;
    }
    gg.close();
    ff.open("ASM1.txt");
    i = 0;
    cout << "The input program is as follows:-\n";
    while (!ff.eof())
    {
        ff.getline(ABC, 40);
        cout << ABC << endl;
        if (i == 0)
        {
            ggd = strtok(ABC, " ");
            ggd = strtok(NULL, " ");
            lc = atoi(ggd);
        }
        split(ABC);
        i++;
    }
    cout << "\nThe intermediate code for the assembly program is as follows:-\n";
    print_IC();
    cout << "\n";
    cout << "The symbols encountered in the program are as follows:-\n";
    print_symtab();
    ff.close();
    return 0;
}