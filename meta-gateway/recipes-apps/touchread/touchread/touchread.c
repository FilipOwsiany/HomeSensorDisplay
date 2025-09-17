#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>   // strcasestr()
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <errno.h>

#ifndef BITS_PER_LONG
#define BITS_PER_LONG (sizeof(unsigned long) * 8)
#endif
#define NBITS(x)       (((x) + BITS_PER_LONG - 1) / BITS_PER_LONG)
#define BIT(x)         (1UL << ((x) % BITS_PER_LONG))
#define OFF(x)         ((x) / BITS_PER_LONG)

static int open_touch_evdev(char *chosen, size_t chosen_sz) {
    const char *by_paths[] = { "/dev/input/by-path", "/dev/input" };
    for (int d = 0; d < 2; ++d) {
        DIR *dir = opendir(by_paths[d]);
        if (!dir) continue;
        struct dirent *de;
        while ((de = readdir(dir))) {
            if (strncmp(de->d_name, "event", 5) != 0 &&
                strstr(de->d_name, "event") == NULL) continue;

            char path[256];
            snprintf(path, sizeof(path), "%s/%s", by_paths[d], de->d_name);

            // resolve link by-path
            char real[256];
            ssize_t n = readlink(path, real, sizeof(real)-1);
            if (n > 0) {
                real[n] = 0;
                const char *base = strrchr(real, '/');
                snprintf(path, sizeof(path), "/dev/input/%s", base ? base+1 : real);
            }

            int fd = open(path, O_RDONLY|O_NONBLOCK);
            if (fd < 0) continue;

            char name[128] = {0};
            ioctl(fd, EVIOCGNAME(sizeof(name)-1), name);

            // check which event types are supported
            unsigned long evbits[NBITS(EV_MAX+1)];
            memset(evbits, 0, sizeof(evbits));
            ioctl(fd, EVIOCGBIT(0, sizeof(evbits)), evbits);

            int has_abs = (evbits[OFF(EV_ABS)] & BIT(EV_ABS)) != 0;
            int looks_like_touch = has_abs && (
                strcasestr(name, "touch") || strcasestr(name, "ts") || strcasestr(name, "goodix")
            );

            if (looks_like_touch) {
                strncpy(chosen, path, chosen_sz);
                close(fd);
                closedir(dir);
                return 0;
            }
            close(fd);
        }
        closedir(dir);
    }
    return -1;
}

static void get_fb_size(const char *fbdev, int *w, int *h) {
    *w = *h = 0;
    int fb = open(fbdev, O_RDONLY);
    if (fb >= 0) {
        struct fb_var_screeninfo vinfo;
        if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo) == 0) {
            *w = vinfo.xres; *h = vinfo.yres;
        }
        close(fb);
    }
}

