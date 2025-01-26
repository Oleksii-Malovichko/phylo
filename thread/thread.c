#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
/* 
pthread_create,
pthread_detach, pthread_join, pthread_mutex_init,
pthread_mutex_destroy, pthread_mutex_lock,
pthread_mutex_unlock */

// ПОТОКИ И МЬЮТЕКСЫ (POSIX Thread)
// phread_create (создать новый поток)

int	counter = 0;
int	counter2 = 0;
pthread_mutex_t mutex;

void	*thread_function(void *arg)
{
	printf("Hello from thread\n");
	return NULL;
}

void	*long_task(void *arg)
{
	sleep(3);
	printf("Long task is finished\n");
	return NULL;
}

void	*compute_task(void *arg)
{
	sleep(1);
	printf("Compute task is finished\n");
	return NULL;
}

void	*thread_func(void *arg)
{
	int *num = (int*)arg;
	*num += 10;
	return (void *)num;
}

void	*increment_without_mutex(void *arg)
{
	int i = 0;
	while (i < 100000)
	{
		counter++;
		i++;
	}
	return NULL;
}

void	*increment_with_mutex(void *arg)
{
	int i = 0;
	while (i < 100000)
	{
		pthread_mutex_lock(&mutex); // захватываем mutex
		counter2++;
		i++;
		pthread_mutex_unlock(&mutex); // освобождаем mutex
	}
}

int main()
{
	pthread_t thread1;
	pthread_t thread2;
	pthread_t thread3;
	pthread_t thread4;
	pthread_t thread5;

	int value = 5;
	int *result;
	if (pthread_create(&thread1, NULL, long_task, NULL) != 0)
		return 1;
	pthread_detach(thread1); 	// не ждем завершения, потоку даем освободить ресурсы автоматически

	if (pthread_create(&thread2, NULL, compute_task, NULL) != 0)
		return 1;
	pthread_join(thread2, NULL); // Ожидаем завершения, чтобы получить результат


	if (pthread_create(&thread3, NULL, thread_func, &value) != 0)
		return 1;
	pthread_join(thread3, (void **)&result);// Получить результат ф-ии
	printf("Value after third thread execution: %d\n", *result);

	// Пример использования общих ресурсов БЕЗ синзронизации, то есть без мьютекса
	// В данном примере описывается гонка за ресурсами, что не является хорошим признаком, так как может привести к непредсказуемым результатам
	pthread_create(&thread4, NULL, increment_without_mutex, NULL);
	pthread_create(&thread5, NULL, increment_without_mutex, NULL);
	pthread_join(thread4, NULL);
	pthread_join(thread5, NULL);
	printf("Результат counter без mutex: %d\n", counter); // ожидаемый результат: 200000. Фактический результат: непредсказуем
	// Пример с использование общих ресурсов С mutex (меняется только ф-я)
	pthread_mutex_init(&mutex, NULL); // инициализация мьтекса
	pthread_create(&thread4, NULL, increment_with_mutex, NULL);
	pthread_create(&thread5, NULL, increment_with_mutex, NULL);
	pthread_join(thread4, NULL);
	pthread_join(thread5, NULL);
	pthread_mutex_destroy(&mutex); // разрушаем мьютекс
	printf("Результат counter с mutex: %d\n", counter2); // результат: 200000
}
/* 
В этом примере thread1 выполняет долгую задачу. Мы отсоединяем его, потому что результат нам не нужен и мы не хотим ждать.
thread2 выполняет задачу, результат которой нам нужно обработать. Поэтому мы ожидаем его завершение с помощью pthread_join.*/