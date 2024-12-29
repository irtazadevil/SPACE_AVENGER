#include <raylib.h>
#include <stdlib.h>
#include <time.h>
// Obstacle properties
struct Obstacle
{
    Vector2 position;
    float speed;
    bool active;
};

int main()
{
    int screen_width = 800;
    int screen_height = 600;
    InitWindow(screen_width, screen_height, "SPACE AVENGERS");
    SetTargetFPS(90);

    // For loading images into my screen
    Texture2D plane = LoadTexture("images/plane.png");
    Texture2D obstacle_texture = LoadTexture("images/obstacles.png");
    Texture2D planet1 = LoadTexture("images/planet1.png");
    Texture2D planet2 = LoadTexture("images/planet2.png");
    Texture2D planet3 = LoadTexture("images/planet3.png");
    Texture2D planet4 = LoadTexture("images/planet4.png");
    Texture2D galaxy = LoadTexture("images/galaxy.png");
    // Struct for obstacle is declared here
    struct Obstacle obstacles[8] = {0};

    // Setting position for images
    
    Vector2 plane_position = {360, 480};
    Vector2 planet1_p = {220, 320};
    Vector2 planet2_p = {600, 360};
    Vector2 planet2_p_s = {50, 750};
    Vector2 planet3_p = {0, 0};
    Vector2 planet4_p = {560, 0};
    Vector2 galaxy_p = {0, 450};
    // Initiallizing the values
    int plane_lives = 3;
    bool plane_hit = false;
    float plane_speed = 200.0;
    float hit_timer;
    int score = 0;
    bool game_over = false;
    bool game_started = false;
    // For getting time since the epoch
    srand(time(0));
    // Game loop starts from here
    while (WindowShouldClose() == false)
    {
        // Game Starting from here
        if (game_over && IsKeyPressed(KEY_E))
        {
            break;
        }

        if (game_started == false)
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("START THE GAME!", screen_width / 2 - 180, screen_height - 340, 40, RED);
            DrawText("PRESS ENTER AVENGER!", screen_width - 520, screen_height - 260, 20, WHITE);
            DrawText("DEVELOPED BY IRTAZA", 285, 400, 20, WHITE);
            DrawTextureEx(planet1, planet1_p, 0.10f, 0.40, WHITE);
            DrawTextureEx(planet2, planet2_p, 0.10f, 3, WHITE);
            DrawTextureEx(planet2, planet2_p_s, 0.10f, 1, WHITE);
            DrawTextureEx(planet3, planet3_p, 0.10f, 2, WHITE);
            DrawTextureEx(planet4, planet4_p, 0.10f, 2, WHITE);
            DrawTextureEx(galaxy, galaxy_p, 0.10f, 3, WHITE);
            EndDrawing();

            if (IsKeyPressed(KEY_ENTER))
            {
                game_started = true;
            }
            continue;
        }

        // For controlling movement of our plane
        if (game_over == false)
        {
            if (IsKeyDown(KEY_UP) && plane_position.y > 0)
            {
                plane_position.y -= plane_speed * GetFrameTime();
            }
            else if (IsKeyDown(KEY_DOWN) && plane_position.y < screen_height - plane.height * 0.10)
            {
                plane_position.y += plane_speed * GetFrameTime();
            }
            else if (IsKeyDown(KEY_LEFT) && plane_position.x > 0)
            {
                plane_position.x -= plane_speed * GetFrameTime();
            }
            else if (IsKeyDown(KEY_RIGHT) && plane_position.x < screen_width - plane.width * 0.10)
            {
                plane_position.x += plane_speed * GetFrameTime();
            }
            // for making random obstacles fall in our way
            for (int i = 0; i < 8; i++)
            {
                if (obstacles[i].active == false)
                {
                    if (rand() % 100 < 2 )
                    {
                        obstacles[i].active = true;
                        obstacles[i].position.x = rand() % (int)(screen_width - obstacle_texture.width * 0.15);
                        obstacles[i].position.y = -obstacle_texture.height * 0.15; // Start off-screen
                        obstacles[i].speed = (150 + rand() % 150);                 // Random speed
                    }
                }
                else
                {
                    obstacles[i].position.y += obstacles[i].speed * GetFrameTime();

                    // Update the plane's collision rectangle based on its position
                    Rectangle plane_rectangle = {plane_position.x, plane_position.y, plane.width * 0.10f, plane.height * 0.10f};
                    Rectangle obstacle_rectangle = {obstacles[i].position.x, obstacles[i].position.y, obstacle_texture.width * 0.15f, obstacle_texture.height * 0.15f};

                    if (CheckCollisionRecs(plane_rectangle, obstacle_rectangle) == true)
                    {
                        obstacles[i].active = false; // Obstacle wwill decativate
                        if (plane_hit == false)
                        {
                            plane_lives -= 1; // Life will be decreased
                            plane_hit = true;
                            hit_timer = 0.5; // For showing that plane is hurt
                        }
                        if (plane_lives <= 0)
                        {
                            game_over = true;
                        }
                    }

                    // Incrementing score and deactivating offscreen obstacles
                    if (obstacles[i].position.y > screen_height)
                    {
                        obstacles[i].active = false;
                        score++;
                    }
                }
            }
            // Handle the hit effect
            if (plane_hit == true)
            {
                hit_timer -= 0.02;
                if (hit_timer <= 0)
                {
                    plane_hit = false;
                }
            }
        }

        // End screen Drawing starts here
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureEx(planet1, planet1_p, 0.10f, 0.40, WHITE);
        DrawTextureEx(planet2, planet2_p, 0.10f, 3, WHITE);
        DrawTextureEx(planet2, planet2_p_s, 0.10f, 1, WHITE);
        DrawTextureEx(planet3, planet3_p, 0.10f, 2, WHITE);
        DrawTextureEx(planet4, planet4_p, 0.10f, 2, WHITE);
        DrawTextureEx(galaxy, galaxy_p, 0.10f, 3, WHITE);
        if (game_over == true)
        {
            DrawText("START OVER AGAIN!", screen_width - 600, screen_height - 340, 40, RED);
            DrawText("1.PRESS R AVENGER!", screen_width - 520, screen_height - 260, 20, WHITE);
            DrawText("2.PRESS E (EXIT)", screen_width - 520, screen_height - 200, 20, WHITE);
            // Reset game here
            if (IsKeyPressed(KEY_R))
            {
                score = 0;
                plane_lives = 3;
                plane_position.x = 360;
                plane_position.y = 480;
                game_over = false;
                game_started = true;
                for (int i = 0; i < 8; i++)
                {
                    obstacles[i].active = false;
                }
            }
        }
        else
        {
            if (plane_hit == true)
            {
                DrawTextureEx(plane, plane_position, 0.0f, 0.10, RED);
            }
            else
            {
                DrawTextureEx(plane, plane_position, 0.0f, 0.10, WHITE);
            }
            // for drawing obstacles
            for (int i = 0; i < 8; i++)
            {
                if (obstacles[i].active)
                {
                    DrawTextureEx(obstacle_texture, obstacles[i].position, 0.0f, 0.25, WHITE);
                }
            }
            // for drawing score and lives
            DrawText(TextFormat("Score: %d", score), 10, 10, 20, RAYWHITE);
            DrawText(TextFormat("Lives: %d", plane_lives), 10, 40, 20, RAYWHITE);
        }
        EndDrawing();
    }
    // Unloading textures to free the memory space
    UnloadTexture(plane);
    UnloadTexture(obstacle_texture);
    UnloadTexture(planet1);
    UnloadTexture(planet2);
    UnloadTexture(planet3);
    UnloadTexture(planet4);
    UnloadTexture(plane);
    UnloadTexture(galaxy);
    CloseWindow();
    return 0;
}
