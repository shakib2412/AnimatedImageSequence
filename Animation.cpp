#include <graphics.h>
#include <conio.h>
#include <string>
#include <vector>
#pragma comment(lib, "Winmm.lib")
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4996)

const int WIDTH = 800;  // Increased window width
const int HEIGHT = 600; // Increased window height

// Function to load transparent PNG
void drawAlphaImage(int x, int y, IMAGE* image)
{
    DWORD* dst = GetImageBuffer();
    DWORD* src = GetImageBuffer(image);
    int image_width = image->getwidth();
    int image_height = image->getheight();
    for (int iy = 0; iy < image_height; iy++)
    {
        for (int ix = 0; ix < image_width; ix++)
        {
            int srcX = ix + iy * image_width;
            int dstX = (x + ix) + (y + iy) * WIDTH;
            if (src[srcX] >> 24)
            {
                dst[dstX] = src[srcX];
            }
        }
    }
}

int main() {
    initgraph(WIDTH, HEIGHT);
    setbkmode(TRANSPARENT);

    // Load the background image
    IMAGE im_bk;
    loadimage(&im_bk, _T("background.png"), WIDTH, HEIGHT);

    // Load the series of images (girl images)
    std::vector<IMAGE> img(7);
    for (int i = 0; i < 7; i++) {
        TCHAR filename[20];
        _stprintf(filename, _T("girl%d.png"), i);
        loadimage(&img[i], filename);
    }

    // Initialize and play background music
    mciSendString(_T("open game_music.mp3 alias bkmusic"), NULL, 0, NULL);
    mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);

    int i = 0;
    int x = WIDTH / 2 - img[0].getwidth() / 2;  // Center horizontally
    int y = HEIGHT / 2 - img[0].getheight() / 2;  // Center vertically

    BeginBatchDraw();  // Start batch drawing

    while (true) {
        // Display the background image
        putimage(0, 0, &im_bk);

        // Display the current girl image on top of the background
        drawAlphaImage(x, y, &img[i]);

        FlushBatchDraw();  // Flush the batch drawing

        Sleep(100);  // Decreased delay for smoother animation
        i = (i + 1) % 7;  // Loop back to the first image using modulo

        // Check for key press to exit
        if (_kbhit()) {
            break;
        }
    }

    EndBatchDraw();  // End batch drawing

    // Stop the music and close the graph
    mciSendString(_T("close bkmusic"), NULL, 0, NULL);
    closegraph();
    return 0;
}