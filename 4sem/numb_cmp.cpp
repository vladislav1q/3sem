#include <iostream>
#include <algorithm>
#include <vector>

bool compare(const std::string& lhs, const std::string& rhs){
    return (lhs.size() == rhs.size()) ? (lhs < rhs) : (lhs.size() < rhs.size());
}

int main() {
    const int n = 5;
    std::vector<std::string> a;
    a.resize(n);

    for(int i = 0; i < n; i++){
        std::cin >> a[i];
    }

//    for(int i = 0; i < n; i++){
//        a.push_back('0');
//    }
    std::sort(a.begin(), a.end(), &compare);
    for(int i = 0; i < n; i++){
        std::cout << a[i] << ' ';
    }

    std::cout << "\n";

    return 0;
}