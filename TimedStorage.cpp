#include "TimedStorage.h"
#include <iostream>


int main() {
    TimedStorage<int> queue;
    std::map<std::chrono::time_point<std::chrono::system_clock>, int> queToClear;
    
    auto a_idx = queue.push(3, std::chrono::milliseconds(3000)); // Добавляем элемент с таймаутом 3 секунды
    auto b_idx = queue.push(4, std::chrono::milliseconds(1000)); // Добавляем элемент с таймаутом 1 секунда

    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Ждем 2 секунды
    std::future<void> fuature = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "in async\n";
    });
    std::cout << "not async\n";

    auto [b_flag, b_value] = queue.pop(b_idx); // Возвратит пару (false, 0)
    auto [a_flag, a_value] = queue.pop(a_idx); // Возвратит пару (true, 3)
    std::cout << b_flag << " " << a_flag << std::endl;
    auto a_clear = boost::get_optional_value_or(a_value, 0);
    auto b_clear = boost::get_optional_value_or(b_value, 0);

    assert(a_flag == true);
    assert(a_clear == 3);
    assert(b_flag == false);
    assert(b_clear == 0);

    return 0;
}
