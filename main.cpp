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
}