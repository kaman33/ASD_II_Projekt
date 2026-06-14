#include "Visualizer.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cstdint>

static sf::Color hex(uint32_t c) {
    return sf::Color(
        (c >> 16) & 0xFF,
        (c >>  8) & 0xFF,
        (c      ) & 0xFF
    );
}

static const sf::Color COL_BG        = hex(0x1a1a2e);
static const sf::Color COL_PANEL     = hex(0x16213e);
static const sf::Color COL_MINE      = hex(0xe4a832);
static const sf::Color COL_HOME      = hex(0x4ecdc4);
static const sf::Color COL_GUARD     = hex(0xe84393);
static const sf::Color COL_EDGE      = hex(0x6c757d);
static const sf::Color COL_PREF_EDGE = hex(0x00d4aa);
static const sf::Color COL_HULL      = hex(0xff6b6b);
static const sf::Color COL_LOUDEST   = hex(0xff4444);
static const sf::Color COL_GRID      = sf::Color(50, 55, 80, 180);
static const sf::Color COL_GRID_LABEL= sf::Color(120, 130, 160, 220);
static const float ATTACK_RESOLVE_TIME = 2.0f;

Visualizer::Visualizer(
    const std::vector<Krasnal>&      dwarves,
    const std::vector<Kopalnia>&     mines,
    const std::vector<Straznik>&     guards,
    const WorkAssignmentResult&      assignmentResult,
    const std::vector<Point>&        patrolHull,
    double                           patrolDistance,
    GuardCommandSolver&              guardSolver
)
    : m_dwarves(dwarves),
      m_mines(mines),
      m_guards(guards),
      m_assignment(assignmentResult),
      m_hull(patrolHull),
      m_patrolDist(patrolDistance),
      m_guardSolver(guardSolver),
      m_window(sf::VideoMode(static_cast<unsigned>(WIN_W), static_cast<unsigned>(WIN_H)),
               "Krolestwo Krolewny Sniezki",
               sf::Style::Titlebar | sf::Style::Close)
{
    m_window.setFramerateLimit(60);
    loadAssets();
    buildLayout();
    buildDwarfAnims();
    buildButtons();
}

void Visualizer::loadAssets() {
    const std::vector<std::string> fontPaths = {
        "assets/font.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/calibri.ttf",
        "C:/Windows/Fonts/segoeui.ttf"
    };
    for (const auto& fp : fontPaths) {
        if (m_font.loadFromFile(fp)) break;
    }

    m_hasDwarfTex = m_texDwarf.loadFromFile("Zasoby/dwarf.png");
    m_hasMineTex  = m_texMine.loadFromFile("Zasoby/mine.png");
    m_hasHomeTex  = m_texHome.loadFromFile("Zasoby/home.png");
    m_hasGuardTex = m_texGuard.loadFromFile("Zasoby/guard.png");

    if (m_hasDwarfTex) m_texDwarf.setSmooth(true);
    if (m_hasMineTex)  m_texMine.setSmooth(true);
    if (m_hasHomeTex)  m_texHome.setSmooth(true);
    if (m_hasGuardTex) m_texGuard.setSmooth(true);
}

void Visualizer::buildLayout() {
    std::vector<double> allX, allY;
    for (const auto& k : m_dwarves) { allX.push_back(k.getHomeX()); allY.push_back(k.getHomeY()); }
    for (const auto& m : m_mines)   { allX.push_back(m.getX());     allY.push_back(m.getY());     }

    if (allX.empty()) { m_scaleX = 1.f; m_scaleY = 1.f; m_minX = 0.f; m_minY = 0.f; return; }

    m_minX = (float)*std::min_element(allX.begin(), allX.end());
    m_minY = (float)*std::min_element(allY.begin(), allY.end());
    float maxX = (float)*std::max_element(allX.begin(), allX.end());
    float maxY = (float)*std::max_element(allY.begin(), allY.end());

    // Wymuś kwadratowy zakres — obie osie mają ten sam max
    float globalMax = std::max(maxX, maxY);
    maxX = globalMax;
    maxY = globalMax;

    float rangeX = maxX - m_minX;
    float rangeY = maxY - m_minY;

    float usableW = MAP_W - 2.f * MARGIN;
    float usableH = MAP_H - 2.f * MARGIN;

    m_scaleX = (rangeX > 0) ? (usableW / rangeX) : 1.f;
    m_scaleY = (rangeY > 0) ? (usableH / rangeY) : 1.f;
    float scale = std::min(m_scaleX, m_scaleY);
    m_scaleX = scale;
    m_scaleY = scale;

    // Zapisz zakres świata do rysowania siatki
    m_worldMaxX = globalMax;
    m_worldMaxY = globalMax;

    m_minePos.clear();
    for (const auto& mine : m_mines)
        m_minePos.push_back(worldToScreen(mine.getX(), mine.getY()));

    m_homePos.clear();
    for (const auto& k : m_dwarves)
        m_homePos.push_back(worldToScreen(k.getHomeX(), k.getHomeY()));

    // Strażnicy wzdłuż otoczki
    m_guardPos.clear();
    int n = (int)m_guards.size();
    if (n == 0) return;

    if (m_hull.size() >= 2) {
        std::vector<float> edgeLengths;
        float totalLen = 0.f;
        for (size_t i = 0; i < m_hull.size(); i++) {
            sf::Vector2f a = worldToScreen(m_hull[i]);
            sf::Vector2f b = worldToScreen(m_hull[(i + 1) % m_hull.size()]);
            float dx = b.x - a.x, dy = b.y - a.y;
            float len = std::sqrt(dx * dx + dy * dy);
            edgeLengths.push_back(len);
            totalLen += len;
        }
        float spacing = totalLen / n;
        size_t edge = 0;
        float edgeDist = 0.f;
        for (int i = 0; i < n; i++) {
            float target = i * spacing;
            while (edge < edgeLengths.size() - 1 && edgeDist + edgeLengths[edge] < target) {
                edgeDist += edgeLengths[edge];
                edge++;
            }
            float t = (edgeLengths[edge] > 0) ? (target - edgeDist) / edgeLengths[edge] : 0.f;
            t = std::max(0.f, std::min(1.f, t));
            sf::Vector2f a = worldToScreen(m_hull[edge % m_hull.size()]);
            sf::Vector2f b = worldToScreen(m_hull[(edge + 1) % m_hull.size()]);
            m_guardPos.push_back({ a.x + t * (b.x - a.x), a.y + t * (b.y - a.y) });
        }
    } else {
        float cx = MAP_W / 2.f, cy = MAP_H / 2.f, r = 250.f;
        for (int i = 0; i < n; i++) {
            float angle = 2.f * 3.14159f * i / n;
            m_guardPos.push_back({cx + r * std::cos(angle), cy + r * std::sin(angle)});
        }
    }
}

