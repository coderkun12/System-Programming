#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

using namespace std;
static int Carea_counter;
char burger[100];

struct SYMPTR
{
    char symb[40], val[4], lc[7];
    struct SYMPTR *next;
};

struct SYMPTR *sy_head;

void add_symb(char lab[], char add[], char val[])
{
    struct SYMPTR *coco = new SYMPTR;
    strcpy(coco->symb, lab);
    strcpy(coco->lc, add);
    strcpy(coco->val, val);
    coco->next = NULL;
    if (sy_head == NULL)
    {
        sy_head = coco;
    }
    else
    {
        struct SYMPTR *momo = sy_head;
        while (momo->next != NULL)
        {
            momo = momo->next;
        }
        momo->next = coco;
    }
}

void find_location(int g)
{
    struct SYMPTR *a = sy_head;
    int e = 1;
    while (e != g)
    {
        a = a->next;
        e++;
    }
    strcat(burger, a->lc);
    strcat(burger, " ");
}

void generate_MC(char str[])
{
    char *uk;
    char t[10];
    uk = strtok(str, " ");
    uk = strtok(NULL, " ");
    int j;
    if (strcmp("AS", uk) != 0)
    {
        strcpy(t, uk);
        uk = strtok(NULL, " ");
        while (uk != NULL)
        {
            // cout << t << "\t";
            if ((uk[0] >= 'A' && uk[0] <= 'Z') || (uk[0] >= 'a' && uk[0] <= 'z'))
            {
                strcpy(t, uk);
            }
            if (uk[0] >= '0' && uk[0] <= '9')
            {
                if (strcmp(t, "S") == 0)
                {
                    find_location(atoi(uk));
                }
                else
                {
                    strcat(burger, uk);
                    strcat(burger, " ");
                }
            }
            uk = strtok(NULL, " ");
        }
    }
}

int main()
{
    char abc[50], label[30], adress[5], val[10];
    char *ptr;
    cout << "Enter the Start of the Code Area: ";
    cin >> Carea_counter;
    fstream op, gg;
    gg.open("ST1.txt");
    while (!gg.eof())
    {
        gg.getline(abc, 50);
        ptr = strtok(abc, " ");
        strcpy(label, ptr);
        ptr = strtok(NULL, " ");
        strcpy(adress, ptr);
        ptr = strtok(NULL, " ");
        strcpy(val, ptr);
        add_symb(label, adress, val);
    }
    gg.close();
    cout << endl;
    int prev, cur, n = 0;
    op.open("IC1.txt");
    while (!op.eof())
    {
        op.getline(abc, 70);
        n++;
    }
    op.close();
    op.open("IC1.txt");
    int i = 0;
    while (!op.eof())
    {
        op.getline(abc, 50);
        if (i != 0 && i != (n - 1))
        {
            generate_MC(abc);
        }
        ptr = strtok(abc, " ");
        cur = atoi(ptr);
        if (i == 0 || i == 1)
        {
            prev = cur;
        }
        else
        {
            Carea_counter = Carea_counter + (cur - prev);
            prev = cur;
        }
        if (i != 0 && i != (n - 1))
        {
            cout << Carea_counter;
        }
        i++;
        cout << " " << burger << endl;
        memset(burger, NULL, sizeof(burger));
    }
    op.close();
    return 0;
}