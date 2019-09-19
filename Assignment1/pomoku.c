#include "pomoku.h"

static int s_board[20][20];
static size_t s_row_count;
static size_t s_col_count;
static int s_black_score;
static int s_white_score;

void init_game(void)
{
    size_t i;
    size_t j;

    /* 보드 초기화 */
    for (i = 0; i < ROW_SIZE_MAX; i++) {
        for (j = 0; j < COLUMN_SIZE_MAX; j++) {
            s_board[i][j] = -1;
        }
    }

    /* 점수 초기화*/
    s_black_score = 0;
    s_white_score = 0;

    /* 15 x 15 판으로 게임 시작*/
    s_row_count = ROW_SIZE_START;
    s_col_count = COLUMN_SIZE_START;
}

size_t get_row_count(void)
{
    return s_row_count;
}

size_t get_column_count(void)
{
    return s_col_count;
}

int get_score(const color_t color)
{
    int score;
    switch (color) {
    case COLOR_BLACK:
        score = s_black_score;
        break;
    case COLOR_WHITE:
        score = s_white_score;
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
    if (row >= s_row_count || col >= s_col_count) {
        return -1;
    }

    switch (s_board[row][col]) {
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
    if (row >= s_row_count || col >= s_col_count) {
        return FALSE;
    }

    if (s_board[row][col] != -1) {
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
            if (s_board[row][i] == color) {
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
    while (i < s_col_count) {
        if (s_board[row][i] == color) {
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
            if (s_board[i][col] == color) {
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
    while (i < s_row_count) {
        if (s_board[i][col] == color) {
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
            if (s_board[i][j] == color) {
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

    while (i < s_row_count && j < s_col_count) {
        if (s_board[i][j] == color) {
            diagonal_left_count++;
        } else {
            break;
        }
        i++;
        j++;
    }

    /* diagonal right */
    /* up */
    if (row >= 1) {
        i = row - 1;
        j = col + 1;

        while (i >= 0 && j < s_col_count) {
            if (s_board[i][j] == color) {
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

        while (i < s_row_count && j >= 0) {
            if (s_board[i][j] == color) {
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

void change_score(const color_t color, int score)
{
    if (color == COLOR_BLACK) {
        s_black_score += score;
    } else {
        s_white_score += score;
    }
}

int place_stone(const color_t color, const size_t row, const size_t col)
{
    int score_to_add;
    if (is_placeable(row, col)) {
        s_board[row][col] = color;
        score_to_add = calculate_scoring(color, row, col);

        change_score(color, score_to_add);
        return TRUE;
    } else {
        return FALSE;
    }
}

int insert_row(const color_t color, const size_t row)
{
    size_t i;
    size_t j;
    if (s_row_count == ROW_SIZE_MAX || get_score(color) < INSERT_AND_REMOVE_SKILL_POINT || row >= s_row_count) {
        return FALSE;
    }

    for (i = s_row_count - 1; i > row; i--) {
        for (j = 0; j < s_col_count; j++) {
            s_board[i][j] = s_board[i - 1][j];
        }
        if (i == 0) {
            break;
        }
    }

    for (j = 0; j < s_col_count; j++) {
        s_board[row][j] = -1;
    }

    change_score(color, -INSERT_AND_REMOVE_SKILL_POINT);
    s_row_count++;
    return TRUE;
}

int insert_column(const color_t color, const size_t col)
{
    size_t i;
    size_t j;
    if (s_col_count == COLUMN_SIZE_MAX || get_score(color) < INSERT_AND_REMOVE_SKILL_POINT || col >= s_col_count) {
        return FALSE;
    }

    for (i = s_col_count - 1; i > col; i--) {
        for (j = 0; j < s_row_count; j++) {
            s_board[j][i] = s_board[j][i - 1];
        }
        if (i == 0) {
            break;
        }
    }

    for (j = 0; j < s_row_count; j++) {
        s_board[j][col] = -1;
    }

    change_score(color, -INSERT_AND_REMOVE_SKILL_POINT);
    s_col_count++;
    return TRUE;
}


int remove_row(const color_t color, const size_t row)
{
    size_t i;
    size_t j;

    if (s_row_count == ROW_SIZE_MIN || get_score(color) < INSERT_AND_REMOVE_SKILL_POINT || row >= s_row_count) {
        return FALSE;
    }

    for (i = row; i < s_row_count; i++) {
        for (j = 0; j < s_col_count; j++) {
            s_board[i][j] = s_board[i + 1][j];
        }
    }

    change_score(color, -INSERT_AND_REMOVE_SKILL_POINT);
    s_row_count--;
    return TRUE;
}

int remove_column(const color_t color, const size_t col)
{
    size_t i;
    size_t j;

    if (s_col_count == COLUMN_SIZE_MIN || get_score(color) < INSERT_AND_REMOVE_SKILL_POINT || col >= s_col_count) {
        return FALSE;
    }

    for (i = col; i < s_col_count; i++) {
        for (j = 0; j < s_row_count; j++) {
            s_board[j][i] = s_board[j][i + 1];
        }
    }

    change_score(color, -INSERT_AND_REMOVE_SKILL_POINT);
    s_col_count--;
    return TRUE;
}


int swap_rows(const color_t color, const size_t row0, size_t const row1)
{
    int temp;
    size_t i;

    if (row0 == row1) {
        return FALSE;
    }

    if (row0 >= s_row_count || row1 > s_row_count || get_score(color) < SWAP_SKILL_POINT) {
        return FALSE;
    }

    for (i = 0; i < s_col_count; i++) {
        temp = s_board[row0][i];
        s_board[row0][i] = s_board[row1][i];
        s_board[row1][i] = temp;
    }

    change_score(color, -SWAP_SKILL_POINT);
    return TRUE;
}

int swap_columns(const color_t color, const size_t col0, const size_t col1)
{
    int temp;
    size_t i;

    if (col0 == col1) {
        return FALSE;
    }

    if (col0 >= s_col_count || col1 > s_col_count || get_score(color) < SWAP_SKILL_POINT) {
        return FALSE;
    }

    for (i = 0; i < s_row_count; i++) {
        temp = s_board[i][col0];
        s_board[i][col0] = s_board[i][col1];
        s_board[col1][i] = temp;
    }

    change_score(color, -SWAP_SKILL_POINT);
    return TRUE;
}


int copy_row(const color_t color, const size_t src, const size_t dst)
{
    size_t i;

    if (src == dst) {
        return FALSE;
    }

    if (src >= s_row_count || dst > s_row_count || get_score(color) < COPY_SKILL_POINT) {
        return FALSE;
    }

    for (i = 0; i < s_col_count; i++) {
        s_board[dst][i] = s_board[src][i];
    }

    change_score(color, -COPY_SKILL_POINT);
    return TRUE;
}


int copy_column(const color_t color, const size_t src, const size_t dst)
{
    size_t i;

    if (src == dst) {
        return FALSE;
    }

    if (src >= s_col_count || dst > s_col_count || get_score(color) < COPY_SKILL_POINT) {
        return FALSE;
    }

    for (i = 0; i < s_row_count; i++) {
        s_board[i][dst] = s_board[i][src];
    }

    change_score(color, -COPY_SKILL_POINT);
    return TRUE;
}
