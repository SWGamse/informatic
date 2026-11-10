/*
cpu_state[0] - счётчик команд
cpu_state[1] - аккумулятор
cpu_state[2] - регистр команд
cpu_state[3] - вектор прерываний
cpu_state[4] - флаг остановки
cpu_state[5] - код последней ошибки
cpu_state[6] - резерв
cpu_state[7] - резерв*/

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int cpu_state[8] = {0}; // инициализация массива состояний процессора
    int memory[256] = {0}; // инициализация массива памяти
    int i = 0;

    FILE *program = fopen("program.txt", "r"); //получение указателя на файл и проверка его наличия
    if (program == NULL)
    {
        printf("File not found!\n");
        return 0;
    }
    while (i < 256 && fscanf(program, "%d", &memory[i]) == 1) i++; // перенос комманд в память
    fclose(program);

    //инициализация cpu_state
    cpu_state[0] = 6;
    cpu_state[1] = 0;
    cpu_state[2] = 0;
    cpu_state[3] = memory[0];
    cpu_state[4] = 0;
    cpu_state[5] = 0;
    while (cpu_state[4] == 0 && cpu_state[5] == 0)
    {
        int current_pc = cpu_state[0];
        cpu_state[2] = memory[cpu_state[0]];

        // Если команда требует операнда (не 0)
        if (cpu_state[2] != 0) 
        {
            // Проверка границ для адреса операнда
            if (cpu_state[0] + 1 >= 256 || cpu_state[0] < 0) {
                cpu_state[5] = 1; // Код ошибки
                cpu_state[0] = cpu_state[3]; // Переход на вектор прерываний
                continue; // Цикл заново с нового pc
            }
            int addr = cpu_state[0] + 1; // Адрес ячейки с операндом
            int operand_val = memory[addr]; // Значение операнда (адрес данных)

            // Проверка границ для операнда
            if (operand_val < 0 || operand_val >= 256) {
                cpu_state[5] = 1; // Код ошибки
                cpu_state[0] = cpu_state[3]; // Переход на вектор прерываний
                continue; // Цикл заново с нового pc
        }
            
            

            switch (cpu_state[2]) 
            {
                case 1: // LOAD
                    cpu_state[1] = memory[operand_val];
                    break;
                case 2: // ADD
                    cpu_state[1] += memory[operand_val];
                    break;
                case 3: // SUB
                    cpu_state[1] -= memory[operand_val];
                    break;
                case 4: // JUMP
                    cpu_state[0] = operand_val;
                    break;
                case 5: // JUMP_IF_ZERO
                    if (cpu_state[1] == 0)
                    {
                        cpu_state[0] = operand_val;
                    }
                    else cpu_state[0] += 2; // Переход к следующей инструкции, если условие не выполнено
                    break;
                default:
                    cpu_state[5] = 2; // Код ошибки
                    cpu_state[0] = cpu_state[3]; // Переход на вектор прерываний
                    continue; // Цикл заново с нового pc
            }
            printf("pc=%d: command %d, operand=%d, acc=%d\n", current_pc, cpu_state[2], operand_val, cpu_state[1]);
        } 
        // если команда = 0, завершение выполнения
        else // HALT
        {
            printf("pc=%d: command 0, acc=%d\n", current_pc, cpu_state[1]);
            cpu_state[4] = 1;
            continue; // перевод флага работы в 1, переход к следующей итерации для исключения варианта продолжения работы после смены флага
        }
        //переход к следующей инструкции с шагом 2, пропуская операнды
        cpu_state[0] += (cpu_state[2] != 4 && cpu_state[2] != 5) ? 2 : 0;
    }
    switch (cpu_state[5]) //вывод кода ошибки
    {
        case 0:
            printf("Program completed successfully.\n");
            break;
        case 1:
            printf("Error: Address of operand out of memory.\n");
            break;
        case 2:
            printf("Error: Invalid command.\n");
            break;
        default:
            printf("Unknown error.\n");
            break;
    }

    return 0;
}