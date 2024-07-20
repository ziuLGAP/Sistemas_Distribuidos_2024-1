#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

void generate_numbers(int *numbers, int n) {
    int N = 1;
    for (int i = 0; i < n; i++) {
        int delta = rand() % 100 + 1;
        N += delta;
        numbers[i] = N;
    }
}

int main() {
    srand(time(0));
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[21] = {0};
    int num_numbers = 10;
    int numbers[num_numbers];

    generate_numbers(numbers, num_numbers);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(65432);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    for (int i = 0; i < num_numbers; i++) {
        printf("Enviando: %d\n", numbers[i]);
        snprintf(buffer, sizeof(buffer), "%d", numbers[i]);
        send(sock, buffer, strlen(buffer), 0);
        read(sock, buffer, 20);
        printf("%s\n", buffer);
        sleep(1);
    }

    snprintf(buffer, sizeof(buffer), "0");
    send(sock, buffer, strlen(buffer), 0);
    close(sock);
    return 0;
}