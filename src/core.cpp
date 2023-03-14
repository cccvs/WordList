#include "core.h"
#include "iostream"

int m = 0, scc_cnt = 0;  // count of valid edge, scc_set_size
int in[MAX_EDGE], out[MAX_EDGE], w[MAX_EDGE]; // out vertex, weight
string s[MAX_EDGE];
vector<int> v_out[MAX_VERTEX], v_in[MAX_VERTEX], v_self[MAX_VERTEX];  // adjacent edge list
int scc_class[MAX_VERTEX], w_self[MAX_VERTEX];  // scc_set_size class of a vertex, weight sum of self loop of a vertex


// stage 1, make scc
int num = 0, top = 0;   // dfs time, stack top
int dfn[MAX_VERTEX] = {0}, low[MAX_VERTEX] = {0}, stack[MAX_VERTEX] = {0};    // dfs time , backtracking value, vertex stack
bool vis_ver[MAX_VERTEX] = {false};  // whether complete visiting vertex

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
        } while (x != y);
        ++scc_cnt;
    }
}

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

void init_graph(char *words[], int len, char reject, bool weight) {
    bool iso_edge[MAX_EDGE];
    for (int v = 0; v < MAX_VERTEX; ++v)
        v_out[v].clear(), v_in[v].clear(), v_self[v].clear();
    check_iso_edge(words, len, iso_edge);
    for (int e = 0; e < len; ++e) {
        if (iso_edge[e])
            continue;
        int x, y;
        if (!reject || reject != words[e][0]) {
            s[m] = words[e], s[m] = words[e];
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
}

void check_loop() {
    for (int v = 0; v < MAX_VERTEX; ++v) {
        int self_loop = 0;
        for (const auto &e: v_out[v])
            self_loop += (v == out[e]) ? 1 : 0;
        if (self_loop > 1)
            throw logic_error("Word ring detected, at least two self ring on one node,");
    }
    // check scc
    if(scc_cnt < MAX_VERTEX)
        throw logic_error("Word ring detected, at least one scc has more than two nodes.");
}

// stage 2, get all chains
vector<string> ans;
vector<string> path;
bool vis_edge[MAX_EDGE] = {false};

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

void dfs_all(int x) {
    if (ans.size() > MAX_CHAIN)
        throw logic_error("Too many word chains!");
    for (const auto &e: v_out[x]) {
        if (!vis_edge[e]) {
            vis_edge[e] = true, path.push_back(s[e]);
            path_to_ans();
            dfs_all(out[e]);
            vis_edge[e] = false, path.pop_back();
        }
    }
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

void solve_dag(char head, char tail) {
    ans.clear(), vertex_seq.clear();
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
    int tail_e = -1, tot_len = INT32_MIN;
    for (int e = 0; e < m; ++e) {
        int x = in[e], y = out[e];
        if (tail && (tail != s[e].back()))
            continue;
        if (pre_edge[e] < 0)
            continue;
        if (x == y && tot_len < max_len[x])
            tot_len = max_len[x], tail_e = v_self[x].back();
        if (x != y && tot_len < max_len[x] + w[e])
            tot_len = max_len[x] + w[e], tail_e = e;
    }
    if (tail_e < 0) {
        ans.clear();
        return;
    }
    // get edge list
    vector<int> reverse_edge;
    reverse_edge.clear();
    int e = tail_e;
    while (e >= 0) {
        reverse_edge.push_back(e);
        e = pre_edge[e];
    }
    // get ans
    for (int i = (int) reverse_edge.size() - 1; i >= 0; --i)
        ans.push_back(s[reverse_edge[i]]);
}

// stage 4, solve loop
vector<pair<int, int>> edge_set[MAX_VERTEX][MAX_VERTEX]; // first len, second edge
int use[MAX_VERTEX][MAX_VERTEX] = {0}; //edge num used in (v, u)
set<int> adj_v[MAX_VERTEX];
pair<ll, ll> cur_s;
map<status, pair<int, int>> rec; // first len, second vertex

//string status_to_str() {
//    string str = "{ ";
//    for (int i = 0; i < 64; ++i)
//        if (cur_s.first & (1ll << i))
//            str += s[i] + " ";
//    for (int i = 0; i < 64; ++i)
//        if (cur_s.second & (1ll << i))
//            str += s[i + 64] + " ";
//    return str + "}";
//}

void update_s(int e) {
    if (e < 64)
        cur_s.first += (1ll << e);
    else
        cur_s.second += (1ll << (e - 64));
}

void init_loop() {
    int max_w[MAX_VERTEX][MAX_VERTEX];
    // clear
    ans.clear(), rec.clear();
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
    if (rec.find({cur_s.first, cur_s.second, v}) != rec.end())
        return;
    int best_len = 0, best_vertex = -1;
    pair<ll, ll> tmp_s = cur_s;
    if (tail && (tail != (char) (v + 'a')))
        best_len = INT32_MIN;
    if (use[v][v] < edge_set[v][v].size()) {
        update_s(edge_set[v][v][use[v][v]++].second);
        dfs_loop(v, tail);
        best_len = rec[{cur_s.first, cur_s.second, v}].first + edge_set[v][v][--use[v][v]].first;
        best_vertex = v;    // can be replaced as best edge?
        cur_s = tmp_s;
    } else {
        for (const auto &u: adj_v[v]) {
            if (use[v][u] < edge_set[v][u].size()) {
                if (scc_class[v] == scc_class[u])
                    update_s(edge_set[v][u][use[v][u]].second);
                else
                    cur_s = {0ll, 0ll};
                ++use[v][u];
                dfs_loop(u, tail);
                int len = rec[{cur_s.first, cur_s.second, u}].first + edge_set[v][u][--use[v][u]].first;
                if (len > best_len)
                    best_len = len, best_vertex = u;
                cur_s = tmp_s; // restore current state at once, or else status will be wrong!
            }
        }
    }
    rec[{cur_s.first, cur_s.second, v}] = {best_len, best_vertex};
}

void solve_loop(char head, char tail) {
    // init
    cur_s = {0ll, 0ll};
    init_loop();
    // memory dfs
    for (int v = 0; v < MAX_VERTEX; ++v) {
        if (!head || (head == (char) (v + 'a')))
            dfs_loop(v, tail);
    }
    // select head edge
    int head_e = -1, tot_len = INT32_MIN;
    for (int e = 0; e < m; ++e) {
        if (!head || (head == (char) (in[e] + 'a'))) {
            cur_s = {0ll, 0ll};
            if (scc_class[in[e]] == scc_class[out[e]])
                update_s(e);
            int rec_len = rec[{cur_s.first, cur_s.second, out[e]}].first;
            if (rec_len <= 0)   // only one edge, can't form a chain
                continue;
            if (tot_len < rec_len + w[e])
                tot_len = rec_len + w[e], head_e = e;
        }
    }
    if (head_e < 0) {
        ans.clear();
        return;
    }
    // get chain
    cur_s = {0ll, 0ll};
    int cur_v = out[head_e];
    ans.push_back(s[head_e]);
    if (scc_class[in[head_e]] == scc_class[out[head_e]])
        update_s(head_e), ++use[in[head_e]][out[head_e]];
    while (true) {
        int next_v = rec[{cur_s.first, cur_s.second, cur_v}].second;
//        cout << status_to_str() << " " << cur_v << " " << next_v << endl;
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
}

// stage final: interface
void ans_to_res(char *result[], void *my_malloc(size_t)) {
    int cur_pos = 0;
    for (const string &str: ans)
        cur_pos += ((int) str.size() + 1);
    char *res_str = (char *) my_malloc(cur_pos);
    cur_pos = 0;
    for (int i = 0; i < ans.size(); ++i) {
        char *p = res_str + cur_pos;
        string str = ans[i];
        for (int j = 0; j < str.size(); ++j)
            p[j] = str[j];
        p[str.size()] = '\0';
        result[i] = p;
        cur_pos += ((int) str.size() + 1);
    }
}

int gen_chains_all(char *words[], int len, char *result[], void *my_malloc(size_t)) {
    init_graph(words, len, 0, false); // weight here can be any
    check_loop();
    ans.clear();
    for (int v = 0; v < MAX_VERTEX; ++v) {
        for (bool &u: vis_ver)
            u = false;
        path.clear();
        dfs_all(v);
    }
    if (ans.empty())
        return 0;
    ans_to_res(result, my_malloc);
    return (int) ans.size();
}

int gen_chain_word(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop,
                   void *my_malloc(size_t)) {
    assert(!(head != 0 && head == reject));
    init_graph(words, len, reject, false);
    if (enable_loop)
        solve_loop(head, tail);
    else
        solve_dag(head, tail);
    if (ans.empty())
        return 0;
    ans_to_res(result, my_malloc);
    assert(ans.size() > 1);
    return (int) ans.size();
}

int gen_chain_char(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop,
                   void *my_malloc(size_t)) {
    assert(!(head != 0 && head == reject));
    init_graph(words, len, reject, true);
    if (enable_loop)
        solve_loop(head, tail);
    else
        solve_dag(head, tail);
    if (ans.empty())
        return 0;
    ans_to_res(result, my_malloc);
    assert(ans.size() > 1);
    return (int) ans.size();
}

// test
//void test1();
//
//void test2();
//
//void test3();
//
//void test4();
//
//void test5();
//
//void test6();
//
//void test7();
//
//int main() {
//    test7();
//}
//
//void test1() {
//    char *result[MAX_CHAIN] = {nullptr};
//    char *words[] = {"abc", "ac", "cde", "ce", "cfe", "eg", "bc"};
//    int len = 7;
//    int r = gen_chain_word(words, len, result, 0, 0, 0, false, std::malloc);
//    if (r > 0) {
//        for (int i = 0; i < r; ++i)
//            cout << result[i] << endl;
//    }
//    cout << r;
//}
//
//void test2() {
//    char *result[MAX_CHAIN] = {nullptr};
//    char *words[] = {"ab", "bc", "cd", "de", "ef", "abcdefeabffeee", "acddd"};
//    int len = 7;
//    int r = gen_chain_char(words, len, result, 0, 0, 0, false, std::malloc);
//    if (r > 0) {
//        for (int i = 0; i < r; ++i)
//            cout << result[i] << endl;
//    }
//    cout << r;
//}
//
//void test3() {
//    char *result[MAX_CHAIN] = {nullptr};
//    char *words[] = {"ab", "bc", "cd", "de", "ef", "abcdefeabffeee", "acddd"};
//    int len = 7;
//    int r = gen_chain_word(words, len, result, 0, 0, 'c', false, std::malloc);
//    if (r > 0) {
//        for (int i = 0; i < r; ++i)
//            cout << result[i] << endl;
//    }
//    cout << r;
//}
//
//void test4() {
//    char *result[MAX_CHAIN] = {nullptr};
//    char *words[] = {"ab", "bc", "cd", "de", "ef", "abcdefeabffeee", "acddd"};
//    int len = 7;
//    int r = gen_chain_word(words, len, result, 0, 'c', 'c', true, std::malloc); // ab, bc
//    if (r > 0) {
//        for (int i = 0; i < r; ++i)
//            cout << result[i] << endl;
//    }
//    cout << r;
//}
//
//void test5() {
//    char *result[MAX_CHAIN] = {nullptr};
//    char *words[] = {"aba", "ac", "cd", "de"};
//    int r = gen_chain_word(words, 4, result, 0, 'c', 'c', true, std::malloc); // aba ac
//    if (r > 0) {
//        for (int i = 0; i < r; ++i)
//            cout << result[i] << endl;
//    }
//    cout << r;
//}
//
//void test6() {
//    char *result[MAX_CHAIN] = {nullptr};
//    char *words[] = {"ab", "bc", "bbbbbbbccccd", "cd"};
//    int r = gen_chain_char(words, 4, result, 'b', 0, 0, true, std::malloc); // bc, cd
//    if (r > 0) {
//        for (int i = 0; i < r; ++i)
//            cout << result[i] << endl;
//    }
//    cout << r;
//}
//
//void test7() {
//    char *result[MAX_CHAIN] = {nullptr};
//    char *words[] = {"ab", "bc", "ca", "de", "ea", "ae"};
//    int r = gen_chain_char(words, 6, result, 'b', 0, 0, true, std::malloc); //bc, ca, ae, ea, ab
//    if (r > 0) {
//        for (int i = 0; i < r; ++i)
//            cout << result[i] << endl;
//    }
//    cout << r;
//}