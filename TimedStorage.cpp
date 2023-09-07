#include "TimedStorage.h"


int main() {
    TimedStorage<int> queue;
    auto a_idx = queue.push(3, std::chrono::milliseconds(3000)); // Добавляем элемент с таймаутом 3 секунды
    auto b_idx = queue.push(4, std::chrono::milliseconds(1000)); // Добавляем элемент с таймаутом 1 секунда

    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Ждем 2 секунды

    auto [b_flag, b_value] = queue.pop(b_idx); // Возвратит пару (false, 0)
    auto [a_flag, a_value] = queue.pop(a_idx); // Возвратит пару (true, 3)

    return 0;
}

// template <typename ItemT>
// struct ItemWithTimer
// {
//     const std::chrono::time_point<std::chrono::system_clock> itemCreated;
//     const std::chrono::time_point<std::chrono::system_clock> itemDeadTime;
//     const ItemT item;

    // делема с забиранием: по ссылке или мувать или в тупую копировать обжект Ы.
    // а если итем простой то можно и копирнуть.
    // template<typename ItemT>
    // ItemWithTimer<ItemT>::ItemWithTimer(const ItemT itemIn, const std::chrono::microseconds timeout) : 
    //    itemCreated(std::chrono::system_clock::now()),
    //    itemTimeout(itemCreated + timeout)
    // {
    //     item.load(itemIn);
    // };



    
    // bool CompateTimes
// };


/// Interface of TimedQueue
// template <typename ItemT>
// class TimedStorage {
// private:
//     // как будто можно юзать вектор или мапу. Смотря что нам нужно, скорость или память. А вот как это объеденить. Ы
//     // пусть это будет мапо
//     std::map<int, ItemT> que;
// public:
//     /// Add element item_t to the queue with timeout
//     /// Return index of the added element
// template <typename ItemT>
// int TimedStorage<ItemT>::push(ItemT item, std::chrono::milliseconds timeout) {
//     // проверку на отрицательный таймаут не делаю с расчетом что пограмист не дурачек
//     currentIndex.fetch_add(1);
//     que.insert([currentIndex, ItemWithTimer<ItemT>(item, timeout)]);
//     return currentIndex.load();
// };

    // Pop element from the queue with index idx
    // Return <exist_flag, element>
// template <typename ItemT>
// std::pair<bool, ItemT> TimedStorage::pop(int idx) {
//     // как будто можно возвращать boost::optional если время кончилось

// }
// };