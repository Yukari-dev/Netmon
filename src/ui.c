#include "ui.h"
#include <string.h>
#define MAX_FEED_LINES 100
static char feed_lines[MAX_FEED_LINES][256];
static int feed_count = 0;

WINDOW *header_win;
WINDOW *feed_win;
WINDOW *stats_win;
WINDOW *footer_win;

void ui_init(const char* device, const char* filter){
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
    refresh();
    
    int ROWS, COLS;
    getmaxyx(stdscr, ROWS, COLS);

    header_win = newwin(3, COLS, 0, 0);

    feed_win   = newwin(ROWS-6, COLS/2, 3, 0);
    box(feed_win, 0, 0);

    stats_win  = newwin(ROWS-6, COLS/2, 3, COLS/2);

    footer_win = newwin(3, COLS, ROWS-3, 0);

    /*wborder(header_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wborder(feed_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wborder(stats_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wborder(footer_win, '|', '|', '-', '-', '+', '+', '+', '+'); */

    box(header_win, 0, 0);
    box(feed_win,   0, 0);
    box(stats_win,  0, 0);
    box(footer_win, 0, 0);

    wattr_on(header_win, A_BOLD, NULL);
    mvwprintw(header_win, 1, 2, "NETMON v1.0");
    mvwprintw(header_win, 1, (COLS/2) - strlen("INTERFACE:wlan0")/2, "INTERFACE:%s", device);
    mvwprintw(header_win, 1, COLS - strlen("FILTER:NONE") - 2, "FILTER:%s", strcmp(filter, "") == 0 ? "NONE" : filter);
    wattr_off(header_win, A_BOLD, NULL);

    mvwprintw(feed_win, 1, 2, "LIVE FEED");
    mvwprintw(stats_win, 1, 2, "TOP TALKERS");

    wrefresh(header_win);
    wrefresh(feed_win);
    wrefresh(stats_win);
    wrefresh(footer_win);
}

void ui_update_feed(const char* src, const char* dest, const char* protocol, int bytes){
    int rows, cols __attribute__((unused));
    getmaxyx(feed_win, rows, cols);
    int visible = rows - 3;

    snprintf(feed_lines[feed_count % MAX_FEED_LINES], 255,
        "%s -> %s [%s] %d bytes", src, dest, protocol, bytes);
    
    feed_count++;

    werase(feed_win);
    box(feed_win, 0, 0);
    mvwprintw(feed_win, 1, 2, "LIVE FEED");

    int start = feed_count > visible ? feed_count - visible : 0;
    for(int i = start; i < feed_count; i++){
        mvwprintw(feed_win, 2 + (i - start), 1, "%s", feed_lines[i % MAX_FEED_LINES]);
    }

    wrefresh(feed_win);
}

void ui_update_stats(StatsTable *stats){
    int rows, cols __attribute__((unused));
    getmaxyx(stats_win, rows, cols);

    werase(stats_win);
    box(stats_win, 0, 0);

    mvwprintw(stats_win, 1, 2, "TOP TALKERS");
    mvwprintw(stats_win, 2, 2, "%-18s %6s %10s", "IP", "PKTS", "BYTES");

    for(int i = 0; i < stats->count; i++){

        mvwprintw(stats_win, 3 + i, 2, "%-18s %6d %10ld",
            stats->entries[i].ip,
            stats->entries[i].packet_count,
            stats->entries[i].bytes_total
        );
    }

    mvwprintw(stats_win, rows-5, 2, "PROTOCOL BREAKDOWN");
    mvwprintw(stats_win, rows-4, 2, "TCP   %d", stats->tcp_count);
    mvwprintw(stats_win, rows-3, 2, "UDP   %d", stats->udp_count);
    mvwprintw(stats_win, rows-2, 2, "ICMP  %d", stats->icmp_count);

    wrefresh(stats_win);
}

void ui_update_footer(CaptureContext *ctx){
    int elapsed = time(NULL) - ctx->start_time;
    int mins = elapsed / 60;
    int secs = elapsed % 60;
    werase(footer_win);
    box(footer_win, 0, 0);
    mvwprintw(footer_win, 1, 2, "elapsed: %02d:%02d", mins, secs);
    wrefresh(footer_win);
}

void ui_cleanup(void){
    endwin();
}