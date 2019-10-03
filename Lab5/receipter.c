#include <stdio.h>
#include "receipter.h"

static const char* s_menu_list[10];
static unsigned int s_order_num = 0;
static double s_menu_price_list[10];
static const char* s_message;
static unsigned int s_menu_count = 0;
static double s_tip = 0;

int add_item(const char* name, double price)
{
    if (s_menu_count == 10 || price >= 10000.00 || price < 0 || name == NULL) {
        return FALSE;
    }

    s_menu_list[s_menu_count] = name;
    s_menu_price_list[s_menu_count] = price;
    s_menu_count++;
    return TRUE;
}

void add_tip(double tip)
{
    s_tip += tip;
}

void add_message(const char* message)
{
    s_message = message;
}

int print_receipt(const char* filename, time_t timestamp)
{
    unsigned int i;
    unsigned int j;
    FILE* stream;
    double subtotal = 0;
    double tax;
    double total;
    char line[52];
    char time_buffer[20];
    char food[26];

    if (s_menu_count == 0) {
        return FALSE;
    }

    stream = fopen(filename, "wb");
    if (stream == NULL) {
        return FALSE;
    }
    fwrite("Charles' Seafood\n", 17, 1, stream);
    fprintf(stream, "%s", "--------------------------------------------------\n");
    strftime(time_buffer, 20, "%Y-%M-%d %H:%M:%S", localtime(&timestamp));
    sprintf(line, "%s                          %05d\n", time_buffer, s_order_num);
    fwrite(line, 51, 1, stream);
    fprintf(stream, "%s", "--------------------------------------------------\n");

    for (i = 0; i < s_menu_count; ++i) {
        for (j = 0; j < 25; ++j) {
            food[j] = s_menu_list[i][j];
        }
        food[j] = '\0';

        fprintf(stream, "%33s           %6.2f\n", food, s_menu_price_list[i]);
        subtotal += s_menu_price_list[i];
    }
    fprintf(stream, "%s", "\n");

    fprintf(stream, "%33s       %10.2f\n", "Subtotal", subtotal);
    if (s_tip > 0) {
        fprintf(stream, "%33s       %10.2f\n", "Tip", s_tip);
    }
    tax = subtotal * 0.05;
    fprintf(stream, "%33s       %10.2f\n", "Tax", tax);
    total = subtotal + s_tip + tax;
    fprintf(stream, "%33s       %10.2f\n", "Total", total);
    fprintf(stream, "%s", "\n");
    if (s_message != NULL) {
        i = 0;
        while (TRUE) {
            if (i == 50) {
                line[50] = '\n';
                line[51] = '\0';
                fprintf(stream, "%s", line);
            }
            if (*s_message == '\0' || i == 75) {
                line[i % 50] = '\n';
                line[i % 50 +1] = '\0';
                fprintf(stream, "%s", line);
                break;
            }
            line[i % 50] = *s_message;
            s_message++;
            i++;
        }

    }
    fprintf(stream, "%s", "==================================================\n");
    fprintf(stream, "%50s\n", "Tax#-51234");
    fclose(stream);

    s_order_num++;
    s_message = NULL;
    s_menu_count = 0;
    s_tip = 0;

    return TRUE;
}
