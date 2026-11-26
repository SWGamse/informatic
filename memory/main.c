#include <stdio.h>
#include <limits.h>

char memory[65536];
char data[256*1024];

long long get_sum(int N);

int main(void) {
    setbuf(stdin, NULL);

    int ch;
    int j = 0;
    int error_code = 0;
    int user_num = 0;
    int has_input = 0;

    char model[64] = {0};
    for (j = 0; (ch = getchar()) != EOF && ch != '\n'; j++) {
        if (j < 63) {
            model[j] = ch;
        } else {
            error_code = 1;
        }
    }

    if (error_code == 0) {
        model[j < 63 ? j : 63] = '\0';
        if (model[0] == '\0') {
            error_code = 2;
        }
    }

    while (error_code == 0 && (ch = getchar()) != EOF && ch != '\n') {
        if (ch < '0' || ch > '9') {
            error_code = 3;
            user_num = -1;

            while ((ch = getchar()) != '\n' && ch != EOF);

            break;
        }
        if (user_num > (INT_MAX - (ch - '0')) / 10) { 
            error_code = 4;
            user_num = -1;

            while ((ch = getchar()) != '\n' && ch != EOF);

            break;
        }
        has_input = 1;
        user_num = user_num * 10 + (ch - '0');
    }
    if (!has_input && error_code == 0) {
        error_code = 5;
    }

    int N = user_num;

    if (error_code == 0 && (N > 100000 || N == 0)) {
        error_code = 6;
    }


    if (error_code == 0) {
        long long sum = get_sum(N);
        int key = model[0];

        char sum_str[32];
        int char_counter = 0;
        while (sum != 0) {
            sum_str[char_counter] = sum % 10 + '0';
            sum /= 10;
            char_counter++;
        }
        
        for (int i = 0; i < char_counter / 2; i++) {
            char temp = sum_str[i];
            sum_str[i] = sum_str[char_counter - i - 1];
            sum_str[char_counter - i - 1] = temp;
        }
        sum_str[char_counter] = '\0';

        char encrypted_str[32];
        for (int i = 0; i < char_counter; i++) {
            encrypted_str[i] = (sum_str[i] - '0' + key) % 10 + '0';
        }
        encrypted_str[char_counter] = '\0';

        printf("UNENCRYPTED SUM: ");
        for (int i = 0; i < char_counter; i++) {
            putchar(sum_str[i]);
        }
        printf("\n");

        printf("ENCRYPTED SUM: ");
        for (int i = 0; i < char_counter; i++) {
            putchar(encrypted_str[i]);
        }
        printf("\n");

    }

    switch (error_code) {
        case 0:
            printf("\n\033[92mProgram executed successfully.\033[0m\n");
            break;
        case 1:
            printf("\n\033[91mError code 1: Too long CPU model name.\033[0m\n");
            break;
        case 2:
            printf("\n\033[91mError code 2: CPU model not recognized.\033[0m\n");
            break;
        case 3:
            printf("\n\033[91mError code 3: Invalid input.\033[0m\n");
            break;
        case 4:
            printf("\n\033[91mError code 4: Integer overflow.\033[0m\n");
            break;
        case 5:
            printf("\n\033[91mError code 5: Empty input.\033[0m\n");
            break;
        case 6:
            printf("\n\033[91mError code 6: Incorrect input number value.\033[0m\n");
            break;
        default:
            printf("\n\033[91mError: Unknown error.\033[0m\n");
    }
    
    return 0;    
}


long long get_sum(int N) {
    long long sum = 0;
    for (long long i = 1; i <= N; i++) {
        sum += i;
        memory[i % 65536] = i % 256;
        data[i % (256 * 1024)] = (sum + i) % 256;
    }
    return sum;
}