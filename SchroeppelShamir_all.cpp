#include <bits/stdc++.h>

using namespace std;

const string ALPHABET = "abcdefghijklmnopqrstuvwxyz012345";

int n;
long long hash_num = 0, s[100], MOD;
string hashed_message;
vector <string> ansList;
vector <pair <long long, long long> > V[4];

bool getBit(long long v, int p)
{
    return (v >> p) & 1;
}

long long turnOn(long long v, int p)
{
    return (1LL << p) | v;
}

void input()
{
    cin >> n;
    n *= 5;
    MOD = 1LL << n;
    long long sum = 0;
    for (int i = 0; i < n; ++i)
    {
        string x;
        cin >> x;
        string y = "";
        for (auto c: x)
        {
            int v = ALPHABET.find(c);
            for (int j = 4; j >= 0; --j)
                y += (char)(48 + getBit(v, j));
        }

        reverse(y.begin(), y.end());
        for (int j = 0; j < n; ++j)
            if (y[j] == '1')
                s[i] += 1LL << j;
    }
}

class inc
{
    private:
        vector <pair <long long, long long> > *a, *b;
        priority_queue <pair <pair <long long, long long>, pair <int, int> > > pq;
    
    public:
        vector <long long> storage;

        inc(vector <pair <long long, long long> > *a, vector <pair <long long, long long> > *b)
        {
            this->a = a;
            this->b = b;
            for (int i = 0; i < b->size(); ++i)
                pq.push({{- ((*a)[0].first + (*b)[i].first), (*a)[0].second + (*b)[i].second}, {0, i}});
        }

        long long top()
        {
            if (pq.empty()) return LLONG_MAX;
            storage.clear();
            long long w = pq.top().first.first;
            long long state = pq.top().first.second;
            while (pq.size() && pq.top().first.first == w)
            {
                auto p = pq.top(); pq.pop();
                int i = p.second.first, j = p.second.second;
                storage.push_back((*a)[i].second + (*b)[j].second);
                if (i < a->size() - 1) 
                    pq.push({{- ((*a)[i + 1].first + (*b)[j].first), (*a)[i + 1].second + (*b)[j].second}, {i + 1, j}});
            }

            return - w;
        }
};

void calc_hash_num()
{
    cin >> hashed_message;
    reverse(hashed_message.begin(), hashed_message.end());
    long long pw32 = 1;
    for (int i = 0; i < n/5; ++i)
    {
        int k = ALPHABET.find(hashed_message[i]), x = 0;
        hash_num += k * pw32;
        pw32 *= 32LL;
    }
}

void backtrack(int k, int r, long long sum, long long state, int id)
{
    if (k > r)
        return V[id].push_back({(id < 2 ? 1 : - 1) * sum, state}), void();

    for (int i = 0; i < 2; ++i)
        if (i) 
            backtrack(k + 1, r, (sum + s[k]) % MOD, turnOn(state, k), id);
        else 
            backtrack(k + 1, r, sum, state, id);
}

void convert(long long stateX)
{
    string ans = "";
    for (int i = 0; i < n/5; ++i)
    {
        int order = 0;
        for (int j = 5 * i, k = 4; j < 5 * (i + 1); ++j, --k)
            order += getBit(stateX, j) * (1 << k);
        ans += ALPHABET[order];
    }
    
    ansList.push_back(ans);
}

void two_pointers(long long hash_num)
{
    inc lPart(&V[0], &V[1]), rPart(&V[2], &V[3]);
    long long last = - 1;
    while (1)
    {
        auto l = lPart.top(), r = last;
        if (r == - 1) last = r = rPart.top();
        if (l == LLONG_MAX || r == LLONG_MAX) 
            break;

        while (1)
        {
            if (r == LLONG_MAX) return;
            if (hash_num - l == - r) 
            {
                for (auto x: lPart.storage)
                    for (auto y: rPart.storage)
                        convert(x + y);
            }
            else if (hash_num - l > - r) 
            {
                last = r;
                break;
            }
            last = r = rPart.top();
        }
    }
}

int main()
{
    // freopen("input.txt", "r", stdin);

    input();
    calc_hash_num();

    backtrack(0, n / 4 - 1, 0, 0, 0);
    backtrack(n / 4, n / 2 - 1, 0, 0, 1);
    backtrack(n / 2, n / 2 + n / 4 - 1, 0, 0, 2);
    backtrack(n / 2 + n / 4, n - 1, 0, 0, 3);
    
    for (int i = 0; i < 4; ++i) sort(V[i].begin(), V[i].end());
    
    two_pointers(hash_num);
    two_pointers(hash_num + MOD);
    two_pointers(hash_num + 2 * MOD);
    two_pointers(hash_num + 3 * MOD);

    sort(ansList.begin(), ansList.end());
    for (auto x: ansList) cout << x << '\n';
    return 0;
}