sf::Vector2f Visualizer::worldToScreen(double wx, double wy) const {
    float sx = MARGIN + ((float)wx - m_minX) * m_scaleX;
    float sy = (MAP_H - MARGIN) - ((float)wy - m_minY) * m_scaleY;
    return {sx, sy};
}
sf::Vector2f Visualizer::worldToScreen(const Point& p) const {
    return worldToScreen(p.x, p.y);
}

void Visualizer::buildDwarfAnims() {
    m_dwarfAnims.clear();
    for (const auto& asgn : m_assignment.assignments) {
        int dIdx = -1, mIdx = -1;
        for (int i = 0; i < (int)m_dwarves.size(); i++)
            if (m_dwarves[i].getId() == asgn.dwarfId) { dIdx = i; break; }
        for (int i = 0; i < (int)m_mines.size(); i++)
            if (m_mines[i].getId() == asgn.mineId) { mIdx = i; break; }
        if (dIdx < 0 || mIdx < 0) continue;

        DwarfAnim da;
        da.dwarfId   = asgn.dwarfId;
        da.start     = m_homePos[dIdx];
        da.end       = m_minePos[mIdx];
        da.pos       = da.start;
        da.progress  = 0.f;
        da.arrived   = false;
        da.preferred = asgn.preferredResource;
        m_dwarfAnims.push_back(da);
    }
}

void Visualizer::buildButtons() {
    float btnW = (MAP_W / 3.f) - 10.f;
    float btnH = 50.f;
    float btnY = MAP_H + 15.f;

    auto makeBtn = [&](sf::RectangleShape& btn, std::optional<sf::Text>& lbl,
                       float x, const std::string& text)
    {
        btn.setSize({btnW, btnH});
        btn.setPosition({x, btnY});
        btn.setFillColor(COL_PANEL);
        btn.setOutlineColor(sf::Color(100, 100, 160));
        btn.setOutlineThickness(2.f);

        lbl.emplace(sf::String::fromUtf8(text.begin(), text.end()), m_font, 14u);
        lbl->setFillColor(sf::Color::White);

        sf::FloatRect bounds = lbl->getLocalBounds();
        lbl->setOrigin(bounds.left + bounds.width / 2.f,
                       bounds.top + bounds.height / 2.f);
        lbl->setPosition(x + btnW / 2.f, btnY + btnH / 2.f);
    };

    makeBtn(m_btnPhase1, m_lblPhase1, 5.f,                "1. Przydzielanie pracy");
    makeBtn(m_btnPhase2, m_lblPhase2, MAP_W / 3.f + 5.f,  "2. Patrol granicy");
    makeBtn(m_btnPhase3, m_lblPhase3, 2.f * MAP_W / 3.f + 5.f, "3. Obrona granicy");
}

void Visualizer::run() {
    m_clock.restart();
    while (m_window.isOpen()) {
        sf::Event e;
        while (m_window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) m_window.close();
            handleEvents(e);
        }
        float dt = m_clock.restart().asSeconds();
        update(dt);
        m_window.clear(COL_BG);
        render();
        m_window.display();
    }
}

void Visualizer::handleEvents(const sf::Event& e) {
    if (e.type == sf::Event::MouseButtonPressed) {
        if (e.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mp(static_cast<float>(e.mouseButton.x),
                            static_cast<float>(e.mouseButton.y));
            if (m_btnPhase1.getGlobalBounds().contains(mp)) {
                m_phase = Phase::WORK_ASSIGNMENT; buildDwarfAnims(); m_animDone = false; m_popup.visible = false; m_panelScrollY = 0.f; return;
            }
            if (m_btnPhase2.getGlobalBounds().contains(mp)) {
                m_phase = Phase::BORDER_PATROL; m_popup.visible = false; m_panelScrollY = 0.f; return;
            }
            if (m_btnPhase3.getGlobalBounds().contains(mp)) {
                m_phase = Phase::GUARD_COMMAND; m_guardRangeL = m_guardRangeR = -1; m_loudestIdx = -1; m_guardQueryDone = false; m_attackTimer = 0.f; m_attackResolved = false; m_popup.visible = false; m_panelScrollY = 0.f; return;
            }
            if (mp.x < MAP_W && mp.y < MAP_H) {
                if (m_phase == Phase::GUARD_COMMAND) onClickGuardRange(mp);
                else onClickMap(mp);
            }
        }
    }
    if (e.type == sf::Event::MouseWheelScrolled) {
        if (e.mouseWheelScroll.x >= MAP_W) {
            const float visiblePanelHeight = WIN_H - 20.f;
            const float maxScroll = std::max(0.f, calculateSidePanelContentHeight() - visiblePanelHeight);
            m_panelScrollY -= e.mouseWheelScroll.delta * 24.f;
            m_panelScrollY = std::max(0.f, std::min(m_panelScrollY, maxScroll));
        }
    }
    if (e.type == sf::Event::KeyPressed) {
        if (e.key.code == sf::Keyboard::Escape) m_popup.visible = false;
        if (e.key.code == sf::Keyboard::Space && m_phase == Phase::WORK_ASSIGNMENT) {
            buildDwarfAnims(); m_animDone = false;
        }
    }
}

