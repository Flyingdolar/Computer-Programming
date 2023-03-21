#include <string.h>

#include "oop.h"

int main() {
    struct PotatoProducer smallten, subarya;
    init_smallten(&smallten);
    init_subarya(&subarya);

    // smallten summon potato and fry potato ball
    struct Potato *p = smallten.produce(&smallten);
    p->print(p);
    struct PotatoBall *pb = p->fry(&p);  // p pointer set to NULL
    pb->print(pb);
    pb->dtor(&pb);  // pb pointer set to NULL

    // subarya produce potato and fry potato ball
    p = subarya.produce(&subarya);
    p->print(p);
    pb = p->fry(&p);  // p pointer set to NULL
    pb->print(pb);
    strncpy(subarya.name, "Handsome Subarya", 31);
    subarya.name[31] = 0;
    pb->print(pb);
    pb->dtor(&pb);  // pb pointer set to NULL
    return 0;
}
