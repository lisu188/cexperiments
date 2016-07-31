#include <vstd.h>

int main() {
    auto neuro = std::make_shared<vstd::neuro<>>(std::vector<int>({2, 5, 25, 5, 1}), 0.2, 1, 0.8);
    neuro->add_teacher({1, 1}, {0});
    neuro->add_teacher({1, 0}, {1});
    neuro->add_teacher({0, 1}, {1});
    neuro->add_teacher({0, 0}, {0});
    std::cout << neuro->teach(0.001,1000000);
}
