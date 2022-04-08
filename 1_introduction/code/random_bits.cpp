#include <iostream>

int main() {
    int n = 100000;
    int x = 989511;
    for (int i = 0; i != n; ++i) {
        x = x * 312523 + 852596;
        std::cout << int(x > 0);
    }
    std::cout << '\n';
    return 0;
}
