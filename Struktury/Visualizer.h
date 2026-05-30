#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <optional>

#include "Krasnal.h"
#include "Kopalnia.h"
#include "Straznik.h"
#include "WorkAssignmentSolver/WorkAssignmentSolver.h"
#include "BorderPatrolSolver.h"
#include "GuardCommandSolver/GuardCommandSolver.h"

constexpr float MAP_W       = 1100.f;
constexpr float MAP_H       = 750.f;
constexpr float PANEL_W     = 300.f;
constexpr float WIN_W       = MAP_W + PANEL_W;
constexpr float WIN_H       = MAP_H + 80.f;
constexpr float MARGIN      = 40.f;

constexpr float NODE_R      = 22.f;
constexpr float GUARD_R     = 10.f;
constexpr float DWARF_R     = 10.f;
constexpr float ANIM_SPEED  = 60.f;

enum class Phase {
    WORK_ASSIGNMENT,
    BORDER_PATROL,
    GUARD_COMMAND
};

struct DwarfAnim {
    int          dwarfId;
    sf::Vector2f start;
    sf::Vector2f end;
    sf::Vector2f pos;
    float        progress;
    bool         arrived;
    bool         preferred;
};

struct InfoPopup {
    bool         visible = false;
    sf::Vector2f screenPos;
    std::vector<std::string> lines;
};

class Visualizer {
public:
    Visualizer(
        const std::vector<Krasnal>&      dwarves,
        const std::vector<Kopalnia>&     mines,
        const std::vector<Straznik>&     guards,
        const WorkAssignmentResult&      assignmentResult,
        const std::vector<Point>&        patrolHull,
        double                           patrolDistance,
        GuardCommandSolver&              guardSolver
    );

    void run();

private:
    // --- dane ---
    std::vector<Krasnal>     m_dwarves;
    std::vector<Kopalnia>    m_mines;
    std::vector<Straznik>    m_guards;
    WorkAssignmentResult     m_assignment;
    std::vector<Point>       m_hull;
    double                   m_patrolDist;
    GuardCommandSolver&      m_guardSolver;

    // --- SFML ---
    sf::RenderWindow         m_window;
    sf::Font                 m_font;

    // --- tekstury ---
    sf::Texture m_texDwarf, m_texMine, m_texHome, m_texGuard;
    bool m_hasDwarfTex = false;
    bool m_hasMineTex  = false;
    bool m_hasHomeTex  = false;
    bool m_hasGuardTex = false;

    // --- stan ---
    Phase m_phase    = Phase::WORK_ASSIGNMENT;
    bool  m_animDone = false;

    // --- animacje ---
    std::vector<DwarfAnim> m_dwarfAnims;
    sf::Clock              m_clock;

    // --- faza 3 ---
    int  m_guardRangeL    = -1;
    int  m_guardRangeR    = -1;
    int  m_loudestIdx     = -1;
    bool m_guardQueryDone = false;
    float m_attackTimer   = 0.f;
    bool m_attackResolved = false;

    // --- popup ---
    InfoPopup m_popup;

    // --- pozycje ekranowe ---
    std::vector<sf::Vector2f> m_minePos;
    std::vector<sf::Vector2f> m_homePos;
    std::vector<sf::Vector2f> m_guardPos;

    // --- przyciski ---
    sf::RectangleShape      m_btnPhase1, m_btnPhase2, m_btnPhase3;
    std::optional<sf::Text> m_lblPhase1, m_lblPhase2, m_lblPhase3;

    // --- skalowanie i zakres świata ---
    float m_scaleX    = 1.f;
    float m_scaleY    = 1.f;
    float m_minX      = 0.f;
    float m_minY      = 0.f;
    float m_worldMaxX = 1.f;
    float m_worldMaxY = 1.f;

    // ---- inicjalizacja ----
    void loadAssets();
    void buildLayout();
    void buildDwarfAnims();
    void buildButtons();

    // ---- pętla ----
    void handleEvents(const sf::Event& e);
    void update(float dt);
    void render();

    // ---- rysowanie faz ----
    void drawPhase1();
    void drawPhase2();
    void drawPhase3();

    // ---- pomocniki rysowania ----
    void drawGrid();
    void drawEdges();
    void drawHullEdges();
    void drawMines();
    void drawHomes();
    void drawGuards();
    void drawBorderAttack();
    void drawDwarfAnims();
    void drawSidePanel();
    void drawButtons();
    void drawPopup();

    void drawNode(sf::Vector2f pos, float r, sf::Color fill,
                  const std::string& label, sf::Texture* tex = nullptr);

    // ---- linia ----
    void drawThickLine(sf::Vector2f a, sf::Vector2f b,   // <-- nowe
                       sf::Color col, float thickness = 2.f);

    // ---- kliknięcia ----
    void onClickMap(sf::Vector2f pos);
    void onClickGuardRange(sf::Vector2f pos);

    // ---- konwersja współrzędnych ----
    sf::Vector2f worldToScreen(double wx, double wy) const;
    sf::Vector2f worldToScreen(const Point& p) const;

    // ---- tekst ----
    sf::Text makeText(const std::string& str, unsigned size,
                      sf::Color col = sf::Color::White) const;
    void     drawTextAt(const std::string& str, float x, float y,
                        unsigned size = 14, sf::Color col = sf::Color::White);
};
