// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797926254D
#pragma once
#include <iostream>
#include <vector> 
#include <algorithm>
#include <map>
#include "Field.h"
using namespace std;

struct Table {
    int num_rows = 0;
    int num_cols = 0;
    vector<string> col_names;
    vector<string> col_types;
    vector<vector<Field>> cols;
    string index_col;
    string index_type;
    unordered_map<Field, vector<int>> hash_index;
    map<Field, vector<int>> bst_index;

    // Max 10 columns per spec — linear scan is O(10) = O(1), no hash map needed.
    int col_index(const string& name) const {
        for (int i = 0; i < static_cast<int>(col_names.size()); ++i)
            if (col_names[static_cast<size_t>(i)] == name) return i;
        return -1;
    }
    bool has_col(const string& name) const { return col_index(name) != -1; }
    vector<Field>& col(const string& name) {
        return cols[static_cast<size_t>(col_index(name))];
    }
    const vector<Field>& col(const string& name) const {
        return cols[static_cast<size_t>(col_index(name))];
    }
    const string& col_type(const string& name) const {
        return col_types[static_cast<size_t>(col_index(name))];
    }
};

class command {
private:
    bool quiet;
    unordered_map<string, Table> tables;

    void consume_line(istream& input) {
        string discard; getline(input, discard);
    }

    string get_type(const Table& table, const string& col_name) {
        if (!table.has_col(col_name)) return "";
        return table.col_type(col_name);
    }

    Field read_field(const Table& table, const string& col_name, istream& input) {
        string col_type = get_type(table, col_name);
        if (col_type == "int")    { int v;    input >> v; return Field(v); }
        if (col_type == "double") { double v; input >> v; return Field(v); }
        if (col_type == "string") { string v; input >> v; return Field(v); }
        bool v; input >> v; return Field(v);
    }

    template <typename Comp>
    vector<int> filter_rows(const vector<Field>& col_data, const Field& val, Comp comp) {
        vector<int> result;
        for (int r = 0; r < static_cast<int>(col_data.size()); ++r)
            if (comp(col_data[static_cast<size_t>(r)], val)) result.push_back(r);
        return result;
    }

    vector<int> evaluate_where(const Table& table, const string& c, char op, istream& input) {
        Field val = read_field(table, c, input);
        const vector<Field>& data = table.col(c);
        if (op == '<')      return filter_rows(data, val, std::less<Field>());
        else if (op == '>') return filter_rows(data, val, std::greater<Field>());
        else                return filter_rows(data, val, std::equal_to<Field>());
    }

    vector<int> bst_lookup(Table& table, const string& c, char op, istream& input) {
        Field val = read_field(table, c, input);
        vector<int> result;
        auto& idx = table.bst_index;
        if (op == '=') {
            auto it = idx.find(val);
            if (it != idx.end())
                result.insert(result.end(), it->second.begin(), it->second.end());
        } else if (op == '<') {
            for (auto it = idx.begin(); it != idx.end() && it->first < val; ++it)
                result.insert(result.end(), it->second.begin(), it->second.end());
        } else {
            for (auto it = idx.upper_bound(val); it != idx.end(); ++it)
                result.insert(result.end(), it->second.begin(), it->second.end());
        }
        return result;
    }

    vector<int> hash_lookup(Table& table, const string& c, istream& input) {
        Field val = read_field(table, c, input);
        auto it = table.hash_index.find(val);
        if (it != table.hash_index.end()) return it->second;
        return {};
    }

    vector<int> where_for_print(Table& t, const string& c, char op, istream& input) {
        if (t.index_col == c) {
            if (t.index_type == "bst") return bst_lookup(t, c, op, input);
            if (t.index_type == "hash" && op == '=') return hash_lookup(t, c, input);
        }
        return evaluate_where(t, c, op, input);
    }

