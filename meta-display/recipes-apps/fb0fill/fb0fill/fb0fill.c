// fbfill.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <errno.h>

static int parse_named(const char *s, uint8_t *r, uint8_t *g, uint8_t *b) {
    struct { const char *name; uint8_t r,g,b; } map[] = {
        {"black",0,0,0}, {"white",255,255,255}, {"red",255,0,0},
        {"green",0,255,0}, {"blue",0,0,255}, {"yellow",255,255,0},
        {"magenta",255,0,255}, {"cyan",0,255,255}, {"grey",128,128,128},
        {"gray",128,128,128}, {"orange",255,165,0}, {"pink",255,192,203}
    };
    for (size_t i=0;i<sizeof(map)/sizeof(map[0]);++i)
        if (strcasecmp(s, map[i].name)==0) { *r=map[i].r; *g=map[i].g; *b=map[i].b; return 1; }
    return 0;
}

static int parse_rgb(const char *s, uint8_t *r, uint8_t *g, uint8_t *b) {
    if (s[0]=='#' && strlen(s)==7) {
        unsigned rr,gg,bb;
        if (sscanf(s+1, "%02x%02x%02x", &rr,&gg,&bb)==3) { *r=rr; *g=gg; *b=bb; return 1; }
        return 0;
    }
    unsigned rr,gg,bb;
    if (sscanf(s, "%u,%u,%u", &rr,&gg,&bb)==3 && rr<=255 && gg<=255 && bb<=255) {
        *r=rr; *g=gg; *b=bb; return 1;
    }
    return 0;
}

static inline uint32_t scale_to_bits(uint8_t v, unsigned length) {
    if (length==0) return 0;
    if (length>=8) return v;
    return (uint32_t)((v * ((1u<<length)-1) + 127) / 255);
}

static uint32_t pack_pixel(const struct fb_var_screeninfo *vinfo, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t pr = scale_to_bits(r, vinfo->red.length)   << vinfo->red.offset;
    uint32_t pg = scale_to_bits(g, vinfo->green.length) << vinfo->green.offset;
    uint32_t pb = scale_to_bits(b, vinfo->blue.length)  << vinfo->blue.offset;
    uint32_t pa = 0;
    if (vinfo->transp.length) {
        uint32_t amax = (1u<<vinfo->transp.length)-1u;
        pa = amax << vinfo->transp.offset;
    }
    return pr | pg | pb | pa;
}

int main(int argc, char **argv) {
    const char *color = (argc > 1) ? argv[1] : "black";
    const char *fbdev = (argc > 2) ? argv[2] : "/dev/fb0";

    uint8_t r=0,g=0,b=0;
    if (!parse_named(color, &r,&g,&b) && !parse_rgb(color, &r,&g,&b)) {
        fprintf(stderr, "Usage: %s <color> [fbdev]\n", argv[0]);
        fprintf(stderr, "Color: name (red/green/blue/...) or #RRGGBB or R,G,B\n");
        return 2;
    }

    printf("[fbfill] Opening framebuffer: %s\n", fbdev);
    int fb = open(fbdev, O_RDWR);
    if (fb < 0) { perror("open fb"); return 1; }

    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    if (ioctl(fb, FBIOGET_FSCREENINFO, &finfo) < 0) { perror("FBIOGET_FSCREENINFO"); close(fb); return 1; }
    if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo) < 0) { perror("FBIOGET_VSCREENINFO"); close(fb); return 1; }

    printf("[fbfill] Resolution: %ux%u, bpp=%u\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
    printf("[fbfill] line_length=%u, smem_len=%u\n", finfo.line_length, finfo.smem_len);
    printf("[fbfill] Components: R(off=%u,len=%u) G(off=%u,len=%u) B(off=%u,len=%u) T(off=%u,len=%u)\n",
           vinfo.red.offset,vinfo.red.length,
           vinfo.green.offset,vinfo.green.length,
           vinfo.blue.offset,vinfo.blue.length,
           vinfo.transp.offset,vinfo.transp.length);

    size_t screensize = finfo.line_length * vinfo.yres;
    printf("[fbfill] Mapping memory (%zu bytes)\n", screensize);
    void *fbmem = mmap(NULL, screensize, PROT_READ|PROT_WRITE, MAP_SHARED, fb, 0);
    if (fbmem == MAP_FAILED) { perror("mmap"); close(fb); return 1; }

    uint32_t pixel = pack_pixel(&vinfo, r,g,b);
    printf("[fbfill] Input color: R=%u G=%u B=%u -> pixel=0x%08X\n", r,g,b,pixel);

    if (vinfo.bits_per_pixel == 32) {
        printf("[fbfill] Filling 32bpp...\n");
        uint32_t *p = (uint32_t*)fbmem;
        size_t pxcount = (size_t)vinfo.yres * (finfo.line_length / 4);
        for (size_t i=0; i<pxcount; ++i) p[i] = pixel;
    } else if (vinfo.bits_per_pixel == 16) {
        printf("[fbfill] Filling 16bpp...\n");
        uint16_t *p = (uint16_t*)fbmem;
        size_t pxcount = (size_t)vinfo.yres * (finfo.line_length / 2);
        uint16_t pix16 = (uint16_t)pixel;
        for (size_t i=0; i<pxcount; ++i) p[i] = pix16;
    } else {
        printf("[fbfill] Filling generically bpp=%u...\n", vinfo.bits_per_pixel);
        uint8_t *base = (uint8_t*)fbmem;
        unsigned bytespp = vinfo.bits_per_pixel / 8;
        for (uint32_t y=0; y<vinfo.yres; ++y) {
            uint8_t *row = base + y * finfo.line_length;
            for (uint32_t x=0; x<vinfo.xres; ++x) {
                for (unsigned k=0; k<bytespp; ++k) row[x*bytespp + k] = (pixel >> (8*k)) & 0xFF;
            }
        }
    }

    printf("[fbfill] Done! The screen should now be filled.\n");

    munmap(fbmem, screensize);
    close(fb);
    return 0;
}
