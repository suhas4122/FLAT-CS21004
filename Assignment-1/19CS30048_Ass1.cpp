/*
Suhas Jain
19CS30048
SOftware Engineering Theory Assignment-3
*/

#include <bits/stdc++.h>
using namespace std;
typedef unsigned int uint;

class NFA
{
    public:
    uint n, m, s, f;
    uint** delta;
    NFA(uint a, uint b, uint c, uint d, uint** e)
    {
        n = a;
        m = b;
        s = c;
        f = d;
        delta = new uint*[n];
        for (uint i = 0; i < n; i++)
        {
            delta[i] = new uint[m];
            for(uint j = 0; j<m; j++)
            {
                delta[i][j] = e[i][j];
            }
        }
    }

    ~NFA()
    {
        for(uint i = 0; i < n; i++)
            delete [] delta[i];
        delete [] delta;
    }
};

class stateSet
{
    public:
    uint size;
    uint* states;

    stateSet(uint s = 0)
    {
        size = s;
        states = new uint[size];
        for(uint i = 0; i<size; i++)
            states[i] = 0;
    }

    ~stateSet()
    {
        delete [] states;
    }

    bool contains(uint x)
    {
        uint a = states[x/32U];
        uint b = 1U << (x%32);
        if(a & b)
            return true;
        return false;
    }

    void push(uint x)
    {
        states[x / 32U] |= (1U << (x % 32U));
    }
};

class DFA
{
    public:
    uint n, m, s;
    stateSet f;
    uint** delta;

    DFA(uint a, uint b, uint c): f(a/32 +1)
    {
        n = a;
        m = b;
        s = c;
        uint size = (n / 32) + 1;
        delta = new uint*[n];
        for(uint i = 0; i < n; i++)
        {
            delta[i] = new uint[m];
            for(uint j = 0; j < m; j++)
            {
                delta[i][j] = 0;
            }
        }
    }

    ~DFA()
    {
        for(uint i = 0; i < n; i++)
            delete [] delta[i];
        delete [] delta;
    }
};

NFA* readNFA(string str)
{
    uint n, m, s, f;
    ifstream ip(str);
    ip >> n >> m;
    int i = 0;
    s = 0;
    while(i != -1)
    {
        ip>>i;
        if(i != -1)
            s = s | (1U << i);
    }
    i = 0;
    f = 0;
    while(i != -1)
    {
        ip>>i;
        if(i != -1)
            f = f | (1U << i);
    }
    uint** delta = new uint *[n];
    for (i = 0; i < n; i++)
    {
        delta[i] = new uint[m];
        for(uint j = 0; j<m; j++)
        {
            delta[i][j] = 0;
        }
    }
    i = 0;
    while(i != -1)
    {
        uint q, r;
        ip >> i;
        if (i != -1)
        {
            ip>> q>> r;
            delta[i][q] |= (1U << r);
        }
    }
    NFA* nfa = new NFA(n, m, s, f, delta);
    delete [] delta;
    return nfa;
}

void printSubset(uint p)
{
    uint position = 0;
    cout<<"{";
    while (p != 0) 
    {
        if((p & 1)==1)
        {
            cout << position;
            if(p >> 1 != 0)
                cout<<",";
        }
        position += 1;
        p = p >> 1;
    }
    cout<<"}";
}

void printNFA(NFA* N)
{
    cout << "\n    " << "Number of states: " << N->n;

    cout << "\n    " << "Input alphabet: {";
    cout<<0;
    for(uint i = 1; i<N->m; i++)
    {
        cout << "," << i;
    }
    cout<<"}";

    cout << "\n    " << "Start states: ";
    printSubset(N->s);

    cout << "\n    " << "Final states: ";
    printSubset(N->f);

    cout << "\n    " << "Transition function";
    for(uint i = 0; i < N->n; i++)
    {
        for(uint j = 0; j < N->m; j++)
        {
            cout << "\n    " << "    " << "Delta(" << i << "," << j << ") = ";
            printSubset(N->delta[i][j]);
        }
    }
    cout << "\n\n";
} 

