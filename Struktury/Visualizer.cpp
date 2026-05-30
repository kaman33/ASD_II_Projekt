#include "Visualizer.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>

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
      m_window(sf::VideoMode({(unsigned)WIN_W, (unsigned)WIN_H}),
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
        if (m_font.openFromFile(fp)) break;
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

        lbl.emplace(m_font, sf::String::fromUtf8(text.begin(), text.end()), 14u);
        lbl->setFillColor(sf::Color::White);

        sf::FloatRect bounds = lbl->getLocalBounds();
        lbl->setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        });
        lbl->setPosition({
            x + btnW / 2.f,
            btnY + btnH / 2.f
        });
    };

    makeBtn(m_btnPhase1, m_lblPhase1, 5.f,                "1. Przydzielanie pracy");
    makeBtn(m_btnPhase2, m_lblPhase2, MAP_W / 3.f + 5.f,  "2. Patrol granicy");
    makeBtn(m_btnPhase3, m_lblPhase3, 2.f * MAP_W / 3.f + 5.f, "3. Obrona granicy");
}

void Visualizer::run() {
    m_clock.restart();
    while (m_window.isOpen()) {
        while (const std::optional<sf::Event> e = m_window.pollEvent()) {
            if (e->is<sf::Event::Closed>()) m_window.close();
            handleEvents(*e);
        }
        float dt = m_clock.restart().asSeconds();
        update(dt);
        m_window.clear(COL_BG);
        render();
        m_window.display();
    }
}

void Visualizer::handleEvents(const sf::Event& e) {
    if (const auto* mb = e.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left) {
            sf::Vector2f mp((float)mb->position.x, (float)mb->position.y);
            if (m_btnPhase1.getGlobalBounds().contains(mp)) {
                m_phase = Phase::WORK_ASSIGNMENT; buildDwarfAnims(); m_animDone = false; m_popup.visible = false; return;
            }
            if (m_btnPhase2.getGlobalBounds().contains(mp)) {
                m_phase = Phase::BORDER_PATROL; m_popup.visible = false; return;
            }
            if (m_btnPhase3.getGlobalBounds().contains(mp)) {
                m_phase = Phase::GUARD_COMMAND; m_guardRangeL = m_guardRangeR = -1; m_loudestIdx = -1; m_guardQueryDone = false; m_popup.visible = false; return;
            }
            if (mp.x < MAP_W && mp.y < MAP_H) {
                if (m_phase == Phase::GUARD_COMMAND) onClickGuardRange(mp);
                else onClickMap(mp);
            }
        }
    }
    if (const auto* kp = e.getIf<sf::Event::KeyPressed>()) {
        if (kp->code == sf::Keyboard::Key::Escape) m_popup.visible = false;
        if (kp->code == sf::Keyboard::Key::Space && m_phase == Phase::WORK_ASSIGNMENT) {
            buildDwarfAnims(); m_animDone = false;
        }
    }
}

void Visualizer::update(float dt) {
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
        sf::Text label(m_font, oss.str(), 10u);
        label.setFillColor(COL_GRID_LABEL);
        sf::FloatRect lb = label.getLocalBounds();
        label.setOrigin({lb.size.x / 2.f, 0.f});
        sf::Vector2f labelPos = worldToScreen(wx, m_minY);
        label.setPosition({labelPos.x, MAP_H - MARGIN + 4.f});
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
        sf::Text label(m_font, oss.str(), 10u);
        label.setFillColor(COL_GRID_LABEL);
        sf::FloatRect lb = label.getLocalBounds();
        label.setOrigin({lb.size.x, lb.size.y / 2.f});
        sf::Vector2f labelPos = worldToScreen(m_minX, wy);
        label.setPosition({MARGIN - 4.f, labelPos.y});
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
    drawGuards();

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

    drawTextAt("KLIKNIJ 2 straznikow aby wybrac zakres [L..R]",
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
    rect.setRotation(sf::degrees(std::atan2(diff.y, diff.x) * 180.f / 3.14159f));
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
            sf::Text idLabel(m_font, idStr, 17u);
            idLabel.setFillColor(sf::Color(220, 220, 255));
            sf::FloatRect lb = idLabel.getLocalBounds();
            idLabel.setOrigin({lb.size.x / 2.f, lb.size.y});
            idLabel.setPosition({pos.x, pos.y - GUARD_R - 2.f});
            m_window.draw(idLabel);
        }

        //  --- głośność ---
        {
            std::string loudStr = std::to_string(m_guards[i].getLoudness());
            sf::Text loudLabel(m_font, loudStr, 17u);
            loudLabel.setFillColor(c);
            sf::FloatRect lb = loudLabel.getLocalBounds();
            loudLabel.setOrigin({lb.size.x / 2.f, 0.f});
            loudLabel.setPosition({pos.x, pos.y + GUARD_R + 2.f});
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

        sf::Text lbl(m_font, std::to_string(da.dwarfId), 11u);
        lbl.setFillColor(sf::Color::White);
        lbl.setPosition({da.pos.x + DWARF_R + 2.f, da.pos.y - 8.f});
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

    sf::Text t(m_font, sf::String::fromUtf8(label.begin(), label.end()), 12u);
    t.setFillColor(sf::Color::White);
    sf::FloatRect tb = t.getLocalBounds();
    t.setOrigin({tb.position.x + tb.size.x / 2.f, 0.f});
    t.setPosition({pos.x, pos.y + r + 3.f});
    m_window.draw(t);
}

void Visualizer::drawSidePanel() {
    float x = MAP_W + 12.f;
    float y = 10.f;
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
            std::string rangeStr = "Zakres: [" + std::to_string(m_guardRangeL);
            if (m_guardRangeR >= 0) rangeStr += " .. " + std::to_string(m_guardRangeR);
            else                    rangeStr += " .. ?]";
            rangeStr += "]";
            line(rangeStr);
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
        sf::Text t(m_font, sf::String::fromUtf8(
            m_popup.lines[i].begin(), m_popup.lines[i].end()), 13u);
        t.setFillColor(sf::Color::White);
        t.setPosition({px + 6.f, py + 6.f + (float)i * 18.f});
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
            } else if (m_guardRangeR < 0) {
                int l = std::min(m_guardRangeL, (int)i);
                int r = std::max(m_guardRangeL, (int)i);
                m_guardRangeL = l; m_guardRangeR = r;
                GuardCommandResult res = m_guardSolver.findLoudestGuard(l, r);
                if (res.found) { m_loudestIdx = res.index; m_guardQueryDone = true; }
            } else {
                m_guardRangeL = (int)i; m_guardRangeR = -1;
                m_loudestIdx = -1; m_guardQueryDone = false;
            }
            return;
        }
    }
}

sf::Text Visualizer::makeText(const std::string& str, unsigned size, sf::Color col) const {
    sf::Text t(m_font, sf::String::fromUtf8(str.begin(), str.end()), size);
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