#ifndef BERTREE_BERTREE_H
#define BERTREE_BERTREE_H

#include <iostream>

#include <berio/io.h>

void tree_scan(ber::octet_input&, std::ostream&);
void tree_print(ber::octet_output&, std::istream&);

#endif // BERTREE_BERTREE_H