void Visualizer::update(float dt) {
    if (m_phase == Phase::GUARD_COMMAND) {
        if (m_guardQueryDone && !m_attackResolved) {
            m_attackTimer += dt;
            if (m_attackTimer >= ATTACK_RESOLVE_TIME) {
                m_attackResolved = true;
            }
        }
        return;
    }

    if (m_phase != Phase::WORK_ASSIGNMENT || m_animDone) return;
    bool allDone = true;
    for (auto& da : m_dwarfAnims) {
        if (da.arrived) continue;
        allDone = false;
        float dx = da.end.x - da.start.x, dy = da.end.y - da.start.y;
        float dist = std::sqrt(dx*dx + dy*dy);
        if (dist < 0.01f) { da.arrived = true; da.pos = da.end; continue; }
        da.progress += ANIM_SPEED * dt / dist;
        if (da.progress >= 1.f) { da.progress = 1.f; da.arrived = true; }
        da.pos = { da.start.x + da.progress * (da.end.x - da.start.x),
                   da.start.y + da.progress * (da.end.y - da.start.y) };
    }
    if (allDone) m_animDone = true;
}

void Visualizer::render() {
    sf::RectangleShape mapBg({MAP_W, MAP_H});
    mapBg.setFillColor(hex(0x0f0f23));
    m_window.draw(mapBg);

    sf::RectangleShape panel({PANEL_W, WIN_H});
    panel.setPosition({MAP_W, 0.f});
    panel.setFillColor(COL_PANEL);
    m_window.draw(panel);

    drawGrid();

    switch (m_phase) {
        case Phase::WORK_ASSIGNMENT: drawPhase1(); break;
        case Phase::BORDER_PATROL:   drawPhase2(); break;
        case Phase::GUARD_COMMAND:   drawPhase3(); break;
    }

    drawSidePanel();
    drawButtons();
    drawPopup();
}

float Visualizer::calculateSidePanelContentHeight() const {
    float y = 10.f;
    auto line = [&](int count = 1) {
        y += 20.f * static_cast<float>(count);
    };
    auto sep = [&]() {
        y += 6.f;
    };

    switch (m_phase) {
    case Phase::WORK_ASSIGNMENT:
        line();
        sep();
        line(2);
        sep();
        line(3);
        sep();
        line(4);
        sep();
        line();
        line(static_cast<int>(m_assignment.assignments.size()) * 2);
        if (!m_assignment.unassignedDwarfIds.empty()) {
            sep();
            line();
            line(static_cast<int>(m_assignment.unassignedDwarfIds.size()));
        }
        break;

    case Phase::BORDER_PATROL:
        line();
        sep();
        line(2);
        sep();
        line();
        line(static_cast<int>(m_hull.size()));
        break;

    case Phase::GUARD_COMMAND:
        line();
        sep();
        line();
        sep();
        if (m_guardRangeL >= 0) {
            line();
            if (m_guardRangeR >= 0) {
                if (m_attackResolved) {
                    line();
                } else {
                    line(2);
                }
            }
            if (m_guardQueryDone && m_loudestIdx >= 0) {
                line(4);
                sep();
                line(4);
            }
        } else {
            line(2);
        }
        sep();
        line();
        line(static_cast<int>(m_guards.size()));
        break;
    }

    return y + 10.f;
}

