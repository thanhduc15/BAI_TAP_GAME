#pragma once
#include "father_file.h"

class HP {
private:
    int mautoida;
    int mauhientai;

public:
    HP();
    void dinhchuong(int damage);
    void hoiphuc();
    bool live() const;
    bool die() const;
    int getMauHienTai() const;
    int getMauToiDa() const;
};
