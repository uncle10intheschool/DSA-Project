#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <random>
#include <climits>
using namespace std;
void generate_int_test(int test_num) {
    int n = 100000;
    cout << n << "\n";
    mt19937 rng(1337); 
    uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
    if (test_num == 1) {
        for (int i = 0; i < n; ++i) cout << 2147483647 - i << "\n";
    } else if (test_num == 2) {
        for (int i = 0; i < n; ++i) cout << -2147483648 + i << "\n";
    } else if (test_num == 3) {
        for (int i = 0; i < n; ++i) {
            if (i % 4 == 0) cout << INT_MIN << "\n";
            else if (i % 4 == 1) cout << INT_MAX << "\n";
            else if (i % 4 == 2) cout << 0 << "\n";
            else cout << dist(rng) << "\n";
        }
    } else if (test_num == 4) {
        for (int i = 0; i < n; ++i) cout << dist(rng) << "\n";
    } else if (test_num == 5) {
        int duplicate_val = 25120445; 
        for (int i = 0; i < n - 1000; ++i) cout << duplicate_val << "\n";
        for (int i = n - 1000; i < n; ++i) cout << dist(rng) << "\n";
    }
}
void generate_strlexi_test(int test_num) {
    int n = 100000;
    string buffer = to_string(n) + "\n";
    buffer.reserve(n * 102 + 10);
    if (test_num == 1 || test_num == 3) {
        for (int i = 0; i < n; ++i) {
            int cycle = i % 26;
            buffer.append(98, 'a');
            buffer.push_back('z' - cycle);
            buffer.push_back('a' + cycle);
            buffer.push_back('\n');
        }
    } else if (test_num == 2) {
        for (int i = 0; i < n; ++i) {
            buffer.append(100, 'z' - (i % 26));
            buffer.push_back('\n');
        }
    } else if (test_num == 4 || test_num == 5) {
        uint32_t seed = (test_num == 4) ? 5489U : 1103515245U;
        mt19937 rng(seed);
        uniform_int_distribution<int> len_dist(10, 100);
        uniform_int_distribution<int> char_dist(0, 25);
        for (int i = 0; i < n; ++i) {
            int len = len_dist(rng);
            for (int j = 0; j < len; ++j) {
                buffer.push_back('a' + char_dist(rng));
            }
            buffer.push_back('\n');
        }
    }
    cout << buffer;
}
string create4LetterDiff(int val) {
    string res = "";
    for(int i = 0; i < 4; i++) {
        res = (char)('a' + (val % 26)) + res;
        val /= 26;
    }
    return res;
}
void generate_strlenlexi_test(int test_num) {
    int n = 10000;
    cout << n << "\n";
    if (test_num == 1) {
        for (int i = n - 1; i >= 0; i--) {
            cout << string(96, 'a') << create4LetterDiff(i) << "\n";
        }
    } else if (test_num == 2) {
        string clone(100, 'z');
        for (int i = 0; i < n; i++) {
            cout << clone << "\n";
        }
    } else if (test_num == 3) {
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) cout << string(99, 'a') << "x\n";
            else cout << string(99, 'a') << "y\n";
        }
    } else if (test_num == 4) {
        for (int i = n / 2 - 1; i >= 0; i--) {
            cout << string(96, 'b') << create4LetterDiff(i) << "\n";
        }
        for (int i = 0; i < n - n / 2; i++) {
            cout << string(96, 'b') << create4LetterDiff(i) << "\n";
        }
    } else if (test_num == 5) {
        srand(25120411);
        for (int i = 0; i < n; i++) {
            string s = "";
            for (int j = 0; j < 100; j++) {
                s += (char)('a' + (rand() % 26));
            }
            cout << s << "\n";
        }
    }
}
int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    if (argc != 3) return -1;
    string problem_type = argv[1];
    int test_num = atoi(argv[2]);
    if (test_num < 1 || test_num > 5) return -1;
    if (problem_type == "int") {
        generate_int_test(test_num);
    } else if (problem_type == "strlexi") {
        generate_strlexi_test(test_num);
    } else if (problem_type == "strlenlexi") {
        generate_strlenlexi_test(test_num);
    }
    return 0;
}