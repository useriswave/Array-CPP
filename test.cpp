#include "array.h"

int main()
{
    Array<std::string_view> arr { "John", "Larry", "Harold" };
    // Array<int> arr { 3, 3, 7, 89, 0, 4, 5, 3 };


    for (const auto& e : arr) {
        std::cout << e << ' ';
    }

    std::cout << arr << '\n';
    return 0;
}