    // Count-only path for quiet mode PRINT WHERE — zero allocation.
    size_t count_where(Table& t, const string& c, char op, istream& input) {
        Field val = read_field(t, c, input);
        if (t.index_col == c) {
            if (t.index_type == "hash" && op == '=') {
                auto it = t.hash_index.find(val);
                return it != t.hash_index.end() ? it->second.size() : 0;
            }
            if (t.index_type == "bst") {
                size_t cnt = 0;
                auto& idx = t.bst_index;
                if (op == '=') {
                    auto it = idx.find(val);
                    if (it != idx.end()) cnt = it->second.size();
                } else if (op == '<') {
                    for (auto it = idx.begin(); it != idx.end() && it->first < val; ++it)
                        cnt += it->second.size();
                } else {
                    for (auto it = idx.upper_bound(val); it != idx.end(); ++it)
                        cnt += it->second.size();
                }
                return cnt;
            }
        }
        const vector<Field>& data = t.col(c);
        size_t cnt = 0;
        for (size_t r = 0; r < data.size(); ++r) {
            if      (op == '<' && data[r] < val) ++cnt;
            else if (op == '>' && data[r] > val) ++cnt;
            else if (op == '=' && data[r] == val) ++cnt;
        }
        return cnt;
    }

    vector<int> where_for_delete(Table& t, const string& c, char op, istream& input) {
        if (t.index_col == c) {
            if (t.index_type == "hash" && op == '=') return hash_lookup(t, c, input);
            if (t.index_type == "bst")               return bst_lookup(t, c, op, input);
        }
        return evaluate_where(t, c, op, input);
    }

    void rebuild_index(Table& t) {
        t.hash_index.clear();
        t.bst_index.clear();
        const vector<Field>& c = t.col(t.index_col);
        for (int r = 0; r < t.num_rows; ++r) {
            if (t.index_type == "hash")
                t.hash_index[c[static_cast<size_t>(r)]].push_back(r);
            else
                t.bst_index[c[static_cast<size_t>(r)]].push_back(r);
        }
    }

public:
    command(bool q) : quiet(q) {}

    void create(istream& line) {
        int column; string name;
        line >> name >> column;
        if (tables.find(name) != tables.end()) {
            cout << "Error during CREATE: Cannot create already existing table "
                 << name << "\n";
            consume_line(line);
            return;
        }
        Table newTable;
        newTable.num_cols = column;
        newTable.num_rows = 0;
        newTable.cols.resize(static_cast<size_t>(column));
        newTable.col_types.reserve(static_cast<size_t>(column));
        newTable.col_names.reserve(static_cast<size_t>(column));
        for (int i = 0; i < column; ++i) {
            string type; line >> type;
            newTable.col_types.push_back(type);
        }
        for (int i = 0; i < column; ++i) {
            string col_name; line >> col_name;
            newTable.col_names.push_back(col_name);
        }
        tables.emplace(name, std::move(newTable));
        Table& inserted = tables[name];
        cout << "New table " << name << " with column(s) ";
        for (const string& c : inserted.col_names) cout << c << " ";
        cout << "created\n";
    }

    void insert(istream& input) {
        string dummy, name; int rows;
        input >> dummy >> name >> rows >> dummy;
        if (tables.find(name) == tables.end()) {
            cout << "Error during INSERT: " << name
                 << " does not name a table in the database\n";
            return;
        }
        Table& t = tables[name];
        size_t ncols = static_cast<size_t>(t.num_cols);

        vector<vector<Field>*> col_ptrs(ncols);
        for (size_t c = 0; c < ncols; ++c)
            col_ptrs[c] = &t.cols[c];

        for (int r = 0; r < rows; ++r) {
            for (size_t c = 0; c < ncols; ++c) {
                if (t.col_types[c] == "string") {
                    string val; input >> val;
                    col_ptrs[c]->push_back(Field(val));
                } else if (t.col_types[c] == "int") {
                    int val; input >> val;
                    col_ptrs[c]->push_back(Field(val));
                } else if (t.col_types[c] == "double") {
                    double val; input >> val;
                    col_ptrs[c]->push_back(Field(val));
                } else {
                    bool val; input >> val;
                    col_ptrs[c]->push_back(Field(val));
                }
            }
        }
        int old_rows = t.num_rows;
        t.num_rows += rows;
        if (!t.index_col.empty()) {
            const vector<Field>& ic = t.col(t.index_col);
            for (int r = old_rows; r < t.num_rows; ++r) {
                if (t.index_type == "hash")
                    t.hash_index[ic[static_cast<size_t>(r)]].push_back(r);
                else
                    t.bst_index[ic[static_cast<size_t>(r)]].push_back(r);
            }
        }
        cout << "Added " << rows << " rows to " << name
             << " from position " << old_rows << " to " << t.num_rows - 1 << "\n";
    }

