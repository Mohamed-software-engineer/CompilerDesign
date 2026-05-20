#ifndef PROGRAM_H
#define PROGRAM_H

#pragma once

#include "../stmt.h"
#include <memory>
#include <vector>

class Program : public ASTNode {
public:
  std::vector<std::unique_ptr<Stmt>> statements;
};
#endif
