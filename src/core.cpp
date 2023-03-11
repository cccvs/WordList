#include "core.h"
#include "error.h"
#include "iostream"

int m, scc_cnt;  // count of valid edge, scc_set
int in[MAX_EDGE], out[MAX_EDGE], w[MAX_EDGE]; // out vertex, weight
string s[MAX_EDGE];
vector<int> v_out[MAX_VERTEX], v_in[MAX_VERTEX], v_self[MAX_VERTEX];  // adjacent edge list
int scc_class[MAX_VERTEX], w_self[MAX_VERTEX];  // scc_set class of a vertex, weight sum of self loop of a vertex
set<int> scc_set[MAX_SCC], scc_out[MAX_SCC], scc_in[MAX_SCC];   // vertex set, out edge set, in edge set


// stage 1
int num, top;   // dfs time, stack top
int dfn[MAX_VERTEX], low[MAX_VERTEX], stack[MAX_VERTEX];    // dfs time , backtracking value, vertex stack
bool vis_ver[MAX_VERTEX];  // whether complete visiting vertex

void tarjan(int x) {
    dfn[x] = low[x] = ++num;
    stack[top++] = x, vis_ver[x] = true;
    for (const auto &e: v_out[x]) {
        if (!dfn[out[e]]) {
            tarjan(out[e]);
            low[x] = min(low[x], low[out[e]]);
        } else if (vis_ver[out[e]])
            low[x] = min(low[x], dfn[out[e]]);
    }
    if (dfn[x] == low[x]) {
        int y;
        do {
            y = stack[--top];
            vis_ver[y] = false;
            scc_class[y] = scc_cnt;
            scc_set[scc_cnt].insert(y);
        } while (x != y);
        ++scc_cnt;
    }
}

void check_scc();

void check_iso_edge(char *words[], int len, bool iso_edge[]) {
    int in_deg[MAX_VERTEX], out_deg[MAX_VERTEX];
    for (int i = 0; i < MAX_VERTEX; ++i)
        in_deg[i] = 0, out_deg[i] = 0;
    for (int i = 0; i < MAX_EDGE; ++i)
        iso_edge[i] = false;
    for (int e = 0; e < len; ++e) {
        string str = words[e];
        ++out_deg[str[0] - 'a'], ++in_deg[str.back() - 'a'];
    }
    for (int e = 0; e < len; ++e) {
        string str = words[e];
        int x = (int) (str[0] - 'a'), y = (int) (str.back() - 'a');
        iso_edge[e] |= (x == y && in_deg[x] + out_deg[x] <= 2);    // single self loop
        iso_edge[e] |= (x != y && in_deg[x] + out_deg[y] <= 0);    // single isolated edge
    }
}

void init_graph(char *words[], int len, char jail, bool weight) {
    bool iso_edge[MAX_EDGE];
    for (int v = 0; v < MAX_VERTEX; ++v)
        v_out[v].clear(), v_in[v].clear(), v_self[v].clear();
    for (int scc = 0; scc < MAX_SCC; ++scc)
        scc_set[scc].clear(), scc_in[scc].clear(), scc_out[scc].clear();
    check_iso_edge(words, len, iso_edge);
    for (int e = 0; e < len; ++e) {
        if (iso_edge[e])
            continue;
        int x, y;
        if (!jail || jail != words[e][0]) {
            s[m] = words[e];
            x = s[m][0] - 'a', y = s[m].back() - 'a';
            in[m] = x, out[m] = y, w[m] = weight ? (int) s[m].length() : 1;
            if (x != y)
                v_out[x].push_back(m), v_in[y].push_back(m);
            else
                v_self[x].push_back(m), w_self[x] += w[m];
            ++m;
        }
    }
    for (int x = 0; x < MAX_VERTEX; ++x) {
        if (!dfn[x])
            tarjan(x);
    }
    for (int e = 0; e < m; ++e) {
        int x = scc_class[in[e]], y = scc_class[out[e]];
        if (x != y)
            scc_out[x].insert(e), scc_in[y].insert(e);
    }
    check_scc();
}

