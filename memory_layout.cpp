
#include "address_helper.h"
#include "class_types.h"
#include "type_helper.h"

#include <iostream>
#include <string_view>
#include <typeinfo>
#include <utility>

namespace {

template <typename T>
void print_object_memory_info(T const* obj, std::string_view const& title)
{
    std::cout << title << ": (instance size=" << sizeof(T) << "bytes)" << std::endl;
    addr_type const obj_addr = get_addr(obj);
    std::cout << " &obj=0x" << std::hex << obj_addr << std::endl;
    if constexpr (has_a_member<T>{})
    {
        addr_type a_addr = get_addr(&obj->a);
        std::cout << "  &obj.a=0x" << std::hex << a_addr << std::dec << " (+" << a_addr - obj_addr << "bytes)"
                  << std::endl;
    }
    else
    {
        std::cout << "  &obj.a=? (private...)" << std::endl;
    }
    addr_type b_addr;
    addr_type c_addr;
    if constexpr (has_b_member<T>{})
    {
        b_addr = get_addr(&obj->b);
    }
    else
    {
        b_addr = get_addr(&obj->get_b());
    }
    if constexpr (has_c_member<T>{})
    {
        c_addr = get_addr(&obj->c);
    }
    else
    {
        c_addr = get_addr(&obj->get_c());
    }
    std::cout << "  &obj.b=0x" << std::hex << b_addr << std::dec << " (+" << b_addr - obj_addr << "bytes)"
              << std::endl;
    std::cout << "  &obj.c=0x" << std::hex << c_addr << std::dec << " (+" << c_addr - obj_addr << "bytes)"
              << std::endl
              << std::endl;
}

// Prints some magic around the vptr and the vtable based on the example of
// the VirtualBaseClass and its derivative VirtualChildClass
void print_vtable_info(VirtualBaseClass* p_base, VirtualChildClass* p_child)
{
    // obtain the addresses of
    //  * VirtualChildClass::get_b() function
    //  * VirtualChildClass::get_c() function
    //  * type_info(p_base) (actually type_info(VirtualChildClass), because p_base==p_child)
    auto pf_get_b = &VirtualChildClass::get_b;
    auto pf_get_c = &VirtualChildClass::get_c;
    addr_type get_b_addr = get_addr((void*)(p_child->*pf_get_b));
    addr_type get_c_addr = get_addr((void*)(p_child->*pf_get_c));
    std::cout << "&VirtualChildClass::get_b()=0x" << std::hex << get_b_addr << std::dec << std::endl;
    std::cout << "&VirtualChildClass::get_c()=0x" << std::hex << get_c_addr << std::dec << std::endl;

    const std::type_info& ti_p_base = typeid(*p_base);
    addr_type p_base_typeinfo_addr = get_addr(&ti_p_base);
    std::cout << "&type_info=0x" << std::hex << p_base_typeinfo_addr << std::dec
              << "     (name=" << ti_p_base.name() << "): " << std::endl
              << std::endl;

    // confirm the suspicion that the first entry in memory for the object
    // is the vptr pointing to the vtable
    addr_type*** vptr = reinterpret_cast<addr_type***>(p_base); // vptr=pointer to the vtable
    addr_type** vtable = reinterpret_cast<addr_type**>(*vptr);
    std::cout << "VirtualChildClass &obj=vptr --> vtable:" << std::endl;
    for (auto n = -2; n < 2; ++n)
    {
        try
        {
            addr_type vtable_target = get_addr(vtable[n]);
            std::cout << "  vtable[" << n << "]=0x" << std::hex << vtable_target << std::dec;
            if (vtable_target == p_base_typeinfo_addr)
                std::cout << "   (=typeinfo(VirtualChildClass))";
            else if (vtable_target == get_b_addr)
                std::cout << "   (=VirtualChildClass::get_b())";
            else if (vtable_target == get_c_addr)
                std::cout << "   (=VirtualChildClass::get_c())";
            std::cout << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}
} // namespace

int main()
{
    std::cout << "Address width=" << sizeof(addr_type) << "bytes (=" << (sizeof(addr_type) * 8) << "bits)"
              << std::endl
              << std::endl;

    // print the memory addresses of instances and the corresponding members
    // (also relative to the object's address)
    PlainOldData pod;
    print_object_memory_info(&pod, "PlainOldData");

    PlainClass pc;
    print_object_memory_info(&pc, "PlainClass");

    VirtualChildClass vcc;
    print_object_memory_info(&vcc, "VirtualChildClass");

    VirtualChildClass* pvcc = new VirtualChildClass();
    VirtualBaseClass* pvbc = dynamic_cast<VirtualBaseClass*>(pvcc);
    print_object_memory_info(pvbc, "(VirtualBaseClass*)&VirtualChildClass");

    print_vtable_info(pvbc, pvcc);

    return EXIT_SUCCESS;
}