void Visualizer::drawGrid() {
    float worldW = m_worldMaxX - m_minX;
    float worldH = m_worldMaxY - m_minY;

    auto niceStep = [](float range) -> float {
        if (range <= 0.f) return 1.f;
        float rawStep = range / 9.f;
        float mag = std::pow(10.f, std::floor(std::log10(rawStep)));
        float norm = rawStep / mag;
        float step;
        if      (norm < 1.5f) step = 1.f;
        else if (norm < 3.5f) step = 2.f;
        else if (norm < 7.5f) step = 5.f;
        else                  step = 10.f;
        return step * mag;
    };

    float stepX = niceStep(worldW);
    float stepY = niceStep(worldH);

    // Pierwsza linia siatki
    float startX = std::ceil(m_minX / stepX) * stepX;
    float startY = std::ceil(m_minY / stepY) * stepY;

    // Rysuj linie pionowe
    for (float wx = startX; wx <= m_worldMaxX + stepX * 0.5f; wx += stepX) {
        sf::Vector2f top    = worldToScreen(wx, m_worldMaxY + stepY);
        sf::Vector2f bottom = worldToScreen(wx, m_minY - stepY);

        // Ogranicz do obszaru mapy
        top.y    = std::max(top.y,    MARGIN * 0.3f);
        bottom.y = std::min(bottom.y, MAP_H - MARGIN * 0.3f);

        sf::Vertex line[2] = {
            sf::Vertex{top,    COL_GRID},
            sf::Vertex{bottom, COL_GRID}
        };
        m_window.draw(line, 2, sf::PrimitiveType::Lines);

        // Etykieta
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << wx;
        sf::Text label(oss.str(), m_font, 10u);
        label.setFillColor(COL_GRID_LABEL);
        sf::FloatRect lb = label.getLocalBounds();
        label.setOrigin(lb.left + lb.width / 2.f, lb.top);
        sf::Vector2f labelPos = worldToScreen(wx, m_minY);
        label.setPosition(labelPos.x, MAP_H - MARGIN + 4.f);
        m_window.draw(label);
    }

    // Rysuj linie poziome
    for (float wy = startY; wy <= m_worldMaxY + stepY * 0.5f; wy += stepY) {
        sf::Vector2f left  = worldToScreen(m_minX - stepX, wy);
        sf::Vector2f right = worldToScreen(m_worldMaxX + stepX, wy);

        left.x  = std::max(left.x,  MARGIN * 0.3f);
        right.x = std::min(right.x, MAP_W - MARGIN * 0.3f);

        sf::Vertex line[2] = {
            sf::Vertex{left,  COL_GRID},
            sf::Vertex{right, COL_GRID}
        };
        m_window.draw(line, 2, sf::PrimitiveType::Lines);

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << wy;
        sf::Text label(oss.str(), m_font, 10u);
        label.setFillColor(COL_GRID_LABEL);
        sf::FloatRect lb = label.getLocalBounds();
        label.setOrigin(lb.left + lb.width, lb.top + lb.height / 2.f);
        sf::Vector2f labelPos = worldToScreen(m_minX, wy);
        label.setPosition(MARGIN - 4.f, labelPos.y);
        m_window.draw(label);
    }

    if (m_minX <= 0.f && m_worldMaxX >= 0.f) {
        sf::Vector2f top    = worldToScreen(0.0, m_worldMaxY + stepY);
        sf::Vector2f bottom = worldToScreen(0.0, m_minY - stepY);
        top.y    = std::max(top.y,    MARGIN * 0.3f);
        bottom.y = std::min(bottom.y, MAP_H - MARGIN * 0.3f);
        sf::Color axisCol(100, 120, 180, 200);
        sf::Vertex line[2] = { sf::Vertex{top, axisCol}, sf::Vertex{bottom, axisCol} };
        m_window.draw(line, 2, sf::PrimitiveType::Lines);
    }
    if (m_minY <= 0.f && m_worldMaxY >= 0.f) {
        sf::Vector2f left  = worldToScreen(m_minX - stepX, 0.0);
        sf::Vector2f right = worldToScreen(m_worldMaxX + stepX, 0.0);
        left.x  = std::max(left.x,  MARGIN * 0.3f);
        right.x = std::min(right.x, MAP_W - MARGIN * 0.3f);
        sf::Color axisCol(100, 120, 180, 200);
        sf::Vertex line[2] = { sf::Vertex{left, axisCol}, sf::Vertex{right, axisCol} };
        m_window.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

void Visualizer::drawPhase1() {
    for (size_t di = 0; di < m_homePos.size(); di++) {
        for (size_t mi = 0; mi < m_minePos.size(); mi++) {
            sf::Vertex line[2] = {
                sf::Vertex{m_homePos[di], sf::Color(70, 70, 90, 100)},
                sf::Vertex{m_minePos[mi], sf::Color(70, 70, 90, 100)}
            };
            m_window.draw(line, 2, sf::PrimitiveType::Lines);
        }
    }

    for (const auto& asgn : m_assignment.assignments) {
        int dIdx = -1, mIdx = -1;
        for (int i = 0; i < (int)m_dwarves.size(); i++)
            if (m_dwarves[i].getId() == asgn.dwarfId) { dIdx = i; break; }
        for (int i = 0; i < (int)m_mines.size(); i++)
            if (m_mines[i].getId() == asgn.mineId) { mIdx = i; break; }
        if (dIdx < 0 || mIdx < 0) continue;

        sf::Color c = asgn.preferredResource ? COL_PREF_EDGE : COL_EDGE;
        drawThickLine(m_homePos[dIdx], m_minePos[mIdx], c, 2.5f);
    }

    drawMines();
    drawHomes();
    drawDwarfAnims();
    drawTextAt("SPACJA = restart animacji  |  KLIKNIJ = szczegoly",
               10, MAP_H - 22, 13, hex(0x888888));
}

void Visualizer::drawPhase2() {
    for (size_t di = 0; di < m_homePos.size(); di++) {
        for (size_t mi = 0; mi < m_minePos.size(); mi++) {
            sf::Vertex line[2] = {
                sf::Vertex{m_homePos[di], sf::Color(50, 50, 70, 80)},
                sf::Vertex{m_minePos[mi], sf::Color(50, 50, 70, 80)}
            };
            m_window.draw(line, 2, sf::PrimitiveType::Lines);
        }
    }
    drawHullEdges();
    drawMines();
    drawHomes();
    std::ostringstream oss;
    oss << "Trasa patrolu: " << std::fixed << std::setprecision(2) << m_patrolDist;
    drawTextAt(oss.str(), 10, MAP_H - 22, 15, COL_HULL);
}

void Visualizer::drawPhase3() {
    drawHullEdges();
    drawMines();
    drawBorderAttack();
    drawGuards();

    if (m_guardRangeL >= 0 && m_guardRangeR < 0 && m_guardRangeL < (int)m_guardPos.size()) {
        sf::CircleShape ci(GUARD_R + 9.f);
        ci.setOrigin({GUARD_R + 9.f, GUARD_R + 9.f});
        ci.setPosition(m_guardPos[m_guardRangeL]);
        ci.setFillColor(sf::Color::Transparent);
        ci.setOutlineColor(hex(0xffaa00));
        ci.setOutlineThickness(3.f);
        m_window.draw(ci);
    }

    if (m_guardRangeL >= 0 && m_guardRangeR >= m_guardRangeL) {
        for (int i = m_guardRangeL; i <= m_guardRangeR && i < (int)m_guardPos.size(); i++) {
            sf::Color c = (i == m_loudestIdx) ? COL_LOUDEST : hex(0xffaa00);
            sf::CircleShape ci(GUARD_R + 7.f);
            ci.setOrigin({GUARD_R + 7.f, GUARD_R + 7.f});
            ci.setPosition(m_guardPos[i]);
            ci.setFillColor(sf::Color::Transparent);
            ci.setOutlineColor(c);
            ci.setOutlineThickness(3.f);
            m_window.draw(ci);
        }
    }

    if (m_guardQueryDone && m_loudestIdx >= 0) {
        sf::Vector2f gp = m_guardPos[m_loudestIdx];
        drawTextAt("NAJGLOSNIEJSZY!", gp.x - 55, gp.y - 32, 13, COL_LOUDEST);
    }

    drawTextAt("KLIKNIJ 2 straznikow aby wybrac atakowany odcinek",
               10, MAP_H - 22, 13, hex(0x888888));
}

void Visualizer::drawThickLine(sf::Vector2f a, sf::Vector2f b,
                                sf::Color col, float thickness)
{
    sf::Vector2f diff = b - a;
    float len = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    if (len < 0.01f) return;

    sf::RectangleShape rect;
    rect.setSize({len, thickness});
    rect.setOrigin({0.f, thickness / 2.f});
    rect.setPosition(a);
    rect.setRotation(std::atan2(diff.y, diff.x) * 180.f / 3.14159f);
    rect.setFillColor(col);
    m_window.draw(rect);
}

void Visualizer::drawHullEdges() {
    if (m_hull.size() < 2) return;
    for (size_t i = 0; i < m_hull.size(); i++) {
        sf::Vector2f a = worldToScreen(m_hull[i]);
        sf::Vector2f b = worldToScreen(m_hull[(i + 1) % m_hull.size()]);
        drawThickLine(a, b, COL_HULL, 3.f);
    }
}

void Visualizer::drawBorderAttack() {
    if (m_guardRangeL < 0 || m_guardRangeR < m_guardRangeL || m_guardPos.empty()) {
        return;
    }

    const int left = std::max(0, m_guardRangeL);
    const int right = std::min(m_guardRangeR, static_cast<int>(m_guardPos.size()) - 1);
    if (left > right) {
        return;
    }

    const int middle = left + (right - left) / 2;
    sf::Vector2f center = m_guardPos[middle];
    if ((right - left) % 2 == 1 && middle + 1 <= right) {
        center.x = (m_guardPos[middle].x + m_guardPos[middle + 1].x) / 2.f;
        center.y = (m_guardPos[middle].y + m_guardPos[middle + 1].y) / 2.f;
    }

    sf::Vector2f borderCenter(0.f, 0.f);
    int borderPointCount = 0;
    if (!m_hull.empty()) {
        for (const Point& point : m_hull) {
            borderCenter += worldToScreen(point);
            borderPointCount++;
        }
    }
    else {
        for (const sf::Vector2f& guardPos : m_guardPos) {
            borderCenter += guardPos;
            borderPointCount++;
        }
    }

    if (borderPointCount > 0) {
        borderCenter.x /= static_cast<float>(borderPointCount);
        borderCenter.y /= static_cast<float>(borderPointCount);
    }
    else {
        borderCenter = {MAP_W / 2.f, MAP_H / 2.f};
    }

    sf::Vector2f outward = center - borderCenter;
    float length = std::sqrt(outward.x * outward.x + outward.y * outward.y);
    if (length < 0.01f) {
        outward = {0.f, -1.f};
        length = 1.f;
    }
    outward /= length;

    sf::Vector2f tangent(-outward.y, outward.x);
    sf::Vector2f labelPos = center + outward * 48.f;
    if (m_attackResolved) {
        for (int i = left; i <= right; i++) {
            sf::CircleShape zone(GUARD_R + 12.f);
            zone.setOrigin({GUARD_R + 12.f, GUARD_R + 12.f});
            zone.setPosition(m_guardPos[i]);
            zone.setFillColor(sf::Color(20, 210, 135, 35));
            zone.setOutlineColor(sf::Color(20, 210, 135, 140));
            zone.setOutlineThickness(2.f);
            m_window.draw(zone);
        }

        for (int i = left; i < right; i++) {
            drawThickLine(m_guardPos[i], m_guardPos[i + 1], sf::Color(20, 210, 135, 120), 7.f);
        }

        sf::CircleShape burst(16.f);
        burst.setOrigin({16.f, 16.f});
        burst.setPosition(center);
        burst.setFillColor(sf::Color(20, 210, 135, 55));
        burst.setOutlineColor(sf::Color(170, 255, 220, 180));
        burst.setOutlineThickness(2.f);
        m_window.draw(burst);

        drawTextAt("ODPARTO", labelPos.x - 34.f, labelPos.y - 8.f, 14, sf::Color(80, 255, 180));
        return;
    }

    const float volleyProgress = std::min(1.f, m_attackTimer / 1.05f);
    const float attackFade = std::max(0.25f, 1.f - std::max(0.f, m_attackTimer - 1.0f) / 1.0f);
    const std::uint8_t attackAlpha = static_cast<std::uint8_t>(120.f * attackFade);

    for (int i = left; i <= right; i++) {
        sf::CircleShape zone(GUARD_R + 14.f);
        zone.setOrigin({GUARD_R + 14.f, GUARD_R + 14.f});
        zone.setPosition(m_guardPos[i]);
        zone.setFillColor(sf::Color(255, 60, 40, static_cast<std::uint8_t>(45.f * attackFade)));
        zone.setOutlineColor(sf::Color(255, 60, 40, attackAlpha));
        zone.setOutlineThickness(2.f);
        m_window.draw(zone);
    }

    for (int i = left; i < right; i++) {
        drawThickLine(m_guardPos[i], m_guardPos[i + 1], sf::Color(255, 60, 40, attackAlpha), 9.f);
    }

    for (int i = 0; i < 3; i++) {
        const float offset = static_cast<float>(i - 1) * 26.f;
        sf::Vector2f target = center + tangent * offset;
        sf::Vector2f targetOutward = target - borderCenter;
        float targetLength = std::sqrt(targetOutward.x * targetOutward.x +
                                       targetOutward.y * targetOutward.y);
        if (targetLength < 0.01f) {
            targetOutward = outward;
        }
        else {
            targetOutward /= targetLength;
        }

        sf::Vector2f start = target + targetOutward * (78.f + static_cast<float>(i) * 12.f);

        drawThickLine(start, target, sf::Color(255, 90, 45, static_cast<std::uint8_t>(190.f * attackFade)), 3.f);

        sf::CircleShape marker(5.f);
        marker.setOrigin({5.f, 5.f});
        marker.setPosition(target);
        marker.setFillColor(sf::Color(255, 120, 55, static_cast<std::uint8_t>(255.f * attackFade)));
        marker.setOutlineColor(sf::Color(255, 220, 180, static_cast<std::uint8_t>(220.f * attackFade)));
        marker.setOutlineThickness(1.f);
        m_window.draw(marker);
    }

    for (int i = left; i <= right; i++) {
        sf::Vector2f guard = m_guardPos[i];
        sf::Vector2f target = center + tangent * (static_cast<float>((i - left) % 5) - 2.f) * 9.f;
        sf::Vector2f shotEnd = guard + (target - guard) * volleyProgress;
        std::uint8_t shotAlpha = static_cast<std::uint8_t>(230.f * std::max(0.35f, volleyProgress));

        drawThickLine(guard, shotEnd, sf::Color(255, 210, 70, shotAlpha), 2.2f);

        sf::CircleShape arrowHead(3.5f);
        arrowHead.setOrigin({3.5f, 3.5f});
        arrowHead.setPosition(shotEnd);
        arrowHead.setFillColor(sf::Color(255, 245, 150, shotAlpha));
        m_window.draw(arrowHead);
    }

    if (m_attackTimer > 0.9f) {
        const float radius = 12.f + std::min(1.f, (m_attackTimer - 0.9f) / 0.7f) * 24.f;
        sf::CircleShape burst(radius);
        burst.setOrigin({radius, radius});
        burst.setPosition(center);
        burst.setFillColor(sf::Color(255, 230, 80, 35));
        burst.setOutlineColor(sf::Color(255, 230, 120, 170));
        burst.setOutlineThickness(2.f);
        m_window.draw(burst);
    }

    drawTextAt("ATAK", labelPos.x - 18.f, labelPos.y - 8.f, 14, sf::Color(255, 95, 55));
}

void Visualizer::drawMines() {
    for (size_t i = 0; i < m_mines.size(); i++) {
        bool active = !m_mines[i].getAssignedDwarves().empty();
        sf::Color c = active ? COL_MINE : sf::Color(100, 80, 20);
        drawNode(m_minePos[i], NODE_R, c,
                 "K" + std::to_string(m_mines[i].getId()),
                 m_hasMineTex ? &m_texMine : nullptr);
    }
}

void Visualizer::drawHomes() {
    for (size_t i = 0; i < m_dwarves.size(); i++) {
        drawNode(m_homePos[i], NODE_R * 0.85f, COL_HOME,
                 "D" + std::to_string(m_dwarves[i].getId()),
                 m_hasHomeTex ? &m_texHome : nullptr);
    }
}

void Visualizer::drawGuards() {
    for (size_t i = 0; i < m_guards.size() && i < m_guardPos.size(); i++) {
        sf::Color c = ((int)i == m_loudestIdx) ? COL_LOUDEST : COL_GUARD;
        sf::Vector2f pos = m_guardPos[i];

        //  --- Kółko ---
        if (m_hasGuardTex) {
            sf::Sprite spr(m_texGuard);
            auto ts = m_texGuard.getSize();
            spr.setOrigin({ts.x / 2.f, ts.y / 2.f});
            float sc = (GUARD_R * 2.f) / (float)std::max(ts.x, ts.y);
            spr.setScale({sc, sc});
            spr.setPosition(pos);
            m_window.draw(spr);
        } else {
            sf::CircleShape ci(GUARD_R);
            ci.setOrigin({GUARD_R, GUARD_R});
            ci.setPosition(pos);
            ci.setFillColor(c);
            ci.setOutlineColor(sf::Color(255,255,255,180));
            ci.setOutlineThickness(1.5f);
            m_window.draw(ci);
        }

        //  --- ID strażnika ---
        {
            std::string idStr = "#" + std::to_string(m_guards[i].getId());
            sf::Text idLabel(idStr, m_font, 17u);
            idLabel.setFillColor(sf::Color(220, 220, 255));
            sf::FloatRect lb = idLabel.getLocalBounds();
            idLabel.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height);
            idLabel.setPosition(pos.x, pos.y - GUARD_R - 2.f);
            m_window.draw(idLabel);
        }

        //  --- głośność ---
        {
            std::string loudStr = std::to_string(m_guards[i].getLoudness());
            sf::Text loudLabel(loudStr, m_font, 17u);
            loudLabel.setFillColor(c);
            sf::FloatRect lb = loudLabel.getLocalBounds();
            loudLabel.setOrigin(lb.left + lb.width / 2.f, lb.top);
            loudLabel.setPosition(pos.x, pos.y + GUARD_R + 2.f);
            m_window.draw(loudLabel);
        }
    }
}

void Visualizer::drawDwarfAnims() {
    for (const auto& da : m_dwarfAnims) {
        sf::Color c = da.preferred ? COL_PREF_EDGE : hex(0xff8800);

        if (m_hasDwarfTex) {
            sf::Sprite spr(m_texDwarf);
            auto ts = m_texDwarf.getSize();
            spr.setOrigin({ts.x / 2.f, ts.y / 2.f});
            float sc = (DWARF_R * 2.f) / (float)std::max(ts.x, ts.y);
            spr.setScale({sc, sc});
            spr.setPosition(da.pos);
            m_window.draw(spr);
        } else {
            sf::CircleShape ci(DWARF_R);
            ci.setOrigin({DWARF_R, DWARF_R});
            ci.setPosition(da.pos);
            ci.setFillColor(c);
            ci.setOutlineColor(sf::Color::White);
            ci.setOutlineThickness(2.f);
            m_window.draw(ci);
        }

        sf::Text lbl(std::to_string(da.dwarfId), m_font, 11u);
        lbl.setFillColor(sf::Color::White);
        lbl.setPosition(da.pos.x + DWARF_R + 2.f, da.pos.y - 8.f);
        m_window.draw(lbl);
    }
}

void Visualizer::drawNode(sf::Vector2f pos, float r, sf::Color fill,
                           const std::string& label, sf::Texture* tex)
{
    if (tex) {
        sf::Sprite spr(*tex);
        auto ts = tex->getSize();
        spr.setOrigin({ts.x / 2.f, ts.y / 2.f});
        float sc = (r * 2.f) / (float)std::max(ts.x, ts.y);
        spr.setScale({sc, sc});
        spr.setPosition(pos);
        m_window.draw(spr);

        // Obramowanie kółka na sprite
        sf::CircleShape outline(r);
        outline.setOrigin({r, r});
        outline.setPosition(pos);
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineColor(sf::Color(255, 255, 255, 160));
        outline.setOutlineThickness(2.f);
        m_window.draw(outline);
    } else {
        sf::CircleShape ci(r);
        ci.setOrigin({r, r});
        ci.setPosition(pos);
        ci.setFillColor(fill);
        ci.setOutlineColor(sf::Color(255, 255, 255, 180));
        ci.setOutlineThickness(2.f);
        m_window.draw(ci);
    }

    sf::Text t(sf::String::fromUtf8(label.begin(), label.end()), m_font, 12u);
    t.setFillColor(sf::Color::White);
    sf::FloatRect tb = t.getLocalBounds();
    t.setOrigin(tb.left + tb.width / 2.f, tb.top);
    t.setPosition(pos.x, pos.y + r + 3.f);
    m_window.draw(t);
}

void Visualizer::drawSidePanel() {
    const float visiblePanelHeight = WIN_H - 20.f;
    const float maxScroll = std::max(0.f, calculateSidePanelContentHeight() - visiblePanelHeight);
    m_panelScrollY = std::max(0.f, std::min(m_panelScrollY, maxScroll));

    float x = MAP_W + 12.f;
    float y = 10.f - m_panelScrollY;
    const unsigned lh = 20;

    auto line = [&](const std::string& s, sf::Color c = sf::Color::White) {
        drawTextAt(s, x, y, 14, c);
        y += lh;
    };
    auto sep = [&]() {
        sf::RectangleShape r({PANEL_W - 24.f, 1.f});
        r.setPosition({x - 2.f, y});
        r.setFillColor(sf::Color(80, 80, 120));
        m_window.draw(r);
        y += 6.f;
    };

    switch (m_phase) {
    case Phase::WORK_ASSIGNMENT:
        line("=== PRZYDZIELANIE PRACY ===", COL_MINE);
        sep();
        line("Krasnoludki: " + std::to_string(m_dwarves.size()));
        line("Kopalnie:    " + std::to_string(m_mines.size()));
        sep();
        line("Przydzielono: " +
             std::to_string(m_assignment.assignedCount) + "/" +
             std::to_string(m_dwarves.size()));
        line("Preferowanych: " +
             std::to_string(m_assignment.preferredAssignedCount), COL_PREF_EDGE);
        {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2) << m_assignment.totalDistance;
            line("Laczny dystans: " + oss.str());
        }
        sep();
        line("Legenda:", hex(0xaaaaaa));
        line("  szare: mozliwe krawedzie", hex(0x666688));
        line("  zielony = preferowany", COL_PREF_EDGE);
        line("  szary   = niepreferowany", COL_EDGE);
        sep();
        line("Przydzialy:");
        for (const auto& a : m_assignment.assignments) {
            std::ostringstream oss;
            oss << " D" << a.dwarfId << " -> K" << a.mineId << " (" << a.resourceType << ")";
            line(oss.str(), a.preferredResource ? COL_PREF_EDGE : COL_EDGE);
            std::ostringstream oss2;
            oss2 << "   dist: " << std::fixed << std::setprecision(2) << a.distance;
            line(oss2.str(), hex(0x888888));
        }
        if (!m_assignment.unassignedDwarfIds.empty()) {
            sep();
            line("Nieprzydzieleni:", sf::Color::Red);
            for (int uid : m_assignment.unassignedDwarfIds)
                line("  D" + std::to_string(uid), sf::Color::Red);
        }
        break;

    case Phase::BORDER_PATROL:
        line("=== PATROL GRANICY ===", COL_HULL);
        sep();
        {
            std::ostringstream oss;
            oss << "Dlugosc trasy: " << std::fixed << std::setprecision(2) << m_patrolDist;
            line(oss.str(), COL_HULL);
        }
        line("Wierzcholki otoczki: " + std::to_string(m_hull.size()));
        sep();
        line("Punkty otoczki:", hex(0xaaaaaa));
        for (const auto& p : m_hull) {
            std::ostringstream oss;
            oss << "  (" << std::fixed << std::setprecision(1) << p.x << ", " << p.y << ")";
            line(oss.str());
        }
        break;

    case Phase::GUARD_COMMAND:
        line("=== OBRONA GRANICY ===", COL_GUARD);
        sep();
        line("Straznicy: " + std::to_string(m_guards.size()));
        sep();
        if (m_guardRangeL >= 0) {
            std::string rangeStr = "Wybrano: [" + std::to_string(m_guardRangeL);
            if (m_guardRangeR >= 0) rangeStr += ", " + std::to_string(m_guardRangeR);
            else                    rangeStr += ", ....";
            rangeStr += "]";
            line(rangeStr);
            if (m_guardRangeR >= 0) {
                if (m_attackResolved) {
                    line("Atak odparty", sf::Color(80, 255, 180));
                }
                else {
                    line("Salwa w toku", hex(0xffcc00));
                    line("Atakowany odcinek granicy", sf::Color(255, 120, 55));
                }
            }
            if (m_guardQueryDone && m_loudestIdx >= 0) {
                line("Najglosniejszy:", COL_LOUDEST);
                line("  idx: "    + std::to_string(m_loudestIdx), COL_LOUDEST);
                line("  id:  "    + std::to_string(m_guards[m_loudestIdx].getId()), COL_LOUDEST);
                line("  glosnosc: "+ std::to_string(m_guards[m_loudestIdx].getLoudness()), COL_LOUDEST);
                sep();
                line("Rozkaz:", hex(0xffcc00));
                line("  Strzaly na cieciwy!", hex(0xffcc00));
                line("  Naciagnac cieciwy!", hex(0xffcc00));
                line("  Strzal!", hex(0xffcc00));
            }
        } else {
            line("Kliknij straznika #1", hex(0x888888));
            line("potem straznika #2", hex(0x888888));
        }
        sep();
        line("Wszyscy straznicy:", hex(0xaaaaaa));
        for (size_t i = 0; i < m_guards.size(); i++) {
            std::string s = "  [" + std::to_string(i) + "] #" +
                            std::to_string(m_guards[i].getId()) +
                            " glos=" + std::to_string(m_guards[i].getLoudness());
            sf::Color c = ((int)i == m_loudestIdx) ? COL_LOUDEST : sf::Color::White;
            line(s, c);
        }
        break;
    }
}

