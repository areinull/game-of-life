#include <ncurses.h>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>
#include <time.h>

namespace {
    std::atomic_bool quit(false);
    int row, col;
    const char DEAD = ' ';
    const char ALIVE = 'X';
}

void loop()
{
    unsigned epoch = 0;
    std::vector<char> field1((row-1)*col, DEAD), field2((row-1)*col, DEAD);

    // init field
    auto fill_rand = [&] {
        for (auto &c: field1) {
            c = rand() % 5 ? DEAD : ALIVE;
        }
    };

    auto fill_rand_rect = [&](int rs, int re, int cs, int ce, int dens) {
        for (int r = rs; r < re; ++r)
            for (int c = cs; c < ce; ++c)
                if (rand() % dens == 0)
                    field1[r * col + c] = ALIVE;
    };

    auto GosperGliderGun = [&]() {
        field1[10*col+2] = field1[10*col+3] = field1[11*col+2] = field1[11*col+3] = ALIVE;
        field1[10*col+12] = field1[11*col+12] = field1[12*col+12] =
        field1[9*col+13] = field1[13*col+13] =
        field1[8*col+14] = field1[14*col+14] =
        field1[8*col+15] = field1[14*col+15] =
        field1[11*col+16] =
        field1[9*col+17] = field1[13*col+17] =
        field1[10*col+18] = field1[11*col+18] = field1[12*col+18] =
        field1[11*col+19] = ALIVE;
        field1[8*col+22] = field1[9*col+22] = field1[10*col+22] =
        field1[8*col+23] = field1[9*col+23] = field1[10*col+23] =
        field1[7*col+24] = field1[11*col+24] =
        field1[7*col+26] = field1[6*col+26] = field1[11*col+26] = field1[12*col+26] = ALIVE;
        field1[8*col+36] = field1[8*col+37] = field1[9*col+36] = field1[9*col+37] = ALIVE;
    };

    GosperGliderGun();

//    fill_rand();

//    fill_rand_rect(row/2 - 5, row/2 +5, col/2 - 5, col/2 + 5, 3);

//    field1[2*col+5] = field1[3*col+5] = field1[4*col+5] = field1[4*col+4] = field1[3*col+3] = ALIVE;

//    field1[18*col+20] = field1[18*col+22] = field1[17*col+22] = field1[16*col+24] = field1[15*col+24] = field1[14*col+24] = field1[14*col+26] = field1[14*col+27] = field1[13*col+26] = field1[15*col+26] = ALIVE;

//    field1[20*col+20] = field1[21*col+20] = field1[21*col+21] = field1[21*col+19] = field1[22*col+19] = ALIVE;

//    field1[25*col+50] = field1[26*col+50] = field1[26*col+51] = field1[30*col+51] = field1[31*col+51] = field1[32*col+51] = field1[31*col+49] = ALIVE;

//    field1[25*col+50] = field1[26*col+50] = field1[26*col+48] = field1[28*col+49] = field1[29*col+50] = field1[30*col+50] = field1[31*col+50] = ALIVE;

//    field1[25*col+50] = field1[25*col+51] = field1[25*col+52] =
//    field1[26*col+51] = field1[26*col+52] =
//    field1[27*col+50] = field1[27*col+51] = field1[27*col+52] = ALIVE;

    field2 = field1;

    while (!quit.load(std::memory_order_acquire))
    {
        /*if (epoch == 2000) {
            fill_rand();
            field2 = field1;
            epoch = 0;
        }*/

        // draw field
        clear();
        mvprintw(0, 0, "Epoch: %u", epoch);
        for (int r=0; r<row-1; ++r)
        {
            move(r+1, 0);
            for (int c=0; c<col; ++c)
            {
                addch(field1[r*col+c]);
            }
        }
        refresh();

        // evaluate state
        for (int cur = 0; cur < field1.size(); ++cur)
        {
            unsigned acnt = 0;
            int nrow, ncol;
            nrow = cur / col - 1; ncol = cur % col - 1;
            if (nrow >= 0 && ncol >= 0 && field2[nrow*col + ncol] == ALIVE) ++acnt;
            nrow = cur / col - 1; ncol = cur % col;
            if (nrow >= 0 && field2[nrow*col + ncol] == ALIVE) ++acnt;
            nrow = cur / col - 1; ncol = cur % col + 1;
            if (nrow >= 0 && ncol < col && field2[nrow*col + ncol] == ALIVE) ++acnt;
            nrow = cur / col; ncol = cur % col - 1;
            if (ncol >= 0 && field2[nrow*col + ncol] == ALIVE) ++acnt;
            nrow = cur / col; ncol = cur % col + 1;
            if (ncol < col && field2[nrow*col + ncol] == ALIVE) ++acnt;
            nrow = cur / col + 1; ncol = cur % col - 1;
            if (nrow < field2.size()/col && ncol >= 0 && field2[nrow*col + ncol] == ALIVE) ++acnt;
            nrow = cur / col + 1; ncol = cur % col;
            if (nrow < field2.size()/col && field2[nrow*col + ncol] == ALIVE) ++acnt;
            nrow = cur / col + 1; ncol = cur % col + 1;
            if (nrow < field2.size()/col && ncol < col && field2[nrow*col + ncol] == ALIVE) ++acnt;

            if (field1[cur] == ALIVE && (acnt < 2 || acnt > 3))
            {
                field1[cur] = DEAD;
            }
            else if (field1[cur] == DEAD && acnt == 3)
            {
                field1[cur] = ALIVE;
            }
        }

        // generate some
        /*if (epoch % 50 == 0) {
            int rc = rand()%(row-10)+5;
            int cc = rand()%(col-10)+5;
            fill_rand_rect(rc - 5, rc + 5, cc - 5, cc + 5, 3);
        }*/

        field2 = field1;
        std::this_thread::sleep_for(epoch++? std::chrono::milliseconds(100): std::chrono::seconds(3));
    }
}

int main()
{
    srand(time(NULL));
    initscr();
    getmaxyx(stdscr,row,col);
    attron(A_BOLD);
    curs_set(0);

    std::thread worker(loop);

    getch();

    quit.store(true, std::memory_order_release);
    worker.join();

    endwin();
    return 0;
}
