#include "Writer.h"
#include "Reader.h"
#include "GateHit.h"

int main()
{
 Writer w;
 w.init();
 Reader r;
 r.init();
 while(r.read())
 {
  GateHit* p_gh = r.get();
  if (p_gh != nullptr )
   w.write(*p_gh);
 }
 r.close();
 w.close();
 //Writer w;
 //w.test();

}
