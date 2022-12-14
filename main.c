#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>


int n;
sem_t warehouse;
sem_t toMove;
sem_t toCount;
int inCar;
sem_t takenEverything;


// функция, которая "ворует со склада"
void* takeFromWareHouse(void* args) {
    while(1){
        sem_wait(&warehouse); // берем из одного семафора значения
        printf("IVANOV STEAL\n"); // debug message чтобы увидеть многопоточность
        sem_post(&toMove);    // перекладываем в другой
    }
}

// функция, которая "перемещает товар в машину"
void* moveToCar(void* args) {
    while(1) {
        sem_wait(&toMove);   // берем из одного семафора значения
        printf("PETROV MOVE\n");  // debug message чтобы увидеть многопоточность
        sem_post(&toCount);  // перекладываем в другой
    }
}

// функция, которая "подсчитывает украденное"
void* countStolen(void* args) {
    while (1) {
        sem_wait(&toCount);  // берем из одного семафора значения
        printf("NECHEPORUK COUNT\n");  // debug message, чтобы увидеть многопоточность
        // обновляем счетчик
        // вообще это тонкое место, потому что
        // если countStolen будет вызываться из разных потоков, то произойдет data_race
        // в рамках текущего условия подобное произойти не может
        inCar += 1;
        // Если в машине уже все вещи со склада, то завершаем работу
        // использован знак >= на всякий случай.
        // В нормальной ситуации не может быть inCar > n, но вдруг что-то пойдет не так -- это место может нас спасти
        if(inCar >= n) {
            sem_post(&takenEverything); // даем знак, что мы перенесли все
        }
    }
}

int main(int argc, char* argv[]) {
    // Проверяем параметры командной строки
    if (argc < 2) {
        // если ничего не было передано, то читаем из stdin
        scanf("%d", &n);
    } else {
        // если параметры были переданы, то берем первый, как n
        n = atoi(argv[1]);
    }
    // выводим уточняющую информацию
    printf("there is %d things in warehouse\n", n);

    // выходим при невалидных данных
    if (n <= 0) {
        printf("soldiers go away with empty hands\n");
        printf("invalid number have been passed: %d, but must be positive integer", n);
        return 1;
    }

    // инициализируем семафоры
    sem_init(&warehouse, 0, n); // количество товаров на складе
    sem_init(&toMove, 0, 0); // количество товаров, которые надо перенести в машину
    sem_init(&toCount, 0, 0); // количество вещей, которые надо посчитать
    sem_init(&takenEverything, 0, 0); // уточняющий семафор, уточняющий все ли было украдено
    inCar = 0; // сколько вещей в машине

    // создаем переменные потоков, которые символизирует героев задачи
    pthread_t ivanov, petrov, necheporuk;

    // раздаем нашим героям задачи
    // создаем и стартуем поток, передавая в него исполняемую функцию
    pthread_create(&necheporuk, NULL, countStolen, NULL);
    pthread_create(&petrov, NULL, moveToCar, NULL);
    pthread_create(&ivanov, NULL, takeFromWareHouse, NULL);

    // ожидаем, когда будет посчитано столько, сколько было на складе вещей изначально
    sem_wait(&takenEverything);

    printf("DONE\n");

    return 0;
}