#ifndef BERTREE_BERTREE_H
#define BERTREE_BERTREE_H

#include <iostream>

#include <berio/io.h>

void tree_scan(ber::octet_input& in, std::ostream& os);
void tree_print(ber::octet_output& out, std::istream& is);

#endif // BERTREE_BERTREE_H
