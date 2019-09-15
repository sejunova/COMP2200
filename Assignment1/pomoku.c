#include "pomoku.h"

static int board[20][20];
static size_t row_count;
static size_t col_count;
static int black_score;
static int white_score;

void init_game(void)
{
    size_t i;
    size_t j;

    /* 보드 초기화 */
    for (i = 0; i < ROW_SIZE_MAX; i++) {
        for (j = 0; j < COLUMN_SIZE_MAX; j++) {
            board[i][j] = -1;
        }
    }

    /* 점수 초기화*/
    black_score = 0;
    white_score = 0;

    /* 15 x 15 판으로 게임 시작*/
    row_count = ROW_SIZE_START;
    col_count = COLUMN_SIZE_START;
}

size_t get_row_count(void)
{
    return row_count;
}

size_t get_column_count(void)
{
    return col_count;
}

int get_score(const color_t color)
{
    int score;
    switch (color) {
        case COLOR_BLACK:
            score = black_score;
            break;
        case COLOR_WHITE:
            score = white_score;
            break;
        default:
            score = -1;
            break;
    }
    return score;
}

int get_color(const size_t row, const size_t col)
{
    int color;
    if (row >= row_count || col >= col_count) {
        return -1;
    }

    switch (board[row][col]) {
        case COLOR_BLACK:
            color = COLOR_BLACK;
            break;
        case COLOR_WHITE:
            color = COLOR_WHITE;
            break;
        default:
            color = -1;
            break;
    }
    return color;
}

int is_placeable(const size_t row, const size_t col)
{
    if (row >= row_count || col >= col_count) {
        return FALSE;
    }

    if (board[row][col] != -1) {
        return FALSE;
    }
    return TRUE;
}

int calculate_scoring(const color_t color, const size_t row, const size_t col)
{
    int score_sum = 0;
    int horizontal_count = 1;
    int vertical_count = 1;
    int diagonal_left_count = 1;
    int diagonal_right_count = 1;
    size_t i;
    size_t j;

    /* horizontal */
    /* left */
    if (col >= 1) {
        i = col - 1;
        while (i >= 0) {
            if (board[row][i] == color) {
                horizontal_count++;
            } else {
                break;
            }
            /* prevent size_t type underflow */
            if (i == 0) {
                break;
            }
            i--;
        }
    }

    /* right */
    i = col + 1;
    while (i < col_count) {
        if (board[row][i] == color) {
            horizontal_count++;
        } else {
            break;
        }
        i++;
    }

    /* vertical */
    /* up */
    if (row >= 1) {
        i = row - 1;
        while (i >= 0) {
            if (board[i][col] == color) {
                vertical_count++;
            } else {
                break;
            }
            /* prevent size_t type underflow */
            if (i == 0) {
                break;
            }
            i--;
        }
    }

    /* down */
    i = row + 1;
    while (i < row_count) {
        if (board[i][col] == color) {
            vertical_count++;
        } else {
            break;
        }
        i++;
    }

    /* diagonal left */
    /* up */
    if (row >= 1 && col >= 1) {
        i = row - 1;
        j = col - 1;

        while (i >= 0 && j >= 0) {
            if (board[i][j] == color) {
                diagonal_left_count++;
            } else {
                break;
            }

            /* prevent size_t type underflow */
            if (i == 0 || j == 0) {
                break;
            }
            i--;
            j--;
        }
    }

    /* down */
    i = row + 1;
    j = col + 1;

    while (i < row_count && j < col_count) {
        if (board[i][j] == color) {
            diagonal_left_count++;
        } else {
            break;
        }
        i ++;
        j ++;
    }

    /* diagonal right */
    /* up */
    if (row >= 1) {
        i = row - 1;
        j = col + 1;

        while (i >= 0 && j < col_count) {
            if (board[i][j] == color) {
                diagonal_right_count++;
            } else {
                break;
            }
            if (i == 0) {
                break;
            }
            i--;
            j++;
        }
   }

    /* down */
    if (col >= 1) {
        i = row + 1;
        j = col - 1;

        while (i < row_count && j >= 0) {
            if (board[i][j] == color) {
                diagonal_right_count++;
            } else {
                break;
            }
            if (j == 0) {
                break;
            }
            i++;
            j--;
        }
    }

    if (horizontal_count >= 5) {
        score_sum += (horizontal_count - 4);
    }
    if (vertical_count >= 5) {
        score_sum += (vertical_count - 4);
    }
    if (diagonal_left_count >= 5) {
        score_sum += (diagonal_left_count - 4);
    }
    if (diagonal_right_count >= 5) {
        score_sum += (diagonal_right_count - 4);
    }
    return score_sum;
}