void Visualizer::drawButtons() {
    auto highlight = [&](sf::RectangleShape& btn, Phase p) {
        btn.setFillColor(m_phase == p ? hex(0x2a2a6e) : COL_PANEL);
    };
    highlight(m_btnPhase1, Phase::WORK_ASSIGNMENT);
    highlight(m_btnPhase2, Phase::BORDER_PATROL);
    highlight(m_btnPhase3, Phase::GUARD_COMMAND);

    m_window.draw(m_btnPhase1); if (m_lblPhase1) m_window.draw(*m_lblPhase1);
    m_window.draw(m_btnPhase2); if (m_lblPhase2) m_window.draw(*m_lblPhase2);
    m_window.draw(m_btnPhase3); if (m_lblPhase3) m_window.draw(*m_lblPhase3);
}

void Visualizer::drawPopup() {
    if (!m_popup.visible || m_popup.lines.empty()) return;
    float pw = 230.f;
    float ph = 20.f + 18.f * (float)m_popup.lines.size();
    float px = std::min(m_popup.screenPos.x + 10.f, WIN_W - pw - 5.f);
    float py = std::max(m_popup.screenPos.y - ph, 5.f);

    sf::RectangleShape bg({pw, ph});
    bg.setPosition({px, py});
    bg.setFillColor(sf::Color(20, 20, 50, 230));
    bg.setOutlineColor(sf::Color(150, 150, 220));
    bg.setOutlineThickness(1.5f);
    m_window.draw(bg);

    for (size_t i = 0; i < m_popup.lines.size(); i++) {
        sf::Text t(sf::String::fromUtf8(
            m_popup.lines[i].begin(), m_popup.lines[i].end()), m_font, 13u);
        t.setFillColor(sf::Color::White);
        t.setPosition(px + 6.f, py + 6.f + static_cast<float>(i) * 18.f);
        m_window.draw(t);
    }
}

