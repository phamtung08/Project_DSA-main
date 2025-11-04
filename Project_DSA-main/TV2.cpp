#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;
// ====================== CẤU TRÚC DỮ LIỆU ======================
struct KhachHang {
    int soThuTu;
    string cmnd;
    string hoTen;
    KhachHang* next;
    KhachHang() : soThuTu(0), next(NULL) {}
};

struct Ve {
    string maVe;
    string maChuyenBay;
    string cmnd;
    string hoTen;
    int soGhe;
    Ve* next;
    Ve() : soGhe(0), next(NULL) {}
};

struct NodeGhe {
    int soGhe;
    NodeGhe* next;
    NodeGhe() : soGhe(0), next(NULL) {}
};

struct ChuyenBay {
    string maChuyenBay;
    int trangThai; 
    NodeGhe* danhSachGheTrong;
    Ve* danhSachVe;
    ChuyenBay() : trangThai(1), danhSachGheTrong(NULL), danhSachVe(NULL) {}
};

// ====================== CÁC BIẾN TOÀN CỤC ======================
ChuyenBay* cayChuyenBay = NULL;
KhachHang* danhSachKhachHang = NULL;

// ====================== HÀM HỖ TRỢ ======================
void clearScreen() {
    system("cls");
}

void themGhe(NodeGhe*& head, int soGhe) {
    NodeGhe* ghe = new NodeGhe();
    ghe->soGhe = soGhe;
    ghe->next = head;
    head = ghe;
}

void xoaGhe(NodeGhe*& head, int soGhe) {
    if (!head) return;
    if (head->soGhe == soGhe) {
        NodeGhe* tmp = head;
        head = head->next;
        delete tmp;
        return;
    }
    NodeGhe* p = head;
    while (p->next && p->next->soGhe != soGhe) p = p->next;
    if (p->next) {
        NodeGhe* tmp = p->next;
        p->next = p->next->next;
        delete tmp;
    }
}