    void remove(string& table_name) {
        if (tables.find(table_name) == tables.end()) {
            cout << "Error during REMOVE: " << table_name
                 << " does not name a table in the database\n";
            return;
        }
        tables.erase(table_name);
        cout << "Table " << table_name << " removed\n";
    }

    void quit() {
        tables.clear();
        cout << "Thanks for being silly!\n";
    }

    void print(istream& input) {
        int cols; string name, dummy, cond;
        vector<string> print_cols;
        input >> dummy >> name >> cols;
        if (tables.find(name) == tables.end()) {
            cout << "Error during PRINT: " << name
                 << " does not name a table in the database\n";
            consume_line(input);
            return;
        }
        Table& t = tables[name];

        // Validate ALL print column names before printing anything.
        for (int i = 0; i < cols; ++i) {
            string col_name; input >> col_name;
            if (!t.has_col(col_name)) {
                cout << "Error during PRINT: " << col_name
                     << " does not name a column in " << name << "\n";
                consume_line(input);
                return;
            }
            print_cols.push_back(col_name);
        }

        input >> cond;

        string cond_col; char op = '=';
        if (cond == "WHERE") {
            input >> cond_col >> op;
            if (!t.has_col(cond_col)) {
                cout << "Error during PRINT: " << cond_col
                     << " does not name a column in " << name << "\n";
                consume_line(input);
                return;
            }
        }

        if (!quiet) {
            for (const string& c : print_cols) cout << c << " ";
            cout << "\n";
        }

        if (cond == "ALL") {
            vector<const vector<Field>*> ptrs(print_cols.size());
            for (size_t i = 0; i < print_cols.size(); ++i)
                ptrs[i] = &t.col(print_cols[i]);
            if (!quiet) {
                for (int r = 0; r < t.num_rows; ++r) {
                    for (auto* p : ptrs) cout << (*p)[static_cast<size_t>(r)] << " ";
                    cout << "\n";
                }
            }
            cout << "Printed " << t.num_rows << " matching rows from " << name << "\n";

        } else {  
            if (quiet) {
                size_t cnt = count_where(t, cond_col, op, input);
                cout << "Printed " << cnt << " matching rows from " << name << "\n";
            } else {
                vector<int> rows = where_for_print(t, cond_col, op, input);
                vector<const vector<Field>*> ptrs(print_cols.size());
                for (size_t i = 0; i < print_cols.size(); ++i)
                    ptrs[i] = &t.col(print_cols[i]);
                for (int row_index : rows) {
                    for (auto* p : ptrs)
                        cout << (*p)[static_cast<size_t>(row_index)] << " ";
                    cout << "\n";
                }
                cout << "Printed " << rows.size() << " matching rows from " << name << "\n";
            }
        }
    }

    void dele(istream& input) {
        string name, dummy, cond;
        input >> dummy >> name;
        if (tables.find(name) == tables.end()) {
            cout << "Error during DELETE: " << name
                 << " does not name a table in the database\n";
            consume_line(input);
            return;
        }
        Table& t = tables[name];
        input >> cond;

        if (cond == "ALL") {
            int deleted = t.num_rows;
            for (auto& cv : t.cols) cv.clear();
            t.num_rows = 0;
            t.hash_index.clear();
            t.bst_index.clear();
            cout << "Deleted " << deleted << " rows from " << name << "\n";

        } else if (cond == "WHERE") {
            string cond_col; char op;
            input >> cond_col >> op;
            if (!t.has_col(cond_col)) {
                cout << "Error during DELETE: " << cond_col
                     << " does not name a column in " << name << "\n";
                consume_line(input);
                return;
            }
            vector<int> to_del = where_for_delete(t, cond_col, op, input);
            std::sort(to_del.begin(), to_del.end());

            size_t ncols = static_cast<size_t>(t.num_cols);
            int keep = t.num_rows - static_cast<int>(to_del.size());
            if (keep < 0) keep = 0;
            for (size_t ci = 0; ci < ncols; ++ci) {
                const vector<Field>& old_col = t.cols[ci];
                vector<Field> new_col;
                new_col.reserve(static_cast<size_t>(keep));
                size_t del_ptr = 0;
                for (int r = 0; r < t.num_rows; ++r) {
                    if (del_ptr < to_del.size() &&
                        to_del[del_ptr] == r) {
                        ++del_ptr;
                    } else {
                        new_col.push_back(old_col[static_cast<size_t>(r)]);
                    }
                }
                t.cols[ci] = std::move(new_col);
            }
            t.num_rows = keep;

            if (!t.index_col.empty()) rebuild_index(t);
            cout << "Deleted " << to_del.size() << " rows from " << name << "\n";
        }
    }

