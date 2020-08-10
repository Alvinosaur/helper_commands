
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

/**
/* for returning one object (ie: struct, vector, etc..) it seems like RVO is used.
/* when returning a tuple and calling make_tuple, it makes sense that a copy is needed
/* so should use std::move
 */

struct A {   // Note: All methods have side effects
  A() { cout << "c'tor" << endl; }
  ~A() { cout << "d'tor" << endl; }

  A(const A&) { cout << "copy c'tor" << endl; }
  A(A&&) { cout << "move c'tor" << endl; }

  A& operator=(const A&) {
    cout << "copy assignment" << endl;
    return *this;
  }

  A& operator=(A&&) {
    cout << "move assignment" << endl;
    return *this;
  }
};

std::tuple<std::vector<A>, std::vector<A>> func() {
    std::vector<A> vec(2, A());
    std::vector<A> vec2(2, A());
    return std::make_tuple(std::move(vec), std::move(vec2));
    // return std::make_tuple(vec, vec2);  // this incurs extra copy's and destructions
}

std::vector<A> func2() {
    std::vector<A> vec(2, A());
    return vec;  // no difference from std::move(vec), RVO used
}

void func3(std::vector<A> vec) {
    (void)vec;
}

int main()
{
    cout<<"Hello World";
    // std::vector<A> vec1, vec2;
    // std::tie(vec1, vec2) = func();

    // vec1 = func2();

    std::vector<A> vec1(2, A());
    func3(std::move(vec1));
    
    return 0;
}
