#include <iostream>

namespace top{
  struct p_t{
    int x, y;
  };
  struct IDraw{
    virtual p_t begin() const = 0;
    virtual p_t next(p_t) const = 0;
  };
  bool operator==(p_t a, p_t b){
    return a.x == b.x && a.y==b.y;
  }
  bool operator!=(p_t a, p_t b)
  {
    return !(a == b);
  }
  struct Dot:IDraw{
    p_t begin() const override;
    p_t next(p_t) const override;
    p_t o;
    Dot(int x, int y);
  };
  struct frame_t{
    p_t left_bot;
    p_t right_top;
  };
  void make_f(IDraw** b, size_t k);
  void get_points(IDraw* b, p_t** ps, size_t& s);
  frame_t build_frame(const p_t* ps, size_t s);
  char* build_canvas(frame_t f);
  void paint_canvas(char* cnv, frame_t fr, const p_t* ps, size_t k, char f);
  void print_canvas(const char* cnv, frame_t fr);
}

top::Dot::Dot(int x, int y):
  IDraw(),
  o{x, y}
{}
top::p_t top::Dot::begin() const{
  return o;

}
top::p_t top::Dot::next(p_t) const{
  return begin();
}
int main(){
  using namespace top;
  int err = 0;
  IDraw* f[3] = {};
  p_t* p = nullptr;
  size_t s = 0;
  char* cnv = nullptr;
  try{
    make_f(f, 3);
    for(size_t i = 0; i < 3; ++i)
    {
      get_points(f[i], &p, s);
    }
    frame_t fr = build_frame(p, s);
    cnv = build_canvas(fr);
    paint_canvas(cnv, fr, p, s, '#');
    print_canvas(cnv, fr);
  }
  catch(...){
    err = 1;
  }
  delete f[0];
  delete f[1];
  delete f[2];
  delete [] p;
  delete [] cnv;
  return err;
}