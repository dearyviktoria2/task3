import java.util.HashMap;
import java.util.Map;

public class HashMapExample {
    public static void main(String[] args) {
        Map<String, Integer> map = new HashMap<>();
        
        // Добавление элементов
        map.put("Alice", 30);
        map.put("Bob", 25);
        map.put("Charlie", 35);
        
        // Получение значения по ключу
        Integer age = map.get("Alice");
        System.out.println("Alice's age: " + age);
        
        // Проверка существования ключа
        if (map.containsKey("Bob")) {
            System.out.println("Bob is in the map");
        }
        
        // Итерация по элементам
        for (Map.Entry<String, Integer> entry : map.entrySet()) {
            System.out.println(entry.getKey() + ": " + entry.getValue());
        }
    }
}