void Visualizer::onClickMap(sf::Vector2f mp) {
    for (size_t i = 0; i < m_minePos.size(); i++) {
        sf::Vector2f d = mp - m_minePos[i];
        if (std::sqrt(d.x*d.x + d.y*d.y) <= NODE_R + 5) {
            const Kopalnia& mine = m_mines[i];
            m_popup = {};
            m_popup.visible = true;
            m_popup.screenPos = mp;
            m_popup.lines.push_back("KOPALNIA #" + std::to_string(mine.getId()));
            m_popup.lines.push_back("Surowiec: " + mine.getResourceType());
            m_popup.lines.push_back("Pojemnosc: " + std::to_string(mine.getCapacity()));
            m_popup.lines.push_back("Przydzieleni: " + std::to_string(mine.getAssignedDwarves().size()));
            std::ostringstream oss;
            oss << "Pos: (" << std::fixed << std::setprecision(1) << mine.getX() << ", " << mine.getY() << ")";
            m_popup.lines.push_back(oss.str());
            return;
        }
    }
    for (size_t i = 0; i < m_homePos.size(); i++) {
        sf::Vector2f d = mp - m_homePos[i];
        if (std::sqrt(d.x*d.x + d.y*d.y) <= NODE_R + 5) {
            const Krasnal& kra = m_dwarves[i];
            m_popup = {};
            m_popup.visible = true;
            m_popup.screenPos = mp;
            m_popup.lines.push_back("KRASNOLUDEK #" + std::to_string(kra.getId()));
            m_popup.lines.push_back("Preferuje: " + kra.getPreferredResource());
            m_popup.lines.push_back("Umiejetnosci:");
            for (const auto& s : kra.getSkills())
                m_popup.lines.push_back("  - " + s);
            return;
        }
    }
    m_popup.visible = false;
}