void change_score(const color_t color, int score) {
    if (color == COLOR_BLACK) {
        black_score += score;
    } else {
        white_score += score;
    }
}

int place_stone(const color_t color, const size_t row, const size_t col)
{
    int score_to_add;
    if (is_placeable(row, col)) {
        board[row][col] = color;
        score_to_add = calculate_scoring(color, row, col);

        change_score(color, score_to_add);
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int insert_row(const color_t color, const size_t row)
{
    size_t i;
    size_t j;
    if (row_count == ROW_SIZE_MAX || get_score(color) < INSERT_AND_REMOVE_SKILL_POINT || row > row_count) {
        return FALSE;
    }

    for (i = row_count; i >= row; i--) {
        for (j = 0; j < col_count; j++) {
            board[i + 1][j] = board[i][j];
        }
    }

    for  (j = 0; j < col_count; j++) {
        board[row][j] = -1;
    }

    change_score(color, -INSERT_AND_REMOVE_SKILL_POINT);
    row_count++;
    return TRUE;
}

int insert_column(const color_t color, const size_t col)
{
    size_t i;
    size_t j;
    if (col_count == COLUMN_SIZE_MAX || get_score(color) < INSERT_AND_REMOVE_SKILL_POINT || col > col_count) {
        return FALSE;
    }

    for (i = col_count; i >= col; i--) {
        for (j = 0; j < row_count; j++) {
            board[j][i + 1] = board[i][j];
        }
    }

    for  (j = 0; j < row_count; j++) {
        board[j][col] = -1;
    }

    change_score(color, -INSERT_AND_REMOVE_SKILL_POINT);
    row_count++;
    return TRUE;
}


int remove_row(const color_t color, const size_t row)
{
    size_t i;
    size_t j;

    if (row_count == ROW_SIZE_MIN || get_score(color) < INSERT_AND_REMOVE_SKILL_POINT || row >= row_count) {
        return FALSE;
    }

    for (i = row; i < row_count; i++) {
        for (j = 0; j < col_count; j++) {
            board[i][j] = board[i + 1][j];
        }
    }

    change_score(color, -INSERT_AND_REMOVE_SKILL_POINT);
    row_count--;
    return TRUE;
}

int remove_column(const color_t color, const size_t col)
{
    size_t i;
    size_t j;

    if (col_count == COLUMN_SIZE_MIN || get_score(color) < INSERT_AND_REMOVE_SKILL_POINT || col >= col_count) {
        return FALSE;
    }

    for (i = col; i < col_count; i++) {
        for (j = 0; j < row_count; j++) {
            board[j][i] = board[j][i + 1];
        }
    }

    change_score(color, -INSERT_AND_REMOVE_SKILL_POINT);
    col_count--;
    return TRUE;
}


int swap_rows(const color_t color, const size_t row0, size_t const row1)
{
    int temp;
    size_t i;

    if (row0 >= row_count || row1 > row_count) {
        return FALSE;
    }

    for (i = 0; i < col_count; i++) {
        temp = board[row0][i];
        board[row0][i] = board[row1][i];
        board[row1][i] = temp;
    }

    change_score(color, -SWAP_SKILL_POINT);
    return TRUE;
}

int swap_columns(const color_t color, const size_t col0, const size_t col1)
{
    int temp;
    size_t i;

    if (col0 >= col_count || col1 > col_count) {
        return FALSE;
    }

    for (i = 0; i < row_count; i++) {
        temp = board[i][col0];
        board[i][col0] = board[i][col1];
        board[col1][i] = temp;
    }

    change_score(color, -SWAP_SKILL_POINT);
    return TRUE;
}


int copy_row(const color_t color, const size_t src, const size_t dst)
{
    size_t i;

    if (src >= row_count || dst > row_count) {
        return FALSE;
    }

    for (i = 0; i < col_count; i++) {
        board[dst][i] = board[src][i];
    }

    change_score(color, -COPY_SKILL_POINT);
    return TRUE;
}


int copy_column(const color_t color, const size_t src, const size_t dst)
{
    size_t i;

    if (src >= col_count || dst > col_count) {
        return FALSE;
    }

    for (i = 0; i < row_count; i++) {
        board[i][dst] = board[i][src];
    }

    change_score(color, -COPY_SKILL_POINT);
    return TRUE;
}

