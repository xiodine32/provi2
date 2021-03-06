//DRAWER

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <algorithm>

using namespace std;

int SCREEN_REAL_WIDTH = 800;
int SCREEN_REAL_HEIGHT = 600;

Uint32 flags = 0;


struct team {
    char nume[40];
    int punctaj;
    int corecte;
    int problema_bonus;
    int raspunsuri[101];
};
struct problema {
    int raspuns;
    int puncte;
    int bonus;
    bool answered[101];

};
team echipa[101], echipa_last[101];
problema probl[101], probl_last[101];
int echipe, probleme;

#ifdef DEBUG
void d(const char *x){
    printf("[DEBUG]%s\n",x);
}
#else

void d(const char *) { }

#endif
int client;

void drawText(int x, int y, double size, double r, double g, double b, double a, const char *X, ...);

char latestData[16384];
int SCREEN_WIDTH, SCREEN_HEIGHT;
bool running = true, key[512], keyl[512];
int screen = 0;
int time_left = 0;
bool sorting = false;

void draw2D(int x, int y, int w, int h, double r, double g, double b, double a, bool hollow = false) {
    glDisable(GL_TEXTURE_2D);
    glColor4d(r, g, b, a);
    if (!hollow)
        glBegin(GL_QUADS);
    else
        glBegin(GL_LINE_LOOP);
    glVertex2i(x, y);
    glVertex2i(x, y + h);
    glVertex2i(x + w, y + h);
    glVertex2i(x + w, y);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

void teamscore_u() {

}

bool sortare(team a, team b) {
    return a.punctaj > b.punctaj;
}

void ordonare() {
    if (sorting)
        sort(echipa + 1, echipa + echipe + 1, sortare);

}

void teamscore_d() {
    const int RS = 32;
    int max = -1;
    for (int i = 1; i <= echipe; i++) {
        if (echipa[i].punctaj > max) {
            max = echipa[i].punctaj;
        }
    }
    for (int i = 1; i <= echipe; i++) {
        drawText(SCREEN_WIDTH - 32, 130 + RS * (i - 1), 16, 1, 1, 1, 1, "%.2d", i);
        int drawedwidth = int(echipa[i].punctaj / double(max) * (SCREEN_WIDTH - 400));
        drawText((int) (300 - (strlen(echipa[i].nume) * 16 * 5) / 6), 130 + RS * (i - 1), 16, 1, 1, 1, 1,
                 echipa[i].nume, i);
        if (max == echipa[i].punctaj) {
            draw2D(300, 130 + RS * (i - 1), drawedwidth, 16, 0, 1, 0, 1);
        }
        else
            draw2D(300, 130 + RS * (i - 1), drawedwidth, 16, 1, 0, 0, 1);
        drawText(SCREEN_WIDTH - 100, 130 + RS * (i - 1), 16, 1, 1, 1, 1, "%.3d", echipa[i].punctaj);
    }
}

void problemscore_u() {
}

void problemscore_d() {
    int max = -1;
    for (int i = 1; i <= probleme; i++) {
        if (probl[i].puncte > max) {
            max = probl[i].puncte;
        }
    }
    for (int i = 1; i <= probleme; i++) {
        int drawedwidth = int(probl[i].puncte / double(max) * (SCREEN_WIDTH - 200));
        drawText(0, 130 + 32 * (i - 1), 16, 1, 1, 1, 1, "prob %.2d", i);
        if (probl[i].bonus == 20)
            draw2D(100, 130 + 32 * (i - 1), drawedwidth, 16, 0, 1, 0, 1);
        else
            draw2D(100, 130 + 32 * (i - 1), drawedwidth, 16, 1, 0, 0, 1);
        drawText(SCREEN_WIDTH - 100, 130 + 32 * (i - 1), 16, 1, 1, 1, 1, "%.2d", probl[i].puncte);
    }
}

void tablescreen_u() {
}

void tablescreen_d() {
    const int RS = 32;
    for (int j = 1; j <= probleme; j++) {
        for (int i = 1; i <= echipe; i++) {
            drawText((int) (300 - (strlen(echipa[i].nume) * 32 * 5) / 12), 150 + (i - 1) * RS + 8, 16, 1, 1, 1, 1,
                     echipa[i].nume);
            if (echipa[i].raspunsuri[j] == 0) {
                double dR = 0.5, dG = 0.5, dB = 0.5; //color drawing
                if (i % 2 == 0) {
                    dB += 0.2;
                } else
                    dR += 0.2;
                draw2D(300 + (j - 1) * RS, 150 + (i - 1) * RS, RS, RS, dR, dG, dB, 1);
            }
            else {
//				printf("ECHIPA %d PROBLEMA %d :%d\n",i,j,(echipa[i].raspunsuri[j]==0));
                if (probl[j].answered[i])
                    draw2D(300 + (j - 1) * RS, 150 + (i - 1) * RS, RS, RS, 0, 1, 0, 1);
                else
                    draw2D(300 + (j - 1) * RS, 150 + (i - 1) * RS, RS, RS, 1, 0.25, 0.25, 1);
            }

            drawText(300 + (j - 1) * RS + 2 + RS / 3, 150 + (i - 1) * RS + 8 + 2, 16, 0, 0, 0, 1, "%d",
                     echipa[i].raspunsuri[j]);
            drawText(300 + (j - 1) * RS + RS / 3, 150 + (i - 1) * RS + 8, 16, 1, 1, 1, 1, "%d",
                     echipa[i].raspunsuri[j]);
            draw2D(300 + (j - 1) * RS, 150 + (i - 1) * RS, RS, RS, 0, 0, 0, 1, true);
        }
        drawText(300 + (j - 1) * RS, 134, 16, 1, 1, 1, 1, "%.2d", j);
    }
    for (int j = 1; j <= probleme; j++)
        for (int i = 1; i <= echipe; i++)
            if (echipa[i].problema_bonus == j) {
                draw2D(300 + (j - 1) * RS, 150 + (i - 1) * RS, RS, RS, 1, 1, 1, 1, true);
            }

}

void waitscreen_u() {

}

void waitscreen_d() {
    drawText((int) (SCREEN_WIDTH / 2 - strlen("Pregatiti-va!") * 32 + 64), SCREEN_HEIGHT / 2 - 32, 64, 1, 1, 1, 1,
             "Pregatiti-va!");
}

void process_data() {
    char pd[16384];
    memcpy(pd, latestData, sizeof(latestData));

    if (strcmp(pd, "ERROR") == 0)return;
    d("processing data");
    char *mover;
    mover = strtok(pd, "|");
    time_left = atoi(mover);
    mover = strtok(NULL, "|");
    echipe = atoi(mover);
    mover = strtok(NULL, "|");
    probleme = atoi(mover);
    for (int i = 1; i <= echipe; i++) {
        mover = strtok(NULL, "|");
        strcpy(echipa[i].nume, mover);
        mover = strtok(NULL, "|");
        echipa[i].punctaj = atoi(mover);
        mover = strtok(NULL, "|");
        echipa[i].corecte = atoi(mover);
        mover = strtok(NULL, "|");
        echipa[i].problema_bonus = atoi(mover);
        for (int j = 1; j <= probleme; j++) {
            mover = strtok(NULL, "|");
            echipa[i].raspunsuri[j] = atoi(mover);
        }

    }
    for (int i = 1; i <= probleme; i++) {
        mover = strtok(NULL, "|");
        probl[i].puncte = atoi(mover);
        mover = strtok(NULL, "|");
        probl[i].raspuns = atoi(mover);
        mover = strtok(NULL, "|");
        probl[i].bonus = atoi(mover);
        for (int j = 1; j <= echipe; j++) {
            mover = strtok(NULL, "|");
            probl[i].answered[j] = (bool) atoi(mover);
        }
    }
    memcpy(echipa_last, echipa, sizeof(echipa));
    memcpy(probl_last, probl, sizeof(probl));
    ordonare();
}

#pragma region Data writing

struct {
    double sx, sy, ex, ey;
} chars[512];

void generate_font() {
    char text[] = {
            "!\"#$%&'()*\n+,-./01234\n56789:;<=>\n?@ABCDEFGH\nIJKLMNOPQR\nSTUVWXYZ[\\\n]^_`abcdef\nghijklmnop\nqrstuvwxyz\n{|}~"};
    double X, Y;
    X = Y = 0;
    const double inc = 50;
    for (int i = 0; text[i] != 0; i++) {
        if (text[i] == '\n') {
            X = 0;
            Y += inc / 512.0;
            continue;
        }
        chars[text[i]].sx = X;
        chars[text[i]].sy = Y;
        chars[text[i]].ex = X + 40 / 512.0;
        chars[text[i]].ey = Y + 45 / 512.0;
        X += inc / 512.0;
    }
}

unsigned int textureID = (unsigned int) -1;

void load() {
    SDL_Surface *surface = SDL_LoadBMP("font_2.bmp");
    unsigned char *s;
    unsigned char *r = new unsigned char[512 * 512 * 4];
    long c = -1;
    s = (unsigned char *) surface->pixels;
    for (int i = 0; i < 512 * 512 * 3; i += 3) {
        unsigned char R = s[i];
        unsigned char G = s[i + 1];
        unsigned char B = s[i + 2];
        unsigned char A = 255;
        if (R == 255 && B == 255 && G == 0) {
            R = G = B = 0;
            A = 0;
        }
        if (R != 255 && B != 255 && G != 255) {
            R = B = G;
            //A=255-G;
        }
        r[++c] = R;
        r[++c] = G;
        r[++c] = B;
        r[++c] = A;
    }
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, r);
    delete r;
    SDL_FreeSurface(surface);
}

