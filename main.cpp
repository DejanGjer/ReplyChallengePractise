#include <bits/stdc++.h>
using namespace std;
#define MAXNM 2000
#define MAXR 500
#define UNVISITED 0
#define VISITED 1
typedef long long ll;
typedef pair<int,int> pii;
typedef pair<ll,ll> pll;
typedef vector<int> vi;
typedef vector<ll> vll;

#define p_b push_back
#define m_p make_pair
#define f first
#define s second
#define sz(x) (int)(x).size()
#define forn(a,b,c) for(int a=b;a<c;a++)
#define ford(a,b,c) for(int a=b;a>=c;a--)
int N,M,C,R;
struct customer{
    int x,y;
    int reward;
};
vector<customer> customers;
int mapa[MAXNM][MAXNM], komp[MAXNM][MAXNM],dist[MAXNM][MAXNM];
pii parent[MAXNM][MAXNM];
bitset<MAXNM*MAXNM> prosao;
bitset<MAXNM*MAXNM> office;

int cell(char c)
{
    switch(c){
        case 'T':
            return 50;
        case 'H':
            return 70;
        case '_':
            return 100;
        case 'X':
            return 120;
        case '+':
            return 150;
        case '*':
            return 200;
        case '~':
            return 800;
        default:
            return -1;
    }
}

vector<pii> susedi(int x, int y)
{
    int neighx[] = {0,1,0,-1};
    int neighy[] = {1,0,-1,0};
    vector<pii> su;
    forn(i,0,4)
    {
        if(x + neighx[i] >= 0 && x + neighx[i] < N && y + neighy[i] >= 0 && y + neighy[i] < M)
            if(mapa[y + neighy[i]][x + neighx[i]] != -1)
                su.push_back(make_pair(x + neighx[i], y + neighy[i]));
    }
    return su;
}

string direction(int x1, int y1 ,int x2, int y2)
{
    if(x2 == x1 && y2 == y1 + 1)
        return "D";
    else if(x2 == x1 && y2 == y1 - 1)
        return "U";
    else if(x2 == x1 + 1 && y2 == y1)
        return "R";
    else if(x2 == x1 - 1 && y2 == y1)
        return "L";
    else
        return "";
}

void komponente_pom(int x, int y,int label)
{
    prosao[y*N + x] = VISITED;
    komp[y][x] = label;
    vector<pii> su = susedi(x,y);
    //cout << sz(su) << endl;
    for(int i = 0; i < sz(su); i++)
    {
        int w = su[i].first, h = su[i].second;
        if(prosao[h*N + w] == UNVISITED)
            komponente_pom(w,h,label);
    }
}

int komponente(int x, int y)
{
    forn(i,0,M)
        forn(j,0,N)
            komp[i][j] = -1;

    int br = 0;
    forn(i,0,M)
    {
        forn(j,0,N)
        {
            if(mapa[i][j] != -1 && komp[i][j] == -1)
            {
                komponente_pom(j,i,br);
                br++;
            }
        }
    }
    return br;
}

pii dijkstra(int x,int y)
{
    priority_queue<pair<int,pii>, vector<pair<int,pii>>, greater<pair<int,pii>>> pq;
    prosao.reset();
    forn(i,0,M)
        forn(j,0,N)
        {
            dist[i][j] = INT_MAX;
            parent[i][j] = make_pair(-1,-1);
        }
    dist[y][x] = 0;
    pq.push(make_pair(0,make_pair(x,y)));
    while(!pq.empty())
    {
        pair<int,pii> node = pq.top();
        int w = node.second.first;
        int h = node.second.second;
        prosao[h*N + w] = VISITED;
        pq.pop();
        if(office[h*N + w] == 1)
            return make_pair(w,h);
        if(node.first == dist[h][w])
        {
            vector<pii> su = susedi(w,h);
            for(int i = 0; i < sz(su); i++)
            {
                int new_x = su[i].first, new_y = su[i].second;
                if((dist[new_y][new_x] > dist[h][w] + mapa[new_y][new_x]) && prosao[new_y*N + new_x] == UNVISITED)
                {
                    dist[new_y][new_x] = dist[h][w] + mapa[new_y][new_x];
                    parent[new_y][new_x] = make_pair(w,h);
                    pq.push(make_pair(dist[new_y][new_x], make_pair(new_x,new_y)));
                }
            }
        }
    }
    return make_pair(-1,-1);
}

string path(int x1, int y1, int x2, int y2)
{
    if(parent[y2][x2] == make_pair(-1,-1))
        return "";
    return direction(x2,y2,parent[y2][x2].first,parent[y2][x2].second) + path(x1,y1,parent[y2][x2].first,parent[y2][x2].second);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    freopen("1_victoria_lake.txt", "r", stdin);
    cin >> N >> M >> C >> R;
    forn(i,0,C)
    {
        int x,y,r;
        cin >> x >> y >> r;
        customer new_c;
        new_c.x = x;
        new_c.y = y;
        new_c.reward = r;
        customers.push_back(new_c);
    }
    forn(i,0,M)
    {
        forn(j,0,N)
        {
            char c;
            cin >> c;
            mapa[i][j] = cell(c);
        }
    }
    prosao.reset();
    int br_komp = komponente(0,0);
    cout << br_komp << endl;
    forn(i,0,M)
        forn(j,0,N)
            office[i*N + j] = 0;
    //office[10*N + 8] = 1;
    //office[5*N + 14] = 1;
    //pii p = dijkstra(2,1);
    //cout << p.first << " " << p.second << endl;
    //cout << dist[10][8] << endl;
    //cout << dist[5][14] << endl;
    //cout << path(2,1,8,10) << endl;
    office[2*N + 7] = 1;
    office[15*N + 10] = 1;
    office[20*N + 20] = 1;
    office[22*N + 29] = 1;
    office[9*N + 31] = 1;
    office[9*N + 41] = 1;
    office[30*N + 38] = 1;
    office[31*N + 48] = 1;
    freopen("test1.txt","w",stdout);
    for(int i = 0; i < sz(customers); i++)
    {
        customer cu = customers[i];
        pii office_coord = dijkstra(cu.x, cu.y);
        cout << office_coord.first << " " << office_coord.second << " ";
        cout << path(cu.x, cu.y, office_coord.first, office_coord.second) << endl;
    }
    return 0;
}
