#include <raylib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

int credits = 50;
bool spinning = false;
std::vector<int> slotValues(3);
std::string message = "";
Sound credit_sound;
Sound jackpot;
Sound twenty_one;
Sound pause_music;
Sound machine_spin;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int NUM_SLOTS = 3;
const int MARGIN = 20;
const int SLOT_SPACING = 100;
const int SLOT_WIDTH = 60;
const int TOTAL_SLOT_WIDTH = (NUM_SLOTS * SLOT_WIDTH) + ((NUM_SLOTS - 1) * SLOT_SPACING); //width of all slots + width of all spacings
const int START_X = (SCREEN_WIDTH - TOTAL_SLOT_WIDTH) / 2; //calculate x pos of the first slot so the entire row + their spacing is centered horizontally
const int SLOT_Y = 280;

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
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    for (int i = 0; i < 3; i++) {
        if (slotValues[i] == 21) {
            credits += 10;
            message = "21! +10 credits";
            PlaySound(twenty_one);
        }
        else if (slotValues[i] == 7) {
            credits += 75;
            message = "7! +75 credits";
            PlaySound(credit_sound);
        }
    }
}

bool Paused = false;
bool showNoCreditsMessage = false;
int CenterX(const char* text, int fontSize) {
    return (GetScreenWidth() - MeasureText(text, fontSize)) / 2;
}

int main()
{
    InitWindow(800, 600, "Slot Machine");
    InitAudioDevice();
    twenty_one = LoadSound("assets/twentyone.mp3");
    credit_sound = LoadSound("assets/credits.mp3");
    jackpot = LoadSound("assets/jackpot.mp3");
    pause_music = LoadSound("assets/pause.mp3");
    machine_spin = LoadSound("assets/spin.mp3");
    SetTargetFPS(60);
    srand(static_cast<unsigned int>(time(0)));

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Welcome to the slots!", CenterX("Welcome to the slots!", 30), 20, 30, PURPLE);

        int ruleY = 80;
        DrawText("Rules:", 20, ruleY, 20, WHITE);
        DrawText("- Hit 21 = +10 credits", 20, ruleY + 30, 18, WHITE);
        DrawText("- Hit 7  = +75 credits", 20, ruleY + 50, 18, WHITE);
        DrawText("- 3 of a kind = +1000 credits", 20, ruleY + 70, 18, WHITE);
        DrawText("- Secret code: 'bonus'", 20, ruleY + 90, 18, WHITE);

        const char* creditText = TextFormat("Credits: %d", credits);
        DrawText(creditText, GetScreenWidth() - MeasureText(creditText, 24) - 20, 20, 24, DARKPURPLE);

        int slotFontSize = 40;
        int slotSpacing = 80;
        int slotWidth = MeasureText("0", slotFontSize);
        int startX = (GetScreenWidth() - ((slotWidth + slotSpacing) * 3 - slotSpacing)) / 2;

        for (int i = 0; i < 3; i++) {
            int x = startX + i * (slotWidth + slotSpacing);
            DrawText(TextFormat("%d", slotValues[i]), x, 280, slotFontSize, GREEN);
        }

        if (IsKeyPressed(KEY_SPACE)) {
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

        if (!message.empty()) {
            DrawText(message.c_str(), CenterX(message.c_str(), 20), 400, 20, GOLD);
        }

        if (showNoCreditsMessage) {
            const char* noCredits = "No credits left.";
            DrawText(noCredits, CenterX(noCredits, 24), 440, 24, RED);
        }

        const char* spinMsg = "Press [SPACE] to Spin";
        DrawText(spinMsg, CenterX(spinMsg, 24), 500, 24, WHITE);
        EndDrawing();
    }

    UnloadSound(credit_sound);
    UnloadSound(jackpot);
    UnloadSound(twenty_one);
    UnloadSound(machine_spin);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}