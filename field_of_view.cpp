#include "stdafx.h"

using namespace std;


FieldOfView::FieldOfView(const Table<PSquare>& s) : squares(s), visibility(squares.getWidth(), squares.getHeight()) {
}

bool FieldOfView::canSee(Vec2 from, Vec2 to) {
  if ((from - to).lengthD() > sightRange)
    return false;
  if (!visibility[from])
    visibility[from] = Visibility(squares, from.x, from.y);
  return visibility[from]->checkVisible(to.x - from.x, to.y - from.y);
}
  
void FieldOfView::squareChanged(Vec2 pos) {
  vector<Vec2> updateList;
  if (!visibility[pos])
    visibility[pos] = Visibility(squares, pos.x, pos.y);
  for (Vec2 v : visibility[pos]->getVisibleTiles())
    if (visibility[v] && visibility[v]->checkVisible(pos.x - v.x, pos.y - v.y)) {
      visibility[v] = Nothing();
    }
}

void FieldOfView::Visibility::setVisible(int x, int y) {
  if (!visible[x + sightRange][y + sightRange] && x * x + y * y <= sightRange * sightRange) {
    visible[x + sightRange][y + sightRange] = 1;
    visibleTiles.push_back(Vec2(px + x, py + y));
  }
}

static int totalIter = 0;
static int numSamples = 0;

FieldOfView::Visibility::Visibility(const Table<PSquare>& squares, int x, int y) : px(x), py(y) {
  memset(visible, 0, (2 * sightRange + 1) * (2 * sightRange + 1));
  calculate(2 * sightRange, 2 * sightRange,2 * sightRange, 2,-1,1,1,1,
      [&](int px, int py) { return !squares[x + px][y + py]->canSeeThru(); },
      [&](int px, int py) { setVisible(px ,py); });
  calculate(2 * sightRange, 2 * sightRange,2 * sightRange, 2,-1,1,1,1,
      [&](int px, int py) { return !squares[x + py][y - px]->canSeeThru(); },
      [&](int px, int py) { setVisible(py, -px); });
  calculate(2 * sightRange, 2 * sightRange,2 * sightRange,2,-1,1,1,1,
      [&](int px, int py) { return !squares[x - px][y - py]->canSeeThru(); },
      [&](int px, int py) { setVisible(-px, -py); });
  calculate(2 * sightRange, 2 * sightRange,2 * sightRange,2,-1,1,1,1,
      [&](int px, int py) { return !squares[x - py][y + px]->canSeeThru(); },
      [&](int px, int py) { setVisible(-py, px); });
  setVisible(0, 0);
  ++numSamples;
  totalIter += visibleTiles.size();
  if (numSamples%100 == 0)
    Debug() << numSamples << " iterations " << totalIter / numSamples << " avg";
}

const vector<Vec2>& FieldOfView::Visibility::getVisibleTiles() const {
  return visibleTiles;
}

const vector<Vec2>& FieldOfView::getVisibleTiles(Vec2 from) {
  if (!visibility[from]) {
    visibility[from] = Visibility(squares, from.x, from.y);
  }
  return visibility[from]->getVisibleTiles();
}


void FieldOfView::Visibility::calculate(int left, int right, int up, int h, int x1, int y1, int x2, int y2,
    function<bool (int, int)> isBlocking, function<void (int, int)> setVisible){
  if (y2*x1>=y1*x2) return;
  if (h>up) return;
  int leftx=x1, lefty=y1, rightx=x2, righty=y2;
  int left_v=(int)floor((double)x1/y1*(h)), 
      right_v=(int)ceil((double)x2/y2*(h)),
      left_b=(int)floor((double)x1/y1*(h-1)),
      right_b=(int)ceil((double)x2/y2*(h+1));
  if (left_v % 2)
    ++left_v;
  if (right_v % 2)
    --right_v;
  if(left_b % 2)
    ++left_b;
  if(right_b % 2)
    --right_b;

  if(left_b>=-left && left_b<=right && isBlocking(left_b/2,h/2)){
    leftx=left_b+1;
    lefty=h+(left_b>=0?-1:1);
  }
  if(left_v<-left) left_v=-left;
  if(right_v>right) right_v=right;
  bool prevBlocking = false;
  for (int i=left_v/2;i<=right_v/2;++i){
    setVisible(i, h / 2);
    bool blocking = isBlocking(i, h / 2);
    if(i > left_v / 2 && blocking && !prevBlocking)
      calculate(left, right, up, h + 2, leftx, lefty, i * 2 - 1, h + (i<=0 ? -1:1), isBlocking, setVisible);
    if(blocking){
      leftx=i*2+1;
      lefty=h+(i>=0?-1:1);
    }
    prevBlocking = blocking;
  }
  calculate(left, right, up, h + 2, leftx, lefty, rightx, righty, isBlocking, setVisible);
}

bool FieldOfView::Visibility::checkVisible(int x, int y) const {
  return x >= -sightRange && y >= -sightRange && x <= sightRange && y <= sightRange && 
    visible[sightRange + x][sightRange + y] == 1;
}


