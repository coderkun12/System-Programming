#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

static int sspt = 0, kpdts = 1, ssts = 1, mdts = 1, kpdd = 0, index = 1, k = 0, SSTAB[50], evcount = 0, supflag = 0, mdtpp = 1;
char pntab[34][40], SSNTAB[56][34], evntabb[100][100];

struct KPDTAB
{
    char k[40], val[55];
    struct KPDTAB *next;
    int index;
};
struct KPDTAB *head_kp = NULL;

struct MNT
{
    char name[90];
    struct MNT *next;
    int pp, kp, ev, MDTP, SSTP, KPDTP, index;
};
struct MNT *head_mnt = NULL;

struct MDT
{
    struct MDT *next;
    int index;
    char inst[90];
};
struct MDT *head_mdt = NULL;

void add_kpdtab(char a[], char b[])
{
    int gh = 1;
    struct KPDTAB *mn = new KPDTAB;
    strcpy(mn->k, a);
    strcpy(mn->val, b);
    mn->next = NULL;
    if (head_kp == NULL)
    {
        mn->index = 1;
        head_kp = mn;
    }
    else
    {
        struct KPDTAB *u = head_kp;
        while (u->next != NULL)
        {
            gh = u->index;
            u = u->next;
        }
        mn->index = gh + 1;
        u->next = mn;
    }
}

void enter_into_MNT(char y[], int kp, int pp)
{
    struct MNT *utt = new MNT;
    utt->ev = 0;
    utt->KPDTP = kpdts;
    utt->pp = pp;
    utt->MDTP = mdts;
    utt->kp = kp;
    utt->SSTP = 0;
    utt->next = NULL;
    utt->index = index;
    index++;
    strcpy(utt->name, y);
    if (head_mnt == NULL)
    {
        head_mnt = utt;
    }
    else
    {
        struct MNT *h = head_mnt;
        while (h->next != NULL)
        {
            h = h->next;
        }
        h->next = utt;
    }
    kpdts += kp;
    ssts++;
}

void add_into_SSNTAB(char label[], int mm)
{
    int y = 0;
    while (y <= strlen(label))
    {
        SSNTAB[sspt][y] = label[y];
        y++;
    }
    SSTAB[sspt] = mm;
    sspt++;
    struct MNT *nj = head_mnt;
    while (nj->next != NULL)
    {
        nj = nj->next;
    }
    nj->SSTP += 1;
}

int search_pn(char str[100])
{
    int i = 0;
    while (i < k)
    {
        if (strcmp(str, pntab[i]) == 0)
        {
            return i;
        }
        i++;
    }
    return -1;
}

int search_evn(char bn[])
{
    int ho = 0;
    while (ho < evcount)
    {
        if (strcmp(evntabb[ho], bn) == 0)
        {
            return ho;
        }
        ho++;
    }
    return -1;
}

int search_SSN(char t[])
{
    int i = 0;
    while (i < sspt)
    {
        if (strcmp(SSNTAB[i], t) == 0)
        {
            return i;
        }
        i++;
    }
    return -1;
}

void process_body(char uo[])
{
    if (strcmp(uo, "START") == 0)
    {
        supflag = 1;
    }
    char *n;
    int kk = 0, mm = mdts, o = 0, i, j, t;
    char hapa[400], label[50], str[500], test[20];
    strcpy(hapa, uo);

    char *lol = strtok(hapa, " ");
    mm++;
    while (lol != NULL)
    {
        if (lol[0] == '.' && strcmp(lol, ".") != 0)
        {
            o = 0;
            i = 0;
            while (o <= strlen(lol))
            {
                if (lol[o] != '.')
                {
                    label[i] = lol[o];
                    i++;
                }
                o++;
            }
            add_into_SSNTAB(label, mm);
            str[kk] = 'S';
            kk++;
            str[kk] = ' ';
            kk++;
            str[kk] = '0' + sspt;
            kk++;
            str[kk] = ' ';
            kk++;
        }
        else if (lol[0] == '&' || lol[1] == '&')
        {
            i = 0;
            j = 0;
            while (i <= strlen(lol))
            {
                if (lol[i] != ',' && lol[i] != '&' && lol[i] != ')' && lol[i] != '(')
                {
                    test[j] = lol[i];
                    j++;
                }
                i++;
            }
            test[j] = '\0';
            if (search_pn(test) != -1)
            {
                t = search_pn(test);
                str[kk] = 'P';
                kk++;
                str[kk] = ' ';
                kk++;
                str[kk] = ('1' + t);
                kk++;
                str[kk] = ' ';
                kk++;
            }
            else if (search_evn(test) != -1)
            {
                t = search_evn(test);
                str[kk] = 'E';
                kk++;
                str[kk] = ' ';
                kk++;
                str[kk] = '1' + t;
                kk++;
                str[kk] = ' ';
                kk++;
            }
        }
        else
        {
            i = 0;
            if (search_SSN(lol) != -1)
            {
                t = search_SSN(lol);
                str[kk] = 'S';
                kk++;
                str[kk] = ' ';
                kk++;
                str[kk] = '0' + t;
                kk++;
                str[kk] = ' ';
                kk++;
            }
            else
            {
                while (i <= strlen(lol))
                {
                    str[kk] = lol[i];
                    i++;
                    kk++;
                }
                str[kk] = ' ';
                kk++;
            }
        }
        mm++;
        lol = strtok(NULL, " ");
    }
    if (supflag == 0)
    {
        cout << mdtpp << "\t";
        mdtpp++;
        i = 0;
        while (i < kk)
        {
            cout << str[i];
            i++;
        }
        cout << endl;
    }
}

