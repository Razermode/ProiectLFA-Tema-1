#include <bits/stdc++.h>
#include <queue>
#include <vector>
#include <unordered_map>
#include <cstring>
#define NMAX 10000

using namespace std;

ifstream fin("nfa.in");
ofstream fout("nfa.out");

int v[NMAX][NMAX], k, m, n, q0, l, x, solutii;
unordered_map <char, vector<int>> M[NMAX];
unordered_map <string, bool> Viz[NMAX];
unordered_map <string, bool> gasit;
vector <vector <int>> inapoi;
vector <int> accesibil, vizitari;
queue <int> Q1,Q2;
queue < pair <int, string> > Q;
char s[302];
bool F[NMAX];

bool evaluate(char s[], int cuv)
{

    int lung = strlen(s);
    Q1.push(q0);
    v[cuv][q0] = 1;
    for(int i = 0; i < lung; i++)
    {
        int j = i+1;
        if(j%2)
        {
            if(Q1.empty())
                return false;
            while(!Q1.empty())
            {
                int x = Q1.front();
                Q1.pop();

                for(int y : M[x][s[i]])
                    if(v[cuv][y] != j)
                    {
                        v[cuv][y] = j;
                        Q2.push(y);
                    }
            }
        }
        else
        {
            if (Q2.empty())
                return false;
            while(!Q2.empty())
            {
                int x = Q2.front();
                Q2.pop();
                for(int y : M[x][s[i]])
                    if(v[cuv][y] != j)
                    {
                        v[cuv][y] = j;
                        Q1.push(y);
                    }
            }
        }
    }
    if((lung+1)%2)
        while(!Q1.empty())
        {
            int x = Q1.front();
            Q1.pop();

            if (F[x])
                return true;
        }
    else
        while(!Q2.empty())
        {
            int x = Q2.front();
            Q2.pop();
            if (F[x])
                return true;
        }
    return false;
}

void solve_prob()
{

    if (F[q0] == 1)
    {
        solutii++;
        fout << "Cuvantul vid\n";
    }
    Q.push({q0,""});
    while (!Q.empty())
    {
        int stare = Q.front().first;
        string cuv_actual = Q.front().second;
        Q.pop();
        for(const auto &y: M[stare])
        {
            char litera = y.first;
            vector <int> vecini = y.second;
            string aux = cuv_actual;
            aux.push_back(litera);
            for(int z : vecini)
            {
                if(accesibil[z])
                {
                    if (Viz[z][aux] == 0)
                    {
                        Viz[z][aux] = 1;
                        Q.push({z,aux});
                        if (F[z] == 1 && (gasit[aux] == 0))
                        {
                            gasit[aux] = 1;
                            solutii++;
                            fout << aux << '\n';
                            if(solutii == 100)
                                return;
                        }
                    }
                }
            }
        }
    }
}

int main ()
{

    fin >> n >> m >> k;
    inapoi = vector <vector <int>> (n+5);
    fin >> q0;
    for(int i = 0; i < k; i++)
    {
        fin >> x;
        F[x] = 1;
    }
    for(int i = 1; i <= m; i++)
    {
        int x, y;
        char a;
        fin >> x >> y >> a;
        M[x][a].push_back(y);
        inapoi[y].push_back(x);
    }
    fin >> l;
    for (int i = 1; i <= l; ++i)
    {
        fin >> s;
        if(evaluate(s,i))
            fout << 1 << '\n';
        else
            fout << 0 << '\n';
    }
    /*
        Folosim vectorul de accesibil si de backwards deoarece, in caz ca avem noduri care cicleaza (exemplu o muchie (3,3), iar 3 nu e nod final) programul va cicla
        de aceeea are sens sa parcurgem doar nodurile in care putem ajunge eventual intr-o stare finala, lucru pe care il vom determina cu un bts cu o "coada" (vectorul vizitari)
    */

    accesibil = vector <int> ((n+5),0);
    accesibil[0] = 0;
    for(int i = 1; i <= n; ++i)
    {
        if (F[i])
        {
            accesibil[i] = 1;
            vizitari.push_back(i);
        }
        else
            accesibil[i] = 0;
    }
    for(int i = 0; i < vizitari.size(); ++i){
        int chestie = vizitari[i];
        for(auto j : inapoi[chestie])
            if(!accesibil[j])
            {
                accesibil[j] = 1;
                vizitari.push_back(j);
            }
    }


    solve_prob();
}
