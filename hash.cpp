#include <unordered_map>
#include <iostream>
#include <string>

void demo_unordered_map() {
    // Создание хэш-таблицы
    std::unordered_map<std::string, int> ages;
    
    // Вставка элементов
    ages["Alice"] = 30;
    ages["Bob"] = 25;
    ages["Charlie"] = 35;
    ages.insert({"David", 28});
    
    // Доступ к элементам
    std::cout << "Alice's age: " << ages["Alice"] << std::endl;
    std::cout << "Bob's age: " << ages.at("Bob") << std::endl;
    
    // Проверка существования ключа
    if (ages.find("Eve") == ages.end()) {
        std::cout << "Eve not found" << std::endl;
    }
    
    // Итерация
    std::cout << "\nAll elements:" << std::endl;
    for (const auto& pair : ages) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
    
    // Размер и статистика
    std::cout << "\nSize: " << ages.size() << std::endl;
    std::cout << "Bucket count: " << ages.bucket_count() << std::endl;
    std::cout << "Load factor: " << ages.load_factor() << std::endl;
}