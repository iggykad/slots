#include <raylib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

// Your sounds
Sound credit_sound;
Sound jackpot;
Sound twenty_one;
Sound pause_sound;
Sound machine_spin;
Sound startup;

int credits = 50;
bool spinning = false;
std::vector<int> slotValues(3);
std::string message = "";

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Apple animation frames for pause screen
std::vector<Texture2D> appleFrames;
int currentFrame = 0;
float frameTimer = 0;
const float frameDelay = 0.066f;
bool Paused = false;
bool showNoCreditsMessage = false;

int CenterX(const char* text, int fontSize) {
    return (GetScreenWidth() - MeasureText(text, fontSize)) / 2;
}

void runSlot(int& credits)
{
    slotValues[0] = rand() % 22;
    slotValues[1] = rand() % 22;
    slotValues[2] = rand() % 22;

    message = "";

    if (slotValues[0] == slotValues[1] && slotValues[1] == slotValues[2]) {
        credits += 1000;
        message = "Jackpot! +1000 credits";
        PlaySound(jackpot);
    }

    for (int i = 0; i < 3; i++) {
        if (slotValues[i] == 21) {
            credits += 10;
            message = "21! +10 credits";
            PlaySound(twenty_one);
        }
        else if (slotValues[i] == 7) {
            credits += 75;
            message = "7! +100 credits";
            PlaySound(credit_sound);
        }
    }
}

//in the assets dir there's a folder with all the frames of the pause screen apple gif, void func loads them
void LoadAppleFrames() {
    appleFrames.clear();
    for (int i = 0; i <= 58; i += 2) {
        std::string path = TextFormat("assets/frame_%02d.png", i);
        if (FileExists(path.c_str())) {
            Image img = LoadImage(path.c_str());
            if (img.data != NULL) {
                Texture2D tex = LoadTextureFromImage(img);
                UnloadImage(img);  //textures get loaded, then this frees the data
                if (tex.id != 0) {
                    appleFrames.push_back(tex);
                }
            }
        }
        else {
            TraceLog(LOG_WARNING, "Missing frame: %s", path.c_str());
        }
    }
}

int main() {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Slot Machine");
    Image icon = LoadImage("assets/frame_00.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    InitAudioDevice();

    if (!IsAudioDeviceReady()) {
        return -1;
    }

    twenty_one = LoadSound("assets/twentyone.mp3");
    credit_sound = LoadSound("assets/credits.mp3");
    jackpot = LoadSound("assets/jackpot.mp3");
    startup = LoadSound("assets/ps3.mp3");
    if (FileExists("assets/pause.mp3")) {
        pause_sound = LoadSound("assets/pause.mp3");
    }
    machine_spin = LoadSound("assets/spin.mp3");

    LoadAppleFrames(); //loads the apple frames, prepares them to appear when the function is called

    SetTargetFPS(60);
    srand(static_cast<unsigned int>(time(0)));

    //loading screen where the april spins

    double loadingStart = GetTime();

    PlaySound(startup);

    while (GetTime() - loadingStart < 9.0)
    {
        frameTimer += GetFrameTime();
        if (frameTimer >= frameDelay) {
            frameTimer = 0;
            currentFrame = (currentFrame + 1) % appleFrames.size();
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);


        if (!appleFrames.empty()) {
            DrawTexture(appleFrames[currentFrame],
                (SCREEN_WIDTH - 250) / 2,
                (SCREEN_HEIGHT - 300) / 2,
                WHITE);
        }

        EndDrawing();
    }

    //======================================

    bool running = true;

    while (running && !WindowShouldClose())
    {
        // Handle pause toggle key
        if (IsKeyPressed(KEY_P)) {
            Paused = !Paused;
            if (Paused) {
                currentFrame = 0;    // reset animation frame
                frameTimer = 0;      // reset timer
                PlaySound(pause_sound);
            }
            else {
                StopSound(pause_sound);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Rules:", 20, 80, 20, WHITE);
        DrawText("Hit 21: +10", 20, 110, 18, WHITE);
        DrawText("Hit 7: +100", 20, 130, 18, WHITE);
        DrawText("3 of a kind: +1000", 20, 150, 18, WHITE);

        const char* creditText = TextFormat("Credits: %d", credits);
        DrawText(creditText, GetScreenWidth() - MeasureText(creditText, 24) - 20, 20, 24, DARKPURPLE);

        DrawText("Welcome to the slots!", CenterX("Welcome to the slots!", 30), 20, 30, PURPLE);

        int slotStartX = (SCREEN_WIDTH - 225) / 2; //slot layout here
        for (int i = 0; i < 3; i++) {
            DrawText(TextFormat("%d", slotValues[i]), slotStartX + (i * 100), 280, 40, GREEN);
        }

        if (!message.empty()) {
            DrawText(message.c_str(), CenterX(message.c_str(), 20), 400, 20, GOLD);
        }

        if (showNoCreditsMessage) {
            DrawText("No credits left.", CenterX("No credits left.", 24), 440, 24, RED);
        }

        DrawText("Press [SPACE] to Spin", CenterX("Press [SPACE] to Spin", 24), 500, 24, GRAY);
        DrawText("Press [P] to Pause", CenterX("Press [P] to Pause", 20), 550, 20, GRAY);

        if (Paused) {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GRAY);

            if (!appleFrames.empty()) {
                
                frameTimer += GetFrameTime();
                if (frameTimer >= frameDelay) {
                    frameTimer = 0;
                    currentFrame = (currentFrame + 1) % appleFrames.size();
                }

                DrawTexture(appleFrames[currentFrame],
                    (SCREEN_WIDTH - 250) / 2,
                    (SCREEN_HEIGHT - 300) / 2,
                    WHITE);
            }

            if (!IsSoundPlaying(pause_sound)) {
                PlaySound(pause_sound);
            }

            DrawText("PAUSED", CenterX("PAUSED", 50), 475, 50, RED);
            DrawText("q to exit", 335, 525, 20, WHITE);
            DrawText("p to unpause", 335, 550, 20, WHITE);

            if (IsKeyPressed(KEY_Q)) {
                running = false;  //exit main 
            }
        }

        EndDrawing();

        if (IsKeyPressed(KEY_SPACE) && !Paused) { //if you spin when unpaused/while pause function is not called
            if (credits >= 5) {
                credits -= 5;
                PlaySound(machine_spin);
                runSlot(credits);
                showNoCreditsMessage = false;
            }
            else {
                showNoCreditsMessage = true;
            }
        }
    }

    for (auto& frame : appleFrames) {
        UnloadTexture(frame);
    }

    UnloadSound(credit_sound);
    UnloadSound(jackpot);
    UnloadSound(twenty_one);
    UnloadSound(pause_sound);
    UnloadSound(machine_spin);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
