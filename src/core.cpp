#include "core.h"
#include "error.h"


int n, m, scc_cnt;  // count of vertex, edge, scc_set
int out[MAX_EDGE], w[MAX_EDGE]; // out vertex, weight
string s[MAX_EDGE];
vector<int> v_adj[MAX_VERTEX];  // adjacent edge list
int c[MAX_VERTEX];  // scc_set class
set<int> scc_set[MAX_SCC];   // vertex in each scc_set


// stage 1
int num, top;   // dfs time, stack top
int dfn[MAX_VERTEX], low[MAX_VERTEX], stack[MAX_VERTEX];    // dfs time , backtracking value, vertex stack
bool vis[MAX_VERTEX];  // whether complete visiting vertex

void tarjan(int x) {
    dfn[x] = low[x] == ++num;
    stack[top++] = x, vis[x] = true;
    for (const auto &e: v_adj[x]) {
        if (!dfn[out[e]]) {
            tarjan(e);
            low[x] = min(low[x], low[out[e]]);
        } else if (vis[out[e]])
            low[x] = min(low[x], dfn[out[e]]);
    }
    if (dfn[x] == low[x]) {
        int y;
        do {
            y = stack[--top];
            vis[y] = false;
            c[y] = scc_cnt;
            scc_set[scc_cnt].insert(y);
        } while (x != y);
        ++scc_cnt;
    }
}

void init_graph(char *words[], int len, bool weight) {
    n = MAX_VERTEX, m = len;
    for (auto & v : v_adj)
        v.clear();
    for (auto & scc : scc_set)
        scc.clear();
    for (int e = 0; e < len; ++e) {
        int x, y;
        s[e] = words[e];
        x = s[e][0] - 'a', y = s[e].back() - 'a';
        out[e] = y, w[e] = weight ? (int) s[e].length() : 1;
        v_adj[x].push_back(e);
    }
    for (int x = 0; x < n; ++x) {
        if (!dfn[x])
            tarjan(x);
    }
}

bool has_loop() {
    // TODO[1]: 打印自环信息
    for (int v = 0; v < n; ++v) {
        int self_loop = 0;
        for (const auto &e: v_adj[v])
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

// stage 2
vector<string> ans;
vector<string> path;
int dfs_all(int x) {
    vis[x] = true;
    path.push_back(s[x]);
    if (ans.size() > MAX_CHAIN)
        return -TOO_MANY_CHAINS;
    if (path.size() > 1) {
        string chain;
        for (int i = 0; ; ++i) {
            chain += path[i];
            if (i == path.size() - 1)
                break;
            chain += " ";
        }
        ans.push_back(chain);
    }
    for (const auto &e: v_adj[x]) {
        int y = out[e];
        if (!vis[y])
            dfs_all(y);
    }
    vis[x] = false;
    return 0;
}

// interface
int gen_chain_word(char *words[], int len, char *result[], char head, char tail, bool enable_loop) {
    init_graph(words, len, false);
    return 0;
}

int gen_chains_all(char *words[], int len, char *result[]) {
    init_graph(words, len, false); // weight here can be any
    if (has_loop())
        return -UNEXPECTED_LOOP;
    for (int v = 0; v < MAX_VERTEX; ++v) {
        for (bool & u : vis)
            u = false;
        path.clear();
        int r = dfs_all(v);
        if (r < 0)
            return r;
    }
    return 0;
}

int gen_chain_char(char *words[], int len, char *result[], char head, char tail, bool enable_loop) {
    init_graph(words, len, true);
    return 0;
}