#include "runner.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <cctype>
#include <exception>

namespace fs = std::filesystem;

static std::string read_all(const fs::path& p) {
    std::ifstream f(p, std::ios::binary);
    std::ostringstream s; s << f.rdbuf();
    return s.str();
}

static void trim_right(std::string& s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back())))
        s.pop_back();
}

static std::string normalize_ws(std::string s) {
    for (char& c : s) if (c=='\r') c = '\n';
    std::string r; r.reserve(s.size());
    bool ws = false;
    for (char c : s) {
        if (std::isspace(static_cast<unsigned char>(c))) { ws = true; continue; }
        if (ws) { if (!r.empty()) r.push_back(' '); ws = false; }
        r.push_back(c);
    }
    trim_right(r);
    return r;
}

static void print_diff(const std::string& got, const std::string& exp) {
    std::cerr << "----- expected -----\n" << exp
              << "\n----- got ----------\n" << got
              << "\n--------------------\n";
}

int main()
{
    try {
        #ifdef TESTS_DIR
        fs::path in_dir  = fs::path(TESTS_DIR) / "e2e" / "in";
        fs::path out_dir = fs::path(TESTS_DIR) / "e2e" / "out";
        #else
        fs::path in_dir  = "tests/e2e/in";
        fs::path out_dir = "tests/e2e/out";
        #endif

        if (!fs::exists(in_dir) || !fs::is_directory(in_dir)) {
            std::cerr << "No input dir: " << in_dir << "\n";
            return 2;
        }
        if (!fs::exists(out_dir) || !fs::is_directory(out_dir)) {
            std::cerr << "No output dir: " << out_dir << "\n";
            return 2;
        }

        std::vector<fs::path> ins, outs;
        for (auto& e : fs::directory_iterator(in_dir)) {
            if (e.is_regular_file() && e.path().extension() == ".in") {
                ins.push_back(e.path());
            }
        }
        for (auto& e : fs::directory_iterator(out_dir)) {
            if (e.is_regular_file() && e.path().extension() == ".out") {
                outs.push_back(e.path());
            }
        }

        auto by_name = [](const fs::path& a, const fs::path& b) {
            return a.filename().string() < b.filename().string();
        };
        std::sort(ins.begin(), ins.end(), by_name);
        std::sort(outs.begin(), outs.end(), by_name);

        if (ins.size() != outs.size()) {
            std::cerr << "Count mismatch: inputs=" << ins.size()
                      << " outputs=" << outs.size() << "\n";
            return 3;
        }

        int passed = 0;
        for (size_t i = 0; i < ins.size(); ++i) {
            std::istringstream in(read_all(ins[i]));
            std::ostringstream out;
            int rc = launcher(in, out);
            std::string got = out.str();
            std::string exp = read_all(outs[i]);
            bool ok = (rc == 0 && got == exp);
            if (!ok) {
                std::string gotN = normalize_ws(got);
                std::string expN = normalize_ws(exp);
                ok = (rc == 0 && gotN == expN);
                if (!ok) {
                    std::cout << "[CASE] " << ins[i].filename().string() << " : FAIL\n";
                    print_diff(got, exp);
                    continue;
                }
            }
            std::cout << "[CASE] " << ins[i].filename().string() << " : OK\n";
            ++passed;
        }

        std::cout << "\nSummary: " << passed << " / " << ins.size() << " passed\n";
        return (passed == (int)ins.size()) ? 0 : 1;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
}