void printDFA(DFA* dfa)
{
    cout<<"\n";
    cout << "    " << "Number of states: "<<dfa->n;
    cout<<"\n";
    cout << "    " << "Input alphabet: {0";
    for(uint i = 1; i<dfa->m; i++)
    {
        cout << "," << i;
    }
    cout << "}";
    cout << "\n";
    cout << "    " << "Start state: " << dfa->s;
    cout << "\n";
    
    uint count = 0;
    for(uint i = 0; i<dfa->f.size; i++)
    {
        uint count1 = 0;
        uint n = dfa->f.states[i];
        while(n)
        {
            n &= (n - 1);
            count1++;
        }
        count += count1;
    }

    if(count >= 64)
    {
        cout << "    " << count << " final states\n";
    }
    else
    {
        cout << "    " << "Final states: {";
        int flag = 1;
        for(uint i = 0; i < dfa->n; i++)
        {
            if(dfa->f.contains(i))
            {
                if(flag == 1)
                {
                    cout << i;
                    flag = 0;
                }
                else 
                    cout << "," << i;
            }
        }
        cout << "}\n";
    }

    if(count >= 64)
    {
        cout << "    " << "Transition function: Skipped\n";
    }
    else
    {
        cout << "    " << "Transition function\n";
        cout << "    " << "a\\p|";
        for(uint i = 0; i<dfa->n; i++)
        {
            if(i>9)
            {
                cout << " " << i;
            }
            else
            {
                cout << "  " << i;
            }
            if(i == dfa->n-1)
            {
                cout << "\n    ---+";
            }
        }
        for(uint i = 0; i<dfa->n; i++)
        {
            cout << "---";
        }
        cout << "\n";
        for(uint i = 0; i<dfa->m; i++)
        {
            if(i>9)
            {
                cout << "    " << i << " |"; 
            }
            else
            {
                cout << "     "  << i << " |";
            }
            for(uint j = 0; j<dfa->n; j++)
            {
                if(dfa->delta[j][i] > 9)
                {
                    cout << " " << dfa->delta[j][i];
                }
                else
                {
                    cout << "  " << dfa->delta[j][i];
                }
            }
            cout<<"\n";
        }
    }
    cout<<"\n";
}

DFA* subsetcons(NFA* nfa)
{
    DFA* dfa = new DFA((1U<<(nfa->n)), nfa->m, nfa->s);

    for(uint i = 0; i < dfa->n; i++)
    {
        for(uint j = 0; j < nfa->n; j++)
        {
            if(i & (1U << j))
            {
                for(uint k = 0; k < dfa->m; k++)
                {
                    dfa->delta[i][k] |= nfa->delta[j][k];
                }
            }
        }
    }

    for(uint i = 0; i < nfa->n; i++)
    {
        if((nfa->f) & (1U << i))
        {
            for(uint j = 0; j < dfa->n; j++)
            {
                if(j & (1U << i))
                {
                    dfa->f.push(j);   
                }
            }
        }
    }
    return dfa;
}

class Stack 
{
    public:
    uint top;
    uint length;
    const static int BOUND = 8;
    uint* a;

    Stack()
    {
        top = -1;
        length = BOUND;
        a = new uint[length + BOUND];
    }

    ~Stack()
    {
        delete [] a;
    }
    
    uint* create_new()
    {
        uint* new_a = new uint[length + BOUND];
    
        for (uint i = 0; i < length; i++)
            new_a[i] = a[i];

        length += BOUND;
        return new_a;
    }

    void push(uint element)
    {
        if (top == length - 1)
            a = create_new();
    
        a[++top] = element;
    }
    
    uint pop()
    {
        if(top >= 0)
        {
            return a[top--];
        }
        else
        {
            cout << "Empty Stacked popped\n";
            exit(1);
        }
    }
    
    bool empty()
    {
        if (top == -1)
            return true;
        return false;
    }
};

stateSet* findreachable(DFA* dfa)
{
    stateSet* v = new stateSet(((dfa->n)/32U)+1);

    Stack s;
    s.push(dfa->s);
    v->push(dfa->s);

    while (!s.empty())
    {
        uint current = s.pop();

        for(uint i = 0; i<dfa->m; i++)
        {
            uint check = v->contains(dfa->delta[current][i]);
            if(check == 0)
            {
                s.push(dfa->delta[current][i]);
                v->push(dfa->delta[current][i]);
            }
        }
    }
    return v;
}

void printStates(stateSet* s)
{
    cout<<"{";
    uint i, curr = 0;
    uint count = 0;
    for(uint i = 0; i<s->size; i++)
    {
        uint count1 = 0;
        uint n = s->states[i];
        while(n)
        {
            n &= (n - 1);
            count1++;
        }
        count += count1;
    }
    uint* reachable = new uint[count];
    uint k = 0;
    for(uint i = 0; i<32U*s->size; i++)
    {
        if(s->contains(i))
            reachable[k++] = i;
    }
    int flag = 1;
    for(uint i = 0; i<count; i++)
    {
        if(flag)
        {
            cout << reachable[i];
            flag = 0;
        }
        else
            cout << "," << reachable[i];
    }
    cout<<"}\n\n";
}

uint bsearch(uint*arr, uint l, uint r, uint x)
{
    if (r >= l) 
    { 
        uint mid = (r + l)/2; 
        if (arr[mid] == x)  return mid; 
        if (arr[mid] > x) return bsearch(arr, l, mid-1, x); 
        return bsearch(arr, mid+1, r, x); 
    } 
    return -1; 
}

