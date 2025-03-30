#include "HP.h"

HP::HP() : mautoida(10), mauhientai(10) {}

void HP::dinhchuong(int damage) {
    mauhientai -= damage;
    if (mauhientai < 0) mauhientai = 0;
}

void HP::hoiphuc() {
    mauhientai++;
    if (mauhientai > mautoida) mauhientai = mautoida;
}

bool HP::live() const { return mauhientai > 0; }
bool HP::die() const { return mauhientai == 0; }
int HP::getMauHienTai() const { return mauhientai; }
int HP::getMauToiDa() const { return mautoida; }
