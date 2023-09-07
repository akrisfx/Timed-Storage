
/*
Необходимо реализовать шаблонный класс временной очереди, где тип шаблона отвечает за хранимый тип данных в контейнере.
Временная очередь это контейнер, который автоматически удаляет элементы из него, по истечению времени timeout.

Необходимо реализовать два метода:
Метод добавления элемента в очередь.
    int push(ItemT item, std::chrono::milliseconds timeout)
    Принимает два параметра:
        item - элемент который необходимо добавить
        timeout - время в миллисекундах, по истечению которого необходимо удалить элемент
    Возвращает:
        Уникальный идентификатор объекта, по которому можно будет позже его получить

Метод получения элемента из очереди
    std::pair<bool, ItemT> pop(int idx)
    Принимает:
        idx - уникальный идентификатор объекта, который вернул метод push()
    Возвращает:
        Пару из двух элементов: флага и значения.
        Если элемент на момент вызова pop() все еще находится в очереди, то метод должен вернуть
        пару {true, item}, то есть флаг показывает наличие элемента в контейнере на момент вызова
        Если элемента в контейнере нет, то возвращается пара {false, item}, где item это объект, сконструированнйы по умолчанию
    Примечания:
        Метод pop() не является блокирующим и не должен дожидаться появления элемента в контейнере

Пример использования:
    int main() {
        TimedStorage<int> queue;
        auto a_idx = queue.push(3, std::chrono::milliseconds(3000)); // Добавляем элемент с таймаутом 3 секунды
        auto b_idx = queue.push(4, std::chrono::milliseconds(1000)); // Добавляем элемент с таймаутом 1 секунда

        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Ждем 2 секунды

        auto [b_flag, b_value] = queue.pop(b_idx); // Возвратит пару (false, 0)
        auto [a_flag, a_value] = queue.pop(a_idx); // Возвратит пару (true, 3)

        return 0;
    }

Примечания:
    Очередь должна быть потокобезопасна, то есть должна иметь возможность работы с несколькими потоками одновременно.



    Ниже приведено примерное объявление класса. Необходимо дополнить его необходимыми полями и реализовать методы push() и pop().
    Для реализации желательно использовать STL, а также стандарт языка C++17.
*/


#ifndef TIMED_STORAGE_H
#define TIMED_STORAGE_H

#include <chrono>
#include <thread>
#include <map>
#include <atomic>
#include <boost/optional.hpp>
#include "utility"
typedef std::chrono::time_point<std::chrono::system_clock> time_point;



template <typename ItemT>
class TimedStorage {
private:
    // следует ли тут хранить атомик ItemT?
    std::map<int, std::pair<std::chrono::time_point<std::chrono::system_clock>, ItemT>> que;
    std::atomic<int> currentIndex = 0;

public:
    /// Add element item_t to the queue with timeout
    /// Return index of the added element
    int push(ItemT item, std::chrono::milliseconds timeout) {
        // проверку на отрицательный таймаут не делаю с расчетом что пограмист не дурачек
        currentIndex.fetch_add(1);
        std::pair<int, std::pair<std::chrono::time_point<std::chrono::system_clock>, ItemT>> toInsert;
        que.insert(std::make_pair(
            currentIndex.load(std::memory_order_relaxed),
            std::make_pair(
                time_point(std::chrono::system_clock::now()) + timeout,
                item
            )
        ));
        return currentIndex.load(std::memory_order_relaxed);
    };

    /// Pop element from the queue with index idx
    /// Return <exist_flag, element>
    // std::pair<bool, ItemT> pop(int idx)
    std::pair<bool, boost::optional<ItemT>> pop(int idx){
        if(que.find(idx) != que.end()) {
            if (que[idx].first > std::chrono::system_clock::now()){
                return std::make_pair(true, que[idx].second);
            } else {
                que.erase(idx);
                return std::make_pair(false, boost::none);
            }
        } else {
            return std::make_pair(false, boost::none); // решил использовать boost::optional
            //return std::make_pair(false, ItemT()) // если всё таки использовать дефолтный конструктор
        }
    }
};

/////////// все что снизу не робит :-). возможно я просто забыл перегрузить какойт оператор или конструктор (правило 5)

//template <typename ItemT>
//class ItemWithTimer
//{
//public:
//    std::chrono::time_point<std::chrono::system_clock> itemCreated;
//    std::chrono::time_point<std::chrono::system_clock> itemTimeout;
//    std::atomic<ItemT> item; // а так ли необходимо юзать std::atimic если пишем мы туда 1 раз
//
//    // делема с забиранием: по ссылке или мувать или в тупую копировать обжект Ы.
//    // а если итем простой то можно и копирнуть.
//    ItemWithTimer(const ItemT& itemIn, const std::chrono::microseconds timeout) {
//        itemCreated = std::chrono::system_clock::now();
//        itemTimeout = itemCreated + timeout;
//        item.store(itemIn);
//    };
//    ItemWithTimer(ItemT&& itemIn, const std::chrono::microseconds timeout) {
//        itemCreated = std::chrono::system_clock::now();
//        itemTimeout = itemCreated + timeout;
//        item.store(std::move(itemIn));
//    }
//    ItemWithTimer(){
//        itemCreated = {};
//        itemTimeout = {};
//        item = ItemT();
//    }
//    ItemWithTimer& operator=(const ItemWithTimer& right){
//        if(this == &right){
//            return *this;
//        }
//        itemCreated = right.itemCreated;
//        itemTimeout = right.itemTimeout;
//        item = right;
//        return *this;
//    }
//};

/// Interface of TimedQueue
// template <typename ItemT>
// class TimedStorage {
// private:
//     std::map<int, ItemWithTimer<ItemT>> que;
//     std::atomic<int> currentIndex = 0;

// public:
//     /// Add element item_t to the queue with timeout
//     /// Return index of the added element
//     int push(ItemT item, std::chrono::milliseconds timeout) {
//         // проверку на отрицательный таймаут не делаю с расчетом что пограмист не дурачек
//         currentIndex.fetch_add(1);
// //        std::pair<int, ItemWithTimer<ItemT>> toInsert;
// //        toInsert.first = currentIndex.load(std::memory_order_relaxed);
// //        toInsert.second = ItemWithTimer<ItemT>(item, timeout);
// //        que.insert(toInsert);
// //        que.insert(std::make_pair(currentIndex.load(std::memory_order_relaxed), ItemWithTimer<ItemT>(item, timeout)));
//         que.emplace(currentIndex.load(std::memory_order_relaxed), ItemWithTimer<ItemT>(item, timeout));
//         return currentIndex.load(std::memory_order_relaxed);
//     };

//     /// Pop element from the queue with index idx
//     /// Return <exist_flag, element>

//     std::pair<bool, boost::optional<ItemT>> pop(int idx){
//         if(que.find(idx) != que.end()) {
//             return std::make_pair(true, que[idx].item.load(std::memory_order_relaxed));
//         } else {
//             return std::make_pair(false, boost::none); // решил использовать boost::optional
//         }
//     }
// };

#endif