bool has_loop() {
    // TODO[1]: 打印自环信息
    for (int v = 0; v < MAX_VERTEX; ++v) {
        int self_loop = 0;
        for (const auto &e: v_out[v])
            self_loop += (v == out[e]) ? 1 : 0;
        if (self_loop > 1)
            return true;
    }
    for (int scc = 0; scc < scc_cnt; ++scc) {
        if (scc_set[scc].size() > 1)
            return true;
    }
    return false;
}

// stage 2, get all chains
vector<string> ans;
vector<string> path;
bool vis_edge[MAX_EDGE];

void path_to_ans() {
    if (path.size() > 1) {
        string chain;
        for (int i = 0;; ++i) {
            chain += path[i];
            if (i == (int) path.size() - 1)
                break;
            chain += " ";
        }
        ans.push_back(chain);
    }
}

int dfs_all(int x) {
    if (ans.size() > MAX_CHAIN)
        return -TOO_MANY_CHAINS;
    for (const auto &e: v_out[x]) {
        if (!vis_edge[e]) {
            vis_edge[e] = true, path.push_back(s[e]);
            path_to_ans();
            dfs_all(out[e]);
            vis_edge[e] = false, path.pop_back();
        }
    }
    return 0;
}

// stage 3, solve dag
vector<int> vertex_seq;

void topo_vertex() {
    int in_deg[MAX_VERTEX];
    queue<int> q;
    while (!q.empty())
        q.pop();
    for (int v = 0; v < MAX_VERTEX; ++v) {
        in_deg[v] = (int) v_in[v].size();
        if (!in_deg[v])
            q.push(v);
    }
    while (!q.empty()) {
        int v = q.front();
        q.pop(), vertex_seq.push_back(v);
        for (const auto &e: v_out[v]) {
            int u = out[e];
            --in_deg[u];
            if (!in_deg[u])
                q.push(u);
        }
    }
    assert(vertex_seq.size() == MAX_VERTEX);
}

int solve_dag(char head, char tail) {
    ans.clear(), path.clear();
    topo_vertex();
    // dp, max_len[v]: max length when tail is v
    int in_edge[MAX_VERTEX], max_len[MAX_VERTEX], pre_edge[MAX_EDGE];
    for (int v = 0; v < MAX_VERTEX; ++v)
        in_edge[v] = -1, max_len[v] = (!head || (head == char(v + 'a'))) ? w_self[v] : INT32_MIN;
    for (int e = 0; e < m; ++e)
        pre_edge[e] = -1;
    for (const auto &v: vertex_seq) {
        if (!v_self[v].empty()) // link first self loop with previous edge
            pre_edge[v_self[v][0]] = in_edge[v];
        for (int i = 0; i < (int) v_self[v].size() - 1; ++i)  // link all self loops
            pre_edge[v_self[v][i + 1]] = v_self[v][i];
        for (const auto &e: v_out[v]) { // v->u
            int u = out[e];
            pre_edge[e] = v_self[v].empty() ? in_edge[v]
                                            : v_self[v].back();    // link out edge with (last self loop/in edge)
            if (max_len[v] + w[e] + w_self[u] > max_len[u])
                max_len[u] = max_len[v] + w[e] + w_self[u], in_edge[u] = e;
        }
    }
    // get tail edge
    int tail_e = -1, sum = INT32_MIN;
    for (int e = 0; e < m; ++e) {
        int x = in[e], y = out[e];
        if (tail && (tail != s[e].back()))
            continue;
        if (pre_edge[e] < 0)
            continue;
        if (x == y && sum < max_len[x])
            sum = max_len[x], tail_e = v_self[x].back();
        if (x != y && sum < max_len[x] + w[e])
            sum = max_len[x] + w[e], tail_e = e;
    }
    if (tail_e < 0)
        return -CHAIN_NOT_EXIST;
    // get edge list
    vector<int> reverse_edge;
    reverse_edge.clear();
    int e = tail_e;
    while (e >= 0) {
        reverse_edge.push_back(e);
        e = pre_edge[e];
    }
    // get ans
    ans.clear();
    for (int i = (int) reverse_edge.size() - 1; i >= 0; --i)
        ans.push_back(s[reverse_edge[i]]);
    return 0;
}

