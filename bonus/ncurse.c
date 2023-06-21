#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // 初始化操作
    initscr();             // 初始化
    noecho();              // 不顯示輸入的字元
    cbreak();              // 將緩衝區關閉
    keypad(stdscr, TRUE);  // 啟用鍵盤
    curs_set(0);           // 隱藏游標

    chtype cmd;   // 用來存放輸入的命令
    int num = 0;  // 用來存放算盤的數值
    while (true) {
        // 畫出一個算盤（ 5 行的算盤）
        // 先畫出外框
        box(stdscr, 0, 0);
        // 顯示標題與說明文字
        mvprintw(0, 5, "NCURSE Abacus");
        mvprintw(2, 5, "Using Up/Down to change value");
        mvprintw(17, 5, "[ Press Q to quit ]");
        // 顯示算盤的數值
        mvprintw(3, 5, "Value: %d", num);
        // 建立一個 5 行算盤的 Window
        WINDOW *win = newwin(12, 19, 5, 5);
        wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

        // 畫出算盤的內容
        // 顯示算盤的內容
        mvwprintw(win, 1, 1, "  5  4  3  2  1");
        mvwaddch(win, 2, 0, ACS_LTEE);
        mvwaddch(win, 2, 18, ACS_RTEE);
        for (uint8_t pos = 1; pos < 18; pos += 1)
            mvwaddch(win, 2, pos, ACS_HLINE), mvwaddch(win, 5, pos, ACS_HLINE);
        // 顯示算盤的數值
        wattron(win, COLOR_PAIR(1));
        for (uint8_t pos = 3; pos < 18; pos += 3)
            mvwaddch(win, 2, pos, ACS_TTEE);
        for (uint8_t pos = 3; pos < 18; pos += 3)
            mvwaddch(win, 3, pos, ACS_DIAMOND);
        for (uint8_t pos = 3; pos < 18; pos += 3) {
            mvwaddch(win, 4, pos, ACS_VLINE);
            mvwaddch(win, 5, pos, ACS_PLUS);
            mvwaddch(win, 6, pos, ACS_VLINE);
        }
        for (uint8_t line = 7; line < 11; line += 1)
            for (uint8_t pos = 3; pos < 18; pos += 3)
                mvwaddch(win, line, pos, ACS_DIAMOND);
        for (uint8_t pos = 3; pos < 18; pos += 3)
            mvwaddch(win, 11, pos, ACS_BTEE);
        // 依據內容改變數值
        // TODO：這裡要改成依據 num 的值來改變算盤的內容
        // 待開發內容
        refresh();
        wrefresh(win);
        // 輸入操作
        cmd = getch();
        // 判斷輸入的命令
        if (cmd == 'q') break;  // 如果是 q 則跳出迴圈
    }
    // 結束操作
    endwin();
    return EXIT_SUCCESS;
}