void drawText(int x, int y, double size, double r, double g, double b, double a, const char *X, ...) {
    va_list arg;
    va_start(arg, X);
    char text[16384] = {0};
    vsprintf(text, X, arg);
    glBindTexture(GL_TEXTURE_2D, textureID);
    int DX = x, DY = y;
    glColor4d(r, g, b, a);
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
    for (int i = 0; text[i] != 0; i++) {
        if (text[i] == ' ') {
            DX += (5 * size) / 6;
            continue;
        }
        glBegin(GL_QUADS);
        glTexCoord2d(chars[text[i]].sx, chars[text[i]].sy);
        glVertex2d(DX, DY);
        glTexCoord2d(chars[text[i]].sx, chars[text[i]].ey);
        glVertex2d(DX, DY + size);
        glTexCoord2d(chars[text[i]].ex, chars[text[i]].ey);
        glVertex2d(DX + size, DY + size);
        glTexCoord2d(chars[text[i]].ex, chars[text[i]].sy);
        glVertex2d(DX + size, DY);
        glEnd();
        DX += (5 * size) / 6;
    }
#pragma clang diagnostic pop


}

#pragma endregion


bool sigla = true;

void draw_timer() {
    if (screen == 0) return;
    if (time_left > 0)
        drawText(SCREEN_WIDTH / 2 - 5 * 64, 0, 128, 1, 1, 1, 1, "%.2dm%.2ds", time_left / 60, time_left % 60);
    else {
        drawText(SCREEN_WIDTH / 2 - 9 * 64, 0, 128, 1, 1, 1, 1, "felicitari!");
    }
}