// stage 4, solve loop
vector<pair<int, int>> edge_set[MAX_VERTEX][MAX_VERTEX]; // first len, second edge
int use[MAX_VERTEX][MAX_VERTEX]; //edge num used in (v, u)
set<int> adj_v[MAX_VERTEX];
pair<ll, ll> cur_s, best_s;
map<pair<pair<ll, ll>, int>, pair<int, int>> rec; // first len, second vertex

void update_s(int e) {
    if (e < 64)
        cur_s.first += (1ll << e);
    else
        cur_s.second += (1ll << (e - 64));
}

void init_loop() {
    int max_w[MAX_VERTEX][MAX_VERTEX];
    // clear
    rec.clear(), ans.clear();
    for (int v = 0; v < MAX_VERTEX; ++v) {
        adj_v[v].clear();
        for (int u = 0; u < MAX_VERTEX; ++u)
            max_w[v][u] = INT32_MIN, edge_set[v][u].clear();
    }
    // edge set
    for (int e = 0; e < m; ++e) {
        if (in[e] != out[e])
            adj_v[in[e]].insert(out[e]);
        edge_set[in[e]][out[e]].emplace_back(w[e], e);
    }
    // sort
    for (auto &v: edge_set) {
        for (auto &u: v)
            sort(u.begin(), u.end(), greater<pair<int, int> >());   // from long to short
    }
}


void dfs_loop(int v, char tail) {
    if (rec.find({{cur_s.first, cur_s.second}, v}) != rec.end())
        return;
    int best_len = 0, best_vertex = -1;
    pair<ll, ll> tmp_s = cur_s;
    if (tail && (tail != (char) (v + 'a')))
        best_len = INT32_MIN;
    if (use[v][v] < edge_set[v][v].size()) {
        update_s(edge_set[v][v][use[v][v]++].second);
        dfs_loop(v, tail);
        best_len = rec[{{cur_s.first, cur_s.second}, v}].first + edge_set[v][v][--use[v][v]].first;
        best_vertex = v;    // can be replaced as best edge?
    } else {
        for (const auto &u: adj_v[v]) {
            if (use[v][u] < edge_set[v][u].size()) {
                if (scc_class[v] == scc_class[u])
                    update_s(edge_set[v][u][use[v][u]].second);
                else
                    cur_s = {0ll, 0ll};
                ++use[v][u];
                dfs_loop(u, tail);
                int len = rec[{{cur_s.first, cur_s.second}, v}].first + edge_set[v][u][--use[v][u]].first;
                if (len > best_len)
                    best_len = len, best_vertex = u;
            }
        }
    }
    cur_s = tmp_s;  // restore current state
    rec[{{cur_s.first, cur_s.second}, v}] = {best_len, best_vertex};
}

int solve_loop(char head, char tail) {
    // init
    init_loop();
    // memory dfs
    for (int v = 0; v < MAX_VERTEX; ++v) {
        if (!head || (head == (char) (v + 'a')))
            dfs_loop(v, tail);
    }
    // select head edge
    int head_e = -1, sum = INT32_MIN;
    for (int e = 0; e < m; ++e) {
        if (!head || (head == (char) (in[e] + 'a'))) {
            cur_s = {0ll, 0ll};
            if (scc_class[in[e]] == scc_class[out[e]])
                update_s(e);
            int rec_len = rec[{{cur_s.first, cur_s.second}, out[e]}].first;
            if (rec_len <= 0)   // only one edge, can't form a chain
                continue;
            if (sum < rec_len + w[e])
                sum = rec_len + w[e], head_e = e;
        }
    }
    if (head_e < 0)
        return -CHAIN_NOT_EXIST;
    // get chain
    cur_s = {0ll, 0ll};
    int cur_v = out[head_e];
    ans.push_back(s[head_e]);
    if (in[head_e] == out[head_e])
        update_s(head_e), ++use[in[head_e]][out[head_e]];
    while (true) {
        int next_v = rec[{{cur_s.first, cur_s.second}, cur_v}].second;
        if (next_v < 0)
            break;
        int e = edge_set[cur_v][next_v][use[cur_v][next_v]++].second;
        ans.push_back(s[e]);
        if (scc_class[cur_v] != scc_class[next_v])
            cur_s = {0ll, 0ll};
        else
            update_s(e);
        cur_v = next_v;
    }
    return 0;
}