void protpro(char str[])
{
    int j = 0, index, l;
    int i, pp = 0, kp = 0, flag;
    char name[40], rt[34], a[10], b[10];
    char *jjk;
    jjk = strtok(str, " ");
    strcpy(name, jjk);
    jjk = strtok(NULL, " ");
    while (jjk != NULL)
    {
        flag = 0;
        i = 0;
        while (i != strlen(jjk) - 1)
        {
            if (jjk[i] == '=')
            {
                index = i;
                flag = 1;
            }
            i++;
        }
        if (flag == 0)
        {
            pp++;
        }
        else
        {
            strcpy(rt, jjk);
            i = 0;
            j = 0;
            while (i <= index)
            {
                if (rt[i] != '&' && rt[i] != '=')
                {
                    a[j] = rt[i];
                    j++;
                }
                i++;
            }
            j = 0;
            while (i <= strlen(jjk))
            {
                if (rt[i] != '&' && rt[i] != '=')
                {
                    b[j] = rt[i];
                    j++;
                }
                i++;
            }
            add_kpdtab(a, b);
            kp++;
            kpdd++;
        }
        if (jjk[0] == '&')
        {
            j = 0;
            i = 0;
            while (i != strlen(jjk) - 1)
            {
                if (jjk[i] != '=')
                {
                    if (jjk[i] != '&' && jjk[i] != ',')
                    {
                        pntab[k][j] = jjk[i];
                        j++;
                    }
                    i++;
                }
                else
                {
                    break;
                }
            }
            k++;
        }
        jjk = strtok(NULL, " ");
    }
    enter_into_MNT(name, kp, pp);
}

void add_evtab(char g[])
{
    char *b = strtok(g, " ");
    b = strtok(NULL, " ");
    int j = 0, kp = 0;
    while (j <= strlen(b))
    {
        if (b[j] != '&')
        {
            evntabb[evcount][kp] = b[j];
            kp++;
        }
        j++;
    }
    evntabb[evcount][kp] = '\0';
    evcount++;
    struct MNT *h = head_mnt;
    while (h->next != NULL)
    {
        h = h->next;
    }
    h->ev += 1;
}

int main()
{
    char abc[40];
    char *ptr;
    int i = 0, flag = 0, j;
    fstream op, gg;
    op.open("MACRO1.txt");
    cout << "The Contents of MACRO Definition Table are:\n";
    while (!op.eof())
    {
        op.getline(abc, 50);
        if (flag == 0)
        {
            if (strcmp(abc, "MACRO") == 0)
            {
                flag = 1;
                i = 0;
            }
        }
        if (flag == 1)
        {
            if (i == 1)
            {
                protpro(abc);
            }
            else if (i > 1 && flag == 1)
            {
                if (abc[0] == 'L' && abc[1] == 'C' && abc[2] == 'L')
                {
                    add_evtab(abc);
                }
                process_body(abc);
            }
            i++;
            if (strcmp(abc, "MEND") == 0)
            {
                flag = 0;
                mdts += (i - 2);
            }
        }
    }
    cout << "\nMACRO Name Table is as follows:" << endl;
    cout << "Name\t#pp\t#kp\t#ev\tKPDTP\tMDTP\tSSTP" << endl;
    struct MNT *hj = head_mnt;
    while (hj != NULL)
    {
        cout << hj->name << "\t " << hj->pp << "\t" << hj->kp << "\t" << hj->ev << "\t" << hj->KPDTP << "\t " << hj->MDTP << "\t" << hj->SSTP << endl;
        hj = hj->next;
    }
    cout << "\n\nThe Keyword Parameter Table is as follows:\nIndex\tValue\tDefault Value\n";
    struct KPDTAB *jk = head_kp;
    while (jk != NULL)
    {
        cout << jk->index << "\t" << jk->k << "\t" << jk->val << endl;
        jk = jk->next;
    }
    cout << "\nThe Parameter Name Table is as follows:\nParameter";
    i = 0;
    while (i < k)
    {
        cout << "\n   "
             << pntab[i];
        i++;
    }
    i = 0;
    cout << "\n\nThe Sequencing Symbol Name Table Contains:\n          Name";
    while (i < sspt)
    {
        cout << "\n          "
             << SSNTAB[i];
        i++;
    }
    i = 0;
    cout << "\n\nThe Sequencing Symbol Table Contains:\n          Value";
    while (i < sspt)
    {
        cout << "\n            "
             << SSTAB[i];
        i++;
    }
    i = 0;
    cout << "\n\nContents of Expansion Time Variable Name Table are:\n\t\tVariable Name\n";
    while (i < evcount)
    {
        cout << "\t          \t" << evntabb[i] << endl;
        i++;
    }
    return 0;
}