    void join(istream& input) {
        string name1, name2, dummy, col1, col2;
        char op; int n;
        input >> name1 >> dummy >> name2
              >> dummy >> col1 >> op >> col2
              >> dummy >> dummy >> n;

        vector<string> print_cols;
        vector<int>    print_tables;
        for (int i = 0; i < n; ++i) {
            string c; int t; input >> c >> t;
            print_cols.push_back(c);
            print_tables.push_back(t);
        }

        if (tables.find(name1) == tables.end()) {
            cout << "Error during JOIN: " << name1
                 << " does not name a table in the database\n";
            consume_line(input); return;
        }
        if (tables.find(name2) == tables.end()) {
            cout << "Error during JOIN: " << name2
                 << " does not name a table in the database\n";
            consume_line(input); return;
        }
        Table& t1 = tables[name1];
        Table& t2 = tables[name2];

        if (!quiet) {
            for (const string& c : print_cols) cout << c << " ";
            cout << "\n";
        }

        const unordered_map<Field, vector<int>>* idx = nullptr;
        unordered_map<Field, vector<int>> temp;
        if (t2.index_col == col2 && t2.index_type == "hash") {
            idx = &t2.hash_index;
        } else {
            const vector<Field>& c2 = t2.col(col2);
            temp.reserve(static_cast<size_t>(t2.num_rows));
            for (int r = 0; r < t2.num_rows; ++r)
                temp[c2[static_cast<size_t>(r)]].push_back(r);
            idx = &temp;
        }

        int count = 0;
        if (quiet) {
            const vector<Field>& c1 = t1.col(col1);
            for (int r = 0; r < t1.num_rows; ++r) {
                auto it = idx->find(c1[static_cast<size_t>(r)]);
                if (it != idx->end())
                    count += static_cast<int>(it->second.size());
            }
        } else {
            vector<const vector<Field>*> out_ptrs(static_cast<size_t>(n));
            for (int i = 0; i < n; ++i) {
                if (print_tables[static_cast<size_t>(i)] == 1)
                    out_ptrs[static_cast<size_t>(i)] = &t1.col(print_cols[static_cast<size_t>(i)]);
                else
                    out_ptrs[static_cast<size_t>(i)] = &t2.col(print_cols[static_cast<size_t>(i)]);
            }
            const vector<Field>& c1 = t1.col(col1);
            for (int r = 0; r < t1.num_rows; ++r) {
                auto it = idx->find(c1[static_cast<size_t>(r)]);
                if (it == idx->end()) continue;
                for (int mr : it->second) {
                    ++count;
                    for (int i = 0; i < n; ++i) {
                        int row = (print_tables[static_cast<size_t>(i)] == 1) ? r : mr;
                        cout << (*out_ptrs[static_cast<size_t>(i)])[static_cast<size_t>(row)] << " ";
                    }
                    cout << "\n";
                }
            }
        }
        cout << "Printed " << count << " rows from joining "
             << name1 << " to " << name2 << "\n";
    }

    void generate(istream& input) {
        string tablename, dummy, indextype, colname;
        input >> dummy >> tablename >> indextype >> dummy >> dummy >> colname;
        if (tables.find(tablename) == tables.end()) {
            cout << "Error during GENERATE: " << tablename
                 << " does not name a table in the database\n"; return;
        }
        Table& t = tables[tablename];
        if (!t.has_col(colname)) {
            cout << "Error during GENERATE: " << colname
                 << " does not name a column in " << tablename << "\n"; return;
        }
        t.hash_index.clear();
        t.bst_index.clear();
        t.index_col  = colname;
        t.index_type = indextype;
        rebuild_index(t);
        int distinct = static_cast<int>(indextype == "hash"
            ? t.hash_index.size() : t.bst_index.size());
        cout << "Generated " << indextype << " index for table " << tablename
             << " on column " << colname << ", with " << distinct << " distinct keys\n";
    }
};