// stage final: interface


int gen_chains_all(char *words[], int len, char *result[], void *malloc(size_t)) {
    init_graph(words, len, 0, false); // weight here can be any
    if (has_loop())
        return -UNEXPECTED_LOOP;
    ans.clear();
    for (int v = 0; v < MAX_VERTEX; ++v) {
        for (bool &u: vis_ver)
            u = false;
        path.clear();
        int r = dfs_all(v);
        if (r < 0)
            return r;
    }
    return 0;
}

int gen_chain_word(char *words[], int len, char *result[], char head, char tail, char jail, bool enable_loop) {
    int r;
    assert(!(head != 0 && head == jail));
    init_graph(words, len, jail, false);
    if (enable_loop) {
        if ((r = solve_loop(head, tail)) < 0)
            return r;
    } else if ((r = solve_dag(head, tail)) < 0)
        return r;
    return 0;
}
int gen_chain_char(char *words[], int len, char *result[], char head, char tail, char jail, bool enable_loop) {
    int r;
    assert(!(head != 0 && head == jail));
    init_graph(words, len, jail, true);
    if (enable_loop) {
        if ((r = solve_loop(head, tail)) < 0)
            return r;
    } else if ((r = solve_dag(head, tail)) < 0)
        return r;
    return 0;
}

// test
void test1();

void test2();

void test3();

void test4();

void test5();

void test6();

int main() {
    test5();
}

void test1() {
    char *result[] = {nullptr};
    char *words[] = {"abc", "ac", "cde", "ce", "cfe", "eg", "bc"};
    int len = 7;
    gen_chain_word(words, len, result, 0, 0, 0, false);
    for (const auto &item: ans)
        cout << item << endl;
}

void test2() {
    char *result[] = {nullptr};
    char *words[] = {"ab", "bc", "cd", "de", "ef", "abcdefeabffeee", "acddd"};
    int len = 7;
    gen_chain_char(words, len, result, 0, 0, 0, false);
    for (const auto &item: ans)
        cout << item << endl;
}

void test3() {
    char *result[] = {nullptr};
    char *words[] = {"ab", "bc", "cd", "de", "ef", "abcdefeabffeee", "acddd"};
    int len = 7;
    gen_chain_word(words, len, result, 0, 0, 'c', false);
    for (const auto &item: ans)
        cout << item << endl;
}

void test4() {
    char *result[] = {nullptr};
    char *words[] = {"ab", "bc", "cd", "de", "ef", "abcdefeabffeee", "acddd"};
    int len = 7;
    int r = gen_chain_word(words, len, result, 0, 'c', 'c', true); // ab, bc
    for (const auto &item: ans)
        cout << item << endl;
    cout << r;
}

void test5() {
    char *result[] = {nullptr};
    char *words[] = {"aba", "ac", "cd", "de"};
    int r = gen_chain_word(words, 4, result, 0, 'c', 'c', true); // aba ac
    for (const auto &item: ans)
        cout << item << endl;
    cout << r;
}

void test6() {
    char *result[] = {nullptr};
    char *words[] = {"ab", "bc", "bbbbbbbccccd", "cd"};
    int r = gen_chain_char(words, 4, result, 'b', 0, 0, true); // bc, cd
    for (const auto &item: ans)
        cout << item << endl;
    cout << r;
}

void check_scc() {
    int sum = 0;
    for (int i = 0; i < scc_cnt; ++i)
        sum += (int) scc_set[i].size();
    assert(sum == MAX_VERTEX);
}