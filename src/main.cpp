#include "raylib.h"
#include "raymath.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

/* ----------------------------------------------------------------------------
Constants
----------------------------------------------------------------------------- */

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;

const int POINT_CLICK_RADIUS = 6;

/* ----------------------------------------------------------------------------
Polyline class
----------------------------------------------------------------------------- */

class Polyline {
  public:
    /**
     * Creates an empty polyline with no points.
     * @param color The color to draw the line.
     */
    Polyline(Color color);

    /** Destructor */
    ~Polyline();

    /**
     * Adds a new segment to the line.
     */
    void AddPoint(Vector2 position);

    /**
     * Draws the polyline.
     */
    void Draw() const;

    /**
     * Draws a line from the last point of the line to the point specified.
     * @param point The ending point for the line to be draw.
     * @param color The color of the line.
     */
    void DrawPreview(Vector2 point, Color previewColor) const;

    /**
     * Adds a new point to the line that matches the first point.
     */
    void Finish();

    /**
     * Checks if the specified point is close to the first point in the line.
     * @param point The point to check if it is close to the first point of this polyline.
     * @param targetDistance If the point is within this distance, consider it "close".
     * @returns true if point is within the target distance to the first point of the line.
     */
    bool IsCloseToFirstPoint(Vector2 point, float targetDistance) const;

  private:
    Vector2* m_points;
    int m_pointsCount;
    Color m_color;
};

Polyline::Polyline(Color color)
{
    m_points = nullptr;
    m_pointsCount = 0;
    m_color = color;
}

Polyline::~Polyline()
{
    // TODO : Implement this
}

void Polyline::AddPoint(Vector2 position)
{
    // TODO : Implement this
}

void Polyline::Draw() const
{
    const int POINT_WIDTH = 5;

    // Process each point in the polyline
    for (int i = 0; i < m_pointsCount; ++i) {
        Vector2* cur = &m_points[i];

        // Draw a rectangle to emphasize each point
        DrawRectangle((int)cur->x - POINT_WIDTH / 2, (int)cur->y - POINT_WIDTH / 2, POINT_WIDTH, POINT_WIDTH, m_color);

        // No line to draw if processing the first point
        if (i == 0)
            continue;

        // Draw line from previous point to this point
        Vector2* prev = &m_points[i - 1];
        DrawLineV(*prev, *cur, m_color);
    }
}

void Polyline::DrawPreview(Vector2 point, Color previewColor) const
{
    // Nothing to draw if there are no points yet.
    if (m_pointsCount == 0)
        return;

    // Draw line from last point in the line to the specified point to preview what the line would look like
    Vector2* last = &m_points[m_pointsCount - 1];
    DrawLineV(*last, point, previewColor);
}

void Polyline::Finish()
{
    if (m_pointsCount == 0)
        throw logic_error("Cannot finish a polyline with no points.");

    AddPoint(m_points[0]);
}

bool Polyline::IsCloseToFirstPoint(Vector2 point, float targetDistance) const
{
    if (m_pointsCount == 0)
        return false;

    // Get absolute value of the distance between the first point and the specified point
    float distance = abs(Vector2Distance(m_points[0], point));
    return distance <= targetDistance;
}

/* ----------------------------------------------------------------------------
App state
----------------------------------------------------------------------------- */

vector<Color> g_colors = {RED, GREEN, BLUE, PINK, ORANGE, GREEN};
int g_nextColorIndex = 0;

vector<Polyline*> g_lines;
Polyline* g_lineBeingBuilt = nullptr;

/* ----------------------------------------------------------------------------
Functions
----------------------------------------------------------------------------- */

/**
 * Gets the color to use for the next line.
 */
Color GetNextLineColor()
{
    Color c = g_colors[g_nextColorIndex];
    g_nextColorIndex = (g_nextColorIndex + 1) % g_colors.size();
    return c;
}

/**
Program entry point.
*/
int main(void)
{
    /* ------------------------------------------------------------------------
    Initialization
    ------------------------------------------------------------------------- */

    // Scale things for high DPI monitors so we can see it
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);

    // Setup the application window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Polylines");

    // Set game to run at 60 frames-per-second
    SetTargetFPS(60);

    // Start with one line
    g_lines.push_back(new Polyline(GetNextLineColor()));
    g_lines[0]->AddPoint({10, 10});
    g_lines[0]->AddPoint({30, 30});
    g_lineBeingBuilt = g_lines.back();

    /* ------------------------------------------------------------------------
    Main loop
    ------------------------------------------------------------------------- */

    while (!WindowShouldClose()) {

        Vector2 mousePos = {(float)GetMouseX(), (float)GetMouseY()};

        /* ------------------------------------------------------------------------
        Update frame
        ------------------------------------------------------------------------- */

        // User can add a new line segment by clicking the left mouse button
        if (g_lineBeingBuilt && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

            // If close enough to the first point, finish the polyline
            if (g_lineBeingBuilt->IsCloseToFirstPoint(mousePos, POINT_CLICK_RADIUS)) {
                g_lineBeingBuilt->Finish();

                // Start a new line
                g_lines.push_back(new Polyline(GetNextLineColor()));
                g_lineBeingBuilt = g_lines.back();
            }
            else {
                g_lineBeingBuilt->AddPoint(mousePos);
            }
        }

        // User can reset all lines if they press the right mouse button
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {

            // Delete created objects
            for (auto line : g_lines)
                delete line;

            // Clear old lines
            g_lines.clear();

            // Start a new line
            g_lines.push_back(new Polyline(GetNextLineColor()));
            g_lineBeingBuilt = g_lines.back();
        }

        /* ------------------------------------------------------------------------
        Draw frame
        ------------------------------------------------------------------------- */

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw all lines
        for (const auto line : g_lines) {
            line->Draw();
        }

        // Draw preview line to mouse cursor for next line segment
        if (g_lineBeingBuilt != nullptr)
            g_lineBeingBuilt->DrawPreview(mousePos, LIGHTGRAY);

        // Highlight the last point if you are close enough to click on it
        if (g_lineBeingBuilt->IsCloseToFirstPoint(mousePos, POINT_CLICK_RADIUS)) {
            DrawRectangle(mousePos.x - POINT_CLICK_RADIUS / 2, mousePos.y - POINT_CLICK_RADIUS / 2, POINT_CLICK_RADIUS, POINT_CLICK_RADIUS, YELLOW);
        }

        EndDrawing();
    }

    // Close window and OpenGL context
    CloseWindow();

    return 0;
}
