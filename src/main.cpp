#include <cstdlib>
#include <iostream> //Added for cout
#include "SharedPtr.h"

int main() {
   std::cout << "Test 1a, default construct, bool, get, equal, copy assign. Goal: 0 5 false 5 true\n";
   SharedPtr<int> a;
   if (a) {
      std::cout << *a.get() << " ";
   } else {
      std::cout << 0 << " ";
   }
   SharedPtr<int> b(new int(5));
   std::cout << *b.get() << " ";
   std::cout << (a == b?"true":"false") << " ";
   a = b;
   std::cout << *a.get() << " ";
   std::cout << (a == b?"true":"false") << "\n";

   std::cout << "Test 1b, move construct, get, equal. Goal: 5 true\n";
   SharedPtr<int> c;
   c = std::move(b);
   std::cout << *c.get() << " ";
   std::cout << (c == b?"true":"false") << "\n";

   std::cout << "Test 1c, copy construct, get, equal, useCount. Goal: 5 true 3\n";
   SharedPtr<int> d(a);
   std::cout << *d.get() << " ";
   std::cout << (d == a?"true":"false") << " ";
   std::cout << d.useCount() << "\n";

   std::cout << "Test 1d, reset, get, equal, useCount. Goal: 0 false 2\n";
   d.reset();
   if (d) {
      std::cout << *d.get() << " ";
   } else {
      std::cout << 0 << " ";
   }
   std::cout << (d == a?"true":"false") << " ";
   std::cout << d.useCount() << "\n";

   std::cout << "Test 1e, reset with new pointer, get, equal, useCount. Goal: 2 false 1\n";
   d.reset(new int(2));
   std::cout << *d.get() << " ";
   std::cout << (d == a?"true":"false") << " ";
   std::cout << d.useCount() << "\n";

   std::cout << "Test 1f, selfreset, get, useCount. Goal: 2 1\n";
   d.reset(d.get());
   std::cout << *d.get() << " ";
   std::cout << d.useCount() << "\n";

   std::cout << "Test 1g, swap, get, equal, useCount. Goal: 5 2 false 2 1\n";
   d.swap(a);
   std::cout << *d.get() << " ";
   std::cout << *a.get() << " ";
   std::cout << (d == a?"true":"false") << " ";
   std::cout << d.useCount() << " ";
   std::cout << a.useCount() << "\n";

   std::cout << "Test 1h, scope test, copy, useCount. Goal: 2 2 1\n";
   {
      SharedPtr<int> e(a);
      std::cout << *e.get() << " ";
      std::cout << a.useCount() << " ";
   }
   std::cout << a.useCount() << "\n";

   std::cout << "Final test, makeSharedBasic, get Goal: 5\n";
   SharedPtr<int> five = makeSharedBasic<int>(5);
   std::cout << *five.get() << "\n";

   return EXIT_SUCCESS;
}