int main(int argc, char **argv) {
    const char *ev = (argc > 1) ? argv[1] : NULL;    // e.g. /dev/input/event5
    const char *fb = (argc > 2) ? argv[2] : "/dev/fb0";
    char chosen[256];

    if (!ev) {
        if (open_touch_evdev(chosen, sizeof(chosen)) == 0) ev = chosen;
        else { fprintf(stderr, "No touchscreen device found.\n"); return 2; }
    }

    int w=0, h=0;
    get_fb_size(fb, &w, &h);
    if (w && h) printf("[touchread] Screen: %dx%d (from %s)\n", w, h, fb);

    int fd = open(ev, O_RDONLY);
    if (fd < 0) { perror("open evdev"); return 1; }

    printf("[touchread] Reading from: %s\n", ev);

    int slot = 0, tracking_id[20]; memset(tracking_id, -1, sizeof(tracking_id));
    int mt_x[20]={0}, mt_y[20]={0};
    int cur_x=0, cur_y=0, touching=0;

    struct input_absinfo ax, ay, amtx, amty;
    int have_abs  = (ioctl(fd, EVIOCGABS(ABS_X), &ax) == 0) && (ioctl(fd, EVIOCGABS(ABS_Y), &ay) == 0);
    int have_mt   = (ioctl(fd, EVIOCGABS(ABS_MT_POSITION_X), &amtx) == 0) && (ioctl(fd, EVIOCGABS(ABS_MT_POSITION_Y), &amty) == 0);

    printf("[touchread] ABS ranges: X[%d..%d] Y[%d..%d], MTX[%d..%d] MTY[%d..%d]\n",
        have_abs?ax.minimum:0, have_abs?ax.maximum:0,
        have_abs?ay.minimum:0, have_abs?ay.maximum:0,
        have_mt?amtx.minimum:0, have_mt?amtx.maximum:0,
        have_mt?amty.minimum:0, have_mt?amty.maximum:0);

    struct input_event evbuf;
    while (1) {
        ssize_t r = read(fd, &evbuf, sizeof(evbuf));
        if (r < 0) {
            if (errno == EAGAIN) { usleep(1000); continue; }
            perror("read"); break;
        }
        if (r != sizeof(evbuf)) continue;

        if (evbuf.type == EV_ABS) {
            switch (evbuf.code) {
                case ABS_X: cur_x = evbuf.value; break;
                case ABS_Y: cur_y = evbuf.value; break;
                case ABS_MT_SLOT:         slot = evbuf.value; break;
                case ABS_MT_TRACKING_ID:
                    if (slot >= 0 && slot < (int)(sizeof(tracking_id)/sizeof(tracking_id[0]))) {
                        tracking_id[slot] = evbuf.value;
                        if (evbuf.value < 0) printf("UP   slot=%d\n", slot);
                        else                 printf("DOWN slot=%d id=%d\n", slot, evbuf.value);
                    }
                    break;
                case ABS_MT_POSITION_X:
                    if (slot >= 0 && slot < (int)(sizeof(mt_x)/sizeof(mt_x[0]))) mt_x[slot] = evbuf.value;
                    break;
                case ABS_MT_POSITION_Y:
                    if (slot >= 0 && slot < (int)(sizeof(mt_y)/sizeof(mt_y[0]))) mt_y[slot] = evbuf.value;
                    break;
            }
        } else if (evbuf.type == EV_KEY && evbuf.code == BTN_TOUCH) {
            touching = evbuf.value;
        } else if (evbuf.type == EV_SYN && evbuf.code == SYN_REPORT) {
            if (have_mt) {
                for (int s=0; s< (int)(sizeof(mt_x)/sizeof(mt_x[0])); ++s) {
                    if (tracking_id[s] >= 0) {
                        int x = mt_x[s], y = mt_y[s];
                        if (w && h && amtx.maximum>amtx.minimum && amty.maximum>amty.minimum) {
                            int X = (int)((long long)(x - amtx.minimum) * w / (amtx.maximum - amtx.minimum));
                            int Y = (int)((long long)(y - amty.minimum) * h / (amty.maximum - amty.minimum));
                            printf("MOVE slot=%d id=%d raw=(%d,%d) px=(%d,%d)\n", s, tracking_id[s], x,y, X,Y);
                        } else {
                            printf("MOVE slot=%d id=%d raw=(%d,%d)\n", s, tracking_id[s], x,y);
                        }
                    }
                }
            } else {
                if (have_abs && w && h && ax.maximum>ax.minimum && ay.maximum>ay.minimum) {
                    int X = (int)((long long)(cur_x - ax.minimum) * w / (ax.maximum - ax.minimum));
                    int Y = (int)((long long)(cur_y - ay.minimum) * h / (ay.maximum - ay.minimum));
                    printf("%s raw=(%d,%d) px=(%d,%d)\n", touching?"TOUCH":"NO-TOUCH", cur_x,cur_y, X,Y);
                } else {
                    printf("%s raw=(%d,%d)\n", touching?"TOUCH":"NO-TOUCH", cur_x,cur_y);
                }
            }
        }
    }

    close(fd);
    return 0;
}
