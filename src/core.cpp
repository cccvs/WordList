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
    int deg[MAX_VERTEX];
    for (int & i : deg)
        i = 0;
    for (int i = 0; i < MAX_EDGE; ++i)
        iso_edge[i] = false;
    for (int e = 0; e < len; ++e) {
        string str = words[e];
        ++deg[str[0] - 'a'], ++deg[str.back() - 'a'];
    }
    for (int e = 0; e < len; ++e) {
        string str = words[e];
        int x = (int) (str[0] - 'a'), y = (int) (str.back() - 'a');
        iso_edge[e] |= (x == y && deg[x] <= 2);             // single self loop
        iso_edge[e] |= (x != y && deg[x] + deg[y] <= 2);    // single isolated edge
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
            if (i == path.size() - 1)
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
    int pre_edge[MAX_VERTEX], max_len[MAX_VERTEX];
    for (int v = 0; v < MAX_VERTEX; ++v)
        pre_edge[v] = -1, max_len[v] = (!head || (head == char(v + 'a'))) ? w_self[v] : INT32_MIN;
    for (const auto &v: vertex_seq) {
        for (const auto &e: v_out[v]) {
            int u = out[e];
            if (max_len[v] + w[e] + w_self[u] > max_len[u])
                max_len[u] = max_len[v] + w[e], pre_edge[u] = e;
        }
    }
    // get tail vertex
    int max_v, max_len_all = INT32_MIN;
    for (int v = 0; v < MAX_VERTEX; ++v) {
        if (!tail || (tail == char(v + 'a'))) {
            if (max_len_all < max_len[v])
                max_len_all = max_len[v], max_v = v;
        }
    }
    // get edge list
    int v = max_v;
    vector<int> reverse_edge;
    reverse_edge.clear();
    while (true) {
        for (const auto &self_e: v_self[v])
            reverse_edge.push_back(self_e);
        int e = pre_edge[v];
        if (e != -1)
            v = in[e], reverse_edge.push_back(e);
        else
            break;
    }
    if (reverse_edge.size() < 2)
        return -CHAIN_NOT_EXIST;
    // get ans
    ans.clear();
    for (int i = (int) reverse_edge.size() - 1; i >= 0; --i)
        ans.push_back(s[reverse_edge[i]]);
    return 0;
}

// stage 4, solve loop
int best_len, tail_vertex[MAX_VERTEX];
vector<int> scc_seq;
pair<ll, ll> cur_s, best_s;
map<pair<pair<ll, ll>, int>, int> rec_edge, rec_len;

void update_s(int e) {
    if (e < 64)
        cur_s.first += (1ll << e);
    else
        cur_s.second += (1ll << (e - 64));
}

void topo_scc() {
    int in_deg[MAX_VERTEX];
    queue<int> q;
    while (!q.empty())
        q.pop();
    for (int scc = 0; scc < scc_cnt; ++scc) {
        in_deg[scc] = (int) scc_in[scc].size();
        if (!in_deg[scc])
            q.push(scc);
    }
    while (!q.empty()) {
        int scc = q.front();
        q.pop(), scc_seq.push_back(scc);
        for (const auto &e: scc_out[scc]) {
            int scc2 = scc_class[out[e]];
            --in_deg[scc2];
            if (!in_deg[scc2])
                q.push(scc2);
        }
    }
    assert(scc_cnt == scc_seq.size());
}

void dfs_loop(int v, int scc) {

}

int solve_loop(int head, int tail) {
    // init
    best_len = INT32_MIN;
    cur_s = {0, 0}, best_s = {0, 0};
    scc_seq.clear(), rec_edge.clear(), rec_len.clear();
    topo_scc();
    for (int i = 0; i < scc_cnt; ++i) {
        int scc = scc_seq[i];
        for (const auto &v: scc_set[scc])
            dfs_loop(v, scc);
    }
    return 0;
}

// stage final: interface
int gen_chain_word(char *words[], int len, char *result[], char head, char tail, char jail, bool enable_loop) {
    int r;
    assert(!(head != 0 && head == jail));
    init_graph(words, len, jail, false);
    if (enable_loop);
    else if ((r = solve_dag(head, tail)) < 0)
        return r;
    return 0;
}

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

int gen_chain_char(char *words[], int len, char *result[], char head, char tail, char jail, bool enable_loop) {
    int r;
    assert(!(head != 0 && head == jail));
    init_graph(words, len, jail, true);
    if (enable_loop);
    else if ((r = solve_dag(head, tail)) < 0)
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
    int r = gen_chain_word(words, len, result, 0, 'c', 'c', false);
    for (const auto &item: ans)
        cout << item << endl;
    cout << r;
}

void test5() {
    char *result[] = {nullptr};
    char *words[] = {"aba", "ac", "cd", "de"};
    int r = gen_chain_word(words, 4, result, 0, 'c', 'c', false);
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