void e() {
    SDL_Event tmp;
    while (SDL_PollEvent(&tmp)) {
        switch (tmp.type) {
            case SDL_QUIT:
                running = false;
                return;
            case SDL_KEYDOWN:
                key[tmp.key.keysym.sym] = true;
                return;
            case SDL_KEYUP:
                key[tmp.key.keysym.sym] = false;
                return;
            default:
                break;
        }
    }
}

void up() {
    if (strcmp(latestData, "ERROR") == 0)
        screen = 0;
    else if (screen == 0)
        screen = 1;

    if (!key[SDLK_f] && keyl[SDLK_f]) {
        flags ^= SDL_FULLSCREEN;
        SDL_SetVideoMode(SCREEN_REAL_WIDTH, SCREEN_REAL_HEIGHT, 0, flags);
    }
    if (!key[SDLK_l] && keyl[SDLK_l])
        sorting = !sorting;


    if (!key[SDLK_1] && keyl[SDLK_1])
        screen = 1;
    if (!key[SDLK_2] && keyl[SDLK_2])
        screen = 2;
    if (!key[SDLK_3] && keyl[SDLK_3])
        screen = 3;
    if (!key[SDLK_0] && keyl[SDLK_0])
        ordonare();
    if (key[SDLK_ESCAPE])
        running = false;
    switch (screen) {
        case 0:
            waitscreen_u();
            break;
        case 1:
            teamscore_u();
            break;
        case 2:
            problemscore_u();
            break;
        case 3:
            tablescreen_u();
            break;
        default:
            break;
    }

    process_data();
    memcpy(keyl, key, sizeof(key));
}

void dr() {
    glClear(GL_COLOR_BUFFER_BIT);
    draw_timer();
    switch (screen) {
        case 0:
            waitscreen_d();
            break;
        case 1:
            teamscore_d();
            break;
        case 2:
            problemscore_d();
            break;
        case 3:
            tablescreen_d();
            break;
        default:
            break;
    }
    SDL_GL_SwapBuffers();
}

void *thread(void *) {
    generate_font();
    SDL_Init(SDL_INIT_VIDEO);
    SCREEN_WIDTH = 1600;
    SCREEN_HEIGHT = 1200;
    flags = SDL_OPENGL;
    SDL_SetVideoMode(SCREEN_REAL_WIDTH, SCREEN_REAL_HEIGHT, 0, flags);

    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, SCREEN_REAL_WIDTH, SCREEN_REAL_HEIGHT);
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    load();

    int lTick = SDL_GetTicks();
    while (running) {
        e();
        while (SDL_GetTicks() - lTick > 1000 / 60) {
            up();
            lTick += 1000 / 60;
        }
        dr();
        SDL_Delay(13);
    }
    return 0;
}


int main(int a, char **argv) {
    char data[16384];
    char strIPADDR[128] = {0};
    printf("IP ADDRESS:");
    scanf("%s", strIPADDR);
    pthread_t pDrawThread;
    pthread_create(&pDrawThread, NULL, thread, NULL);

    d("host");
    struct sockaddr_in sin;

    memset(&sin, 0, sizeof sin);

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ((in_addr *) (gethostbyname(strIPADDR)->h_addr))->s_addr;
    sin.sin_port = htons(1030);

    client = socket(AF_INET, SOCK_STREAM, 0);


    if (connect(client, (sockaddr *) &sin, sizeof sin) == -1) {
        d("ERROR CONNECTING");
        return 1;
    }
    char thedata[16384] = "data";

    while (running) {
        if (send(client, thedata, sizeof(thedata), 0) <= 0) {
            d("ERR");
            running = false;
        };
        recv(client, data, sizeof(data), 0);
        //	d(data);
        memcpy(latestData, data, sizeof(data));
    }
    return 0;
}