void Visualizer::onClickGuardRange(sf::Vector2f mp) {
    for (size_t i = 0; i < m_guardPos.size(); i++) {
        sf::Vector2f d = mp - m_guardPos[i];
        if (std::sqrt(d.x*d.x + d.y*d.y) <= GUARD_R + 12) {
            if (m_guardRangeL < 0) {
                m_guardRangeL = (int)i; m_guardRangeR = -1;
                m_loudestIdx = -1; m_guardQueryDone = false;
                m_attackTimer = 0.f; m_attackResolved = false;
            } else if (m_guardRangeR < 0) {
                int l = std::min(m_guardRangeL, (int)i);
                int r = std::max(m_guardRangeL, (int)i);
                m_guardRangeL = l; m_guardRangeR = r;
                GuardCommandResult res = m_guardSolver.findLoudestGuard(l, r);
                if (res.found) { m_loudestIdx = res.index; m_guardQueryDone = true; }
                m_attackTimer = 0.f; m_attackResolved = false;
            } else {
                m_guardRangeL = (int)i; m_guardRangeR = -1;
                m_loudestIdx = -1; m_guardQueryDone = false;
                m_attackTimer = 0.f; m_attackResolved = false;
            }
            return;
        }
    }
}

sf::Text Visualizer::makeText(const std::string& str, unsigned size, sf::Color col) const {
    sf::Text t(sf::String::fromUtf8(str.begin(), str.end()), m_font, size);
    t.setFillColor(col);
    return t;
}

void Visualizer::drawTextAt(const std::string& str, float x, float y,
                             unsigned size, sf::Color col)
{
    auto t = makeText(str, size, col);
    t.setPosition({x, y});
    m_window.draw(t);
}
