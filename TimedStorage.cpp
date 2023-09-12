#include "TimedStorage.h"
#include <iostream>
#include <optional>


int main() {
    TimedStorage<int> queue;

    auto a_idx = queue.push(3, 3000ms); // Добавляем элемент с таймаутом 3 секунды
    auto b_idx = queue.push(4, 1000ms); // Добавляем элемент с таймаутом 1 секунда
    auto c_idx = queue.push(8, 30ms);
    auto [c1_flag, c1_value] = queue.pop(c_idx); 
    assert(boost::get_optional_value_or(c1_value, 0) == 8);
    

    std::this_thread::sleep_for(2000ms); // Ждем 2 секунды

    auto [b_flag, b_value] = queue.pop(b_idx); // Возвратит пару (false, 0)
    auto [a_flag, a_value] = queue.pop(a_idx); // Возвратит пару (true, 3)
    std::tie(c1_flag, c1_value) = queue.pop(c_idx); 

    std::cout << b_flag << " " << a_flag << std::endl;
    auto a_clear = boost::get_optional_value_or(a_value, 0);
    auto b_clear = boost::get_optional_value_or(b_value, 0);

    assert(boost::get_optional_value_or(c1_value, 0) == 0);
    assert(a_flag == true);
    assert(a_clear == 3);
    assert(b_flag == false);
    assert(b_clear == 0);

    return 0;
}