DFA* rmunreachable(DFA* dfa, stateSet* s)
{
    uint count = 0;
    for(uint i = 0; i<s->size; i++)
    {
        uint count1 = 0;
        uint n = s->states[i];
        while(n)
        {
            n &= (n - 1);
            count1++;
        }
        count += count1;
    }
    uint* reachable = new uint[count];
    uint k = 0;
    for(uint i = 0; i<32U*s->size; i++)
    {
        if(s->contains(i))
            reachable[k++] = i;
    }
    
    DFA* new_dfa = new DFA(count, dfa->m, bsearch(reachable, 0, count-1, dfa->s));

    for(uint i = 0; i<new_dfa->n; i++)
    {
        if(dfa->f.contains(reachable[i]))
        {
            new_dfa->f.push(i);
        }
        for(uint j = 0; j<dfa->m; j++)
        {
            new_dfa->delta[i][j] = bsearch(reachable, 0, count-1, dfa->delta[reachable[i]][j]);
        }
    }

    return new_dfa;
}

uint** findequiv(DFA* dfa)
{
    uint** m = new uint*[dfa->n];
    for(uint i = 0; i<dfa->n; i++)
    {
        m[i] = new uint[dfa->n];
        for(uint j = 0; j<dfa->n; j++)
        {
            m[i][j] = 0;
        }
    }

    for(uint i = 0; i<dfa->n; i++)
    {
        for(uint j = i+1; j<dfa->n; j++)
        {
            if((dfa->f.contains(i) && !dfa->f.contains(j)) || (dfa->f.contains(j) && !dfa->f.contains(i)))
            {
                m[j][i] = 1;
            }
        }
    }

    uint found = 1;
    uint ind = 1;

    while(found)
    {
        found = 0;
        for(uint j = 0; j < dfa->n; j++)
        {
            for(uint i = j + 1; i < dfa->n; i++)
            {
                for(uint k = 0; k < dfa->m; k++)
                {
                    if(((m[dfa->delta[i][k]][dfa->delta[j][k]] == ind) || (m[dfa->delta[j][k]][dfa->delta[i][k]] == ind)) && m[i][j] == 0)
                    {
                        found++;
                        m[i][j] = ind + 1;
                        break;
                    }
                }
            }
        }
        ind++;
    }
    return m;
}

DFA* collapse(DFA* dfa, uint**M)
{
    uint* tg = new uint[dfa->n];
    uint* allot = new uint[dfa->n];
    uint* groupallot = new uint[dfa->n];
    uint** groups = new uint*[dfa->n];

    for(uint i = 0; i<dfa->n; i++)
    {
        allot[i] = 0;
        groupallot[i] = 0;
    }

    uint temp = 0, ind = 0;
    for(uint j = 0; j < dfa->n; j++)
    {
        if(allot[j] == 0)
        {
            for(uint i = 0; i<dfa->n; i++)
            {
                tg[i] = 0;
            } 

            ind = 0;
            allot[j] = 1;
            groupallot[j] = temp;

            for(uint i = j + 1; i < dfa->n; i++)
            {
                if(M[i][j] == 0)
                {
                    tg[ind] = i;
                    allot[i] = 1;
                    groupallot[i] = temp;
                    ind++;
                }
            }

            groups[temp] = new uint[ind + 2];

            for(uint i = 0; i< ind+2 ; i++)
            {
                groups[temp][i] = 0;
            }

            groups[temp][0] = j;

            uint i = 0;
            while(i<ind)
            {
                groups[temp][i + 1] = tg[i];
                i++;
            }
            groups[temp][ind + 1] = 0;
            temp++;
        }
    }

    for(uint i = 0; i < temp; i++)
    {
        if(i > 9)
            cout << "\n    " << "Group " << i << ": {" << groups[i][0];
        else
            cout << "\n    " << "Group  " << i << ": {" << groups[i][0];
        
        uint j = 1;
        while(groups[i][j] != 0)
        {
            cout << "," << groups[i][j];
            j++;
        }
        cout << "}";
    }
    cout << "\n\n";

    DFA* D11 = new DFA(temp, dfa->m, groupallot[dfa->s]);

    for(uint i = 0; i < dfa->n; i++)
    {
        if(dfa->f.contains(i))
        {
            D11->f.push(groupallot[i]);
        }     
    }

    for(uint i = 0; i < dfa->n; i++)
    {   
        for(uint j = 0; j < dfa->m; j++)
        {
            uint g = groupallot[i];
            D11->delta[g][j] = groupallot[dfa->delta[i][j]];
        }
    }
    return D11;
}

int main()
{
    string s;
    cout <<"Enter file name : ";
    cin >> s;
    cout << endl;
    NFA* a = readNFA(s);
    cout << "+++ Input NFA";
    printNFA(a);

    DFA* b = subsetcons(a);
    cout << "+++ Converted DFA";
    printDFA(b); 

    stateSet* c = findreachable(b);
    cout << "+++ Reachable states: ";
    printStates(c);

    DFA* d = rmunreachable(b, c);
    cout << "+++ Reduced DFA after removing unreachable states";
    printDFA(d);

    uint** e = findequiv(d);
    cout << "+++ Equivalent states";
    DFA* f = collapse(d, e);
    cout << "+++ Reduced DFA after collapsing equivalent states";
    printDFA(f);
}