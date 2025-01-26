#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
/* 
memset(заполняет массив нулями), printf, malloc, free, write, usleep,

gettimeofday,

pthread_create,
pthread_detach, pthread_join, pthread_mutex_init,
pthread_mutex_destroy, pthread_mutex_lock,
pthread_mutex_unlock
 */

// Получить тек время
void	get_time(void) // преобразовать эту ф-ю так, чтобы она показывала время в читательном формате
{
	struct timeval tv;

	gettimeofday(&tv, NULL); // второй аргумент отвечает за timezone
	printf("Seconds since epoch: %ld\n", tv.tv_sec);
	printf("Microseconds: %ld\n", tv.tv_usec);
}


int main(int n, char **args)
{
	get_time();
}