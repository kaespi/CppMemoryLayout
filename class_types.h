
#include <stdint.h>

namespace {

using a_type = int32_t;
using b_type = int32_t;
using c_type = int64_t; // <-- to guarantee that a+b+c = multiple of 64 bits

// POD ("Plain Old Data structure"), i.e. a structure consisting just of fields
struct PlainOldData
{
    a_type a{17};
    b_type b{42};
    c_type c{1024};
};

// A very basic class without any polymorphism (i.e. statically linked)
class PlainClass
{
  public:
    PlainClass() = default;
    ~PlainClass() = default;

    a_type a{17};

    b_type const& get_b() const { return b; }
    c_type const& get_c() const { return c; }

  private:
    b_type b{42};

  public:
    c_type c{1024};
};

// A base and child class using the concept of virtual functions
class VirtualBaseClass
{
  public:
    virtual b_type const& get_b() const = 0;
    virtual c_type const& get_c() const = 0;
};

class VirtualChildClass final : public VirtualBaseClass
{
  public:
    VirtualChildClass() = default;
    ~VirtualChildClass() = default;

    virtual b_type const& get_b() const override { return b; }
    virtual c_type const& get_c() const override { return c; }

    a_type a{17};

  private:
    b_type b{42};

  public:
    c_type c{1024};
};

} // namespace
