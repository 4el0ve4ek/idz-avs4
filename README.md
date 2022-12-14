
# Индивидуальное домашнее заданее №4 по предмету Архитектура Вычислительных систем
### Аксенов Даниил, БПИ218, Вариант 28

Первая военная задача. 
Темной-темной ночью прапорщики Иванов, Петров и Нечепорук занимаются хищением военного имущества со склада родной военной части.
Будучи умными людьми и отличниками боевой и строевой подготовки, прапорщики ввели разделение труда.
Иванов выносит имущество со склада и передает его в руки Петрову, который грузит его в грузовик. 
Нечепорук стоит на шухере и заодно подсчитывает рыночную стоимость добычи поле погрузки в грузовик очередной партии похищенного.
Требуется составить многопоточное приложение, моделирующее деятельность прапорщиков.
При решении использовать парадигму «производитель-потребитель».

### Входные параметры
Это не указано явно в условии задачи, поэтому мной было выбран следующий входной параметр:
- n - количество имущества на складе(натуральное число)

Входные данные можно передать при вызове: `./main 10`. Либо через поток ввода, если вызвать программу без параметров:

```
./main
10
```


### Решение
Данная задача не вызвала трудностей. В условии мы видим трех различных людей, каждый из которых занимается своими вещами.
Связаны они между собой только точками передачи "имущества". Данную концепцию легко перенести в код.
Понятно, что это 3 потока(по одному на человека), связанные между собой семафорами. 
Эти потоки представляют собой конвейер -- 
Первый занимает позицию во "входном" семафоре и освобождает позицию во втором семафоре, Второй поток "берет" из второго семафора и "кладет" в третьий семафор и тд

По итогу был написан код, который содержится в файле `main.c`. Компиляция проводилась с помощью комманды `gcc main.c -o main -pthread` -- для gcc надо явно указать, что ты используешь потоки


### Тестовое покрытие

В данной задачи тут нет особой логики, которую нужно покрыть тестами. Поэтому тут будут тесты, которые просто проверяют работоспособность программы.
Для того чтобы было понятно на каком этапе выполнения сейчас находиться код. 
Были добавлен вывод доп информации внутрь функций, которые выполняются в дочерних потоках, чтобы убедиться в многопоточности моего приложения.

#### Тест 1:
Входные данные:
```
1
```
Выходные данные:
```
IVANOV STEAL
PETROV MOVE
NECHEPORUK COUNT
DONE
```

#### Тест 2:
Входные данные:
```
2
```
Выходные данные:

Тут может быть 5 вариантов(если ничего не пропустил), перечислю их: 
```
IVANOV STEAL     |   IVANOV STEAL     |   IVANOV STEAL
PETROV MOVE      |   PETROV MOVE      |   PETROV MOVE
NECHEPORUK COUNT |   IVANOV STEAL     |   IVANOV STEAL
IVANOV STEAL     |   NECHEPORUK COUNT |   PETROV MOVE
PETROV MOVE      |   PETROV MOVE      |   NECHEPORUK COUNT
NECHEPORUK COUNT |   NECHEPORUK COUNT |   NECHEPORUK COUNT
DONE             |   DONE             |   DONE
```

```
IVANOV STEAL     |   IVANOV STEAL    
IVANOV STEAL     |   IVANOV STEAL    
PETROV MOVE      |   PETROV MOVE     
PETROV MOVE      |   NECHEPORUK COUNT 
NECHEPORUK COUNT |   PETROV MOVE      
NECHEPORUK COUNT |   NECHEPORUK COUNT 
DONE             |   DONE             
```

#### Тест 3:
Входные данные:
```
5
```
Выходные данные:

Как можно заметить -- количество различных вариантов ответа растет, но у всех есть общие правила.
Во-первых, в ответе всегда будет ровно n строк каждого типа действий. Во-вторых перед каждой i-ой строкой вида `PETROV MOVE` будет хотя бы i строк вида `IVANOV STEAL`, потому что Петров не может переносить то, что Иванов еще не своровал(Аналогично про действия Нечепорука и Петрова). И наконец, `DONE` будет всегда последней в потоке вывода.

Возможный ответ в данном случае:
```
IVANOV STEAL
IVANOV STEAL
IVANOV STEAL
IVANOV STEAL
IVANOV STEAL
PETROV MOVE
PETROV MOVE
PETROV MOVE
NECHEPORUK COUNT
NECHEPORUK COUNT
NECHEPORUK COUNT
PETROV MOVE
PETROV MOVE
NECHEPORUK COUNT
NECHEPORUK COUNT
DONE
```