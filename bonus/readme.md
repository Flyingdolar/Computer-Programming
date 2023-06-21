# Ncurse 中文操作說明

> 程式設計期末加分題
> 40768033T 設計四 鄭翔元
>
> 大綱
>
> 1. 如何將 Ncurse 裝到你的電腦上
> 2. Ncurse 簡介與使用方式
> 3. Ncurse 在程式碼當中的使用
> 4. Ncurse 的視窗 (Windows) 介紹
> 5. Ncurse 的顏色 (Color) 介紹
> 6. 範例程式碼

## 一、如何將 Ncurse 裝到你的電腦上

### 官方網站下載安裝

#### 下載 Ncurse

1. 進入 [Ncurse 官網](https://invisible-island.net/ncurses/#downloads)，下載最新版本的 Ncurse

2, 使用 `tar -zxvf` 解壓縮

#### 安裝 Ncurse

1. 開啟終端機，進入解壓縮後的資料夾，使用 `./configure` 進行設定

2. 使用 `make` 進行編譯

3. 使用 `sudo make install` 進行安裝

### 使用套件管理工具安裝

#### 使用 Homebrew 安裝

```bash
brew install ncurses
```

#### 使用 apt 安裝

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

## 二、Ncurse 簡介與使用方式

Ncurse 是一個可以在終端機上使用的圖形化介面，可以讓使用者在終端機上使用滑鼠、鍵盤等輸入裝置來操作程式，並且可以在終端機上顯示圖形介面。

### 在程式中引入 Ncurse

```c
#include <ncurses.h>
```

編譯的時候要加上 `-lncurses` 參數

```bash
gcc -o main main.c -lncurses
```

當然，除了 Dynamic Linking 的方式之外，你也可以嘗試 Static Linking 的方式，這樣就可以移植到其他電腦上執行了。

```bash
gcc -o main main.c libncurses.a
```

其中 `libncurses.a` 是 Ncurse 的靜態函式庫，可以在 Ncurse 的安裝目錄中找到。

## 三、Ncurse 在程式碼當中的使用

### 初始化 Ncurse 的操作

1. 在執行任何 ncurse 的函式之前，都要先初始化 Ncurse

    ```c
    initscr();
    ```

2. 在結束程式之前，要記得結束 Ncurse

    ```c
    endwin();
    ```

3. 使用 `cbreak()` 來關閉緩衝區，這樣就可以一次輸入一個字元了

    ```c
    cbreak();
    ```

4. 使用 `noecho()` 來關閉輸入的字元顯示，這樣就不會在輸入的時候顯示輸入的字元

    ```c
    noecho();
    ```

5. 使用 `keypad()` 來開啟鍵盤的功能，這樣就可以使用鍵盤上的特殊鍵了

    ```c
    keypad(stdscr, TRUE);
    ```

### Ncurse 的視窗 (Windows) 介紹

`Window *win` 是 Ncurse 的視窗，可以使用 `WINDOW *win` 來宣告一個視窗，並且使用 `newwin()` 來建立一個視窗。

```c
WINDOW *win;
win = newwin(10, 10, 0, 0);
```

其中帶入了四個參數，分別是 `LINES`、`COLS`、`y`、`x`，其中 `LINES` 和 `COLS` 是終端機的長寬，而 `y` 和 `x` 則是視窗的起始位置。

而 Windows 會長得像是一個 2D 的陣列，造型如下：

```c
                      The Screen <stdscr>
    [0,0]    +---------------------------------------+ [0, COLS-1]
             |                                       |
             |                                       |
             |       [0,0]               [0, COLS-1] |
             |       +---------------------------+   |
             |       |                           |   |
             |       |                           |   |
             |       |          Windows          |   |
             |       |                           |   |
             |       |                           |   |
             |       +---------------------------+   |
             |      [LINES-1,0]    [LINES-1, COLS-1] |
[LINES-1, 0] +---------------------------------------+ [LINES-1, COLS-1]
```

其中 y 座標通常在一些函數中都是放在前面，x 座標則是放在後面，例如 `mvwprintw(win, y, x, "Hello World");`

或者說另外一種比較好理解的方式就是， y 當成 row ， x 當成 col ( row 在前， col 在後 )

注意的是，在使用視窗的時候，要注意 x 和 y 的範圍，不要超過視窗的範圍，否則會出現錯誤。
如果不知道視窗的範圍，可以使用 `getmaxyx()` 來取得視窗的範圍。

```c
int y, x;
getmaxyx(win, y, x);
```

那一個頁面 (stdscr) 裏面可以有多個視窗嗎？

答案是可以的，但記得不能夠重疊，否則會出現錯誤。
另外，如果要在視窗中使用 Ncurse 的函式，要記得先使用 `wrefresh()` 來更新視窗，否則畫面不會更新。

```c
wrefresh(win);
refresh(); // 也可以使用 refresh() 來更新畫面，但是 refresh() 只能用在 stdscr 上
```

請記住，如果要結束視窗，要使用 `delwin()` 來刪除視窗

```c
delwin(win);
```

`stdscr` 是 Ncurse 的預設視窗，如果沒有特別指定視窗，則會使用該物件來當作視窗

> 所以當你的程式沒有需要使用到分割畫面的多個視窗的時候，就可以不用特別宣告視窗，直接使用 `stdscr` 來當作視窗

### Ncurse 的視窗操作

#### 1. 移動視窗的游標

```c
wmove(win, y, x);
move(y, x); // 也可以使用 move() 來移動游標，但是 move() 只能用在 stdscr 上
```

#### 2. 讀取使用者的輸入內容

```c
int ch;
ch = wgetch(win);
getch(); // 也可以使用 getch() 來讀取使用者的輸入內容，但是 getch() 只能用在 stdscr 上
```

為了方便起見，一般來說我們會再加上幾個特別的屬性：

1. `cbreak()`：關閉緩衝區，這樣就可以一次輸入一個字元了

2. `noecho()`：關閉輸入的字元顯示，這樣就不會在輸入的時候顯示輸入的字元

3. `keypad()`：開啟鍵盤的功能，這樣就可以使用鍵盤上的特殊鍵了

    特殊按鍵是包括像是上下左右、Home、End、PageUp、PageDown、Delete、Insert、F1~F12...etc.（詳細部分可以參考 `getch` 的 Manual 手冊說明）

    ```bash
    man getch
    ```

4. `nodelay()`：關閉輸入的等待，這樣就可以不用等待使用者輸入了

    主要是可以拿來寫像是遊戲上下左右移動的那種感覺...etc.

另外，輸入與移動鼠標是可以使用一個函式同時進行的，例如：

```c
int mvwgetch(WINDOW *win, int y, int x);
int mvgetch(int y, int x);  // 也可以使用 mvgetch() 來讀取使用者的輸入內容，但是 mvgetch() 只能用在 stdscr 上
```

使用 `mvgetch()` 時，會先移動游標到指定的位置，然後再讀取使用者的輸入內容

#### 3. 輸出東西至畫面上

```c
int ch = 'A';
char str[100] = "Hello World";
waddch(win, ch);    // 印出一個字元
waddstr(win, str);  // 印出一個字串
```

那關於輸出， ncurse 也有一些特別的屬性：

1. 換行：當輸出的字元超過視窗的範圍（橫向）時，會自動換行

2. 捲動：當輸出的字元超過視窗的範圍（直向）時，會自動捲動視窗；不過前提是你需要跑一段設定的程式碼

    ```c
    scrollok(win, TRUE);
    ```

    這樣就可以讓視窗自動捲動了

3. 字元的屬性其實是使用一種叫做 `chtype` 的型別，他是讀取 ASCII 碼的，所以如果要輸出特殊的字元，可以使用 ASCII 碼來輸出

    ```c
    waddch(win, 65); // 輸出 ASCII 碼 65 的字元
    ```

4. 更新方式：如果要讓輸出的字元立即顯示在視窗上，可以使用 `wrefresh()` 來更新視窗

    ```c
    wrefresh(win);
    ```

    但是如果你的程式有使用到 `nodelay()` 來關閉輸入的等待，那麼你就不需要使用 `wrefresh()` 來更新視窗了，因為 `nodelay()` 會自動更新視窗

    或者說，可以使用 `wechochar()` 來輸出字元，這樣就不需要使用 `wrefresh()` 來更新視窗了

    ```c
    wechochar(win, ch);
    // 其效力等同於
    waddch(win, ch), wrefresh(win);
    ```

#### 4. 畫面上文字的一些特別屬性

我們是可以在顯示畫面上的文字做一些特別的設定的，具體的使用方式如下：

```c
int waddch(WINDOW *win, const chtype ch | SOME_ATTRIBUTES | ANOTHER_ATTRIBUTES);
```

其中 `SOME_ATTRIBUTES` 與 `ANOTHER_ATTRIBUTES` 是一些特別的屬性，可以使用 `OR` 的方式來同時使用多個屬性

其實說穿了，就是這些函式在帶入你指定的字元之前，先進行 OR 的運算，然後再帶入函式中
故而，如果你要使用這些屬性，就必須要先將字元轉換成 `chtype` 型別，然後再帶入函式中

```c
int waddch(WINDOW *win, const chtype ch | A_BOLD | A_UNDERLINE);
```

以上面這個範例來說，就可以將字元設定成粗體與底線的樣式

1. 一些常見的屬性參數

    | 屬性名稱 | 屬性說明 |
    | -------- | -------- |
    | A_BOLD   | 粗體     |
    | A_DIM    | 暗淡     |
    | A_REVERSE| 反白     |
    | A_UNDERLINE | 底線   |
    | A_BLINK  | 閃爍     |
    | A_STANDOUT | 突顯   |
    | A_INVIS  | 隱藏     |
    | A_NORMAL | 正常     |
    | A_PROTECT | 保護    |
    | A_ALTCHARSET | 替代字元集 |
    | COLOR_PAIR(n) | 顏色對 |

2. 顏色的使用

    顏色的函式會同時定義 前景色（foreground）與 背景色（background），並且會使用 `COLOR_PAIR(n)` 來定義顏色對

    ```c
    int start_color(void);
    int init_pair(short pair, short f, short b);
    ```

    與其他屬性不同的地方是，顏色的屬性是需要先使用 `start_color()` 來啟用的，然後再使用 `init_pair()` 來定義顏色對（像上面示範的那樣）

    參數上，其中 `pair` 是顏色對的編號，`f` 是前景色，`b` 是背景色
    所以你可以設定很多組顏色對，然後在使用時，就可以使用 `COLOR_PAIR(n)` 來指定顏色對

    ```c
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    waddch(win, 'A' | COLOR_PAIR(1));
    ```

    關於顏色的部分，在 `ncurses.h` 中有定義一些顏色的常數，可以直接使用

    | 顏色名稱 | 顏色說明 |
    | -------- | -------- |
    | COLOR_BLACK | 黑色 |
    | COLOR_RED | 紅色 |
    | COLOR_GREEN | 綠色 |
    | COLOR_YELLOW | 黃色 |
    | COLOR_BLUE | 藍色 |
    | COLOR_MAGENTA | 洋紅 |
    | COLOR_CYAN | 青色 |
    | COLOR_WHITE | 白色 |

    那這些顏色是可以進行調整的，可以使用 `init_color()` 來進行調整

    ```c
    int init_color(short color, short r, short g, short b);
    ```

    其中 `color` 是顏色的編號，`r`、`g`、`b` 則是顏色的 RGB 值
    這樣就可以對顏色進行調整了

    如果今天情況相反，你想要知道現在這些被定義的顏色資訊，可以使用
        `color_content()` 與 `pair_content` 來取得

    ```c
    int color_content(short color, short *r, short *g, short *b);
    int pair_content(short pair, short *f, short *b);
    ```

    - `color` 是顏色的編號，`r`、`g`、`b` 則是顏色的 RGB 值
    - `pair` 是顏色對的編號，`f` 是前景色，`b` 是背景色

3. 圖像物件

    除了文字之外，我們也可以在畫面上顯示一些圖像物件，這些圖像物件是使用 `chtype` 來定義的，所以你可以使用 `chtype` 來定義你想要的圖像物件

    ```c
    chtype ch = ACS_ULCORNER;
    ```

    這樣就可以定義一個左上角的圖像物件了

    這些圖像物件的定義，都是以 `ACS_` 開頭的，所以你可以使用 `ACS_` 來查詢這些圖像物件的定義

    ```c
    int ch = ACS_ULCORNER;
    ```

    這樣就可以查詢到 `ACS_ULCORNER` 的定義了

    這些圖像物件的定義，都是以 `ACS_` 開頭的，所以你可以使用 `ACS_` 來查詢這些圖像物件的定義

    | 圖像物件名稱 | 圖像物件說明 |
    | ------------ | ------------ |
    | ACS_ULCORNER | 左上角       |
    | ACS_LLCORNER | 左下角       |
    | ACS_URCORNER | 右上角       |
    | ACS_LRCORNER | 右下角       |
    | ACS_LTEE     | 左 T 形      |
    | ACS_RTEE     | 右 T 形      |
    | ACS_BTEE     | 下 T 形      |
    | ACS_TTEE     | 上 T 形      |
    | ACS_HLINE    | 水平線       |
    | ACS_VLINE    | 垂直線       |
    | ACS_PLUS     | 十字線       |
    | ACS_S1       | 細線         |
    | ACS_S9       | 細線         |
    | ACS_DIAMOND  | 菱形         |

    不過想當然要這樣一個一個放進去，那就太麻煩了，所以 `ncurses` 也提供了一些函式來幫助我們處理這些圖像物件

    ```c
    int box(WINDOW *win, chtype verch, chtype horch);
    int hline(WINDOW *win, chtype ch, int n);
    int vline(WINDOW *win, chtype ch, int n);
    int border(WINDOW *win, chtype ls, chtype rs, chtype ts, chtype bs,
               chtype tl, chtype tr, chtype bl, chtype br);
    ```

    - `box()` 可以幫助我們畫出一個方框，其中 `verch` 是垂直線的圖像物件，`horch` 是水平線的圖像物件
    - `hline()` 可以幫助我們畫出一條水平線，其中 `ch` 是水平線的圖像物件，`n` 是水平線的長度
    - `vline()` 可以幫助我們畫出一條垂直線，其中 `ch` 是垂直線的圖像物件，`n` 是垂直線的長度
    - `border()` 可以幫助我們畫出一個方框，其中 `ls` 是左邊的垂直線的圖像物件，`rs` 是右邊的垂直線的圖像物件，`ts` 是上面的水平線的圖像物件，`bs` 是下面的水平線的圖像物件，`tl` 是左上角的圖像物件，`tr` 是右上角的圖像物件，`bl` 是左下角的圖像物件，`br` 是右下角的圖像物件

## 四、參考資料

- [ncurses 官網](https://invisible-island.net/ncurses/announce.html)
- [Ncurse Programming Guide](https://jbwyatt.com/ncurses.html#init)

## 五、程式碼範例

詳細部分請參考 [Example Code](ncurses.c)

有關於繪製一個算盤的小範例

如果不想要看 Code 的話，可以直接執行 `make` 來編譯並執行

```bash
make        # 編譯
make run    # 編譯並執行
make clean  # 清除編譯出來的檔案
```

---

> `2023.06.21` 撰寫於寫完卡坦島之後的彌留時間......（好想睡覺 QAQ）
