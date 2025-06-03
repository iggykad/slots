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

int main() 
{
    InitWindow(800, 600, "Slot Machine");
    InitAudioDevice();
    twenty_one = LoadSound("twentyone.mp3");
    credit_sound = LoadSound("credits.mp3");
    jackpot = LoadSound("jackpot.mp3");
    pause_music = LoadSound("pause.mp3");
    machine_spin = LoadSound("spin.mp3");
    SetTargetFPS(60);
    srand(static_cast<unsigned int>(time(0))); //starting value 0, this seeds rand and helps generate random numbers. supposedly not efficient but i do not care right now


    while (!WindowShouldClose()) //check if KEY_ESCAPE pressed or windows close icon clicked
    {

        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Welcome to the slots!", 200, 20, 30, RED);

        DrawText("Rules:", 20, 60, 20, WHITE);
        DrawText("- Hit 21 = +10 credits", 20, 90, 18, WHITE);
        DrawText("- Hit 7  = +75 credits", 20, 110, 18, WHITE);
        DrawText("- 3 of a kind = +1000 credits", 20, 130, 18, WHITE);
        DrawText("- Secret code: 'bonus'", 20, 150, 18, WHITE);
        DrawText(TextFormat("Credits: %d", credits), 20, 300, 24, YELLOW);

        for (int i = 0; i < 3; i++) {
            int x = 300 + i * 80;
            DrawText(TextFormat("%d", slotValues[i]), x, 280, 40, GREEN);
        }

        if (IsKeyPressed(KEY_SPACE) && credits >= 5) {
            credits -= 5;
            PlaySound(machine_spin);
            runSlot(credits);

        }
        if (IsKeyPressed(KEY_SPACE) && credits < 5) {
            BeginDrawing();
            DrawText("No credits left.", 20, 335, 24, RED);
        }


        DrawText("Press [SPACE] to Spin", 200, 500, 24, SKYBLUE);

        if (!message.empty()) {
            DrawText(message.c_str(), 250, 400, 20, GOLD);
        }

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