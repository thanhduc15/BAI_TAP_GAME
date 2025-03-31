#pragma once

class HP {
private:
    int mautoida;
    int mauhientai;

public:
    HP();
    void dinhchuong(int damage);
    void hoiphuc();
    void resetToFull(); // Thêm hàm mới để hồi đầy máu
    bool live() const;
    bool die() const;
    int getMauHienTai() const;
    int getMauToiDa() const;
};
