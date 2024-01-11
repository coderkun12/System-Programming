#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <ctype.h>
using namespace std;

static int pool_tab[10][2] = {(0, 0), (0, 0), (0, 0), (0, 0), (0, 0), (0, 0), (0, 0), (0, 0), (0, 0)};
// To maintain LC
static int lc = 0;
static int lac_pred = 0;
static int flag = 0, ko = 0;
static int temp_lc = 0, count = 0, LTORG_Cnt = 0, count2 = 0;

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
    if (strcmp(mnemo, "LTORG") == 0)
    {
        cout << count2;
        lc = lc + count2 - 1;
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
struct LITPTR
{
    char val[4];
    int lc, index;
    struct LITPTR *next;
};

struct LITPTR *sy_head;

void add_to_lit_table(char val[])
{
    int a;
    struct LITPTR *newn = new LITPTR;
    newn->lc = 0;
    newn->next = NULL;
    strcpy(newn->val, val);
    if (sy_head == NULL)
    {
        newn->index = 1;
        sy_head = newn;
        pool_tab[ko][0] = 1;
        pool_tab[ko][1] = 1;
        ko++;
        count++;
    }
    else
    {
        struct LITPTR *bobo = sy_head;
        while (bobo->next != NULL)
        {
            a = bobo->index;
            bobo = bobo->next;
        }
        bobo->next = newn;
        bobo->index = a + 1;
        count++;
    }
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
    int op = 0;
    struct MOTPTR *cur = head;
    while (cur != NULL)
    {
        if (strcmp(a, cur->mnemo) == 0)
        {
            op = 1;
            lac_pred = lc;
            lc = lc + cur->size;
            break;
        }
        cur = cur->next;
    }
    return op;
}

void add_pool_id()
{
    int a = 0;
    pool_tab[ko][0] = ko + 1;
    pool_tab[ko][1] = count + 1;
    ko++;
    count = 0;
}

int add_lc_lit()
{
    struct LITPTR *n = sy_head;
    while (n != NULL)
    {
        if (n->lc == 0)
        {
            n->lc = temp_lc;
            temp_lc++;
            lc = lac_pred + 1;
            flag = 1;
            return 1;
        }
        n = n->next;
    }
    return 0;
}
// Function which is used to split the lines and is actual brain of the program.
void split(char str[])
{
    char label[50], mnemonic[10], op1[6], op2[6], temp1[10], temp3[10];
    char *oper;
    char *c;
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
        }
        // TO check if its a mnemonic
        else if (match(token) == 1)
        {
            // Because AD's dont have these fields
            strcpy(mnemonic, token);
            if (strcmp(token, "START") == 0 || strcmp(token, "END") == 0)
            {
                strcpy(op1, "-");
                strcpy(op2, "-");
                strcpy(label, "-");
            }
            else if (strcmp(token, "LTORG") == 0)
            {
                temp_lc = lc;
                strcpy(op1, "-");
                strcpy(op2, "-");
                strcpy(label, "-");
                LTORG_Cnt++;
                int h;
                flag = 1;
                count2 = count;
                if (LTORG_Cnt != 1)
                {
                    add_pool_id();
                }
                else
                {
                    count = 0;
                }
                do
                {
                    h = add_lc_lit();
                } while (h != 0);
                flag = 0;
            }
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
                if (oper[0] != '=')
                {
                    strcpy(op2, oper);
                }
                else
                {
                    j = 0;
                    // While is used to remove the problem of immediate values being considered as operands.
                    while (j < strlen(oper))
                    {
                        if (oper[j] >= '0' && oper[j] <= '9')
                        {
                            c[0] = oper[j];
                        }
                        j++;
                    }
                    strcpy(op2, c);
                    add_to_lit_table(c);
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
    struct MOTPTR *g = new MOTPTR;
    struct MOTPTR *h = new MOTPTR;
    struct MOTPTR *i = new MOTPTR;
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
    d->next = g;
    strcpy(d->mnemo, "SUB");
    strcpy(d->opc, "03");
    strcpy(d->type, "AS");
    d->size = 2;
    g->next = h;
    strcpy(g->mnemo, "PRINT");
    strcpy(g->opc, "04");
    strcpy(g->type, "IS");
    g->size = 2;
    h->next = i;
    strcpy(h->mnemo, "MUL");
    strcpy(h->opc, "05");
    strcpy(h->type, "IS");
    h->size = 2;
    i->next = NULL;
    strcpy(i->mnemo, "LTORG");
    strcpy(i->opc, "06");
    strcpy(i->type, "AS");
    i->size = 0;
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

    else
    {
        struct LITPTR *cur = sy_head;
        while (strcmp(cur->val, t) != 0)
        {
            cur = cur->next;
        }
        cout << " (L, " << cur->index << ")";
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

void print_LIT()
{
    cout << "Index \tValue  \t  LC" << endl;
    struct LITPTR *u = sy_head;
    while (u->next != NULL)
    {
        cout << u->index << "\t   " << u->val << "      " << u->lc << endl;
        u = u->next;
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
    gg.open("ASM2.txt");
    while (!gg.eof())
    {
        gg.getline(ABC, 100);
        lines++;
    }
    gg.close();
    ff.open("ASM2.txt");
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
    cout << "\nThe intemediate code is as follows:\n";
    print_IC();
    cout << "\nThe pool table encountered in the program are as follows:-\n";
    int h = 0;
    cout << "Pool Number\t\t Literal Index\n";
    while (pool_tab[h][1] != 0)
    {
        cout << "   " << pool_tab[h][0] << "\t\t\t    " << pool_tab[h][1] << endl;
        h++;
    }
    cout << "The literal table is as follows:\n";
    print_LIT();
    ff.close();
    return 0;
}