int demGheTrong(NodeGhe* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

void themVe(Ve*& head, Ve* ve) {
    ve->next = head;
    head = ve;
}

void xoaVe(Ve*& head, string maVe) {
    if (!head) return;
    if (head->maVe == maVe) {
        Ve* tmp = head;
        head = head->next;
        delete tmp;
        return;
    }
    Ve* p = head;
    while (p->next && p->next->maVe != maVe) p = p->next;
    if (p->next) {
        Ve* tmp = p->next;
        p->next = p->next->next;
        delete tmp;
    }
}

// ====================== HÀM GHI FILE ======================
void ghiFileVe(Ve* ve) {
    if (!ve) return;
    ofstream file(ve->maVe + ".txt");
    file << "Ma ve: " << ve->maVe << endl;
    file << "Ma chuyen bay: " << ve->maChuyenBay << endl;
    file << "CMND: " << ve->cmnd << endl;
    file << "Ho ten: " << ve->hoTen << endl;
    file << "So ghe: " << ve->soGhe << endl;
    file.close();
}

void xoaFileVe(string maVe) {
    remove((maVe + ".txt").c_str());
}

void ghiFileChuyenBay() {
    ofstream f("chuyenbay.txt", ios::app);
    f << "Cap nhat thong tin chuyen bay.\n";
    f.close();
}

void ghiFileKhachHang() {
    ofstream f("khachhang.txt", ios::app);
    f << "Cap nhat danh sach khach hang.\n";
    f.close();
}

// ====================== TÌM KIẾM GIẢ LẬP ======================
ChuyenBay* timChuyenBay(ChuyenBay* root, string maCB) {
    if (root && root->maChuyenBay == maCB) return root;
    return cayChuyenBay;
}

void timVe(ChuyenBay* root, string maVe, ChuyenBay*& cb, Ve*& ve) {
    if (!root) return;
    Ve* p = root->danhSachVe;
    while (p) {
        if (p->maVe == maVe) {
            cb = root;
            ve = p;
            return;
        }
        p = p->next;
    }
}

// ====================== CẬP NHẬT TRẠNG THÁI ======================
void capNhatTrangThaiChuyenBay(ChuyenBay* cb) {
    if (!cb) return;
    int soGheTrong = demGheTrong(cb->danhSachGheTrong);
    cb->trangThai = (soGheTrong == 0) ? 2 : 1;
}

// ====================== QUẢN LÝ KHÁCH HÀNG ======================
void themKhachHang(KhachHang*& head, string cmnd, string hoTen) {
    KhachHang* kh = new KhachHang();
    int maxSTT = 0;
    for (KhachHang* p = head; p; p = p->next)
        if (p->soThuTu > maxSTT) maxSTT = p->soThuTu;
    kh->soThuTu = maxSTT + 1;
    kh->cmnd = cmnd;
    kh->hoTen = hoTen;
    kh->next = head;
    head = kh;
}

void xoaKhachHang(KhachHang*& head, int soThuTu) {
    if (!head) return;
    if (head->soThuTu == soThuTu) {
        KhachHang* tmp = head;
        head = head->next;
        delete tmp;
        return;
    }
    KhachHang* p = head;
    while (p->next && p->next->soThuTu != soThuTu) p = p->next;
    if (p->next) {
        KhachHang* tmp = p->next;
        p->next = p->next->next;
        delete tmp;
    }
}

// ====================== ĐẶT VÉ ======================
void datVe() {
    clearScreen();
    cout << "\n========== DAT VE ==========\n";
    string maChuyenBay, cmnd, hoTen;
    int soGhe;

    cout << "Nhap ma chuyen bay: ";
    getline(cin, maChuyenBay);

    ChuyenBay* cb = timChuyenBay(cayChuyenBay, maChuyenBay);
    if (!cb) { cout << "Khong tim thay chuyen bay!\n"; return; }

    if (cb->trangThai != 1) {
        cout << "Chuyen bay khong con ve!\n";
        return;
    }

    cout << "Nhap CMND: "; getline(cin, cmnd);
    cout << "Nhap ho ten: "; getline(cin, hoTen);
    cout << "Nhap so ghe: "; cin >> soGhe; cin.ignore();

    themKhachHang(danhSachKhachHang, cmnd, hoTen);
    ghiFileKhachHang();
    cout << "\nKhach hang da vao danh sach cho.\n";
}

// ====================== XỬ LÝ ĐẶT VÉ ======================
void xuLyDatVe() {
    clearScreen();
    cout << "\n========== XU LY DAT VE ==========\n";
    if (!danhSachKhachHang) { cout << "Khong co khach hang nao!\n"; return; }

    int soThuTu;
    cout << "Nhap STT khach hang can xu ly: ";
    cin >> soThuTu; cin.ignore();

    string maChuyenBay; int soGhe;
    cout << "Nhap ma chuyen bay: ";
    getline(cin, maChuyenBay);
    cout << "Nhap so ghe: ";
    cin >> soGhe; cin.ignore();

    ChuyenBay* cb = timChuyenBay(cayChuyenBay, maChuyenBay);
    if (!cb) { cout << "Khong tim thay chuyen bay!\n"; return; }

    Ve* veNew = new Ve();
    veNew->maVe = maChuyenBay + "_" + to_string(soGhe);
    veNew->maChuyenBay = maChuyenBay;
    veNew->cmnd = "123456789";
    veNew->hoTen = "Test";
    veNew->soGhe = soGhe;

    themVe(cb->danhSachVe, veNew);
    xoaGhe(cb->danhSachGheTrong, soGhe);
    capNhatTrangThaiChuyenBay(cb);
    ghiFileVe(veNew);
    ghiFileChuyenBay();
    cout << "\nXu ly dat ve thanh cong!\n";
}

// ====================== TRẢ VÉ ======================
void xuLyTraVe() {
    clearScreen();
    cout << "\n========== TRA VE ==========\n";
    string maVe;
    cout << "Nhap ma ve: ";
    getline(cin, maVe);

    ChuyenBay* cb = NULL; Ve* ve = NULL;
    timVe(cayChuyenBay, maVe, cb, ve);

    if (ve == NULL || cb == NULL) {
    cout << "Khong tim thay ve!\n";
    return;
    }
    if (ve->maVe != maVe) { 
        cout << "Ma ve khong ton tai!\n"; 
        return;
    }

    int soGhe = ve->soGhe;
    xoaVe(cb->danhSachVe, maVe);
    themGhe(cb->danhSachGheTrong, soGhe);
    capNhatTrangThaiChuyenBay(cb);
    xoaFileVe(maVe);
    ghiFileChuyenBay();
    cout << "\nTra ve thanh cong!\n";
}

// ====================== MAIN DEMO ======================
int main() {
    cayChuyenBay = new ChuyenBay();
    cayChuyenBay->maChuyenBay = "VN001";
    for (int i = 1; i <= 3; i++) themGhe(cayChuyenBay->danhSachGheTrong, i);

    while (true) {
        clearScreen();
        cout << "\n===== MENU TEST TV2 =====\n";
        cout << "1. Dat ve\n";
        cout << "2. Xu ly dat ve\n";
        cout << "3. Tra ve\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";
        int ch; cin >> ch; cin.ignore();
        if (ch == 0) break;
        if (ch == 1) datVe();
        else if (ch == 2) xuLyDatVe();
        else if (ch == 3) xuLyTraVe();
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }
    return 0;
}
