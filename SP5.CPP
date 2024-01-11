#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
static int apindex = 0, tot = 0, usedparcount = 1, upprev = 1;
struct kpdtab
{
    struct kpdtab *next;
    int index;
    char name[30], value[30];
};
struct kpdtab *kpd_head = NULL;
// Creates KPDTAB
void entrykpd(char t[])
{
    struct kpdtab *b = new kpdtab;
    b->next = NULL;
    int i = 0;
    char *n = strtok(t, " ");
    while (n != NULL)
    {
        if (i == 0)
        {
            b->index = atoi(n);
        }
        else if (i == 1)
        {
            strcpy(b->name, n);
        }
        else if (i == 2)
        {
            strcpy(b->value, n);
        }
        i++;
        n = strtok(NULL, " ");
    }
    if (kpd_head == NULL)
    {
        kpd_head = b;
    }
    else
    {
        struct kpdtab *y = kpd_head;
        while (y->next != NULL)
        {
            y = y->next;
        }
        y->next = b;
    }
}
struct mnt_tab
{
    struct mnt_tab *next;
    char name[40];
    int pp, kp, ev, kpdtp, mdtp, sstp;
};
struct mnt_tab *mnt_head = NULL;
struct mdt
{
    struct mdt *next;
    char t[50];
};
struct mdt *head_mdt = NULL;
// Creates entry in MDT
void entry_mdt(char g[])
{
    struct mdt *ab = new mdt;
    strcpy(ab->t, g);
    ab->next = NULL;
    if (head_mdt == NULL)
    {
        head_mdt = ab;
    }
    else
    {
        struct mdt *n = head_mdt;
        while (n->next != NULL)
        {
            n = n->next;
        }
        n->next = ab;
    }
}
// Creates entry in MNT
void entry_mnt(char b[])
{
    struct mnt_tab *nn = new mnt_tab;
    nn->next = NULL;
    int i = 0;
    char *g = strtok(b, " ");
    while (g != NULL)
    {
        if (i == 0)
        {
            strcpy(nn->name, b);
        }
        else if (i == 1)
        {
            nn->pp = atoi(g);
        }
        else if (i == 2)
        {
            nn->kp = atoi(g);
        }
        else if (i == 3)
        {
            nn->ev = atoi(g);
        }
        else if (i == 4)
        {
            nn->kpdtp = atoi(g);
        }
        else if (i == 5)
        {
            nn->mdtp = atoi(g);
        }
        else if (i == 6)
        {
            nn->sstp = atoi(g);
        }
        i++;
        g = strtok(NULL, " ");
    }
    if (mnt_head == NULL)
    {
        mnt_head = nn;
    }
    else
    {
        struct mnt_tab *yo = mnt_head;
        while (yo->next != NULL)
        {
            yo = yo->next;
        }
        yo->next = nn;
    }
}
struct apttab
{
    int index;
    struct apttab *next;
    char a[50] = "FIFO";
};
struct apttab *head_apt = NULL;
// Creates entry in aptab
void add_aptab(char t[])
{
    struct apttab *g = new apttab;
    g->index = apindex + 1;
    apindex += 1;
    g->next = NULL;
    int i = 0, k = 0;
    while (i < strlen(t))
    {
        if (t[i] != ',' && t[i] != ')' && t[i] != '(')
        {
            g->a[k] = t[i];
            k++;
        }
        i++;
    }
    g->a[k] = '\0';
    if (head_apt == NULL)
    {
        head_apt = g;
    }
    else
    {
        struct apttab *a = head_apt;
        while (a->next != NULL)
        {
            a = a->next;
        }
        a->next = g;
    }
    tot--;
}
// Processes expansion and prints it.
void process_expansion(int mdtp, int pp, int kp)
{
    int index;
    char huh;
    char *p;
    char l[50], output[100];
    int j = 1, m = 0, i;
    int flag = 0, flag2;
    struct mdt *hhah = head_mdt;
    while (j != mdtp)
    {
        j++;
        hhah = hhah->next;
    }
    while (strcmp(hhah->t, "MEND") != 0)
    {
        j = 0;
        char op[50];
        flag = 0;
        strcpy(l, hhah->t);
        while (j < strlen(hhah->t))
        {
            if (hhah->t[j] == 'S' && hhah->t[j + 1] == 'E' && hhah->t[j + 2] == 'T')
            {
                flag = 1;
                break;
            }
            j++;
        }
        p = strtok(l, " ");
        if ((p[0] == 'L' && p[1] == 'C' && p[2] == 'L') || (flag == 1))
        {
        }
        else
        {
            while (p != NULL)
            {
                if (strlen(p) == 1)
                {
                    if ((p[0] >= 'A' && p[0] <= 'Z') || (p[0] >= 'a' && p[0] <= 'z'))
                    {
                        huh = p[0];
                        flag = 1;
                    }
                    else if (p[0] >= '0' && p[0] <= '9')
                    {
                        index = atoi(p);
                        if (flag == 1)
                        {
                            if (huh == 'P')
                            {
                                struct apttab *ui = head_apt;
                                i = upprev;
                                while (i < usedparcount + 1)
                                {
                                    j = 0;
                                    if (index == ui->index)
                                    {
                                        flag2 = 1;
                                        while (j < strlen(ui->a))
                                        {
                                            output[m] = ui->a[j];
                                            j++;
                                            m++;
                                        }
                                        break;
                                    }
                                    i++;
                                    ui = ui->next;
                                }
                                if (flag2 == 0)
                                {
                                    struct kpdtab *mn = kpd_head;
                                    j = 1;
                                    while (j != kp)
                                    {
                                        mn = mn->next;
                                        j++;
                                    }
                                    j = 0;
                                    while (j < strlen(mn->value))
                                    {
                                        output[m] = mn->value[j];
                                        m++;
                                        j++;
                                    }
                                }
                            }
                            output[m] = ' ';
                            m++;
                        }
                        flag = 0;
                    }
                }
                else
                {
                    i = 0;
                    if (strcmp(p, "MEND") != 0)
                    {
                        while (i < strlen(p))
                        {
                            output[m] = p[i];
                            m++;
                            i++;
                        }
                        output[m] = ' ';
                        m++;
                    }
                }
                p = strtok(NULL, " ");
            }
        }
        output[m] = '\0';
        cout << output << endl;
        m = 0;
        hhah = hhah->next;
    }
}
void macroexpansion(char g[])
{
    char kobe[50];
    strcpy(kobe, g);
    int mdtp = 0, pp = 0, kp = 0;
    char *h = strtok(g, " ");
    struct mnt_tab *k = mnt_head;
    while (k != NULL)
    {
        if (strcmp(k->name, h) == 0)
        {
            pp = k->pp;
            kp = k->kp;
            mdtp = k->mdtp;
            break;
        }
        k = k->next;
    }
    tot = pp + kp;
    upprev = usedparcount;
    usedparcount += tot;
    h = strtok(NULL, " ");
    while (h != NULL)
    {
        add_aptab(h);
        h = strtok(NULL, " ");
    }
    if (tot == 0 || tot == 1)
    {
        apindex = 0;
    }
    process_expansion(mdtp, pp, kp);
}
void maincode(char a[])
{
    char b[50];
    strcpy(b, a);
    int flag = 0;
    char *bb = strtok(a, " ");
    struct mnt_tab *abc = mnt_head;
    while (abc != NULL)
    {
        if (strcmp(bb, abc->name) == 0)
        {
            flag = 1;
            break;
        }
        abc = abc->next;
    }
    if (flag == 0)
    {
        cout << b << endl;
    }
    else
    {
        macroexpansion(b);
    }
}
int main()
{
    char abc[100];
    fstream ok;
    ok.open("MNT.txt");
    while (!ok.eof())
    {
        ok.getline(abc, 40);
        entry_mnt(abc);
    }
    ok.close();
    ok.open("KPDTAB.txt");
    while (!ok.eof())
    {
        ok.getline(abc, 40);
        entrykpd(abc);
    }
    ok.close();
    ok.open("MDT.txt");
    while (!ok.eof())
    {
        ok.getline(abc, 40);
        entry_mdt(abc);
    }
    ok.close();
    ok.open("MacroExpansion.txt");
    while (!ok.eof())
    {
        ok.getline(abc, 40);
        maincode(abc);
    }
    ok.close();
    struct apttab *aa = head_apt;
    cout << "The Actual Parameter tale is as follows:\nIndex\tValue\n";
    while (aa != NULL)
    {
        cout << aa->index << "\t" << aa->a << endl;
        aa = aa->next;
    }
    return 0;
}