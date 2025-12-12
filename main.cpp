#include <iostream>
#include <cstddef>

namespace top
{
  struct p_t
  {
    int x, y;
  };
  struct IDraw
  {
    virtual p_t begin() const = 0;
    virtual p_t next(p_t) const = 0;
    virtual ~IDraw() = default;
  };
  bool operator== (p_t a, p_t b)
  {
    return a.x == b.x && a.y == b.y;
  }
  bool operator!= (p_t a, p_t b)
  {
    return !(a == b);
  }
  struct Dot: IDraw
  {
    p_t begin() const override;
    p_t next(p_t) const override;
    p_t o;
    Dot(int x, int y);
    explicit Dot(p_t dd);
  };
  struct VerticalLine: IDraw
  {
    VerticalLine(int x, int y0, int y1);
    p_t begin() const override;
    p_t next(p_t) const override;
    int x, y0, y1;
  };
  struct HorizontalLine: IDraw{
    HorizontalLine(int y, int x0, int x1);
    p_t begin() const override;
    p_t next(p_t) const override;
    int y, x0, x1;
  };
  struct frame_t
  {
    p_t aa;
    p_t bb;
  };
  size_t get_points(IDraw* b, p_t** ps, size_t& s);
  frame_t build_frame(const p_t* ps, size_t s);
  void extend(p_t** pts, size_t s, p_t p);
  size_t points(const IDraw& d, p_t** pts, size_t s);
  char* canvas(frame_t fr, char fill);
  void flush(std::ostream& os, const char* cnv, top::frame_t fr);
  void paint(char* cnv, top::frame_t fr, p_t p, char fill);
  frame_t frame(const top::p_t* pts, size_t s);
  size_t rows(top::frame_t fr);
  size_t cols(top::frame_t fr);
}
top::Dot::Dot(int x, int y):
  o{x, y}
{}
top::p_t top::Dot::begin() const{
  return o;
}
top::p_t top::Dot::next(p_t p) const{
  return p;
}
top::VerticalLine::VerticalLine(int x, int yStart, int yEnd):
  x{x},
  y0{yStart},
  y1{yEnd}
{}
top::p_t top::VerticalLine::begin() const
{
  return top::p_t{x, y0};
}
top::p_t top::VerticalLine::next(top::p_t curr) const
{
  if (curr.y == y1)
  {
    return curr;
  }
  return top::p_t{x, curr.y + 1};
}
top::HorizontalLine::HorizontalLine(int y, int xStart, int xEnd):
  y{y},
  x0{xStart},
  x1{xEnd}
{}
top::p_t top::HorizontalLine::begin() const
{
  return top::p_t{x0, y};
}
top::p_t top::HorizontalLine::next(p_t curr) const
{
  if (curr.x == x1)
  {
    return curr;
  }
  return top::p_t{curr.x + 1, y};
}
size_t top::rows(top::frame_t fr)
{
  return (fr.bb.y - fr.aa.y + 1); 
}
size_t top::cols(top::frame_t fr)
{
  return (fr.bb.x - fr.aa.x + 1); 
}
top::frame_t top::frame(const top::p_t* pts, size_t s)
{
  if (!s)
  {
    throw std::logic_error("bad size");
  }
  int minx = pts[0].x, maxx = minx;
  int miny = pts[0].y, maxy = miny;
  for(size_t i = 1; i < s; ++i)
  {
    minx = std::min(minx, pts[i].x);
    maxx = std::max(maxx, pts[i].x);
    miny = std::min(miny, pts[i].y);
    maxy = std::max(maxy, pts[i].y);
  }
  top::p_t aa{minx, miny};
  top::p_t bb{maxx, maxy};
  return top::frame_t{aa, bb};
}
void top::extend(top::p_t** pts, size_t s, top::p_t p)
{
  top::p_t* res = new top::p_t[s + 1];
  for(size_t i = 0; i < s; ++i)
  {
    res[i] = (*pts)[i];
  }
  res[s] = p;
  delete [] *pts;
  *pts = res;
}
size_t top::points(const top::IDraw& d, top::p_t** pts, size_t s)
{
  top::p_t p = d.begin();
  extend(pts, s, p);
  size_t delta = 1;
  while(d.next(p) != d.begin())
  {
    p = d.next(p);
    extend(pts, s + delta, p);
    ++delta;
  }
  return delta;
}
size_t top::get_points(top::IDraw* figure, top::p_t** p, size_t& s)
{
  top::p_t st = figure->begin();
  size_t count = 0;
  extend(p, s, st);
  ++s;
  ++count;
  top::p_t curr = st;
  top::p_t next_pt = figure->next(curr);
  while (next_pt != curr && !(next_pt == st))
  {
    extend(p, s, next_pt);
    ++count;
    ++s;
    curr = next_pt;
    next_pt = figure->next(curr);
    if (next_pt == curr){
      break;
    }
  }
  return count;
}
char* top::canvas(top::frame_t fr, char fill)
{
  char* cnv = new char[top::rows(fr) * top::cols(fr)];
  for(size_t i = 0; i < top::rows(fr) * top::cols(fr); ++i)
  {
    cnv[i] = fill;
  }
  return cnv;
}
void top::paint(char* cnv, top::frame_t fr, top::p_t p, char fill)
{
  int dx = p.x - fr.aa.x;
  int dy = p.y - fr.aa.y;
  cnv[dy * top::cols(fr) + dx] = fill;
}
void top::flush(std::ostream& os, const char* cnv, top::frame_t fr)
{
  for(size_t i = 0; i < top::rows(fr); ++i)
  {
    for (size_t j = 0; j < top::cols(fr); ++j)
    {
      os << cnv[i * top::cols(fr) + j];
    }
    os << "\n";
  }
}
int main(){
  using namespace top;
  int err = 0;
  IDraw* f[3] = {};
  p_t* p = nullptr;
  size_t s = 0;
  char* cnv = nullptr;
  try{
    f[0] = new Dot(2, 3);
    f[1] = new VerticalLine(5, 1, 7);
    f[2] = new HorizontalLine(8, 1, 7);
    for(size_t i = 0; i < 3; ++i)
    {
      get_points(f[i], &p, s);
    }
    frame_t fr = frame(p, s);
    cnv = canvas(fr, '.');
    for (size_t i = 0; i < s; ++i)
    {
      paint(cnv, fr, p[i], '#');
    }
    flush(std::cout, cnv, fr);
  }
  catch(...){
    err = 1;
  }
  delete f[0];
  delete f[1];
  delete f[2];
  delete [] cnv;
  return err;
}
